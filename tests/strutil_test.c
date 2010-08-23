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


#define StrutilTestEp(_name, tBLOCK)                \
	MakeTest(Strutil, _name, tBLOCK,                \
		char **actual;                              \
		char  *exp_str;                             \
		int    exp_int;                             \
		,                                           \
		test_result =                              \
		ep_num_test(name, actual, exp_int, exp_str);\
		free(actual);                               \
	);
	
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
	StrutilTestM("Toindex - 4.avi",{
		test_result = ep_num_test(name, ep_num(name), 4, "Toindex");		
	})
	StrutilTestEp("AAA - 1.mkv",{
		actual  = ep_num(name);
		exp_str = "AAA";
		exp_int = 1;
	})
	StrutilTestEp("aaa - 1.mkv",{
		actual  = ep_num(name);
		exp_str = "aaa";
		exp_int = 1;
	})
	StrutilTestEp("dsdsd - 32.mkv",{
		actual  = ep_num(name);
		exp_str = "dsdsd";
		exp_int = 32;
	})
	StrutilTestEp("Zs - 322.mkv",{
		actual  = ep_num(name);
		exp_str = "Zs";
		exp_int = 322;
	})
	StrutilTestEp("zs - 322.mkv",{
		actual  = ep_num(name);
		exp_str = "zs";
		exp_int = 322;
	})
	StrutilTestEp("T - 32.mkv",{
		actual  = ep_num(name);
		exp_str = "T";
		exp_int = 32;
	})
	StrutilTestEp("t - 22.mkv",{
		actual  = ep_num(name);
		exp_str = "t";
		exp_int = 22;
	})
}StrutilEndSection

};


TestRun
StrutilPrintTestResults
StrutilMakeResult;
return StrutilResults;
}
