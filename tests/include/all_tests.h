#ifndef ALLL_TESTS_HEAD
#define ALLL_TESTS_HEAD 

#include "string_test.h"
#include "time_test.h"
#include "opt_test.h"
#include "strutil_test.h"
#include "strarr_test.h"
#include "string_extra_test.h"

#include "all_tests.def"

typedef struct {
	char *name;
	void (*func)();
} Test;

// opt_test broken in fork mallocing?


#endif