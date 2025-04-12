#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>
#include <chrono>
#include <unordered_set>

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
    Signal<int, int> signal_adjust_pantilt;

    void video_pipe0_start();
    void video_pipe0_stop();
    void video_pipe0_restart();

    void video_pipe1_start();
    void video_pipe1_stop();
    void video_pipe1_restart();

    void video_pipe2_start();
    void video_pipe2_stop();
    void video_pipe2_restart();

private:
    void video_pipe0();
    void video_pipe1();
    void video_pipe2();

    bool video_run_;
    bool pipe0_run_;
    bool pipe1_run_;
    bool pipe2_run_;

    std::mutex mtx_video;
    std::unique_ptr<std::thread> video_thread0;
    std::unique_ptr<std::thread> video_thread1;
    std::unique_ptr<std::thread> video_thread2;
};