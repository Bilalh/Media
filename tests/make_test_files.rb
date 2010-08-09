#!/usr/bin/env ruby1.9
require "FileUtils"
require "Facets"    

(puts "needs name"; exit(0))  if ARGV.length == 0

NAME  = ARGV[0] ;
TNAME = NAME.titlecase;
LNAME = NAME.lowercase
UNAME = NAME.upcase


header = 
%{#ifndef #{UNAME}_TEST_HEADER
#define #{UNAME}_TEST_HEADER
#include "tests.h"
#include "#{LNAME}_helper.h"
//Unit Under Test (relative path)


static int NUM_SECTION = 0; // for Section numbering
// Standard Macros 
#define #{TNAME}Var               MakeVar(#{UNAME})
#define #{TNAME}EndSection        EndSection(#{UNAME})
#define #{TNAME}PrintTestResults  PrintTestResults(#{UNAME})
#define #{TNAME}MakeResult        MakeTestResult(#{UNAME})
#define #{TNAME}Results           TestResults

// Setup that is done before the test is run
#define #{TNAME}Setup

// funtions to run on test data
#define #{TNAME}Test(str, tBLOCK)\\
	{\\
		\\
		tBLOCK\\
		\\
	}
#endif


}

cfile = 
%{#include "#{LNAME}_test.h"
#{TNAME}Var
TestResult #{LNAME}_test_main(int test_no) {
#{TNAME}Setup

Sections {
Section("Section 1"){
	#{TNAME}Test("Result", {
		// put data to calculate result here  
	})
}#{TNAME}EndSection
};

TestRun
#{TNAME}PrintTestResults
#{TNAME}MakeResult;
return #{TNAME}Results;
}

#ifndef ALL_TESTS
MakeMain(#{LNAME})
#endif
}

helper_head = 
%{#ifndef #{UNAME}_HELPER_HEADER
#define #{UNAME}_HELPER_HEADER

#include "#{LNAME}_test.h"

bool #{LNAME}_test_start ( type expected, type actual );

#endif
}

helper_c = 
%{#include "#{LNAME}_helper.h"

bool string_test_start ( type expected, type actual ){
	
}

}


File.create("#{LNAME}_test.h",header)
File.create("#{LNAME}_test.c",cfile)
File.create("#{LNAME}_helper.h",helper_head)
File.create("#{LNAME}_helper.c",helper_c)

