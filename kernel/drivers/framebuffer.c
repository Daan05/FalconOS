#include "framebuffer.h"

// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((
    used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used,
               section(".limine_requests_"
                       "start"))) static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((
    used,
    section(
        ".limine_requests_end"))) static volatile LIMINE_REQUESTS_END_MARKER;

static const uint8_t font[128][8] = {
    [0x20] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // Space
    [0x21] = {0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x00}, // !
    [0x22] = {0x36, 0x36, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00}, // "
    [0x23] = {0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00}, // #
    [0x24] = {0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00}, // $
    [0x25] = {0x63, 0x33, 0x18, 0x0C, 0x06, 0x33, 0x31, 0x00}, // %
    [0x26] = {0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00}, // &
    [0x27] = {0x06, 0x06, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00}, // '
    [0x28] = {0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00}, // (
    [0x29] = {0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00}, // )
    [0x2A] = {0x00, 0x36, 0x1C, 0x7F, 0x1C, 0x36, 0x00, 0x00}, // *
    [0x2B] = {0x00, 0x0C, 0x0C, 0x7F, 0x0C, 0x0C, 0x00, 0x00}, // +
    [0x2C] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06}, // ,
    [0x2D] = {0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00}, // -
    [0x2E] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00}, // .
    [0x2F] = {0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00}, // /

    [0x30] = {0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00}, // 0
    [0x31] = {0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00}, // 1
    [0x32] = {0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00}, // 2
    [0x33] = {0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00}, // 3
    [0x34] = {0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00}, // 4
    [0x35] = {0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00}, // 5
    [0x36] = {0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00}, // 6
    [0x37] = {0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00}, // 7
    [0x38] = {0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00}, // 8
    [0x39] = {0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00}, // 9

    [0x3A] = {0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00}, // :
    [0x3B] = {0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06}, // ;
    [0x3C] = {0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00}, // <
    [0x3D] = {0x00, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x00, 0x00}, // =
    [0x3E] = {0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00}, // >
    [0x3F] = {0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00}, // ?

    [0x40] = {0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00}, // @

    [0x41] = {0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00}, // A
    [0x42] = {0x3F, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x3F, 0x00}, // B
    [0x43] = {0x1E, 0x33, 0x03, 0x03, 0x03, 0x33, 0x1E, 0x00}, // C
    [0x44] = {0x3F, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00}, // D
    [0x45] = {0x3F, 0x03, 0x03, 0x1F, 0x03, 0x03, 0x3F, 0x00}, // E
    [0x46] = {0x3F, 0x03, 0x03, 0x1F, 0x03, 0x03, 0x03, 0x00}, // F
    [0x47] = {0x1E, 0x33, 0x03, 0x3B, 0x33, 0x33, 0x1E, 0x00}, // G
    [0x48] = {0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00}, // H
    [0x49] = {0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00}, // I
    [0x4A] = {0x3C, 0x18, 0x18, 0x18, 0x18, 0x1B, 0x0F, 0x00}, // J
    [0x4B] = {0x33, 0x1B, 0x0F, 0x07, 0x0F, 0x1B, 0x33, 0x00}, // K
    [0x4C] = {0x03, 0x03, 0x03, 0x03, 0x03, 0x33, 0x3F, 0x00}, // L
    [0x4D] = {0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00}, // M
    [0x4E] = {0x33, 0x37, 0x3F, 0x3F, 0x3B, 0x33, 0x33, 0x00}, // N
    [0x4F] = {0x1E, 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x00}, // O
    [0x50] = {0x3F, 0x33, 0x33, 0x3F, 0x03, 0x03, 0x03, 0x00}, // P
    [0x51] = {0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00}, // Q
    [0x52] = {0x3F, 0x33, 0x33, 0x3F, 0x0F, 0x1B, 0x33, 0x00}, // R
    [0x53] = {0x1E, 0x33, 0x03, 0x1E, 0x30, 0x33, 0x1E, 0x00}, // S
    [0x54] = {0x3F, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x00}, // T
    [0x55] = {0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x00}, // U
    [0x56] = {0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00}, // V
    [0x57] = {0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00}, // W
    [0x58] = {0x33, 0x33, 0x1E, 0x0C, 0x1E, 0x33, 0x33, 0x00}, // X
    [0x59] = {0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x00}, // Y
    [0x5A] = {0x3F, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x3F, 0x00}, // Z
    [0x5B] = {0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00}, // [
    [0x5C] = {0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x00}, // Backslash
    [0x5D] = {0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00}, // ]
    [0x5E] = {0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00}, // ^
    [0x5F] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00}, // _
    [0x60] = {0x06, 0x06, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00}, // `
    [0x61] = {0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x3E, 0x00}, // a
    [0x62] = {0x03, 0x03, 0x1F, 0x33, 0x33, 0x33, 0x1F, 0x00}, // b
    [0x63] = {0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00}, // c
    [0x64] = {0x30, 0x30, 0x3E, 0x33, 0x33, 0x33, 0x3E, 0x00}, // d
    [0x65] = {0x00, 0x00, 0x1E, 0x33, 0x3F, 0x03, 0x1E, 0x00}, // e
    [0x66] = {0x1C, 0x36, 0x06, 0x1F, 0x06, 0x06, 0x06, 0x00}, // f
    [0x67] = {0x00, 0x00, 0x3E, 0x33, 0x33, 0x3E, 0x30, 0x1F}, // g
    [0x68] = {0x03, 0x03, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00}, // h
    [0x69] = {0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00}, // i
    [0x6A] = {0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x1B, 0x0E}, // j
    [0x6B] = {0x03, 0x03, 0x33, 0x1B, 0x0F, 0x1B, 0x33, 0x00}, // k
    [0x6C] = {0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00}, // l
    [0x6D] = {0x00, 0x00, 0x3B, 0x7F, 0x6B, 0x63, 0x63, 0x00}, // m
    [0x6E] = {0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00}, // n
    [0x6F] = {0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00}, // o
    [0x70] = {0x00, 0x00, 0x1F, 0x33, 0x33, 0x1F, 0x03, 0x03}, // p
    [0x71] = {0x00, 0x00, 0x3E, 0x33, 0x33, 0x3E, 0x30, 0x30}, // q
    [0x72] = {0x00, 0x00, 0x1F, 0x33, 0x03, 0x03, 0x03, 0x00}, // r
    [0x73] = {0x00, 0x00, 0x1E, 0x03, 0x1E, 0x30, 0x1F, 0x00}, // s
    [0x74] = {0x06, 0x06, 0x1F, 0x06, 0x06, 0x36, 0x1C, 0x00}, // t
    [0x75] = {0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x3E, 0x00}, // u
    [0x76] = {0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00}, // v
    [0x77] = {0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00}, // w
    [0x78] = {0x00, 0x00, 0x33, 0x1E, 0x0C, 0x1E, 0x33, 0x00}, // x
    [0x79] = {0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F}, // y
    [0x7A] = {0x00, 0x00, 0x3F, 0x18, 0x0C, 0x06, 0x3F, 0x00}, // z
    [0x7B] = {0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00}, // {
    [0x7C] = {0x0C, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x0C, 0x00}, // |
    [0x7D] = {0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00}, // }
    [0x7E] = {0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // ~
    [0x7F] = {0x3E, 0x63, 0x55, 0x49, 0x55, 0x63, 0x3E, 0x00}, // DEL (Custom)
};

// Function to draw a pixel at (x, y)
void putpixel(int x, int y, uint32_t color) {
  struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];
  uint32_t *pixels = fb->address;
  pixels[y * (fb->pitch / 4) + x] = color;
}

// Function to draw a character
void draw_char(int x, int y, unsigned char c, uint32_t color) {
  if (c >= 128)
    return; // Only handle ASCII 0-127

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (font[(int)c][row] & (1 << (col))) {
        putpixel(x + col, y + row, color);
      }
    }
  }
}

// Function to draw a string
void draw_string(int x, int y, const char *str, uint32_t color) {
  while (*str) {
    draw_char(x, y, *str, color);
    x += 8; // Move to next character position
    str++;
  }
}

void clear_screen(uint32_t color) {
  struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];

  for (size_t y = 0; y < fb->height; y++) {
    for (size_t x = 0; x < fb->width; x++) {
      putpixel(x, y, color);
    }
  }
}

int initialize_framebuffer() {
  // Ensure the bootloader actually understands our base revision (see spec).
  if (LIMINE_BASE_REVISION_SUPPORTED == false) {
    return 0;
  }

  // Ensure we got a framebuffer.
  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1) {
    return 0;
  }

  // Fetch the first framebuffer.
  struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];

  // Clear the screen to dark gray
  clear_screen(0x101010);

  return 1;
}
