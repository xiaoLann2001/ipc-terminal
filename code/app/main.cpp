#include <memory>

#include "global.h"

#include "NetworkModule.h"
#include "Led.h"
#include "pantilt.h"
#include "Display.h"
#include "Video.h"

#include "onvif_server.h"

// 日志级别
int rkipc_log_level = LOG_LEVEL_DEBUG;

// 用户配置文件路径
char ini_path[] = "ipc-terminal.ini";

// 主线程退出标志
bool quit = false;

// 信号处理函数
static void sigterm_handler(int sig) {
    fprintf(stderr, "Caught signal %d, cleaning up...\n", sig);
    quit = true;
}

int main(int argc, char *argv[]) {
    // 注册信号处理函数，捕获 SIGINT 和 SIGTERM
    signal(SIGINT, sigterm_handler);
    signal(SIGTERM, sigterm_handler);

    LOG_INFO("Initializing IPC Terminal...\n");

    // 动态参数初始化
    rk_param_init(ini_path);

    // ONVIF 服务初始化
    // LOG_DEBUG("ONVIF server initializing\n");
    // onvif_server_init();
    
    // Video 模块初始化
    std::unique_ptr<AbstractModule> videoModule = std::make_unique<Video>();

    // Loop
    while (!quit) {
        sleep(1);
    }

    // ONVIF 服务反初始化
    // LOG_DEBUG("ONVIF server deinitializing\n");
    // onvif_server_deinit();

    // 动态参数反初始化，将内存中的键值对落盘
    rk_param_deinit();

    LOG_INFO("Program exited\n");
    return 0;
}