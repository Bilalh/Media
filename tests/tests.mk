names  = string time opt strutil strarr string_extra
tests  = $(addprefix test_, ${names})

string_extra_req  = ${string_req} string_buffer_extra.o
strarr_req        = string_array.o
opt_req           = option_parser.o string_buffer.o string_util.o
string_req        = string_buffer.o
strutil_req       = string_util.o
time_req          = string_util.o time_util.o
hist_req          = history/history.o string_util.o time_util.o
play_req          = playlist.o
ml_req            = history/ml.o string_util.o ${hist_req} ${string_extra_req}

# Makes the test_%                          : ${test_%_req}
$(foreach name,${names},$(eval test_${name} : ${${name}_req}))

test_strarr : ${strutil_req}

all_tests: $(foreach name,${names},${name}_test.o ${name}_helper.o )
all_tests: $(foreach name,${names},${${name}_req})
