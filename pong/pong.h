#ifndef PONG_H
#define PONG_H

#include <stdint.h>

struct paddle {
  int x, y;
  uint32_t color;
};

struct ball {
  int x, y, radius;
  int vel_x, vel_y;
  uint32_t color;
};

void init_pong();
void update_game();

void set_key_lu_down(int a);
void set_key_ld_down(int a);
void set_key_ru_down(int a);
void set_key_rd_down(int a);

void move_left_up();
void move_left_down();
void move_right_up();
void move_right_down();

#endif
