#ifndef STRUTIL_HELPER_HEADER
#define STRUTIL_HELPER_HEADER

#include "strutil_test.h"
#include "string_util.h"

#define StrutilPrintTest(title,data)  PrintTest(title,data)
bool strutil_test_start ( char *name, char *actual, char *expected);

#endif
