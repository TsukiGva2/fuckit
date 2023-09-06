#ifndef __FUCKIT_H__
#define __FUCKIT_H__

#include "fuckit_config.h"

#include "debug/debug.h"
#define malloc(x) my_malloc(x, __FILE__, __LINE__, __FUNCTION__)
#define free(x) my_free(x, __FILE__, __LINE__, __FUNCTION__)
#define LOG(s) my_log(s, __FILE__, __LINE__, __FUNCTION__)

#define HAN_RETURN_OK (HAN_Return(HAN_OK, NULL, __FILE__, __LINE__, __FUNCTION__))
#define HAN_RETURN(c, t) (HAN_Return(c, t, __FILE__, __LINE__, __FUNCTION__))

#define DEFER(f, l) do{\
				HAN_Status* h = f;\
				if (h->code != HAN_OK) {\
					LOG(h);\
					goto l;\
				}\
			}while(0)

#define FAIL(f) do{\
				HAN_Status* h = f;\
				if (h->code != HAN_OK) {\
					return h;\
				}\
			}while(0)

#endif

