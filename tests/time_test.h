#ifndef TIME_TEST_HEDER
#define TIME_TEST_HEDER
#include <stdio.h>
#include <stdbool.h>
#include "../time_util.h"
#include "time_helper.h"


#define SEP "-------------------------------------------------------------------"
#define printTitle(title) printf("\n%s\n%s\n%s\n",SEP,title,SEP)

#define TimeSetup\
	int TIME_TEST_PASED = 0, TIME_TEST_FAILED = 0, TIME_TEST_TOTAL = 0;\
	int TIME_TOTAL_TESTS_PASSED = 0,  TIME_TOTAL_TESTS_FAILED = 0, TIME_TOTAL_TESTS = 0;\
	{\
		struct tm *tm = currentTime();\
		char now[20];\
		MAKE_TIME_STR(now, tm);\
		printTitle(now);\
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

#define TimeEndSection\
	printf("\n\t****Total %i, %s %i %s %i %2.1f%%****\n",\
		TIME_TEST_TOTAL, \
		(TIME_TEST_FAILED == 0 ? "failed" : "FAILED"), TIME_TEST_FAILED,\
		(TIME_TEST_PASED  == TIME_TEST_TOTAL ? "Passed" : "passed"), TIME_TEST_PASED,\
		(float) TIME_TEST_PASED / (float) TIME_TEST_TOTAL * 100 );\
		TIME_TOTAL_TESTS_PASSED += TIME_TEST_PASED ;\
		TIME_TOTAL_TESTS_FAILED += TIME_TEST_FAILED;\
		TIME_TOTAL_TESTS        += TIME_TEST_TOTAL;\
		TIME_TEST_PASED = 0, TIME_TEST_FAILED = 0, TIME_TEST_TOTAL = 0;

#define TimeEnd\
	printf("\n     *******TOTAL %i, %s %i %s %i %2.1f%%*******\n\n",\
		TIME_TOTAL_TESTS, \
		(TIME_TOTAL_TESTS_FAILED == 0 ? "failed" : "FAILED"), TIME_TOTAL_TESTS_FAILED,\
		(TIME_TOTAL_TESTS_PASSED  == TIME_TOTAL_TESTS ? "Passed" : "passed"), TIME_TOTAL_TESTS_PASSED,\
		(float) TIME_TOTAL_TESTS_PASSED / (float) TIME_TOTAL_TESTS * 100 );\

#endif