#pragma once

#include "global.h"

#include <thread>
#include <mutex>
#include <condition_variable>

extern "C" {
    #include "pwm.h"
}

class Pantilt {
public:
    Pantilt();
    ~Pantilt();

    void up(int step);

    void down(int step);

    void left(int step);

    void right(int step);

    void onAjustPantilt(int delta_pan, int delta_tilt);

    void reset();

    // Onvif interface
    void relativeMove(float delta_x, float delta_y);

private:
    // 当前Pan角度，范围：-180°~180°，角度大于0表示向左，小于0表示向右
    int pan_angle = 0;
    // 当前Tilt角度，范围：-45°~90°，角度大于0表示俯视，小于0表示仰视
    int tilt_angle = 0;

    const int pan_step_limit = 15;
    const int tilt_step_limit = 15;
    const int up_limit = -60;
    const int down_limit = 90;
    // const int left_limit = -180; // 360度舵机
    // const int right_limit = 180; // 360度舵机
    const int left_limit = -90; // 180度舵机
    const int right_limit = 90; // 180度舵机
    const int min_duty_cycle_ns = 500000;
    const int max_duty_cycle_ns = 2500000;

    // 云台预置点及回位时间
    int preset_pan = 0;
    int preset_tilt = 0;
    int preset_home_enable = 0;
    int preset_home_time = 0;

    // 是否有操作
    bool has_operation = false;  

    // 线程：检查云台是否在归位位置
    bool home_position_thread_run = true;
    std::mutex mtx_home_position;
    std::condition_variable cv_home_position;
    std::thread home_position_thread;

    // 设置俯仰角度
    void setTilt(int angle);

    // 设置旋转角度
    void setPan(int angle);

    // 检查并自动归位
    void homePositionCheck();

    // 辅助函数：将角度映射为 PWM 占空比
    unsigned int mapAngleToDutyCycle(int angle, int max_angle);
};
