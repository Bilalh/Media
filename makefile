CC     = gcc -std=c99 -fblocks
CFLAGS = -g -Wall  ${INCLUDES}
INCLUDES = -I./hash -I/usr/include/libxml2
LIBS   = -lsqlite3 -lxml2
media: main.c media.o string_util.o history.o playlist.o
	${CC} ${LIBS} ${CFlAGS} -o $@ $^


test: string_util.o tests/string_util_test.o
	${CC} ${CFLAGS} $? -o $@

tempc: temp.o string_util.o hash/hashtable_itr.o hash/hashtable.o hash/hashtable_utility.o
	${CC} ${CFLAGS} $? -o $@

%.o: %.c
	${CC} ${CFLAGS} -c $<

clean:
	rm -f *.o *.out media  test tempc *~
