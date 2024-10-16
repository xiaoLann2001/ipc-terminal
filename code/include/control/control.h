#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int module_id;
    int opcode;
    int param;
} ControlSignal;

/**
 * @brief 初始化控制模块
 */
void control_init();

/**
 * @brief 处理并解析接收到的控制信号
 * @param data 从网络接收到的数据
 * @return 解析后的ControlSignal结构
 */
ControlSignal control_parse_signal(const char *data);

/**
 * @brief 分发控制信号到相应的模块
 * @param signal 已解析的控制信号
 */
void control_dispatch_signal(const ControlSignal *signal);

#endif // CONTROL_H
