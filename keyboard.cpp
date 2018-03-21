#include "stdafx.h"
#include "keyboard.h"
#include "gamelogic.h"
#include "files.h"
#include <string.h>
#include "render.h"
#include "draw.h"
#include "game_init.h"

int handle_keyboard(status_t *status) {

		if (status->game_status == STATUS_GAME && SDL_PollEvent(&status->event)) {
			switch (status->event.type) {
			case SDL_KEYDOWN:
				if (status->event.key.keysym.sym == SDLK_ESCAPE) status->quit = 1;
				if (status->event.key.keysym.sym == SDLK_UP)move(status, UP);
				if (status->event.key.keysym.sym == SDLK_DOWN)move(status, DOWN);
				if (status->event.key.keysym.sym == SDLK_RIGHT)move(status, RIGHT);
				if (status->event.key.keysym.sym == SDLK_LEFT)move(status, LEFT);
				if (status->event.key.keysym.sym == SDLK_n)if (reset_status(status) == LOAD_ERROR)return LOAD_ERROR;
				if (status->event.key.keysym.sym == SDLK_m) {

					status->pause = true;
					status->game_status = STATUS_MENU;
				}
				break;
			case SDL_QUIT:
				status->quit = 1;
				break;
			}
		}
		else if (status->game_status == STATUS_MENU && SDL_PollEvent(&status->event)) {
			switch (status->event.type) {
			case SDL_KEYDOWN:
				if (status->event.key.keysym.sym == SDLK_ESCAPE) status->quit = 1;
				if (status->event.key.keysym.sym == SDLK_UP)change_option(status, UP);
				if (status->event.key.keysym.sym == SDLK_DOWN)change_option(status, DOWN);
				if (status->event.key.keysym.sym == SDLK_RETURN)choose_option(status);
				break;
			case SDL_QUIT:
				status->quit = 1;
				break;
			}
		}
		else if (status->game_status == STATUS_LOAD && SDL_PollEvent(&status->event)) {
			switch (status->event.type) {
			case SDL_KEYDOWN:
				if (status->event.key.keysym.sym == SDLK_ESCAPE) status->game_status = STATUS_MENU;
				if (status->event.key.keysym.sym == SDLK_UP)change_game(status, UP);
				if (status->event.key.keysym.sym == SDLK_DOWN)change_game(status, DOWN);
				if (status->event.key.keysym.sym == SDLK_RETURN)choose_game(status);
				break;
			case SDL_QUIT:
				status->quit = 1;
				break;
			}
		}
		else if (status->game_status == STATUS_ERROR && SDL_PollEvent(&status->event)) {
			switch (status->event.type) {
			case SDL_KEYDOWN:
				 status->game_status = STATUS_ADD;
				break;
			case SDL_QUIT:
				status->quit = 1;
				break;
			}
		}
		else if (status->game_status == STATUS_HIGH_SCORES && SDL_PollEvent(&status->event)) {
			switch (status->event.type) {
			case SDL_KEYDOWN:
				if (status->event.key.keysym.sym == SDLK_ESCAPE) status->game_status = STATUS_MENU;
				if (status->event.key.keysym.sym == SDLK_UP)change_high_score(status, UP);
				if (status->event.key.keysym.sym == SDLK_DOWN)change_high_score(status, DOWN);
				if (status->event.key.keysym.sym == SDLK_RETURN) {

					get_high_scores(status);
					status->game_status = STATUS_SHOW_SCORES;;
				}
				break;
			case SDL_QUIT:
				status->quit = 1;
				break;
			}
		}
		else if (status->game_status == STATUS_SHOW_SCORES && SDL_PollEvent(&status->event)) {
			switch (status->event.type) {
			case SDL_KEYDOWN:
				if (status->event.key.keysym.sym == SDLK_ESCAPE) status->game_status = STATUS_HIGH_SCORES;
				if (status->event.key.keysym.sym == SDLK_LEFT) {

					status->sort_by = SORT_TIME;
					sort_high_scores(status);
				}
				if (status->event.key.keysym.sym == SDLK_RIGHT) {

					status->sort_by = SORT_MOVE;
					sort_high_scores(status);
				}
				break;
			case SDL_QUIT:
				status->quit = 1;
				break;
			}
		}
	return LOAD_SUCCESS;
}


void handle_text_input(status_t *status, char *text) {

	SDL_StartTextInput();
	bool key_input = true;

	while (key_input) {
		
		while (SDL_PollEvent(&status->event)) {

			switch(status->event.type){

				case SDL_TEXTINPUT:

					if (strlen(text) < MAX_NAME - 1)strcat(text, status->event.text.text);
					break;

				case SDL_KEYDOWN:
					if (status->event.key.keysym.sym == SDLK_RETURN) key_input = false;
					if (status->event.key.keysym.sym == SDLK_ESCAPE) {

						key_input = false;
						status->game_status = STATUS_MENU;
					}
					if (status->event.key.keysym.sym == SDLK_BACKSPACE)if(strlen(text) > 0)text[strlen(text) - 1] = '\0';
					break;
			}
		}

		if (status->game_status == STATUS_ADD) {

			draw_add_instruct(status);

			DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, 2 * FIELD_SIZE, text, status->charset);
		}
		else if (status->game_status == STATUS_GAME) {

			draw_win_screen_info(status);

			DrawString(status->screen, status->screen->w / 2 - strlen(text) * 8 / 2, SCREEN_HEIGHT/4 + FIELD_SIZE, text, status->charset);
		}
		
		refresh(status);
	}

	SDL_StopTextInput();
}