// SokobanInf.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "structures.h"
#include "game_init.h"
#include "render.h"
#include "keyboard.h"

int main(int argc, char **argv) {

	status_t status;

	if (init_game(&status) == INIT_ERROR)return INIT_ERROR;

	while (!status.quit) {

			update_time(&status);

			render(&status);

			if (handle_keyboard(&status) == LOAD_ERROR)status.quit = 1;

			update_fps(&status);

	}		

	delete_board(&status);
	free_game(&status);
	return 0;
}
