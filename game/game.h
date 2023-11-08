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
typedef void (*update_fnptr_t)(GAME_GO*); // GAME*
typedef struct GAME_OBJECT {
  GAME_GO go;

  update_fnptr_t update;

  struct GAME_OBJECT* prev;
  struct GAME_OBJECT* next;
} GAME_OBJECT;

typedef enum KeyMask {
  KEY_H = 1<<0,
  KEY_J = 1<<1,
  KEY_K = 1<<2,
  KEY_L = 1<<3,
  KEY_Q = 1<<4,
  KEY_SPACE = 1<<5,
  KEY_NONE = 1<<6
} KeyMask;

/* game */
typedef struct GAME_CONFIG {
  bool update_on_input;
} GAME_CONFIG;

typedef struct GAME {
  SDL_Window*  win;
  SDL_Surface* scr;

  GAME_CONFIG config;
  
  LCD* lcd;

  FPS fps;

  GAME_OBJECT objs_head;
  BG bg;

  bool running;
  bool update;

  KeyMask keys_pressed;
} GAME;

/* Functions */
HAN_Status* GAME_Create(
						GAME* game,
						LCD* lcd,
						HAN_Status* handler);

GAME*       GAME_Get_Self (void);

/* logic */
void        GAME_Loop_Begin(void);

HAN_Status* GAME_Loop_Update(void);
HAN_Status* GAME_Loop_Draw(void);

void        GAME_Loop_Finish(void);

/* input */
KeyMask     GAME_Get_KeyMask(SDL_Keycode);

void        GAME_Handle_Keydown(SDL_Keycode);
void        GAME_Handle_Keyup(SDL_Keycode);

bool        GAME_Is_Key_Pressed(SDL_Keycode);

// returns true if a keydown input was received
bool        GAME_Loop_Process_Input(void);

/* misc */
HAN_Status* GAME_Load_BG(void);
void				GAME_Free(GAME* game);

/* game objects (but not game object enough for gameobj.h) */
HAN_Status* GAME_Add_Game_Object(GAME_OBJECT* go,
                                 update_fnptr_t up);
HAN_Status* GAME_Game_Object_Update_All(void);

#endif

