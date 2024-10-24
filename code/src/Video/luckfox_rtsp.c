#include "Video/luckfox_rtsp.h"

pthread_mutex_t g_rtsp_mutex = PTHREAD_MUTEX_INITIALIZER;
rtsp_demo_handle g_rtsplive = NULL;
rtsp_session_handle g_rtsp_session_0, g_rtsp_session_1;

int rtsp_init() {
	g_rtsplive = create_rtsp_demo(554);
	g_rtsp_session_0 = rtsp_new_session(g_rtsplive, RTSP_URL_0);
	g_rtsp_session_1 = rtsp_new_session(g_rtsplive, RTSP_URL_1);
	rtsp_set_video(g_rtsp_session_0, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
	rtsp_set_video(g_rtsp_session_1, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);

	rtsp_sync_video_ts(g_rtsp_session_0, rtsp_get_reltime(), rtsp_get_ntptime());
	rtsp_sync_video_ts(g_rtsp_session_1, rtsp_get_reltime(), rtsp_get_ntptime());

	return 0;
}

int rtsp_deinit() {
	if (g_rtsp_session_0) {
		rtsp_del_session(g_rtsp_session_0);
		g_rtsp_session_0 = NULL;
	}
	if (g_rtsp_session_1) {
		rtsp_del_session(g_rtsp_session_1);
		g_rtsp_session_1 = NULL;
	}
	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);
	g_rtsplive = NULL;
	return 0;
}

int rtsp_send_frame(int vencChannelId, VENC_STREAM_S* stFrame) {
    if (0 == vencChannelId) {
        if (RK_SUCCESS == RK_MPI_VENC_GetStream(vencChannelId, stFrame, -1)) {
            if (g_rtsplive && g_rtsp_session_0) {
                void *pData = RK_MPI_MB_Handle2VirAddr(stFrame->pstPack->pMbBlk);
				pthread_mutex_lock(&g_rtsp_mutex);
                rtsp_tx_video(g_rtsp_session_0, (uint8_t *)pData, stFrame->pstPack->u32Len, stFrame->pstPack->u64PTS);
                rtsp_do_event(g_rtsplive);
				pthread_mutex_unlock(&g_rtsp_mutex);
            }
        }
    }

    if (1 == vencChannelId) {
        if (RK_SUCCESS == RK_MPI_VENC_GetStream(vencChannelId, stFrame, -1)) {
            if (g_rtsplive && g_rtsp_session_1) {
                void *pData = RK_MPI_MB_Handle2VirAddr(stFrame->pstPack->pMbBlk);
				pthread_mutex_lock(&g_rtsp_mutex);
                rtsp_tx_video(g_rtsp_session_1, (uint8_t *)pData, stFrame->pstPack->u32Len, stFrame->pstPack->u64PTS);
                rtsp_do_event(g_rtsplive);
				pthread_mutex_unlock(&g_rtsp_mutex);
            }
        }
    }
    return 0;
}