#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_entry {
  uint16_t offset_low;
  uint16_t selector;
  uint8_t ist;
  uint8_t attributes;
  uint16_t offset_mid;
  uint32_t offset_high;
  uint32_t zero;
} __attribute__((packed));

struct idt_ptr {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

#define IDT_SIZE 256

void idt_init(void);
void idt_set_entry(int vector, uint64_t handler, uint16_t selector,
                   uint8_t attributes);

#endif // IDT_H
