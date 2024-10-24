#include "Video/Video.h"

Video::Video() {
    
    flag_quit = false;

    rkmpi_sys_init();
    rkaiq_init();
    vi_dev_init();
    rtsp_init();

    video_thread0 = new std::thread(&Video::video_pipe0, this);
    video_thread1 = new std::thread(&Video::video_pipe1, this);
}

Video::~Video() {
    {
        std::lock_guard<std::mutex> lock(mtx_video);
        flag_quit = true;
    }

    if (video_thread0->joinable()) {
        video_thread0->join();
    }

    if (video_thread1->joinable()) {
        video_thread1->join();
    }

    rtsp_deinit();
    vi_dev_deinit();
    rkaiq_deinit();
    rkmpi_sys_deinit();
}

void Video::video_pipe0() {
    std::cout << "Video pipe 0 started" << std::endl;
    int pipeId = 0;
    int viChannelId = 0;
    int vencChannelId = 0;
    // int video_width = HIGH_BIT_RATE_WIDTH;
    // int video_height = HIGH_BIT_RATE_HEIGHT;
    int video_width = 2304;
    int video_height = 1296;

    VENC_STREAM_S stFrame;	

    stFrame.pstPack = (VENC_PACK_S *)malloc(sizeof(VENC_PACK_S));

    vi_chn_init(pipeId, viChannelId, video_width, video_height, RK_FMT_YUV420SP);
	venc_init(vencChannelId, video_width, video_height, RK_VIDEO_ID_AVC, RK_FMT_YUV420SP);

    MPP_CHN_S vi_chn, venc_chn;
    bind_vi_to_venc(pipeId, &vi_chn, &venc_chn);

    while (!flag_quit) {
        // 获取编码后的帧，发送到 RTSP 服务器
        rtsp_send_frame(vencChannelId, &stFrame);

        // 释放编码后的帧
        venc_release_frame(vencChannelId, &stFrame);
    }

    unbind_vi_to_venc(pipeId, &vi_chn, &venc_chn);
    venc_deinit(vencChannelId);
    vi_chn_deinit(pipeId, viChannelId);
    free(stFrame.pstPack);
}

void Video::video_pipe1() {
    std::cout << "Video pipe 1 started" << std::endl;
    int pipeId = 0;
    int viChannelId = 1;
    int vencChannelId = 1;
    int video_width = 640;
    int video_height = 480;

#if FPS_SHOW
    char fps_text[16];
	float fps = 0;
	memset(fps_text, 0, 16);
    int x_scaled = (float)50.0 / 640 * video_width;
    int y_scaled = (float)50.0 / 480 * video_height;
    int font_scaled = (float)1.0 / 480 * video_height;
    int thickness_scaled = (float)1.0 / 480 * video_height;
#endif
    VENC_STREAM_S stFrame;	
	VIDEO_FRAME_INFO_S stViFrame;
    stFrame.pstPack = (VENC_PACK_S *)malloc(sizeof(VENC_PACK_S));

    MB_BLK src_blk;
    MB_POOL src_Pool;
    create_MB_pool(&src_blk, &src_Pool, video_width, video_height);

    // Build venc_frame
    VIDEO_FRAME_INFO_S venc_frame;
	venc_frame.stVFrame.u32Width = video_width;
	venc_frame.stVFrame.u32Height = video_height;
	venc_frame.stVFrame.u32VirWidth = video_width;
	venc_frame.stVFrame.u32VirHeight = video_height;
	venc_frame.stVFrame.enPixelFormat =  RK_FMT_RGB888; 
	venc_frame.stVFrame.u32FrameFlag = 160;
	venc_frame.stVFrame.pMbBlk = src_blk;
	unsigned char *venc_data = (unsigned char *)RK_MPI_MB_Handle2VirAddr(src_blk);
	cv::Mat frame(cv::Size(video_width, video_height), CV_8UC3, venc_data);
    cv::Mat yuv420sp(video_height + video_height / 2, video_width, CV_8UC1);
    cv::Mat bgr(video_height, video_width, CV_8UC3);

    vi_chn_init(pipeId, viChannelId, video_width, video_height, RK_FMT_YUV420SP);
	venc_init(vencChannelId, video_width, video_height, RK_VIDEO_ID_AVC, RK_FMT_RGB888);

    while (!flag_quit) {
        void *vi_data = vi_get_frame(pipeId, viChannelId, video_width, video_height, &stViFrame);

        yuv420sp.data = (unsigned char*)vi_data;
        bgr.data = venc_data;	
        cv::cvtColor(yuv420sp, bgr, cv::COLOR_YUV420sp2BGR);
        cv::resize(bgr, frame, cv::Size(video_width ,video_height), 0, 0, cv::INTER_LINEAR);
        // cv::resize(bgr, frame, cv::Size(video_width ,video_height), 0, 0, cv::INTER_NEAREST);
#if FPS_SHOW
        sprintf(fps_text,"fps = %.2f",fps);		
        cv::putText(frame,fps_text,
                        cv::Point(x_scaled, y_scaled),
                        cv::FONT_HERSHEY_SIMPLEX,1,
                        cv::Scalar(0,255,0),1);
#endif
        memcpy(venc_data, frame.data, video_width * video_height * 3); 
        signal_video_frame.emit(frame);

        venc_encode_frame(vencChannelId, &venc_frame);
        rtsp_send_frame(vencChannelId, &stFrame);
#if FPS_SHOW
            RK_U64 nowUs = TEST_COMM_GetNowUs();
            fps = (float) 1000000 / (float)(nowUs - venc_frame.stVFrame.u64PTS);
#endif
        vi_release_frame(pipeId, viChannelId, &stViFrame);
        venc_release_frame(vencChannelId, &stFrame);
    }

    venc_deinit(vencChannelId);
    vi_chn_deinit(pipeId, viChannelId);
    free(stFrame.pstPack);
    destroy_MB_pool(&src_blk, &src_Pool);
}

void Video::video_pipe2() {
    std::cout << "Video pipe 2 started" << std::endl;
    int pipeId = 0;
    int viChannelId = 2;
    int vencChannelId = 2;
    int video_width = 640;
    int video_height = 480;

	VIDEO_FRAME_INFO_S stViFrame;
    cv::Mat yuv420sp(video_height + video_height / 2, video_width, CV_8UC1);
    cv::Mat bgr(video_height, video_width, CV_8UC3);

    vi_chn_init(pipeId, viChannelId, video_width, video_height, RK_FMT_YUV420SP);

    while (!flag_quit) {
        yuv420sp.data = (unsigned char*)vi_get_frame(pipeId, viChannelId, video_width, video_height, &stViFrame);
        cv::cvtColor(yuv420sp, bgr, cv::COLOR_YUV420sp2BGR);
        cv::resize(bgr, bgr, cv::Size(video_width ,video_height), 0, 0, cv::INTER_LINEAR);


        vi_release_frame(pipeId, viChannelId, &stViFrame);
    }

    vi_chn_deinit(pipeId, viChannelId);
}
