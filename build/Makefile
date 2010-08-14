CC          = gcc
CFLAGS      = -g -Wall

compile     = ${CC} -std=gnu99 -fblocks
flags       = ${CFLAGS} ${includes} -I../include -I../src
includes    = -I/opt/local/include -I/usr/include/libxml2 
libs        = -lsqlite3 -lxml2 -L/opt/local/lib -lpcre -lcurl
objs        = 

# sets the include path 
vpath %.c ../src 
vpath %.h ../include/

opt: history.o media.o option_parser.o playlist.o string_buffer.o string_util.o time_util.o 
	${compile} ${libs} ${flags} -o $@ $^

# Rules for tests
include ../tests/tests.mk 
# Makes all files required by tests  
test_%: ../tests/%_test.o ../tests/%_helper.o
	${compile} ${libs} ${flags}  ${^F} -o $@

%.o: %.c
	${compile} ${flags} ${DFLAGS}  -c $^ -o ${@F}

clean:
	rm -f *.o *.out test_* ${objs} *~
