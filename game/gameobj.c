#include "gameobj.h"

HAN_Status* GAME_GO_Create(GAME_GO* obj,
                           uint8_t x, uint8_t y,
                           const size_t *sprite_ids,
                           size_t sprite_count) {
	obj->x = x;
	obj->y = y;

	obj->id = 0;
	obj->sprite_ids = sprite_ids;
	obj->sprite_count = sprite_count;

	obj->sprite = 0;

  return HAN_RETURN_OK;
}

