#include "string_test.h"
StringVar

// Setup that is done onces before the tests are run
#define StringSetup 

// Setup that is done before each test
#define StringBefore \
	char *expected;\
	String actual;

// test_result must be a true value for true and false otherwise
#define StringAfter(name) \
	test_result = string_test_start (name, expected, &actual);

TestResult string_test_main(int test_no) {
StringSetup

Sections {
Section("Basic Features"){
	StringTest("Empty String", {
		new_string(&actual,1);
		expected = "";
	})
	StringTest("Simple Add", {
		new_string(&actual,1);
		string_add(&actual, "-fs",false);
		expected = "-fs";
	})
	StringTest("Simple Push", {
		new_string(&actual,5);
		string_push(&actual, "dasdasdas");
		expected = " dasdasdas";
	})
	StringTest("Simple append", {
		new_string(&actual,3);
		string_append(&actual, "zazaz azaza");
		expected = "zazaz azaza";
	})
}StringEndSection

Section("Simple Multiple adds/pushes"){
	StringTest("Add multiple", {
		new_string(&actual,-1);
		string_add(&actual, "-fs",false);
		string_add(&actual, "--fast",true);
		expected = "-fs --fast";
	})
	StringTest("Add multiple", {
		new_string(&actual,13);
		string_add(&actual, "aab",true);
		string_add(&actual, "bbc",true);
		string_add(&actual, "ccd",true);
		expected = " aab bbc ccd";
	})
	StringTest("Add multiple push append", {
		new_string(&actual,7);
		string_append(&actual, "dasxsafasf safsafa");
		string_push(&actual, "fdsfdsfdsfdsfsd");
		expected = "dasxsafasf safsafa fdsfdsfdsfdsfsd";
	})
}StringEndSection

Section("Multiple adds/pushes _m"){
	StringTest("Add multiple add_m", {
		new_string(&actual,13);
		string_add_m(&actual, true, 3, "aab", "bbc", "ccd");
		expected = " aab bbc ccd";
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
