#ifndef TEST_HEADER_MAIN
#define TEST_HEADER_MAIN
//includes
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <Block.h>
#include <dispatch/dispatch.h>


// common macros
#define FAIL false
#define PASS true
#define PRINT_FAIL puts(" : Wrong")
#define PRINT_PASS puts(" : correct")

#define LINE_LENGTH 57 // max line length in output 
#define LINE_REC_1  31 // recommend first  arg length 
#define LINE_REC_2  24 // recommend second arg length 
#define ERROR_SEP "   " // sep for error

#define PrintTest(title,data)  printf("%-*s %*s", LINE_REC_1, title, LINE_REC_2, data)


// struct for returning results
typedef struct{
	int number_of_tests;
	int passes;
	int failures;
} TestResult;

// Sets up a test section
#define StartSection(title,num) ^{ PrintTitleN(title,num)
#define EndSection(NAME) \
	EndPrintReset(NAME)\
	SectionEnd
#define SectionEnd \
	},

// Sets the the block to hold all the sections of the tests. 
#define Sections void (^blocks[])() =

// Make variables that are needed by the macros  
#define MakeVar(NAME) \
	static int NAME##_TEST_PASED = 0, NAME##_TEST_FAILED = 0, NAME##_TEST_TOTAL = 0;\
	int NAME##_TOTAL_TESTS_PASSED = 0,  NAME##_TOTAL_TESTS_FAILED = 0, NAME##_TOTAL_TESTS = 0;

// Runs all test sections if the number is <= -2 or > the number of tests -1
// runs the last test if -1, otherwise runs the specify test
#define TestRun \
int test_size = sizeof(blocks)/sizeof(blocks[0]);\
if (test_size > 0){\
	if (test_no == -1){\
		(blocks[test_size-1])();\
	}\
	else if ( test_no >= 0 && test_no < test_size ){\
		(blocks[test_no])();\
	}else{\
		for(int i = 0 ;i < test_size; i++ ) (blocks[i])();\
	}\
}

// Makes the results objects
#define TestResults test_result
#define MakeTestResult(NAME) \
TestResult test_result = {\
	.number_of_tests = 1,\
	.passes          = 1,\
	.failures        = 0\
}

// prints the test results of the section
#define PrintTestResults(NAME)\
	printf("\n     *******TOTAL %i, %s %i %s %i %2.1f%%*******\n\n",\
		NAME##_TOTAL_TESTS, \
		(NAME##_TOTAL_TESTS_FAILED == 0 ? "failed" : "FAILED"), NAME##_TOTAL_TESTS_FAILED,\
		(NAME##_TOTAL_TESTS_PASSED  == NAME##_TOTAL_TESTS ? "Passed" : "passed"), NAME##_TOTAL_TESTS_PASSED,\
		(float) NAME##_TOTAL_TESTS_PASSED / (float) NAME##_TOTAL_TESTS * 100 );
	
// *****Helper Macros*****
	
	
// Printing Section titles
#define SEP "-------------------------------------------------------------------"
#define PrintTitle(title) printf("\n%s\n%s\n%s\n",SEP,title,SEP)
#define PrintTitleN(title,num) \
	printf("\n%s\n%i: %s\n%s\n", SEP,num,title,SEP);\
	num++;

// print the section results and reset the counter.
#define EndPrintReset(NAME)\
	printf("\n\t****Total %i, %s %i %s %i %2.1f%%****\n",\
		NAME##_TEST_TOTAL, \
		(NAME##_TEST_FAILED == 0 ? "failed" : "FAILED"), NAME##_TEST_FAILED,\
		(NAME##_TEST_PASED  == NAME##_TEST_TOTAL ? "Passed" : "passed"), NAME##_TEST_PASED,\
		(float) NAME##_TEST_PASED / (float) NAME##_TEST_TOTAL * 100 );\
		NAME##_TOTAL_TESTS_PASSED += NAME##_TEST_PASED ;\
		NAME##_TOTAL_TESTS_FAILED += NAME##_TEST_FAILED;\
		NAME##_TOTAL_TESTS        += NAME##_TEST_TOTAL;\
		NAME##_TEST_PASED = 0, NAME##_TEST_FAILED = 0, NAME##_TEST_TOTAL = 0;
// Makes a main method unless a running all tests at once 
#define MakeMain(NAME)\
	int main (int argc, char const *argv[]) {\
		long num = -2;\
		if (argc == 2){\
			int res = strtol(argv[1], NULL, 10);\
			if (errno != EINVAL && res >= -1) {\
				num = res;\
			}\
		}\
		return NAME##_test_main(num).failures;\
	}	
	
#endif
