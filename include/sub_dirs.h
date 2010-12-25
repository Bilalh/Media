#ifndef SUB_DIRS_HEAD
#define SUB_DIRS_HEAD 

// Array of strings with it length and the total number of chars
typedef struct{
	char **str_arr;
	int length;
	int total_length;
} StringsPlusMeta;

/**
 * @brief Get All the files that match the regex, 
 * will look in sub dirs
 *
 * @param dir The starting path
 * @param regex The regex to use for matching
 * @param safe  Use only safe files if true
 * @return a StringsPlusMeta ptr
**/
StringsPlusMeta* get_files_recursive(char *dir, char *regex, bool safe);

/**
 * @brief Get All the files that match the regex
 *
 * @param dir The starting path
 * @param regex The regex to use for matching
 * @param safe  Use only safe files if true
 * @return a StringsPlusMeta ptr
**/
StringsPlusMeta* get_files(char *dir, char *regex, bool safe);



#endif