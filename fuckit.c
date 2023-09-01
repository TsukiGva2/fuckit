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

#include "LCD.h"

int load_lcd_bmp(SDL_Surface** lcd_bmp) {
	*lcd_bmp = SDL_LoadBMP("lcd.bmp");

	if (*lcd_bmp == NULL) // TODO: error
		return 1;

	return 0;
}

int main(void) {
	LCD lcd;

	lcd.attr.rows = 2;
	lcd.attr.cols = 16;
	lcd.attr.size = lcd.attr.rows*lcd.attr.cols;
	lcd.attr.gap = 2;

	// TODO: error handling
	LCD_Char_Data_Initialize(&lcd);


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

	int s = load_lcd_bmp(&lcd_bmp);
	if (s != 0) goto cleanup; // 1

	scr = SDL_GetWindowSurface(win);
	if (scr == NULL) {
		printf("couldnt get win surface");
		s = 1;
		goto cleanup;
	}
	
	// TODO: error
	LCD_Displayed_Character_Create_All(&lcd);

	int run = 1;
	uint32_t ft;
	uint32_t min_ft = 16; // 60 fps

	// TODO: better tests than this
	// this is a pretty hacky way
	// to test the system
	// XXX
	LCD_Char custom = {
		0b00000,
		0b11111,
		0b11011,
		0b10101,
		0b10101,
		0b10101,
		0b11011,
		0b11111
	};

	size_t id =
		LCD_Char_Data_Create_Custom_Char(&lcd, custom);

	LCD_Out_Custom_Char(&lcd, id);

	LCD_Displayed_Character_Update_All(&lcd);

	LCD_State_Set_Cursor(&lcd, 2, 1);

	LCD_Out_Text(&lcd, "ABCDEFGHIJ");

	// up till here

	while (run) {
		ft = SDL_GetTicks();

		SDL_BlitScaled(
					lcd_bmp, NULL, scr, &lcd_size);

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

		uint32_t curr = SDL_GetTicks() - ft;
		if (curr < min_ft)
			SDL_Delay(min_ft - curr);
	}
	
cleanup:
	LCD_Free(&lcd);

	SDL_FreeSurface(lcd_bmp);
	SDL_DestroyWindow(win);

	win     = NULL;
	lcd_bmp = NULL;

	SDL_Quit();

	return s;
}

