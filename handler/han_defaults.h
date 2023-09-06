#ifndef __HAN_DEFAULTS__
#define __HAN_DEFAULTS__

#include "../fuckit_config.h"

const char* HAN_DEFAULT_MESSAGES[] = {
	"ALL GOOD",
	"Couldn't allocate enough memory!",
	"Out of bounds! requested index may not exist",
	"Couldn't create SDL Surface (try SDL_GetError?)",
	"Couldn't create SDL Window (try SDL_GetError?)",
	"Couldn't load " LCD_IMG_FILE " (try SDL_GetError?)"
};
#endif

