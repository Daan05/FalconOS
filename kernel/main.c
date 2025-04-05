#include "cpu_info.h"
#include "devices/keyboard.h"
#include "devices/pic.h"
#include "devices/pit.h"
#include "interrupts/idt.h"
#include "memory/mem.h"
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
  terminal_printf("terminal initialized\n");

  // remap pic
  remap_pic();

  // init timer
  if (!init_pit(1000)) { // 1000hz
    terminal_printf("invalid frequency\n");
    hcf();
  }

  init_keyboard();

  // enable interrupts
  init_idt();
  terminal_printf("IDT initialized\n");

  terminal_printf("total usable memory: %u bytes\n", get_total_usable_memory());

  detect_cpu();

  // hanging
  hcf();
}

// https: // github.com/RickleAndMortimer/MakenOS?tab=0BSD-1-ov-file
