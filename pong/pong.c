#include "pong.h"

#include "drivers/framebuffer.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

static uint32_t background_color;
static uint32_t paddle_width;
static uint32_t paddle_height;
static struct paddle lp;
static struct paddle rp;
static struct ball b;
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
  b.x = fb->width / 2;
  b.y = fb->height / 2;
  b.vel_x = -4;
  b.vel_y = 0;
  b.radius = paddle_width;
  b.color = 0xffffff;
  key_lu_down = 0;
  key_ld_down = 0;
  key_ru_down = 0;
  key_rd_down = 0;

  clear_screen(background_color);
}

void update_game() {
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

  // undraw ball
  draw_circle(b.x, b.y, b.radius, background_color);
  // move
  b.x += b.vel_x;
  b.y += b.vel_y;
  // check bounds
  if (b.x < b.radius) { // left side
    if (b.y > lp.y && b.y < lp.y + paddle_height) {
      b.x = b.radius;
      b.vel_x *= -1;
    } else {
      // stop the game
      for (;;) {
      }
    }
  }
  if (b.x > fb->width - b.radius) { // right side
    // if (b.y > rp.y && b.y < rp.y + paddle_height) {
    // b.x = fb->width - b.radius;
    // b.vel_x *= -1;
    // }
  }
  if (b.y < b.radius) { // top side
    b.y = b.radius;
    b.vel_y *= -1;
  }
  if (b.y > fb->height - b.radius) { // down side
    // what the heck is wrong with these two lines
    // how do they cause a crash???
    // b.y = fb->height - b.radius;
    // b.vel_y = -b.vel_y;
  }

  //  redraw
  draw_circle(b.x, b.y, b.radius, b.color);
}

void set_key_lu_down(int a) { key_lu_down = a; }
void set_key_ld_down(int a) { key_ld_down = a; }
void set_key_ru_down(int a) { key_ru_down = a; }
void set_key_rd_down(int a) { key_rd_down = a; }

void move_left_up() { lp.y = MAX(0, lp.y - 5); }
void move_left_down() { lp.y = MIN(fb->height - paddle_height, lp.y + 5); }
void move_right_up() { rp.y = MAX(0, rp.y - 5); }
void move_right_down() { rp.y = MIN(fb->height - paddle_height, rp.y + 5); }
