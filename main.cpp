
#include "ipc_global.h"

#include "framebuffer/framebuffer.h"

bool quit = false;

static void sigterm_handler(int sig) {
	fprintf(stderr, "Caught signal %d, cleaning up...\n", sig);
	quit = true;
}

int main() {

    // Register signal handlers for SIGINT (Ctrl+C) and SIGTERM
    signal(SIGINT, sigterm_handler);
    signal(SIGTERM, sigterm_handler);

    if (-1 == framebuffer_init(FB_DEVICE)) {
        fprintf(stderr, "Failed to initialize framebuffer.\n");
        return -1;
    }

    int width, height, bit_depth;
    framebuffer_get_resolution(&width, &height, &bit_depth);
    printf("Framebuffer resolution: %dx%d, bit depth: %d\n", width, height, bit_depth);

    if (bit_depth != 16) {
        return -1;
    }

    framebuffer_clear(RGB_BLACK);

    cv::Mat img = cv::imread("Violet.jpg");
    cv::resize(img, img, cv::Size(width / 2, height / 2));
    cv::cvtColor(img, img, cv::COLOR_BGR2BGRA);
    int off_x = 0, off_y = 0;
    int direct = 0;
    while (!quit) {
        if (framebuffer_set_frame((uint32_t *)img.data, off_x, off_y, img.cols, img.rows, FB_COLOR_RGBA32) < 0) {
            direct = 1 - direct;
        }
        usleep(1000000 / 30);
        framebuffer_clear(0x000000);
        if (direct == 0) {
            off_x += 1;
            off_y += 1;
        } else {
            off_x -= 1;
            off_y -= 1;
        }
    }

    return 0;
}