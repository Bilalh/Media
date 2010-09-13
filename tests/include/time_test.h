#ifndef TIME_TEST_HEADER
#define TIME_TEST_HEADER
#include "tests.h"
#include "time_helper.h"
#include <include/time_util.h>

static int TIME_NUM_SECTION = 0; // for Section numbering

// Standard Macros 
#undef  Section
#define Section(title)           StartSection(title,TIME_NUM_SECTION) 
#define TimeAdd(test_result)     TestEndAdd(TIME,test_result)
#define TimeVar                  MakeVar(TIME)
#define TimeEndSection           EndSection(TIME)
#define TimePrintTestResults     PrintTestResults(TIME)
#define TimeMakeResult           MakeTestResult(TIME)
#define TimeResults              TestResults

// Setup that is done before the test is run
#define TimeSetup\
	{\
		struct tm *tm = currentTime();\
		char now[20];\
		MAKE_TIME_STR(now, tm);\
		PrintTitle(now);\
	}

// funtions to run on test data
#define TimeTest(str, tBLOCK)\
	{\
		struct tm *tm = currentTime();\
		tBLOCK\
		timegm(tm);\
		bool test_result = time_test_start(str,tm);\
		TimeAdd(test_result);\
		free(tm);\
	}

TestResult time_test_main(int test_no);
#endif
