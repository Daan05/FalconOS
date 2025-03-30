#include "pic.h"
#include "io.h"

void pic_remap(void) {
  uint8_t a1, a2;

  // Save current masks
  a1 = inb(0x21);
  a2 = inb(0xA1);

  // Start initialization in cascade mode
  outb(0x20, 0x11);
  outb(0xA0, 0x11);

  // Set vector offsets: master at 0x20 (32), slave at 0x28 (40)
  outb(0x21, 0x20);
  outb(0xA1, 0x28);

  // Tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
  outb(0x21, 0x04);
  outb(0xA1, 0x02);

  // Set PICs to 8086/88 (MCS-80/85) mode
  outb(0x21, 0x01);
  outb(0xA1, 0x01);

  // Restore saved masks
  outb(0x21, a1);
  outb(0xA1, a2);
}
