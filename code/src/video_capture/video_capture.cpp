#include "video_capture/video_capture.h"

// 函数实现
int videocapture_init(int width, int height) {
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
    MPP_CHN_S stSrcChn, stvpssChn;
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

void *videocapture_get_frame(VIDEO_FRAME_INFO_S *stVpssFrame) {
    if (RK_MPI_VPSS_GetChnFrame(0, 0, stVpssFrame, -1) == RK_SUCCESS) {
        return RK_MPI_MB_Handle2VirAddr(stVpssFrame->stVFrame.pMbBlk);
    }
    return NULL;
}

int videocapture_release_frame(VIDEO_FRAME_INFO_S *stVpssFrame) {
    return RK_MPI_VPSS_ReleaseChnFrame(0, 0, stVpssFrame);
}

void videocapture_cleanup() {
    MPP_CHN_S stSrcChn, stvpssChn;
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;
    stvpssChn.enModId = RK_ID_VPSS;
    stvpssChn.s32DevId = 0;
    stvpssChn.s32ChnId = 0;

    RK_MPI_SYS_UnBind(&stSrcChn, &stvpssChn);
    RK_MPI_VI_DisableChn(0, 0);
    RK_MPI_VI_DisableDev(0);
    RK_MPI_VPSS_StopGrp(0);
    RK_MPI_VPSS_DestroyGrp(0);
    RK_MPI_SYS_Exit();
}
