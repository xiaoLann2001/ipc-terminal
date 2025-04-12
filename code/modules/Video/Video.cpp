#include "Video.h"

Video::Video()
{

    video_run_ = true;
    pipe0_run_ = true;
    pipe1_run_ = true;
    pipe2_run_ = true;

    rkaiq_init();
    rkmpi_sys_init();
    vi_dev_init();
    rtsp_init();

    video_thread0 = std::make_unique<std::thread>(&Video::video_pipe0, this);
    video_thread1 = std::make_unique<std::thread>(&Video::video_pipe1, this);
    
    int ai_enable = rk_param_get_int("ai:enable", 0);
    if (ai_enable) {
        video_thread2 = std::make_unique<std::thread>(&Video::video_pipe2, this);
    }
}

Video::~Video()
{
    {
        std::lock_guard<std::mutex> lock(mtx_video);
        video_run_ = false;
    }

    if (video_thread0 && video_thread2->joinable()) video_thread2->join();
    if (video_thread0 && video_thread1->joinable()) video_thread1->join();
    if (video_thread0 && video_thread0->joinable()) video_thread0->join();

    rtsp_deinit();
    vi_dev_deinit();
    rkmpi_sys_deinit();
    rkaiq_deinit();

    LOG_DEBUG("Video deinitialized\n");
}

void Video::video_pipe0()
{
    std::cout << "Video pipe 0 started" << std::endl;
    int pipeId = 0;
    int viChannelId = 0;
    int vencChannelId = 0;
    int video_width = rk_param_get_int("video.0:width", 2304);
    int video_height = rk_param_get_int("video.0:height", 1296);
    // int video_width = 2304;
    // int video_height = 1296;

    VENC_STREAM_S stFrame;

    stFrame.pstPack = (VENC_PACK_S *)malloc(sizeof(VENC_PACK_S));

    vi_chn_init(pipeId, viChannelId, video_width, video_height, RK_FMT_YUV420SP);
    venc_init(vencChannelId, video_width, video_height, RK_VIDEO_ID_AVC, RK_FMT_YUV420SP);

    MPP_CHN_S vi_chn, venc_chn;
    bind_vi_to_venc(pipeId, &vi_chn, &venc_chn);

    rkipc_osd_init();

    while (video_run_ && pipe0_run_)
    {
        // 获取编码后的帧，发送到 RTSP 服务器
        rtsp_send_frame(vencChannelId, &stFrame);

        // 释放编码后的帧
        venc_release_frame(vencChannelId, &stFrame);
    }

    rkipc_osd_deinit();

    // wait for ai rgn to deinit
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
    int video_width = rk_param_get_int("video.1:width", 704);
    int video_height = rk_param_get_int("video.1:height", 576);
    // int video_width = 704;
    // int video_height = 576;

#if FPS_SHOW
    char fps_text[16];
    float fps = 0;
    memset(fps_text, 0, 16);
    int x_scaled = (float)50.0 / 704 * video_width;
    int y_scaled = (float)50.0 / 576 * video_height;
    int font_scaled = (float)1.0 / 576 * video_height;
    int thickness_scaled = (float)1.0 / 576 * video_height;
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

    while (video_run_ && pipe1_run_)
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

    int ai_od_enable = rk_param_get_int("ai.od:enable", 0);
    int line_pixel = rk_param_get_int("ai.od:line_pixel", 2);

    if (ai_od_enable) {
        int people_detect = rk_param_get_int("ai.od:people_detect", 0);     // class 0
        int vehicle_detect = rk_param_get_int("ai.od:vehicle_detect", 0);   // class 1,2,3,4,5,7,8
        int pet_detect = rk_param_get_int("ai.od:pet_detect", 0);           // class 15,16
        
        // detect classes set
        std::unordered_set<int> detect_classes;
        if (people_detect) {
            detect_classes.insert(0);
        }
        if (vehicle_detect) {
            detect_classes.insert(1);
            detect_classes.insert(2);
            detect_classes.insert(3);
            detect_classes.insert(4);
            detect_classes.insert(5);
            detect_classes.insert(7);
            detect_classes.insert(8);
        } 
        if (pet_detect) {
            detect_classes.insert(15);
            detect_classes.insert(16);
        }

        int ai_follow_enable = rk_param_get_int("ai.follow:enable", 0);
        int ai_follow_people = rk_param_get_int("ai.follow:people_follow", 0);
        int ai_follow_vehicle = rk_param_get_int("ai.follow:vehicle_follow", 0);
        int ai_follow_pet = rk_param_get_int("ai.follow:pet_follow", 0);
        int ai_follow_tolerance_width = rk_param_get_int("ai.follow:tolerance_width", 100);
        int ai_follow_tolerance_height = rk_param_get_int("ai.follow:tolerance_height", 100);
        int ai_follow_roi_x = rk_param_get_int("ai.follow:roi_x", 0);
        int ai_follow_roi_y = rk_param_get_int("ai.follow:roi_y", 0);
        int ai_follow_roi_width = rk_param_get_int("ai.follow:roi_width", 2304);
        int ai_follow_roi_height = rk_param_get_int("ai.follow:roi_height", 1296);

        // follow classes set
        std::unordered_set<int> follow_classes;
        if (ai_follow_enable) {
            // 只能选择跟随一个目标，从前往后优先级递减
            if (ai_follow_people) {
                follow_classes.insert(0);
            } else if (ai_follow_vehicle) {
                follow_classes.insert(1);
                follow_classes.insert(2);
                follow_classes.insert(3);
                follow_classes.insert(4);
                follow_classes.insert(5);
                follow_classes.insert(7);
                follow_classes.insert(8);
            } else if (ai_follow_pet) {
                follow_classes.insert(15);
                follow_classes.insert(16);
            }
        }

        // follow target info
        bool is_follow_target_detected;
        int follow_sX, follow_sY, follow_eX, follow_eY;
        int follow_sX_last, follow_sY_last, follow_eX_last, follow_eY_last;
        float follow_target_prop;

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

        RGN_HANDLE RgnHandle = 0;
        RGN_CANVAS_INFO_S stCanvasInfo;
        int batch_num = 0;

        // ai osd
        memset(&stCanvasInfo, 0, sizeof(RGN_CANVAS_INFO_S));
        // wait for venc to start
        usleep(500 * 1000);
        rgn_draw_nn_init(RgnHandle);

        // video parameters
        int pipeId = 0;
        int viChannelId = 2;
        int vencChannelId = 2;
        // int video_width = 640;
        // int video_height = 640;
        int video_width = MODEL_WIDTH;
        int video_height = MODEL_HEIGHT;
        int rgn_video_width = 2304;
        int rgn_video_height = 1296;
        int rgn_square_size = rgn_video_width * rgn_video_height;

        // video frame container
        VIDEO_FRAME_INFO_S stViFrame;
        cv::Mat yuv420sp(video_height + video_height / 2, video_width, CV_8UC1);
        cv::Mat bgr(video_height, video_width, CV_8UC3);

        vi_chn_init(pipeId, viChannelId, video_width, video_height, RK_FMT_YUV420SP);

        while (video_run_ && pipe2_run_)
        {
            // usleep(100 * 1000);
            // get vi frame
            yuv420sp.data = (unsigned char *)vi_get_frame(pipeId, viChannelId, video_width, video_height, &stViFrame);
            cv::cvtColor(yuv420sp, bgr, cv::COLOR_YUV420sp2BGR);
            // cv::resize(bgr, bgr, cv::Size(video_width, video_height), 0, 0, cv::INTER_LINEAR);

            // letterbox
            cv::Mat letterboxImage = letterbox(bgr, video_width, video_height);
            memcpy(rknn_app_ctx.input_mems[0]->virt_addr, letterboxImage.data, MODEL_HEIGHT * MODEL_HEIGHT * 3);

            // inference
            inference_yolov5_model(&rknn_app_ctx, &od_results);

            // draw osd
            std::vector<RgnDrawParams> tasks(20);
            // printf("od_results.count: %d\n", od_results.count);

            // init follow target info
            if (ai_follow_enable && follow_classes.size() > 0)
            {
                is_follow_target_detected = false;
                follow_sX = 0;
                follow_sY = 0;
                follow_eX = 0;
                follow_eY = 0;
                follow_sX_last = 0;
                follow_sY_last = 0;
                follow_eX_last = 0;
                follow_eY_last = 0;
                follow_target_prop = 0;
            }

            for (int i = 0; i < od_results.count; i++)
            {
                if (od_results.count >= 1)
                {
                    object_detect_result *det_result = &(od_results.results[i]);

                    if (detect_classes.count(det_result->cls_id) > 0)
                    {
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

                    // 更新跟随目标坐标，若有多个目标则选择置信度最高的目标，
                    // 若置信度相差不超过 0.1 则选择最靠近中心的目标
                    if (ai_follow_enable && follow_classes.count(det_result->cls_id) > 0)
                    {
                        is_follow_target_detected = true;   // 用于判断是否检测到跟随目标

                        bool update_target = false;  // 用于判断是否需要更新跟随目标

                        if (follow_sX == 0 && follow_sY == 0 && follow_eX == 0 && follow_eY == 0)
                        {
                            // 初始化跟随目标
                            update_target = true;
                        }
                        else
                        {
                            // 计算当前目标与视频中心的偏移
                            int delta_x = (sX + eX) / 2 - rgn_video_width / 2;
                            int delta_y = (sY + eY) / 2 - rgn_video_height / 2;
                            // 计算上一个目标与视频中心的偏移
                            int delta_last_x = (follow_sX_last + follow_eX_last) / 2 - rgn_video_width / 2;
                            int delta_last_y = (follow_sY_last + follow_eY_last) / 2 - rgn_video_height / 2;

                            // 计算当前目标的面积
                            int target_area = (eX - sX) * (eY - sY);
                            // 计算上一个目标的面积
                            int last_target_area = (follow_eX_last - follow_sX_last) * (follow_eY_last - follow_sY_last);

                            // 权衡置信度和面积，计算得分
                            float square_coefficient = 0.6;
                            float score = det_result->prop + square_coefficient * (float)target_area / (float)rgn_square_size;
                            float last_score = follow_target_prop + square_coefficient * (float)last_target_area/ (float)rgn_square_size;

                            // LOG_DEBUG("target_prop: %.3f, target_area: %d, score: %.3f\n", det_result->prop, target_area, score);
                            // LOG_DEBUG("last_target_prop: %.3f, last_target_area: %d, last_score: %.3f\n", follow_target_prop, last_target_area, last_score);

                            // 如果当前目标的得分更高，选择当前目标
                            if (score > last_score)
                            {
                                update_target = true;
                            }
                        }

                        // 如果决定更新目标，则更新目标的坐标和置信度
                        if (update_target)
                        {
                            follow_sX = sX;
                            follow_sY = sY;
                            follow_eX = eX;
                            follow_eY = eY;

                            // 保存当前目标的坐标和置信度
                            follow_sX_last = sX;
                            follow_eX_last = eX;
                            follow_sY_last = sY;
                            follow_eY_last = eY;
                            follow_target_prop = det_result->prop;
                        }
                    }
                }
            }
            rgn_add_draw_tasks_batch(tasks);

            vi_release_frame(pipeId, viChannelId, &stViFrame);

            if (ai_follow_enable && is_follow_target_detected)
            {
                // 计算目标中心位置与图像中心的偏移量
                int cx = (follow_sX + follow_eX) / 2;
                int cy = (follow_sY + follow_eY) / 2;

                // 若不在感兴趣区域内则不跟随
                if (cx < ai_follow_roi_x || cx > ai_follow_roi_x + ai_follow_roi_width ||
                    cy < ai_follow_roi_y || cy > ai_follow_roi_y + ai_follow_roi_height)
                {
                    continue;
                }

                int delta_x = cx - rgn_video_width / 2;
                int delta_y = cy - rgn_video_height / 2;

                // 设置容忍范围，若偏移量在容忍范围内则不调整云台
                if (abs(delta_x) < ai_follow_tolerance_width && abs(delta_y) < ai_follow_tolerance_height)
                {
                    continue;
                }

                // LOG_DEBUG("follow target: (%d, %d, %d, %d)\n", follow_sX, follow_sY, follow_eX, follow_eY);
                // LOG_DEBUG("cx: %d, cy: %d\n", cx, cy);
                // LOG_DEBUG("delta_x: %d, delta_y: %d\n", delta_x, delta_y);

                // 根据偏移量计算云台需要调整的角度，基于画幅 左右 90°，俯仰 45°，加上系数微调
                float pan_coefficient = 0.3;
                float tilt_coefficient = 0.3;
                int delta_pan = (int)(pan_coefficient * (float)delta_x / (float)rgn_video_width * 90);
                int delta_tilt = (int)(tilt_coefficient * (float)delta_y / (float)rgn_video_height * 45);

                // LOG_DEBUG("delta_pan: %d, delta_tilt: %d\n", delta_pan, delta_tilt);

                // 发射信号来调整云台位置
                signal_adjust_pantilt.emit(delta_pan, delta_tilt);  // 传递偏移量给舵机控制类
            }
        }

        rgn_draw_nn_deinit();
        vi_chn_deinit(pipeId, viChannelId);
        release_yolov5_model(&rknn_app_ctx);
        deinit_post_process();
    }
}

void Video::video_pipe0_start() {
    {
        // 线程2依赖线程0
        std::lock_guard<std::mutex> lock(mtx_video);
        // 若已经启动则直接返回
        if (pipe0_run_) {
            LOG_ERROR("Video pipe 0 already started\n");
            return;
        }
        pipe0_run_ = true;  // 设置运行标志位
        video_thread0 = std::make_unique<std::thread>(&Video::video_pipe0, this);
        // 若 AI 使能则启动 AI 线程
        int ai_enable = rk_param_get_int("ai:enable", 0);
        if (ai_enable && !pipe2_run_) {
            pipe2_run_ = true;
            video_thread2 = std::make_unique<std::thread>(&Video::video_pipe2, this);
        }
    }

    LOG_DEBUG("Video pipe 0 started\n");
}

void Video::video_pipe0_stop() {
    {
        // 线程2依赖线程0
        std::lock_guard<std::mutex> lock(mtx_video);
        if (!pipe0_run_) {
            LOG_ERROR("Video pipe 0 already stopped\n");
            return;
        }
        pipe2_run_ = false;
        pipe0_run_ = false;
    }

    if (video_thread2 && video_thread2->joinable()) video_thread2->join();
    if (video_thread2 && video_thread0->joinable()) video_thread0->join();

    LOG_DEBUG("Video pipe 0 stopped\n");
}

void Video::video_pipe0_restart() {
    video_pipe0_stop();
    video_pipe0_start();
    LOG_DEBUG("Video pipe 0 restarted\n");
}

void Video::video_pipe1_start() {
    {
        std::lock_guard<std::mutex> lock(mtx_video);
        if (pipe1_run_) {
            LOG_ERROR("Video pipe 1 already started\n");
            return;
        }
        pipe1_run_ = true;
    }

    video_thread1 = std::make_unique<std::thread>(&Video::video_pipe1, this);

    LOG_DEBUG("Video pipe 1 started\n");
}

void Video::video_pipe1_stop() {
    {
        std::lock_guard<std::mutex> lock(mtx_video);
        if (!pipe1_run_) {
            LOG_ERROR("Video pipe 1 already stopped\n");
            return;
        }
        pipe1_run_ = false;
    }

    if (video_thread1 && video_thread1->joinable()) video_thread1->join();

    LOG_DEBUG("Video pipe 1 stopped\n");
}

void Video::video_pipe1_restart() {
    video_pipe1_stop();
    video_pipe1_start();
    LOG_DEBUG("Video pipe 1 restarted\n");
}

void Video::video_pipe2_start() {
    {
        // 线程2依赖线程0
        std::lock_guard<std::mutex> lock(mtx_video);
        int ai_enable = rk_param_get_int("ai:enable", 0);
        // 若ai未使能或者线程2已经启动或者线程1未启动则直接返回
        if (!ai_enable) {
            LOG_ERROR("AI is disabled\n");
            return;
        }
        if (pipe2_run_) {
            LOG_ERROR("Video pipe 2 already started\n");
            return;
        }
        if (!pipe0_run_) {
            LOG_ERROR("Video pipe 0 not started\n");
            return;
        }
        pipe2_run_ = true;  // 设置运行标志位
        video_thread2 = std::make_unique<std::thread>(&Video::video_pipe2, this);
    }

    LOG_DEBUG("Video pipe 2 started\n");
}

void Video::video_pipe2_stop() {
    {
        // 线程2依赖线程0
        std::lock_guard<std::mutex> lock(mtx_video);
        if (!pipe2_run_) {
            LOG_ERROR("Video pipe 2 already stopped\n");
            return;
        }
        pipe2_run_ = false;
    }

    if (video_thread2 && video_thread2->joinable()) video_thread2->join();

    LOG_DEBUG("Video pipe 2 stopped\n");
}

void Video::video_pipe2_restart() {
    video_pipe2_stop();
    video_pipe2_start();
    LOG_DEBUG("Video pipe 2 restarted\n");
}