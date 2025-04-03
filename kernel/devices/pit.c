#include "pit.h"
#include "../include/io.h"
#include "../interrupts/isr.h"
#include "../terminal.h"

static uint32_t freq;

void timer_handler(InterruptFrame *frame) {
  static uint64_t timer_ticks = 0;
  timer_ticks++;

  if (timer_ticks % freq == 0) { // 1 second
    terminal_printf("running for %d seconds\n", timer_ticks / freq);
  }

  outb(0x20, 0x20); // PIC EOI
}

int init_pit(uint32_t frequency) {
  if (frequency > PIT_FREQ) {
    return 0;
  }

  freq = frequency;
  uint16_t divisor = PIT_FREQ / freq;

  outb(0x43, 0x36);

  outb(0x40, divisor & 0xFF);
  outb(0x40, (divisor >> 8) & 0xFF);

  register_interrupt_handler(32, timer_handler);

  return 1;
}
