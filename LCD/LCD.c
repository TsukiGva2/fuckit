#include "LCD.h"
#include "../fuckit.h"
#include "LCD_Font.h"

/* Output */
HAN_Status* LCD_Out_Clear(LCD* lcd) {
	HAN_Status* h;

	h = LCD_State_Set_Cursor(lcd, 0, 0);
	if (h->code != HAN_OK) {
		return h;
	}

	for (size_t i = 0; i < lcd->attr.size; i++) {
		h = LCD_Displayed_Character_Set_Data(lcd, 0);

		if (h->code != HAN_OK) {
			return h;
		}

		LCD_State_Cursor_Advance(lcd);
	}

	return HAN_RETURN_OK;
}

HAN_Status* LCD_Out_Text(LCD* lcd, char* s) {
	HAN_Status* h;

	for (char* c = s; *c != '\0'; c++) {
		if (*c >= 65 && *c <= 90) {
			h = LCD_Displayed_Character_Set_Data(lcd, (*c)-64);

			if (h->code != HAN_OK) {
				return h;
			}
		}

		LCD_State_Cursor_Advance(lcd);
	}

	return HAN_RETURN_OK;
}

HAN_Status* LCD_Out_Custom_Char(LCD* lcd, size_t id) {
	HAN_Status* h;
	h = LCD_Displayed_Character_Set_Data(lcd, id);

	LCD_State_Cursor_Advance(lcd);

	return h;
}

/* Char Data */
HAN_Status* LCD_Char_Data_Initialize(LCD* lcd) {
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

	if (lcd->char_data.chars == NULL) {
		return HAN_RETURN(HAN_ALLOC_ERR, "calloc on lcd->char_data.chars returned NULL");
	}

	LCD_Char_Wrapper* w = lcd->char_data.chars;
	for (size_t i = 0; i < lcd->char_data.cap; i++) {
		/* WARNING: IF YOU CHANGE THIS, YOU GOT
		 * TO CHANGE A LOT OF STUFF */
		w[i].size  = sizeof(LCD_Char);

		w[i].index = i;

		// old LOAD_FONT function
		// TODO: maybe make this a
		// separate function?
		if (i < lcd->char_data.len)
			LCD_Char_Wrapper_Copy_Data_From_Array_Offset(
										&lcd->char_data, LCD_FONT, i, i,
										sizeof(LCD_FONT));
	}

	return HAN_RETURN_OK;
}

HAN_Status* LCD_Char_Wrapper_Copy_Data(
											LCD_Char_Data* cd,
											uint8_t* data,
											size_t index) {
	if (index > cd->len) {
		return HAN_RETURN(HAN_OUT_OF_BOUNDS, "index is greater than cd->len");
	}

	// XXX? kinda hacky
	memcpy(cd->chars[index].data, data, cd->chars[index].size);

	return HAN_RETURN_OK;
}

					/* this basically means: load from font index */
HAN_Status* LCD_Char_Wrapper_Copy_Data_From_Array_Offset(
											LCD_Char_Data* cd,
											/* AT LEAST sizeof(LCD_Char) AND NOT NULL */
											const uint8_t data[static sizeof(LCD_Char)],
											size_t index,
											size_t offset,
											size_t size) {

	size_t off = offset * sizeof(LCD_Char); // XXX: assumes cd->w[index].size == sizeof(LCD_Char)
	
	if (off > size) {
		return HAN_RETURN(HAN_OUT_OF_BOUNDS, "specified offset is bigger than array");
	}

	// XXX? kinda hacky
	// big oopsie there
	// >	(was using w[offset] instead
	// >   of w[index], that could fuck
	// >	 up pretty bad)
	return LCD_Char_Wrapper_Copy_Data(
			cd,
			data + off,
			index
		);
}

HAN_Status* LCD_Char_Data_Check_Bounds(LCD* lcd) {
	size_t len = lcd->char_data.len;

	if (len > lcd->char_data.cap) {
		lcd->char_data.cap += 5;

		lcd->char_data.chars = 
			(LCD_Char_Wrapper*)realloc(
					lcd->char_data.chars,
					lcd->char_data.cap *
					sizeof(LCD_Char_Wrapper));

		if (lcd->char_data.chars == NULL) {
			return HAN_RETURN(HAN_ALLOC_ERR, "realloc lcd->char_data.chars returned NULL");
		}
	}

	return HAN_RETURN_OK;
}

HAN_Status* LCD_Char_Data_Create_Custom_Char(
		size_t* id,
		LCD* lcd,
		uint8_t* data) {
	// NOTE: no checking because
	// lcd->char_data.len is checked
	// for out of bounds after increment
	// also len is by definition smaller
	// than cap
	LCD_Char_Wrapper_Copy_Data(
			&lcd->char_data,
			data, lcd->char_data.len);

	*id = lcd->char_data.len;

	lcd->char_data.len++;
	
	return LCD_Char_Data_Check_Bounds(lcd);
}

/* State */
HAN_Status* LCD_State_Set_Cursor(LCD* lcd, int x, int y) {
	if (x >= lcd->attr.cols ||
			y >= lcd->attr.rows ||
			x < 0 ||
			y < 0) return HAN_RETURN(HAN_OUT_OF_BOUNDS, "invalid (x,y) value passed");

	int d = x + (y * lcd->attr.cols);
	LCD_DC* node = lcd->dc_head;

	for (; d > 0; d--) {
		node = node->next;
	}

	if (node == NULL) {
		return HAN_RETURN(HAN_ALLOC_ERR, "why the hell is node == NULL");
	}

	lcd->state.cursor = node;
	return HAN_RETURN_OK;
}

void LCD_State_Cursor_Advance(LCD* lcd) {
	if (lcd->state.cursor->next == NULL)
		lcd->state.cursor = lcd->dc_head; // this is now
																			// default behaviour

	lcd->state.cursor = lcd->state.cursor->next;
}

/* Displayed_Character */
HAN_Status* LCD_Displayed_Character_Update_Single(LCD_DC* dc) {
	SDL_LockSurface(dc->surface_data);

	/*uint8_t* pixels =
		(uint8_t*)malloc(
				dc->surface_data->h *
				dc->surface_data->pitch);*/
	uint8_t* pixels = dc->surface_data->pixels;

	/*if (pixels == NULL) {
		return HAN_RETURN(HAN_ALLOC_ERR, "pixels == NULL");
	}*/

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

	/*memcpy(
			dc->surface_data->pixels,
			pixels,
			dc->surface_data->h *
			dc->surface_data->pitch);*/

	//free(pixels);

	SDL_UnlockSurface(dc->surface_data);

	return HAN_RETURN_OK;
}

HAN_Status* LCD_Displayed_Character_Update_All(LCD* lcd) {
	LCD_DC* node = lcd->dc_head;
	HAN_Status* h;

	for (; node->next != NULL; node = node->next) {
		h = LCD_Displayed_Character_Update_Single(node);

		if (h->code != HAN_OK) {
			return h;
		}
	}

	return HAN_RETURN_OK;
}

HAN_Status* LCD_Displayed_Character_Set_Data(
															LCD* lcd,
															size_t id) {
	if (id >= lcd->char_data.len)
		return HAN_RETURN(HAN_OUT_OF_BOUNDS, "id is greater than list length");

	lcd->state.cursor->char_wrapped =
		&lcd->char_data.chars[id];

	return LCD_Displayed_Character_Update_Single(
														lcd->state.cursor);
}

HAN_Status* LCD_Displayed_Character_Create_Single(
																	LCD_DC** ret,
																	int x, int y,
																	LCD* lcd) {
	LCD_DC* dc =
		(LCD_DC*)malloc(
				sizeof(LCD_DC));

	*ret = dc;

	if (dc == NULL) {
		return HAN_RETURN(HAN_ALLOC_ERR, "couldnt allocate single Displayed_Character");
	}
	
	dc->surface_data = NULL;
	dc->next = NULL;
	dc->char_wrapped = lcd->char_data.chars;
	
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

	if (sfc == NULL) {
		return HAN_RETURN(HAN_CREATE_SURFACE_ERR, "couldnt create RGBSurface 'sfc'");
	}

	dc->surface_data = sfc;
	dc->next = NULL;

	return HAN_RETURN_OK;
}

HAN_Status* LCD_Displayed_Character_Create_All(LCD* lcd) {
	int row = 0;
	int col = 0;

	HAN_Status* h;
	h = LCD_Displayed_Character_Create_Single(
			&lcd->dc_head, col, row, lcd);

	if (h->code != HAN_OK) {
		return h;
	}

	col = 0;

	LCD_DC* node = lcd->dc_head;

	while (row < lcd->attr.rows) {
		col++;
		if (col >= lcd->attr.cols) {
			col = 0;
			row++;
		}

		h = LCD_Displayed_Character_Create_Single(
					&node->next, col, row, lcd);

		if (h->code != HAN_OK) {
			return h;
		}

		node = node->next;
	}

	lcd->state.cursor = lcd->dc_head;

	return HAN_RETURN_OK;
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
	LCD_Free_Char_Data(&lcd->char_data);
	LCD_Free_Displayed_Characters(lcd->dc_head);
	lcd->dc_head = NULL;

	// no need to free lcd,
	// it's in the stack
	// dumbass.
	// free(lcd)
}

void LCD_Free_Char_Data(LCD_Char_Data* cd) {
	if (cd->chars != NULL)
		free(cd->chars);

	cd->chars = NULL;
}

void LCD_Free_Displayed_Characters(LCD_DC* head) {
	if (head == NULL) return;

	LCD_DC* node = head;
	LCD_DC* next = head->next;

	for (; next != NULL; node = next) {
		next = node->next;

		if (node->surface_data != NULL)
			SDL_FreeSurface(node->surface_data);

		free(node);
	}
}

