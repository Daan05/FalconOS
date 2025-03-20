#include <stdint.h>
#include "stdio.h"
#include "drivers/timer.h"
#include "kernel/io_ports.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void isr0();
extern void isr1();
extern void isr128(); // System call
extern void isr129(); // System call
extern void isr130(); // System call
extern void isr131(); // System call
extern void isr132(); // System call

#define IRQ(num)                                                               \
    void __attribute__((naked)) irq##num() {                                   \
        asm volatile("push $0\n\t"        /* Dummy error code */               \
                     "push $" #num "\n\t" /* IRQ number (0-15) */              \
                     "jmp irq_common_stub\n\t" ::                              \
                         : "memory");                                          \
    }

void __attribute__((naked)) irq_common_stub() {
    asm volatile(
        /* Save registers and DS (same as ISR stub) */
        "pusha\n\t"
        "mov %%ds, %%eax\n\t"
        "push %%eax\n\t"

        /* Load kernel data segments */
        "mov $0x10, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"

        /* Call C handler */
        "push 0x28(%%esp)\n\t" // Push IRQ number
        "call irq_handler\n\t"
        "add $4, %%esp\n\t" // Cleanup IRQ number

        /* Restore registers and DS */
        "pop %%eax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        "popa\n\t"

        /* Cleanup error code and IRQ number */
        "add $8, %%esp\n\t"

        /* Return from interrupt */
        "iret\n\t" ::
            : "eax", "memory");
}

void irq_handler(uint8_t irq_num) {
    switch (irq_num) {
    case 0:
        timer_handler(irq_num);
        break;
        // ... handle other IRQs
    }

    // Send End-of-Interrupt (EOI) to PIC
    if (irq_num >= 8)
        outb(0xA0, 0x20); // Notify slave PIC
    outb(0x20, 0x20);     // Notify master PIC
}

// Define IRQ handlers for timer (IRQ0) and keyboard (IRQ1)
IRQ(0) // Timer

// Macro for ISRs without error codes
#define ISR_NOERRCODE(num)                                                     \
    void __attribute__((naked)) isr##num() {                                   \
        asm volatile("push $" #num "\n\t" /* Interrupt number */               \
                     "push $0\n\t"        /* Dummy error code */               \
                     "jmp isr_common_stub\n\t" ::                              \
                         : "memory");                                          \
    }

// Macro for ISRs with error codes (e.g., exceptions 8, 10-14)
#define ISR_ERRCODE(num)                                                       \
    void __attribute__((naked)) isr##num() {                                   \
        asm volatile("push $" #num "\n\t" /* Interrupt number */               \
                     "jmp isr_common_stub\n\t" ::                              \
                         : "memory");                                          \
    }

// Common ISR handler stub
void __attribute__((naked)) isr_common_stub() {
    asm volatile(
        /* Save registers and DS */
        "pusha\n\t"
        "mov %%ds, %%eax\n\t"
        "push %%eax\n\t"

        /* Load kernel data segments */
        "mov $0x10, %%ax\n\t" // 0x10 = data segment selector
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"

        /* Pass int_no and err_code to C handler */
        "mov 0x28(%%esp), %%eax\n\t" // Interrupt number (was pushed first)
        "mov 0x24(%%esp), %%ebx\n\t" // Error code (was pushed second)
        "push %%ebx\n\t"             // Push err_code (2nd arg)
        "push %%eax\n\t"             // Push int_no (1st arg)
        "call isr_handler\n\t"
        "add $8, %%esp\n\t" // Cleanup args

        /* Restore original DS and registers */
        "pop %%eax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        "popa\n\t"

        /* Cleanup error code and int_no */
        "add $8, %%esp\n\t"

        /* Return from interrupt */
        "iret\n\t" ::
            : "eax", "ebx", "memory");
}

// C ISR handler (called from assembly)
void isr_handler(uint32_t int_no, uint32_t err_code) {
    printf("Interrupt: 0x%x, Error: 0x%x\n", int_no, err_code);
}

// Define ISRs using the fixed macros
ISR_NOERRCODE(0)   // Divide-by-zero
ISR_NOERRCODE(1)   // Debug
ISR_NOERRCODE(2)   // NMI
ISR_NOERRCODE(3)   // Breakpoint
ISR_NOERRCODE(4)   // Overflow
ISR_NOERRCODE(5)   // BOUND Range Exceeded
ISR_NOERRCODE(6)   // Invalid Opcode
ISR_NOERRCODE(7)   // Device Not Available
ISR_ERRCODE(8)     // Double Fault
ISR_NOERRCODE(9)   // Coprocessor Segment Overrun
ISR_ERRCODE(10)    // Invalid TSS
ISR_ERRCODE(11)    // Segment Not Present
ISR_ERRCODE(12)    // Stack-Segment Fault
ISR_ERRCODE(13)    // General Protection Fault
ISR_ERRCODE(14)    // Page Fault
ISR_NOERRCODE(128) // System call (int 0x80)
ISR_NOERRCODE(129) // System call (int 0x81)
ISR_NOERRCODE(130) // System call (int 0x82)
ISR_NOERRCODE(131) // System call (int 0x83)
ISR_NOERRCODE(132) // System call (int 0x84)

#ifdef __cplusplus
}
#endif
