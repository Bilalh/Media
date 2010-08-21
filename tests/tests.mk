names = string time
tests = $(addprefix test_, ${names})

test_opt_req    = 
test_string_req = string_buffer.o
test_time_req   = string_util.o time_util.o

# Makes the test_% : ${test_%_req}
$(foreach name,${names},$(eval test_${name}:${test_${name}_req}))
all_tests: $(foreach name,${names},${name}_test.o ${name}_helper.o )
all_tests: $(foreach name,${names},${test_${name}_req})
