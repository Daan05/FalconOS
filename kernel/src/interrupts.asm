; Assemble with NASM or your favorite assembler

global idt_flush
global timer_interrupt_handler

section .text
; Loads the IDT using lidt instruction
idt_flush:
    lidt [rdi]   ; rdi holds pointer to our idt_ptr structure
    ret

; Timer interrupt handler stub
; This stub saves registers, calls our C handler, sends the EOI to the PIC, and returns.
timer_interrupt_handler:
    cli                   ; disable interrupts while handling
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; Call the C timer handler
    extern timer_handler
    call timer_handler

    ; Send End Of Interrupt (EOI) to PIC
    mov al, 0x20
    out 0x20, al

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    sti                   ; re-enable interrupts
    iretq

