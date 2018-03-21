#include "stdafx.h"
#include "game_init.h"
#include <stdio.h>

int init_game(status_t *status) {

	if (init_status(status) == LOAD_ERROR)return INIT_ERROR;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return INIT_ERROR;
	}

	status->rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&status->window, &status->renderer);

	if (status->rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return INIT_ERROR;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(status->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(status->renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(status->window, "Sokoban Kamil Jedrzejczak 171660");


	status->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	status->scrtex = SDL_CreateTexture(status->renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	status->charset = SDL_LoadBMP("./bmp/cs8x8.bmp");
	if (status->charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		free_game(status);
		return INIT_ERROR;
	};
	SDL_SetColorKey(status->charset, true, 0x000000);

	return INIT_SUCCESS;
}


void free_game(status_t *status) {

	SDL_FreeSurface(status->charset);
	SDL_FreeSurface(status->screen);
	SDL_FreeSurface(status->board->box);
	SDL_FreeSurface(status->board->box_on_target);
	SDL_FreeSurface(status->board->wall);
	SDL_FreeSurface(status->board->floor);
	SDL_FreeSurface(status->board->target);
	for(int i = 0; i < 8; i++)SDL_FreeSurface(status->board->player[i]);
	SDL_DestroyTexture(status->scrtex);
	SDL_DestroyRenderer(status->renderer);
	SDL_DestroyWindow(status->window);
	delete status->board;
	for (int i = 0; i < status->boards_number; i++)delete[] status->boards_avalaible[i];
	delete[] status->boards_avalaible;

	SDL_Quit();
}