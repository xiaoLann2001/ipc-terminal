#include "Control/Control.h"

/**
 * @brief 控制类构造函数
 */
Control::Control() {
    init();
    led_init(LED0);
    led_init(LED1);

    // 显示类初始化
    display = new Display();

    // 视频类初始化
    video = new Video(1920, 1080);

    video->signal_video_frame.connect(display, &Display::push_frame);
}

Control::~Control() {
    led_deinit(LED0);
    led_deinit(LED1);

    delete display;
    delete video;
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
        std::bind(&Control::handlePantiltControl, this, OP_PANTILT_RIGHT, std::placeholders::_1)
    };

    // 初始化 DISPLAY 模块控制函数
    display_control_functions = {
        std::bind(&Control::handleDisplayControl, this, OP_DISPLAY_START, std::placeholders::_1),
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
        case ID_LED:
            if (signal.opcode != OP_LED_ON && 
                signal.opcode != OP_LED_OFF &&
                signal.opcode != OP_LED_TOGGLE &&
                signal.opcode != OP_LED_BLINK
                ) {
                std::cerr << "Unknown LED opcode: " << signal.opcode << std::endl;
            } else if (signal.param != PA_LED0 && 
                       signal.param != PA_LED1) {
                std::cerr << "Unknown LED param: " << signal.param << std::endl;
            }
            break;
        case ID_PANTILT:
            if (signal.opcode != OP_PANTILT_UP && 
                signal.opcode != OP_PANTILT_DOWN &&
                signal.opcode != OP_PANTILT_LEFT && 
                signal.opcode != OP_PANTILT_RIGHT
                ) {
                std::cerr << "Unknown pantilt opcode: " << signal.opcode << std::endl;
            }
            break;
        case ID_VIDEO:
            // 暂时不处理视频模块的操作
            break;
        case ID_DISPLAY:
            if (signal.opcode != OP_DISPLAY_START && 
                signal.opcode != OP_DISPLAY_PAUSE &&
                signal.opcode != OP_DISPLAY_RESUME
                ) {
                std::cerr << "Unknown display opcode: " << signal.opcode << std::endl;
            }
            break;
        default:
            std::cerr << "Unknown module ID: " << signal.module_id << std::endl;
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
            if (signal.opcode >= 0 && signal.opcode < led_control_functions.size()) {
                led_control_functions[signal.opcode](signal.param);
            } else {
                std::cerr << "Invalid LED opcode: " << signal.opcode << std::endl;
            }
            break;
        
        case ID_PANTILT:
            if (signal.opcode >= 0 && signal.opcode < pantilt_control_functions.size()) {
                pantilt_control_functions[signal.opcode](signal.param);
            } else {
                std::cerr << "Invalid Pantilt opcode: " << signal.opcode << std::endl;
            }
            break;

        case ID_DISPLAY:
            if (signal.opcode >= 0 && signal.opcode < display_control_functions.size()) {
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
            std::cout << "Pantilt moved UP" << std::endl;
            break;
        case OP_PANTILT_DOWN:
            std::cout << "Pantilt moved DOWN" << std::endl;
            break;
        case OP_PANTILT_LEFT:
            std::cout << "Pantilt moved LEFT" << std::endl;
            break;
        case OP_PANTILT_RIGHT:
            std::cout << "Pantilt moved RIGHT" << std::endl;
            break;
        default:
            std::cerr << "Unknown Pantilt opcode: " << opcode << std::endl;
    }
}

// 处理 Display 控制
void Control::handleDisplayControl(int opcode, int param) {
    switch (opcode) {
        case OP_DISPLAY_START:
            std::cout << "Display started" << std::endl;
            break;
        case OP_DISPLAY_PAUSE:
            std::cout << "Display paused" << std::endl;
            break;
        case OP_DISPLAY_RESUME:
            std::cout << "Display resumed" << std::endl;
            break;
        default:
            std::cerr << "Unknown Display opcode: " << opcode << std::endl;
    }
}

