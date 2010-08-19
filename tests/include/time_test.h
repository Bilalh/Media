#ifndef TIME_TEST_HEADER
#define TIME_TEST_HEADER
#include "tests.h"
#include "time_helper.h"
//Unit Under Test (relative path)
#include "time_util.h"

static int NUM_SECTION = 0; // for Section numbering
// Standard Macros 
#define TimeVar               MakeVar(TIME)
#define TimeEndSection        EndSection(TIME)
#define TimePrintTestResults  PrintTestResults(TIME)
#define TimeMakeResult        MakeTestResult(TIME)
#define TimeResults           TestResults

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
		if (time_test_start(str,tm)){\
			TIME_TEST_PASED++;\
		}else{\
			TIME_TEST_FAILED++;\
		}\
		TIME_TEST_TOTAL++;\
	}

TestResult time_test_main(int test_no);
#endif
