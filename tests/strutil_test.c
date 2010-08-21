#include "include/strutil_test.h"
StrutilVar

// Setup that is done once before the tests are run
#define StrutilSetup 

// Setup that is done before each test
#define StrutilBefore \
	char* actual;
	
// test_result must be a true value for true and false otherwise
#define StrutilAfter(name) \
	test_result = strutil_test_start (name, actual);\

	// free(actual);

TestResult strutil_test_main(int test_no) {
StrutilSetup

Sections {
Section("Basic Features"){
	StrutilTest("Name", {
		actual = "Nxame";
	})
}StrutilEndSection

};

TestRun
StrutilPrintTestResults
StrutilMakeResult;
return StrutilResults;
}

