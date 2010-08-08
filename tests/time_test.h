#ifndef TIME_TEST_HEDER
#define TIME_TEST_HEDER
#include "tests.h"
#include "time_helper.h"
#include "../time_util.h"

static int NUM_SECTION = 0;


#define TimeVar MakeVar(TIME)

#define TimeSetup\
	{\
		struct tm *tm = currentTime();\
		char now[20];\
		MAKE_TIME_STR(now, tm);\
		PrintTitle(now);\
	}

#define TestTime(str, tBLOCK)\
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

#define TimeEndSection EndSection(TIME)

#define TimeEnd\
	printf("\n     *******TOTAL %i, %s %i %s %i %2.1f%%*******\n\n",\
		TIME_TOTAL_TESTS, \
		(TIME_TOTAL_TESTS_FAILED == 0 ? "failed" : "FAILED"), TIME_TOTAL_TESTS_FAILED,\
		(TIME_TOTAL_TESTS_PASSED  == TIME_TOTAL_TESTS ? "Passed" : "passed"), TIME_TOTAL_TESTS_PASSED,\
		(float) TIME_TOTAL_TESTS_PASSED / (float) TIME_TOTAL_TESTS * 100 );

#define TimeResult\
	TIME_TOTAL_TESTS_FAILED;
	
#endif

