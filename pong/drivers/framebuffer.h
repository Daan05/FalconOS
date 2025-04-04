#ifndef DRIVERS_FRAMEBUFFER_H
#define DRIVERS_FRAMEBUFFER_H

#include <limine.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

int init_framebuffer();
void putpixel(int x, int y, uint32_t color);
void draw_char(int x, int y, unsigned char c, uint32_t color);
void draw_hline(int x, int y, int width, uint32_t color);
void draw_circle(int x, int y, int radius, uint32_t color);
void fill_rect(int x, int y, int width, int height, uint32_t color);
void clear_screen(uint32_t color);
int init_framebuffer();
struct limine_framebuffer *get_framebuffer();

#endif
