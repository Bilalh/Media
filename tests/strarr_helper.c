#include "include/strarr_helper.h"

#define StrarrPrintTest(title,data)  PrintTest(title,data)
#define STRARR_TEST_FAIL(actual,exp) PRINT_FAIL; STRARR_SHOW_FAIL_DATA(actual,exp); return FAIL
#define STRARR_SHOW_FAIL_DATA(actual,exp) ;
	//printf("%s%-14s: '%s'\n", ERROR_SEP, "expected", exp );\
	//printf("%s%-14s: '%s'\n", ERROR_SEP, "act->str", actual->str);

bool strarr_test_start ( char *name, ArrayList *actualA, char **expected, int elen ){
	
	int error;
	int alen = actualA->index;
	char **actual = (char**) actualA->arr;
	
	if ( alen != elen ) goto fail;
	
	for(int i = 0; i < alen; ++i){
		if (strcmp(actual[i],expected[i]) != 0){
			error = i;
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
	
	eprintf("missmatch @ %i\n",error );
	
	for(int j = 0; j < min; ++j){
		char texp[strlen(expected[j])+2], tact[strlen(actual[j])+2];
		sprintf(texp, "'%s'",expected[j] ); sprintf(tact, "'%s'",actual[j] );
		eprintf("exp[%2d]:%-10s\tact[%2d]:%-10s\n",  j, texp, j, tact);
	}
	puts("");
	for(int i = min; i < max; ++i){
		eprintf("\t\t%s[%2d]: '%s'\n",err, i,eptr[i]  );
	}
	
	return false;
	
	PRINT_PASS;
	return PASS;
}

