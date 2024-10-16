#include "control/control.h"
#include "led/led.h"
#include "pantilt/pantilt.h"
#include "video/video.h"
#include "display/display.h"
#include "network/network.h"

/**
 * @brief 初始化控制模块
 */
void control_init() {
    // 可以在此进行初始化操作，比如启动所需的线程或资源
    printf("Control module initialized.\n");
}

/**
 * @brief 处理并解析接收到的控制信号
 * @param data 从网络接收到的数据
 * @return 解析后的ControlSignal结构
 */
ControlSignal control_parse_signal(const char *data) {
    ControlSignal signal;
    sscanf(data, "%d %d %d", &signal.module_id, &signal.opcode, &signal.param);
    return signal;
}

/**
 * @brief 分发控制信号到相应的模块
 * @param signal 已解析的控制信号
 */
void control_dispatch_signal(const ControlSignal *signal) {
    switch (signal->module_id) {
        case 1:  // LED 模块
            led_control(signal->opcode, signal->param);
            break;
        case 2:  // 舵机云台 模块
            pantilt_control(signal->opcode, signal->param);
            break;
        case 3:  // 视频模块
            video_control(signal->opcode, signal->param);
            break;
        case 4:  // 显示模块
            display_control(signal->opcode, signal->param);
            break;
        default:
            printf("Unknown module ID: %d\n", signal->module_id);
    }
}
