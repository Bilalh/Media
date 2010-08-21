#ifndef ALLL_TESTS_HEAD
#define ALLL_TESTS_HEAD 

#include "string_test.h"
#include "time_test.h"
#include "all_tests.def"

typedef struct {
	char *name;
	void (*func)();
} Test;

#endif