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
	int s = 1;

	HAN_Status handler;
	GAME game;
	LCD lcd;
	
	SDL_Surface* lcd_bmp = NULL;
	SDL_Rect     lcd_size = 

	DEFER (
		GAME_Create(&game, &lcd, &handler), // if making a game was
																				// this easy lmao
		cleanup
	);

	LCD_Char custom = {
		0b00000,
		0b00000,
		0b00100,
		0b00100,
		0b00100,
		0b11110,
		0b11111,
		0b00000
	};

	size_t id = 0;
	DEFER (
			LCD_Char_Data_Create_Custom_Char(&id, &lcd, custom),
			cleanup
	);

	while (game.running) {
		GAME_Loop_Begin();

		DEFER (
			GAME_Loop_Update(),
			cleanup
		);

		GAME_Loop_Draw();

		GAME_Loop_Finish();

		/*
		if (frames % 30 == 0) {
			CHECK (
				LCD_Out_Clear(&lcd),
				cleanup
			);

			CHECK (
				LCD_State_Set_Cursor(&lcd, 0, 0),
				cleanup
			);

			CHECK (
				LCD_Out_Text(&lcd, "BY TSUKIGVA"),
				cleanup
			);

			CHECK (
				LCD_Displayed_Character_Update_All(&lcd),
				cleanup
			);
		}
		*/	
	}
	
cleanup:
	GAME_Free(&game);
	//LCD_Free(&lcd);

	//SDL_DestroyWindow(win);
	//SDL_Quit();

die:
	return s;
}

