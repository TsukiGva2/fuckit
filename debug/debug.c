#include <stdlib.h>
#include <stdio.h>

#include "../debug/debug.h"

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

void* my_malloc(size_t size, const char* file, int line, const char* func) {
	void* p = malloc(size);

	printf("allocated: %s, %i, %s, %p[%li]\n", file, line, func, p, size);

	return p;
}

void my_free(void* ptr, const char* file, int line, const char* func) {
	printf("freed: %s, %i, %s, %p\n", file, line, func, ptr);

	free(ptr);
}

