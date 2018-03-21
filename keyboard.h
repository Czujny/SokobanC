#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H
#define UP 0
#define RIGHT 1
#define DOWN 3
#define LEFT 4

#include "structures.h"

int handle_keyboard(status_t *status);

void handle_text_input(status_t *status, char *text);

#endif // !KEYBOARD_H
