#include "Video/Video.h"

Video::Video(int width, int height) 
    : width(width), height(height){
    flag_quit = false;

    video_thread = new std::thread(&Video::video_process, this);
}

Video::~Video() {
    {
        std::lock_guard<std::mutex> lock(mtx_video);
        flag_quit = true;
    }

    if (video_thread->joinable()) {
        video_thread->join();
    }
}

int Video::videoinit() {
    fps = 0;
	memset(fps_text,0,16);

    H264_PTS = 0;
    H264_TimeRef = 0;
    stFrame.pstPack = (VENC_PACK_S *)malloc(sizeof(VENC_PACK_S));

    // 创建内存池
    MB_POOL_CONFIG_S PoolCfg;
    memset(&PoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
    PoolCfg.u64MBSize = width * height * 3;
    PoolCfg.u32MBCnt = 1;
    PoolCfg.enAllocType = MB_ALLOC_TYPE_DMA;
    src_Pool = RK_MPI_MB_CreatePool(&PoolCfg);
    src_Blk = RK_MPI_MB_GetMB(src_Pool, width * height * 3, RK_TRUE);

    // Build h264_frame
	h264_frame.stVFrame.u32Width = width;
	h264_frame.stVFrame.u32Height = height;
	h264_frame.stVFrame.u32VirWidth = width;
	h264_frame.stVFrame.u32VirHeight = height;
	h264_frame.stVFrame.enPixelFormat =  RK_FMT_RGB888; 
	h264_frame.stVFrame.u32FrameFlag = 160;
	h264_frame.stVFrame.pMbBlk = src_Blk;
	data = (unsigned char *)RK_MPI_MB_Handle2VirAddr(src_Blk);
	frame = cv::Mat(cv::Size(width,height),CV_8UC3,data);

    // RKAIQ 初始化
    RK_BOOL multi_sensor = RK_FALSE;
    const char *iq_dir = "/etc/iqfiles";
    rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
    SAMPLE_COMM_ISP_Init(0, hdr_mode, multi_sensor, iq_dir);
    SAMPLE_COMM_ISP_Run(0);

    // RK MPI 系统初始化
    if (RK_MPI_SYS_Init() != RK_SUCCESS) {
        printf("RK MPI system initialization failed!\n");
        return -1;
    }

    // rtsp init	
	g_rtsplive = create_rtsp_demo(554);
	g_rtsp_session = rtsp_new_session(g_rtsplive, "/live/0");
	rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
	rtsp_sync_video_ts(g_rtsp_session, rtsp_get_reltime(), rtsp_get_ntptime());

    // VI 初始化
    vi_dev_init();
    vi_chn_init(0, width, height);

    // venc init
	RK_CODEC_ID_E enCodecType = RK_VIDEO_ID_AVC;
	venc_init(0, width, height, enCodecType);

    return 0;
}

cv::Mat Video::getFrame() {
    if (RK_MPI_VI_GetChnFrame(0, 0, &stViFrame, -1) == RK_SUCCESS) {
        void *vi_data = RK_MPI_MB_Handle2VirAddr(stViFrame.stVFrame.pMbBlk);

        cv::Mat yuv420sp(height + height / 2, width, CV_8UC1, vi_data);
        cv::Mat bgr(height, width, CV_8UC3, data);			
        cv::cvtColor(yuv420sp, bgr, cv::COLOR_YUV420sp2BGR);
        cv::resize(bgr, frame, cv::Size(width ,height), 0, 0, cv::INTER_LINEAR);
        
        sprintf(fps_text,"fps = %.2f",fps);		
        cv::putText(frame,fps_text,
                        cv::Point(100, 100),
                        cv::FONT_HERSHEY_SIMPLEX,3,
                        cv::Scalar(0,255,0),4);

        memcpy(data, frame.data, width * height * 3);  
    }
    return frame;
}

void Video::encodeFrame() {
    // get vi frame
    h264_frame.stVFrame.u32TimeRef = H264_TimeRef++;
    h264_frame.stVFrame.u64PTS = TEST_COMM_GetNowUs(); 
    RK_MPI_VENC_SendFrame(0, &h264_frame, -1);
}

void Video::sendRtspFrame() {
    if (RK_SUCCESS == RK_MPI_VENC_GetStream(0, &stFrame, -1)) {
        if (g_rtsplive && g_rtsp_session) {
            void *pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
            rtsp_tx_video(g_rtsp_session, (uint8_t *)pData, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS);
            rtsp_do_event(g_rtsplive);
        }
        RK_U64 nowUs = TEST_COMM_GetNowUs();
		fps = (float) 1000000 / (float)(nowUs - h264_frame.stVFrame.u64PTS);
    }
}

void Video::releaseChnFrame() {
    RK_S32 s32Ret = RK_MPI_VI_ReleaseChnFrame(0, 0, &stViFrame);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
    }
}

void Video::releaseVencFrame() {
    RK_S32 s32Ret = RK_MPI_VENC_ReleaseStream(0, &stFrame);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
    }
}

void Video::cleanup() {
    if (g_rtsplive) {
        rtsp_del_demo(g_rtsplive);
    }

    RK_MPI_SYS_UnBind(&stSrcChn, &stvpssChn);
    RK_MPI_VI_DisableChn(0, 0);
    RK_MPI_VI_DisableDev(0);
    RK_MPI_VPSS_StopGrp(0);
    RK_MPI_VPSS_DestroyGrp(0);
    SAMPLE_COMM_ISP_Stop(0);
    RK_MPI_SYS_Exit();

    if (src_Blk) {
        RK_MPI_MB_ReleaseMB(src_Blk);
    }

    if (src_Pool) {
        RK_MPI_MB_DestroyPool(src_Pool);
    }
}

void Video::video_process() {
    videoinit();
    while (!flag_quit) {
        cv::Mat frame = getFrame();
        signal_video_frame.emit(frame);
        encodeFrame();
        sendRtspFrame();
        releaseChnFrame();
        releaseVencFrame();
    }
    cleanup();
}