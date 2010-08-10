#include <string.h>
#include "string_helper.h"

#define StringPrintTest(title,data)  \
	printf("%-*s '%-*s'", LINE_REC_1, title, LINE_REC_2-2, data)
#define STRING_TEST_FAIL(S,exp) PRINT_FAIL; STRING_SHOW_FAIL_DATA(S,exp); return FAIL
#define STRING_SHOW_FAIL_DATA(S,exp) \
		printf("%s%-14s: '%s'\n", ERROR_SEP, "expected",    exp    );\
		printf("%s%-14s: '%s'\n", ERROR_SEP, "act->str",    S->str    );\
		printf("%s%-14s: %i \n" , ERROR_SEP, "act->index",  S->index  );\
		printf("%s%-14s: %i \n" , ERROR_SEP, "act->length", S->length );
		

bool string_test_start (char *name, char *expected, String *actual ){
	if (actual->length <0 || actual->index <0 ) {
		StringPrintTest(name,"");
		STRING_TEST_FAIL(actual,expected);
	}
	
	int length = strlen(expected);
	if ( strncmp(expected, actual->str, length) != 0 ){
		StringPrintTest(name,"");
		STRING_TEST_FAIL(actual,expected);
	}
	
	StringPrintTest(name,  length < LINE_REC_2-2 ? actual->str : "" );
	PRINT_PASS;
	return PASS;
}

