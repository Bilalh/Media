#ifndef STRING_ARRAY_HEAD
#define STRING_ARRAY_HEAD 

typedef struct {
	void **arr;
	int  length;
	int  index;
} ArrayList;

/**
 * @brief Creates a New Array of Strings
 *
 * @param length The starting length
 * @return  a pointer to a StringArray
**/
ArrayList *new_arraylist(int length);
void arraylist_init(ArrayList *sa, int length );

/**
 * @brief Adds a string to the array
 *
 * @param sa The string object
 * @param str The string that will be copied to the StringArray
**/
void arraylist_string_add(ArrayList *sa, const char *str);

void arraylist_add(ArrayList *sa, void * const ptr);


#endif