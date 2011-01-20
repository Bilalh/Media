#ifndef STRING_EXTRA_TEST_HEADER
#define STRING_EXTRA_TEST_HEADER
#include "tests.h"
#include "string_extra_helper.h"

// for Section numbering
static int STRING_EXTRA_NUM_SECTION = 0; 

// Standard Macros 
#undef  Section
#define Section(title)                StartSection(title,STRING_EXTRA_NUM_SECTION)
#define String_extraVar               MakeVar(STRING_EXTRA)
#define String_extraEndSection        EndSection(STRING_EXTRA)
#define String_extraPrintTestResults  PrintTestResults(STRING_EXTRA)
#define String_extraMakeResult        MakeTestResult(STRING_EXTRA)
#define String_extraResults           TestResults

// Setup that is done before the test is run
#define String_extraSetup

// function to run on test data
#define String_extraTest(name, tBLOCK)\
	{\
		bool test_result;\
		char *_name = name;\
		String_extraBefore\
		tBLOCK\
		String_extraAfter(name)\
		if (test_result){\
			STRING_EXTRA_TEST_PASED++;\
		}else{\
			STRING_EXTRA_TEST_FAILED++;\
		}\
		STRING_EXTRA_TEST_TOTAL++;\
	}

TestResult string_extra_test_main(int test_no);
#endif

