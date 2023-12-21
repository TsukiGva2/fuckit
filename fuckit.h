#ifndef __FUCKIT_H__
#define __FUCKIT_H__

#include "fuckit_config.h"
#include "handler/han.h"

#define malloc(x) my_malloc(x, __FILE__, __LINE__, __FUNCTION__)
#define free(x) my_free(x, __FILE__, __LINE__, __FUNCTION__)

#endif

