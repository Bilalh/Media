#include "include/strutil_test.h"
StrutilVar

// Setup that is done once before the tests are run
#define StrutilSetup 

// Setup that is done before each test
#define StrutilBefore \
	char* actual;\
	char* expected; 
	
// test_result must be a true value for true and false otherwise
#define StrutilAfter(name) \
	test_result = strutil_test_start (name, actual, expected);\
	free(actual);

TestResult strutil_test_main(int test_no) {
StrutilSetup

Sections {
Section("str_lower"){
	StrutilTest("ABC", {
		actual   = str_lower(name, strlen(name));
		expected = "abc";
	})
	StrutilTest("dsdsDDKDKDsdsdsdsa", {
		actual   = str_lower(name, strlen(name));
		expected = "dsdsddkdkdsdsdsdsa";
	})
	StrutilTest("Actual = Str_Lower(Name);", {
		actual   = str_lower(name, strlen(name));
		expected = "actual = str_lower(name);";
	})
	StrutilTest("dsadasdascxczxc", {
		actual   = str_lower(name, strlen(name));
		expected = "dsadasdascxczxc";
	})
	StrutilTest("sad≈≈sa", {
		actual = str_lower(name, strlen(name));
		expected = "sad≈≈sa";
	})
}StrutilEndSection

Section("str_replace"){
	StrutilTest("str",{
		actual   = str_replace(name, strlen(name), "str", "string");
		expected = "string";
	})
	StrutilTest("_str_",{
		actual   = str_replace(name, strlen(name), "str", "string");
		expected = "_string_";
	})
	StrutilTest("_rep_REP_rpp_ree_ReP_rep",{
		actual   = str_replace(name, strlen(name), "rep", "new");
		expected = "_new_REP_rpp_ree_ReP_new";
	})
	StrutilTest("cpp|rb",{
		actual   = str_replace(name, strlen(name),"rb","ruby");
		actual   = str_replace(actual, strlen(actual),"cpp", "c plus plus");
		expected = "c plus plus|ruby";
	})
}StrutilEndSection

Section("ep_num"){
	StrutilTestM("a - 1",{
		test_result = false;
		PrintTesti(name,33);
		PrintRes;
	})
}StrutilEndSection

};


TestRun
StrutilPrintTestResults
StrutilMakeResult;
return StrutilResults;
}

