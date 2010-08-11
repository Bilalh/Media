#ifndef STRING_STURCT_HEAD
#define STRING_STURCT_HEAD

#include "stdbool.h"

typedef struct {
	char *str;
	int length;
	int index;
} String;


void new_string(String *s, int length );
void string_add(String *s, char *str, bool add_space);
void string_add_m(String *s, bool add_space, int length, ...);

// add the space before adding the string
#define string_push_m(s,length, ...) string_add_m(s, true, length, __VA_ARGS__)
#define string_push(s,str)           string_add(s, str, true)

// does not add the space before the string
#define string_append_m(s,length, ...) string_add_m(s, false, length, __VA_ARGS__)
#define string_append(s,str)           string_add(s, str, false)


#endif