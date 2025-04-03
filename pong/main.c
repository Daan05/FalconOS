#include "devices/keyboard.h"
#include "devices/pic.h"
#include "devices/pit.h"
#include "drivers/framebuffer.h"
#include "interrupts/idt.h"
#include "pong.h"

static void game_loop(void) {
  for (;;) {
    asm("hlt");
  }
}

// pong gets played by 2 players
// LEFT:
//      W - up
//      S - down
//
//  RIGHT:
//      I - up
//      k - down
//
void kernel_main(void) {
  if (!init_framebuffer()) {
    // couldn't get a framebuffer
    return;
  }

  // remap pic
  remap_pic();

  // init timer
  if (!init_pit(100)) { // this game will run at 100 FPS
    return;
  }

  init_keyboard();

  // enable interrupts
  init_idt();

  init_pong();

  game_loop();
}

// https: // github.com/RickleAndMortimer/MakenOS?tab=0BSD-1-ov-file
