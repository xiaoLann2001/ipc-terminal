#ifndef __VIDEO_CAPTURE_H_
#define __VIDEO_CAPTURE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 将 C++ 头文件包含在 extern "C" 之外
#include "luckfox_mpi.h"

// 定义视频宽高
#define VIDEO_WIDTH  720
#define VIDEO_HEIGHT 480

// // 确保在 C++ 环境下正确使用 C 链接
// #ifdef __cplusplus
// extern "C" {
// #endif

// 初始化视频捕捉系统
int videocapture_init(int width, int height);

// 获取帧数据
void *videocapture_get_frame(VIDEO_FRAME_INFO_S *stVpssFrame);

// 释放帧
int videocapture_release_frame(VIDEO_FRAME_INFO_S *stVpssFrame);

// 清理系统资源
void videocapture_cleanup();

// #ifdef __cplusplus
// }
// #endif

#endif // __VIDEO_CAPTURE_H_
