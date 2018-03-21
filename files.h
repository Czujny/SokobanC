#pragma once
#ifndef FILES_H
#define FILES_H

#include "structures.h"

#define MAX_NAME 12
#define GAMES_DIR "boards/"
#define CONF_FILE "sok.conf"
#define HIGH_SCORES_DIR "highscores/"
#define GAMES_EXTENSION ".sb"
#define HIGH_SCORES_EXTENSION ".hs"
#define NO_HIGH_SCORES 0
#define FLAG_SAVE 0
#define FLAG_LOAD 1

int load_board(status_t *status, const char *file_name);

int load_conf(status_t *status);

int check_file(char *file_name);

void refresh_conf_file(status_t *status);

void save_high_score_to_file(status_t *status, char *nick);

char *get_game_name(status_t *status, int flag);

char *get_high_score_file_name(status_t *status, int flag);

void get_high_scores(status_t *status);
#endif // !FILES_H
