#ifndef TIMER_DRIVER_C
#define TIMER_DRIVER_C

#include "kernel/io_ports.h"
#include "stdio.h"

#include "stdint.h"

#define PIT_FREQUENCY 1000 // 1000 Hz

void timer_handler(uint8_t irq_num);
void timer_install();
int getTimerFrequency();
uint64_t getTicks();
#endif
