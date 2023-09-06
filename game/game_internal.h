#ifndef __GAME_INTERNAL__
#define __GAME_INTERNAL__

#include "game.h"

/* internal */
static GAME* _GAME_Get_Self(GAME* game);
static GAME* GAME_Get_Self (void);

static HAN_Status* GAME_Create_SDL_Window_And_Screen(void);

static void _GAME_Check_Update_Condition(void);
static void GAME_Loop_Process_Input(void);

#endif

