#include "map.h"

void water_update(GAME_GO* self) {
}
void grass_update(GAME_GO* self) {
}
void tree_update(GAME_GO* self) {
}

MAP* _MAP_Set_Self(MAP* p) {
  static MAP* self = NULL;

  if (p) self = p;

  return self;
}

MAP* MAP_Get_Self(void) {
  return _MAP_Set_Self(NULL);
}

