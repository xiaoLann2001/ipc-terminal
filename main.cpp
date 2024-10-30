
#include "global.h"
#include "Network.h"
#include "Control.h"
#include "Led.h"
#include "Pantilt.h"
#include "Display.h"
#include "Video.h"

int rkipc_log_level = LOG_LEVEL_DEBUG;
char ini_path[] = "ipc-terminal.ini";

bool quit = false;

static void sigterm_handler(int sig) {
    fprintf(stderr, "Caught signal %d, cleaning up...\n", sig);
    quit = true;
}

int main(int argc, char *argv[]) {
    // 注册信号处理函数，捕获 SIGINT 和 SIGTERM
    signal(SIGINT, sigterm_handler);
    signal(SIGTERM, sigterm_handler);

    rk_param_init(ini_path);

    // Network 模块初始化
    LOG_DEBUG("Network module initialing\n");
    Network network;

    // Control 模块初始化
    LOG_DEBUG("Control module initializing\n");
    Control control;
    network.signal_network_received.connect(&control, &Control::onNetworkReceived);

    // LED 模块初始化
    LOG_DEBUG("LED module initializing\n");
    Led led0(LED0);
    control.registerControlFunction(ID_LED, OP_LED_ON, std::bind(&Led::on, &led0));
    control.registerControlFunction(ID_LED, OP_LED_OFF, std::bind(&Led::off, &led0));
    control.registerControlFunction(ID_LED, OP_LED_TOGGLE, std::bind(&Led::toggle, &led0));
    control.registerControlFunction(ID_LED, OP_LED_BLINK, std::bind(&Led::blink, &led0, std::placeholders::_1));

    // Pantilt 模块初始化
    LOG_DEBUG("Pantilt module initializing\n");
    Pantilt pantilt;
    control.registerControlFunction(ID_PANTILT, OP_PANTILT_UP, std::bind(&Pantilt::up, &pantilt));
    control.registerControlFunction(ID_PANTILT, OP_PANTILT_DOWN, std::bind(&Pantilt::down, &pantilt));
    control.registerControlFunction(ID_PANTILT, OP_PANTILT_LEFT, std::bind(&Pantilt::left, &pantilt));
    control.registerControlFunction(ID_PANTILT, OP_PANTILT_RIGHT, std::bind(&Pantilt::right, &pantilt));
    control.registerControlFunction(ID_PANTILT, OP_PANTILT_RESET, std::bind(&Pantilt::reset, &pantilt));

    // 显示类初始化
    LOG_DEBUG("Display module initializing\n");
    Display display;
    control.registerControlFunction(ID_DISPLAY, OP_DISPLAY_PAUSE, std::bind(&Display::pause, &display));
    control.registerControlFunction(ID_DISPLAY, OP_DISPLAY_RESUME, std::bind(&Display::resume, &display));

    // 视频类初始化
    LOG_DEBUG("Video module initializing\n");
    Video video;

    video.signal_video_frame.connect(&display, &Display::push_frame);

    while (!quit) {
        // LOG_ERROR("neteork:server_ip%s\n", rk_param_get_string("network:server_ip", ""));
        // LOG_WARN("neteork:server_ip%s\n", rk_param_get_string("network:server_ip", ""));
        // LOG_INFO("neteork:server_ip%s\n", rk_param_get_string("network:server_ip", ""));
        // LOG_DEBUG("neteork:server_ip%s\n", rk_param_get_string("network:server_ip", ""));

        // network->send_data("Hello, server");
        sleep(1);
    }

    rk_param_deinit();

    return 0;
}
