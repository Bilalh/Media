CC = gcc -std=c99 -fblocks
CFLAGS = -g -Wall -I ./hashmap -I .
media: main.c media.o string_util.o
	${CC} ${CFlAGS} -o $@ $^

test:  string_util.o tests/string_util_test.o
	${CC} ${CFLAGS} -o $@ $^

tempc: media.o string_util.o temp.o hashmap/hashmap.o
	${CC} ${CFLAGS} -o $@ $^

.o:
	${CC} ${CFLAGS} -c $*.c

clean:
	rm -f *.o *.out media  test tempc *~
