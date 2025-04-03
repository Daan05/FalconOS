#include "keyboard.h"

#include "../include/io.h"
#include "../pong.h"

void keyboard_handler(InterruptFrame *frame) {
  uint8_t scancode = inb(0x60);

  // Key press (scancode < 0x80)
  if (scancode < 0x80) {
    switch (scancode) {
    case 0x11: // W press
      set_key_lu_down(1);
      break;
    case 0x1F: // S press
      set_key_ld_down(1);
      break;
    case 0x17: // I press
      set_key_ru_down(1);
      break;
    case 0x25: // K press
      set_key_rd_down(1);
      break;
    default:
      break;
    }
  } else {
    // Key release (scancode >= 0x80)
    uint8_t released = scancode - 0x80;
    switch (released) {
    case 0x11: // W press
      set_key_lu_down(0);
      break;
    case 0x1F: // S press
      set_key_ld_down(0);
      break;
    case 0x17: // I up press
      set_key_ru_down(0);
      break;
    case 0x25: // K down press
      set_key_rd_down(0);
      break;
    default:
      break;
    }
  }

  outb(0x20, 0x20); // EOI
}

void init_keyboard() {
  // Initialize PS/2 controller
  int timeout = 1000;
  while ((inb(0x64) & 0x02) && timeout-- > 0) {
  } // Wait for input buffer

  outb(0x64, 0xAE); // Enable first PS/2 port
  outb(0x60, 0xF4); // Enable keyboard

  // Flush any pending data
  while (inb(0x64) & 0x01) {
    (void)inb(0x60);
  }

  // Register handler
  register_interrupt_handler(0x21, keyboard_handler);
}
