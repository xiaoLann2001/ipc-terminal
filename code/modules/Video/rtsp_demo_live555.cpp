// rtsp_demo_live555.cpp
// 基于 live555 的 RTSP 服务器实现，按 rtsp_demo.h C 接口提供能力。
// 支持：H264 视频（SPS/PPS 可设置） + G711A 音频（采样率/通道可设置）。
// 设计：PushSource + OnDemandServerMediaSubsession（reuseFirstSource=true）。

#include "rtsp_demo.h"

#include <liveMedia.hh>
#include <StreamReplicator.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>
#include <memory>
#include <cstring>
#include <chrono>
#include <algorithm>
#include <cstdio>
#include <sys/time.h>
#include <atomic>
#include <functional>

using namespace std;

// ========================= Debug logging =========================
#ifndef RTSP_LIVE555_DEBUG
#define RTSP_LIVE555_DEBUG 0
#endif

#if RTSP_LIVE555_DEBUG
#define RTSP_DBG(fmt, ...) do { \
    struct timeval __rtsp_tv; gettimeofday(&__rtsp_tv, NULL); \
    fprintf(stderr, "[rtsp][%ld.%03ld] " fmt "\n", (long)__rtsp_tv.tv_sec, (long)(__rtsp_tv.tv_usec/1000), ##__VA_ARGS__); \
} while(0)
#define RTSP_ERR(fmt, ...) do { \
    struct timeval __rtsp_tv; gettimeofday(&__rtsp_tv, NULL); \
    fprintf(stderr, "[rtsp][ERR][%ld.%03ld] " fmt "\n", (long)__rtsp_tv.tv_sec, (long)(__rtsp_tv.tv_usec/1000), ##__VA_ARGS__); \
} while(0)
#else
#define RTSP_DBG(fmt, ...) ((void)0)
#define RTSP_ERR(fmt, ...) ((void)0)
#endif

//////////////////////////////////////////////////////////////////////////
// helpers: 时间，单位：微秒
static uint64_t now_us() {
    using namespace std::chrono;
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}

// 帮助函数：设置事件循环退出变量
static void setWatchVariable(void* clientData) {
    EventLoopWatchVariable* watch = (EventLoopWatchVariable*)clientData;
    *watch = 1;
}

//////////////////////////////////////////////////////////////////////////
// PushSource: 可被外部推入数据的 FramedSource。支持一次推入一个完整的 RTP payload（如一个 H264 NAL 单元）
class PushSource : public FramedSource {
public:
    static PushSource* createNew(UsageEnvironment& env) {
        return new PushSource(env);
    }
    virtual ~PushSource() {}
    // 通过零延迟任务唤醒（可取消），避免在对象销毁后仍触发回调导致UAF
    static void deliverTask(void* clientData);

    void shutdown() {
        std::lock_guard<std::mutex> lk(mutex_);
        shuttingDown_ = true;
        queue_.clear();
        waiting_ = false;
        if (pendingTask_ != 0) {
            envir().taskScheduler().unscheduleDelayedTask(pendingTask_);
            pendingTask_ = 0;
        }
        RTSP_DBG("PushSource::shutdown");
    }

    // from external threads 调用
    void push_frame(const uint8_t* data, unsigned dataSize, uint64_t pts_us) {
        std::lock_guard<std::mutex> lk(mutex_);
        if (shuttingDown_) return;
        // 限制队列长度，避免无客户端时无限增长
        static const size_t kMaxQueue = 60; // ~2秒缓存@30fps
        if (queue_.size() >= kMaxQueue) {
            queue_.pop_front();
            RTSP_DBG("PushSource::push_frame: drop oldest to cap queue at %zu", kMaxQueue);
        }
        queue_.emplace_back(Frame{vector<uint8_t>(data, data + dataSize), dataSize, pts_us});
        RTSP_DBG("PushSource::push_frame: size=%u, pts_us=%llu, queue=%zu, waiting=%d", dataSize, (unsigned long long)pts_us, queue_.size(), (int)waiting_);
        // 如果有等待的消费者（正在等待数据），投递一个零延迟任务（可取消）到 live555 事件循环
        if (waiting_ && pendingTask_ == 0) {
            pendingTask_ = envir().taskScheduler().scheduleDelayedTask(0, &PushSource::deliverTask, this);
        }
    }

protected:
    PushSource(UsageEnvironment& env) : FramedSource(env) {}
    // 客户端不再拉数据时由 live555 调用，清理等待与任务
    virtual void doStopGettingFrames() override {
        std::lock_guard<std::mutex> lk(mutex_);
        waiting_ = false;
        if (pendingTask_ != 0) {
            envir().taskScheduler().unscheduleDelayedTask(pendingTask_);
            pendingTask_ = 0;
        }
        RTSP_DBG("PushSource::doStopGettingFrames");
        FramedSource::doStopGettingFrames();
    }

private:
    struct Frame { vector<uint8_t> payload; unsigned size; uint64_t pts_us; };

    virtual void doGetNextFrame() override {
        // Called by live555 when it wants data.
        std::unique_lock<std::mutex> lk(mutex_);
        if (shuttingDown_) {
            RTSP_DBG("PushSource::doGetNextFrame: shutting down");
            handleClosure();
            return;
        }
        if (queue_.empty()) {
            // no data: mark waiting, return and wait for push_frame to schedule a task
            waiting_ = true;
            RTSP_DBG("PushSource::doGetNextFrame: no data, waiting=1");
            // don't block here; when push_frame arrives it will schedule deliverFrameStub
            return;
        }
        // 有数据可立即下发
        deliverFrameLocked(lk);
    }

    bool isCurrentlyAwaitingData() { return waiting_; }

    // 在持有锁的情况下从队列取出并拷贝数据，调用 afterGetting
    void deliverFrameLocked(std::unique_lock<std::mutex>& lk) {
        if (queue_.empty()) return;
        Frame f = std::move(queue_.front());
        queue_.pop_front();
        waiting_ = false;
        // 解锁再进行拷贝和回调，避免潜在的重入导致死锁
        lk.unlock();

        unsigned copySize = f.size;
        if (copySize > fMaxSize) copySize = fMaxSize;
        memcpy(fTo, f.payload.data(), copySize);
        fFrameSize = copySize;
        if (copySize < f.size) fNumTruncatedBytes = f.size - copySize;

        RTSP_DBG("PushSource::deliver: size=%u/%u, trunc=%u, pts_us=%llu", copySize, f.size, fNumTruncatedBytes, (unsigned long long)f.pts_us);

        fPresentationTime.tv_sec = (long)(f.pts_us / 1000000ULL);
        fPresentationTime.tv_usec = (long)(f.pts_us % 1000000ULL);

        FramedSource::afterGetting(this);
    }

    std::mutex mutex_;
    std::deque<Frame> queue_;
    bool waiting_{false};
    bool shuttingDown_{false};
    TaskToken pendingTask_{0};
};

// deliverTask 的类外定义（零延迟任务回调）
void PushSource::deliverTask(void* clientData) {
    PushSource* src = static_cast<PushSource*>(clientData);
    if (!src) return;
    std::unique_lock<std::mutex> lk(src->mutex_);
    // 任务被触发，清除 token，避免重复
    src->pendingTask_ = 0;
    if (src->shuttingDown_) return;
    // 只有在等待数据且队列非空时才下发
    if (!src->waiting_ || src->queue_.empty()) return;
    src->deliverFrameLocked(lk);
}

//////////////////////////////////////////////////////////////////////////
// VideoSubsession: 返回 H264VideoRTPSink，并使用 PushSource 作为源
class PushVideoSubsession : public OnDemandServerMediaSubsession {
public:
    static PushVideoSubsession* createNew(UsageEnvironment& env, int payloadType = 96) {
        return new PushVideoSubsession(env, payloadType);
    }

    PushSource* getPushSource() { return pushSource_.get(); }
    void setSpsPps(const std::vector<uint8_t>& sps, const std::vector<uint8_t>& pps) {
        sps_ = sps; pps_ = pps;
        RTSP_DBG("VideoSubsession set SPS(%zu) PPS(%zu)", sps_.size(), pps_.size());
    }
    bool hasParameterSets() const { return !sps_.empty() && !pps_.empty(); }
    void pushSpsPps(PushSource* src, uint64_t pts_us) {
        if (!src || sps_.empty() || pps_.empty()) return;
        static const uint8_t sc4[] = {0,0,0,1};
        std::vector<uint8_t> pkt;
        pkt.reserve(4 + sps_.size());
        pkt.insert(pkt.end(), sc4, sc4+4);
        pkt.insert(pkt.end(), sps_.begin(), sps_.end());
        src->push_frame(pkt.data(), (unsigned)pkt.size(), pts_us);
        pkt.clear();
        pkt.reserve(4 + pps_.size());
        pkt.insert(pkt.end(), sc4, sc4+4);
        pkt.insert(pkt.end(), pps_.begin(), pps_.end());
        src->push_frame(pkt.data(), (unsigned)pkt.size(), pts_us);
        RTSP_DBG("Injected SPS/PPS before IDR");
    }

protected:
    PushVideoSubsession(UsageEnvironment& env, int payloadType)
        : OnDemandServerMediaSubsession(env, True /*reuseFirstSource*/), payloadType_(payloadType) {
        // 提前创建 push 源，便于在客户端连接前即可推流缓存
        pushSource_.reset(PushSource::createNew(envir()));
        // 使用 StreamReplicator 做一对多复用，避免过滤器回收底层源造成 double free
        videoReplicator_ = StreamReplicator::createNew(envir(), pushSource_.get(), False /*deleteInputSourceOnClose*/);
        RTSP_DBG("VideoSubsession: created PushSource=%p, Replicator=%p", (void*)pushSource_.get(), (void*)videoReplicator_);
    }

    virtual ~PushVideoSubsession() {
        if (pushSource_) pushSource_->shutdown();
        if (videoReplicator_) {
            RTSP_DBG("VideoSubsession dtor: closing Replicator=%p", (void*)videoReplicator_);
            Medium::close(videoReplicator_);
            videoReplicator_ = nullptr;
        }
        // pushSource_ 将由 unique_ptr 自动释放
    }

    virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate) override {
        estBitrate = 500; // kbps hint
        // 使用 H264VideoStreamFramer 解析 AnnexB 起始码格式（RK 编码默认带 0x000001），避免 DiscreteFramer 报 start code 错误
        RTSP_DBG("VideoSubsession::createNewStreamSource: client=%u, estBitrate=%u (replica, AnnexB)", clientSessionId, estBitrate);
        FramedSource* replica = videoReplicator_ ? videoReplicator_->createStreamReplica() : pushSource_.get();
        return H264VideoStreamFramer::createNew(envir(), replica);
    }

    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource) override {
        // 增大输出包缓冲，避免大 NAL 被截断（日志显示最大接近 100KB）
        unsigned newMax = 150000; // 保守 > 最大观察值
        if (OutPacketBuffer::maxSize < newMax) {
            OutPacketBuffer::maxSize = newMax;
            RTSP_DBG("OutPacketBuffer::maxSize set to %u", OutPacketBuffer::maxSize);
        }
        if (!sps_.empty() && !pps_.empty()) {
            RTSP_DBG("VideoSubsession::createNewRTPSink: Using SPS/PPS" );
            return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic,
                                               sps_.data(), (unsigned)sps_.size(),
                                               pps_.data(), (unsigned)pps_.size());
        }
        RTSP_DBG("VideoSubsession::createNewRTPSink: No SPS/PPS, dynamic PT=%u", rtpPayloadTypeIfDynamic);
        return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
    }

private:
    int payloadType_;
    std::unique_ptr<PushSource> pushSource_;
    StreamReplicator* videoReplicator_{nullptr};
    std::vector<uint8_t> sps_;
    std::vector<uint8_t> pps_;
};

//////////////////////////////////////////////////////////////////////////
// AudioSubsession 示例：这里以 PCMA (G.711 A) 为例，示意如何做
class PushAudioSubsession : public OnDemandServerMediaSubsession {
public:
    static PushAudioSubsession* createNew(UsageEnvironment& env, unsigned char rtpPayloadType = 8 /*PCMA*/) {
        return new PushAudioSubsession(env, rtpPayloadType);
    }

    PushSource* getPushSource() { return pushSource_.get(); }
    void setAudioParams(unsigned sampleRate, unsigned channels) {
        if (sampleRate > 0) sampleRate_ = sampleRate;
        if (channels > 0) channels_ = channels;
        RTSP_DBG("AudioSubsession set params: sampleRate=%u, channels=%u", sampleRate_, channels_);
    }

protected:
    PushAudioSubsession(UsageEnvironment& env, unsigned char rtpPayloadType)
        : OnDemandServerMediaSubsession(env, True), rtpPayloadType_(rtpPayloadType) {
        pushSource_.reset(PushSource::createNew(envir()));
        audioReplicator_ = StreamReplicator::createNew(envir(), pushSource_.get(), False);
        RTSP_DBG("AudioSubsession: created PushSource=%p, Replicator=%p", (void*)pushSource_.get(), (void*)audioReplicator_);
    }

    virtual ~PushAudioSubsession() {
        if (pushSource_) pushSource_->shutdown();
        if (audioReplicator_) {
            RTSP_DBG("AudioSubsession dtor: closing Replicator=%p", (void*)audioReplicator_);
            Medium::close(audioReplicator_);
            audioReplicator_ = nullptr;
        }
    }

    virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate) override {
        estBitrate = 64;
        RTSP_DBG("AudioSubsession::createNewStreamSource: client=%u, estBitrate=%u (replica)", clientSessionId, estBitrate);
        FramedSource* replica = audioReplicator_ ? audioReplicator_->createStreamReplica() : pushSource_.get();
        return replica;
    }

    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource) override {
        // PCMA (G711A) 默认 8000Hz，1 通道，可由外部覆盖
        unsigned sr = sampleRate_ == 0 ? 8000 : sampleRate_;
        unsigned ch = channels_ == 0 ? 1 : channels_;
    RTSP_DBG("AudioSubsession::createNewRTPSink: PT=%u, sr=%u, ch=%u", rtpPayloadTypeIfDynamic, sr, ch);
    return SimpleRTPSink::createNew(envir(), rtpGroupsock,
                                        rtpPayloadTypeIfDynamic,
                                        sr,
                                        "audio",
                                        "PCMA",
                                        ch);
    }

private:
    unsigned char rtpPayloadType_;
    std::unique_ptr<PushSource> pushSource_;
    StreamReplicator* audioReplicator_{nullptr};
    unsigned sampleRate_{8000};
    unsigned channels_{1};
};

//////////////////////////////////////////////////////////////////////////
// RtspSession & RtspDemo 管理
struct RtspSession {
    string path;
    ServerMediaSession* sms;
    PushVideoSubsession* videoSub;
    PushAudioSubsession* audioSub;
    class RtspDemo* owner; // 用于删除会话
};

class RtspDemo {
public:
    RtspDemo(int port) {
        scheduler_ = BasicTaskScheduler::createNew();
        env_ = BasicUsageEnvironment::createNew(*scheduler_);
        // 此实现不使用全局 EventTrigger，改用可取消的零延迟任务机制
        rtspServer_ = RTSPServer::createNew(*env_, port);
        if (!rtspServer_) {
            RTSP_ERR("Failed to create RTSPServer on port %d: %s", port, env_->getResultMsg());
        } else {
            RTSP_DBG("RTSPServer created on port %d", port);
        }

        // 启动专用事件循环线程，避免主线程逐步 doEventOnce 导致处理不及时
        watchVar_ = 0;
        eventThread_ = std::thread([this]() {
            RTSP_DBG("RTSP event loop thread start");
            env_->taskScheduler().doEventLoop(&watchVar_);
            RTSP_DBG("RTSP event loop thread exit");
        });
    }

    ~RtspDemo() {
        // 先关闭并移除所有会话（在事件线程执行），确保子资源释放安全
        if (rtspServer_) {
            for (auto* s : sessions_) {
                if (!s) continue;
                RTSP_DBG("RtspDemo dtor: removing session path=%s", s->path.c_str());
                if (s->sms) {
                    runOnEventLoopSync([this, s]{
                        if (this->rtspServer_ && s->sms) {
                            this->rtspServer_->removeServerMediaSession(s->sms);
                            Medium::close(s->sms);
                        }
                    });
                    s->sms = nullptr;
                }
                s->videoSub = nullptr;
                s->audioSub = nullptr;
                delete s;
            }
            sessions_.clear();
        }

        // 关闭 RTSPServer（在事件线程执行）
        if (rtspServer_) {
            runOnEventLoopSync([this]{ if (this->rtspServer_) Medium::close(this->rtspServer_); });
            rtspServer_ = nullptr;
            RTSP_DBG("RTSPServer closed");
        }

        // 请求事件循环退出并等待线程结束
        if (env_) {
            watchVar_ = 1;
            env_->taskScheduler().scheduleDelayedTask(0, setWatchVariable, &watchVar_);
        }
        if (eventThread_.joinable()) eventThread_.join();

        // 释放环境与调度器
        if (env_) {
            env_->reclaim();
            env_ = nullptr;
            RTSP_DBG("UsageEnvironment reclaimed");
        }
        if (scheduler_) {
            delete scheduler_;
            scheduler_ = nullptr;
            RTSP_DBG("TaskScheduler deleted");
        }
    }

    UsageEnvironment& envir() { return *env_; }
    RTSPServer* server() { return rtspServer_; }
    void removeSession(RtspSession* target) {
        if (!target) return;
        auto it = std::find(sessions_.begin(), sessions_.end(), target);
        if (it != sessions_.end()) sessions_.erase(it);
    }

    // 在 live555 事件线程同步执行任务，直到任务完成
    void runOnEventLoopSync(const std::function<void()>& fn) {
        if (!env_) return;
        std::mutex m;
        std::condition_variable cv;
        bool done = false;
        struct Closure {
            std::function<void()> fn;
            std::mutex* m;
            std::condition_variable* cv;
            bool* done;
        };
        auto* c = new Closure{fn, &m, &cv, &done};
        env_->taskScheduler().scheduleDelayedTask(0,
            [](void* cd){
                auto* c = static_cast<Closure*>(cd);
                if (c->fn) c->fn();
                {
                    std::unique_lock<std::mutex> lk(*c->m);
                    *c->done = true;
                }
                c->cv->notify_one();
                delete c;
            }, c);
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&]{ return done; });
    }

    RtspSession* newSession(const char* path) {
        string s = path ? path : "";
        // 修正 server 使用的 URL 名称，去掉开头的 '/'
        string name = s;
        while (!name.empty() && name[0] == '/') name.erase(0, 1);
        if (name.empty()) name = "live";
        // create ServerMediaSession
        ServerMediaSession* sms = ServerMediaSession::createNew(envir(), name.c_str(), s.c_str(), "RTSP Live Session");
        if (!sms) {
            RTSP_ERR("Failed to create ServerMediaSession for path %s", s.c_str());
            return nullptr;
        }
        // create subsessions
        PushVideoSubsession* vsub = PushVideoSubsession::createNew(envir());
        PushAudioSubsession* asub = PushAudioSubsession::createNew(envir());
        sms->addSubsession(vsub);
        sms->addSubsession(asub);
        if (!rtspServer_) {
            RTSP_ERR("RTSPServer not available when adding session %s", s.c_str());
        } else {
            rtspServer_->addServerMediaSession(sms);
            char* url = rtspServer_->rtspURL(sms);
            if (url) { RTSP_DBG("New session added: path=%s, url=%s", s.c_str(), url); delete[] url; }
            RTSP_DBG("Hint: If your client fails, try removing duplicate '/' in URL");
        }
        RtspSession* rs = new RtspSession();
        rs->path = s;
        rs->sms = sms;
        rs->videoSub = vsub;
        rs->audioSub = asub;
        rs->owner = this;
        sessions_.push_back(rs);
        return rs;
    }

    // find by path
    RtspSession* findSession(const char* path) {
        for (auto* s : sessions_) {
            if (s->path == path) return s;
        }
        return nullptr;
    }
    void doEventOnce() {
        // 事件线程常驻运行，这里无需再推进事件循环
        // 为保持兼容旧调用，留空实现并返回
        (void)0;
    }

    // 进入阻塞事件循环（如果需要）
    void loopForever() {
        env_->taskScheduler().doEventLoop();
    }

private:
    TaskScheduler* scheduler_{nullptr};
    UsageEnvironment* env_{nullptr};
    RTSPServer* rtspServer_{nullptr};
    vector<RtspSession*> sessions_;
    // 专用事件循环线程与退出监视变量
    std::thread eventThread_;
    EventLoopWatchVariable watchVar_{};
};

//////////////////////////////////////////////////////////////////////////
// 全局容器（简单实现）
static std::mutex g_demo_mutex;
static std::vector<RtspDemo*> g_demos;

//////////////////////////////////////////////////////////////////////////
// C API 实现
extern "C" {

rtsp_demo_handle rtsp_new_demo(int port) {
    std::lock_guard<std::mutex> lk(g_demo_mutex);
    RtspDemo* d = new RtspDemo(port);
    g_demos.push_back(d);
    RTSP_DBG("rtsp_new_demo: demo=%p port=%d", (void*)d, port);
    return (rtsp_demo_handle)d;
}
rtsp_demo_handle create_rtsp_demo(int port) { return rtsp_new_demo(port); }

int rtsp_do_event(rtsp_demo_handle demo) {
    RtspDemo* d = (RtspDemo*)demo;
    if (!d) return -1;
    RTSP_DBG("rtsp_do_event: demo=%p", (void*)d);
    d->doEventOnce();
    return 0;
}

rtsp_session_handle rtsp_new_session(rtsp_demo_handle demo, const char* path) {
    RtspDemo* d = (RtspDemo*)demo;
    if (!d || !path) return nullptr;
    RTSP_DBG("rtsp_new_session: demo=%p path=%s", (void*)d, path);
    RtspSession* s = d->newSession(path);
    return (rtsp_session_handle)s;
}
rtsp_session_handle create_rtsp_session(rtsp_demo_handle demo, const char* path) { return rtsp_new_session(demo, path); }

int rtsp_set_video(rtsp_session_handle session, int codec_id, const uint8_t* codec_data, int data_len) {
    // 仅处理 H264：codec_data 可能包含 AnnexB 起始码的 NAL 单元（SPS/PPS）
    if (!session) return -1;
    RtspSession* s = (RtspSession*)session;
    if (!s->videoSub) return -1;
    if (codec_id != RTSP_CODEC_ID_VIDEO_H264 || !codec_data || data_len <= 0) return 0;

    auto find_nalus = [](const uint8_t* data, int len) {
        vector<vector<uint8_t>> nalus;
        int i = 0; int start = -1; int scSize = 0;
        auto isStart = [&](int pos, int& size)->bool{
            if (pos + 3 <= len && data[pos]==0 && data[pos+1]==0 && data[pos+2]==1){ size=3; return true; }
            if (pos + 4 <= len && data[pos]==0 && data[pos+1]==0 && data[pos+2]==0 && data[pos+3]==1){ size=4; return true; }
            return false;
        };
        while (i < len) {
            int size=0;
            if (isStart(i,size)) {
                if (start>=0) {
                    nalus.emplace_back(data+start+scSize, data+i);
                }
                start = i; scSize = size; i += size; continue;
            }
            ++i;
        }
        if (start>=0) nalus.emplace_back(data+start+scSize, data+len);
        return nalus;
    };

    auto nalus = find_nalus(codec_data, data_len);
    vector<uint8_t> sps, pps;
    for (auto& n : nalus) {
        if (n.empty()) continue;
        uint8_t nalType = n[0] & 0x1F;
        if (nalType == 7) sps = n;
        else if (nalType == 8) pps = n;
    }
    if (!sps.empty() && !pps.empty()) {
        s->videoSub->setSpsPps(sps, pps);
        RTSP_DBG("rtsp_set_video: SPS/PPS parsed and set, sps=%zu pps=%zu", sps.size(), pps.size());
    } else {
        RTSP_DBG("rtsp_set_video: No SPS/PPS found in codec_data (len=%d)", data_len);
    }
    return 0;
}

int rtsp_set_audio(rtsp_session_handle session, int codec_id, const uint8_t* codec_data, int data_len) {
    // 仅支持 G711A/U：无需 codec_data
    (void)codec_id; (void)codec_data; (void)data_len;
    return 0;
}

int rtsp_set_audio_sample_rate (rtsp_session_handle session,int sample_rate){
    if (!session) return -1;
    RtspSession* s = (RtspSession*)session;
    if (s->audioSub) s->audioSub->setAudioParams(sample_rate, 0);
    return 0;
}
int rtsp_set_audio_channels (rtsp_session_handle session,int channels){
    if (!session) return -1;
    RtspSession* s = (RtspSession*)session;
    if (s->audioSub) s->audioSub->setAudioParams(0, channels);
    return 0;
}

int rtsp_sever_tx_video(rtsp_demo_handle demo, rtsp_session_handle session, const uint8_t *frame,
                        int len, uint64_t ts) {
    // demo级别推送（等同于 session 推送）
    (void)demo;
    RTSP_DBG("rtsp_sever_tx_video: session=%p len=%d ts=%llu", session, len, (unsigned long long)ts);
    return rtsp_tx_video(session, frame, len, ts);
}

int rtsp_tx_video(rtsp_session_handle session, const uint8_t *frame, int len, uint64_t ts) {
    if (!session || !frame || len <= 0) return -1;
    RtspSession* s = (RtspSession*)session;
    if (!s->videoSub) return -1;
    PushSource* src = s->videoSub->getPushSource();
    if (!src) return -1;
    // push frame; 保证 ts 为微秒（us），如果用户给的是 ms 或 90K ticks，请转换
    RTSP_DBG("rtsp_tx_video: session=%p len=%d ts=%llu", session, len, (unsigned long long)ts);
    // 检测 IDR：AnnexB 起始码后第一个字节的 NAL 类型为 5
    auto find_first_nal = [&](const uint8_t* data, int size)->const uint8_t*{
        for (int i=0;i+3<size;i++){
            if (data[i]==0 && data[i+1]==0 && ((data[i+2]==1) || (i+4<size && data[i+2]==0 && data[i+3]==1))){
                int sc = (data[i+2]==1)?3:4;
                int off = i+sc;
                if (off < size) return data+off;
            }
        }
        return nullptr;
    };
    const uint8_t* nal = find_first_nal(frame, len);
    if (nal) {
        uint8_t nalType = nal[0] & 0x1F;
        if (nalType == 5 && s->videoSub->hasParameterSets()) {
            // 在 IDR 前注入 SPS/PPS，帮助解码器快速恢复
            s->videoSub->pushSpsPps(src, ts);
        }
    }
    src->push_frame(frame, (unsigned)len, ts);
    return 0;
}

int rtsp_tx_audio(rtsp_session_handle session, const uint8_t *frame, int len, uint64_t ts) {
    if (!session || !frame || len <= 0) return -1;
    RtspSession* s = (RtspSession*)session;
    if (!s->audioSub) return -1;
    PushSource* src = s->audioSub->getPushSource();
    if (!src) return -1;
    RTSP_DBG("rtsp_tx_audio: session=%p len=%d ts=%llu", session, len, (unsigned long long)ts);
    src->push_frame(frame, (unsigned)len, ts);
    return 0;
}

void rtsp_del_session(rtsp_session_handle session) {
    if (!session) return;
    RtspSession* s = (RtspSession*)session;
    // 从服务器移除并释放
    RTSP_DBG("rtsp_del_session: session=%p path=%s", session, s->path.c_str());
    if (s->owner && s->owner->server() && s->sms) {
        s->owner->runOnEventLoopSync([s]{
            if (s->owner->server() && s->sms) s->owner->server()->removeServerMediaSession(s->sms);
            if (s->sms) { Medium::close(s->sms); s->sms = nullptr; }
        });
    }
    s->videoSub = nullptr;
    s->audioSub = nullptr;
    if (s->owner) s->owner->removeSession(s);
    delete s;
}

void rtsp_del_demo(rtsp_demo_handle demo) {
    std::lock_guard<std::mutex> lk(g_demo_mutex);
    RtspDemo* d = (RtspDemo*)demo;
    if (!d) return;
    // find and remove
    auto it = std::find(g_demos.begin(), g_demos.end(), d);
    if (it != g_demos.end()) g_demos.erase(it);
    RTSP_DBG("rtsp_del_demo: demo=%p", (void*)d);
    delete d;
}

uint64_t rtsp_get_reltime(void) {
    return now_us();
}

uint64_t rtsp_get_ntptime(void) {
    // 简单实现：返回系统时间 (UNIX epoch) in microseconds
    using namespace std::chrono;
    auto now = system_clock::now();
    return duration_cast<microseconds>(now.time_since_epoch()).count();
}

int rtsp_sync_video_ts(rtsp_session_handle /*session*/, uint64_t /*ts*/, uint64_t /*ntptime*/) { return 0; }
int rtsp_sync_audio_ts(rtsp_session_handle /*session*/, uint64_t /*ts*/, uint64_t /*ntptime*/) { return 0; }

} // extern "C"
