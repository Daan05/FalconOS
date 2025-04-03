#ifndef DEVICES_KEYBOARD_H
#define DEVICES_KEYBOARD_H

#include "../interrupts/isr.h"

void keyboard_handler(InterruptFrame *frame);
void init_keyboard();

#endif
