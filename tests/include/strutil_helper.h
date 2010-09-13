#ifndef STRUTIL_HELPER_HEADER
#define STRUTIL_HELPER_HEADER

#include "strutil_test.h"
#include <include/string_util.h>

#define StrutilPrintTest(title,data)  PrintTest(title,data)

bool strutil_test_start ( char *name, char *actual, char *expected);
bool ep_num_test(char *name, char **actual, long exp_int, char *exp_str);
bool spilt_words_test (char *name, char **actual, int alen, char **exp, int elen);
#endif
