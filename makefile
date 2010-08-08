CC        = gcc -std=gnu99 -fblocks
CFLAGS    = -g -Wall ${INCLUDES}
INCLUDES  = -I./hash -I/usr/include/libxml2 -I/opt/local/include
LIBS      = -lsqlite3 -lxml2 -lcurl -L/opt/local/lib -lpcre
OBJ       = media ml test tempc time opt ttime tstring
MEDIA_OBJ = history.o main.o media.o option_parser.o playlist.o string.o string_util.o  time_util.o

media: ${MEDIA_OBJ}
	${CC} ${LIBS} ${CFlAGS} -o $@ $^

ml: ml.o string_util.o
	${CC} ${LIBS} ${CFlAGS} -o $@ $^

opt: history.o media.o option_parser.o playlist.o string.o string_util.o time_util.o 
	${CC} ${LIBS} ${CFLAGS} -o $@ $^

time: time_util.o string_util.o
	${CC} ${LIBS} ${CFlAGS} -o $@ $^

tstring: string_util.o tests/string_util_test.o
	${CC} ${CFLAGS} $? -o $@

ttime: string_util.o time_util.o tests/time_test.o tests/time_helper.o
	${CC} ${LIBS} ${CFLAGS}  string_util.o time_util.o time_test.o time_helper.o -o $@

btest: tests/block_test.o
	${CC} ${CFLAGS} block_test.o -o $@

tempc: temp.o string_util.o hash/hashtable_itr.o hash/hashtable.o hash/hashtable_utility.o
	${CC} ${CFLAGS} $? -o $@

# get with of the main method of each test
tall: DFLAGS += -DALL_TESTS
tall: ttime

%.o: %.c
	echo ${DFLAGS}
	${CC} ${CFLAGS} ${DFLAGS}  -c $<

clean:
	rm -f *.o *.out ${OBJ} *~
