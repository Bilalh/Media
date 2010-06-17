#ifndef STRING_UTILHEAD
#define STRING_UTILHEAD
#include <stddef.h>


char** ep_num(char *s);

/**
 * Match string against the extended regular expression in
 * pattern, treating errors as no match.
**/
int match(const char *string, char *pattern);

char *str_replace(char *s, size_t len,  char *sub, char *rep);

char *str_spilt_replace(char *s);

char *spilt_args(char **arr, int length, char *separator, char *ending );

typedef struct {
	char **args;
	int  length;
	int  *lengths;
	int  total;
} SpiltData;

SpiltData *spilt_func(char *s);

#endif
