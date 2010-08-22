#ifndef STRINGUTILHEAD
#define STRINGUTILHEAD
#include <stddef.h>


/**
 * @brief return the char before the number and the last char before after the text
 *
 * @param s - string with a number at the end.
 * @return The number at the end
 */
char** ep_num(char *s);

/**
 * @brief Match string against the extended regular expression in
 * pattern, treating errors as no match.
 *
 * @param string
 * @param pattern - perl regex
 * @return Non zero on match (1?), 0 on any error
 */
int match(const char *string, char *pattern);

/**
 * @brief Replaces the substring sub with rep in the first len charaters of s.
 *
 * @param s
 * @param len
 * @param sub
 * @param rep
 * @return
 */
char *str_replace(char *s, size_t len,  char *sub, char *rep);

/**
 * @brief Concatenates the array with separator and adds the ending to the end
 *
 * @param s
 * @return
 */
char *str_spilt_replace(char *s);

/**
 * @brief Concatenates the array with separator and adds
 * the ending to the end
 *
 * @param arr
 * @param length
 * @param separator
 * @return
 */
char *spilt_args(char **arr, size_t length, char *separator, char *ending );

typedef struct {
	char **args;
	int  length;
	int  *lengths;
	int  total;
} SpiltData;

SpiltData *str_spilt_func(char *s);

char *str_lower(char *s, size_t length);

#endif
