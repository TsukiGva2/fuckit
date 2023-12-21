/* --------------------------------------------
 *                  FUCK IT!                  
 *                By Tsuki Gva
 *
 *                TODO's:
 *									- make the LCD cols
 *										be a linked list
 *										( cool )
 * -------------------------------------------- */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "fuckit.h"
#include "LCD/LCD.h"
#include "game/game.h"

#include "resources/sprites.h"

#include "player/player.h"
#include "map/map.h"

int main() {
	HAN_Status handler;
	GAME game;
	LCD lcd;
  
  // seeding rng
  srand(time(NULL));

	DEFER (
		GAME_Create(&game, &lcd, &handler), // if making a game was
																				// this easy lmao
		cleanup
	);	

  // creating player "object"
	PLAYER player;
  _PLAYER_Set_Self(&player);

	player.id = 0;

	DEFER (
			LCD_Char_Data_Create_Custom_Char(&player.id, &lcd, player_sprite),
			cleanup
	);	

  // TODO: no checking for errors because there
  // are currently no errors that can be thrown
  // by this function
  GAME_GO_Create(&player.obj.go, 0, 0, &player.id, 1);
  GAME_Add_Game_Object(&player.obj, &player_update);

  // creating map "object"
  MAP map;
  _MAP_Set_Self(&map);

  map.terrain.water_id = 0;
  map.terrain.grass_id = 0;
  map.terrain.tree_id  = 0;

  DEFER (
			LCD_Char_Data_Create_Custom_Char(&map.terrain.water_id, &lcd, water_spr),
			cleanup
	);
	DEFER (
			LCD_Char_Data_Create_Custom_Char(&map.terrain.grass_id, &lcd, grass_spr),
			cleanup
	);
	DEFER (
			LCD_Char_Data_Create_Custom_Char(&map.terrain.tree_id, &lcd, tree_spr),
			cleanup
	);

  // terrain
	map.size        = MAP_SIZE;
  map.objs[0]     = player.obj;
  map.game_map[0] = player.id;

	player.map_position = 0;

  //printf("%zu %zu %zu\n", water_id, grass_id, tree_id);

  for (size_t i = 1; i < map.size; i++) {
    int r = rand() % TERRAINS;

    CELL_ID* id = NULL;
    update_fnptr_t fn = NULL;

    switch (r) {
      case 0:
        id = &map.terrain.water_id;
        fn = &water_update;
        break;
      case 1:
        id = &map.terrain.grass_id;
        fn = &grass_update;
        break;
      case 2:
        id = &map.terrain.tree_id;
        fn = &tree_update;
        break;
    }

    map.game_map[i] = *id;

    GAME_GO_Create(&map.objs[i].go, i % MAP_COLS, ceil(i / MAP_COLS), id, 1);
    GAME_Add_Game_Object(&map.objs[i], fn);
  }

	while (game.running) {
		GAME_Loop_Begin();

		DEFER (
			GAME_Loop_Update(),
			cleanup
		);

		GAME_Loop_Draw();

		GAME_Loop_Finish();
	}
	
cleanup:
	GAME_Free(&game);
	//SDL_DestroyWindow(win);
	//SDL_Quit();
}

