#ifndef STRING_TEST_HEADER
#define STRING_TEST_HEADER
#include "tests.h"
#include "string_helper.h"

// for Section numbering
static int STRING_NUM_SECTION = 0; 

// Standard Macros
#undef  Section
#define Section(title)          StartSection(title,STRING_NUM_SECTION) 
#define StringVar               MakeVar(STRING)
#define StringEndSection        EndSection(STRING)
#define StringPrintTestResults  PrintTestResults(STRING)
#define StringMakeResult        MakeTestResult(STRING)
#define StringResults           TestResults

// funtions to run on test data
#define StringTest(name, tBLOCK)\
	{\
		bool test_result;\
		StringBefore\
		tBLOCK\
		StringAfter(name)\
		if (test_result){\
			STRING_TEST_PASED++;\
		}else{\
			STRING_TEST_FAILED++;\
		}\
		STRING_TEST_TOTAL++;\
	}

TestResult string_test_main(int test_no);
#endif
