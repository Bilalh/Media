#include "include/strutil_helper.h"

#define StrutilPrintTest(title,data)  PrintTest(title,data)
#define STRUTIL_TEST_FAIL(actual,exp) PRINT_FAIL; STRUTIL_SHOW_FAIL_DATA(actual,exp); return FAIL
#define STRUTIL_SHOW_FAIL_DATA(actual,exp) \
	printf("%s%-14s: '%s'\n", ERROR_SEP, "expected", exp );\
	printf("%s%-14s: '%s'\n", ERROR_SEP, "actual", actual);

bool strutil_test_start ( char *name, char *actual, char *expected){

	if (actual <0 || actual <0 ) {
		StrutilPrintTest(name,"");
		STRUTIL_TEST_FAIL(actual,expected);
	}
	
	if ( strcmp(expected,actual) != 0 ){
		StrutilPrintTest(name,"");
		STRUTIL_TEST_FAIL(actual, expected);
	}
	
	StrutilPrintTest(name,  
		strlen(actual) <= LINE_REC_2 ? actual : "" 
	);
	
	PRINT_PASS;
	return PASS;
}

