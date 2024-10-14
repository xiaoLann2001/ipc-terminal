#include "display.h"
#include "framebuffer.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <jpeglib.h> // 用于处理 JPEG 图像格式
#include <errno.h>

static int fb_initialized = 0; // 布尔值，标记 framebuffer 是否已初始化

/**
 * @brief 初始化 display 模块。
 *
 * @return 成功返回 0，失败返回 -1。
 */
int display_init(void) {
    if (framebuffer_init(FB_DEVICE) != 0) {
        return -1; // 初始化 framebuffer 失败
    }
    fb_initialized = 1;
    return 0;
}

/**
 * @brief 释放 display 模块资源。
 */
void display_deinit(void) {
    if (fb_initialized) {
        framebuffer_deinit();
        fb_initialized = 0;
    }
}

/**
 * @brief 显示图像到屏幕。
 *
 * @param image_path 图像文件的路径（支持 JPEG 格式）。
 * @param x 图像在屏幕上的 X 坐标。
 * @param y 图像在屏幕上的 Y 坐标。
 * @return 成功返回 0，失败返回 -1。
 */
int display_show_image(const char *image_path, int x, int y) {
    if (!fb_initialized) {
        return -1; // framebuffer 未初始化
    }

    // 打开图像文件
    FILE *file = fopen(image_path, "rb");
    if (!file) {
        perror("Error opening image file");
        return -1;
    }

    // 初始化 JPEG 解码器
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    // 获取图像信息
    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int channels = cinfo.output_components;

    // 分配内存以存储图像数据
    uint8_t *buffer = (uint8_t *)malloc(width * height * channels);
    if (!buffer) {
        perror("Memory allocation failed");
        jpeg_destroy_decompress(&cinfo);
        fclose(file);
        return -1;
    }

    // 读取图像数据
    uint8_t *row_ptr[1];
    while (cinfo.output_scanline < height) {
        row_ptr[0] = buffer + (cinfo.output_scanline) * width * channels;
        jpeg_read_scanlines(&cinfo, row_ptr, 1);
    }

    // 将图像绘制到 framebuffer
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel_index = (i * width + j) * channels;
            uint32_t color = (buffer[pixel_index] << 16) | (buffer[pixel_index + 1] << 8) | buffer[pixel_index + 2];
            framebuffer_set_pixel(x + j, y + i, color);
        }
    }

    // 释放资源
    free(buffer);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(file);
    return 0;
}

/**
 * @brief 绘制文本到屏幕（简单实现，只绘制一个字符）。
 *
 * @param text 要绘制的文本。
 * @param x 文本的 X 坐标。
 * @param y 文本的 Y 坐标。
 * @param color 文本颜色。
 * @return 成功返回 0，失败返回 -1。
 */
int display_draw_text(const char *text, int x, int y, uint32_t color) {
    if (!fb_initialized) {
        return -1; // framebuffer 未初始化
    }
    
    // 这里只是绘制文本的第一个字符作为示例
    if (text && strlen(text) > 0) {
        framebuffer_set_pixel(x, y, color); // 仅示例，实际需要绘制每个字符
    }
    return 0;
}

/**
 * @brief 绘制矩形到屏幕。
 *
 * @param x 矩形的 X 坐标。
 * @param y 矩形的 Y 坐标。
 * @param width 矩形的宽度。
 * @param height 矩形的高度。
 * @param color 矩形颜色。
 * @return 成功返回 0，失败返回 -1。
 */
int display_draw_rectangle(int x, int y, int width, int height, uint32_t color) {
    if (!fb_initialized) {
        return -1; // framebuffer 未初始化
    }
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            framebuffer_set_pixel(x + j, y + i, color);
        }
    }
    return 0;
}

/**
 * @brief 刷新屏幕，确保所有绘图操作生效。
 *
 * @return 成功返回 0，失败返回 -1。
 */
int display_refresh(void) {
    // 在这个简单的实现中，不需要特别的刷新逻辑。
    return 0; // 成功
}
