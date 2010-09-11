                           ### Variables ###
CC             = gcc
CFLAGS         = -g -Wall
PERL           = perl
compile        = ${CC} -std=gnu99 -fblocks
flags          = ${CFLAGS} ${includes} -I../include -I../src -I ../lib/hash
includes       = -I/opt/local/include -I/usr/include/libxml2 
libs           = -lsqlite3 -lxml2 -L/opt/local/lib -lpcre -lcurl
.DEFAULT_GOAL := all_tests

                           ### Includes ###
vpath %.c ../src ../lib/hash
vpath %.h ../include/
# Rules for tests and requiments
include ../tests/tests.mk


                           ### Objects ###
objs       = media optt tempc hashe2 new
media_req  = main.o media.o ${opt_req} ${hist_req} ${play_req} ${strutil_req}
optt_req   = ${opt_req} optm.o
tempc_req  = temp.o ${strutil_req} hashtable_itr.o hashtable.o hashtable_utility.o
hashe2_req = hashe2.o
new_req    = ${strutil_req} new.o

                           ### Objects rules ###
# makes name : ${name_req}
$(foreach name,${objs},$(eval ${name}:${${name}_req}))

${objs}: 
	${compile} ${libs} ${flags} ${^F} -o $@

list_objs:
	@${PERL} -e 'print join "\n", qw{${objs}}, ""'


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
	rm -f *.o *.out test_* *_wrapper.c *_wrapper.o  all_tests ${objs} *~
