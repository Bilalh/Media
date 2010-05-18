void media(char *path);

/**
 * Match string against the extended regular expression in
 * pattern, treating errors as no match.
 *
 * Return 1 for match, 0 for no match.
**/
int match(const char *string, char *pattern);