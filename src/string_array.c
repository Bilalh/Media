#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <include/string_array.h>


ArrayList *new_arraylist(int length){
	
	if (length <= 0) length = 16;
	ArrayList *sa = malloc(sizeof(ArrayList));
	arraylist_init(sa,length);
	return sa;
}

void arraylist_init(ArrayList *sa, int length ){
	assert(sa);
	if (length <= 0) length = 16;
	sa->arr    = malloc(sizeof(char*) * length);
	sa->length = length;
	sa->index  = 0;
}

void arraylist_string_add(ArrayList *sa, const char *str){
	arraylist_add(sa,strdup(str));
}	


void arraylist_add(ArrayList *sa, void *const ptr){
	assert(sa);
	
	if (sa->index + 1 == sa->length){
		sa->length *=2;
		sa->arr = realloc(sa->arr, sizeof(char*) * sa->length);
	}
	
	sa->arr[sa->index++] = ptr;
}	
