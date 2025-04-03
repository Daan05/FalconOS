#include "pic.h"

#include "../include/io.h"

void remap_pic() {
  // ICW1: Initialize
  outb(0x20, 0x11); // Master PIC
  outb(0xA0, 0x11); // Slave PIC

  // ICW2: Remap IRQ0-IRQ7 to 0x20-0x27
  outb(0x21, 0x20);
  outb(0xA1, 0x28); // IRQ8-IRQ15 to 0x28-0x2F

  // ICW3: Cascade
  outb(0x21, 0x04); // Tell master about slave at IRQ2
  outb(0xA1, 0x02);

  // ICW4: Enable 8086 mode
  outb(0x21, 0x01);
  outb(0xA1, 0x01);

  // Mask all interrupts except PIT(IRQ0) and Keyboard(IRQ1)
  outb(0x21, 0xFC); // 0xFC = 11111100 (unmask IRQ0 and IRQ1)
  outb(0xA1, 0xFF); // Mask all slave IRQs (0xFF = 11111111)
}
