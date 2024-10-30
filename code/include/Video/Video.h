#pragma once

#include <iostream>
#include <thread>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "luckfox_rtsp.h"
#include "luckfox_video.h"
#include "luckfox_rgn_draw.h"
#include "luckfox_osd.h"

#include "yolov5.h"
#include "postprocess.h"

#include "Signal.h"

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