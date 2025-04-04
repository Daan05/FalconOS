#include "isr.h"

void (*interrupt_handlers[256])(InterruptFrame *frame);

void exception_handler(InterruptFrame *frame) {}

void irq_handler(InterruptFrame *frame) {
  if (interrupt_handlers[frame->int_no] != NULL) {
    interrupt_handlers[frame->int_no](frame);
    // terminal_printf("handler is registered | int_no %d\n", frame->int_no);
  } else {
  }
}

void register_interrupt_handler(uint8_t interrupt,
                                void (*handler)(InterruptFrame *frame)) {
  interrupt_handlers[interrupt] = handler;
}
