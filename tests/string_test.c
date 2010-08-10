#include "string_test.h"
StringVar

#define StringBefore \
	char *expected;\
	String actual;

// test_result must be a true value for true and false otherwise
#define StringAfter(name) \
	test_result=string_test_start (name, expected, &actual);

TestResult string_test_main(int test_no) {
StringSetup

Sections {
Section("Basic Features"){
	StringTest("Empty String", {
		new_string(&actual,1);
		expected = "";
	})
	StringTest("Simple push", {
		new_string(&actual,1);
		string_push(&actual, "-fs");
		expected = " -fs";
	})
}StringEndSection

};

TestRun
StringPrintTestResults
StringMakeResult;
return StringResults;
}

#ifndef ALL_TESTS
MakeMain(string)
#endif
