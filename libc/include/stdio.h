#ifndef LIBC_STDIO_H
#define LIBC_STDIO_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
