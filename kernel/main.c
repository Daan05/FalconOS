#include "drivers/framebuffer.h"
#include "interrupts/idt.h"

// Halt and catch fire function.
static void hcf(void) {
  for (;;) {
    asm("hlt");
  }
}

void kernel_main(void) {
  if (!initialize_framebuffer()) {
    hcf();
  }
  draw_string(16, 16, "framebuffer initialized", 0xffffff);

  init_idt();
  draw_string(16, 32, "IDT Initialized", 0xffffff);

  draw_string(16, 48, "HANGING....", 0xffffff);
  hcf();
}

// https: // github.com/RickleAndMortimer/MakenOS?tab=0BSD-1-ov-file
