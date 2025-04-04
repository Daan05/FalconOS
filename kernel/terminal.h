#ifndef TERMINAL_H
#define TERMINAL_H

#define PIXELS_PER_CHARACTER 8 // a character is 8x8 pixels

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

int init_terminal();
void terminal_set_foreground_color(uint32_t color);
void terminal_set_background_color(uint32_t color);
void terminal_putchar(char c);
void terminal_unputchar();
void terminal_printf(const char *str, ...);
void terminal_scroll_down(int num_lines);
void terminal_scroll_up(int num_lines);

#endif
