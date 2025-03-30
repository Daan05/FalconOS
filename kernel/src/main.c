#include "framebuffer.h"

// Halt and catch fire function.
static void hcf(void) {
  for (;;) {
    asm("hlt");
  }
}

void kernel_main(void) {
  if (!initialize_framebuffer()) {
    return;
  }

  draw_string(0, 0,
              "The quick brown fox jumps over the lazy dog.,?![](){} 1 2 3 4 5 "
              "6 7 8 9 9 / * - + \\ @ # $ % ^ & <>:; \"'| ~ ` iIlL",
              0xff0000);

  // We're done, just hang...
  hcf();
}
