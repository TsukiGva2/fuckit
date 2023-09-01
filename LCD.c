#include "LCD.h"
#include "debug.h"
#include "fuckit.h"
#include "LCD_Font.h"

/* Output */
void LCD_Out_Text(LCD* lcd, char* s) {
	for (char* c = s; *c != '\0'; c++) {
		if (*c >= 65 && *c <= 90) {
			LCD_Displayed_Character_Set_Data(lcd, (*c)-64);
		}

		LCD_State_Cursor_Advance(lcd);
	}
}

void LCD_Out_Custom_Char(LCD* lcd, size_t id) {
	LCD_Displayed_Character_Set_Data(lcd, id);
	LCD_State_Cursor_Advance(lcd);
}

/* Char Data */
void LCD_Char_Data_Initialize(LCD* lcd) {
	/*
	 * LCD_Char_Data:
	 *	len
	 *	cap
	 *
	 *	data
	 */

	lcd->char_data.len = sizeof(LCD_FONT) / LCD_CHAR_H;
	lcd->char_data.cap = lcd->char_data.len + 5;

	lcd->char_data.chars =
		(LCD_Char_Wrapper*)calloc(
				lcd->char_data.cap,
				sizeof(LCD_Char_Wrapper));

	if (lcd->char_data.chars == NULL) { // TODO: error
		return;
	}

	LCD_Char_Wrapper* w = lcd->char_data.chars;
	for (int i = 0; i < lcd->char_data.cap; i++) {
		w[i].size  = sizeof(LCD_Char);
		w[i].index = i;

		// old LOAD_FONT function
		// TODO: maybe make this a
		// separate function?
		if (i < lcd->char_data.len)
			LCD_Char_Wrapper_Copy_Data_Offset(
													w, LCD_FONT, i, i);
	}
}

void LCD_Char_Wrapper_Copy_Data(
											LCD_Char_Wrapper* w,
											uint8_t* data, size_t index) {
	// XXX? kinda hacky
	memcpy(w[index].data, data, w[index].size);
}

void LCD_Char_Wrapper_Copy_Data_Offset(
											LCD_Char_Wrapper* w,
											uint8_t* data, size_t index,
											size_t offset) {
	// XXX? kinda hacky
	memcpy(
			w[offset].data,
			data + (offset * w[offset].size),
			w[offset].size);
}

void LCD_Char_Data_Check_Bounds(LCD* lcd) {
	size_t len = lcd->char_data.len;

	if (len > lcd->char_data.cap) {
		lcd->char_data.cap += 5;

		lcd->char_data.chars = 
			(LCD_Char_Wrapper*)realloc(
					lcd->char_data.chars,
					lcd->char_data.cap *
					sizeof(LCD_Char_Wrapper));

		if (lcd->char_data.chars == NULL) { // TODO: error
			return;
		}
	}
}

size_t LCD_Char_Data_Create_Custom_Char(
		LCD* lcd,
		uint8_t* data) {
	LCD_Char_Wrapper_Copy_Data(
			lcd->char_data.chars,
			data, lcd->char_data.len);

	size_t len = lcd->char_data.len;

	lcd->char_data.len++;
	LCD_Char_Data_Check_Bounds(lcd);

	return len;
}

/* State */
void LCD_State_Set_Cursor(LCD* lcd, int x, int y) {
	if (x >= lcd->attr.cols ||
			y >= lcd->attr.rows ||
			x < 0 ||
			y < 0) return; // TODO: error

	int d = x + (y * lcd->attr.cols);
	LCD_DC* node = lcd->dc_head;

	for (; d > 0; d--) {
		node = node->next;
	}

	if (node != NULL) // TODO: error
		lcd->state.cursor = node;
}

void LCD_State_Cursor_Advance(LCD* lcd) {
	if (lcd->state.cursor->next == NULL)
		lcd->state.cursor = lcd->dc_head; // TODO: error?

	lcd->state.cursor = lcd->state.cursor->next;
}

/* Displayed_Character */
void LCD_Displayed_Character_Update_Single(LCD_DC* dc) {
	SDL_LockSurface(dc->surface_data);

	uint8_t* pixels =
		(uint8_t*)malloc(
				dc->surface_data->h *
				dc->surface_data->pitch);

	if (pixels == NULL) { // TODO: error
		return;
	}

	int x, y; x = 0; y = 0;
	int max_x = LCD_CHAR_W * 4; // 3 colors + alpha
	
	uint8_t last_bit = LCD_CHAR_W - 1;
	for (y = 0; y < LCD_CHAR_H; y++) {
		for (x = 0; x < max_x; x += 4) {
			size_t position =
				x + (y * dc->surface_data->pitch);

			pixels[position + 1] = 0x7F; // green

			if ((dc->char_wrapped->data[y] &
					( 1 << (last_bit - (x/4)) )) != 0) {
				pixels[position + 1] = 0xFF;
			}

			pixels[position    ] = 0;
			pixels[position + 2] = 0;
			pixels[position + 3] = 0xFF; // alpha
		}
	}

	memcpy(
			dc->surface_data->pixels,
			pixels,
			dc->surface_data->h *
			dc->surface_data->pitch);

	free(pixels);

	SDL_UnlockSurface(dc->surface_data);
}

void LCD_Displayed_Character_Update_All(LCD* lcd) {
	LCD_DC* node = lcd->dc_head;
	for (; node->next != NULL; node = node->next) {
		LCD_Displayed_Character_Update_Single(node);
	}
}

void LCD_Displayed_Character_Set_Data(
															LCD* lcd,
															size_t id) {
	if (id >= lcd->char_data.len)
		return; // TODO: error

	lcd->state.cursor->char_wrapped =
		&lcd->char_data.chars[id];

	LCD_Displayed_Character_Update_Single(
											lcd->state.cursor);
}

LCD_DC* LCD_Displayed_Character_Create_Single(
																	int x, int y,
																	LCD* lcd) {
	LCD_DC* dc =
		(LCD_DC*)malloc(
				sizeof(LCD_DC));
	
	// XXX: but it works!
	dc->rect =
		(SDL_Rect)
		{LCD_STARTPOS_X + (COLW*x*MULT) + (lcd->attr.gap*x),
		 LCD_STARTPOS_Y + (COLH*y*MULT) + (lcd->attr.gap*y),
		 COLW*MULT,
		 COLH*MULT};

	SDL_Surface* sfc = NULL;
	sfc = SDL_CreateRGBSurface(0,
												LCD_CHAR_W,
												LCD_CHAR_H,
												32, 0, 0, 0, 0);

	if (sfc == NULL) { // TODO: error
		return NULL;
	}

	dc->surface_data = sfc;
	dc->next = NULL;

	dc->char_wrapped = lcd->char_data.chars;

	return dc;
}

void LCD_Displayed_Character_Create_All(LCD* lcd) {
	int row = 0;
	int col = 0;

	lcd->dc_head =
		LCD_Displayed_Character_Create_Single(col, row, lcd);

	col = 0;

	LCD_DC* node = lcd->dc_head;

	while (row < lcd->attr.rows) {
		col++;
		if (col >= lcd->attr.cols) {
			col = 0;
			row++;
		}

		node->next =
			LCD_Displayed_Character_Create_Single(
					col, row, lcd);	

		// TODO: error
		if (node != NULL)
			node = node->next;
	}

	lcd->state.cursor = lcd->dc_head;
}

void LCD_Displayed_Character_Draw_Single(
															LCD_DC* dc,
															SDL_Surface* scr) {
	SDL_BlitScaled(dc->surface_data, NULL, scr, &dc->rect);
}

void LCD_Displayed_Character_Draw_All(
															LCD* lcd,
															SDL_Surface* scr) {
	LCD_DC* node = lcd->dc_head;
	for (; node->next != NULL; node = node->next) {
		LCD_Displayed_Character_Draw_Single(node, scr);
	}
}

/* Misc */
void LCD_Free(LCD* lcd) {

}

