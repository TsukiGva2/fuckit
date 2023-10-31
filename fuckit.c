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
#include <SDL2/SDL.h>

#include "fuckit.h"
#include "LCD/LCD.h"
#include "game/game.h"

#define P 2

uint8_t map[] = {1,1,1,1,1,1,
                 1,0,0,0,0,1,
                 1,0,P,0,0,1,
                 1,0,0,0,0,1,
                 1,0,0,0,0,1,
                 1,1,1,1,1,1
                };

void player_update(GAME_GO* player) {
  GAME* game = GAME_Get_Self();

  if (++player->x >= game->lcd->attr.cols) {
    player->x = 0;
  }
}

void tile_update(GAME_GO* tile) {
  // nothing
}

int main(void) {
	HAN_Status handler;
	GAME game;
	LCD lcd;

	DEFER (
		GAME_Create(&game, &lcd, &handler), // if making a game was
																				// this easy lmao
		cleanup
	);

	LCD_Char player_sprite = {
		0b00000,
		0b01100,
		0b10010,
		0b11110,
		0b11110,
		0b10010,
		0b00000,
		0b00000
	};

  LCD_Char wall = {
		0b00000,
		0b01100,
		0b10010,
		0b11110,
		0b11110,
		0b10010,
		0b00000,
		0b00000
  };

	size_t player_sprite_id = 0;
	DEFER (
			LCD_Char_Data_Create_Custom_Char(&player_sprite_id, &lcd, player_sprite),
			cleanup
	);

  GAME_OBJECT player;
  // TODO: no checking for errors because there
  // are currently no errors that can be thrown
  // by this function
  GAME_GO_Create(&player.go, 0, 0, &player_sprite_id, 1);
  GAME_Add_Game_Object(&player, &player_update);

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

