#include "framebuffer.h"

static int fb_fd = -1;  // Framebuffer 文件描述符
static struct fb_var_screeninfo vinfo;  // 可变的屏幕信息
static struct fb_fix_screeninfo finfo;  // 固定的屏幕信息
static uint8_t *fb_ptr = NULL;  // 内存映射指针
static long screensize = 0;  // 屏幕内存大小

/**
 * @brief 初始化 Framebuffer，映射内存。
 *
 * @param device Framebuffer 设备路径，例如 "/dev/fb0"。
 * @return 成功返回 0，失败返回 -1。
 */
int framebuffer_init(const char *device) {
    fb_fd = open(device, O_RDWR);
    if (fb_fd == -1) {
        perror("Error opening framebuffer device");
        return -1;
    }

    // 获取固定屏幕信息
    if (ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo)) {
        perror("Error reading fixed information");
        return -1;
    }

    // 获取可变屏幕信息
    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo)) {
        perror("Error reading variable information");
        return -1;
    }

    // 计算屏幕的总大小 (行数 * 每行字节数)
    screensize = vinfo.yres_virtual * finfo.line_length;

    // 使用 mmap 将 framebuffer 映射到内存
    fb_ptr = (uint8_t *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if ((intptr_t)fb_ptr == -1) {
        perror("Error mapping framebuffer device to memory");
        return -1;
    }

    return 0;
}

/**
 * @brief 释放 Framebuffer 资源，取消内存映射。
 */
void framebuffer_deinit(void) {
    if (fb_ptr != NULL) {
        munmap(fb_ptr, screensize);
        fb_ptr = NULL;
    }
    if (fb_fd != -1) {
        close(fb_fd);
        fb_fd = -1;
    }
}

/**
 * @brief 获取 Framebuffer 的分辨率和位深。
 *
 * @param width 返回宽度。
 * @param height 返回高度。
 * @param bit_depth 返回位深。
 * @return 成功返回 0，失败返回 -1。
 */
int framebuffer_get_resolution(int *width, int *height, int *bit_depth) {
    if (fb_fd == -1) {
        return -1;
    }
    *width = vinfo.xres;
    *height = vinfo.yres;
    *bit_depth = vinfo.bits_per_pixel;
    return 0;
}

/**
 * @brief 在指定位置设置像素颜色。
 *
 * @param x 坐标 X。
 * @param y 坐标 Y。
 * @param color 要设置的颜色值。
 * @return 成功返回 0，失败返回 -1。
 */
int framebuffer_set_pixel(int x, int y, uint32_t color) {
    if (fb_fd == -1 || x < 0 || y < 0 || x >= vinfo.xres || y >= vinfo.yres) {
        return -1;
    }

    // 计算像素在 framebuffer 内存中的偏移
    long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                    (y + vinfo.yoffset) * finfo.line_length;

    // 根据屏幕的位深设置像素颜色
    if (vinfo.bits_per_pixel == 32) {
        *((uint32_t *)(fb_ptr + location)) = color;
    } else if (vinfo.bits_per_pixel == 16) {
        uint16_t b = (color & 0xFF) >> 3;
        uint16_t g = ((color >> 8) & 0xFF) >> 2;
        uint16_t r = ((color >> 16) & 0xFF) >> 3;
        uint16_t rgb = (r << 11) | (g << 5) | b;
        *((uint16_t *)(fb_ptr + location)) = rgb;
    } else {
        // 不支持的位深
        return -1;
    }
    return 0;
}

/**
 * @brief 将 32 位颜色值转换为 32 位颜色值。
 *
 * @param color 要转换的颜色值。
 * @param format 颜色格式。
 * @return 转换后的 32 位颜色值。
 */
uint32_t convert_color_bit32(uint32_t color, enum FB_COLOR_FORMAT format) {
    switch (format) {
        case FB_COLOR_ARGB32: {
            return color;  // 已经是 ARGB32 格式
        }
        case FB_COLOR_RGBA32: {
            // 从 RGBA32 转换为 ARGB32
            return ((color & 0xFF) << 24) | (color >> 8);
        }
        default:
            return 0;  // 不支持的格式，返回 0
    }
}

/**
 * @brief 将 32 位颜色值转换为 16 位颜色值。
 *
 * @param color 要转换的颜色值。
 * @param format 颜色格式。
 * @return 转换后的 16 位颜色值。
 */
uint16_t convert_color_bit16(uint32_t color, enum FB_COLOR_FORMAT format) {
    switch (format) {
        case FB_COLOR_RGB565: {
            return color;  // 已经是 RGB565 格式
        }
        case FB_COLOR_RGB24: {
            // 从 RGB24 转换为 RGB565
            uint16_t b = (color & 0xFF) >> 3;           // 5 bits for blue
            uint16_t g = ((color >> 8) & 0xFF) >> 2;    // 6 bits for green
            uint16_t r = ((color >> 16) & 0xFF) >> 3;   // 5 bits for red
            return (r << 11) | (g << 5) | b;            // 返回 RGB565 颜色
        }
        case FB_COLOR_BGR24: {
            // 从 BGR24 转换为 RGB565
            uint16_t r = (color & 0xFF) >> 3;           // 5 bits for red
            uint16_t g = ((color >> 8) & 0xFF) >> 2;    // 6 bits for green
            uint16_t b = ((color >> 16) & 0xFF) >> 3;   // 5 bits for blue
            return (r << 11) | (g << 5) | b;            // 返回 RGB565 颜色
        }
        case FB_COLOR_RGBA32: {
            // 从 RGBA32 转换为 RGB565
            uint16_t b = (color & 0xFF) >> 3;           // 5 bits for blue
            uint16_t g = ((color >> 8) & 0xFF) >> 2;    // 6 bits for green
            uint16_t r = ((color >> 16) & 0xFF) >> 3;   // 5 bits for red
            return (r << 11) | (g << 5) | b;            // 返回 RGB565 颜色
        }
        case FB_COLOR_ARGB32: {
            // 从 ARGB32 转换为 RGB565
            uint16_t b = (color & 0xFF) >> 3;           // 5 bits for blue
            uint16_t g = ((color >> 8) & 0xFF) >> 2;    // 6 bits for green
            uint16_t r = ((color >> 16) & 0xFF) >> 3;   // 5 bits for red
            return (r << 11) | (g << 5) | b;            // 返回 RGB565 颜色
        }
        default:
            return 0;  // 不支持的格式，返回 0
    }
}

/**
 * @brief 在指定位置绘制帧。
 * 
 * @param frame 帧数据的指针。 注意要四字节对齐。
 * @param off_x 起始 X 坐标。
 * @param off_y 起始 Y 坐标。
 * @param img_width 图像宽度。
 * @param img_height 图像高度。
 * @param format 帧数据的颜色格式。
 * @return 成功返回 0，失败返回 -1。
 */
int framebuffer_set_region(uint32_t *frame, int off_x, int off_y, int img_width, int img_height, enum FB_COLOR_FORMAT format) {
    if (fb_fd == -1) {
        fprintf(stderr, "Framebuffer not initialized\n");
        return -1;
    }

    // 边界检查，确保偏移在 framebuffer 范围内
    if (off_x < 0 || off_y < 0 || off_x >= vinfo.xres || off_y >= vinfo.yres) {
        fprintf(stderr, "Invalid offset: %d, %d\n", off_x, off_y);
        return -1;
    }

    // 计算绘制区域的实际宽度和高度（不能超出屏幕或图像的范围）
    int draw_width = (off_x + img_width > vinfo.xres) ? vinfo.xres - off_x : img_width;
    int draw_height = (off_y + img_height > vinfo.yres) ? vinfo.yres - off_y : img_height;

    int x, y;
    if (vinfo.bits_per_pixel == 32) {
        uint8_t *fb_draw = fb_ptr + (off_y * finfo.line_length + off_x * 4);  // 计算帧缓冲区中的起始位置
        for (y = 0; y < draw_height; y++) {
            for (x = 0; x < draw_width; x++) {
                uint32_t color = frame[y * img_width + x];
                *((uint32_t *)(fb_draw + x * 4)) = convert_color_bit32(color, format);
            }
            fb_draw += finfo.line_length;  // 跳到下一行 framebuffer 行起始位置
        }
    } else if (vinfo.bits_per_pixel == 16) {
        uint8_t *fb_draw = fb_ptr + (off_y * finfo.line_length + off_x * 2);  // 计算帧缓冲区中的起始位置
        for (y = 0; y < draw_height; y++) {
            for (x = 0; x < draw_width; x++) {
                uint32_t color = frame[y * img_width + x];
                *((uint16_t *)(fb_draw + x * 2)) = convert_color_bit16(color, format);
            }
            fb_draw += finfo.line_length;  // 跳到下一行 framebuffer 行起始位置
        }
    } else {
        fprintf(stderr, "Unsupported bit depth: %d\n", vinfo.bits_per_pixel);
        return -1;
    }
    return 0;
}

/**
 * @brief 在 framebuffer 中绘制 RGB565 格式的帧。
 * 
 * @param frame 帧数据的指针。 注意要两字节对齐。
 * @param width 帧的宽度。
 * @param height 帧的高度。
 * @return 成功返回 0，失败返回 -1。
 */
int framebuffer_set_frame_rgb565(uint16_t *frame, int width, int height) {
    if (fb_fd == -1) {
        fprintf(stderr, "Framebuffer not initialized\n");
        return -1;
    }

    // 假设输入数据是目标分辨率的，直接使用 memcpy 复制到 framebuffer
    if (vinfo.bits_per_pixel == 16) {
        memcpy(fb_ptr, frame, width * height * 2);
    } else {
        fprintf(stderr, "Unsupported bit depth: %d\n", vinfo.bits_per_pixel);
        return -1;
    }

    return 0;
}

/**
 * @brief 刷新 Framebuffer (如果需要的话)。
 *
 * @return 成功返回 0，失败返回 -1。
 */
int framebuffer_refresh(void) {
    // 对于多数 framebuffer 不需要刷新，通常由硬件处理。
    // 如果有双缓冲等情况，可以在这里添加相关逻辑。
    return 0;
}

/**
 * @brief 用指定的颜色清空屏幕。
 *
 * @param color 用于清空屏幕的颜色值。
 */
void framebuffer_clear(uint32_t color) {
    int x, y;
    for (y = 0; y < vinfo.yres; y++) {
        for (x = 0; x < vinfo.xres; x++) {
            framebuffer_set_pixel(x, y, color);
        }
    }
}
