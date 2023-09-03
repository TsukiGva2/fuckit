#ifndef __HAN_H__
#define __HAN_H__

typedef enum HAN_ERROR_CODE {
	HAN_OK,
	HAN_ALLOC_ERR,
	HAN_OUT_OF_BOUNDS,
	HAN_CREATE_SURFACE_ERR
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

#endif

