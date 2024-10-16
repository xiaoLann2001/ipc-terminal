#ifndef __VIDEO_H_
#define __VIDEO_H_

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>
// #include <vector>

#include "luckfox_mpi.h"
#include "rtsp_demo.h"

// 初始化视频捕捉系统
int video_capture_init(int width, int height);

// 获取帧数据
void *video_capture_get_frame(VIDEO_FRAME_INFO_S *stVpssFrame);

// 释放帧
RK_S32 video_capture_release_frame(VIDEO_FRAME_INFO_S *stVpssFrame);

// 清理系统资源
void video_capture_cleanup();

// 视频编码器初始化
void video_encode_init(int width, int height, RK_CODEC_ID_E enCodecType);

// 对vpss帧进行编码
RK_S32 video_encode(VIDEO_FRAME_INFO_S *stVpssFrame, VENC_STREAM_S *stFrame);

RK_S32 video_encode_release_frame(VENC_STREAM_S *stFrame);

void video_encode_cleanup();

// RTSP初始化
void video_rtsp_init(int port);

// 发送视频流到RTSP
void video_rtsp_send(VENC_STREAM_S *stFrame);

void video_rtsp_cleanup();


#endif // __VIDEO_CAPTURE_H_
