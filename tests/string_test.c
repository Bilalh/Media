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
Section("Section 1"){
	StringTest("Empty String", {
		new_string(&actual,1);
		expected = "";
	})
	StringTest("Empty String2", {
		new_string(&actual,1);
		expected = "s";
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
