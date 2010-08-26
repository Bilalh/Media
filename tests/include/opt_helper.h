#ifndef OPT_HELPER_HEADER
#define OPT_HELPER_HEADER

#include "opt_test.h"
#include "option_parser.h"
#include "string_util.h"

#define OptPrintTest(title,data)  PrintTest(title,data)

bool opt_test_start ( char *name, MediaArgs *actual, MediaArgs *expected );

#endif
