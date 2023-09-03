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

#include "debug/debug.h"
#define malloc(x) my_malloc(x, __FILE__, __LINE__, __FUNCTION__)
#define free(x) my_free(x, __FILE__, __LINE__, __FUNCTION__)
#define LOG(s) my_log(s, __FILE__, __LINE__, __FUNCTION__)

#define HAN_RETURN_OK (HAN_Return(HAN_OK, NULL, __FILE__, __LINE__, __FUNCTION__))
#define HAN_RETURN(c, t) (HAN_Return(c, t, __FILE__, __LINE__, __FUNCTION__))

#define CHECK(f, l) do{\
				HAN_Status* h = f;\
				if (h->code != HAN_OK) {\
					LOG(h);\
					goto l;\
				}\
			}while(0)

#endif

