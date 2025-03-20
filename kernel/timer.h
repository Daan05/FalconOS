#include "kernel/io_ports.h"
#include "stdio.h"

#define PIT_FREQUENCY 1000 // 1000 Hz

static unsigned long long timerCount = 0;

void timer_handler(uint8_t irq_num) {
    printf("Tick: %d | %d\n", timerCount, irq_num);
    timerCount++;
}

void timer_install() {
    // Configure PIT channel 0 to fire at `PIT_FREQUENCY` Hz
    uint16_t divisor = 1193180 / PIT_FREQUENCY;

    outb(0x43, 0x36);                  // Command port: Channel 0, mode 3
    outb(0x40, divisor & 0xFF);        // Low byte
    outb(0x40, (divisor >> 8) & 0xFF); // High byte
}
