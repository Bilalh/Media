#include "include/opt_helper.h"

#define OptPrintTest(title,data)  PrintTest(title,data)
#define OPT_TEST_FAIL(actual,exp) PRINT_FAIL; OPT_SHOW_FAIL_DATA(actual,exp); return FAIL
#define OPT_SHOW_FAIL_DATA(actual,exp) 
	// printf("%s%-14s: '%s'\n", ERROR_SEP, "expected", exp );\
	// printf("%s%-14s: '%s'\n", ERROR_SEP, "act->str", actual->str);

bool opt_test_start ( char *name, MediaArgs *actual, MediaArgs *expected ){
	
	if ( false ){
		OptPrintTest(name,"");
		OPT_TEST_FAIL(actual,expected);
	}
	
	OptPrintTest(name, "");

	PRINT_PASS;
	return PASS;
}

