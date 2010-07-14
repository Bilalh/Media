CC        = gcc -std=gnu99 -fblocks
CFLAGS    = -g -Wall ${INCLUDES}
INCLUDES  = -I./hash -I/usr/include/libxml2
LIBS      = -lsqlite3 -lxml2 -lcurl
OBJ       = media ml test tempc time opt
MEDIA_OBJ = history.o main.o media.o option_parser.o playlist.o string.o string_util.o  time_util.o

media: ${MEDIA_OBJ}
	${CC} ${LIBS} ${CFlAGS} -o $@ $^

ml: ml.o string_util.o
	${CC} ${LIBS} ${CFlAGS} -o $@ $^

opt: history.o media.o option_parser.o playlist.o string.o string_util.o time_util.o 
	${CC} ${LIBS} ${CFLAGS} -o $@ $^

time: time_util.o string_util.o
	${CC} ${CFlAGS} -o $@ $^

test: string_util.o tests/string_util_test.o
	${CC} ${CFLAGS} $? -o $@

ttest: string_util.o time_util.o tests/time_test.o
	${CC} ${CFLAGS} string_util.o time_util.o time_test.o -o $@

tempc: temp.o string_util.o hash/hashtable_itr.o hash/hashtable.o hash/hashtable_utility.o
	${CC} ${CFLAGS} $? -o $@

%.o: %.c
	${CC} ${CFLAGS} -c $<

clean:
	rm -f *.o *.out ${OBJ} *~
