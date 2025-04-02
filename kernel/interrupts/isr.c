#include "isr.h"

#include "../terminal.h"

void (*interrupt_handlers[256])(InterruptFrame *frame);

void exceptionHandler(InterruptFrame *frame) {
  // printNumber(frame->int_no);
  // printNumber(frame->err_code);
  terminal_set_foreground_color(0xff0000);
  terminal_printf("INTERRUPT >> int_no: %d, err_code: %d\n", frame->int_no,
                  frame->err_code);
  terminal_set_foreground_color(0xffffff);
}

void registerInterruptHandler(uint8_t interrupt,
                              void (*handler)(InterruptFrame *frame)) {
  interrupt_handlers[interrupt] = handler;
}

void irqHandler(InterruptFrame *frame) {
  if (&interrupt_handlers[frame->int_no] != NULL) {
    interrupt_handlers[frame->int_no](frame);
  }

  terminal_printf("some interrupt 32-255\n");
}
