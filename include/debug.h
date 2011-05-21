
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

// basename can not be used because of corruption 
#define dprintf(fmt, ...) \
		printf("%15s:%14s:%3i%s", __FILE__,__func__, __LINE__,"   ");\
	printf(fmt,## __VA_ARGS__);
#endif
