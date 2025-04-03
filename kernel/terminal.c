#include "terminal.h"

#include "drivers/framebuffer.h"

#include "string.h"

static int terminal_y;
static int terminal_x;
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

  terminal_y = PIXELS_PER_CHARACTER;
  terminal_x = PIXELS_PER_CHARACTER;
  terminal_width = fb->width - (fb->width % PIXELS_PER_CHARACTER);
  terminal_height = fb->height - (fb->height % (PIXELS_PER_CHARACTER + 2));
  foreground_color = 0xffffff; // white
  background_color = 0x101010; // darkgray

  clear_screen(background_color);
  terminal_printf(
      "TERMINAL:\n    width: %d\n    heigth: "
      "%d\n    foreground: %X\n    background: %X\n    fb pitch: %d\n\n",
      terminal_width, terminal_height, foreground_color, background_color,
      fb->pitch);

  return 1;
}

void terminal_set_foreground_color(uint32_t color) { foreground_color = color; }
void terminal_set_background_color(uint32_t color) { background_color = color; }

void terminal_putchar(char c) {
  if (terminal_y >= terminal_height - PIXELS_PER_CHARACTER) {
    terminal_scroll_down(1);
  }

  if (c == '\n') {
    terminal_y += PIXELS_PER_CHARACTER + 2;
    terminal_x = PIXELS_PER_CHARACTER;
    return;
  }

  if (c == '\b') {
    terminal_unputchar();
    return;
  }

  draw_char(terminal_x, terminal_y, c, foreground_color);
  terminal_x += PIXELS_PER_CHARACTER;
  if (terminal_x == terminal_width) {
    terminal_x = PIXELS_PER_CHARACTER;
    terminal_y += PIXELS_PER_CHARACTER + 2;
  }
}

void terminal_unputchar() {
  terminal_x -= PIXELS_PER_CHARACTER;

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      putpixel(terminal_x + x, terminal_y + y, background_color);
    }
  }
}

static void print_string(const char *str) {
  while (*str) {
    terminal_putchar(*str++);
  }
}

static void print_int(int value) {
  if (value < 0) {
    terminal_putchar('-');
    value = -value;
  }

  char buffer[12]; // Enough for -2,147,483,648 and null terminator
  int i = 0;
  do {
    buffer[i++] = '0' + (value % 10);
    value /= 10;
  } while (value > 0);
  buffer[i] = '\0';

  // Reverse the buffer
  for (int j = i - 1; j >= 0; j--) {
    terminal_putchar(buffer[j]);
  }
}

static void print_uint(unsigned int value) {
  char buffer[11]; // Enough for 4,294,967,295 and null terminator
  int i = 0;
  do {
    buffer[i++] = '0' + (value % 10);
    value /= 10;
  } while (value > 0);
  buffer[i] = '\0';

  // Reverse the buffer
  for (int j = i - 1; j >= 0; j--) {
    terminal_putchar(buffer[j]);
  }
}

static void print_hex(unsigned int value, bool uppercase) {
  char buffer[9]; // Enough for 0xFFFFFFFF and null terminator
  int i = 0;
  do {
    int digit = value % 16;
    if (digit < 10) {
      buffer[i++] = '0' + digit;
    } else {
      buffer[i++] = (uppercase ? 'A' : 'a') + (digit - 10);
    }
    value /= 16;
  } while (value > 0);
  buffer[i] = '\0';

  // Reverse the buffer
  for (int j = i - 1; j >= 0; j--) {
    terminal_putchar(buffer[j]);
  }
}

void terminal_printf(const char *format, ...) {
  va_list args;
  va_start(args, format);

  while (*format) {
    if (*format == '%') {
      format++;
      switch (*format) {
      case 'c': {
        char c = (char)va_arg(args, int);
        terminal_putchar(c);
        break;
      }
      case 's': {
        const char *str = va_arg(args, const char *);
        print_string(str ? str : "(null)");
        break;
      }
      case 'd': {
        int value = va_arg(args, int);
        print_int(value);
        break;
      }
      case 'u': {
        unsigned int value = va_arg(args, unsigned int);
        print_uint(value);
        break;
      }
      case 'x': {
        unsigned int value = va_arg(args, unsigned int);
        print_hex(value, false);
        break;
      }
      case 'X': {
        unsigned int value = va_arg(args, unsigned int);
        print_hex(value, true);
        break;
      }
      case '%': {
        terminal_putchar('%');
        break;
      }
      default:
        terminal_putchar('%');
        terminal_putchar(*format);
      }
    } else {
      terminal_putchar(*format);
    }
    format++;
  }

  va_end(args);
}

void terminal_scroll_down(int num_lines) {
  // Calculate pixel dimensions
  int line_height = PIXELS_PER_CHARACTER + 2; // Your character height + spacing
  int scroll_pixels = num_lines * line_height;

  // Get framebuffer info
  struct limine_framebuffer *fb = get_framebuffer();
  uint32_t *fb_base = fb->address;
  uint32_t pitch_32 = fb->pitch / 4; // Pitch in 32-bit words

  // Calculate scroll region bounds
  int start_y = PIXELS_PER_CHARACTER; // Skip initial margin
  int end_y = terminal_height - scroll_pixels;

  // Scroll the content up (move everything up by scroll_pixels)
  for (int y = start_y; y < end_y; y++) {
    uint32_t *src_row = &fb_base[(y + scroll_pixels) * pitch_32];
    uint32_t *dest_row = &fb_base[y * pitch_32];
    memcpy(dest_row, src_row, terminal_width * sizeof(uint32_t));
  }

  // Clear the newly exposed area at the bottom
  for (int y = end_y; y < terminal_height; y++) {
    uint32_t *row = &fb_base[y * pitch_32];
    for (int x = 0; x < terminal_width; x++) {
      row[x] = background_color;
    }
  }

  // Adjust cursor position
  terminal_y -= scroll_pixels;
  if (terminal_y < PIXELS_PER_CHARACTER) {
    terminal_y = PIXELS_PER_CHARACTER;
  }
}

void terminal_scroll_up(int numLines) { return; }
