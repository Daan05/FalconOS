#include "pong.h"

#include "drivers/framebuffer.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

static uint32_t background_color;
static uint32_t paddle_width;
static uint32_t paddle_height;
static struct paddle lp;
static struct paddle rp;
static int key_lu_down;
static int key_ld_down;
static int key_ru_down;
static int key_rd_down;
static struct limine_framebuffer *fb;

void init_pong() {
  fb = get_framebuffer();
  background_color = 0x000000;
  paddle_width = 32;
  paddle_height = 256;
  lp.x = 0;
  lp.y = (fb->height / 2) - (paddle_height / 2);
  lp.color = 0xffffff;
  rp.x = fb->width - paddle_width;
  rp.y = lp.y;
  rp.color = lp.color;
  key_lu_down = 0;
  key_ld_down = 0;
  key_ru_down = 0;
  key_rd_down = 0;

  clear_screen(background_color);
}

void update_paddles() {
  // update left paddle
  // undraw
  fill_rect(lp.x, lp.y, paddle_width, paddle_height, background_color);
  // update pos
  if (key_lu_down) {
    move_left_up();
  }
  if (key_ld_down) {
    move_left_down();
  }
  // redraw
  fill_rect(lp.x, lp.y, paddle_width, paddle_height, lp.color);

  // update left paddle
  // undraw
  fill_rect(rp.x, rp.y, paddle_width, paddle_height, background_color);
  // update pos
  if (key_ru_down) {
    move_right_up();
  }
  if (key_rd_down) {
    move_right_down();
  }
  // redraw
  fill_rect(rp.x, rp.y, paddle_width, paddle_height, rp.color);
}

void set_key_lu_down(int a) { key_lu_down = a; }
void set_key_ld_down(int a) { key_ld_down = a; }
void set_key_ru_down(int a) { key_ru_down = a; }
void set_key_rd_down(int a) { key_rd_down = a; }

void move_left_up() { lp.y = MAX(0, lp.y - 5); }
void move_left_down() { lp.y = MIN(fb->height - paddle_height, lp.y + 5); }
void move_right_up() { rp.y = MAX(0, rp.y - 5); }
void move_right_down() { rp.y = MIN(fb->height - paddle_height, rp.y + 5); }
