#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>
#include <chrono>
#include <unordered_set>
#include <atomic>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "AbstractModule.h"

#include "luckfox_rtsp.h"
#include "luckfox_video.h"
#include "luckfox_rgn_draw.h"
#include "luckfox_osd.h"

#include "yolov5.h"
#include "postprocess.h"

class Video : public AbstractModule {   // 基类指针要指向派生类对象时，必须是公有继承
public:
    Video();
    ~Video() override;  // 派生类需要重写虚析构函数

    bool init() override { return true;}
    void deinit() override {}

private:
    void video_pipe0();
    void video_pipe1();
    void video_pipe2();

    std::atomic<bool> video_run_;

    std::mutex mtx_video;
    std::unique_ptr<std::thread> video_thread0;
    std::unique_ptr<std::thread> video_thread1;
    std::unique_ptr<std::thread> video_thread2;
};