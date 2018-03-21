#pragma once
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#define NO_MOVE 0
#define FLAG_UP -1
#define FLAG_LEFT -1
#define FLAG_DOWN 1
#define FLAG_RIGHT 1
#define BOX_MOVE 1
#define PLAYER_MOVE 2

#include "structures.h"
#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"

void move(status_t *status, int direction);

int execute_move(status_t *status, int flag_x, int flag_y);

bool box_move(status_t *status, int flag_x, int flag_y);

void change_option(status_t *status, int direction);

void choose_option(status_t *status);

void change_game(status_t *status, int direction);

void choose_game(status_t *status);

int add_game(status_t *status, char *file_name);

void change_high_score(status_t *staus, int direction);

void add_high_score(status_t *status, char *nick);

#endif // !GAME_LOGIC_H
