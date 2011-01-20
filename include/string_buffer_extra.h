#ifndef STRING_BUFFER_EXTRA_HEADER
#define STRING_BUFFER_EXTRA_HEADER

#include "string_buffer.h"

/**
 * @brief  Use sqlite3's mprintf on the String 
 *
 * @param s - The String object to copy to.
 * @param fmt - The format string.
**/
void string_mprintf(String *s, const char *fmt,  ... );

#endif