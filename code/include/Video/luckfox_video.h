#ifndef __LUCKFOX_MPI_H
#define __LUCKFOX_MPI_H

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

#include "sample_comm.h"

#define TEST_ARGB32_PIX_SIZE 4
#define TEST_ARGB32_RED 0xFF0000FF
#define TEST_ARGB32_GREEN 0x00FF00FF
#define TEST_ARGB32_BLUE 0x0000FFFF
#define TEST_ARGB32_TRANS 0x00000000
#define TEST_ARGB32_BLACK 0x000000FF

#define HIGH_BIT_RATE_WIDTH 2304
#define HIGH_BIT_RATE_HEIGHT 1296
#define HIGH_BIT_RATE_FPS 15
#define LOW_BIT_RATE_WIDTH 640
#define LOW_BIT_RATE_HEIGHT 480
#define LOW_BIT_RATE_FPS 30

#define FPS_SHOW 1

int rkaiq_init();
int rkaiq_deinit();
int rkmpi_sys_init();
int rkmpi_sys_deinit();
int create_MB_pool(MB_BLK *src_Blk, MB_POOL *src_Pool, int width, int height);
int destroy_MB_pool(MB_BLK *src_Blk, MB_POOL *src_Pool);

RK_U64 TEST_COMM_GetNowUs();
// RK_S32 test_rgn_overlay_line_process(int sX ,int sY,int type, int group);
// RK_S32 rgn_overlay_release(int group);

int vi_dev_init();
int vi_dev_deinit();
int vi_chn_init(int pipeId, int channelId, int width, int height, PIXEL_FORMAT_E enPixelFormat);
int vi_chn_deinit(int pipeId, int channelId);
void* vi_get_frame(int pipeId, int viChannelId, int width, int height, VIDEO_FRAME_INFO_S *stViFrame);
int vi_release_frame(int pipeId, int viChannelId, VIDEO_FRAME_INFO_S *stViFrame);

int venc_init(int chnId, int width, int height, RK_CODEC_ID_E enType, PIXEL_FORMAT_E enPixelFormat);
int venc_deinit(int chnId);
int venc_encode_frame(int vencChannelId, VIDEO_FRAME_INFO_S *venc_frame);
int venc_release_frame(int vencChannelId, VENC_STREAM_S *stFrame);

int bind_vi_to_venc(int pipeId, MPP_CHN_S *vi_chn, MPP_CHN_S *venc_chn);
int unbind_vi_to_venc(int pipeId, MPP_CHN_S *vi_chn, MPP_CHN_S *venc_chn);

int vpss_init(int VpssChn, int width, int height);

#endif