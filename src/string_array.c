#include <stdlib.h>
#include <string.h>
#include <include/string_array.h>


StringArray *string_array_new(int length){
	
	if (length <= 0) length = 16;
	StringArray *sa = malloc(sizeof(StringArray));
	new_string_array(sa,length);
	return sa;
}

void new_string_array(StringArray *sa, int length ){
	
	if (length <= 0) length = 16;
	sa->arr    = malloc(sizeof(char*) * length);
	sa->length = length;
	sa->index  = 0;
}

void string_array_add(StringArray *sa, char *str){
	
	if (sa->index == sa->length){
		sa->length *=2;
		sa = realloc(sa, sizeof(char*) * sa->length);
	}
	
	sa->arr[sa->index++] = strdup(str);
}