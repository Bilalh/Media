#include <sys/types.h>
/**
 * Match string against the extended regular expression in
 * pattern, treating errors as no match.
 *
 * non zero on match, 0 on any error
**/
int match(const char *string, char *pattern);


char *str_replace(char *s, size_t len,  char *sub, char *rep);

char *strrep1(char *s, char *sub, char *rep);