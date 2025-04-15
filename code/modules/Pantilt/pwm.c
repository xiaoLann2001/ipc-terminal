#include "pwm.h"

#define SYSFS_PWM_PATH "/sys/class/pwm/pwmchip"

/**
 * @brief 写入 sysfs 文件。
 * 
 * @param path 文件路径。
 * @param value 要写入的值。
 * @return 成功返回 0，失败返回 -1。
 */
static int sysfs_write(const char *path, const char *value) {
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open sysfs file");
        return -1;
    }
    write(fd, value, strlen(value));
    close(fd);
    return 0;
}

/**
 * @brief 读取 sysfs 文件。
 * 
 * @param path 文件路径。
 * @param value 读取的值。
 * @param size 读取的大小。
 * @return 成功返回 0，失败返回 -1。
 */
static int sysfs_read(const char *path, char *value, size_t size) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open sysfs file");
        return -1;
    }
    read(fd, value, size);
    close(fd);
    return 0;
}

/**
 * @brief 生成 PWM 文件路径.
 *
 * 此函数根据给定的 PWM 通道号生成 PWM 操作文件的路径.
 *
 * @param path 用于存储生成的路径的缓冲区.
 * @param pwm_num PWM 通道号.
 * @param filename 需要拼接的文件名.
 */
void pwm_generate_path(char *path, int pwm_num, const char *filename) {
    sprintf(path, "%s%d/pwm0/%s", SYSFS_PWM_PATH, pwm_num, filename);
}

/**
 * @brief 初始化 PWM 通道.
 *
 * 此函数导出指定的 PWM 通道，并为后续操作做好准备.
 *
 * @param pwm_num PWM 通道号.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_init(enum PWM_num pwm_num) {
    char path[64];
    
    // Check if PWM is already exported
    sprintf(path, "%s%d/pwm0", SYSFS_PWM_PATH, pwm_num);
    if (access(path, F_OK) == 0) {
        fprintf(stdout, "PWM%d already exported\n", pwm_num);
        return 0;
    }
    
    // Export the PWM
    sprintf(path, "%s%d/export", SYSFS_PWM_PATH, pwm_num);
    return sysfs_write(path, "0");
}

/**
 * @brief 释放 PWM 通道.
 *
 * 此函数取消导出 PWM 通道，释放资源.
 *
 * @param pwm_num PWM 通道号.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_deinit(enum PWM_num pwm_num) {
    char path[64];
    
    // Unexport the PWM
    sprintf(path, "%s%d/unexport", SYSFS_PWM_PATH, pwm_num);
    return sysfs_write(path, "0");
}

/**
 * @brief 设置 PWM 周期.
 *
 * 设置指定 PWM 通道的周期（单位：纳秒）.
 *
 * @param pwm_num PWM 通道号.
 * @param period_ns PWM 周期（纳秒）.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_set_period(enum PWM_num pwm_num, unsigned int period_ns) {
    char path[64];
    char value[32];
    
    // Write period to sysfs
    pwm_generate_path(path, pwm_num, "period");
    sprintf(value, "%u", period_ns);
    return sysfs_write(path, value);
}

/**
 * @brief 设置 PWM 占空比.
 *
 * 设置指定 PWM 通道的占空比（单位：纳秒）.
 *
 * @param pwm_num PWM 通道号.
 * @param duty_cycle_ns PWM 占空比（纳秒）.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_set_duty_cycle(enum PWM_num pwm_num, unsigned int duty_cycle_ns) {
    char path[64];
    char value[32];
    
    // Write duty cycle to sysfs
    pwm_generate_path(path, pwm_num, "duty_cycle");
    sprintf(value, "%u", duty_cycle_ns);
    return sysfs_write(path, value);
}

/**
 * @brief 设置 PWM 输出极性.
 *
 * 设置指定 PWM 通道的输出极性.
 *
 * @param pwm_num PWM 通道号.
 * @param polarity 输出极性（normal/inverted）.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_set_polarity(enum PWM_num pwm_num, const char* polarity) {
    char path[64];
    
    // Write polarity to sysfs
    pwm_generate_path(path, pwm_num, "polarity");
    return sysfs_write(path, polarity);
}

/**
 * @brief 启用 PWM 通道.
 *
 * @param pwm_num PWM 通道号.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_enable(enum PWM_num pwm_num) {
    char path[64];
    char value[8];
    
    // Write enable to sysfs
    pwm_generate_path(path, pwm_num, "enable");
    sprintf(value, "%d", 1);
    return sysfs_write(path, value);
}

/**
 * @brief 停止 PWM 输出.
 *
 * @param pwm_num PWM 通道号.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_disable(enum PWM_num pwm_num) {
    char path[64];
    char value[8];
    
    // Write disable to sysfs
    pwm_generate_path(path, pwm_num, "enable");
    sprintf(value, "%d", 0);
    return sysfs_write(path, value);
}

/**
 * @brief 获取 PWM 通道的状态.
 *
 * 读取 PWM 通道的当前周期、占空比和启用状态.
 *
 * @param pwm_num PWM 通道号.
 * @param status 保存 PWM 状态.
 * @return 成功返回 0，失败返回 -1.
 */
int pwm_get_status(enum PWM_num pwm_num, struct Pwm_status *status) {
    char path[64];
    char value[32];

    // Read period
    pwm_generate_path(path, pwm_num, "period");
    if (sysfs_read(path, value, sizeof(value)) == 0) {
        status->period_ns = atoi(value);
    } else {
        return -1;
    }

    // Read duty cycle
    pwm_generate_path(path, pwm_num, "duty_cycle");
    if (sysfs_read(path, value, sizeof(value)) == 0) {
        status->duty_cycle_ns = atoi(value);
    } else {
        return -1;
    }

    // Read enabled status
    pwm_generate_path(path, pwm_num, "enable");
    if (sysfs_read(path, value, sizeof(value)) == 0) {
        status->enabled = atoi(value);
    } else {
        return -1;
    }

    return 0;
}
