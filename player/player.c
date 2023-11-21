#include "player.h"

void player_update(GAME_GO* player_go) {
  GAME* game = GAME_Get_Self();
  MAP* map = MAP_Get_Self();

  printf("%zu\n", map->game_map[1]);
}

PLAYER* _PLAYER_Set_Self(PLAYER* p) {
  static PLAYER* self = NULL;

  if (p) self = p;

  return self;
}

PLAYER* PLAYER_Get_Self(void) {
  return _PLAYER_Set_Self(NULL);
}

