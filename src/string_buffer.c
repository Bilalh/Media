#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include <include/string_buffer.h>
#include <include/debug.h>

String *string_new(size_t length){
	if (length <= 0) length = 1;
	String *s = malloc(sizeof(String));
	new_string(s,length);
	return s;
}

void new_string(String *s, size_t length ){
	assert(s);
	if (length <= 0) length = 1;
	s->str    = malloc(sizeof(char) * length);
	s->str[0] ='\0';
	s->length = length;
	s->index  = 0;
}

// assumes the s->str is assigned by malloc
// adds a space before append the string if add_space
void string_add(String *s, char *str, bool add_space_before) {
	assert(s); assert(str);
	size_t str_len = strlen(str);
	sbdprintf("a ind %i len %i \n", s->index, s->length);
	if ( (s->length - s->index) < str_len +1 ) { // 1 for \0
		sbdprintf("reallocing to %d\n", s->length);
		s->length += str_len * 2 + 2; // 2 for \0 and space
		s->str = realloc(s->str, s->length);
	}
	sbdprintf("b ind %i len %i \n", s->index, s->length);
	if (add_space_before) s->str[s->index++] = ' ';
	strncpy(&s->str[s->index], str, str_len + 1);
	s->index += str_len;
	sbdprintf("'%s'\n", s->str);
}



// assumes the s->str is assigned by malloc
// adds a space before append the string
void string_add_m(String *s, bool add_space, size_t length, ...){
	assert(s);
	// Sets up the variable argument list
	va_list args; 
	va_start(args, length);
	int index = 0;
	while(index < length ){
		char *str = va_arg(args, char *);
		string_add(s,str,add_space);
		index++;
	}
	
	va_end(args); // finished using argument list
}


int string_sprintf(String *s, size_t length,  const char *fmt,  ... ){
	assert(s); assert(fmt);
	va_list args;
	va_start(args, fmt);
	sbdprintf("a '%s'\n", s->str);
	sbdprintf("a ind %i len %i r_len %i \n", s->index, s->length, length);
	if ( (s->length - s->index) < length + 2 ) { // 1 for \0
		s->length += length * 2 + 2; 
		sbdprintf("reallocing to %d\n", s->length);
		s->str = realloc(s->str, s->length);
	}
	sbdprintf("b ind %i len %i \n", s->index, s->length);
	
	int wrote = vsnprintf(&s->str[s->index], length, fmt, args);
	va_end(args);
	
	sbdprintf("c ind %i len %i spf %i \n", s->index, s->length, wrote);
	
	// makes sure there a null at the end
	if (wrote >= length ){
		sbdprintf("tl '%s'\n", s->str);
		if (length <= 1) {
			return wrote;
		}
		sbdprintf("spf %d %d\n",wrote, wrote );
		s->index += wrote - length;
		s->str[s->index] = '\0';
	}else{
		s->index += wrote; // to start \0 next time 
	}
	sbdprintf("c '%s'\n", s->str);
	return wrote;
}


