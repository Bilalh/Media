#include "include/opt_test.h"
OptVar

// Setup that is done once before the tests are run
#define OptSetup 

// Setup that is done before each test
#define OptBefore \
	MediaArgs *expected = new_media_args();\
	MediaArgs *actual   = new_media_args();

// test_result must be a true value for true and false otherwise
#define OptAfter(name) \
	test_result = opt_test_start (name, expected, &actual);\
	free(actual); \
	free(expected);

TestResult opt_test_main(int test_no) {
OptSetup	
Sections {
Section("Basic Features"){
	OptTest("-fs -as", {
		
	})
}OptEndSection

};

TestRun
OptPrintTestResults
OptMakeResult;
return OptResults;
}
