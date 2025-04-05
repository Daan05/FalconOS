#ifndef CPU_INFO_H
#define CPU_INFO_H

#include <stdint.h>

void cpuid(uint32_t eax, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);
void detect_cpu();

#endif
