#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include <include/string_buffer_extra.h>
#include <include/string_buffer.h>
#include <include/debug.h>

#include <sqlite3.h>


void string_mprintf(String *s,  const char *fmt,  ... ){
	assert(s); assert(fmt);
	va_list args;
	va_start(args, fmt);
	char *res = sqlite3_vmprintf(fmt, args);
	va_end(args);
	
	string_append(s,res);
	sqlite3_free(res);
	
}
