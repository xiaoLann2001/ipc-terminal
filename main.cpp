
#include "ipc_global.h"

#include "display/framebuffer.h"
#include "video/video.h"
#include "rk_debug.h"

bool quit = false;
std::mutex mtx;
std::condition_variable cond_var;
bool frame_ready = false;  // 标记是否有新的帧

static void sigterm_handler(int sig) {
	fprintf(stderr, "Caught signal %d, cleaning up...\n", sig);
	quit = true;
    cond_var.notify_all();  // 通知显示线程退出
}

void display_on_fb(cv::Mat* frame) {
	cv::Mat rotated_frame, resized_frame, rgb565;
    int width, height, bit_depth;
    framebuffer_get_resolution(&width, &height, &bit_depth);

	while (!quit) {
		std::unique_lock<std::mutex> ulock(mtx);
        cond_var.wait(ulock, [] { return frame_ready; });  // 等待新帧
        if (!quit){
            break;
        }

		// show on /dev/fb0
		// 旋转图像
		cv::rotate(*frame, rotated_frame, cv::ROTATE_90_COUNTERCLOCKWISE);

		// 调整帧的大小到 framebuffer 的分辨率
		cv::resize(rotated_frame, resized_frame, cv::Size(width, height));

		// 将帧转换为 RGB565 格式
		cv::cvtColor(resized_frame, rgb565, cv::COLOR_RGB2BGR565);

		// 写入 RGB565 数据到 framebuffer
		framebuffer_set_frame_rgb565((uint16_t *)rgb565.data, width, height);

		frame_ready = false;  // 重置标记，表示帧已处理
        ulock.unlock();  // 解锁以允许主线程更新帧
	}
}

int main() {
    // Register signal handlers for SIGINT (Ctrl+C) and SIGTERM
    signal(SIGINT, sigterm_handler);
    signal(SIGTERM, sigterm_handler);

    RK_S32 s32Ret = RK_SUCCESS;
    int width = 720;
    int height = 480;
    cv::Mat frame(height, width, CV_8UC3);		
    cv::Mat frame_copy(height, width, CV_8UC3);

    //h264_frame	
	VENC_STREAM_S stFrame;	
	stFrame.pstPack = (VENC_PACK_S *)malloc(sizeof(VENC_PACK_S));
 	VIDEO_FRAME_INFO_S h264_frame;
 	VIDEO_FRAME_INFO_S stVpssFrame;

    video_rtsp_init(554);
    video_capture_init(width, height);
    video_encode_init(width, height, RK_VIDEO_ID_AVC);

    framebuffer_init(FB_DEVICE);

    std::thread t_display(display_on_fb, &frame_copy);

    while (!quit) {

        // void *data = video_capture_get_frame(&stVpssFrame);

        // if (data) {
        //     frame.data = (uchar *)data;
        //     frame_copy = frame;	
        //     frame_ready = true;
        //     cond_var.notify_one();
        // }

        s32Ret = video_encode(&stVpssFrame, &stFrame);
        if (RK_SUCCESS == s32Ret) {
            video_rtsp_send(&stFrame);
        }

        s32Ret = video_capture_release_frame(&stVpssFrame);
        if (RK_SUCCESS == s32Ret) {
			RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
        }

        s32Ret = video_encode_release_frame(&stFrame);
        if (RK_SUCCESS == s32Ret) {
			RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
        }
    }

    t_display.join();

    framebuffer_deinit();
    video_capture_cleanup();
    video_encode_cleanup();
    free(stFrame.pstPack);
    video_rtsp_cleanup();

    return 0;
}
