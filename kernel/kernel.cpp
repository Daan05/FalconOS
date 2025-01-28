#include "kernel/io_ports.h"
#include "kernel/tty.h"

#include "vga.h"

#include <stdint.h>
#include <stdio.h>

extern "C" void kernel_main(void) {
    // initialize the tty before doing anything
    tty_initialize();

    // set the fg and bg color
    tty_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));

    // print whatever you want
    printf("Welcome to FOS\n");
    printf("Falcon OS\n");
    printf("\n");
    printf("without a doubt the best OS. GNU/Linux shivvering rn\n");
    printf("\n");
    printf("Hello, World!\n");
    printf("Character: %c\n", 'A');
    printf("String: %s\n", "Test");
    printf("Signed Int: %d\n", -123);
    printf("Unsigned Int: %u\n", 456);
    printf("Hexadecimal (lowercase): %x\n", 255);
    printf("Hexadecimal (uppercase): %X\n", 255);
    printf("Percent: %%\n");

    while (true) {
        uint8_t scancode = inb(0x60);
        if (scancode < 128)
            tty_putchar(scancode + 'A');
    }
}
