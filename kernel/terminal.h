#ifndef TERMINAL_H
#define TERMINAL_H

#define PIXELS_PER_CHARACTER 8 // a character is 8x8 pixels

#include <stdint.h>

int init_terminal();
void terminal_set_foreground_color(uint32_t color);
void terminal_set_background_color(uint32_t color);
void terminal_putchar(char c);
void terminal_unputchar();
void terminal_printf(const char *str);
void terminal_scroll_down(int lineCount);
void terminal_scroll_up(int lineCount);

#endif
