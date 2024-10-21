#include "Control/Control.h"

/**
 * @brief 控制类构造函数
 */
Control::Control() {
    init();

    // LED 模块初始化
    printf("LED module initialized\n");
    led_init(LED0);
    led_init(LED1);

    // Pantilt 模块初始化
    printf("Pantilt module initialized\n");
    pantilt = new Pantilt();

    // 显示类初始化
    printf("Display module initialized\n");
    display = new Display();

    // 视频类初始化
    printf("Video module initialized\n");
    video = new Video(720, 480);

    video->signal_video_frame.connect(display, &Display::push_frame);

    printf("Control module initialized\n");
}

Control::~Control() {
    led_deinit(LED0);
    led_deinit(LED1);

    delete display;
    delete video;
    delete pantilt;
}

/**
 * @brief 初始化控制模块
 */
void Control::init() {
    // 初始化 LED 模块控制函数
    led_control_functions = {
        std::bind(&Control::handleLEDControl, this, OP_LED_ON, std::placeholders::_1),
        std::bind(&Control::handleLEDControl, this, OP_LED_OFF, std::placeholders::_1),
        std::bind(&Control::handleLEDControl, this, OP_LED_TOGGLE, std::placeholders::_1),
        std::bind(&Control::handleLEDControl, this, OP_LED_BLINK, std::placeholders::_1)
    };

    // 初始化 PANTILT 模块控制函数
    pantilt_control_functions = {
        std::bind(&Control::handlePantiltControl, this, OP_PANTILT_UP, std::placeholders::_1),
        std::bind(&Control::handlePantiltControl, this, OP_PANTILT_DOWN, std::placeholders::_1),
        std::bind(&Control::handlePantiltControl, this, OP_PANTILT_LEFT, std::placeholders::_1),
        std::bind(&Control::handlePantiltControl, this, OP_PANTILT_RIGHT, std::placeholders::_1),
        std::bind(&Control::handlePantiltControl, this, OP_PANTILT_RESET, std::placeholders::_1)
    };

    // 初始化 DISPLAY 模块控制函数
    display_control_functions = {
        std::bind(&Control::handleDisplayControl, this, OP_DISPLAY_PAUSE, std::placeholders::_1),
        std::bind(&Control::handleDisplayControl, this, OP_DISPLAY_RESUME, std::placeholders::_1)
    };
}

/**
 * @brief 处理并解析接收到的控制信号
 * @param data 从网络接收到的数据
 * @return 解析后的ControlSignal结构
 */
ControlSignal Control::parseSignal(const std::string& data) {
    // 解析数据，格式为 "module_id opcode param"
    ControlSignal signal;
    std::istringstream ss(data);
    ss >> signal.module_id >> signal.opcode >> signal.param;

    // 判断解析的操作码是否合法
    switch (signal.module_id) {
        case ID_UNKNOWN:
            std::cerr << "Unknown module ID: " << signal.module_id << std::endl;
            break;
        case ID_LED:
            if (signal.opcode < 0 || signal.opcode >= OP_LED_MAX) {
                std::cerr << "Unknown LED opcode: " << signal.opcode << std::endl;
                signal.module_id = ID_UNKNOWN;
                if (signal.param < 0 || signal.param >= LEDParam::PA_LED_MAX) {
                    std::cerr << "Unknown LED param: " << signal.param << std::endl;
                    signal.module_id = ID_UNKNOWN;
                }  
            } 
            break;
        case ID_PANTILT:
            if (signal.opcode < 0 || signal.opcode >= OP_PANTILT_MAX) {
                std::cerr << "Unknown pantilt opcode: " << signal.opcode << std::endl;
                signal.module_id = ID_UNKNOWN;
            }
            break;
        case ID_VIDEO:
            // 暂时不处理视频模块的操作
            break;
        case ID_DISPLAY:
            if (signal.opcode < 0 || signal.opcode >= OP_DISPLAY_MAX) {
                std::cerr << "Unknown display opcode: " << signal.opcode << std::endl;
                signal.module_id = ID_UNKNOWN;
            }
            break;
        default:
            std::cerr << "Unknown module ID: " << signal.module_id << std::endl;
            signal.module_id = ID_UNKNOWN;
    }
    return signal;
}

/**
 * @brief 分发控制信号到相应的模块
 * @param signal 已解析的控制信号
 */
void Control::dispatchSignal(const ControlSignal& signal) {
    switch (signal.module_id) {
        case ID_LED:
            if (signal.opcode >= 0 && signal.opcode < OP_LED_MAX) {
                if (signal.param >= 0 && signal.param < LEDParam::PA_LED_MAX) {
                    led_control_functions[signal.opcode](signal.param);
                }
            } else {
                std::cerr << "Invalid LED opcode: " << signal.opcode << std::endl;
            }
            break;
        
        case ID_PANTILT:
            if (signal.opcode >= 0 && signal.opcode < OP_PANTILT_MAX) {
                pantilt_control_functions[signal.opcode](signal.param);
            } else {
                std::cerr << "Invalid Pantilt opcode: " << signal.opcode << std::endl;
            }
            break;

        case ID_DISPLAY:
            if (signal.opcode >= 0 && signal.opcode < OP_DISPLAY_MAX) {
                display_control_functions[signal.opcode](signal.param);
            } else {
                std::cerr << "Invalid Display opcode: " << signal.opcode << std::endl;
            }
            break;

        default:
            std::cerr << "Unknown module ID: " << signal.module_id << std::endl;
    }
}

// 接收网络数据槽函数
void Control::onNetworkReceived(const std::string& data) {
    std::cout << "Received: " << data << std::endl;
    ControlSignal signal = parseSignal(data);
    std::cout << "Parsed signal: module_id=" << signal.module_id
            << ", opcode=" << signal.opcode
            << ", param=" << signal.param << std::endl;
    if (signal.module_id == ID_UNKNOWN) return;
    dispatchSignal(signal);
}

// 处理 LED 控制
void Control::handleLEDControl(int opcode, int param) {
    int num = LED0 + param;
    switch (opcode) {
        case OP_LED_ON:
            std::cout << "LED" << param << " is turned ON" << std::endl;
            led_on((Led_num)num);  // 调用实际的 LED 控制函数，传递具体灯号
            break;
        case OP_LED_OFF:
            std::cout << "LED" << param << " is turned OFF" << std::endl;
            led_off((Led_num)num);
            break;
        case OP_LED_TOGGLE:
            std::cout << "LED" << param << " toggled" << std::endl;
            led_toggle((Led_num)num);
            break;
        case OP_LED_BLINK:
            std::cout << "LED" << param << " is blinking" << std::endl;
            led_blink((Led_num)num, 500000);
            break;
        default:
            std::cerr << "Unknown LED opcode: " << opcode << std::endl;
    }
}

// 处理 Pantilt 控制
void Control::handlePantiltControl(int opcode, int param) {
    switch (opcode) {
        case OP_PANTILT_UP:
            pantilt->up();
            std::cout << "Pantilt moved UP" << std::endl;
            break;
        case OP_PANTILT_DOWN:
            pantilt->down();
            std::cout << "Pantilt moved DOWN" << std::endl;
            break;
        case OP_PANTILT_LEFT:
            pantilt->left();
            std::cout << "Pantilt moved LEFT" << std::endl;
            break;
        case OP_PANTILT_RIGHT:
            pantilt->right();
            std::cout << "Pantilt moved RIGHT" << std::endl;
            break;
        case OP_PANTILT_RESET:
            pantilt->reset();
            std::cout << "Pantilt moved RESET" << std::endl;
            break;
        default:
            std::cerr << "Unknown Pantilt opcode: " << opcode << std::endl;
    }
}

// 处理 Display 控制
void Control::handleDisplayControl(int opcode, int param) {
    switch (opcode) {
        case OP_DISPLAY_PAUSE:
            display->pause_display();
            std::cout << "Display paused" << std::endl;
            break;
        case OP_DISPLAY_RESUME:
            display->resume_display();
            std::cout << "Display resumed" << std::endl;
            break;
        default:
            std::cerr << "Unknown Display opcode: " << opcode << std::endl;
    }
}

