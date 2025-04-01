#include "cpu/isr.h"
#include "drivers/framebuffer.h"

// Halt and catch fire function.
static void hcf(void) {
  for (;;) {
    asm("hlt");
  }
}

void kernel_main(void) {
  isr_install();

  if (!initialize_framebuffer()) {
    hcf();
  }

  isr_4();

  draw_string(32, 32, "Hello from kernel...", 0xff0000);

  hcf();
}
