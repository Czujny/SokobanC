#include "stdafx.h"
#include "files.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int load_board(status_t *status, const char *file_name) {

	char dir[8] = GAMES_DIR;
	char *board_dir = new char[strlen(file_name) + strlen(dir) + 1](); // strlen nie liczy \0
	strcat(board_dir, dir);
	strcat(board_dir, file_name);
	FILE *game = fopen(board_dir, "r");

	delete[] board_dir;

	if (status->board->board != NULL)delete_board(status);
	if (game != NULL){
		char buff[4];
		fgets(buff, 4, game);
		int width = atoi(buff);
		fgets(buff, 4, game);
		int height = atoi(buff);

		if (width > 18 || height > 13)return LOAD_ERROR;

		int **board = new int*[height];
		int targets = 0;

		for (int i = 0; i < height; i++)board[i] = new int[width];

		for (int i = 0; i < height; i++) {

			for (int j = 0; j < width; j++) {

				board[i][j] = fgetc(game) - '0';
				if (board[i][j] == BOX_ON_TARGET)status->points++;
				if (board[i][j] == TARGET || board[i][j] == BOX_ON_TARGET || board[i][j] == PLAYER_ON_TARGET)targets++;
				else if (board[i][j] == PLAYER || board[i][j] == PLAYER_ON_TARGET) {
					status->board->playerx = j;
					status->board->playery = i;
				}
			}

			fgetc(game);	//pozbycie sie ENTERa
		}

		status->board->board = board;
		status->board->height = height;
		status->board->width = width;
		status->board->targets = targets;

		fclose(game);

		return LOAD_SUCCESS;
	}
	
	return LOAD_ERROR;
}


int load_conf(status_t *status) {

	FILE *conf = fopen(CONF_FILE, "r");
	char board_name[MAX_NAME] = "";

	if (conf != NULL) {

		char buff;
		int i = 0, j = 0;

		char buff1[4];
		fgets(buff1, 4, conf);
		status->boards_number = atoi(buff1);

		status->boards_avalaible = new char*[status->boards_number];
		for (int k = 0; k < status->boards_number; k++)status->boards_avalaible[k] = new char[MAX_NAME]();

		while ((buff = fgetc(conf)) != EOF) {

			if (buff == '\n') {
				
				board_name[i] = '\0';
				for (int k = 0; k <= i; k++)status->boards_avalaible[j][k] = board_name[k];
				for (int l = 0; l < MAX_NAME; l++)board_name[l] = NULL;
				i = 0;
				j++;
				continue;
			}
			board_name[i] = buff;
			i++;
		}

		fclose(conf);

		return LOAD_SUCCESS;
	}

	return LOAD_ERROR;
}


int check_file(char *file_name) {

	char *check_extension = NULL;
	check_extension = strstr(file_name, GAMES_EXTENSION);
	if (check_extension != NULL && *(check_extension+1) == '\0') {

		char dir[8] = GAMES_DIR;
		char *board_dir = new char[strlen(file_name) + strlen(dir) + 1](); // strlen nie liczy \0
		strcat(board_dir, dir);
		strcat(board_dir, file_name);
		FILE *file = fopen(board_dir, "r");

		if (file != NULL) {

			fclose(file);
			return LOAD_SUCCESS;
		}
		else return LOAD_ERROR;
	}
	else return LOAD_ERROR;
}


void refresh_conf_file(status_t *status) {

	FILE *conf = fopen(CONF_FILE, "w");

	if (conf != NULL) {

		char buff[4];
		_itoa(status->boards_number, buff, 10);
		fputs(buff, conf);

		fputc('\n', conf);

		for (int i = 0; i < status->boards_number; i++) {

			fputs(status->boards_avalaible[i], conf);
			fputc('\n', conf);
		}

		fclose(conf);
	}
}

void save_high_score_to_file(status_t *status, char *nick) {

	char *save_dir = get_high_score_file_name(status, FLAG_SAVE);

	FILE *high_score_file = fopen(save_dir, "a");

	fprintf(high_score_file, "%s %d %.1f\n", nick, status->moves, status->worldTime);

	delete[] save_dir;
	fclose(high_score_file);
}

char *get_game_name(status_t *status, int flag) {

	char *board_name = NULL;
	if (flag == FLAG_SAVE)board_name = status->boards_avalaible[status->actual_board];
	else if (flag == FLAG_LOAD)board_name = status->boards_avalaible[status->board_chosen];

	char *game_name = new char[strlen(board_name) - strlen(GAMES_EXTENSION) + 1]();

	int length = strcspn(board_name, GAMES_EXTENSION);

	for (int i = 0; i < length; i++)game_name[i] = board_name[i];

	game_name[length] = '\0';

	return game_name;
}

char *get_high_score_file_name(status_t *status, int flag) {

	char *game_name = get_game_name(status, flag);

	char *file_dir = new char[strlen(game_name) + strlen(HIGH_SCORES_DIR) + strlen(HIGH_SCORES_EXTENSION) + 1](); // strlen nie liczy \0
	strcat(file_dir, HIGH_SCORES_DIR);
	strcat(file_dir, game_name);
	strcat(file_dir, HIGH_SCORES_EXTENSION);

	delete[] game_name;

	return file_dir;
}

void get_high_scores(status_t *status) {

	int high_scores_number = 0;

	high_score_t *high_scores = new high_score_t[1]();

	high_scores[0].nick = new char[MAX_NAME];

	char *file_name = get_high_score_file_name(status, FLAG_LOAD);

	FILE *high_scores_file = fopen(file_name, "r");

	delete[] file_name;

	if (high_scores_file != NULL) {

		while (fscanf(high_scores_file, "%s%d%f", high_scores[high_scores_number].nick, &high_scores[high_scores_number].moves,
			&high_scores[high_scores_number].time) == 3) {

			high_scores_number++;
			high_score_t *temp = new high_score_t[high_scores_number + 1]();
			for (int i = 0; i < high_scores_number; i++) {
				
				temp[i].moves = high_scores[i].moves;
				temp[i].nick = high_scores[i].nick;
				temp[i].time = high_scores[i].time;
			}

			delete[] high_scores;
			high_scores = temp;
			high_scores[high_scores_number].nick = new char[MAX_NAME];
		}

		if (status->high_scores != NULL) {

			for (int i = 0; i <= status->high_scores_number; i++)delete[] status->high_scores[i].nick;
			delete[] status->high_scores;
		}
		status->high_scores = high_scores;
		status->high_scores_number = high_scores_number;
		fclose(high_scores_file);
		return;
	}

	status->high_scores = NULL;
}