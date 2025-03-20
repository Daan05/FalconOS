#include "keyboard.h"

int poll_keyboard() {
    if (inb(0x64) & 1) { // Check if there's data available
        uint8_t scancode = inb(0x60);
        if (scancode < 0x80) { // Key press (not release)
            char c = scancode_to_ascii(scancode);
            if (c != 0)
                putchar(c);
        }
        return 1;
    }
    return 0;
}
