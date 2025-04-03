#ifndef DEVICES_PIT_H
#define DEVICES_PIT_H

#define PIT_FREQ 1193182

#include <stdint.h>

int init_pit(uint32_t frequency);

#endif
