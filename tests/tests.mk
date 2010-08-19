ALL_TEST = test_time test_string 
#test_opt
all_tests: string_test.o string_helper.o time_test.o time_helper.o 
all_tests: string_buffer.o string_util.o time_util.o
test_opt: 
test_string: string_buffer.o
test_time:   string_util.o time_util.o
