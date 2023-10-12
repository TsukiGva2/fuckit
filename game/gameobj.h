#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "../fuckit.h"
#include "../LCD/LCD.h"
#include "../handler/han.h"

// make this a double linked list?
typedef struct GAME_GO {
	uint8_t x; // the screen is fucking 16x2
						 // i dont need big ints for this
	uint8_t y;
	size_t id;
	
	size_t sprite;

	const size_t *sprite_ids;
	size_t sprite_count;
} GAME_GO;
/*typedef GAME_GO GAME_Game_Object;*/

/* functions */
HAN_Status* GAME_GO_Create(GAME_GO* obj,
													 uint8_t x, uint8_t y,
													 const size_t *sprite_ids,
													 size_t sprite_count);

#endif

