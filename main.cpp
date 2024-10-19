#include "ipc_global.h"
#include "rk_debug.h"

#include "network/network.h"
#include "control/control.h"

bool quit = false;

static void sigterm_handler(int sig) {
    fprintf(stderr, "Caught signal %d, cleaning up...\n", sig);
    quit = true;
}

int main() {
    // 注册信号处理函数，捕获 SIGINT 和 SIGTERM
    signal(SIGINT, sigterm_handler);
    signal(SIGTERM, sigterm_handler);

    Network *network = new Network("192.168.5.42", 8888);
    Control *control = new Control();
    network->signal_network_received.connect(control, &Control::onNetworkReceived);

    while (!quit) {
        // network->send_data("Hello, server");
        sleep(1);
    }

    delete network;
    delete control;


    // // 显示类初始化
    // Display *display = new Display();

    // int width;
    // int height;
    // int bit_depth;
    // display->get_resolution(&width, &height, &bit_depth);
    // cv::Mat frame(height, width, CV_8UC3);

    // RK_S32 s32Ret = RK_SUCCESS;

    // // 视频采集初始化
    // video_capture_init(width, height);

    // while (!quit) {
    //     VIDEO_FRAME_INFO_S stVpssFrame;
    //     void *data = video_capture_get_frame(&stVpssFrame);

    //     if (data) {
    //         // 复制采集到的帧并传给Display类显示
    //         frame.data = (uchar *)data;
    //         display->push_frame(frame);
    //     }

    //     s32Ret = video_capture_release_frame(&stVpssFrame);
    //     if (RK_SUCCESS != s32Ret) {
    //         RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
    //     }
    // }

    // printf("Exit\n");
    // delete display;
    // video_capture_cleanup();


    return 0;
}
