
#include "include/strarr_test.h"

StrarrVar

// Setup that is done once before the tests are run
#define StrarrSetup 

// Setup that is done before each test
#define StrarrBefore \
	ArrayList *actual;\
	int  length;\
	char **expected = spilt_string_m(_name, &length);\
	
// test_result must be a true value for true and false otherwise
#define StrarrAfter(name) \
	test_result = strarr_test_start(name, actual, expected, length);\
	for(int i = 0; i < length; ++i){\
		free(expected[i]);\
	}\
	
TestResult strarr_test_main(int test_no) {
StrarrSetup

Sections {
Section("Basic Features"){
	StrarrTest("a b", {
		actual   = new_arraylist(2);
		arraylist_string_add(actual, "a");
		arraylist_string_add(actual, "b");
		
	})
	StrarrTest("a b c",{
		actual   = new_arraylist(2);
		arraylist_string_add(actual, "a");
		arraylist_string_add(actual, "b");
		arraylist_string_add(actual, "c");
	})
	StrarrTest("das dasd sajfdsiof jdsiof dsfoijds fiodsj fods fjdsoifj dsoig ffdsg fd",{
		actual   = new_arraylist(3);
		arraylist_string_add(actual, "das");
		arraylist_string_add(actual, "dasd");
		arraylist_string_add(actual, "sajfdsiof");
		arraylist_string_add(actual, "jdsiof");
		arraylist_string_add(actual, "dsfoijds");
		arraylist_string_add(actual, "fiodsj");
		arraylist_string_add(actual, "fods");
		arraylist_string_add(actual, "fjdsoifj");
		arraylist_string_add(actual, "dsoig");
		arraylist_string_add(actual, "ffdsg");
		arraylist_string_add(actual, "fd");
	})
}StrarrEndSection

};

TestRun
StrarrPrintTestResults
StrarrMakeResult;
return StrarrResults;
}

