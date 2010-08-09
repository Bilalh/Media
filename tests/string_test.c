#include "string_test.h"
StringVar
TestResult string_test_main(int test_no) {
StringSetup

Sections {
Section("Section 1"){
	StringTest("Abc", {
		
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
