#include "irq.h"
#include "framebuffer.h"

static uint64_t timer_ticks = 0;

void timer_handler(void) {
  timer_ticks++;
  draw_char(600, 600, 'X', 0xffffff);
}
