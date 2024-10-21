#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include "Signal/Signal.h"

#include "led/led.h"
#include "Pantilt/Pantilt.h"
#include "Video/Video.h"
#include "Display/Display.h"
#include "Network/Network.h"

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

    Display *display;
    Video *video;
    Pantilt *pantilt;

    Signal<ControlSignal> signal_control_received;

    // 接收网络数据槽函数
    void onNetworkReceived(const std::string& data);

private:
    ControlSignal parseSignal(const std::string& data);

    void dispatchSignal(const ControlSignal& signal);

    void init();

    // 函数指针数组，用于存储不同模块的操作处理函数
    using ControlFunction = std::function<void(int)>;
    std::vector<ControlFunction> led_control_functions;
    std::vector<ControlFunction> pantilt_control_functions;
    std::vector<ControlFunction> display_control_functions;

    // 定义各个模块的处理函数
    void handleLEDControl(int opcode, int param);
    void handlePantiltControl(int opcode, int param);
    void handleDisplayControl(int opcode, int param);
};

