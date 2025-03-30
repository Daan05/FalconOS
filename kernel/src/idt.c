#include "idt.h"
#include "string.h"

// Global IDT and pointer
struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;

// External assembly function to load the IDT
extern void idt_flush(uint64_t idtp_ptr);

void idt_set_entry(int vector, uint64_t handler, uint16_t selector,
                   uint8_t attributes) {
  idt[vector].offset_low = handler & 0xFFFF;
  idt[vector].selector = selector;
  idt[vector].ist = 0;
  idt[vector].attributes = attributes;
  idt[vector].offset_mid = (handler >> 16) & 0xFFFF;
  idt[vector].offset_high = (handler >> 32) & 0xFFFFFFFF;
  idt[vector].zero = 0;
}

void idt_init(void) {
  idtp.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
  idtp.base = (uint64_t)&idt;
  memset(idt, 0, sizeof(struct idt_entry) * IDT_SIZE);

  // Set up timer interrupt (IRQ0 is remapped to vector 32)
  extern void timer_interrupt_handler();
  idt_set_entry(32, (uint64_t)timer_interrupt_handler, 0x08, 0x8E);

  // Load our IDT
  idt_flush((uint64_t)&idtp);
}
