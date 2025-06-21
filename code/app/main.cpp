#include <memory>

#include "global.h"

#include "TestClass.h"

#include "NetworkModule.h"
#include "Control.h"
#include "Led.h"
// #include "Pantilt.h"
#include "pantilt.h"
#include "Display.h"
#include "Video.h"

#include "onvif_server.h"

// 模块配置
#define ONVIF_SERVER_ENABLE 0
#define NETWORK_ENABLE 1
#define CONTROL_ENABLE 0
#define LED_ENABLE 0
#define PANTILT_ENABLE 0
#define DISPLAY_ENABLE 0
#define VIDEO_ENABLE 0

#ifdef __cplusplus
extern "C" {
    struct Pantilt g_pantilt = {0};
}
#endif

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

    LOG_INFO("Initializing IPC Terminal...\n");

    rk_param_init(ini_path);

#if 0
    std::unique_ptr<TestClass> testA = std::make_unique<TestClass>("TestA");
    std::unique_ptr<TestClass> testB = std::make_unique<TestClass>("TestB");
    testA->subscribe(testB.get());
    testA->init();
    testB->init();
    testA->notify("TestA Event", 42);
    testB->notify("TestB Event", 84);
    testA->deinit();
    testB->deinit();
#endif
#if NETWORK_ENABLE
    // Network 模块初始化
    LOG_DEBUG("Network module initialing\n");
    std::unique_ptr<NetworkModule> network = std::make_unique<NetworkModule>();
#endif
    std::unique_ptr<TestClass> testA = std::make_unique<TestClass>("TestA");
    std::unique_ptr<TestClass> testB = std::make_unique<TestClass>("TestB");
    testA->subscribe(network.get());
    testB->subscribe(testA.get());
    testA->init();
    testB->init();
    
// #if NETWORK_ENABLE
//     // Network 模块初始化
//     LOG_DEBUG("Network module initialing\n");
//     Network *network = new Network();
// #endif
#if CONTROL_ENABLE
    // Control 模块初始化
    LOG_DEBUG("Control module initializing\n");
    Control *control = new Control();
#endif
#if NETWORK_ENABLE && CONTROL_ENABLE
    network->signal_network_received.connect(control, &Control::onNetworkReceived);
#endif
#if LED_ENABLE
    // LED 模块初始化
    LOG_DEBUG("LED module initializing\n");
    Led *led0 = new Led(LED0);
    control->registerControlFunction(ID_LED, OP_LED_ON, std::bind(&Led::on, led0));
    control->registerControlFunction(ID_LED, OP_LED_OFF, std::bind(&Led::off, led0));
    control->registerControlFunction(ID_LED, OP_LED_TOGGLE, std::bind(&Led::toggle, led0));
    control->registerControlFunction(ID_LED, OP_LED_BLINK, std::bind(&Led::blink, led0, std::placeholders::_1));
#endif
#if PANTILT_ENABLE
    // Pantilt 模块初始化
    LOG_DEBUG("Pantilt module initializing\n");
    // Pantilt *pantilt = new Pantilt();
    // control->registerControlFunction(ID_PANTILT, OP_PANTILT_UP, std::bind(&Pantilt::up, pantilt, std::placeholders::_1));
    // control->registerControlFunction(ID_PANTILT, OP_PANTILT_DOWN, std::bind(&Pantilt::down, pantilt, std::placeholders::_1));
    // control->registerControlFunction(ID_PANTILT, OP_PANTILT_LEFT, std::bind(&Pantilt::left, pantilt, std::placeholders::_1));
    // control->registerControlFunction(ID_PANTILT, OP_PANTILT_RIGHT, std::bind(&Pantilt::right, pantilt, std::placeholders::_1));
    // control->registerControlFunction(ID_PANTILT, OP_PANTILT_RESET, std::bind(&Pantilt::reset, pantilt));

    pantilt_init(&g_pantilt);

#endif
#if DISPLAY_ENABLE
    // 显示类初始化
    LOG_DEBUG("Display module initializing\n");
    Display *display = new Display();
    control->registerControlFunction(ID_DISPLAY, OP_DISPLAY_PAUSE, std::bind(&Display::pause, display));
    control->registerControlFunction(ID_DISPLAY, OP_DISPLAY_RESUME, std::bind(&Display::resume, display));
#endif
#if VIDEO_ENABLE
    // 视频类初始化
    LOG_DEBUG("Video module initializing\n");
    system("RkLunch-stop.sh");
    Video *video = new Video();
    control->registerControlFunction(ID_VIDEO, OP_VIDEO_PIPE0_START, std::bind(&Video::video_pipe0_start, video));
    control->registerControlFunction(ID_VIDEO, OP_VIDEO_PIPE0_STOP, std::bind(&Video::video_pipe0_stop, video));
    control->registerControlFunction(ID_VIDEO, OP_VIDEO_PIPE0_RESTART, std::bind(&Video::video_pipe0_restart, video));
    control->registerControlFunction(ID_VIDEO, OP_VIDEO_PIPE1_START, std::bind(&Video::video_pipe1_start, video));
    control->registerControlFunction(ID_VIDEO, OP_VIDEO_PIPE1_STOP, std::bind(&Video::video_pipe1_stop, video));
    control->registerControlFunction(ID_VIDEO, OP_VIDEO_PIPE1_RESTART, std::bind(&Video::video_pipe1_restart, video));
#endif
#if VIDEO_ENABLE && DISPLAY_ENABLE
    video->signal_video_frame.connect(display, &Display::push_frame);
#endif
#if VIDEO_ENABLE && PANTILT_ENABLE
    // video->signal_adjust_pantilt.connect(pantilt, &Pantilt::onAjustPantilt);
#endif
#if ONVIF_SERVER_ENABLE
    // ONVIF 服务初始化
    LOG_DEBUG("ONVIF server initializing\n");
    onvif_server_init();
#endif

    while (!quit) {
        // LOG_ERROR("neteork:server_ip%s\n", rk_param_get_string("network:server_ip", ""));
        // LOG_WARN("neteork:server_ip%s\n", rk_param_get_string("network:server_ip", ""));
        // LOG_INFO("neteork:server_ip%s\n", rk_param_get_string("network:server_ip", ""));
        // LOG_DEBUG("neteork:server_ip%s\n", rk_param_get_string("network:server_ip", ""));

        // network->send_data("Hello, server");
        sleep(1);
    }

    try {
#if ONVIF_SERVER_ENABLE
        onvif_server_deinit();
        LOG_DEBUG("ONVIF server deinitialized\n");
#endif
#if VIDEO_ENABLE
        delete video;
        LOG_DEBUG("Video module deinitialized\n");
#endif
#if DISPLAY_ENABLE
        delete display;
        LOG_DEBUG("Display module deinitialized\n");
#endif
#if PANTILT_ENABLE
        // delete pantilt;
        pantilt_deinit(&g_pantilt);
        LOG_DEBUG("Pantilt module deinitialized\n");
#endif
#if LED_ENABLE
        delete led0;
        LOG_DEBUG("LED module deinitialized\n");
#endif
#if CONTROL_ENABLE
        delete control;
        LOG_DEBUG("Control module deinitialized\n");
#endif
// #if NETWORK_ENABLE
//         delete network;
//         LOG_DEBUG("Network module deinitialized\n");
// #endif
    } catch (std::exception &e) {
        LOG_ERROR("Exception: %s\n", e.what());
    }

    rk_param_deinit();

    LOG_INFO("Program exited\n");
    return 0;
}
