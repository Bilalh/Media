#include "include/all_tests.h"

/**
 * @brief Runs all the test and shows results
 *
 * @return 
 */
int main (void) {
	
	TestResult (*tests[]) (int test_no) ={
		ALL_TESTS
	};
	
	int length = sizeof(tests)/sizeof(tests[0]);
	
	int failed = 0, passed = 0, total = 0;
	
	TestResult results[length];
	
	for(int i = 0; i <length; ++i){
		results[i] = tests[i](-2);
		failed += results[i].failures;
		passed += results[i].passes;
		total  += results[i].number_of_tests;
	}
	
	
	printf(" **********        RESULTS: %6s %3i/%-3i       ********** \n", 
		total == passed ? "Passed": "FAILED" ,
		passed, total
	);
	
	for(int i = 0; i < length; ++i){
		printf("%22s: %6s  %6s %2i passed  %2i %2.1f%%\n", 
			results[i].name,
			results[i].passes == results[i].number_of_tests ? "Passed" : "FAILED",
			results[i].passes == results[i].number_of_tests ? "failed" : "Failed",
			results[i].failures,
			results[i].passes,
			(float) results[i].passes / results[i].number_of_tests * 100
		);
	}
	
	return failed;
}