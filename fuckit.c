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

int load_lcd_bmp(SDL_Surface** lcd_bmp) {
	*lcd_bmp = SDL_LoadBMP("resources/lcd.bmp");

	if (*lcd_bmp == NULL) // TODO: error
		return 1;

	return 0;
}

int main(void) {
	int s = 1;

	HAN_Status handler;
	HAN_Create(&handler);

	LCD lcd;

	lcd.handler = &handler;

	lcd.attr.rows = 2;
	lcd.attr.cols = 16;
	lcd.attr.size = lcd.attr.rows*lcd.attr.cols;
	lcd.attr.gap = 2;

	CHECK(
		LCD_Char_Data_Initialize(&lcd),
		die
	);

	SDL_Window*  win     = NULL;
	SDL_Surface* scr     = NULL;

	SDL_Surface* lcd_bmp = NULL;
	
	SDL_Rect lcd_size = 
				{0, 0, WIN_W, WIN_H};

	
	win = SDL_CreateWindow(
					"fuck it!",
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					WIN_W, WIN_H,
					SDL_WINDOW_SHOWN);
	if (win == NULL) {
		printf("Couldnt create win");
		return 1;
	}

	// XXX: get rid of these goto's someday
	// XXX: i ended up adding more goto's
	// kill me

	s = load_lcd_bmp(&lcd_bmp);
	if (s != 0) goto cleanup; // 1

	scr = SDL_GetWindowSurface(win);
	if (scr == NULL) {
		printf("couldnt get win surface");
		s = 1;
		goto cleanup;
	}
	
	CHECK (
		LCD_Displayed_Character_Create_All(&lcd),
		cleanup
	);

	int run = 1;
	uint32_t ft;
	uint32_t min_ft = 16; // 60 fps
	uint32_t curr   = 0;

	// TODO: better tests than this
	// this is a pretty hacky way
	// to test the system
	// XXX
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
	CHECK (
			LCD_Char_Data_Create_Custom_Char(&id, &lcd, custom),
			cleanup
	);
/*
	CHECK (
		LCD_Out_Custom_Char(&lcd, id),
		cleanup
	);

	CHECK (
		LCD_Displayed_Character_Update_All(&lcd),
		cleanup
	);

	CHECK (
		LCD_State_Set_Cursor(&lcd, 1, 0),
		cleanup
	);

	CHECK (
		LCD_Out_Text(&lcd, "ABCDEFGHIJKMNOPQRSTUVWXYZ"),
		cleanup
	);
*/
	// up till here

	uint32_t frames = 0;
	int x = 0;
	int y = 0;

	while (run) {
		ft = SDL_GetTicks();

		SDL_BlitScaled(
					lcd_bmp, NULL, scr, &lcd_size);

		if (frames % 30 == 0) {
			CHECK (
				LCD_Out_Clear(&lcd),
				cleanup
			);

			CHECK (
				LCD_State_Set_Cursor(&lcd, x, y),
				cleanup
			);

			x++;

			if (x >= lcd.attr.cols - 10) {
				x = 0;
				y += 1;
				y %= 2;
			}

			CHECK (
				LCD_Out_Text(&lcd, "BY TSUKIGVA"),
				cleanup
			);

			CHECK (
				LCD_Displayed_Character_Update_All(&lcd),
				cleanup
			);
		}

		LCD_Displayed_Character_Draw_All(&lcd, scr);

		SDL_UpdateWindowSurface(win);

		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				run = 0;
			if (e.type == SDL_KEYDOWN)
				if (e.key.keysym.sym
						== SDLK_ESCAPE)
					run = 0;
		}

		curr = SDL_GetTicks() - ft;
		if (curr < min_ft)
			SDL_Delay(min_ft - curr);

		frames++;
	}
	
cleanup:
	LCD_Free(&lcd);

	SDL_FreeSurface(lcd_bmp);
	SDL_DestroyWindow(win);

	win     = NULL;
	lcd_bmp = NULL;

	SDL_Quit();

die:
	return s;
}

