#include "timer.h"

static uint64_t tickCount = 0;

void timer_handler(uint8_t irq_num) {
    // printf("Tick: %d | %d\n", timerCount, irq_num);
    tickCount++;
}

void timer_install() {
    // Configure PIT channel 0 to fire at `PIT_FREQUENCY` Hz
    uint16_t divisor = 1193180 / PIT_FREQUENCY;

    outb(0x43, 0x36);                  // Command port: Channel 0, mode 3
    outb(0x40, divisor & 0xFF);        // Low byte
    outb(0x40, (divisor >> 8) & 0xFF); // High byte
}

int getTimerFrequency() { return PIT_FREQUENCY; }

uint64_t getTicks() { return tickCount; }
