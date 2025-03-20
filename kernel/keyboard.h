#include "kernel/io_ports.h"
#include "stdio.h"

void keyboard_handler(uint8_t irq_num) {
    uint8_t scancode = inb(0x60); // Read scancode from keyboard controller

    printf("Keypressed: %d | %d\n", scancode, irq_num);
}

void keyboard_install() {
    // Enable keyboard IRQ (IRQ1)
    outb(0x21, inb(0x21) & ~0x02); // Unmask IRQ1 in PIC1
}
