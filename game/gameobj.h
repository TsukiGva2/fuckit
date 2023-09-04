#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <SDL2/SDL.h>
#include <stdtypes.h>

#include "../fuckit.h"
#include "../LCD/LCD.h"
#include "../handler/han.h"

typedef struct GAME_GO {
	uint8_t x; // the screen is fucking 16x2
						 // i dont need big ints for this
	uint8_t y;
	LCD_Char sprite;
} GAME_GO;
/*typedef GAME_GO GAME_Game_Object;*/

#endif

