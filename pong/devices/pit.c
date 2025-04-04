#include "pit.h"
#include "../include/io.h"
#include "../interrupts/isr.h"
#include "../pong.h"

static uint32_t freq;

void timer_handler(InterruptFrame *frame) {
  static uint64_t timer_ticks = 0;
  timer_ticks++;

  update_game();

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

  register_interrupt_handler(0x20, timer_handler);

  return 1;
}
