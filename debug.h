// #define DEBUG
#ifdef DEBUG
#define dprintf(fmt, ...) \
		printf("%s:%i%s", __FILE__, __LINE__,"   ");\
		printf(fmt, __VA_ARGS__)

#else
#define dprintf(fmt, ...) ;
#endif
