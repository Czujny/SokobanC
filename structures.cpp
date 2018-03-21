#include "stdafx.h"
#include "structures.h"
#include "game_init.h"
#include <stdio.h>
#include <string.h>
#include "files.h"


int init_status(status_t *status) {

	status->t1 = SDL_GetTicks();

	board_t *board = new board_t();

	status->pause = false;
	status->game_status = STATUS_GAME;
	status->option = RESUME;
	status->sort_by = SORT_TIME;
	status->high_scores = NULL;
	status->high_scores_number = 0;
	status->actual_board = 0;
	status->board_chosen = 0;
	status->moves = 0;
	status->frames = 0;
	status->fpsTimer = 0;
	status->fps = 0;
	status->quit = 0;
	status->worldTime = 0;
	status->points = 0;
	status->board = board;

	if (load_conf(status) == LOAD_ERROR)return LOAD_ERROR;
	if(load_bmps(status) == LOAD_ERROR)return LOAD_ERROR;
	if (load_board(status, status->boards_avalaible[0]) == LOAD_ERROR)return LOAD_ERROR;

	return LOAD_SUCCESS;
}


int load_bmps(status_t *status) {

	status->board->box = SDL_LoadBMP("./bmp/box.bmp");
	if (status->board->box == NULL) {
		printf("SDL_LoadBMP(box.bmp) error: %s\n", SDL_GetError());
		return LOAD_ERROR;
	}

	status->board->box_on_target = SDL_LoadBMP("./bmp/boxtarget.bmp");
	if (status->board->box_on_target == NULL) {
		printf("SDL_LoadBMP(box_on_target.bmp) error: %s\n", SDL_GetError());
		return LOAD_ERROR;
	}

	status->board->wall = SDL_LoadBMP("./bmp/wall.bmp");
	if (status->board->wall == NULL) {
		printf("SDL_LoadBMP(wall2.bmp) error: %s\n", SDL_GetError());
		return LOAD_ERROR;
	}

	status->board->target = SDL_LoadBMP("./bmp/target.bmp");
	if (status->board->target == NULL) {
		printf("SDL_LoadBMP(target.bmp) error: %s\n", SDL_GetError());
		return LOAD_ERROR;
	}

	status->board->floor = SDL_LoadBMP("./bmp/floor.bmp");
	if (status->board->floor == NULL) {
		printf("SDL_LoadBMP(floor2.bmp) error: %s\n", SDL_GetError());
		return LOAD_ERROR;
	}

	if (load_player_bmps(status) == LOAD_ERROR)return LOAD_ERROR;

	return LOAD_SUCCESS;
}


int load_player_bmps(status_t *status) {

	char dir[18] = "./bmp/player0.bmp";

	for (int i = 0; i < 8; i++) {

		dir[12] = i + '0';
		status->board->player[i] = SDL_LoadBMP(dir);
		if (status->board->player[i] == NULL) {
			printf("SDL_LoadBMP(player.bmp) error: %s\n", SDL_GetError());
			return LOAD_ERROR;
		}
	}
	
	return LOAD_SUCCESS;
}


void update_time(status_t *status) {

	if (!status->pause) {

		status->t2 = SDL_GetTicks();

		status->delta = (status->t2 - status->t1) * 0.001;
		status->t1 = status->t2;

		status->worldTime += status->delta;
	}
	else status->t1 = SDL_GetTicks();
}


void update_fps(status_t *status) {

	if (!status->pause) {

		status->fpsTimer += status->delta;
		if (status->fpsTimer > 0.5) {
			status->fps = status->frames * 2;
			status->frames = 0;
			status->fpsTimer -= 0.5;
		}

		status->frames++;
	}
}


int reset_status(status_t *status) {

	status->t1 = SDL_GetTicks();

	status->pause = false;
	status->option = RESUME;
	status->moves = 0;
	status->frames = 0;
	status->delta = 0;
	status->fpsTimer = 0;
	status->fps = 0;
	status->quit = 0;
	status->worldTime = 0;
	status->points = 0;

	delete_board(status);

	if (load_board(status, status->boards_avalaible[status->actual_board]) == LOAD_ERROR)return LOAD_ERROR;

	return LOAD_SUCCESS;
}


void delete_board(status_t *status) {

	for (int i = 0; i < status->board->height; i++)delete[] status->board->board[i];
	delete[] status->board->board;
	status->board->board = NULL;
}

void sort_high_scores(status_t *status) {

	high_score_t *high_scores = status->high_scores;

	if (status->sort_by == SORT_TIME) {

		for (int i = 0; i < status->high_scores_number - 1; i++) {

			if (high_scores[i].time > high_scores[i + 1].time) {

				high_score_t temp = high_scores[i];
				high_scores[i] = high_scores[i + 1];
				high_scores[i + 1] = temp;
			}
		}
	}
	else if (status->sort_by == SORT_MOVE) {

		for (int i = 0; i < status->high_scores_number - 1; i++) {

			if (high_scores[i].moves > high_scores[i + 1].moves) {

				high_score_t temp = high_scores[i];
				high_scores[i] = high_scores[i + 1];
				high_scores[i + 1] = temp;
			}
		}
	}
}
