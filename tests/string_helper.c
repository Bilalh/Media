#include <string.h>
#include "string_helper.h"

#define StringPrintTest(title,data)  \
	printf("%-*s '%*s'", LINE_REC_1, title, LINE_REC_2-2, data)
#define STRING_TEST_FAIL(S) PRINT_FAIL; STRING_SHOW_FAIL_DATA(S); return FAIL
#define STRING_SHOW_FAIL_DATA(S) \
		printf("%s%-14s: '%s'\n", ERROR_SEP, "act->str",    S->str    );\
		printf("%s%-14s: %i \n", ERROR_SEP, "act->index",  S->index  );\
		printf("%s%-14s: %i \n", ERROR_SEP, "act->length", S->length );
		

bool string_test_start (char *name, char *expected, String *actual ){
	StringPrintTest(name, actual->str);
	if (actual->length <0 || actual->index <0 ) {
		STRING_TEST_FAIL(actual);
	}
	
	int length = strlen(expected);
	if ( strncmp(expected, actual->str, length) != 0 ){
		STRING_TEST_FAIL(actual);
	}
	
	PRINT_PASS;
	return PASS;
}

