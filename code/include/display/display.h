#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#ifdef __cplusplus
extern "C" {
#endif

// 初始化 display 模块
int display_init(void);

// 释放 display 模块资源
void display_deinit(void);

// 在屏幕上显示图像
int display_show_image(const char *image_path, int x, int y);

// 绘制文本
int display_draw_text(const char *text, int x, int y, uint32_t color);

// 绘制图形，比如矩形或线条
int display_draw_rectangle(int x, int y, int width, int height, uint32_t color);

// 刷新屏幕，确保所有绘图操作生效
int display_refresh(void);

#ifdef __cplusplus
}
#endif

#endif // __DISPLAY_H__
