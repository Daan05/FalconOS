#include "kernel/tty.h"
#include "stdio.h"

void putchar(char c) {
    tty_putchar(c);
    tty_set_cursor;
}
