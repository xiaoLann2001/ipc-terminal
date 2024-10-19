#ifndef DISPLAY_H
#define DISPLAY_H

#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <opencv2/opencv.hpp>

class Display {
public:
    Display();
    ~Display();

    enum class PushResult {
        Success,
        FrameTypeError,
        QueueFull
    };

    void get_resolution(int* width, int* height, int* bit_depth) {
        *width = this->width;
        *height = this->height;
        *bit_depth = this->bit_depth;
    }

    // 接收帧并将其放入队列
    void push_frame(const cv::Mat& frame);

    // 启动显示线程
    void start_display();

    // 暂停显示
    void pause_display();

    // 继续显示
    void resume_display();

private:
    // 显示线程的运行函数
    void display_on_fb();

    // 线程管理
    std::thread display_thread;
    bool flag_stop = false;
    bool flag_pause = false;
    bool flag_quit = false;

    // 线程安全的队列和同步机制
    std::queue<cv::Mat> frame_queue;
    std::mutex mtx_display;
    std::condition_variable cond_var_display;

    // 帧缓冲参数
    int width = 0;
    int height = 0;
    int bit_depth = 0;
};

#endif // DISPLAY_H
