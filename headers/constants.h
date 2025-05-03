#include <raylib.h>
#include <string.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H

static const int width = 800;
static const int height = 600;
static const char* title = "Paulo";

static const int block_size = 24;
static const int font_size = 32;

static const int count_rows = 20;
static const int count_cols = 10;

static const float switch_figure_delay = 0.12f;
static const float move_figure_delay = 0.05f;

static const int fading_time = 44;

static const int field_pos_x = block_size * 3;
static const int field_pos_y = block_size * 2;

static const Color FADING_COLOR_ARR[count_cols] = {RED, BLUE, YELLOW, PINK, BLACK, BROWN, VIOLET, PURPLE, PINK, RED};

static const char* figure_i[] = {"1111", "1 1 1 1", NULL};
static const char* figure_o[] = {"11 11", NULL};
static const char* figure_t[] = {"111 010", "10 11 10", "010 111", "01 11 01", NULL};
static const char* figure_s[] = {"011 110", "10 11 01", NULL};
static const char* figure_z[] = {"110 011", "01 11 10", NULL};
static const char* figure_l[] = {"100 111", "11 10 10", "111 001", "01 01 11", NULL};
static const char* figure_j[] = {"001 111", "10 10 11", "111 100", "11 01 01", NULL};

static const Color bacgkround_color = (Color){30, 30, 30, 100};

#endif