#ifndef STRING_ARRAY_HEAD
#define STRING_ARRAY_HEAD 

typedef struct {
	char **arr;
	int  length;
	int  index;
} StringArray;

/**
 * @brief Creates a New Array of Strings
 *
 * @param length The starting length
 * @return  a pointer to a StringArray
**/
StringArray *string_array_new(int length);
void new_string_array(StringArray *sa, int length );

/**
 * @brief Adds a string to the array
 *
 * @param sa The string object
 * @param str The string that will be copied to the StringArray
**/
void string_array_add(StringArray *sa, const char *str);


#endif