#ifndef __LCD_H__
#define __LCD_H__

#include <SDL2/SDL.h>
#include "../debug/debug.h"
#include "../fuckit.h"

#define LCD_CHAR_W 5
#define LCD_CHAR_H 8

// Char_Data
typedef uint8_t LCD_Char[LCD_CHAR_H];
typedef struct LCD_Char_Wrapper {
	size_t size;
	size_t index;

	LCD_Char data;
} LCD_Char_Wrapper;

typedef struct LCD_Char_Data {
	size_t len;
	size_t cap;

	LCD_Char_Wrapper* chars;
} LCD_Char_Data;

// LCD_Displayed_Character
typedef struct LCD_DC {
	SDL_Rect rect;
	SDL_Surface* surface_data;

	LCD_Char_Wrapper* char_wrapped;

	struct LCD_DC* next;
} LCD_DC;

// TODO: keep me updated!
// LCD_State
typedef struct LCD_State {
	LCD_DC* cursor;
} LCD_State;

// LCD
typedef struct LCD_Attr {
	uint8_t rows;
	uint8_t cols;
	uint8_t size;
	uint8_t gap;
} LCD_Attr;

typedef struct LCD {
	LCD_Char_Data char_data;
	LCD_DC* dc_head;
	LCD_Attr attr;
	LCD_State state;
} LCD;

// FUNCTIONS:

// Output:
HAN_Status* LCD_Out_Clear(LCD* lcd);
HAN_Status* LCD_Out_Text(LCD* lcd, char* s);
HAN_Status* LCD_Out_Custom_Char(LCD* lcd, size_t id);

// Char Data
HAN_Status* LCD_Char_Data_Initialize(LCD* lcd);
HAN_Status* LCD_Char_Data_Create_Custom_Char(
																		size_t* id,
																		LCD* lcd,
																		uint8_t* data);
// TODO: dead function lol
/*
HAN_Status* LCD_Char_Data_Create_Custom_Chars(
																		size_t* id,
																		LCD* lcd,
																		uint8_t* data,
																		size_t count);
*/

HAN_Status* LCD_Char_Data_Check_Bounds(LCD* lcd);

HAN_Status* LCD_Char_Wrapper_Copy_Data(
												LCD_Char_Data* cd,
												uint8_t* data,
												size_t index);

HAN_Status* LCD_Char_Wrapper_Copy_Data_From_Array_Offset(
												LCD_Char_Data* cd,
												uint8_t* data,
												size_t index,
												size_t offset,
												size_t size);

// State functions:
HAN_Status* LCD_State_Set_Cursor(LCD* lcd, int x, int y);
void        LCD_State_Cursor_Advance(LCD* lcd);

// Displayed_Character functions:
HAN_Status* LCD_Displayed_Character_Update_Single(LCD_DC* dc);
HAN_Status* LCD_Displayed_Character_Update_All(LCD* lcd);

HAN_Status* LCD_Displayed_Character_Set_Data(
																		LCD* lcd,
																		size_t id);


HAN_Status* LCD_Displayed_Character_Create_Single(
																			LCD_DC** ret,
																			int x, int y,
																			LCD* lcd);

HAN_Status* LCD_Displayed_Character_Create_All(LCD* lcd);


void        LCD_Displayed_Character_Draw_Single(LCD_DC* dc, SDL_Surface* scr);
void        LCD_Displayed_Character_Draw_All(LCD* lcd, SDL_Surface* scr);

// Misc
void        LCD_Free(LCD* lcd);
void        LCD_Free_Char_Data(LCD_Char_Data* cd);
void        LCD_Free_Displayed_Characters(LCD_DC* dc);

#endif

