#ifndef MEDIA_MENU_HH

#include <stdbool.h>
#include <include/media_args.h>

/**
 * @brief Shows a menu that allows the user which series to use
 *
 * @param filenames The filename to choose from
 * @param length A Pointer to the length of the filenames
 * @param free_unused free unused filenames (non done yet)
 * @param ma A pointer to args
**/
void show_menu(char **filenames, size_t *length, bool free_unused, MediaArgs *ma);

void old_show_menu(char **filenames, size_t *length, bool free_unused);

#ifdef MMDEBUG
#define mmprintf(fmt,...) dprintf(fmt, ...) 
#else 
#define mmprintf(fmt,...)
#endif

#endif