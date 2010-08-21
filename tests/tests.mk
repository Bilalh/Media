names  = string time opt
tnames = $(filter-out opt,${names})
tests  = $(addprefix test_, ${tnames})

opt_req    = option_parser.o string_buffer.o string_util.o
string_req = string_buffer.o
time_req   = string_util.o time_util.o


# Makes the test_% : ${test_%_req}
$(foreach name,${names},$(eval test_${name}:${${name}_req}))
all_tests: $(foreach name,${tnames},${name}_test.o ${name}_helper.o )
all_tests: $(foreach name,${tnames},${${name}_req})
