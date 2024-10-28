#include "Video/Video.h"

Video::Video()
{

    flag_quit = false;

    rkaiq_init();
    rkmpi_sys_init();
    vi_dev_init();
    rtsp_init();

    video_thread0 = new std::thread(&Video::video_pipe0, this);
    video_thread1 = new std::thread(&Video::video_pipe1, this);
    video_thread2 = new std::thread(&Video::video_pipe2, this);
}

Video::~Video()
{
    {
        std::lock_guard<std::mutex> lock(mtx_video);
        flag_quit = true;
    }

    if (video_thread0->joinable())
    {
        video_thread0->join();
    }

    if (video_thread1->joinable())
    {
        video_thread1->join();
    }

    if (video_thread2->joinable())
    {
        video_thread2->join();
    }

    rtsp_deinit();
    vi_dev_deinit();
    rkmpi_sys_deinit();
    rkaiq_deinit();
}

void Video::video_pipe0()
{
    std::cout << "Video pipe 0 started" << std::endl;
    int pipeId = 0;
    int viChannelId = 0;
    int vencChannelId = 0;
    int video_width = HIGH_BIT_RATE_WIDTH;
    int video_height = HIGH_BIT_RATE_HEIGHT;
    // int video_width = 2304;
    // int video_height = 1296;

    VENC_STREAM_S stFrame;

    stFrame.pstPack = (VENC_PACK_S *)malloc(sizeof(VENC_PACK_S));

    vi_chn_init(pipeId, viChannelId, video_width, video_height, RK_FMT_YUV420SP);
    venc_init(vencChannelId, video_width, video_height, RK_VIDEO_ID_AVC, RK_FMT_YUV420SP);

    MPP_CHN_S vi_chn, venc_chn;
    bind_vi_to_venc(pipeId, &vi_chn, &venc_chn);

    rkipc_osd_init();

    while (!flag_quit)
    {
        // 获取编码后的帧，发送到 RTSP 服务器
        rtsp_send_frame(vencChannelId, &stFrame);

        // 释放编码后的帧
        venc_release_frame(vencChannelId, &stFrame);
    }

    rkipc_osd_deinit();

    usleep(500 * 1000);
    unbind_vi_to_venc(pipeId, &vi_chn, &venc_chn);
    venc_deinit(vencChannelId);
    vi_chn_deinit(pipeId, viChannelId);
    free(stFrame.pstPack);
}

void Video::video_pipe1()
{
    std::cout << "Video pipe 1 started" << std::endl;
    int pipeId = 0;
    int viChannelId = 1;
    int vencChannelId = 1;
    int video_width = LOW_BIT_RATE_WIDTH;
    int video_height = LOW_BIT_RATE_HEIGHT;
    // int video_width = 640;
    // int video_height = 480;

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
    venc_frame.stVFrame.enPixelFormat = RK_FMT_RGB888;
    venc_frame.stVFrame.u32FrameFlag = 160;
    venc_frame.stVFrame.pMbBlk = src_blk;
    unsigned char *venc_data = (unsigned char *)RK_MPI_MB_Handle2VirAddr(src_blk);
    cv::Mat frame(cv::Size(video_width, video_height), CV_8UC3, venc_data);
    cv::Mat yuv420sp(video_height + video_height / 2, video_width, CV_8UC1);
    cv::Mat bgr(video_height, video_width, CV_8UC3);

    vi_chn_init(pipeId, viChannelId, video_width, video_height, RK_FMT_YUV420SP);
    venc_init(vencChannelId, video_width, video_height, RK_VIDEO_ID_AVC, RK_FMT_RGB888);

    while (!flag_quit)
    {
        void *vi_data = vi_get_frame(pipeId, viChannelId, video_width, video_height, &stViFrame);

        yuv420sp.data = (unsigned char *)vi_data;
        bgr.data = venc_data;
        cv::cvtColor(yuv420sp, bgr, cv::COLOR_YUV420sp2BGR);
        cv::resize(bgr, frame, cv::Size(video_width, video_height), 0, 0, cv::INTER_LINEAR);
        // cv::resize(bgr, frame, cv::Size(video_width ,video_height), 0, 0, cv::INTER_NEAREST);
#if FPS_SHOW
        sprintf(fps_text, "fps = %.2f", fps);
        cv::putText(frame, fps_text,
                    cv::Point(x_scaled, y_scaled),
                    cv::FONT_HERSHEY_SIMPLEX, 1,
                    cv::Scalar(0, 255, 0), 1);
#endif
        memcpy(venc_data, frame.data, video_width * video_height * 3);
        signal_video_frame.emit(frame);

        venc_encode_frame(vencChannelId, &venc_frame);
        rtsp_send_frame(vencChannelId, &stFrame);
#if FPS_SHOW
        RK_U64 nowUs = TEST_COMM_GetNowUs();
        fps = (float)1000000 / (float)(nowUs - venc_frame.stVFrame.u64PTS);
#endif
        vi_release_frame(pipeId, viChannelId, &stViFrame);
        venc_release_frame(vencChannelId, &stFrame);
    }

    venc_deinit(vencChannelId);
    vi_chn_deinit(pipeId, viChannelId);
    free(stFrame.pstPack);
    destroy_MB_pool(&src_blk, &src_Pool);
}

void Video::video_pipe2()
{
    std::cout << "Video pipe 2 started" << std::endl;

    // 0: 'person', 1: 'bicycle', 2: 'car', 3: 'motorcycle', 4: 'airplane', 
    // 5: 'bus', 6: 'train', 7: 'truck', 8: 'boat', 9: 'traffic light', 
    // 10: 'fire hydrant', 11: 'stop sign', 12: 'parking meter', 13: 'bench', 14: 'bird', 
    // 15: 'cat', 16: 'dog', 17: 'horse', 18: 'sheep', 19: 'cow', 
    // 20: 'elephant', 21: 'bear', 22: 'zebra', 23: 'giraffe', 24: 'backpack', 
    // 25: 'umbrella', 26: 'handbag', 27: 'tie', 28: 'suitcase', 29: 'frisbee', 
    // 30: 'skis', 31: 'snowboard', 32: 'sports ball', 33: 'kite', 34: 'baseball bat', 
    // 35: 'baseball glove', 36: 'skateboard', 37: 'surfboard', 38: 'tennis racket', 39: 'bottle', 
    // 40: 'wine glass', 41: 'cup', 42: 'fork', 43: 'knife', 44: 'spoon', 
    // 45: 'bowl', 46: 'banana', 47: 'apple', 48: 'sandwich', 49: 'orange', 
    // 50: 'broccoli', 51: 'carrot', 52: 'hot dog', 53: 'pizza', 54: 'donut', 
    // 55: 'cake', 56: 'chair', 57: 'couch', 58: 'potted plant', 59: 'bed', 
    // 60: 'dining table', 61: 'toilet', 62: 'tv', 63: 'laptop', 64: 'mouse', 
    // 65: 'remote', 66: 'keyboard', 67: 'cell phone', 68: 'microwave', 69: 'oven', 
    // 70: 'toaster', 71: 'sink', 72: 'refrigerator', 73: 'book', 74: 'clock', 
    // 75: 'vase', 76: 'scissors', 77: 'teddy bear', 78: 'hair drier', 79: 'toothbrush'

    // Rknn model
    int sX, sY, eX, eY;
    char text[16];
    rknn_app_context_t rknn_app_ctx;
    object_detect_result_list od_results;
    const char *model_path = "./model/yolov5.rknn";
    const char *label_path = "./model/coco_80_labels_list.txt";
    memset(&rknn_app_ctx, 0, sizeof(rknn_app_context_t));

    init_yolov5_model(model_path, &rknn_app_ctx);
    init_post_process(label_path);

    // osd
    int ret = 0;
	int line_pixel = 2;
	RGN_HANDLE RgnHandle = 0;
	RGN_CANVAS_INFO_S stCanvasInfo;
    int batch_num = 0;
	memset(&stCanvasInfo, 0, sizeof(RGN_CANVAS_INFO_S));
    // wait for venc to start
    usleep(500 * 1000);
    rgn_draw_nn_init(RgnHandle);

    // video parameters
    int pipeId = 0;
    int viChannelId = 2;
    int vencChannelId = 2;
    int video_width = 640;
    int video_height = 640;
    int rgn_video_width = 2304;
    int rgn_video_height = 1296;

    VIDEO_FRAME_INFO_S stViFrame;
    cv::Mat yuv420sp(video_height + video_height / 2, video_width, CV_8UC1);
    cv::Mat bgr(video_height, video_width, CV_8UC3);

    vi_chn_init(pipeId, viChannelId, video_width, video_height, RK_FMT_YUV420SP);

    while (!flag_quit)
    {
        // usleep(100 * 1000);
        // get vi frame
        yuv420sp.data = (unsigned char *)vi_get_frame(pipeId, viChannelId, video_width, video_height, &stViFrame);
        cv::cvtColor(yuv420sp, bgr, cv::COLOR_YUV420sp2BGR);
        cv::resize(bgr, bgr, cv::Size(video_width, video_height), 0, 0, cv::INTER_LINEAR);

        // letterbox
        cv::Mat letterboxImage = letterbox(bgr, video_width, video_height);
        memcpy(rknn_app_ctx.input_mems[0]->virt_addr, letterboxImage.data, MODEL_HEIGHT * MODEL_HEIGHT * 3);

        inference_yolov5_model(&rknn_app_ctx, &od_results);

        // draw osd
        std::vector<RgnDrawParams> tasks(20);
        // printf("od_results.count: %d\n", od_results.count);
        for (int i = 0; i < od_results.count; i++)
        {
            if (od_results.count >= 1)
            {
                object_detect_result *det_result = &(od_results.results[i]);
                
                // if (det_result->cls_id > 8) continue;

                sX = (int)(det_result->box.left);
                sY = (int)(det_result->box.top);
                eX = (int)(det_result->box.right);
                eY = (int)(det_result->box.bottom);
                mapCoordinates(&sX, &sY);
                mapCoordinates(&eX, &eY);
                sX = (int)((float)sX / (float)video_width * rgn_video_width);
                sY = (int)((float)sY / (float)video_height * rgn_video_height);
                eX = (int)((float)eX / (float)video_width * rgn_video_width);
                eY = (int)((float)eY / (float)video_height * rgn_video_height);
                // printf("%s @ (%d %d %d %d) %.3f\n", coco_cls_to_name(det_result->cls_id),
                //        sX, sY, eX, eY, det_result->prop);

                RgnDrawParams task;
                task.RgnHandle = RgnHandle;
                task.x = sX;
                task.y = sY;
                task.w = eX - sX;
                task.h = eY - sY;
                task.line_pixel = line_pixel;
                tasks.push_back(task);
            }
        }
        rgn_add_draw_tasks_batch(tasks);

        vi_release_frame(pipeId, viChannelId, &stViFrame);
    }

    rgn_draw_nn_deinit();
    vi_chn_deinit(pipeId, viChannelId);
    release_yolov5_model(&rknn_app_ctx);
    deinit_post_process();
}
