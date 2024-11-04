#include "Control/Control.h"

/**
 * @brief 控制类构造函数
 */
Control::Control() {

}

/**
 * @brief 控制类析构函数
 */
Control::~Control() {
    LOG_DEBUG("Control module deinitializing\n");
}

/**
 * @brief 注册控制函数
 * 
 * @param module_id 模块 ID
 * @param opcode 操作码
 * @param func 控制函数
 */
void Control::registerControlFunction(int module_id, int opcode, std::function<void(int)> func) {
    control_functions[module_id][opcode] = func;
}

/**
 * @brief 处理并解析接收到的控制信号
 * 
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
 * 
 * @param signal 已解析的控制信号
 */
void Control::dispatchSignal(const ControlSignal& signal) {
    int module_id = signal.module_id;
    int opcode = signal.opcode;
    int param = signal.param;
    if (control_functions.count(module_id) && control_functions[module_id].count(opcode)) {
        control_functions[module_id][opcode](param);
    } else {
        LOG_ERROR("Unknown control signal: module_id=%d, opcode=%d, param=%d\n", module_id, opcode, param);
    }
}

/**
 * @brief 接收网络数据槽函数
 * 
 * @param data 从网络接收到的数据
 */
void Control::onNetworkReceived(const std::string& data) {
    std::cout << "Received: " << data << std::endl;
    ControlSignal signal = parseSignal(data);
    std::cout << "Parsed signal: module_id=" << signal.module_id
            << ", opcode=" << signal.opcode
            << ", param=" << signal.param << std::endl;
    if (signal.module_id == ID_UNKNOWN) return;
    dispatchSignal(signal);
}
