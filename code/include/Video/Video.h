#pragma once

#include <iostream>
#include <thread>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Video/luckfox_rtsp.h"
#include "Video/luckfox_video.h"

#include "Signal/Signal.h"

class Video {
public:
    Video();
    ~Video();

    Signal<cv::Mat> signal_video_frame;

private:
    void video_pipe0();
    void video_pipe1();
    void video_pipe2();

    bool flag_quit;
    std::mutex mtx_video;
    std::thread *video_thread0;
    std::thread *video_thread1;
    std::thread *video_thread2;
};