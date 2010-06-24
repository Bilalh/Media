CC        = gcc -std=c99 -fblocks
CFLAGS    = -g -Wall  ${INCLUDES}
INCLUDES  = -I./hash -I/usr/include/libxml2
LIBS      = -lsqlite3 -lxml2 -lcurl
OBJ       = media ml test tempc time
MEDIA_OBJ = main.o media.o string_util.o history.o playlist.o option_parser.o time_util.o

media: ${MEDIA_OBJ}
	${CC} ${LIBS} ${CFlAGS} -o $@ $^

ml: ml.o string_util.o
	${CC} ${LIBS} ${CFlAGS} -o $@ $^

opt: option_parser.o media.o string_util.o history.o time_util.o playlist.o
	${CC} ${LIBS} ${CFLAGS} -o $@ $^

time: time_util.o string_util.o
	${CC} ${CFlAGS} -o $@ $^

test: string_util.o tests/string_util_test.o
	${CC} ${CFLAGS} $? -o $@

tempc: temp.o string_util.o hash/hashtable_itr.o hash/hashtable.o hash/hashtable_utility.o
	${CC} ${CFLAGS} $? -o $@

%.o: %.c
	${CC} ${CFLAGS} -c $<

clean:
	rm -f *.o *.out ${OBJ} *~
