#ifndef __PWM_H__
#define __PWM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#define RV1106_MAX

#define PWM8_PERIOD 20000000    // 20ms
#define PWM8_MIN    500000      // 0.5ms 向上
#define PWM8_MID    1500000     // 1.5ms 复位
#define PWM8_MAX    2500000     // 2.5ms 向下

#define PWM9_PERIOD 20000000    // 20ms
#define PWM9_MIN    500000      // 0.5ms 向上
#define PWM9_MID    1500000     // 1.5ms 复位
#define PWM9_MAX    2500000     // 2.5ms 向下

#ifdef RV1106_MAX
enum PWM_num {
    PWM8_M1 = 8,
    PWM9_M1 = 9,
};
#endif  // RV1106_MAX

// 定义 PWM 状态结构体
struct Pwm_status {
    unsigned int period_ns;      // PWM 周期（纳秒）
    char polarity[8];            // PWM 输出极性
    unsigned int duty_cycle_ns;  // PWM 占空比（纳秒）
    int enabled;                 // PWM 是否启用
};

/**
 * @brief 生成 PWM 文件路径.
 *
 * 此函数根据给定的 PWM 通道号生成 PWM 操作文件的路径.
 *
 * @param path 用于存储生成的路径的缓冲区.
 * @param pwm_num PWM 通道号.
 * @param filename 需要拼接的文件名.
 */
void pwm_generate_path(char *path, int pwm_num, const char *filename);

/**
 * @brief 初始化 PWM 通道.
 *
 * 此函数导出指定的 PWM 通道，并为后续操作做好准备.
 *
 * @param pwm_num PWM 通道号.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_init(enum PWM_num pwm_num);

/**
 * @brief 释放 PWM 通道.
 *
 * 此函数取消导出 PWM 通道，释放资源.
 *
 * @param pwm_num PWM 通道号.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_deinit(enum PWM_num pwm_num);

/**
 * @brief 设置 PWM 周期.
 *
 * 设置指定 PWM 通道的周期（单位：纳秒）.
 *
 * @param pwm_num PWM 通道号.
 * @param period_ns PWM 周期（纳秒）.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_set_period(enum PWM_num pwm_num, unsigned int period_ns);

/**
 * @brief 设置 PWM 占空比.
 *
 * 设置指定 PWM 通道的占空比（单位：纳秒）.
 *
 * @param pwm_num PWM 通道号.
 * @param duty_cycle_ns PWM 占空比（纳秒）.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_set_duty_cycle(enum PWM_num pwm_num, unsigned int duty_cycle_ns);

/**
 * @brief 设置 PWM 输出极性.
 *
 * 设置指定 PWM 通道的输出极性，可选值为 "normal" 或 "inverted".
 *
 * @param pwm_num PWM 通道号.
 * @param polarity 输出极性，可选值为 "normal" 或 "inverted".
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_set_polarity(enum PWM_num pwm_num, const char* polarity);

/**
 * @brief 启动PWM 输出.
 *
 * 此函数用于启用指定的 PWM 通道.
 *
 * @param pwm_num PWM 通道号.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_enable(enum PWM_num pwm_num);

/**
 * @brief 停止 PWM 输出.
 *
 * 此函数用于禁用指定的 PWM 通道.
 *
 * @param pwm_num PWM 通道号.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_disable(enum PWM_num pwm_num);

/**
 * @brief 获取 PWM 通道的状态.
 *
 * 读取 PWM 通道的当前周期、占空比和启用状态.
 *
 * @param pwm_num PWM 通道号.
 * @param status 指向 Pwm_status 结构体的指针，用于存储读取到的状态信息.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_get_status(enum PWM_num pwm_num, struct Pwm_status *status);

#ifdef __cplusplus
}
#endif

#endif // __PWM_H__
