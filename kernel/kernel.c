#include "kernel/tty.h"

#include "isr.h"
#include "gdt.h"
#include "idt.h"
#include "vga.h"
#include "keyboard.h"

#include <stdint.h>
#include "stdio.h"

void kernel_main(void) {
    gdt_load();
    tty_initialize();
    idt_init();
    idt_load();

    // set the fg and bg color
    tty_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));

    timer_install(); // Start timer

    printf("Welcome to FOS\n");
    printf("Falcon OS\n\n");
    printf("without a doubt the best OS. GNU/Linux shivvering rn\n\n");

    tty_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    while (1) {
        poll_keyboard();
        tty_set_cursor();
    }
}
