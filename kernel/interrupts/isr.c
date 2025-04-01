#include "isr.h"

#include "../drivers/framebuffer.h"

void (*interrupt_handlers[256])(InterruptFrame *frame);

void exceptionHandler(InterruptFrame *frame) {
  // printNumber(frame->int_no);
  // printNumber(frame->err_code);
  draw_string(600, 16, "some interrupt just happened!", 0xff0000);
}

void registerInterruptHandler(uint8_t interrupt,
                              void (*handler)(InterruptFrame *frame)) {
  interrupt_handlers[interrupt] = handler;
}

void irqHandler(InterruptFrame *frame) {
  if (&interrupt_handlers[frame->int_no] != NULL) {
    interrupt_handlers[frame->int_no](frame);
  }

  draw_string(600, 32, "some interrupt 32-255", 0xff0000);
}
