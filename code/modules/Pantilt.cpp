#include "Pantilt.h"

/**
 * @brief Pantilt 类构造函数。
 */
Pantilt::Pantilt() {
    // 初始化 PWM8 和 PWM9
    pwm_init(PWM8_M1);  // 用于控制俯仰的180°舵机
    pwm_set_period(PWM8_M1, 20000000);  // 设置 PWM8 周期为 20ms
    pwm_set_duty_cycle(PWM8_M1, 1500000);  // 设置 PWM8 初始占空比为 1.5ms
    pwm_set_polarity(PWM8_M1, "normal");  // 设置 PWM8 输出极性为正常
    // pwm_enable(PWM8_M1, 1);  // 启用 PWM8

    pwm_init(PWM9_M1);  // 用于控制旋转的180°舵机
    pwm_set_period(PWM9_M1, 20000000);  // 设置 PWM9 周期为 20ms
    pwm_set_duty_cycle(PWM9_M1, 1500000);  // 设置 PWM9 初始占空比为 1.5ms
    pwm_set_polarity(PWM9_M1, "normal");  // 设置 PWM9 输出极性为正常
    // pwm_enable(PWM9_M1, 1);  // 启用 PWM9

    preset_pan = rk_param_get_int("ptz:preset_pan", 0);  // 读取俯仰预置点
    preset_tilt = rk_param_get_int("ptz:preset_tilt", 0);  // 读取旋转预置点
    preset_home_enable = rk_param_get_int("ptz:preset_home_enable", 0);  // 读取回位使能
    preset_home_time = rk_param_get_int("ptz:preset_home_time", 30);  // 读取回位时间

    if (preset_pan < left_limit || preset_pan > right_limit) {
        preset_pan = 0;
    }
    if (preset_tilt < up_limit || preset_tilt > down_limit) {
        preset_tilt = 0;
    }
    if (preset_home_time < 0) {
        preset_home_time = 0;
    }

    reset();  // 初始化舵机角度

    // 启动归位检查线程
    if (preset_home_enable) {
        home_position_thread_run = true;
        home_position_thread = std::thread(&Pantilt::homePositionCheck, this);
    }
}

/**
 * @brief Pantilt 类析构函数。
 */
Pantilt::~Pantilt() {
    reset();  // 复位舵机

    pwm_enable(PWM8_M1, 0);  // 禁用 PWM8
    pwm_deinit(PWM8_M1);

    pwm_enable(PWM9_M1, 0);  // 禁用 PWM9
    pwm_deinit(PWM9_M1);

    home_position_thread_run = false;
    cv_home_position.notify_all();

    if (home_position_thread.joinable()) {
        home_position_thread.join();
    }

    LOG_DEBUG("Pantilt module deinitialized\n");
}

/**
 * @brief 俯仰角上抬。
 */
void Pantilt::up(int step) {
    if (step < 0 ) {
        LOG_ERROR("step should be positive\n");
        return;
    }
    if (step > tilt_step_limit) {
        step = tilt_step_limit;
    }
    setTilt(tilt_angle - step);
    LOG_DEBUG("tilt_angle: %d\n", tilt_angle);
}

/**
 * @brief 俯仰角下视。
 */
void Pantilt::down(int step) {
    if (step < 0 ) {
        LOG_ERROR("step should be positive\n");
        return;
    }
    if (step > tilt_step_limit) {
        step = tilt_step_limit;
    }
    setTilt(tilt_angle + step);
    LOG_DEBUG("tilt_angle: %d\n", tilt_angle);
}

/**
 * @brief 左转。
 */
void Pantilt::left(int step) {
    if (step < 0 ) {
        LOG_ERROR("step should be positive\n");
        return;
    }
    if (step > pan_step_limit) {
        step = pan_step_limit;
    }
    setPan(pan_angle + step);
    LOG_DEBUG("pan_angle: %d\n", pan_angle);
}

/**
 * @brief 右转。
 */
void Pantilt::right(int step) {
    if (step < 0 ) {
        LOG_ERROR("step should be positive\n");
        return;
    }
    if (step > pan_step_limit) {
        step = pan_step_limit;
    }
    setPan(pan_angle - step);
    LOG_DEBUG("pan_angle: %d\n", pan_angle);
}

/**
 * @brief 调整云台角度。
 * 
 * @param delta_pan 旋转角度变化。
 * @param delta_tilt 俯仰角度变化。
 */
void Pantilt::onAjustPantilt(int delta_pan, int delta_tilt) {
    // 限制旋转步长
    if (delta_pan < -pan_step_limit) {
        delta_pan = -pan_step_limit;
    } else if (delta_pan > pan_step_limit) {
        delta_pan = pan_step_limit;
    }
    // 限制俯仰步长
    if (delta_tilt < -tilt_step_limit) {
        delta_tilt = -tilt_step_limit;
    } else if (delta_tilt > tilt_step_limit) {
        delta_tilt = tilt_step_limit;
    }

    int pan = pan_angle - delta_pan;
    int tilt = tilt_angle + delta_tilt;

    setPan(pan);
    setTilt(tilt);
}

/**
 * @brief 复位舵机。
 */
void Pantilt::reset() {
    // LOG_DEBUG("reset\n");

    setPan(preset_pan);
    setTilt(preset_tilt);

    has_operation = true;
    cv_home_position.notify_all();
}

/**
 * @brief Onvif 调用接口，相对移动
*/
void Pantilt::relativeMove(float delta_x, float delta_y) {
    // 浮点数转换为角度
    int delta_pan = static_cast<int>(delta_x * (right_limit - left_limit) / 2);
    int delta_tilt = static_cast<int>(delta_y * (down_limit - up_limit) / 2);

    // 限制旋转步长
    if (delta_pan < -pan_step_limit) {
        delta_pan = -pan_step_limit;
    } else if (delta_pan > pan_step_limit) {
        delta_pan = pan_step_limit;
    }
    // 限制俯仰步长
    if (delta_tilt < -tilt_step_limit) {
        delta_tilt = -tilt_step_limit;
    } else if (delta_tilt > tilt_step_limit) {
        delta_tilt = tilt_step_limit;
    }

    int pan = pan_angle - delta_pan;
    int tilt = tilt_angle + delta_tilt;

    setPan(pan);
    setTilt(tilt);
}

/**
 * @brief 设置俯仰角度。
 * 
 * @param target_angle 目标角度。
 */
void Pantilt::setTilt(int target_angle) {
    if (tilt_angle == target_angle) return;  // 如果目标角度不变，跳过设置

    // 限制目标角度在 -60 到 90 范围内
    if (target_angle < up_limit) {
        target_angle = up_limit;
    } else if (target_angle > down_limit) {
        target_angle = down_limit;
    }
    unsigned int duty_cycle_ns = mapAngleToDutyCycle(target_angle + 90, 180);  // 将角度映射为占空比

    // pwm_set_period(PWM8_M1, period_ns);
    pwm_set_duty_cycle(PWM8_M1, duty_cycle_ns);
    pwm_enable(PWM8_M1, 1);  // 启用 PWM8
    tilt_angle = target_angle;

    has_operation = true;
    cv_home_position.notify_all();
}

// /**
//  * @brief 设置旋转角度（360度舵机）。
//  * 
//  * @param target_angle 目标角度。
//  */
// void Pantilt::setPan(int target_angle) {
//     // 限制目标角度在 -180 到 180 范围内
//     if (target_angle < -180) {
//         target_angle = -180;
//     } else if (target_angle > 180) {
//         target_angle = 180;
//     }

//     unsigned int duty_cycle_ns;
//     int angle_diff = target_angle - pan_angle;  // 计算目标角度与当前角度的差值

//     // 根据角度差设置旋转方向
//     if (angle_diff < 0) {  // 逆时针旋转
//         duty_cycle_ns = 1000000;  // 对应逆时针旋转的 PWM 占空比
//     } else if (angle_diff > 0) {  // 顺时针旋转
//         duty_cycle_ns = 2000000;  // 对应顺时针旋转的 PWM 占空比
//     } else {
//         return;  // 角度相同，不需要旋转
//     }

//     // 计算需要旋转的时间，设定旋转速度大约为1.078秒一圈
//     float rotation_time = (abs(angle_diff) / 360.0) * 1.078 * 1000000;  // us

//     // 设置 PWM 占空比，启用 PWM9
//     pwm_set_duty_cycle(PWM9_M1, duty_cycle_ns);

//     // 让舵机旋转相应的时间
//     usleep(rotation_time);  // 微秒级延时

//     // 停止旋转
//     pwm_set_duty_cycle(PWM9_M1, 1500000);  // 设置为 1.5ms 停止旋转
//     pan_angle = target_angle;
// }

/**
 * @brief 设置旋转角度（180度舵机）。
 * 
 * @param target_angle 目标角度。
 */
void Pantilt::setPan(int target_angle) {
    if (pan_angle == target_angle) return;  // 如果目标角度不变，跳过设置

    // 限制目标角度在 -90 到 90 范围内
    if (target_angle < left_limit) {
        target_angle = left_limit;
    } else if (target_angle > right_limit) {
        target_angle = right_limit;
    }
    unsigned int duty_cycle_ns = mapAngleToDutyCycle(target_angle + 90, 180);  // 将角度映射为占空比

    pwm_set_duty_cycle(PWM9_M1, duty_cycle_ns);
    pwm_enable(PWM9_M1, 1);  // 启用 PWM9
    pan_angle = target_angle;

    has_operation = true;
    cv_home_position.notify_all();
}

/**
 * @brief 将角度映射为 PWM 占空比。
 * 
 * @param angle 角度。
 * @param max_angle 最大角度。
 * @return unsigned int PWM 占空比。
 */
unsigned int Pantilt::mapAngleToDutyCycle(int angle, int max_angle) {
    return min_duty_cycle_ns + (angle * (max_duty_cycle_ns - min_duty_cycle_ns)) / max_angle;
}

void Pantilt::homePositionCheck() {
    while (home_position_thread_run) {
        // 使用 std::unique_lock 和 condition_variable 等待条件满足
        std::unique_lock<std::mutex> lock(mtx_home_position);

        // 如果当前不在归位位置，等待 preset_home_time 秒，检查线程退出或是否已归位
        cv_home_position.wait_for(lock, std::chrono::seconds(preset_home_time), [this] {
            return !home_position_thread_run || has_operation;  // 等待条件：线程退出或有操作
        });

        // 如果线程被通知退出，结束线程
        if (!home_position_thread_run) {
            break;
        }

        // 若有操作，重置 has_operation 标志
        if (has_operation) {
            has_operation = false;
            // LOG_DEBUG("has operation\n");
            continue;
        }

        // 如果不在归位位置，执行归位操作
        if (pan_angle != preset_pan || tilt_angle != preset_tilt) {
            // LOG_DEBUG("pan_angle: %d, tilt_angle: %d\n", pan_angle, tilt_angle);
            // LOG_DEBUG("preset_pan: %d, preset_tilt: %d\n", preset_pan, preset_tilt);
            reset();  // 执行归位  
        }
    }
}
