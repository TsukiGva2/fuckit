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

int main(void) {
	HAN_Status handler;
	GAME game;
	LCD lcd;

	DEFER (
		GAME_Create(&game, &lcd, &handler), // if making a game was
																				// this easy lmao
		cleanup
	);

	LCD_Char custom = {
		0b00000,
		0b00000,
		0b10010,
		0b11111,
		0b11111,
		0b10010,
		0b00000,
		0b00000
	};

	size_t id = 0;
	DEFER (
			LCD_Char_Data_Create_Custom_Char(&id, &lcd, custom),
			cleanup
	);

  GAME_OBJECT car;
  // TODO: no checking for errors because there
  // are currently no errors that can be thrown
  // by this function
  GAME_GO_Create(&car.go, 0, 0, &id, 1);
  GAME_Add_Game_Object(&car);

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

