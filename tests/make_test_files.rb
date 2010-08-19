#!/usr/bin/env ruby1.9
require "FileUtils"
require "Facets"    


(puts "needs name"; exit(0))  if ARGV.length == 0
(puts "makes tests file named NAME_test.c NAME_helper.c and the headers and \
the headers in includes. Also adds a makefile entry to tests.mk"; 
	exit(0))  if ARGV[0] =~ /--?h|--?help/i

NAME  = ARGV[0] ;
TNAME = NAME.titlecase;
LNAME = NAME.lowercase
UNAME = NAME.upcase


header = 
%{#ifndef #{UNAME}_TEST_HEADER
#define #{UNAME}_TEST_HEADER
#include "tests.h"
#include "#{LNAME}_helper.h"

// for Section numbering
static int NUM_SECTION = 0; 

// Standard Macros 
#define #{TNAME}Var               MakeVar(#{UNAME})
#define #{TNAME}EndSection        EndSection(#{UNAME})
#define #{TNAME}PrintTestResults  PrintTestResults(#{UNAME})
#define #{TNAME}MakeResult        MakeTestResult(#{UNAME})
#define #{TNAME}Results           TestResults

// Setup that is done before the test is run
#define #{TNAME}Setup

// function to run on test data
#define #{TNAME}Test(name, tBLOCK)\\
	{\\
		bool test_result;\\
		#{TNAME}before\\
		tBLOCK\\
		#{TNAME}After(name)\\
		if (test_result){\\
			#{UNAME}_TEST_PASED++;\\
		}else{\\
			#{UNAME}_TEST_FAILED++;\\
		}\\
		#{UNAME}_TEST_TOTAL++;\\
	}

TestResult #{LNAME}_test_main(int test_no);
#endif

}

cfile = 
%{#include "#{LNAME}_test.h"
#{TNAME}Var

// Setup that is done once before the tests are run
#define #{TNAME}Setup 

// Setup that is done before each test
#define #{TNAME}Before \\
	TYPE expected;\\
	TYPE actual;

// test_result must be a true value for true and false otherwise
#define #{TNAME}After(name) \\
	test_result = #{LNAME}_test_start (name, expected, &actual);

TestResult #{LNAME}_test_main(int test_no) {
#{TNAME}Setup

Sections {
Section("Basic Features"){
	#{TNAME}Test("Name", {
		// put data to calculate result here  
	})
}#{TNAME}EndSection

};

TestRun
#{TNAME}PrintTestResults
#{TNAME}MakeResult;
return #{TNAME}Results;
}

}

helper_head = 
%{#ifndef #{UNAME}_HELPER_HEADER
#define #{UNAME}_HELPER_HEADER

#include "#{LNAME}_test.h"
// Include all files under test here

bool #{LNAME}_test_start ( char *name, TYPE actual, TYPE expected );

#endif
}

helper_c = 
%{#include "#{LNAME}_helper.h"

#define #{TNAME}PrintTest(title,data)  PrintTest(title,data)
#define #{UNAME}_TEST_FAIL(actual,exp) PRINT_FAIL; #{UNAME}_SHOW_FAIL_DATA(actual,exp); return FAIL
#define #{UNAME}_SHOW_FAIL_DATA(actual,exp) \\
		printf("%s%-14s: '%s'\\n", ERROR_SEP, "expected", exp );\\
		printf("%s%-14s: '%s'\\n", ERROR_SEP, "act->str", actual->str);

bool #{TNAME}_test_start ( char *name, TYPE actual, TYPE expected ){
	
	if ( false ){
		#{TNAME}PrintTest(name,"");
		#{UNAME}_TEST_FAIL(actual,expected);
	}
	
	#{TNAME}PrintTest(name,  
		strlen(actual->str) < LINE_REC_2 ? actual->str : "" 
	);

	PRINT_PASS
	return PASS;
}

}


File.create("include/#{LNAME}_test.h",header)
File.create("#{LNAME}_test.c",cfile)
File.create("include/#{LNAME}_helper.h",helper_head)
File.create("#{LNAME}_helper.c",helper_c)

mk_arr = File.read_list("tests.mk")
if (mk_arr.grep Regexp.new "test_#{LNAME}").size == 0 then
	mk_arr[0] += " test_#{LNAME}"
	mk_arr.push "test_#{LNAME}: "
else
	puts "Rules has already been added to make file"
end

mk_arr.sort!
File.writelines("tests.mk",mk_arr)
