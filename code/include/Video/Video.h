#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "rtsp_demo.h"
#include "luckfox_mpi.h"

#include "Signal/Signal.h"
#include <thread>

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
    int rtsp_port;
    rtsp_demo_handle g_rtsplive;
    rtsp_session_handle g_rtsp_session;
    MPP_CHN_S stSrcChn, stvpssChn;

    MB_POOL src_Pool;
    MB_BLK src_Blk;

    char fps_text[16];
	float fps;

    VENC_STREAM_S stFrame;	
	RK_U64 H264_PTS;
	RK_U32 H264_TimeRef; 
	VIDEO_FRAME_INFO_S stViFrame;
    VIDEO_FRAME_INFO_S h264_frame;
    unsigned char *data;
    cv::Mat frame;
};