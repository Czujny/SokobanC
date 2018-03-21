#include "stdafx.h"
#include "gamelogic.h"
#include "keyboard.h"
#include "render.h"
#include "files.h"

void move(status_t *status, int direction) {

	if (!status->pause && status->game_status == STATUS_GAME) {

		SDL_FlushEvent(SDL_KEYDOWN);

		int posx = status->board->playerx, posy = status->board->playery;

		switch (direction) {

			case UP: {
				int move = execute_move(status, NO_MOVE, FLAG_UP);
				if (move != NO_MOVE) {
					status->moves++;
					status->board->playery--;
					animate_move(status, posx, posy, NO_MOVE, FLAG_UP, move);
				}
				break;
			}
			case RIGHT: {
				int move = execute_move(status, FLAG_RIGHT, NO_MOVE);
				if (move != NO_MOVE) {
					status->moves++;
					status->board->playerx++;
					animate_move(status, posx, posy, FLAG_RIGHT, NO_MOVE, move);
				}
				break;
			}
			case DOWN: {
				int move = execute_move(status, NO_MOVE, FLAG_DOWN);
				if (move != NO_MOVE) {
					status->moves++;
					status->board->playery++;
					animate_move(status, posx, posy, NO_MOVE, FLAG_DOWN, move);
				}
				break;
			}
			case LEFT: {
				int move = execute_move(status, FLAG_LEFT, NO_MOVE);
				if (move != NO_MOVE) {
					status->moves++;
					status->board->playerx--;
					animate_move(status, posx, posy, FLAG_LEFT, NO_MOVE, move);
				}
				break;
			}
		}
	}
}


int execute_move(status_t *status, int flag_x, int flag_y) {

	int starty = status->board->playery, startx = status->board->playerx;

	if (status->board->board[starty + flag_y][startx + flag_x] != WALL) {

		if (status->board->board[starty][startx] == PLAYER_ON_TARGET) {

			if (status->board->board[starty + flag_y][startx + flag_x] == TARGET) {
				status->board->board[starty + flag_y][startx + flag_x] = PLAYER_ON_TARGET;
				status->board->board[starty][startx] = TARGET;
				return PLAYER_MOVE;
			}
			else if (status->board->board[starty + flag_y][startx + flag_x] == FLOOR) {

				status->board->board[starty + flag_y][startx + flag_x] = PLAYER;
				status->board->board[starty][startx] = TARGET;
				return PLAYER_MOVE;
			}
			else if (box_move(status, flag_x, flag_y))return BOX_MOVE;
		}
		else {

			if (status->board->board[starty + flag_y][startx + flag_x] == TARGET) {

				status->board->board[starty + flag_y][startx + flag_x] = PLAYER_ON_TARGET;
				status->board->board[starty][startx] = FLOOR;
				return PLAYER_MOVE;
			}
			else if (status->board->board[starty + flag_y][startx + flag_x] == FLOOR) {

				status->board->board[starty + flag_y][startx + flag_x] = PLAYER;
				status->board->board[starty][startx] = FLOOR;
				return PLAYER_MOVE;
			}
			else if (box_move(status, flag_x, flag_y))return BOX_MOVE;
		}
	}

	return false;
}


bool box_move(status_t *status, int flag_x, int flag_y) {

	int starty = status->board->playery, startx = status->board->playerx;

	if (status->board->board[starty + 2 * flag_y][startx + 2 * flag_x] != WALL && status->board->board[starty + 2 * flag_y][startx + 2 * flag_x] != BOX &&
		status->board->board[starty + 2 * flag_y][startx + 2 * flag_x] != BOX_ON_TARGET) {

		switch (status->board->board[starty][startx]) {

			case PLAYER:
				if (status->board->board[starty + 2 * flag_y][startx + 2 * flag_x] == TARGET) {

					status->board->board[starty + 2 * flag_y][startx + 2 * flag_x] = BOX_ON_TARGET;
					status->points++;
					if (status->board->board[starty + flag_y][startx + flag_x] == BOX_ON_TARGET) {
						status->board->board[starty + flag_y][startx + flag_x] = PLAYER_ON_TARGET;
						status->points--;
					}
					else status->board->board[starty + flag_y][startx + flag_x] = PLAYER;
					status->board->board[starty][startx] = FLOOR;
					return true;
				}
				else if (status->board->board[starty + 2 * flag_y][startx + 2 * flag_x] == FLOOR) {

					status->board->board[starty + 2 * flag_y][startx + 2 * flag_x] = BOX;
					if (status->board->board[starty + flag_y][startx + flag_x] == BOX_ON_TARGET) {
						status->board->board[starty + flag_y][startx + flag_x] = PLAYER_ON_TARGET;
						status->points--;
					}
					else status->board->board[starty + flag_y][startx + flag_x] = PLAYER;
					status->board->board[starty][startx] = FLOOR;
					return true;
				}
				break;
			case PLAYER_ON_TARGET:
				if (status->board->board[starty + 2 * flag_y][startx + 2 * flag_x] == TARGET) {

					status->board->board[starty + 2 * flag_y][startx + 2 * flag_x] = BOX_ON_TARGET;
					status->points++;
					if (status->board->board[starty + flag_y][startx + flag_x] == BOX_ON_TARGET) {
						status->board->board[starty + flag_y][startx + flag_x] = PLAYER_ON_TARGET;
						status->points--;
					}
					else status->board->board[starty + flag_y][startx + flag_x] = PLAYER;
					status->board->board[starty][startx] = TARGET;
					return true;
				}
				else if (status->board->board[starty + 2 * flag_y][startx + 2 * flag_x] == FLOOR) {

					status->board->board[starty + 2 * flag_y][startx + 2 * flag_x] = BOX;
					if (status->board->board[starty + flag_y][startx + flag_x] == BOX_ON_TARGET) {
						status->board->board[starty + flag_y][startx + flag_x] = PLAYER_ON_TARGET;
						status->points--;
					}
					else status->board->board[starty + flag_y][startx + flag_x] = PLAYER;
					status->board->board[starty][startx] = TARGET;
					return true;
				}
				break;
		}
	}
	return false;
}


void change_option(status_t *status, int direction) {

	if (direction == UP && status->option > RESUME)status->option--;
	if (direction == DOWN && status->option < EXIT)status->option++;
}


void choose_option(status_t *status) {

	switch (status->option) {

		case RESUME:
			status->game_status = STATUS_GAME;
			status->pause = false;
			break;
		case LOAD:
			status->game_status = STATUS_LOAD;
			break;
		case ADD:
			status->game_status = STATUS_ADD;
			break;
		case HIGH_SCORES:
			status->game_status = STATUS_HIGH_SCORES;
			break;
		case EXIT:
			status->quit = true;
			break;
	}
}


void change_game(status_t *status, int direction) {

	if (direction == UP && status->actual_board > 0)status->actual_board--;
	if (direction == DOWN && status->actual_board < status->boards_number - 1 && status->actual_board < MAX_BOARDS - 1)status->actual_board++;
}


void choose_game(status_t *status) {

	reset_status(status);
	status->game_status = STATUS_GAME;
}


int add_game(status_t *status, char *game_name) {

	if (check_file(game_name) == LOAD_ERROR || status->boards_number > MAX_BOARDS)return LOAD_ERROR;
	else {

		status->boards_number++;
		char **temp = new char*[status->boards_number];
		for (int k = 0; k < status->boards_number; k++)temp[k] = new char[MAX_NAME]();

		for (int i = 0; i < status->boards_number - 1; i++) {

			for (int j = 0; status->boards_avalaible[i][j] != '\0'; j++)temp[i][j] = status->boards_avalaible[i][j];

			delete[] status->boards_avalaible[i];
		}

		for (int i = 0; game_name[i] != '\0'; i++)temp[status->boards_number - 1][i] = game_name[i];

		delete[] status->boards_avalaible;

		status->boards_avalaible = temp;

		refresh_conf_file(status);

		return LOAD_SUCCESS;
	}
}

void change_high_score(status_t *status, int direction) {

	if (direction == UP && status->board_chosen > 0)status->board_chosen--;
	if (direction == DOWN && status->board_chosen < status->boards_number - 1 && status->board_chosen < MAX_BOARDS - 1)status->board_chosen++;
}

void add_high_score(status_t *status, char *nick) {

	save_high_score_to_file(status, nick);
}
