#ifndef STRUTIL_TEST_HEADER
#define STRUTIL_TEST_HEADER
#include "tests.h"
#include "strutil_helper.h"

// for Section numbering
static int STRUTIL_NUM_SECTION = 1; 

// Standard Macros 
#undef  Section
#define Section(title)              StartSection(title,STRUTIL_NUM_SECTION)
#define StrutilAdd(test_result)     TestEndAdd(STRUTIL,test_result)
#define StrutilEndSection           EndSection(STRUTIL)
#define StrutilMakeResult           MakeTestResult(STRUTIL)
#define StrutilPrintTestResults     PrintTestResults(STRUTIL)
#define StrutilResults              TestResults
#define StrutilTestM(_name, tBLOCK) TestManual(Strutil, _name, tBLOCK)
#define StrutilVar                  MakeVar(STRUTIL)

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

TestResult strutil_test_main(int test_no);
#endif

