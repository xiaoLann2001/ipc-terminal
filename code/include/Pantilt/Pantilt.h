#ifndef PANTILT_H
#define PANTILT_H

class Pantilt {
public:
    Pantilt();
    ~Pantilt();

    void up();

    void down();

    void left();

    void right();

    void reset();

private:
    // 当前Pan角度，范围：-180°~180°，角度大于0表示向左，小于0表示向右
    int pan_angle = 0;
    // 当前Tilt角度，范围：-45°~90°，角度大于0表示俯视，小于0表示仰视
    int tilt_angle = 0;

    const int up_limit = -45;
    const int down_limit = 90;
    const int left_limit = -180;
    const int right_limit = 180;
    const int min_duty_cycle_ns = 500000;
    const int max_duty_cycle_ns = 2500000;

    // 设置俯仰角度
    void setTilt(int angle);

    // 设置旋转角度
    void setPan(int angle);

    // 辅助函数：将角度映射为 PWM 占空比
    unsigned int mapAngleToDutyCycle(int angle, int max_angle);
};

#endif // PANTILT_H
