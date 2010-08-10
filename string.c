#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "string.h"


void new_string(String *s, int length ){
	if (length <= 0) length = 1;
	s->str    = malloc(sizeof(char) * length);
	s->str[0] ='\0';
	s->length = length;
	s->index  = 0;
}

// assumes the s->str is assigned by malloc
void string_push(String *s, char *str) {
	int str_len = strlen(str);

	printf("a in %i len %i \n", s->index, s->length);
	if ( (s->length - s->index) < str_len +1 ) { // 1 for \0
		s->length += str_len * 2 + 2; // 2 for \0 and space
		s->str = realloc(s->str, s->length);
	}
	printf("b in %i len %i \n", s->index, s->length);

	s->str[s->index++] = ' ';
	strncpy(&s->str[s->index], str, str_len + 1);
	s->index += str_len;
	printf("'%s'\n", s->str);
}

// assumes the s->str is assigned by malloc
void string_push_m(String *s,int length, ...){
	// Sets up the variable argument list
	va_list args; 
	va_start(args, length);
	int index = 0;
	while(index < length ){
		char *str = va_arg(args, char *);
		string_push(s,str);
		index++;
	}
	
	va_end(args); // finished using argument list
}