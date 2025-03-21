#include "kernel/tty.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "kernel/io_ports.h"
#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

void tty_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t *)0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void tty_setcolor(uint8_t color) { terminal_color = color; }

void tty_putentryat(char c, uint8_t color, size_t x, size_t y) {
    size_t const index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void tty_putchar(char c) {
    if (terminal_row == VGA_HEIGHT) {
        tty_scroll();
    }

    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;
        return;
    }

    if (c == '\b') {
        tty_unputchar();
        return;
    }

    tty_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

void tty_unputchar() {
    terminal_column--;

    if (terminal_column < 0) {
        terminal_column = 79;
        terminal_row--;
        // this part is not working
        while ((terminal_buffer[terminal_row * VGA_WIDTH + terminal_column] &
                0x00ff) == ' ') {
            terminal_column--;
        }
    }

    tty_putchar(' ');
    terminal_column--;
}

void tty_scroll() {
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            size_t const index = y * VGA_WIDTH + x;
            terminal_buffer[index] = terminal_buffer[index + VGA_WIDTH];
        }
    }

    for (size_t i = 0; i < VGA_WIDTH; i++) {
        terminal_buffer[VGA_WIDTH * (VGA_HEIGHT - 1) + i] =
            vga_entry(' ', terminal_color);
    }
    terminal_row--;
}

void tty_write(const char *data, size_t size) {
    for (size_t i = 0; i < size; i++)
        tty_putchar(data[i]);
}

void tty_writestring(const char *data) { tty_write(data, strlen(data)); }

void tty_set_cursor() {
    uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}
