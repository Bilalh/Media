#ifndef OPT_HELPER_HEADER
#define OPT_HELPER_HEADER

#include "opt_test.h"
#include <include/option_parser.h>
#include <include/string_util.h>
#include <include/string_buffer.h>

#define OptPrintTest(title,data)  PrintTest(title,data)

bool opt_test_start ( char *name,  MediaArgs *expected );

#endif
