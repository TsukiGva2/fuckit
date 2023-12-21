#include "player.h"
#include <stdlib.h>

// DOWN:
// [_, x, _, _, -- x (1) + COLS (4) = 5
//  _, 5, _, _] -- we succesfully get the
//              -- cell below x,
//              -- which has index 5

#define UP_ADJ(x)    (x - MAP_COLS)
#define DOWN_ADJ(x)  (x + MAP_COLS)
#define LEFT_ADJ(x)  (x - 1)
#define RIGHT_ADJ(x) (x + 1)

void player_update(GAME_GO* player_obj) {
  //GAME* game     = GAME_Get_Self();
  MAP* map       = MAP_Get_Self();
	PLAYER* player = PLAYER_Get_Self();

	int pos = player->map_position;

	// TODO: Read player actions 
	// like: look for trees
	// or    swim
	// etc.
	// and try the best to perform them

	// Interact with adjacent cells

  int directions[] = {
    UP_ADJ(pos), DOWN_ADJ(pos), LEFT_ADJ(pos), RIGHT_ADJ(pos)
  };

  GAME_OBJECT* objptr;
  CELL_ID interacting_id;

  for (int i = 0; i < 4; i++) {
    GAME_OBJECT* objptr = NULL;

    int adj_pos = directions[i];

    if (adj_pos < MAP_SIZE && adj_pos >= 0) {
      objptr = &map->objs[adj_pos];
      interacting_id = map->game_map[adj_pos];

      printf("%d\n", adj_pos);

      // TODO: handle this error
      // instead of just showing
      // and moving on
      DEFER(
          interact(player, objptr, interacting_id),
          cont
      );
    }
cont:
    continue;
  }
}

HAN_Status* interact(PLAYER* p, GAME_OBJECT* obj, CELL_ID cid) {
  // check if can interact

  //debug:
  printf("player pos: %d\n adj cell id: %zu\nobj x, y: %d, %d\n", p->map_position, cid, obj->go.x, obj->go.y);

  return HAN_RETURN_OK;
}

PLAYER* _PLAYER_Set_Self(PLAYER* p) {
  static PLAYER* self = NULL;

  if (p) self = p;

  return self;
}

PLAYER* PLAYER_Get_Self(void) {
  return _PLAYER_Set_Self(NULL);
}

