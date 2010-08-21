#ifndef STRUTIL_TEST_HEADER
#define STRUTIL_TEST_HEADER
#include "tests.h"
#include "strutil_helper.h"

// for Section numbering
static int STRUTIL_NUM_SECTION = 0; 

// Standard Macros 
#undef  Section
#define Section(title)            StartSection(title,STRUTIL_NUM_SECTION)
#define StrutilVar               MakeVar(STRUTIL)
#define StrutilEndSection        EndSection(STRUTIL)
#define StrutilPrintTestResults  PrintTestResults(STRUTIL)
#define StrutilMakeResult        MakeTestResult(STRUTIL)
#define StrutilResults           TestResults

// Setup that is done before the test is run
#define StrutilSetup

// function to run on test data
#define StrutilTest(name, tBLOCK)\
	{\
		bool test_result;\
		StrutilBefore\
		tBLOCK\
		StrutilAfter(name)\
		if (test_result){\
			STRUTIL_TEST_PASED++;\
		}else{\
			STRUTIL_TEST_FAILED++;\
		}\
		STRUTIL_TEST_TOTAL++;\
	}

TestResult strutil_test_main(int test_no);
#endif

