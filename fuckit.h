#ifndef __FUCKIT_H__
#define __FUCKIT_H__

#define COLS 16
#define ROWS 2

#define MULT 2

#define WIN_W 331*MULT
#define WIN_H 149*MULT

#define COLW 15
#define COLH 24

#define LCD_STARTPOS_X 76
#define LCD_STARTPOS_Y 100

#include "debug.h"
#define malloc(x) my_malloc(x, __FILE__, __LINE__, __FUNCTION__)
#define free(x) my_free(x, __FILE__, __LINE__, __FUNCTION__)

#endif

