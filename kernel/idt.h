#include <stdint.h>

#include "kernel/io_ports.h"

#ifdef __cplusplus
extern "C" {
#endif

// Define IDT entry structure
struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_entry idt[256];

// Set an IDT entry
void idt_set_gate(uint8_t num, void (*handler)()) {
    uint32_t offset = (uint32_t)handler;
    idt[num].offset_low = offset & 0xFFFF;
    idt[num].selector = 0x08; // Kernel code segment
    idt[num].zero = 0;
    idt[num].type_attr = 0x8E; // 0x8E = interrupt gate (present, ring 0)
    idt[num].offset_high = (offset >> 16) & 0xFFFF;
}

void pic_remap() {
    outb(0x20, 0x11); // ICW1: Initialize PIC1
    outb(0xA0, 0x11); // ICW1: Initialize PIC2

    outb(0x21, 0x20); // ICW2: PIC1 handles IRQ0-7 (interrupts 0x20-0x27)
    outb(0xA1, 0x28); // ICW2: PIC2 handles IRQ8-15 (interrupts 0x28-0x2F)

    outb(0x21, 0x04); // ICW3: PIC1 is connected via IRQ2
    outb(0xA1, 0x02); // ICW3: PIC2 is cascaded to PIC1

    outb(0x21, 0x01); // ICW4: Enable 8086 mode for PIC1
    outb(0xA1, 0x01); // ICW4: Enable 8086 mode for PIC2

    // Mask all interrupts except IRQ0 (timer) and IRQ1 (keyboard)
    outb(0x21, 0xFC);
    outb(0xA1, 0xFF);
}

void __attribute__((naked)) isr_default() {
    asm volatile("push $0\n\t"
                 "push $255\n\t"
                 "jmp isr_common_stub\n\t");
}

void idt_init() {
    pic_remap();

    // Set exceptions 0-31 to their correct handlers
    idt_set_gate(0, isr0);   // Divide-by-zero
    idt_set_gate(1, isr1);   // Debug
    idt_set_gate(2, isr2);   // NMI
    idt_set_gate(3, isr3);   // Breakpoint
    idt_set_gate(4, isr4);   // Overflow
    idt_set_gate(5, isr5);   // BOUND Range Exceeded
    idt_set_gate(6, isr6);   // Invalid Opcode
    idt_set_gate(7, isr7);   // Device Not Available
    idt_set_gate(8, isr8);   // Double Fault (ERRCODE)
    idt_set_gate(9, isr9);   // Coprocessor Segment Overrun
    idt_set_gate(10, isr10); // Invalid TSS (ERRCODE)
    idt_set_gate(11, isr11); // Segment Not Present (ERRCODE)
    idt_set_gate(12, isr12); // Stack-Segment Fault (ERRCODE)
    idt_set_gate(13, isr13); // General Protection Fault (ERRCODE)
    idt_set_gate(14, isr14); // Page Fault (ERRCODE)
    // ... Define remaining exceptions up to 31

    // Set IRQs 32-255 to the default handler
    for (int i = 32; i < 256; i++) {
        idt_set_gate(i, isr_default);
    }

    // Map IRQ0 (timer) to IDT entry 0x20
    idt_set_gate(33, irq0);
    // Map IRQ1 (keyboard) to IDT entry 0x21
    idt_set_gate(32, irq1);

    idt_set_gate(128, isr128);
    idt_set_gate(129, isr129);
    idt_set_gate(130, isr130);
    idt_set_gate(131, isr131);
    idt_set_gate(132, isr132);
}

// Load IDT and enable interrupts
void idt_load() {
    struct {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) idt_desc = {.limit = sizeof(idt) - 1,
                                          .base = (uint32_t)&idt};

    asm volatile("lidt %0" : : "m"(idt_desc));
    asm volatile("sti"); // Enable interrupts
}

#ifdef __cplusplus
}
#endif
