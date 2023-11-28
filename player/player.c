#include "player.h"
#include <stdlib.h>

void player_update(GAME_GO* player_obj) {
  GAME* game     = GAME_Get_Self();
  MAP* map       = MAP_Get_Self();
	PLAYER* player = PLAYER_Get_Self();

	// get up/down left/right
	GAME_OBJECT* up    = NULL;
	GAME_OBJECT* down  = NULL;
	GAME_OBJECT* left  = NULL;
	GAME_OBJECT* right = NULL;

	int pos = player->map_position;

	// TODO: Read player actions 
	// like: look for trees
	// or    swim
	// etc.
	// and try the best to perform them


	// Interact with adjacent cells
	// y bound down
	int adj_down = pos + MAP_COLS;
	if ((pos + MAP_COLS) < MAP_SIZE) {
		down = &map->objs[adj_down];
		CELL_ID interacting_id = map->game_map[adj_down];

		interact(&player, down, interacting_id);
	}
}

PLAYER* _PLAYER_Set_Self(PLAYER* p) {
  static PLAYER* self = NULL;

  if (p) self = p;

  return self;
}

PLAYER* PLAYER_Get_Self(void) {
  return _PLAYER_Set_Self(NULL);
}

