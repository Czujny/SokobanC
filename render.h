#pragma once
#ifndef RENDER_H
#define RENDER_H

#include "structures.h"

#define X_STARTING 32
#define Y_STARTING 64
#define FIELD_SIZE 32
#define MAX_IN_ROW 14
#define MAX_HIGH_SCORES 12
#define MAX_BOARDS 56
#define MAX_SCORES 36

void render(status_t *status);

void refresh(status_t *status);

void draw_game(status_t *status);

void draw_top_bar(status_t *status);

void draw_board(status_t *status);

void draw_win_screen(status_t *status);

void draw_win_screen_info(status_t *status);

void draw_player(status_t *status);

void animate_move(status_t *status, int x_start, int y_start, int x_dir, int y_dir, int box_move);

void draw_menu(status_t *status);

void draw_load_menu(status_t *status);

void draw_add_menu(status_t *status);

void draw_add_instruct(status_t *status);

void draw_error(status_t *status);

void draw_high_scores(status_t *status);

void draw_score_list(status_t *status);

#endif // !RENDER_H
