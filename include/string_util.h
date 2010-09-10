#ifndef STRINGUTILHEAD
#define STRINGUTILHEAD
#include <stddef.h>
#include <stdbool.h>
#include "uthash.h"

/**
 * @brief Places the number in var
 *
 * @param ep_num_char char** returned by ep_num
 * @param var name of The variable
**/
#define EP_GET_NUMBER(ep_num_char, var) \
	long var = strtol(ep_num_char[0] + 1, NULL, 10);\
	if (var == 0 ) var++;

/**
 * @brief Places the name in var
 *
 * @param ep_num_char char** returned by ep_num
 * @param var Name of the variable
 * @param orginal_string The string 
**/
#define EP_GET_NAME(ep_char, var, orginal_string)\
	char var[ ep_char[EP_NUM_INDEX(ep_char)] - orginal_string + 1]; \
	strncpy(var, orginal_string, ep_char[EP_NUM_INDEX(ep_char)] - orginal_string);\
	var[ep_char[EP_NUM_INDEX(ep_char)] - orginal_string] = '\0';

#define EP_NUM_INDEX(ep_num_char) (ep_num_char[1] != NULL ? 1 : 0)


/**
 * @brief Gets the postion of 
 *
 * @param s - string with a number at the end.
 * @return [0] - The index before the number starts
 *         [1] - ans[index - NAME + 1] gives the length of the name part
 *               where index = 0 unless [1] is not null
 *         [0] == NULL - Number not found
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
 * @brief Spilts a string into array of strings 
 *
 * @param str The string to split
 * @param res_length a int pointer to hold the length. 
 * (can be used to set the starting length)
 * @param malloces - true uses malloc for spilt strings 
 * @return A array of strings
**/
char** spilt_string_malloc (char *str, int *res_length, bool malloces);

// malloces false
char** spilt_string(char *str, int *res_length);
// malloces true
char** spilt_string_m (char *str, int *res_length);

/**
 * @brief strcmp with null checking
 *
 * @return true if they are equal or both null otherwise false
**/
bool strcmp_null(const char *s1, const char *s2 );

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
	char **args; ///< args
	int  length;
	int  *lengths;
	int  total;
} SpiltData;


/**
 * @brief 
 *
 * @param s 
 * @return 
**/
SpiltData *str_spilt_func(char *s);

typedef struct  {
	char *key;
	char *val;
	UT_hash_handle hh;
} Mapping;

Mapping *load_hash(const char *filename);


/**
 * @brief 
 *
 * @param s 
 * @param length 
 * @return 
**/
char *str_lower(char *s, size_t length);

#endif
