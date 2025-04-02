#include "terminal.h"

#include "drivers/framebuffer.h"

static int terminal_x;
static int terminal_y;
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

  terminal_x = PIXELS_PER_CHARACTER;
  terminal_y = PIXELS_PER_CHARACTER;
  terminal_width = fb->width - (fb->width % PIXELS_PER_CHARACTER);
  terminal_height = fb->height - (fb->height % (PIXELS_PER_CHARACTER + 2));
  foreground_color = 0xffffff; // white
  background_color = 0x101010; // darkgray

  terminal_printf("TERMINAL:\n    width: %d\n    heigth: "
                  "%d\n    foreground: %X\n    background: %X\n\n",
                  terminal_width, terminal_height, foreground_color,
                  background_color);

  return 1;
}

void terminal_set_foreground_color(uint32_t color) { foreground_color = color; }
void terminal_set_background_color(uint32_t color) { background_color = color; }

void terminal_putchar(char c) {
  if (terminal_x == terminal_height) {
    terminal_scroll_down(1);
  }

  if (c == '\n') {
    terminal_x += PIXELS_PER_CHARACTER + 2;
    terminal_y = PIXELS_PER_CHARACTER;
    return;
  }

  if (c == '\b') {
    terminal_unputchar();
    return;
  }

  draw_char(terminal_y, terminal_x, c, foreground_color);
  terminal_y += PIXELS_PER_CHARACTER;
  if (terminal_y == terminal_width) {
    terminal_y = PIXELS_PER_CHARACTER;
    terminal_x += PIXELS_PER_CHARACTER + 2;
    if (terminal_x == terminal_height)
      terminal_x = PIXELS_PER_CHARACTER;
  }
}

void terminal_unputchar() {}

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

void terminal_scroll_down(int numLines) { return; }
void terminal_scroll_up(int numLines) { return; }
