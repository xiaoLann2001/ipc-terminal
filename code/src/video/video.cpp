#include "video/video.h"

// for rtsp
rtsp_demo_handle g_rtsplive;
rtsp_session_handle g_rtsp_session;
MPP_CHN_S stSrcChn, stvpssChn;

// 函数实现
int video_capture_init(int width, int height) {
    // rkaiq init
	RK_BOOL multi_sensor = RK_FALSE;	
	const char *iq_dir = "/etc/iqfiles";
	rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
	//hdr_mode = RK_AIQ_WORKING_MODE_ISP_HDR2;
	SAMPLE_COMM_ISP_Init(0, hdr_mode, multi_sensor, iq_dir);
	SAMPLE_COMM_ISP_Run(0);
    
    // rkmpi 初始化
    if (RK_MPI_SYS_Init() != RK_SUCCESS) {
        printf("RK MPI system initialization failed!\n");
        return -1;
    }

    // VI 初始化
    vi_dev_init();
    vi_chn_init(0, width, height);

    // VPSS 初始化
    vpss_init(0, width, height);

    // 绑定 VI 到 VPSS
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;

    stvpssChn.enModId = RK_ID_VPSS;
    stvpssChn.s32DevId = 0;
    stvpssChn.s32ChnId = 0;

    printf("====RK_MPI_SYS_Bind VI0 to VPSS0====\n");
    if (RK_MPI_SYS_Bind(&stSrcChn, &stvpssChn) != RK_SUCCESS) {
        printf("Failed to bind VI to VPSS\n");
        return -1;
    }

    return 0;
}

void *video_capture_get_frame(VIDEO_FRAME_INFO_S *stVpssFrame) {
    if (RK_MPI_VPSS_GetChnFrame(0, 0, stVpssFrame, -1) == RK_SUCCESS) {
        return RK_MPI_MB_Handle2VirAddr(stVpssFrame->stVFrame.pMbBlk);
    }
    return NULL;
}

RK_S32 video_capture_release_frame(VIDEO_FRAME_INFO_S *stVpssFrame) {
    return RK_MPI_VPSS_ReleaseChnFrame(0, 0, stVpssFrame);
}

void video_capture_cleanup() {
    printf("\n----------1-----------\n");
    RK_MPI_SYS_UnBind(&stSrcChn, &stvpssChn);

    printf("\n----------2-----------\n");

    RK_MPI_VI_DisableChn(0, 0);
    RK_MPI_VI_DisableDev(0);

    printf("\n----------3-----------\n");

    RK_MPI_VPSS_StopGrp(0);
    RK_MPI_VPSS_DestroyGrp(0);

    printf("\n----------4-----------\n");

	SAMPLE_COMM_ISP_Stop(0);

    printf("\n----------5-----------\n");

    RK_MPI_SYS_Exit();
}

// 视频编码器初始化
void video_encode_init(int width, int height, RK_CODEC_ID_E enCodecType) {
	venc_init(0, width, height, enCodecType);
}

// 对vpss帧进行编码
RK_S32 video_encode(VIDEO_FRAME_INFO_S *stVpssFrame, VENC_STREAM_S *stFrame) {
    RK_MPI_VENC_SendFrame(0, stVpssFrame, -1);
    return RK_MPI_VENC_GetStream(0, stFrame, -1);
}

RK_S32 video_encode_release_frame(VENC_STREAM_S *stFrame) {
    return RK_MPI_VENC_ReleaseStream(0, stFrame);
}

void video_encode_cleanup() {
    RK_MPI_VENC_StopRecvFrame(0);
	RK_MPI_VENC_DestroyChn(0);
}

// RTSP初始化
void video_rtsp_init(int port) {
    g_rtsplive = create_rtsp_demo(port);
	g_rtsp_session = rtsp_new_session(g_rtsplive, "/live/0");
	rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
	// rtsp_sync_video_ts(g_rtsp_session, rtsp_get_reltime(), rtsp_get_ntptime());
}

// 发送视频流到RTSP
void video_rtsp_send(VENC_STREAM_S *stFrame) {
    if(g_rtsplive && g_rtsp_session)
    {
        //printf("len = %d PTS = %d \n",stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS);	
        void *pData = RK_MPI_MB_Handle2VirAddr(stFrame->pstPack->pMbBlk);
        rtsp_tx_video(g_rtsp_session, (uint8_t *)pData, stFrame->pstPack->u32Len,
                        stFrame->pstPack->u64PTS);
        rtsp_do_event(g_rtsplive);
    }
    // RK_U64 nowUs = TEST_COMM_GetNowUs();
    // float fps = (float) 1000000 / (float)(nowUs - stVpssFrame->stVFrame.u64PTS);
}

void video_rtsp_cleanup() {
    if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);
}
