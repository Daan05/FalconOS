#include "isr.h"

#include "../terminal.h"

void (*interrupt_handlers[256])(InterruptFrame *frame);

void exception_handler(InterruptFrame *frame) {
  terminal_printf("CPU exception | int_no: %d, err_code: %d\n", frame->int_no,
                  frame->err_code);
}

void irq_handler(InterruptFrame *frame) {
  if (interrupt_handlers[frame->int_no] != NULL) {
    interrupt_handlers[frame->int_no](frame);
    // terminal_printf("handler is registered | int_no %d\n", frame->int_no);
  } else {
    terminal_printf("handler is not registered | int_no %d\n", frame->int_no);
  }
}

void register_interrupt_handler(uint8_t interrupt,
                                void (*handler)(InterruptFrame *frame)) {
  interrupt_handlers[interrupt] = handler;
}
