#ifndef MEDIA_MENU_HH

#include <stdbool.h>

void show_menu(char **filenames, size_t *length, bool free_unused);

void old_show_menu(char **filenames, size_t *length, bool free_unused);

#ifdef MMDEBUG
#define mmprintf(fmt,...) dprintf(fmt, ...) 
#else 
#define mmprintf(fmt,...)
#endif

#endif