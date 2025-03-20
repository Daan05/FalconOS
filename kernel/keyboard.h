#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include "kernel/io_ports.h"
#include "stdio.h"

static char scancode_to_ascii(uint8_t scancode) {
    static const char keymap[] = {
        0,    27,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',
        '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',  ']',
        '\n', 0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,    '*',
        0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,
        0,    0,    0,   0,   '-', 0,   0,   0,   '+'};
    return (scancode < sizeof(keymap)) ? keymap[scancode] : 0;
}

int poll_keyboard();

#endif
