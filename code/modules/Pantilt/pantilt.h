#ifndef PANTILT_H
#define PANTILT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <pthread.h>

#define MAX_PRESET_NUM 8
#define MAX_PRESET_TOUR_NUM 4

// 云台的 PWM 编号
struct PtzPwmIndex {
    int x_pwm_index;    // 水平云台的 PWM 编号
    int y_pwm_index;    // 垂直云台的 PWM 编号
};

// 云台状态
struct PtzPosition {
    float x;            // 当前水平位置
    float y;            // 当前垂直位置
};

// 云台角度限制
struct PtzLimits {
    float x_min;        // 最小水平位置
    float x_max;        // 最大水平位置
    float y_min;        // 最小垂直位置
    float y_max;        // 最大垂直位置
};

// 移动速度
struct PtzSpeed {
    float x_speed;      // 水平移动速度
    float y_speed;      // 垂直移动速度
};

// 预置点结构体
struct PtzPreset {
    char token[64];     // 预置点唯一标识
    char name[64];      // 用户可读的名称
    float x;            // 预置点 x 值
    float y;            // 预置点 y 值
    bool valid;         // 是否已经被创建
};

// 预置点管理器
struct PtzPresetManager {
    int size;           // 预置点数量
    struct PtzPreset presets[MAX_PRESET_NUM];   // 预置点数组
};

struct Pantilt {
    struct PtzPwmIndex ptz_pwm_index;
    struct PtzPosition ptz_position;
    struct PtzLimits ptz_limits;
    struct PtzPresetManager ptz_preset_manager;
};

// 全局变量声明，默认在 pantilt.c 中定义
extern struct Pantilt g_pantilt;

int pantilt_init(struct Pantilt* pt);
int pantilt_deinit(struct Pantilt* pt);

void pantilt_relative_move(struct Pantilt* pt, float delta_x, float delta_y);
void pantilt_absolute_move(struct Pantilt* pt, float x, float y);
void pantilt_continuous_move(struct Pantilt* pt, float x_speed, float y_speed, int timeout);
void pantilt_stop(struct Pantilt* pt);

void pantilt_get_range(struct Pantilt* pt, float* x_min, float* x_max, float* y_min, float* y_max);

// void pantilt_goto_home_position(struct Pantilt* pt);
// void pantilt_set_home_position(struct Pantilt* pt, float x, float y);

// void pantilt_goto_preset(struct Pantilt* pt, int preset_id);
// void pantilt_get_preset(struct Pantilt* pt, float* x, float* y);
// void pantilt_set_preset(struct Pantilt* pt, float x, float y);
// void pantilt_remove_preset(Pantilt* pt);

// void pantilt_get_preset_tour(struct Pantilt* pt, int tour_id, int* preset_ids, int* size);
// void pantilt_modify_preset_tour(struct Pantilt* pt, int tour_id, int preset_id);
// void pantilt_remove_preset_tour(struct Pantilt* pt);
// void pantilt_move_and_start_tracking(struct Pantilt* pt);
// void pantilt_operate_preset_tour(struct Pantilt* pt);

void pantilt_get_status(struct Pantilt* pt, float* x, float* y);

#ifdef __cplusplus
}
#endif

#endif // PANTILT_H
