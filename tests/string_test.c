#include "include/string_test.h"
StringVar

// Setup that is done onces before the tests are run
#define StringSetup 

// Setup that is done before each test
#define StringBefore \
	char *expected;\
	String actual;

// test_result must be a true value for true and false otherwise
#define StringAfter(name) \
	test_result = string_test_start (name, &actual, expected );

TestResult string_test_main(int test_no) {
StringSetup

Sections {
Section("Basic Features"){
	StringTest("Empty String", {
		new_string(&actual,1);
		expected = "";
	})
	StringTest("Simple Add", {
		new_string(&actual,1);
		string_add(&actual, "-fs",false);
		expected = "-fs";
	})
	StringTest("Simple Push", {
		new_string(&actual,5);
		string_push(&actual, "dasdasdas");
		expected = " dasdasdas";
	})
	StringTest("Simple append", {
		new_string(&actual,3);
		string_append(&actual, "zazaz azaza");
		expected = "zazaz azaza";
	})
}StringEndSection

Section("Simple Multiple adds/pushes"){
	StringTest("Add multiple", {
		new_string(&actual,-1);
		string_add(&actual, "-fs",false);
		string_add(&actual, "--fast",true);
		expected = "-fs --fast";
	})
	StringTest("Add multiple", {
		new_string(&actual,13);
		string_add(&actual, "aab",true);
		string_add(&actual, "bbc",true);
		string_add(&actual, "ccd",true);
		expected = " aab bbc ccd";
	})
	StringTest("Add multiple push append", {
		new_string(&actual,7);
		string_append(&actual, "dasxsafasf safsafa");
		string_push(&actual, "fdsfdsfdsfdsfsd");
		expected = "dasxsafasf safsafa fdsfdsfdsfdsfsd";
	})
}StringEndSection

Section("Multiple adds/pushes _m"){
	StringTest("multiple add_m", {
		new_string(&actual,13);
		string_add_m(&actual, true, 3, "aab", "bbc", "ccd");
		expected = " aab bbc ccd";
	})
	StringTest("multiple add_m", {
		new_string(&actual,13);
		string_add_m(&actual, true, 3, "aab", "bbc", "ccd");
		expected = " aab bbc ccd";
	})
	StringTest("multiple push_m", {
		new_string(&actual,13);
		string_push_m(&actual, 3, "aab", "bbc", "ccd");
		expected = " aab bbc ccd";
	})
	StringTest("multiple push_m", {
		new_string(&actual,13);
		string_push_m(&actual, 3, "aab", "bbc", "ccd");
		expected = " aab bbc ccd";
	})
	
}StringEndSection


Section("string_sprintf"){
	StringTest("simple %s",{
		new_string(&actual,10);
		string_sprintf(&actual, 5, "%s", "1234"); 
		expected = "1234";
	})
	
	StringTest("simple %d",{
		new_string(&actual,10);
		string_sprintf(&actual, 5, "%d", 1234); 
		expected = "1234";
	})
	
	StringTest("multi %s",{
		new_string(&actual,10);
		string_sprintf(&actual, 10, "%s %s", "1234", "abcd"); 
		expected = "1234 abcd";
	})
	
	StringTest("multi commands",{
		new_string(&actual,10);
		string_sprintf(&actual, 6, "%s ", "1234"); 
		string_sprintf(&actual, 6, "%d ", 5678); 
		string_sprintf(&actual, 10, "%s %s", "1234", "abcd"); 
		expected = "1234 5678 1234 abcd";
	})

	StringTest("%s long",{
		new_string(&actual,10);
		string_sprintf(&actual, 70,
			"Update SeriesInfo Set Total = %s, Id = %s where Title = '%s';", 
			"13", "6547", "Angel b"
		);
		expected = 
			"Update SeriesInfo Set Total = 13, Id = 6547 where Title = 'Angel b';";
	})
	
	StringTest("2* %s long",{
		new_string(&actual,10);
		string_sprintf(&actual, 70,
			"Update SeriesInfo Set Total = %s, Id = %s where Title = '%s'; ", 
			"13", "6547", "Angel b"
		);
		string_sprintf(&actual, 70,
			"Update SeriesInfo Set Total = %s, Id = %s where Title = '%s'; ", 
			"13", "6547", "Angel b"
		);
		expected = 
			"Update SeriesInfo Set Total = 13, Id = 6547 where Title = 'Angel b'; "
			"Update SeriesInfo Set Total = 13, Id = 6547 where Title = 'Angel b'; "
		;
	})
	
	StringTest("10* %s long",{
		new_string(&actual,10);

		for(int i = 1; i < 11; ++i){
			string_sprintf(&actual, 70,
				"Update SeriesInfo Set Total = %02d, Id = %s where Title = '%s'; ", 
				i, "6547", "Angel b"
			);
		}
		
		expected = 
			"Update SeriesInfo Set Total = 01, Id = 6547 where Title = 'Angel b'; "
			"Update SeriesInfo Set Total = 02, Id = 6547 where Title = 'Angel b'; "
			"Update SeriesInfo Set Total = 03, Id = 6547 where Title = 'Angel b'; "
			"Update SeriesInfo Set Total = 04, Id = 6547 where Title = 'Angel b'; "
			"Update SeriesInfo Set Total = 05, Id = 6547 where Title = 'Angel b'; "
			"Update SeriesInfo Set Total = 06, Id = 6547 where Title = 'Angel b'; "
			"Update SeriesInfo Set Total = 07, Id = 6547 where Title = 'Angel b'; "
			"Update SeriesInfo Set Total = 08, Id = 6547 where Title = 'Angel b'; "
			"Update SeriesInfo Set Total = 09, Id = 6547 where Title = 'Angel b'; "
			"Update SeriesInfo Set Total = 10, Id = 6547 where Title = 'Angel b'; "
		;
	})
	
	
}StringEndSection

};

TestRun
StringPrintTestResults
StringMakeResult;
return StringResults;
}

