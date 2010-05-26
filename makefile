CC = gcc -std=c99 -fblocks
CFLAGS = -g -Wall -I ./hash -I .
media: main.c media.o string_util.o
	${CC} ${CFlAGS} -o $@ $^

test:  string_util.o tests/string_util_test.o
	${CC} ${CFLAGS} -o $@ $^

hashtable_itr.o: hash/hashtable_itr.c
	gcc -g -Wall -O -c hash/hashtable_itr.c -o hashtable_itr.o

hashtable.o: hash/hashtable.c
	gcc -g -Wall -O -c hash/hashtable.c -o hashtable.o	


tempc: hashtable.o temp.o
	${CC} ${CFLAGS} -o $@ $^

.o:
	${CC} ${CFLAGS} -c $*.c

clean:
	rm -f *.o *.out media  test tempc *~
