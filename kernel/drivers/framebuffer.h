#ifndef DRIVERS_FRAMEBUFFER_H
#define DRIVERS_FRAMEBUFFER_H

#include <limine.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

int initialize_framebuffer();
void putpixel(int x, int y, uint32_t color);
void draw_char(int x, int y, unsigned char c, uint32_t color);
void draw_string(int x, int y, const char *str, uint32_t color);

#endif
