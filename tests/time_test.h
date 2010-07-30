// To Use 
// TestTime(teststring,
// 		data
// )
// can use tm which is a struct tm

#ifndef TIME_TEST_HEDER
#define TIME_TEST_HEDER
#include <stdio.h>
#include <stdbool.h>
#include "../time_util.h"
#include "time_helper.h"


#define SEP "---------------------------------------------------------"
#define printTitle(title) printf("\n%s\n%s\n%s\n",SEP,title,SEP)

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
#endif


#define TimeSetup\
	static int TIME_TEST_PASED = 0, TIME_TEST_FAILED = 0, TIME_TEST_TOTAL = 0;\
	{\
		struct tm *tm = currentTime();\
		char now[20];\
		MAKE_TIME_STR(now, tm);\
		printTitle(now);\
	}

#define TimeEndSection\
	printf("\n\t****Total %i, %s %i %s %i %2.1f%%****\n",\
		TIME_TEST_TOTAL, \
		(TIME_TEST_FAILED == 0 ? "failed" : "FAILED"), TIME_TEST_FAILED,\
		(TIME_TEST_PASED  == TIME_TEST_TOTAL ? "Passed" : "passed"), TIME_TEST_PASED,\
		(float) TIME_TEST_PASED / (float) TIME_TEST_TOTAL * 100 );\
		TIME_TEST_PASED = 0, TIME_TEST_FAILED = 0, TIME_TEST_TOTAL = 0;

