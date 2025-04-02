#include "terminal.h"

#include "drivers/framebuffer.h"

static int terminal_row;
static int terminal_col;
static int terminal_width;
static int terminal_height;
static uint32_t foreground_color;
static uint32_t background_color;
static struct limine_framebuffer *fb;

int init_terminal() {
  if (!init_framebuffer()) {
    return 0;
  }

  struct limine_framebuffer *fb = get_framebuffer();

  terminal_row = PIXELS_PER_CHARACTER;
  terminal_col = PIXELS_PER_CHARACTER;
  terminal_width = fb->width - (fb->width % PIXELS_PER_CHARACTER);
  terminal_height = fb->height - (fb->height % (PIXELS_PER_CHARACTER + 2));
  foreground_color = 0xffffff; // white
  background_color = 0x101010; // darkgray

  return 1;
}

void terminal_set_foreground_color(uint32_t color) { foreground_color = color; }
void terminal_set_background_color(uint32_t color) { background_color = color; }

void terminal_putchar(char c) {
  if (terminal_row == terminal_height) {
    terminal_scroll_down(1);
  }

  if (c == '\n') {
    terminal_row += PIXELS_PER_CHARACTER + 2;
    terminal_col = PIXELS_PER_CHARACTER;
    return;
  }

  if (c == '\b') {
    terminal_unputchar();
    return;
  }

  draw_char(terminal_col, terminal_row, c, foreground_color);
  terminal_col += PIXELS_PER_CHARACTER;
  if (terminal_col == terminal_width) {
    terminal_col = PIXELS_PER_CHARACTER;
    terminal_row += PIXELS_PER_CHARACTER + 2;
    if (terminal_row == terminal_height)
      terminal_row = PIXELS_PER_CHARACTER;
  }
}

void terminal_unputchar() {}

void terminal_printf(const char *str) {
  while (*str) {
    terminal_putchar(*str);
    str++;
  }
}

void terminal_scroll_down(int numLines) { return; }
void terminal_scroll_up(int numLines) { return; }
