#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FB_DEVICE "/dev/fb0"

#define RGB(r, g, b)  (((r) << 16) | ((g) << 8) | (b))
#define RGB_WHITE  0xFFFFFF
#define RGB_BLACK  0x000000
#define RGB_RED    0xFF0000
#define RGB_GREEN  0x00FF00
#define RGB_BLUE   0x0000FF

enum FB_COLOR_FORMAT {
    FB_COLOR_RGB565,
    FB_COLOR_RGB24,
    FB_COLOR_BGR24,
    FB_COLOR_RGBA32,
    FB_COLOR_ARGB32,
};

int framebuffer_init(const char *device);

void framebuffer_deinit(void);

int framebuffer_get_resolution(int *width, int *height, int *bit_depth);

int framebuffer_set_pixel(int x, int y, uint32_t color);

int framebuffer_set_frame(uint32_t *frame, int off_x, int off_y, int img_width, int img_height, enum FB_COLOR_FORMAT format);

int framebuffer_refresh(void);

void framebuffer_clear(uint32_t color);

#ifdef __cplusplus
}
#endif

#endif // __FRAMEBUFFER_H__


