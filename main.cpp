
#include "ipc_global.h"

// #include "framebuffer/framebuffer.h"
#include "video_capture/video_capture.h"

bool quit = false;

static void sigterm_handler(int sig) {
	fprintf(stderr, "Caught signal %d, cleaning up...\n", sig);
	quit = true;
}

// // 互斥锁和条件变量
// std::mutex mtx;
// std::condition_variable cond_var;
// bool frame_ready = false;  // 标记是否有新的帧

// void display_on_fb(cv::Mat* frame) {
// 	cv::Mat rotated_frame, resized_frame, rgb565;
//     int width, height, bit_depth;
//     framebuffer_get_resolution(&width, &height, &bit_depth);

// 	while (1) {
// 		std::unique_lock<std::mutex> ulock(mtx);
//         cond_var.wait(ulock, [] { return frame_ready; });  // 等待新帧

// 		// show on /dev/fb0
// 		// 旋转图像
// 		cv::rotate(*frame, rotated_frame, cv::ROTATE_90_COUNTERCLOCKWISE);

// 		// 调整帧的大小到 framebuffer 的分辨率
// 		cv::resize(rotated_frame, resized_frame, cv::Size(width, height));

// 		// 将帧转换为 RGB565 格式
// 		cv::cvtColor(resized_frame, rgb565, cv::COLOR_RGB2BGR565);

// 		// 写入 RGB565 数据到 framebuffer
// 		framebuffer_set_frame_rgb565((uint16_t *)rgb565.data, width, height);

// 		frame_ready = false;  // 重置标记，表示帧已处理
//         ulock.unlock();  // 解锁以允许主线程更新帧
// 	}
// }

int main() {
    // Register signal handlers for SIGINT (Ctrl+C) and SIGTERM
    signal(SIGINT, sigterm_handler);
    signal(SIGTERM, sigterm_handler);

    int width = 720;
    int height = 480;

    videocapture_init(width, height);

    // // printf("debug point 0\n");

    // framebuffer_init(FB_DEVICE);

    // // printf("debug point 1\n");

    // cv::Mat frame_copy;
    // std::thread t_display(display_on_fb, &frame_copy);

    // // printf("debug point 2\n");

    // VIDEO_FRAME_INFO_S stVpssFrame;
    // while (!quit) {
    //     void *data = videocapture_get_frame(&stVpssFrame);

    //     // printf("debug point 3\n");

    //     cv::Mat frame(height, width, CV_8UC3, data);		
        
    //     frame_copy = frame;	
    //     frame_ready = true;
    //     cond_var.notify_one();

    //     videocapture_release_frame(&stVpssFrame);
    // }

    // t_display.join();

    videocapture_cleanup();

    return 0;
}
