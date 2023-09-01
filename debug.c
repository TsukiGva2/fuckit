#include <stdlib.h>
#include <stdio.h>
#include "debug.h"

void* my_malloc(size_t size, const char* file, int line, const char* func) {
	void* p = malloc(size);

	printf("allocated: %s, %i, %s, %p[%li]\n", file, line, func, p, size);

	return p;
}

void my_free(void* ptr, const char* file, int line, const char* func) {
	printf("freed: %s, %i, %s, %p\n", file, line, func, ptr);

	free(ptr);
}

