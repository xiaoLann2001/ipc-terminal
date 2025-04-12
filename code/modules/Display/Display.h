#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>

#include "global.h"

class Display {
public:
    Display();
    ~Display();

    // 推送帧到显示队列
    void get_resolution(int* width, int* height, int* bit_depth) {
        *width = this->width;
        *height = this->height;
        *bit_depth = this->bit_depth;
    }

    // 接收帧并将其放入队列
    void push_frame(const cv::Mat& frame);

    // 暂停显示
    void pause();

    // 继续显示
    void resume();

private:
    // 显示线程的运行函数
    void display_on_fb();

    // 线程管理
    std::thread display_thread;
    bool flag_pause;
    bool flag_quit;

    // 线程安全的队列和同步机制
    std::queue<cv::Mat> frame_queue;
    std::mutex mtx_display;
    std::condition_variable cond_var_display;

    // 帧缓冲参数
    int width = 0;
    int height = 0;
    int bit_depth = 0;
};

