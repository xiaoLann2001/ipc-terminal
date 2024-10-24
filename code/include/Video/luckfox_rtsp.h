#ifndef _LUCKFOX_RTSP_H_
#define _LUCKFOX_RTSP_H_

#include "rtsp_demo.h"
#include "sample_comm.h"

#define RTSP_PORT 554
#define RTSP_URL_0 "/live/0"
#define RTSP_URL_1 "/live/1"

#ifdef __cplusplus
extern "C" {
#endif

int rtsp_init();
int rtsp_deinit();
int rtsp_send_frame(int vencChannelId, VENC_STREAM_S* stFrame);

#ifdef __cplusplus
}
#endif

#endif