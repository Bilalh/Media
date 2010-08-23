#include "include/strutil_helper.h"

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

bool ep_num_test(char *name, char **actual, long exp_int, char *exp_str){
	
	bool test_result = false;
	if (actual[0] != NULL) {
		long num = strtol(actual[0] + 1, NULL, 10);
		if (num == 0 ) num++;

		int index = actual[1] != NULL ? 1 : 0;
		
		char str[actual[index] - name + 1]; // 1 for \0
		
		strncpy(str, name, actual[index] - name);
		str[actual[index] - name] = '\0';
		
		if (num == exp_int && strcmp(str, exp_str) == 0 ){
			PrintTestl(name,num);
			test_result = true;
			PRINT_PASS;
		}else{
			PRINT_NAME_FAIL(name);
			eprintf("expected string: '%s'\n",  exp_str );
			eprintf("expected number:  %li\n",   exp_int );
			eprintf("  actual string: '%s'\n",  str );
			eprintf("  actual number:  %li\n",   num );
			
		}
	}else{
		PRINT_NAME_FAIL(name);
		eprintf("expected string: '%s'\n", exp_str );
		eprintf("expected number: %li\n", exp_int );
		eprintf("actual str/int : NULL\n");
		
	}
	
	return test_result;
}