#include <string.h>


#include "include/string_extra_helper.h"
#include "include/string_helper.h"

#define String_extraPrintTest(title,data)  \
	printf("%-*s '%-*s'", LINE_REC_1, title, LINE_REC_2-2, data)
#define STRING_EXTRA_TEST_FAIL(S,exp) PRINT_FAIL; STRING_EXTRA_SHOW_FAIL_DATA(S,exp); return FAIL
#define STRING_EXTRA_SHOW_FAIL_DATA(S,exp) \
		printf("%s%-14s: '%s'\n", ERROR_SEP, "expected",    exp    );\
		printf("%s%-14s: '%s'\n", ERROR_SEP, "act->str",    S->str    );\
		printf("%s%-14s: %i \n" , ERROR_SEP, "act->index",  S->index  );\
		printf("%s%-14s: %i \n" , ERROR_SEP, "act->length", S->length );
		

bool string_extra_test_start (char *name, String *actual, char *expected  ){
	if (actual->length <0 || actual->index <0 ) {
		String_extraPrintTest(name,"");
		STRING_EXTRA_TEST_FAIL(actual,expected);
	}
	
	int length = strlen(expected);
	if ( strncmp(expected, actual->str, length) != 0 ){
		String_extraPrintTest(name,"");
		STRING_EXTRA_TEST_FAIL(actual,expected);
	}
	
	
	String_extraPrintTest(name,  strlen(actual->str) < LINE_REC_2-2 ? actual->str : "" );
	PRINT_PASS;
	return PASS;
}

