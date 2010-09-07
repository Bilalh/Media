CC          = gcc
CFLAGS      = -g -Wall
compile     = ${CC} -std=gnu99 -fblocks
flags       = ${CFLAGS} ${includes} -I../include -I../src -I ../lib/hash
includes    = -I/opt/local/include -I/usr/include/libxml2 
libs        = -lsqlite3 -lxml2 -L/opt/local/lib -lpcre -lcurl
objs        =  all_tests opt tempc

# sets the include path 
vpath %.c ../src ../lib/hash
vpath %.h ../include/

all_tests:

opt: history.o media.o option_parser.o playlist.o string_buffer.o string_util.o time_util.o 
	${compile} ${libs} ${flags} -o $@ $^

tempc: temp.o string_util.o hashtable_itr.o hashtable.o hashtable_utility.o
	${compile} ${libs} ${flags} $? -o $@

# Rules for tests
include ../tests/tests.mk

all_tests: ${tests} ../tests/all_tests.o
	${compile} ${libs} ${flags} $(filter %.o,${^F}) -o $@
	@echo "Built all tests : ${tests}"

test_%: ../tests/%_test.o ../tests/%_helper.o %_wrapper.o
	${compile} ${libs} ${flags}  ${^F} -o $@

# makes main method
%_wrapper.c:
	./make_wrapper.sh "$@" "../tests/include/"


%.o: %.c
	${compile} ${flags} -c $^ -o ${@F}

clean:
	rm -f *.o *.out test_* *_wrapper.c *_wrapper.o  ${objs} *~

