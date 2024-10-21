#include "Pantilt/Pantilt.h"

extern "C" {
    #include "Pantilt/pwm.h"
}

Pantilt::Pantilt() {
    // 初始化 PWM8 和 PWM9
    pwm_init(PWM8_M1);  // 用于控制俯仰的180°舵机
    pwm_set_period(PWM8_M1, 20000000);  // 设置 PWM8 周期为 20ms
    pwm_set_duty_cycle(PWM8_M1, 1500000);  // 设置 PWM8 初始占空比为 1.5ms
    pwm_set_polarity(PWM8_M1, "normal");  // 设置 PWM8 输出极性为正常
    pwm_enable(PWM8_M1, 1);  // 启用 PWM8

    pwm_init(PWM9_M1);  // 用于控制旋转的360°舵机
    pwm_set_period(PWM9_M1, 20000000);  // 设置 PWM9 周期为 20ms
    pwm_set_duty_cycle(PWM9_M1, 1500000);  // 设置 PWM9 初始占空比为 1.5ms
    pwm_set_polarity(PWM9_M1, "normal");  // 设置 PWM9 输出极性为正常
    pwm_enable(PWM9_M1, 1);  // 启用 PWM9

    reset();  // 初始化舵机角度
}

Pantilt::~Pantilt() {
    reset();  // 复位舵机

    pwm_enable(PWM8_M1, 0);  // 禁用 PWM8
    pwm_deinit(PWM8_M1);

    pwm_enable(PWM9_M1, 0);  // 禁用 PWM9
    pwm_deinit(PWM9_M1);
}

void Pantilt::up() {
    setTilt(tilt_angle - 5);
    printf("tilt_angle: %d\n", tilt_angle);
}

void Pantilt::down() {
    setTilt(tilt_angle + 5);
    printf("tilt_angle: %d\n", tilt_angle);
}

void Pantilt::left() {
    setPan(pan_angle + 15);
    printf("pan_angle: %d\n", pan_angle);
}

void Pantilt::right() {
    setPan(pan_angle - 15);
    printf("pan_angle: %d\n", pan_angle);
}

void Pantilt::reset() {
    setPan(0);
    setTilt(0);
}

void Pantilt::setTilt(int target_angle) {
    // 限制目标角度在 -45 到 90 范围内
    if (target_angle < -45) {
        target_angle = -45;
    } else if (target_angle > 90) {
        target_angle = 90;
    }
    unsigned int duty_cycle_ns = mapAngleToDutyCycle(target_angle + 90, 180);  // 将角度映射为占空比

    // pwm_set_period(PWM8_M1, period_ns);
    pwm_set_duty_cycle(PWM8_M1, duty_cycle_ns);
    pwm_enable(PWM8_M1, 1);  // 启用 PWM8
    tilt_angle = target_angle;
}

void Pantilt::setPan(int target_angle) {
    // 限制目标角度在 -180 到 180 范围内
    if (target_angle < -180) {
        target_angle = -180;
    } else if (target_angle > 180) {
        target_angle = 180;
    }

    unsigned int duty_cycle_ns;
    int angle_diff = target_angle - pan_angle;  // 计算目标角度与当前角度的差值

    // 根据角度差设置旋转方向
    if (angle_diff < 0) {  // 逆时针旋转
        duty_cycle_ns = 1000000;  // 对应逆时针旋转的 PWM 占空比
    } else if (angle_diff > 0) {  // 顺时针旋转
        duty_cycle_ns = 2000000;  // 对应顺时针旋转的 PWM 占空比
    } else {
        return;  // 角度相同，不需要旋转
    }

    // 计算需要旋转的时间，设定旋转速度大约为1.078秒一圈
    float rotation_time = (abs(angle_diff) / 360.0) * 1.078 * 1000000;  // us

    // 设置 PWM 占空比，启用 PWM9
    pwm_set_duty_cycle(PWM9_M1, duty_cycle_ns);

    // 让舵机旋转相应的时间
    usleep(rotation_time);  // 微秒级延时

    // 停止旋转
    pwm_set_duty_cycle(PWM9_M1, 1500000);  // 设置为 1.5ms 停止旋转
    pan_angle = target_angle;
}

unsigned int Pantilt::mapAngleToDutyCycle(int angle, int max_angle) {
    return min_duty_cycle_ns + (angle * (max_duty_cycle_ns - min_duty_cycle_ns)) / max_angle;
}
