#include "pantilt.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#include "pwm.h"
#include "global.h"

/**
 * @brief 将 x 映射到 PWM 占空比范围.
*/
static inline unsigned int map_x_to_duty(float x, float x_min, float x_max, unsigned int min_duty, unsigned int max_duty) {
    // y 1.3ms
    // ^
    // |
    // .---> x 1.7ms ，向右占空比增加，向上占空比减少
    return (unsigned int)((x_max - x) / (x_max - x_min) * (max_duty - min_duty) + min_duty);
}

/**
 * @brief 将 y 映射到 PWM 占空比范围.
*/
static inline unsigned int map_y_to_duty(float y, float y_min, float y_max, unsigned int min_duty, unsigned int max_duty) {
    return (unsigned int)((y_max - y) / (y_max - y_min) * (max_duty - min_duty) + min_duty);
}

/**
 * @brief 初始化预置点.
 *
 * @param pt Pantilt 结构体指针.
 * @return 0 成功，-1 失败.
 */
static int pantilt_preset_init(struct Pantilt* pt) {
    if (pt == NULL) {
        LOG_ERROR("struct Pantilt* pointer is NULL\n");
        return -1;
    }
    
    // 初始化预置点
    for (int i = 0; i < MAX_PRESET_NUM; i++) {
        pt->ptz_preset_manager.presets[i].valid = false;
        snprintf(pt->ptz_preset_manager.presets[i].token, sizeof(pt->ptz_preset_manager.presets[i].token), "preset_%d", i);
        snprintf(pt->ptz_preset_manager.presets[i].name, sizeof(pt->ptz_preset_manager.presets[i].name), "Preset %d", i);
    }

    // 初始化预置点数量
    pt->ptz_preset_manager.size = 0;

    return 0;
}

/**
 * @brief 初始化云台.
 *
 * @param pt Pantilt 结构体指针.
 * @return 0 成功，-1 失败.
 */
int pantilt_init(struct Pantilt* pt) {
    if (pt == NULL) {
        LOG_ERROR("struct Pantilt* pointer is NULL\n");
        return -1;
    }
    
    // 初始化 PanTilt 结构体
    pt->ptz_pwm_index.x_pwm_index = PWM9_M1;
    pt->ptz_pwm_index.y_pwm_index = PWM8_M1;
    pt->ptz_position.x = 0;
    pt->ptz_position.y = 0;
    pt->ptz_limits.x_min = rk_param_get_int("ptz:pan_min", 0) / 90.0;
    pt->ptz_limits.x_max = rk_param_get_int("ptz:pan_max", 0) / 90.0;
    pt->ptz_limits.y_min = rk_param_get_int("ptz:tilt_min", 0) / 90.0;
    pt->ptz_limits.y_max = rk_param_get_int("ptz:tilt_max", 0) / 90.0;

    // 初始化 X轴 PWM
    if (pwm_init(pt->ptz_pwm_index.x_pwm_index) < 0) {
        LOG_ERROR("Failed to initialize PWM for x axis\n");
        return -1;
    }
    if (pwm_set_period(pt->ptz_pwm_index.x_pwm_index, PWM9_PERIOD) < 0) {
        LOG_ERROR("Failed to set PWM period for x axis\n");
        return -1;
    }
    if (pwm_set_duty_cycle(pt->ptz_pwm_index.x_pwm_index, PWM9_MID) < 0) {
        LOG_ERROR("Failed to set PWM duty cycle for x axis\n");
        return -1;
    }
    if (pwm_set_polarity(pt->ptz_pwm_index.x_pwm_index, "normal") < 0) {
        LOG_ERROR("Failed to set PWM polarity for x axis\n");
        return -1;
    }
    if (pwm_enable(pt->ptz_pwm_index.x_pwm_index) < 0) {
        LOG_ERROR("Failed to enable PWM for x axis\n");
        return -1;
    }

    // 初始化 Y 轴 PWM
    if (pwm_init(pt->ptz_pwm_index.y_pwm_index) < 0) {
        LOG_ERROR("Failed to initialize PWM for y axis\n");
        return -1;
    }
    if (pwm_set_period(pt->ptz_pwm_index.y_pwm_index, PWM8_PERIOD) < 0) {
        LOG_ERROR("Failed to set PWM period for y axis\n");
        return -1;
    }
    if (pwm_set_duty_cycle(pt->ptz_pwm_index.y_pwm_index, PWM8_MID) < 0) {
        LOG_ERROR("Failed to set PWM duty cycle for y axis\n");
        return -1;
    }
    if (pwm_set_polarity(pt->ptz_pwm_index.y_pwm_index, "normal") < 0) {
        LOG_ERROR("Failed to set PWM polarity for y axis\n");
        return -1;
    }
    if (pwm_enable(pt->ptz_pwm_index.y_pwm_index) < 0) {
        LOG_ERROR("Failed to enable PWM for y axis\n");
        return -1;
    }

    // 初始化预置点
    if (pantilt_preset_init(pt) < 0) {
        LOG_ERROR("Failed to initialize preset points\n");
        return -1;
    }
}

/**
 * @brief 释放云台资源.
 * 
 * @param pt Pantilt 结构体指针.
 * @return 0 成功，-1 失败.
 */
int pantilt_deinit(struct Pantilt* pt) {
    if (pt == NULL) {
        LOG_ERROR("struct Pantilt* pointer is NULL\n");
        return -1;
    }

    // 禁用 PWM
    if (pwm_disable(pt->ptz_pwm_index.x_pwm_index) < 0) {
        LOG_ERROR("Failed to disable PWM for x axis\n");
    }
    if (pwm_disable(pt->ptz_pwm_index.y_pwm_index) < 0) {
        LOG_ERROR("Failed to disable PWM for y axis\n");
    }

    // 释放 PWM
    if (pwm_deinit(pt->ptz_pwm_index.x_pwm_index) < 0) {
        LOG_ERROR("Failed to deinitialize PWM for x axis\n");
    }
    if (pwm_deinit(pt->ptz_pwm_index.y_pwm_index) < 0) {
        LOG_ERROR("Failed to deinitialize PWM for y axis\n");
    }

    return 0;
}

/**
 * @brief 相对移动云台
 */
void pantilt_relative_move(struct Pantilt* pt, float delta_x, float delta_y) {
    if (pt == NULL) {
        LOG_ERROR("struct Pantilt* pointer is NULL\n");
        return;
    }

    // 计算新的位置
    float new_x = pt->ptz_position.x + delta_x;
    float new_y = pt->ptz_position.y + delta_y;

    // 限制新的位置在范围内
    if (new_x < pt->ptz_limits.x_min) new_x = pt->ptz_limits.x_min;
    if (new_x > pt->ptz_limits.x_max) new_x = pt->ptz_limits.x_max;
    if (new_y < pt->ptz_limits.y_min) new_y = pt->ptz_limits.y_min;
    if (new_y > pt->ptz_limits.y_max) new_y = pt->ptz_limits.y_max;

    // 根据坐标求 PWM 占空比
    unsigned int x_duty = map_x_to_duty(new_x, pt->ptz_limits.x_min, pt->ptz_limits.x_max, PWM9_MIN, PWM9_MAX);
    unsigned int y_duty = map_y_to_duty(new_y, pt->ptz_limits.y_min, pt->ptz_limits.y_max, PWM8_MIN, PWM8_MAX);

    // 更新云台状态
    pt->ptz_position.x = new_x;
    pt->ptz_position.y = new_y;

    // 设置 PWM 占空比
    pwm_disable(pt->ptz_pwm_index.x_pwm_index);
    pwm_set_duty_cycle(pt->ptz_pwm_index.x_pwm_index, x_duty);
    pwm_set_duty_cycle(pt->ptz_pwm_index.y_pwm_index, y_duty);
    pwm_enable(pt->ptz_pwm_index.x_pwm_index);
}

void pantilt_absolute_move(struct Pantilt* pt, float x, float y) {
    if (pt == NULL) {
        LOG_ERROR("struct Pantilt* pointer is NULL\n");
        return;
    }

    // 限制新的位置在范围内
    if (x < pt->ptz_limits.x_min) x = pt->ptz_limits.x_min;
    if (x > pt->ptz_limits.x_max) x = pt->ptz_limits.x_max;
    if (y < pt->ptz_limits.y_min) y = pt->ptz_limits.y_min;
    if (y > pt->ptz_limits.y_max) y = pt->ptz_limits.y_max;

    // 根据坐标求 PWM 占空比
    unsigned int x_duty = map_x_to_duty(x, pt->ptz_limits.x_min, pt->ptz_limits.x_max, PWM9_MIN, PWM9_MAX);
    unsigned int y_duty = map_y_to_duty(y, pt->ptz_limits.y_min, pt->ptz_limits.y_max, PWM8_MIN, PWM8_MAX);

    // 更新云台状态
    pt->ptz_position.x = x;
    pt->ptz_position.y = y;

    // 设置 PWM 占空比
    pwm_disable(pt->ptz_pwm_index.x_pwm_index);
    pwm_set_duty_cycle(pt->ptz_pwm_index.x_pwm_index, x_duty);
    pwm_set_duty_cycle(pt->ptz_pwm_index.y_pwm_index, y_duty);
    pwm_enable(pt->ptz_pwm_index.x_pwm_index);
}

void pantilt_continuous_move(struct Pantilt* pt, float x_speed, float y_speed, int timeout) {

}

void pantilt_stop(struct Pantilt* pt) {

}



void pantilt_get_range(struct Pantilt* pt, float* x_min, float* x_max, float* y_min, float* y_max) {
    if (pt == NULL || x_min == NULL || x_max == NULL || y_min == NULL || y_max == NULL) {
        LOG_ERROR("pointer is NULL\n");
        return;
    }

    *x_min = pt->ptz_limits.x_min;
    *x_max = pt->ptz_limits.x_max;
    *y_min = pt->ptz_limits.y_min;
    *y_max = pt->ptz_limits.y_max;
}

void pantilt_get_status(struct Pantilt* pt, float* x, float* y) {
    if (pt == NULL || x == NULL || y == NULL) {
        LOG_ERROR("pointer is NULL\n");
        return;
    }

    *x = pt->ptz_position.x;
    *y = pt->ptz_position.y;
}




