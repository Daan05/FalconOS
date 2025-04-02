#include "interrupts/idt.h"
#include "terminal.h"

// Halt and catch fire function.
static void hcf(void) {
  for (;;) {
    asm("hlt");
  }
}

void kernel_main(void) {
  if (!init_terminal()) {
    // couldn't get a framebuffer
    hcf();
  }
  terminal_printf("terminal initialized\n2nd line");

  // enable interrupts
  init_idt();
  terminal_printf("IDT initialized\n");

  // hanging
  terminal_printf("hanging...\n");
  hcf();
}

// https: // github.com/RickleAndMortimer/MakenOS?tab=0BSD-1-ov-file
