#!/bin/bash
# prints the commands used with the libs gcc and -L shortened
# if $2 then makes $1 if c then make clean first

if [ -n "$2" ]; then
	if [ "${2::1}" == "c" ]; then
		echo "make clean"
		make clean
	fi
	
	clear &&  echo "make $1" && make ${FLAGS} $1 && ./$1 $3
else

clear && echo "make $1" && make $1 -n | \
sed 's!gcc -std=gnu99 -fblocks!Gcc!g' | \
sed 's!-g -Wall -I/opt/local/include -I/usr/include/libxml2  -I../include -I../src!Inc!g' \
| sed 's!-lsqlite3 -lxml2 -L/opt/local/lib -lpcre -lcurl!Lib!g'

fi
