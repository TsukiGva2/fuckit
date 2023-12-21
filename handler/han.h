#ifndef __HAN_H__
#define __HAN_H__

typedef enum HAN_ERROR_CODE {
	HAN_OK,
	HAN_ALLOC_ERR,
	HAN_OUT_OF_BOUNDS,
	HAN_CREATE_SURFACE_ERR,
	HAN_CREATE_WINDOW_ERR,
	HAN_LOAD_IMG_ERR,
  HAN_INTERACT_ERR
} HAN_ERROR_CODE;

typedef struct HAN_Details {
	const char* details;
	const char* file;
	const char* func;
	int         line;
} HAN_Details;

typedef struct HAN_Status {
	HAN_ERROR_CODE code;
	const char* message;
	HAN_Details details;
} HAN_Status;

// Functions
void        HAN_Create(HAN_Status* handler);

HAN_Status* _HAN_Get_Self(HAN_Status* handler);
HAN_Status* HAN_Get_Self(void);

HAN_Status* HAN_Return(
		HAN_ERROR_CODE code,
		const char* details,
		const char* file,
		int line,
		const char* func);

#include "../debug/debug.h"

void my_log(HAN_Status* s, const char* file, int line, const char* func);
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

