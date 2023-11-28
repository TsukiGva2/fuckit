#ifndef __FUCKIT_MAP_H__
#define __FUCKIT_MAP_H__

#define MAP_COLS 5
#define MAP_ROWS 2
#define MAP_SIZE (MAP_COLS * MAP_ROWS)

#define TERRAINS 3 // water grass tree

#include "../game/game.h"

typedef struct TERRAIN {
  size_t water_id;
  size_t grass_id;
  size_t tree_id;
} TERRAIN;

typedef struct MAP {
  GAME_OBJECT objs[MAP_SIZE];

  TERRAIN terrain;

  size_t game_map[MAP_SIZE];
} MAP;

// terrain
void water_update(GAME_GO* self);
void grass_update(GAME_GO* self);
void tree_update(GAME_GO* self);

// map
MAP* _MAP_Set_Self(MAP* m);
MAP* MAP_Get_Self(void);

#endif

