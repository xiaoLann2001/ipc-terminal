#include "Led.h"

/**
 * @brief LED 类构造函数。
 * 
 * @param led_num LED 编号。
 */
Led::Led(enum Led_num led_num) : led_num_(led_num), on_time_(0), off_time_(0), blink_frequency_(1), mode_("manual") {
    // 初始化 GPIO
    if (gpio_init((Gpio_num)led_num_, GPIO_OUTPUT) == -1) {
        LOG_ERROR("Failed to initialize LED: ", led_num_ );
    } else {
        gpio_write((Gpio_num)led_num_, 1);  // 初始状态关闭LED
        LOG_DEBUG("LED %d initialized\n", led_num_);
    }

    // 读取用户配置参数
    mode_ = rk_param_get_string("led:mode", "manual");
    if (mode_ == "schedule") {
        on_time_ = rk_param_get_int("led:on_time", 0);
        off_time_ = rk_param_get_int("led:off_time", 0);
    }
    blink_frequency_ = rk_param_get_int("led:blink_frequency", 1);

    // 校验 blink_frequency_，确保在合理范围内
    if (blink_frequency_ < 1 || blink_frequency_ > 3) {
        blink_frequency_ = 1;  // 设置为默认值
    }
}

/**
 * @brief LED 类析构函数。
 */
Led::~Led() {
    off();  // 关闭 LED
    gpio_deinit((Gpio_num)led_num_);
}

/**
 * @brief 打开 LED。
 */
void Led::on() {
    gpio_write((Gpio_num)led_num_, 0);  // 设置 GPIO 为低电平（开启 LED）
}

/**
 * @brief 关闭 LED。
 */
void Led::off() {
    gpio_write((Gpio_num)led_num_, 1);  // 设置 GPIO 为高电平（关闭 LED）
}

/**
 * @brief 切换 LED 状态。
 */
void Led::toggle() {
    gpio_toggle((Gpio_num)led_num_);  // 反转当前 LED 状态
}

/**
 * @brief 闪烁 LED。
 * 
 * @param duration_s 闪烁持续时间（秒）。
 */
void Led::blink(int duration_s) {
    std::lock_guard<std::mutex> lock(led_mutex_);

    // 确保 blink_frequency_ 在合理范围内
    if (blink_frequency_ < 1 || blink_frequency_ > 3) {
        blink_frequency_ = 1;
    }

    // 限制 duration_s 在 1 到 5 秒之间
    if (duration_s <= 0 || duration_s > 5) {
        duration_s = 1;
    }

    int blink_num = duration_s * blink_frequency_;
    while (blink_num--) {
        on();
        usleep(500000 / blink_frequency_);
        off();
        usleep(500000 / blink_frequency_);
    }
}

/**
 * @brief 设置 LED 工作模式。
 * 
 * @param mode 工作模式。
 */
void Led::set_mode(const char* mode) {
    std::lock_guard<std::mutex> lock(led_mutex_);
    mode_ = mode;
    rk_param_set_string("led:mode", mode_);
}

/**
 * @brief 设置点亮时间。
 * 
 * @param on_time 点亮时间（毫秒）。
 */
void Led::set_on_time(int on_time) {
    std::lock_guard<std::mutex> lock(led_mutex_);
    on_time_ = on_time;
    rk_param_set_int("led:on_time", on_time_);
}

/**
 * @brief 设置熄灭时间。
 * 
 * @param off_time 熄灭时间（毫秒）。
 */
void Led::set_off_time(int off_time) {
    std::lock_guard<std::mutex> lock(led_mutex_);
    off_time_ = off_time;
    rk_param_set_int("led:off_time", off_time_);
}

/**
 * @brief 设置闪烁频率。
 * 
 * @param blink_frequency 闪烁频率。
 */
void Led::set_blink_frequency(int blink_frequency) {
    std::lock_guard<std::mutex> lock(led_mutex_);
    if (blink_frequency >= 1 && blink_frequency <= 3) {
        blink_frequency_ = blink_frequency;
        rk_param_set_int("led:blink_frequency", blink_frequency_);
    } else {
        LOG_WARN("Invalid blink frequency: %d\n", blink_frequency);
        blink_frequency_ = 1;  // 设置为默认值
    }
}
