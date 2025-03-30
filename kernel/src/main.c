#include "framebuffer.h"
#include "idt.h"
#include "timer.h"

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

  draw_string(0, 0, "Initializing IDT...", 0xff0000);
  idt_init();
  draw_string(0, 16, "IDT initialized.", 0xff0000);

  draw_string(0, 32, "Initializing timer...", 0xff0000);
  timer_init(100); // 100 Hz
  draw_string(0, 48, "Timer initialized.", 0xff0000);

  asm volatile("sti");

  draw_string(0, 64, "Initializations finished.", 0xff0000);

  hcf();
}
