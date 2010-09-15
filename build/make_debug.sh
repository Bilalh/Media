#!/bin/bash
# prints the commands used with the libs gcc and -L shortened

if [ "$1" = "clear" ]; then
	export CLEARNNN="clear && "
	exit
elif [ "$1" = "noclear" ]; then
	export CLEARNNN=" "
	exit
fi

if [[ "$1" == "-h" || "$2" == "-h"  ]]; then
	echo "    c  - make clean first"
	echo "    *  - make and run"
	echo "    b  - build only"
	echo "   ''  - make -n "
	exit
fi

if [ -n "$2" ]; then
	
	if [  "${2::1}" == "a" ]; then
		echo "make default"
		make all_tests 
		./all_tests $3
		exit
	fi
	
	if [  "${2::1}" == "c" ]; then
		echo "make clean"
		make clean
	fi
	
	if [  "${2::1}" == "d" ]; then
		echo "make clean"
		make clean
		exit
	fi
	
	${CLEARNNN} echo "make $1" && make ${FLAGS} $1
	
	if [[ $? == 0 &&  "${2::1}" != "b" ]]; then
		res=$1
		shift; shift;
		./$res $*
	fi
	
else

${CLEARNNN} && echo "make $1" && make $1 -n | \
sed 's!gcc -std=gnu99 -fblocks!Gcc!g' | \
sed 's!-I ../lib/hash!!g' | \
sed 's!-g -Wall -I/opt/local/include -I/usr/include/libxml2!Inc!g' \
| sed 's!-I../src -I../!!g' \
| sed 's!-lsqlite3 -lxml2 -L/opt/local/lib -lpcre -lcurl!Lib!g'

fi
