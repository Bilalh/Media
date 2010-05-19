CC = gcc -std=c99 -fblocks
CFLAGS = -g -Wall
media: main.c media.c
	${CC} ${CFlAGS} -o $@ $^

test: test.c cmdline.c
	${CC} ${CFLAGS} -o $@ $^

.o:
	${CC} ${CFLAGS} -c $*.c

clean:
	rm -f *.o *.out media *~
