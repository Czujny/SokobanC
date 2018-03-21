#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"

#define LOAD_ERROR 0
#define LOAD_SUCCESS 1
#define VOID 0
#define WALL 1
#define FLOOR 2
#define TARGET 3
#define BOX 4
#define PLAYER 5
#define BOX_ON_TARGET 6
#define PLAYER_ON_TARGET 7
#define STATUS_GAME 0
#define STATUS_MENU 1
#define STATUS_LOAD 2
#define STATUS_ADD 3
#define STATUS_ERROR 4
#define STATUS_HIGH_SCORES 5
#define STATUS_SHOW_SCORES 6
#define RESUME 1
#define LOAD 2
#define ADD 3
#define HIGH_SCORES 4
#define EXIT 5
#define SORT_MOVE 12
#define SORT_TIME 6


typedef struct {

	SDL_Surface *box, *box_on_target, *wall, *floor, *target, *player[8];
	int **board;
	int width, height;
	int targets;
	int playerx, playery;
}board_t;


typedef struct {

	char *nick;
	int moves;
	float time;
}high_score_t;


typedef struct {

	int t1, t2, quit, frames, rc;
	bool pause;
	double delta, worldTime, fpsTimer, fps;
	int points, moves;
	int game_status, option;
	char **boards_avalaible;
	int boards_number, actual_board;
	int board_chosen;
	int sort_by, high_scores_number;
	high_score_t *high_scores;
	board_t *board;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;
}status_t;


int init_status(status_t *status);

int load_bmps(status_t *status);

int load_player_bmps(status_t *status);

void update_time(status_t *status);

void update_fps(status_t *status);

int reset_status(status_t *status);

void delete_board(status_t *status);

void sort_high_scores(status_t *status);

#endif // !STRUCTURES_H
