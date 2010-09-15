#ifndef STRING_STURCT_HEAD
#define STRING_STURCT_HEAD

#include <stdbool.h>

typedef struct {
	char *str;
	int length;
	int index;
} String;


/**
 * @brief Creates new String Objec with index 0. 
 * The char pointer is asigned by malloc
 *
 * @param s A pointer to a String
 * @param length The starting length
 */
void new_string(String *s, int length);
String *string_new(int length);

/**
 * @brief Copies the char array into the String
 *
 * @param s The String object to copy to
 * @param str The array to copy from
 * @param add_space_before If true a space is added by before
 */
void string_add(String *s, char *str, bool add_space_before);
/**
 * @brief Copies the char array into the String
 *
 * @param s The String object to copy to
 * @param add_space_before If true a space is added by before
 * @param length the number of char array to copy 
 * @param ... the char arrays
 */
void string_add_m(String *s, bool add_space, int length, ...);


/**
 * @brief  Use sprintf on the String 
 *
 * @param s - The String object to copy to.
 * @param length - The length required (inculde \0).  
 *                 A length <=1 means that no data is written 
 * @param fmt - The format string.
 * @return the return value of sprintf
**/
int string_sprintf(String *s, int length,  const char *fmt,  ... );

// add the space before adding the string
#define string_push_m(s,length, ...) string_add_m(s, true, length, __VA_ARGS__)
#define string_push(s,str)           string_add(s, str, true)

// does not add the space before the string
#define string_append_m(s,length, ...) string_add_m(s, false, length, __VA_ARGS__)
#define string_append(s,str)           string_add(s, str, false)


#endif
