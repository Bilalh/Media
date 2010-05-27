CC = gcc -std=c99 -fblocks
CFLAGS = -g -Wall -I . -I ./hash
media: main.c media.o string_util.o
	${CC} ${CFlAGS} -o $@ $^

rif: rif.o hashtable.o hashtable_itr.o
	gcc -g -Wall -O -lm -o rif hashtable.o hashtable_itr.o rif.o

test:  string_util.o tests/string_util_test.o
	${CC} ${CFLAGS} -o $@ $^


tempc:  temp.o hashtable_itr.o hashtable.o
	${CC} ${CFLAGS} -o $@ $^

.o:
	${CC} ${CFLAGS} -c $*.c

clean:
	rm -f *.o *.out media  test tempc *~
