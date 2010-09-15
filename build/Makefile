                           ### Variables ###
CC             = gcc
CFLAGS         = -g -Wall
PERL           = perl
compile        = ${CC} -std=gnu99 -fblocks
flags          = ${CFLAGS} ${includes} -I../src -I../
includes       = -I/opt/local/include -I/usr/include/libxml2 
libs           = -lsqlite3 -lxml2 -L/opt/local/lib -lpcre -lcurl
.DEFAULT_GOAL := media

                           ### Includes ###
vpath %.c ../src 
vpath %.h ../
# Rules for tests and requiments
include ../tests/tests.mk


                           ### Objects ###
objs       = media mal nums numsf numss hista
examples   = optt hashe2

media_req  = app/media_main.o media.o ${opt_req} ${hist_req} ${play_req} ${strutil_req}
mal_req    = app/ml_main.o ${ml_req} ${hist_req}

nums_req   = app/latest_main.o ${hist_req}
numsf_req  = app/latest_f_main.o ${hist_req}
numss_req  = app/latest_fs_main.o ${hist_req}

hista_req  = app/history_add_main.o ${hist_req} ${time_req}  ${strutil_req} ${string_req}

optt_req   = examples/optm.o ${opt_req}
hashe2_req = examples/hashe2.o


                           ### Objects rules ###
# makes name : ${name_req}
$(foreach name,${objs} ${examples},$(eval ${name}:${${name}_req}))

${objs} ${examples}: 
	${compile} ${libs} ${flags} ${^F} -o $@

list_objs:
	@${PERL} -e 'print join "\n", qw{${objs}}, ""'

all:      ${objs}
examples: ${examples}


                           ### Tests ###
all_tests: ${tests} ../tests/all_tests.o
	${compile} ${libs} ${flags} $(filter %.o,${^F}) -o $@
	@echo "Built all tests : ${tests}"

test_%: ../tests/%_test.o ../tests/%_helper.o %_wrapper.o
	${compile} ${libs} ${flags}  ${^F} -o $@

list_tests:
	@${PERL} -e 'print join "\n", qw{${tests}}, ""'

# makes main method
%_wrapper.c:
	./make_wrapper.sh "$@" "../tests/include/"


                           ### util ###
%.o: %.c
	${compile} ${flags} -c $^ -o ${@F}

clean:
	rm -f *.o *.out test_* *_wrapper.c *_wrapper.o  all_tests ${objs} ${examples} *~

oclean: 
	rm -f *.o *.out test_* *_wrapper.c *_wrapper.o  all_tests ${examples} *~
