#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

void string_push(String *s, char *str) {
	int str_len = strlen(str);

	printf("a in %i len %i \n", s->index, s->length);
	if ( (s->length - s->index) < str_len +1 ) { // 1 for \0
		s->length += str_len * 2 + 2; // 2 for /0 and space
		s->str = realloc(s->str, s->length);
	}
	printf("b in %i len %i \n", s->index, s->length);

	s->str[s->index++] = ' ';
	strncpy(&s->str[s->index], str, str_len + 1);
	s->index += str_len;
	printf("'%s'\n", s->str);
}
