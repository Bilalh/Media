//FIXME write test

#include "include/strarr_test.h"

StrarrVar

// Setup that is done once before the tests are run
#define StrarrSetup 

// Setup that is done before each test
#define StrarrBefore \
	StringArray *actual;\
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
		actual   = string_array_new(2);
		string_array_add(actual, "a");
		string_array_add(actual, "b");
		
	})
	StrarrTest("a b c",{
		actual   = string_array_new(2);
		string_array_add(actual, "a");
		string_array_add(actual, "b");
		string_array_add(actual, "c");
	})
	StrarrTest("das dasd sajfdsiof jdsiof dsfoijds fiodsj fods fjdsoifj dsoig ffdsg fd",{
		actual   = string_array_new(3);
		string_array_add(actual, "das");
		string_array_add(actual, "dasd");
		string_array_add(actual, "sajfdsiof");
		string_array_add(actual, "jdsiof");
		string_array_add(actual, "dsfoijds");
		string_array_add(actual, "fiodsj");
		string_array_add(actual, "fods");
		string_array_add(actual, "fjdsoifj");
		string_array_add(actual, "dsoig");
		string_array_add(actual, "ffdsg");
		string_array_add(actual, "fd");
	})
}StrarrEndSection

};

TestRun
StrarrPrintTestResults
StrarrMakeResult;
return StrarrResults;
}

