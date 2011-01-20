#include "include/string_extra_test.h"
String_extraVar

// Setup that is done once before the tests are run
#define String_extraSetup 

// Setup that is done before each test
#define String_extraBefore \
	char *expected;\
	String actual;

// test_result must be a true value for true and false otherwise
#define String_extraAfter(name) \
	test_result = string_extra_test_start (name, &actual, expected );


TestResult string_extra_test_main(int test_no) {
String_extraSetup

Sections {
Section("Basic Features"){
	String_extraTest("Name", {
		new_string(&actual,1);
		expected = "";
	})
	String_extraTest("printf",{
		new_string(&actual, 10);
		string_sprintf(&actual, 0, "%s", "aaa");
		string_sprintf(&actual, 2, "%s", "aaa");
		expected = "a";
	})
}String_extraEndSection


Section("string_mprintf"){
	String_extraTest("basic",{
		new_string(&actual, 10);
		string_mprintf(&actual, "%s", "aaa");
		expected = "aaa";
	})
	String_extraTest("sql",{
		new_string(&actual, 1111);
		string_mprintf(&actual, 
		" UPDATE SeriesInfo"
		" SET Score   = %d"
		" WHERE Title = %Q; ",
		7,"Tales of Symphonia Tethe\'alla");
		expected = 
		" UPDATE SeriesInfo"
		" SET Score   = 7"
		" WHERE Title = 'Tales of Symphonia Tethe\'\'alla'; ";
	})
}String_extraEndSection

};

TestRun
String_extraPrintTestResults
String_extraMakeResult;
return String_extraResults;
}

