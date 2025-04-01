#include "idt.h"

extern void *isr_stub_table[];
extern void *irq_stub_table[];

IDTEntry idt_entries[256];

IDTPointer idt_ptr =
    (IDTPointer){(uint16_t)sizeof(idt_entries) - 1, (uintptr_t)&idt_entries[0]};

void set_idt_entry(IDTEntry *target, uint64_t offset, uint16_t selector,
                   uint8_t ist, uint8_t type_attributes) {
  target->offset_1 = offset & 0xFFFF;
  target->selector = selector;
  target->ist = ist;
  target->type_attributes = type_attributes;
  target->offset_2 = (offset >> 16) & 0xFFFF;
  target->offset_3 = (offset >> 32) & 0xFFFFFFFF;
  target->zero = 0;
}

void init_idt() {
  for (uint8_t i = 0; i < 32; i++) {
    set_idt_entry(&idt_entries[i], (uint64_t)isr_stub_table[i], 0x28, 0, 0x8E);
  }
  for (uint8_t i = 32; i < 255; i++) {
    set_idt_entry(&idt_entries[i], (uint64_t)irq_stub_table[i - 32], 0x28, 0,
                  0x8E);
  }
  __asm__ volatile("lidt %0" : : "m"(idt_ptr));
  __asm__ volatile("sti");
}
