ALL_TEST = test_time test_string
test_string: string_buffer.o
test_time:   string_util.o time_util.o
