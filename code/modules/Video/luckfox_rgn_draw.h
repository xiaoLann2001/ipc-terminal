#include <pthread.h>
#include <queue>
#include <iostream>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

#include "sample_comm.h"
#include "yolov5.h"
#include "postprocess.h"

struct RgnDrawParams {
    RGN_HANDLE RgnHandle;   // 区域句柄
    int x, y, w, h;         // 绘制区域的坐标
    int line_pixel;         // 线条粗细
};

struct RgnDrawTask {
    RgnDrawParams params;
    int batch_idx;    // 批量绘制的批次
    int batch_num;    // 批量绘制的数量
};

typedef enum rkCOLOR_INDEX_E {
	RGN_COLOR_LUT_INDEX_0 = 0,
	RGN_COLOR_LUT_INDEX_1 = 1,
} COLOR_INDEX_E;

static RK_U8 rgn_color_lut_0_left_value[4] = {0x03, 0xf, 0x3f, 0xff};
static RK_U8 rgn_color_lut_0_right_value[4] = {0xc0, 0xf0, 0xfc, 0xff};
static RK_U8 rgn_color_lut_1_left_value[4] = {0x02, 0xa, 0x2a, 0xaa};
static RK_U8 rgn_color_lut_1_right_value[4] = {0x80, 0xa0, 0xa8, 0xaa};

RK_S32 rgn_draw_nn_init(RGN_HANDLE RgnHandle);
int rgn_draw_nn_deinit();
// RK_S32 draw_rect_2bpp(RK_U8 *buffer, RK_U32 width, RK_U32 height, int rgn_x, int rgn_y, int rgn_w,
//                       int rgn_h, int line_pixel, COLOR_INDEX_E color_index);
// int rgn_draw_queue_push(RgnDrawParams params);
// int rgn_draw_queue_pop(RgnDrawParams *params);
void rgn_add_draw_task(const RgnDrawParams& task);
void rgn_add_draw_tasks_batch(const std::vector<RgnDrawParams>& tasks);
void *rgn_draw_thread(void *arg);