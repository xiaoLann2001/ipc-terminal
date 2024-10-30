#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <functional>

#include "global.h"

struct ControlSignal {
    int module_id;
    int opcode;
    int param;
};

enum ModuleID {
    ID_UNKNOWN = 0,
    ID_LED,
    ID_PANTILT,
    ID_VIDEO,
    ID_DISPLAY
};

enum LEDOpcode {
    OP_LED_ON = 0,
    OP_LED_OFF,
    OP_LED_TOGGLE,
    OP_LED_BLINK,
    OP_LED_MAX
};

enum LEDParam {
    PA_LED0 = 0,
    PA_LED1,
    PA_LED_MAX
};

enum PantiltOpcode {
    OP_PANTILT_UP = 0,
    OP_PANTILT_DOWN,
    OP_PANTILT_LEFT,
    OP_PANTILT_RIGHT,
    OP_PANTILT_RESET,
    OP_PANTILT_MAX
};

enum DisplayOpcode {
    OP_DISPLAY_PAUSE = 0,
    OP_DISPLAY_RESUME,
    OP_DISPLAY_MAX
};

class Control {
public:
    Control();
    ~Control();

    Signal<ControlSignal> signal_control_received;

    // 接收网络数据槽函数
    void onNetworkReceived(const std::string& data);

    // 注册控制函数
    void registerControlFunction(int module_id, int opcode, std::function<void(int)> func);

private:
    // 控制信号解析函数
    ControlSignal parseSignal(const std::string& data);

    // 控制函数（采用函数指针数组）进行回调
    void dispatchSignal(const ControlSignal& signal);

    // 映射各个模块的控制函数，使用哈希表存储
    std::unordered_map<int, std::unordered_map<int, std::function<void(int)>>> control_functions;
};

