#ifndef __FUCKIT_PLAYER_H__
#define __FUCKIT_PLAYER_H__

#include "../fuckit.h"
#include "../map/map.h"

typedef struct PLAYER {
  int hp;
} PLAYER;

extern const LCD_Char player_sprite;

void player_update(GAME_GO* player);

PLAYER* _PLAYER_Set_Self(PLAYER* p);
PLAYER* PLAYER_Get_Self(void);

#endif

