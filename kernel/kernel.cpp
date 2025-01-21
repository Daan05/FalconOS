#include "kernel/tty.h"

#include "vga.h"

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
}
