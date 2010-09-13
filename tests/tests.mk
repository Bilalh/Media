names  = string time opt strutil
tests  = $(addprefix test_, ${names})

opt_req          = option_parser.o string_buffer.o string_util.o
string_req       = string_buffer.o
strutil_req      = string_util.o
time_req         = string_util.o time_util.o
hist_req         = sql/history.o string_util.o time_util.o
play_req         = playlist.o

# Makes the test_% : ${test_%_req}
$(foreach name,${names},$(eval test_${name}:${${name}_req}))

all_tests: $(foreach name,${names},${name}_test.o ${name}_helper.o )
all_tests: $(foreach name,${names},${${name}_req})
