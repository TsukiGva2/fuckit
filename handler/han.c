#include "han.h"
#include "han_defaults.h"
#include <stdlib.h>

#include <stdio.h>

void HAN_Create(HAN_Status* handler) {
	handler->code = HAN_OK;
	handler->message =
		HAN_DEFAULT_MESSAGES[handler->code];
	handler->details.details = handler->message;
	handler->details.func = __FUNCTION__;
	handler->details.file = __FILE__;
	handler->details.line = __LINE__;

	_HAN_Get_Self(handler);
}

// XXX XXX XXX XXX XXX ALL THE FUCKING WAY
// but i'm lazy and yeah it's the best i could
// think of
HAN_Status* _HAN_Get_Self(HAN_Status* handler) {
	static HAN_Status* self = NULL;
	if (handler != NULL) {
		self = handler;
	}

	return self;
}

HAN_Status* HAN_Get_Self(void) {
	return _HAN_Get_Self(NULL);
}

HAN_Status* HAN_Return(
		HAN_ERROR_CODE code,
		const char* details,
		const char* file,
		int line,
		const char* func) {
	HAN_Status* handler = HAN_Get_Self();

	handler->code = code;
	handler->message = 
		HAN_DEFAULT_MESSAGES[handler->code];

	if (details != NULL) {
		handler->details.details = details;
		handler->details.line    = line;
		handler->details.func    = func;
		handler->details.file    = file;
	}

	return handler;
}

void my_log(HAN_Status* s, const char* file, int line, const char* func) {
	printf("in file: %s:%i, function: %s\n"
"STATUS: %d\n",
	file, line, func, s->code);

	printf("Description: %s\n\n"
"Additional details: %s\n",
	s->message, s->details.details);

	printf("in file: %s:%i, %s\n",
	s->details.file, s->details.line,
	s->details.func);
}

