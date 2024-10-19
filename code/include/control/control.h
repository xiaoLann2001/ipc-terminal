#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <iostream>
#include <string>
#include <sstream>

#include "Signal/Signal.h"

#include "led/led.h"
#include "pantilt/pantilt.h"
#include "video/video.h"
#include "display/display.h"
#include "network/network.h"

struct ControlSignal {
    int module_id;
    int opcode;
    int param;
};

enum ModuleID {
    ID_LED = 1,
    ID_PANTILT,
    ID_VIDEO,
    ID_DISPLAY
};

enum LEDOpcode {
    OP_LED_ON = 0,
    OP_LED_OFF,
    OP_LED_TOGGLE,
    OP_LED_BLINK
};

enum LEDParam {
    PA_LED0 = LED0,
    PA_LED1 = LED1
};

enum PantiltOpcode {
    OP_PANTILT_UP = 0,
    OP_PANTILT_DOWN,
    OP_PANTILT_LEFT,
    OP_PANTILT_RIGHT
};

enum DisplayOpcode {
    OP_DISPLAY_START = 0,
    OP_DISPLAY_PAUSE,
    OP_DISPLAY_RESUME,
};

class Control {
public:
    Control();
    ~Control();

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

#endif // __CONTROL_H__
