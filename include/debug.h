#define DEBUG

#include <libgen.h>
// written in wrong place for Doxygen to work
#ifndef DEBUG
/**
 * @brief Debug version of printf
 *
 * @param fmt format string
 * @param ... args to printf
 */
#define dprintf(fmt, ...) 
#else

#define dprintf(fmt, ...) \
		printf(" %s:%i%s", basename(__FILE__), __LINE__,"   ");\
		printf(fmt,## __VA_ARGS__)
#endif
