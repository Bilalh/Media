CC        = gcc -std=gnu99 -fblocks
CFLAGS    = -g -Wall ${INCLUDES}
INCLUDES  = -I./hash -I/usr/include/libxml2 -I/opt/local/include
LIBS      = -lsqlite3 -lxml2 -lcurl -L/opt/local/lib -lpcre
OBJ       = media ml test tempc opt
MEDIA_OBJ = history.o main.o media.o option_parser.o playlist.o string.o string_util.o  time_util.o

media: ${MEDIA_OBJ}
	${CC} ${LIBS} ${CFlAGS} -o $@ $^

ml: ml.o string_util.o
	${CC} ${LIBS} ${CFlAGS} -o $@ $^

opt: history.o media.o option_parser.o playlist.o string.o string_util.o time_util.o 
	${CC} ${LIBS} ${CFLAGS} -o $@ $^


# makes all tests
all_tests: test_time
# get rid of the main method of each test
run_all_test: DFLAGS += -DALL_TESTS
run_all_test: all_tests

test_time: string_util.o time_util.o  tests/time_test.o tests/time_helper.o
	${CC} ${LIBS} ${CFLAGS}  $^ -o $@

test_string: string.o tests/string_test.o tests/string_helper.o
	${CC} ${LIBS} ${CFLAGS}  $^ -o $@


test_block: tests/block_test.o
	${CC} ${CFLAGS} block_test.o -o $@

tempc: temp.o string_util.o hash/hashtable_itr.o hash/hashtable.o hash/hashtable_utility.o
	${CC} ${CFLAGS} $? -o $@

%.o: %.c
	${CC} ${CFLAGS} ${DFLAGS}  -c $^ -o $@

clean:
	rm -f *.o *.out test_* ${OBJ} *~
