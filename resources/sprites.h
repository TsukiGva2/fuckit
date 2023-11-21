#ifndef __SPRITES_H_FUCKIT__
#define __SPRITES_H_FUCKIT__

#include "../LCD/LCD.h"

const LCD_Char player_sprite = {
  0b00000,
  0b01100,
  0b10010,
  0b11110,
  0b11110,
  0b10010,
  0b00000,
  0b00000
};

const LCD_Char water_spr = {
	0b00000,
	0b00000,
	0b00000,
	0b01101,
	0b11111,
	0b11111,
	0b11111,
	0b00000
};

const LCD_Char grass_spr = {
	0b00000,
	0b00000,
	0b01110,
	0b00000,
	0b01110,
	0b00000,
	0b01110,
	0b00000
};

const LCD_Char tree_spr = {
	0b00000,
	0b01110,
	0b11111,
	0b11111,
	0b00100,
	0b00100,
	0b00100,
	0b00000
};

#endif

