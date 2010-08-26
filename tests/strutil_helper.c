#include "include/strutil_helper.h"

#define STRUTIL_TEST_FAIL(actual,exp) PRINT_FAIL; STRUTIL_SHOW_FAIL_DATA(actual,exp); return FAIL
#define STRUTIL_SHOW_FAIL_DATA(actual,expected) STRING_FAIL(actual,expected);

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

		EP_GET_NUMBER(actual,num);
		EP_GET_NAME(actual, str, name);
		
		if (num == exp_int && strcmp(str, exp_str) == 0 ){
			PrintTestl(name,num);
			test_result = true;
			PRINT_PASS;
		}else{
			PRINT_NAME_FAIL(name);
			eprintf("expected string: '%s'\n",  exp_str );
			eprintf("  actual string: '%s'\n",  str );
			eprintf("   num expected:  %li        actual: %li\n",   exp_int,num );
			
		}
	}else{
		PRINT_NAME_FAIL(name);
		eprintf("expected string: '%s'\n", exp_str );
		eprintf("expected number: %li\n", exp_int );
		eprintf("actual str/int : NULL\n");
		
	}
	
	return test_result;
}

bool spilt_words_test (char *name, char **actual, int alen, char **expected, int elen){
	bool test_result;
	
	if ( alen != elen ) goto fail;
	
	for(int i = 0; i < alen; ++i){
		if (strcmp(actual[i],expected[i]) != 0){
			goto fail;
		}
	}
	
	PRINT_NAME_PASS(name);
	return true;
	
	fail:
	PRINT_NAME_FAIL(name);
	int max, min;
	eprintf("num expected:  %i        actual: %i\n",   elen,alen );
	
	char *err, **eptr;
	if (alen > elen) {
		max  = alen;
		min  = elen;
		err = "act";
		eptr = actual;
	}else{
		min  = alen;
		max  = elen;
		err = "exp";
		eptr = expected;
	}
		
	for(int j = 0; j < min; ++j){
		eprintf("exp[%d]:'%s'\tact[%d]:'%s'\n",  j, expected[j], j, actual[j]);
	}
	puts("");
	for(int i = min; i < max; ++i){
		eprintf("\t\t%s[%d]: '%s'\n",err, i,eptr[i]  );
	}
	
	return false;
}
