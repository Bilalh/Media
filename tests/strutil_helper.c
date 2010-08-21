#include "include/strutil_helper.h"

#define StrutilPrintTest(title,data)  PrintTest(title,data)
#define STRUTIL_TEST_FAIL(actual,exp) PRINT_FAIL; STRUTIL_SHOW_FAIL_DATA(actual,exp); return FAIL
#define STRUTIL_SHOW_FAIL_DATA(actual,exp) \
	printf("%s%-14s: '%s'\n", ERROR_SEP, "expected", exp );\
	printf("%s%-14s: '%s'\n", ERROR_SEP, "actual", actual);

bool strutil_test_start ( char *name, char *actual){
	
	if ( strcmp(name,actual) != 0 ){
		StrutilPrintTest(name,"");
		STRUTIL_TEST_FAIL(actual,name);
	}
	
	StrutilPrintTest(name,  
		strlen(actual) < LINE_REC_2 ? actual : "" 
	);

	PRINT_PASS;
	return PASS;
}

