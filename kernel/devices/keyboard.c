#include "keyboard.h"

#include "../include/io.h"
#include "../terminal.h"

static bool shift_pressed = false;
static bool caps_lock = false;

static char scancode_to_ascii(uint8_t scancode, bool shift) {
  static const char keymap_normal[] = {
      0,    27,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',
      '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',  ']',
      '\n', 0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
      0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,    '*',
      0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
      0,    0,    0,   0,   '-', 0,   0,   0,   '+'};

  static const char keymap_shifted[] = {
      0,    27,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',  '+',
      '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{',  '}',
      '\n', 0,    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',
      0,    '|',  'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,    '*',
      0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
      0,    0,    0,   0,   '-', 0,   0,   0,   '+'};

  if (scancode >= sizeof(keymap_normal))
    return 0;
  return shift ? keymap_shifted[scancode] : keymap_normal[scancode];
}

void keyboard_handler(InterruptFrame *frame) {
  uint8_t scancode = inb(0x60);

  // Key press (scancode < 0x80)
  if (scancode < 0x80) {
    switch (scancode) {
    case 0x2A: // Left Shift press
    case 0x36: // Right Shift press
      shift_pressed = true;
      break;
    case 0x3A:                // Caps Lock press
      caps_lock = !caps_lock; // Toggle state
      break;
    default:
      char c = scancode_to_ascii(scancode, shift_pressed ^ caps_lock);
      if (c != 0) {
        terminal_putchar(c);
      }
      break;
    }
  } else {
    // Key release (scancode >= 0x80)
    uint8_t released = scancode - 0x80;
    switch (released) {
    case 0x2A: // Left Shift release
    case 0x36: // Right Shift release
      shift_pressed = false;
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
