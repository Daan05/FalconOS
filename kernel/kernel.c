#include "kernel/io_ports.h"
#include "kernel/tty.h"

#include "isr.h"
#include "gdt.h"
#include "idt.h"
#include "vga.h"

#include <stdint.h>
#include <stdio.h>

void kernel_main(void) {
    gdt_load();
    tty_initialize();
    idt_init();
    idt_load();

    // set the fg and bg color
    tty_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));

    timer_install();    // Start timer
    keyboard_install(); // Enable keyboard

    printf("Timer and keyboard initialized!\n");

    while (1) {
        // Kernel idle loop
        asm volatile("hlt");
    }
}
