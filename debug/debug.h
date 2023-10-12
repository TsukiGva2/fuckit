#ifndef __DEBUG_H__GAME__
#define __DEBUG_H__GAME__

#include "stdlib.h"
#include "../handler/han.h"

void my_log(HAN_Status* s, const char* file, int line, const char* func);

void* my_malloc(size_t size, const char* file, int line, const char* func);
void my_free(void* ptr, const char* file, int line, const char* func);

#endif
