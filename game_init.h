#pragma once
#ifndef GAME_INIT_H
#define GAME_INIT_H

#include "structures.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define INIT_SUCCESS 1
#define INIT_ERROR 0

int init_game(status_t *status);

void free_game(status_t *status);

#endif // !GAME_INIT_H


