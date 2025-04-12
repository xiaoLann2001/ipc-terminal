#include "luckfox_rgn_draw.h"

// 记录绘制任务批次号
std::mutex idxMutex;
static int batch_idx = 0;

// 全局队列，用于存储绘制任务
std::queue<RgnDrawTask> drawTaskQueue;
std::mutex queueMutex;
std::condition_variable cv_draw_queue;

// 绘制线程运行标志及线程ID
bool rgn_thread_run_ = true;
pthread_t rgn_draw_thread_id;

RK_S32 rgn_draw_nn_init(RGN_HANDLE RgnHandle)
{
	int ret = 0;
	RGN_ATTR_S stRgnAttr;
	MPP_CHN_S stMppChn;
	RGN_CHN_ATTR_S stRgnChnAttr;
	BITMAP_S stBitmap;
	int rotation = 0;

	// create overlay regions
	memset(&stRgnAttr, 0, sizeof(stRgnAttr));
	stRgnAttr.enType = OVERLAY_RGN;
	stRgnAttr.unAttr.stOverlay.enPixelFmt = RK_FMT_2BPP;
	stRgnAttr.unAttr.stOverlay.u32CanvasNum = 1;
	stRgnAttr.unAttr.stOverlay.stSize.u32Width = 2304;
	stRgnAttr.unAttr.stOverlay.stSize.u32Height = 1296;
	ret = RK_MPI_RGN_Create(RgnHandle, &stRgnAttr);
	if (RK_SUCCESS != ret) {
		printf("RK_MPI_RGN_Create (%d) failed with %#x\n", RgnHandle, ret);
		RK_MPI_RGN_Destroy(RgnHandle);
		return RK_FAILURE;
	}
	printf("The handle: %d, create success\n", RgnHandle);
	// after malloc max size, it needs to be set to the actual size
	stRgnAttr.unAttr.stOverlay.stSize.u32Width = 2304;
	stRgnAttr.unAttr.stOverlay.stSize.u32Height = 1296;
	ret = RK_MPI_RGN_SetAttr(RgnHandle, &stRgnAttr);
	if (RK_SUCCESS != ret) {
		printf("RK_MPI_RGN_SetAttr (%d) failed with %#x!", RgnHandle, ret);
		return RK_FAILURE;
	}

	// display overlay regions to venc groups
	memset(&stRgnChnAttr, 0, sizeof(stRgnChnAttr));
	stRgnChnAttr.bShow = RK_TRUE;
	stRgnChnAttr.enType = OVERLAY_RGN;
	stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 0;
	stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 128;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = RgnHandle;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32ColorLUT[0] = 0xFF0000;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32ColorLUT[1] = 0x0000FF;
	stMppChn.enModId = RK_ID_VENC;
	stMppChn.s32DevId = 0;
	stMppChn.s32ChnId = 0;
	ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
	if (RK_SUCCESS != ret) {
		printf("RK_MPI_RGN_AttachToChn (%d) to venc0 failed with %#x\n", RgnHandle, ret);
		return RK_FAILURE;
	}
	printf("RK_MPI_RGN_AttachToChn to venc0 success\n");

	pthread_create(&rgn_draw_thread_id, NULL, rgn_draw_thread, (void *)RgnHandle);
    rgn_thread_run_ = true;

	return ret;
}

int rgn_draw_nn_deinit() {
	if (!rgn_thread_run_) {
		printf("rgn thread is not running\n");
		return 0;
	}
	rgn_thread_run_ = false;
    cv_draw_queue.notify_all();
	pthread_join(rgn_draw_thread_id, NULL);

	int ret = 0;
	// Detach osd from chn
	MPP_CHN_S stMppChn;
	RGN_HANDLE RgnHandle = 0;
	stMppChn.enModId = RK_ID_VENC;
	stMppChn.s32DevId = 0;
	stMppChn.s32ChnId = 0;
	ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
	if (RK_SUCCESS != ret)
		printf("RK_MPI_RGN_DetachFrmChn (%d) to venc0 failed with %#x\n", RgnHandle, ret);

	// destory region
	ret = RK_MPI_RGN_Destroy(RgnHandle);
	if (RK_SUCCESS != ret) {
		printf("RK_MPI_RGN_Destroy [%d] failed with %#x\n", RgnHandle, ret);
	}
	printf("Destory handle:%d success\n", RgnHandle);

	return ret;
}

RK_S32 draw_rect_2bpp(RK_U8 *buffer, RK_U32 width, RK_U32 height, int rgn_x, int rgn_y, int rgn_w,
                      int rgn_h, int line_pixel, COLOR_INDEX_E color_index) {
	int i;
	RK_U8 *ptr = buffer;
	RK_U8 value = 0;
	if (color_index == 0)
		value = 0xff;
	if (color_index == 1)
		value = 0xaa;

	if (line_pixel > 4) {
		printf("line_pixel > 4, not support\n", line_pixel);
		return -1;
	}

	// printf("YUV %dx%d, rgn (%d,%d,%d,%d), line pixel %d\n", width, height, rgn_x, rgn_y, rgn_w,
	//        rgn_h, line_pixel);

	// printf("YUV %dx%d, rgn (%d,%d,%d,%d), line pixel %d\n", width, height, rgn_x, rgn_y, rgn_w,
	// rgn_h, line_pixel); draw top line
	ptr += (width * rgn_y + rgn_x) >> 2;
	for (i = 0; i < line_pixel; i++) {
		memset(ptr, value, (rgn_w + 3) >> 2);
		ptr += width >> 2;
	}
	// draw letft/right line
	for (i = 0; i < (rgn_h - line_pixel * 2); i++) {
		if (color_index == RGN_COLOR_LUT_INDEX_1) {
			*ptr = rgn_color_lut_1_left_value[line_pixel - 1];
			*(ptr + ((rgn_w + 3) >> 2)) = rgn_color_lut_1_right_value[line_pixel - 1];
		} else {
			*ptr = rgn_color_lut_0_left_value[line_pixel - 1];
			*(ptr + ((rgn_w + 3) >> 2)) = rgn_color_lut_0_right_value[line_pixel - 1];
		}
		ptr += width >> 2;
	}
	// draw bottom line
	for (i = 0; i < line_pixel; i++) {
		memset(ptr, value, (rgn_w + 3) >> 2);
		ptr += width >> 2;
	}
	// printf("draw rect success\n");
	return 0;
}

int rgn_draw_queue_push(RgnDrawTask tasks) {
    std::unique_lock<std::mutex> lck(queueMutex);
    drawTaskQueue.push(tasks);
    return 0;
}

int rgn_draw_queue_pop(RgnDrawTask *tasks) {
    std::unique_lock<std::mutex> lck(queueMutex);
    if (drawTaskQueue.empty()) {
        return -1;
    }
    *tasks = drawTaskQueue.front();
    drawTaskQueue.pop();
    return 0;
}

// 添加绘制任务
void rgn_add_draw_task(const RgnDrawParams& params) {
    RgnDrawTask task;
    task.params = params;
    idxMutex.lock();
    task.batch_idx = batch_idx;
    batch_idx = (batch_idx + 1) % INT32_MAX;
    idxMutex.unlock();
    task.batch_num = 1;
    rgn_draw_queue_push(task);
    cv_draw_queue.notify_all();  // 通知绘制线程新任务到来
}

// 批量添加绘制任务，填充绘制批次号，绘制批次数量
void rgn_add_draw_tasks_batch(const std::vector<RgnDrawParams>& params) {
    for (const auto& p : params) {
        RgnDrawTask task;
        task.params = p;
        idxMutex.lock();
        task.batch_idx = batch_idx;
        idxMutex.unlock();
        task.batch_num = params.size();
        // printf("add draw task %d\n", batch_idx);
        // printf("add draw task count %d\n", params.size());
        rgn_draw_queue_push(task);
    }
    idxMutex.lock();
    batch_idx = (batch_idx + 1) % INT32_MAX;
    idxMutex.unlock();
    cv_draw_queue.notify_all();  // 通知绘制线程新任务到来
}

// 绘制线程，支持异步绘制和批量绘制
void *rgn_draw_thread(void *arg) {
	RGN_HANDLE RgnHandle = (RGN_HANDLE)arg;
	int ret;
	int line_pixel = 2;
	RGN_CANVAS_INFO_S stCanvasInfo;

	while (rgn_thread_run_) {
        // usleep(100 * 1000);
        // get canvas info
        ret = RK_MPI_RGN_GetCanvasInfo(RgnHandle, &stCanvasInfo);
        if (ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_RGN_GetCanvasInfo failed with %#x!", ret);
            continue;
        }

        if ((stCanvasInfo.stSize.u32Width % 16 != 0) ||
            (stCanvasInfo.stSize.u32Height % 16 != 0)) {
            printf("The width and height of the canvas must be multiples of 16");
            continue;
        }

        memset((void *)stCanvasInfo.u64VirAddr, 0,
            stCanvasInfo.u32VirWidth * stCanvasInfo.u32VirHeight >> 2);

        // 从队列中获取批次任务并绘制
        RgnDrawParams currentTask;
        int currentBatchIdx = -1;
        int currentBatchCount = 0;

        while (rgn_thread_run_) {
            std::unique_lock<std::mutex> lck(queueMutex);
            
            cv_draw_queue.wait(lck, [] { 
                return !drawTaskQueue.empty() || !rgn_thread_run_; }); // 队列为空时，等待新任务

            if (drawTaskQueue.empty()) {
                lck.unlock();
                return NULL; // 如果队列为空，说明退出标志位为1，退出线程
            }

            RgnDrawTask &peekTask = drawTaskQueue.front();  // 查看队首任务
            
            if (currentBatchIdx == -1) {
                currentBatchIdx = peekTask.batch_idx;  // 初始化批次号
                currentBatchCount = peekTask.batch_num;
                // printf("draw batch %d\n", currentBatchIdx);
                // printf("draw batch count %d\n", currentBatchCount);
            }

            // 检查队首任务是否属于当前批次
            if (peekTask.batch_idx != currentBatchIdx || currentBatchCount == 0) {
                lck.unlock();
                break;  // 队首任务不属于当前批次，退出循环
            }

            // 如果属于同一批次，从队列中取出任务并解锁
            RgnDrawTask currentTask = drawTaskQueue.front();
            drawTaskQueue.pop();
            lck.unlock();

            int x = currentTask.params.x / 2 * 2;
            int y = currentTask.params.y / 2 * 2;
            int w = currentTask.params.w / 2 * 2;
            int h = currentTask.params.h / 2 * 2;
            x = x > 0 ? x : 0;
            y = y > 0 ? y : 0;
            w = w > 0 ? w : 0;
            h = h > 0 ? h : 0;
            if (w == 0 || h == 0) {
                continue;
            }
            if (x + w + line_pixel > stCanvasInfo.stSize.u32Width) {
                w -= line_pixel;
            }
            if (y + h + line_pixel > stCanvasInfo.stSize.u32Height) {
                h -= line_pixel;
            }

            // 绘制任务
            draw_rect_2bpp((RK_U8 *)stCanvasInfo.u64VirAddr, 
                        stCanvasInfo.u32VirWidth, stCanvasInfo.u32VirHeight, 
                        x, y, w, h, line_pixel, 
                        RGN_COLOR_LUT_INDEX_0);
            
            currentBatchCount--;

            if (currentBatchCount <= 0) {
                break;  // 当前批次任务已经绘制完毕，退出循环
            }
        }

        // printf("draw batch %d\n", currentBatchNum);

        ret = RK_MPI_RGN_UpdateCanvas(RgnHandle);
        if (ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_RGN_UpdateCanvas failed with %#x!", ret);
            continue;
        }
    }
    return NULL;
}
