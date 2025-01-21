#include "stdio.h"

#include "kernel/tty.h"

int printf(const char *__restrict format, ...) {
    tty_writestring(format);
    return 0;
}
