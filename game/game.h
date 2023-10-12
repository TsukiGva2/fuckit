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

/* game objects */
typedef struct GAME_OBJECT {
  GAME_GO go;

  struct GAME_OBJECT* prev;
  struct GAME_OBJECT* next;
} GAME_OBJECT;

/* game */
typedef struct GAME {
  SDL_Window*  win;
  SDL_Surface* scr;
  
  LCD* lcd;

  FPS fps;

  GAME_OBJECT objs_head;
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
void        GAME_Loop_Begin(void);


HAN_Status* GAME_Loop_Update(void);
HAN_Status* GAME_Loop_Draw(void);

void        GAME_Loop_Finish(void);

/* misc */
HAN_Status* GAME_Load_BG(void);
void				GAME_Free(GAME* game);

/* game objects (but not game object enough for gameobj.h) */
HAN_Status* GAME_Add_Game_Object(GAME_OBJECT* go);
HAN_Status* GAME_Game_Object_Update_All(void);

#endif

