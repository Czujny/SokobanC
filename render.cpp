#include "stdafx.h"
#include "render.h"
#include <string.h>
#include "draw.h"
#include "game_init.h"
#include "gamelogic.h"
#include "keyboard.h"
#include "files.h"



void render(status_t *status) {

	if (status->game_status == STATUS_GAME)draw_game(status);
	else if (status->game_status == STATUS_MENU)draw_menu(status);
	else if (status->game_status == STATUS_LOAD)draw_load_menu(status);
	else if (status->game_status == STATUS_ADD)draw_add_menu(status);
	else if (status->game_status == STATUS_ERROR)draw_error(status);
	else if (status->game_status == STATUS_HIGH_SCORES)draw_high_scores(status);
	else if (status->game_status == STATUS_SHOW_SCORES)draw_score_list(status);

	refresh(status);
}


void refresh(status_t *status) {

	SDL_UpdateTexture(status->scrtex, NULL, status->screen->pixels, status->screen->pitch);
	SDL_RenderCopy(status->renderer, status->scrtex, NULL, NULL);
	SDL_RenderPresent(status->renderer);
}


void draw_game(status_t *status) {

	int background = SDL_MapRGB(status->screen->format, 0xD7, 0xC4, 0x96);

	SDL_FillRect(status->screen, NULL, background);

	draw_board(status);
	draw_player(status);
	draw_top_bar(status);

	if (status->points == status->board->targets)draw_win_screen(status);

}


void draw_top_bar(status_t *status) {

	char text[128];
	int red = SDL_MapRGB(status->screen->format, 0xFF, 0x00, 0x00);
	int blue = SDL_MapRGB(status->screen->format, 0x11, 0x11, 0xCC);
	DrawRectangle(status->screen, 4, 4, SCREEN_WIDTH - 8, 36, red, blue);
	sprintf(text, "Time: %.1lfs  %d / %d Boxes on target:  Moves: %d", status->worldTime, status->points, status->board->targets, status->moves);
	DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, 10, text, status->charset);
	sprintf(text, "Esc - exit  N-new game");
	DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, 26, text, status->charset);

}


void draw_board(status_t *status) {

	for (int i = 0; i < status->board->height; i++) {

		for (int j = 0; j < status->board->width; j++) {

			switch (status->board->board[i][j]) {

				case WALL:
					DrawSurface(status->screen, status->board->wall, X_STARTING + j*FIELD_SIZE, Y_STARTING + i*FIELD_SIZE);
					break;
				case FLOOR:
					DrawSurface(status->screen, status->board->floor, X_STARTING + j*FIELD_SIZE, Y_STARTING + i*FIELD_SIZE);
					break;
				case TARGET:
					DrawSurface(status->screen, status->board->target, X_STARTING + j*FIELD_SIZE, Y_STARTING + i*FIELD_SIZE);
					break;
				case BOX:
					DrawSurface(status->screen, status->board->box, X_STARTING + j*FIELD_SIZE, Y_STARTING + i*FIELD_SIZE);
					break;
				case BOX_ON_TARGET:
					DrawSurface(status->screen, status->board->box_on_target, X_STARTING + j*FIELD_SIZE, Y_STARTING + i*FIELD_SIZE);
					break;
			}
		}
	}
}


void draw_player(status_t *status) {
	DrawSurface(status->screen, status->board->player[0], X_STARTING + status->board->playerx*FIELD_SIZE, Y_STARTING + status->board->playery*FIELD_SIZE);
}


void draw_win_screen(status_t *status) {

	char nick[20] = "";
	
	handle_text_input(status, nick);

	add_high_score(status, nick);

	reset_status(status);
	status->pause = true;
	status->game_status = STATUS_MENU;
}

void draw_win_screen_info(status_t *status) {

	int red = SDL_MapRGB(status->screen->format, 0xFF, 0x00, 0x00);
	int blue = SDL_MapRGB(status->screen->format, 0x11, 0x11, 0xCC);
	char text[128];

	DrawRectangle(status->screen, 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH - 8, 50, red, blue);
	sprintf(text, "ALL BOXES ON TARGETS! Type nick for leaderboards:");
	DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, SCREEN_HEIGHT / 4 + 15, text, status->charset);

}

void animate_move(status_t *status, int x_start, int y_start, int x_dir, int y_dir, int box_move) {

	int x_draw = x_start * FIELD_SIZE, y_draw = y_start * FIELD_SIZE;
	int playerx = status->board->playerx, playery = status->board->playery;
	int anim = 0;
	int anim_speed = 0, move_speed = 0;

	while (x_draw != playerx * FIELD_SIZE || y_draw != playery * FIELD_SIZE) {

		update_time(status);
		draw_top_bar(status);
		if (box_move == BOX_MOVE)
			DrawSurface(status->screen, status->board->box, X_STARTING + x_draw + x_dir*FIELD_SIZE, Y_STARTING + y_draw + y_dir*FIELD_SIZE);

		if(status->board->board[playery - y_dir][playerx - x_dir] == FLOOR)
			DrawSurface(status->screen, status->board->floor, X_STARTING + FIELD_SIZE*(playerx - x_dir), Y_STARTING + FIELD_SIZE*(playery - y_dir));
		else if (status->board->board[playery - y_dir][playerx - x_dir] == TARGET)
			DrawSurface(status->screen, status->board->target, X_STARTING + FIELD_SIZE*(playerx - x_dir), Y_STARTING + FIELD_SIZE*(playery - y_dir));

		DrawSurface(status->screen, status->board->player[anim], X_STARTING + x_draw, Y_STARTING + y_draw);

		refresh(status);

		anim_speed = (++anim_speed) % 12;
		move_speed = (++move_speed) % 6;
		if (move_speed == 0) {
			x_draw += x_dir;
			y_draw += y_dir;
		}
		if(anim_speed == 0)anim = (++anim) % 8;
	}
	
}


void draw_menu(status_t *status) {

	int red = SDL_MapRGB(status->screen->format, 0xFF, 0x00, 0x00);
	int background = SDL_MapRGB(status->screen->format, 0x11, 0x11, 0xCC);
	char text[20];

	SDL_FillRect(status->screen, NULL, background);

	DrawRectangle(status->screen, 4, 4, SCREEN_WIDTH - 8, SCREEN_HEIGHT - 8, red, background);
	
	DrawRectangle(status->screen, status->screen->w / 2 - 20 * 8 / 2, status->option*FIELD_SIZE-10, 20*8, FIELD_SIZE - 5, red, background);

	sprintf(text, "Resume game");
	DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, FIELD_SIZE, text, status->charset);
	sprintf(text, "Load board");
	DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, 2*FIELD_SIZE, text, status->charset);
	sprintf(text, "Add board");
	DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, 3*FIELD_SIZE, text, status->charset);
	sprintf(text, "High scores");
	DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, 4*FIELD_SIZE, text, status->charset);
	sprintf(text, "Exit game");
	DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, 5 * FIELD_SIZE, text, status->charset);
}


void draw_load_menu(status_t *status) {

	int red = SDL_MapRGB(status->screen->format, 0xFF, 0x00, 0x00);
	int background = SDL_MapRGB(status->screen->format, 0x11, 0x11, 0xCC);
	int row = 1;

	SDL_FillRect(status->screen, NULL, background);

	DrawRectangle(status->screen, 4, 4, SCREEN_WIDTH - 8, SCREEN_HEIGHT - 8, red, background);

	for (int i = 0, j = 0; i < status->boards_number; i++, j++) {

		if(i == status->actual_board)DrawRectangle(status->screen, row * 30 * 8 / 2 - 2*FIELD_SIZE - 18, FIELD_SIZE + j*FIELD_SIZE - 8,
			20 * 8, FIELD_SIZE - 5, red, background);

		DrawString(status->screen, row*30*8/2 - FIELD_SIZE, FIELD_SIZE + j*FIELD_SIZE,
			status->boards_avalaible[i], status->charset);
		
		if (i % MAX_IN_ROW == MAX_IN_ROW - 1) {
			j = -1;
			row++;
		}

		if (i == MAX_BOARDS - 1)break;
	}
}


void draw_add_menu(status_t *status) {

	char input[15] = "";

	draw_add_instruct(status);
	
	handle_text_input(status, input);
	
	if (status->game_status == STATUS_ADD && add_game(status, input) == LOAD_ERROR)status->game_status = STATUS_ERROR;
	else status->game_status = STATUS_MENU;
	
}


void draw_add_instruct(status_t *status) {

	int red = SDL_MapRGB(status->screen->format, 0xFF, 0x00, 0x00);
	int background = SDL_MapRGB(status->screen->format, 0x11, 0x11, 0xCC);
	char text[40] = "";

	SDL_FillRect(status->screen, NULL, background);

	DrawRectangle(status->screen, 4, 4, SCREEN_WIDTH - 8, SCREEN_HEIGHT - 8, red, background);

	sprintf(text, "Type the file name of your game:");
	DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, FIELD_SIZE, text, status->charset);
}


void draw_error(status_t *status) {

	int red = SDL_MapRGB(status->screen->format, 0xFF, 0x00, 0x00);
	int background = SDL_MapRGB(status->screen->format, 0x11, 0x11, 0xCC);
	char text[40] = "";

	DrawRectangle(status->screen, 4, 4, SCREEN_WIDTH - 8, SCREEN_HEIGHT - 8, red, background);

	if (status->boards_number > MAX_BOARDS)sprintf(text, "There are too many games");
	else sprintf(text, "Wrong file");
	DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, FIELD_SIZE, text, status->charset);
}

void draw_high_scores(status_t *status) {


	int red = SDL_MapRGB(status->screen->format, 0xFF, 0x00, 0x00);
	int background = SDL_MapRGB(status->screen->format, 0x11, 0x11, 0xCC);
	char text[40] = "";
	int row = 1;

	DrawRectangle(status->screen, 4, 4, SCREEN_WIDTH - 8, SCREEN_HEIGHT - 8, red, background);
	sprintf(text, "High Scores:");
	DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, 6, text, status->charset);

	for (int i = 0, j = 0; i < status->boards_number; i++, j++) {

		if (i == status->board_chosen)DrawRectangle(status->screen, row * 30 * 8 / 2 - 2 * FIELD_SIZE - 18, FIELD_SIZE + j*FIELD_SIZE - 8,
			20 * 8, FIELD_SIZE - 5, red, background);

		DrawString(status->screen, row * 30 * 8 / 2 - FIELD_SIZE, FIELD_SIZE + j*FIELD_SIZE,
			status->boards_avalaible[i], status->charset);

		if (i % MAX_IN_ROW == MAX_IN_ROW - 1) {
			j = -1;
			row++;
		}

		if (i == MAX_BOARDS - 1)break;
	}
}

void draw_score_list(status_t *status) {

	int red = SDL_MapRGB(status->screen->format, 0xFF, 0x00, 0x00);
	int background = SDL_MapRGB(status->screen->format, 0x11, 0x11, 0xCC);
	char text[120] = "";

	DrawRectangle(status->screen, 4, 4, SCREEN_WIDTH - 8, SCREEN_HEIGHT - 8, red, background);
	sprintf(text, "High Scores: %s", status->boards_avalaible[status->board_chosen]);
	DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, FIELD_SIZE, text, status->charset);

	DrawRectangle(status->screen, status->sort_by*FIELD_SIZE - 10, 2 * FIELD_SIZE - 10, 10 * 8, FIELD_SIZE - 5, red, background);

	sprintf(text, "By time");
	DrawString(status->screen, SORT_TIME*FIELD_SIZE, 2 * FIELD_SIZE, text, status->charset);

	sprintf(text, "By moves");
	DrawString(status->screen, SORT_MOVE*FIELD_SIZE, 2 * FIELD_SIZE, text, status->charset);

	if (status->high_scores != NULL) {

		int row = 0;

		for (int i = 0, j = 0; i < status->high_scores_number; i++, j++) {

			sprintf(text, "%d. %s %d %.1f", i + 1, status->high_scores[i].nick, status->high_scores[i].moves, status->high_scores[i].time);
			DrawString(status->screen, FIELD_SIZE + row * 30 * 7, 3 * FIELD_SIZE + j*FIELD_SIZE,
				text, status->charset);

			if (i % MAX_HIGH_SCORES == MAX_HIGH_SCORES - 1) {
				j = -1;
				row++;
			}

			if (i == MAX_SCORES - 1)break;
		}
	}

}