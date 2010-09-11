#ifndef TEST_HEADER_MAIN
#define TEST_HEADER_MAIN
//includes
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <Block.h>
#include <dispatch/dispatch.h>


// common macros
#define FAIL false
#define PASS true
#define PRINT_FAIL puts(" : Wrong")
#define PRINT_PASS puts(" : correct")

#define LINE_LENGTH 57  // max line length in output 
#define LINE_REC_1  40  // recommend first  arg length 
#define LINE_REC_2  15  // recommend second arg length 
#define ERROR_SEP "   " // sep for error


#define PrintTest(title,data)          PrintTestV(title,data,"s","s")
#define PrintTesti(title,data)         PrintTestV(title,data,"s","i")
#define PrintTestl(title,data)         PrintTestV(title,data,"s","li")
#define PrintTestii(title,data)        PrintTestV(title,data,"i","i")
#define PrintTestll(title,data)        PrintTestV(title,data,"li","li")
#define PrintRes \
	if (test_result){\
		PRINT_PASS;\
	}else{\
		PRINT_FAIL;\
	}

/**
 * @brief Print the test name and extra data
 *
 * @param title The title 
 * @param data  Extra data to print
 * @param first printf type 
 * @param second printf type 
**/
#define PrintTestV(title,data,first,sec)  \
	printf("%-*"first"%*"sec, LINE_REC_1, title, LINE_REC_2, data)

#define PrintTestA(title,type)  \
printf("%-*"type, LINE_REC_1 +  LINE_REC_2, title)

#define eprintf(FMT, args...) printf(ERROR_SEP FMT,## args );
#define eprintf(FMT, args...) printf(ERROR_SEP FMT,## args );
#define esprintf(STRING, FMT, args...) sprintf(STRING, ERROR_SEP FMT,## args );

#define PRINT_NAME_FAIL(title) PrintTest(title, ""); PRINT_FAIL
#define PRINT_NAME_PASS(title) PrintTest(title, ""); PRINT_PASS

#define PRINT_NAME_FAIL_a(title) PrintTestA(title, "s"); PRINT_FAIL
#define PRINT_NAME_PASS_a(title) PrintTestA(title, "s"); PRINT_PASS

#define PRINT_IF_PASSED(name) \
	if(name,test_result) {\
		PRINT_NAME_PASS(name);\
	}

#define PRINT_IF_PASSED_a(name) \
	if(name,test_result){\
		PRINT_NAME_PASS_a(name);\
	}



#define STRING_FAIL(actual,expected)\
	printf("%s%-14s: '%s'\n", ERROR_SEP, "expected", expected );\
	printf("%s%-14s: '%s'\n", ERROR_SEP, "actual", actual);

// struct for returning results
typedef struct{
	int  number_of_tests;
	int  passes;
	int  failures;
	char *name;
} TestResult;

// Sets up a test section
#define StartSection(title,num) ^{ PrintTitleN(title,num)
#define EndSection(NAME)\
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
	.number_of_tests = NAME##_TOTAL_TESTS,\
	.passes          = NAME##_TOTAL_TESTS_PASSED,\
	.failures        = NAME##_TOTAL_TESTS_FAILED,\
	.name            = strdup(__func__)\
};\
test_result.name[strlen(test_result.name)-5] = '\0';

// prints the test results of the section
#define PrintTestResults(NAME)\
	printf("\n     *******TOTAL %2i, %s %2i %s %2i %2.1f%%*******\n\n",\
		NAME##_TOTAL_TESTS, \
		(NAME##_TOTAL_TESTS_FAILED == 0 ? "failed" : "FAILED"), NAME##_TOTAL_TESTS_FAILED,\
		(NAME##_TOTAL_TESTS_PASSED  == NAME##_TOTAL_TESTS ? "Passed" : "passed"), NAME##_TOTAL_TESTS_PASSED,\
		(float) NAME##_TOTAL_TESTS_PASSED / (float) NAME##_TOTAL_TESTS * 100 );
	
	
// *****Helper Macros*****

#define TRUTH(boolean) (boolean ? "true" : "false" )
#define NULLCHECK(val) (val == NULL ? "NULL" : val )

// Printing Section titles
#define SEP "-------------------------------------------------------------------"
#define PrintTitle(title) printf("\n%s\n%s\n%s\n",SEP,title,SEP)
#define PrintTitleN(title,num) \
	printf("\n%s\n%2i: %s\n%s\n", SEP,num,title,SEP);\
	num++;

// Adds 1 to counters 
#define TestEndAdd(NAME, test_result) \
	if (test_result){\
		NAME##_TEST_PASED++;\
	}else{\
		NAME##_TEST_FAILED++;\
	}\
	NAME##_TEST_TOTAL++;

// makes name, test_result and adds to counters
#define TestManual(NAME, _name, tBLOCK)\
	MakeTest(NAME, _name, tBLOCK,,)

#define MakeTest(NAME, _name, tBLOCK, BEFORE, AFTER)\
	{\
		BEFORE\
		char *name = _name;\
		bool test_result = true;\
		tBLOCK\
		AFTER\
		NAME##Add(test_result)\
	}


// print the section results and reset the counter.
#define EndPrintReset(NAME)\
	printf("\n\t****Total %2i, %s %2i %s %2i %2.1f%%****\n",\
		NAME##_TEST_TOTAL, \
		(NAME##_TEST_FAILED == 0 ? "failed" : "FAILED"), NAME##_TEST_FAILED,\
		(NAME##_TEST_PASED  == NAME##_TEST_TOTAL ? "Passed" : "passed"), NAME##_TEST_PASED,\
		(float) NAME##_TEST_PASED / (float) NAME##_TEST_TOTAL * 100 );\
		NAME##_TOTAL_TESTS_PASSED += NAME##_TEST_PASED ;\
		NAME##_TOTAL_TESTS_FAILED += NAME##_TEST_FAILED;\
		NAME##_TOTAL_TESTS        += NAME##_TEST_TOTAL;\
		NAME##_TEST_PASED = 0, NAME##_TEST_FAILED = 0, NAME##_TEST_TOTAL = 0;

// Makes a main method unless a running all tests at once allows -1 for last section
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

#define TEST_DIR "../tests/"
	
#endif
