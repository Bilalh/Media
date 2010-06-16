CC = gcc -std=c99 -fblocks
CFLAGS = -g -Wall -I . -I ./hash
media: main.c media.o string_util.o history.o
	${CC} -l sqlite3 ${CFlAGS} -o $@ $^

str: string_util.o
	${CC} ${CFlAGS} -o $@ $^

test: string_util.o tests/string_util_test.o
	${CC} ${CFLAGS} -o $@ $^

tempc: temp.o string_util.o hash/hashtable_itr.o hash/hashtable.o hash/hashtable_utility.o
	${CC} ${CFLAGS} -o $@ $^

.o:
	${CC} ${CFLAGS} -c $*.c

clean:
	rm -f *.o *.out media  test tempc *~
