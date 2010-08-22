#ifndef STRUTIL_TEST_HEADER
#define STRUTIL_TEST_HEADER
#include "tests.h"
#include "strutil_helper.h"

// for Section numbering
static int STRUTIL_NUM_SECTION = 0; 

// Standard Macros 
#undef  Section
#define Section(title)           StartSection(title,STRUTIL_NUM_SECTION)
#define StrutilVar               MakeVar(STRUTIL)
#define StrutilEndSection        EndSection(STRUTIL)
#define StrutilPrintTestResults  PrintTestResults(STRUTIL)
#define StrutilMakeResult        MakeTestResult(STRUTIL)
#define StrutilResults           TestResults

// Setup that is done before the test is run
#define StrutilSetup

// function to run on test data
#define StrutilTest(_name, tBLOCK)\
	{\
		char *name = _name;\
		bool test_result;\
		StrutilBefore\
		tBLOCK\
		StrutilAfter(_name)\
		StrutilAdd(test_result)\
	}

#define StrutilTestM(_name, tBLOCK)\
	{\
		char *name = _name;\
		bool test_result;\
		StrutilPrintTest(_name,"");\
		tBLOCK\
		if ( test_result ){\
			PRINT_PASS;\
		}else{\
			PRINT_FAIL;\
		}\
		StrutilAdd(test_result)\
	}

#define StrutilAdd(test_result) \
	if (test_result){\
		STRUTIL_TEST_PASED++;\
	}else{\
		STRUTIL_TEST_FAILED++;\
	}\
	STRUTIL_TEST_TOTAL++;

TestResult strutil_test_main(int test_no);
#endif

