#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "rtsp_demo.h"
#include "luckfox_mpi.h"

#include "Signal/Signal.h"
#include <thread>

#define RTSP_PORT 554
#define RTSP_PATH "/live/0"

#define FPS_SHOW 0

class Video {
public:
    Video(int width, int height);
    ~Video();

    Signal<cv::Mat> signal_video_frame;

private:
    int videoinit();

    cv::Mat getFrame();

    void encodeFrame();

    void sendRtspFrame();

    void releaseChnFrame();

    void releaseVencFrame();

    void cleanup();

    void video_process();

    bool flag_quit;
    std::mutex mtx_video;
    std::thread *video_thread;

    int width;
    int height;

    rtsp_demo_handle g_rtsplive;
    rtsp_session_handle g_rtsp_session;
    // MPP_CHN_S stSrcChn, stvpssChn;

    MB_POOL src_Pool;
    MB_BLK src_Blk;

	RK_U64 H264_PTS;
	RK_U32 H264_TimeRef; 
    VENC_STREAM_S stFrame;	
	VIDEO_FRAME_INFO_S stViFrame;
    VIDEO_FRAME_INFO_S h264_frame;
    unsigned char *venc_data = NULL;
    void *vi_data = NULL;
    cv::Mat yuv420sp;
    cv::Mat bgr;
    cv::Mat frame;

#if FPS_SHOW
    char fps_text[16];
	float fps;
#endif
};