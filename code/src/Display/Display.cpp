#include "Display/Display.h"

extern "C" {
    #include "Display/framebuffer.h"
}

Display::Display() {
    // 初始化 framebuffer 设备，自动获取分辨率和色深
    framebuffer_init(FB_DEVICE);
    framebuffer_get_resolution(&(this->width), &(this->height), &(this->bit_depth));

    flag_pause = false;

    // 启动显示线程
    display_thread = std::thread(&Display::display_on_fb, this);
}

Display::~Display() {
    // 设置退出标志，并唤醒所有等待的线程
    {
        // 作用域自动加锁，超出作用域自动解锁
        std::lock_guard<std::mutex> lock(mtx_display);
        flag_quit = true;
    }
    cond_var_display.notify_all();

    // 等待线程结束
    if (display_thread.joinable()) {
        display_thread.join();
    }

    // 释放 framebuffer 资源
    framebuffer_deinit();
}

void Display::push_frame(const cv::Mat& frame) {
    if (frame.rows != this->height || 
        frame.cols != this->width ||
        frame.type() != CV_8UC3) {
        return;
    }
    
    {
        std::lock_guard<std::mutex> lock(mtx_display);
        if (frame_queue.size() < 10) { // 限制队列长度，避免内存占用过多
            frame_queue.push(frame);
        }
    }
    cond_var_display.notify_one(); // 唤醒显示线程
}

void Display::start_display() {
    resume_display();
}

void Display::pause_display() {
    std::lock_guard<std::mutex> lock(mtx_display);
    flag_pause = true;
}

void Display::resume_display() {
    {
        std::lock_guard<std::mutex> lock(mtx_display);
        flag_pause = false;
    }
    cond_var_display.notify_one(); // 唤醒显示线程继续工作
}

void Display::display_on_fb() {
    while (true) {
        std::unique_lock<std::mutex> ulock(mtx_display);

        // 等待队列有数据或退出信号
        cond_var_display.wait(ulock, [this] {
            return !frame_queue.empty() || flag_quit;
        });

        // 如果设置了退出标志，退出线程
        if (flag_quit) {
            break;
        }

        // 暂停时等待唤醒
        if (flag_pause) {
            cond_var_display.wait(ulock, [this] { 
                return !flag_pause || flag_quit; 
            });
        }

        // 取出一帧
        cv::Mat frame = frame_queue.front();
        frame_queue.pop();

        ulock.unlock(); // 解锁以允许其他线程推送帧

        // 处理帧：旋转、调整大小、转换格式，并显示在 framebuffer 上
        cv::Mat rotated_frame, resized_frame, rgb565;
        cv::rotate(frame, rotated_frame, cv::ROTATE_90_COUNTERCLOCKWISE);
        cv::resize(rotated_frame, resized_frame, cv::Size(width, height));
        cv::cvtColor(resized_frame, rgb565, cv::COLOR_RGB2BGR565);

        framebuffer_set_frame_rgb565((uint16_t*)rgb565.data, width, height);
    }
}