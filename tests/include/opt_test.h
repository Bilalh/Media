#ifndef OPT_TEST_HEADER
#define OPT_TEST_HEADER
#include "tests.h"
#include "opt_helper.h"
#include  <sys/types.h>

// for Section numbering
static int OPT_NUM_SECTION = 0; 

// Standard Macros 
#undef  Section
#define Section(title)          StartSection(title,OPT_NUM_SECTION)
#define OptAdd(test_result)     TestEndAdd(OPT,test_result)
#define OptEndSection           EndSection(OPT)
#define OptMakeResult           MakeTestResult(OPT)
#define OptPrintTestResults     PrintTestResults(OPT)
#define OptResults              TestResults
#define OptTestM(_name, tBLOCK) TestManual(OPT, _name, tBLOCK)
#define OptVar                  MakeVar(OPT)

// function to run on test data
// uses fork since getopt_long can only be used once per program
// the parent adds it to the results 
#define OptTest(_name, tBLOCK){\
	int stat_loc;\
    pid_t  pid = fork();\
	if (pid == 0)\
	{\
		char *name = "  "_name;\
		bool test_result;\
		OptBefore\
		tBLOCK\
		OptAfter(name)\
		exit(test_result);\
	}else{\
		wait(&stat_loc);\
		OptAdd(WEXITSTATUS(stat_loc))\
	}\
}

TestResult opt_test_main(int test_no);
#endif

