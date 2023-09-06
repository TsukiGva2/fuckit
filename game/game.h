#ifndef __FUCKIT_GAME_H__
#define __FUCKIT_GAME_H__

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>

#include "../fuckit.h"
#include "../LCD/LCD.h"
#include "../handler/han.h"

#include "gameobj.h"

typedef struct FPS {
	uint32_t min_ft;
	uint32_t curr;
	uint32_t ft;

	uint64_t frames; // may overflow
} FPS;

typedef struct BG {
	SDL_Rect r;
	SDL_Surface* img;
} BG;

typedef struct GAME {
	SDL_Window*  win;
	SDL_Surface* scr;
	
	LCD* lcd;

	FPS fps;

	GAME_GO* objs;
	BG bg;

	bool running;
	bool update;
} GAME;

/* Functions */
HAN_Status* GAME_Create(
		GAME* game,
		LCD* lcd,
		HAN_Status* handler);

/* logic */
void				GAME_Loop_Begin(void);


HAN_Status* GAME_Loop_Update(void);
HAN_Status*	GAME_Loop_Draw(void);

void				GAME_Loop_Finish(void);

/* misc */
void GAME_Free(GAME* game);

HAN_Status* GAME_Load_BG(void);

#endif

