#include <sys/types.h>
/**
 * Match string against the extended regular expression in
 * pattern, treating errors as no match.
 *
 * non zero on match, 0 on any error
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
