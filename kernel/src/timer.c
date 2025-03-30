#include "timer.h"
#include "io.h"
#include "pic.h"

void timer_init(uint32_t frequency) {
  // Remap the PIC so that IRQ0 (timer) is at vector 32
  pic_remap();

  // The PIT input clock frequency is ~1193182 Hz.
  uint32_t divisor = 1193182 / frequency;

  // Set PIT to mode 3 (square wave) on channel 0.
  outb(0x43, 0x36);
  outb(0x40, (uint8_t)(divisor & 0xFF));        // low byte
  outb(0x40, (uint8_t)((divisor >> 8) & 0xFF)); // high byte
}
