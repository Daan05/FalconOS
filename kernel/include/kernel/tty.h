#ifndef TTY_HPP
#define TTY_HPP

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void tty_initialize();
void tty_setcolor(uint8_t color);
void tty_putchar(char c);
void tty_scroll();
void tty_write(const char *data, size_t size);
void tty_writestring(const char *data);

#ifdef __cplusplus
}
#endif

#endif
