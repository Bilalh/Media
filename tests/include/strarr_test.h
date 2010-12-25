#ifndef STRARR_TEST_HEADER
#define STRARR_TEST_HEADER
#include "tests.h"
#include "strarr_helper.h"

// for Section numbering
static int STRARR_NUM_SECTION = 0; 

// Standard Macros 
#undef  Section
#define Section(title)          StartSection(title,STRARR_NUM_SECTION)
#define StrarrVar               MakeVar(STRARR)
#define StrarrEndSection        EndSection(STRARR)
#define StrarrPrintTestResults  PrintTestResults(STRARR)
#define StrarrMakeResult        MakeTestResult(STRARR)
#define StrarrResults           TestResults

// Setup that is done before the test is run
#define StrarrSetup

// function to run on test data
#define StrarrTest(name, tBLOCK)\
	{\
		bool test_result;\
		char *_name = name;\
		StrarrBefore\
		tBLOCK\
		StrarrAfter(name)\
		if (test_result){\
			STRARR_TEST_PASED++;\
		}else{\
			STRARR_TEST_FAILED++;\
		}\
		STRARR_TEST_TOTAL++;\
	}

TestResult strarr_test_main(int test_no);
#endif

