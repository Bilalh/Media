#ifndef OPT_TEST_HEADER
#define OPT_TEST_HEADER
#include "tests.h"
#include "opt_helper.h"

// for Section numbering
static int OPT_NUM_SECTION = 0; 

// Standard Macros 
#undef  Section
#define Section(title)       StartSection(title,OPT_NUM_SECTION)
#define OptVar               MakeVar(OPT)
#define OptEndSection        EndSection(OPT)
#define OptPrintTestResults  PrintTestResults(OPT)
#define OptMakeResult        MakeTestResult(OPT)
#define OptResults           TestResults

// function to run on test data
#define OptTest(name, tBLOCK)\
	{\
		bool test_result;\
		OptBefore\
		tBLOCK\
		OptAfter(name)\
		if (test_result){\
			OPT_TEST_PASED++;\
		}else{\
			OPT_TEST_FAILED++;\
		}\
		OPT_TEST_TOTAL++;\
	}

TestResult opt_test_main(int test_no);
#endif

