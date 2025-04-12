// Led.h
#pragma once

#include <mutex>

#include "global.h"
#include "gpio.h"

// LED Resources
enum Led_num {
    LED0 = GPIO1_C6_d,
    LED1 = GPIO1_C7_d,
};

class Led {
public:
    explicit Led(enum Led_num led_num);

    ~Led();

    void on();

    void off();

    void toggle();

    void blink(int duration_s);

    void set_mode(const char* mode);

    void set_on_time(int on_time);

    void set_off_time(int off_time);

    void set_blink_frequency(int blink_frequency);

private:
    enum Led_num led_num_;  // LED 对应的 GPIO 号
    
    // 用户配置参数
    const char* mode_;
    int on_time_;
    int off_time_;
    int blink_frequency_;

    // 互斥锁保护参数
    std::mutex led_mutex_;
};
