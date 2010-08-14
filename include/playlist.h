#ifndef PLAYLISTHEAD
#define PLAYLISTHEAD
#include <stdbool.h>
#include "media.h"

bool make_playlist(char *filename,char *dir, char **names, Pformat playtypes);

bool make_m3u(char *filepath, char **names);

bool make_plist(char *filepath, char **names);

bool make_pls(char *filepath, char **names);

bool make_xspf(char *filepath, char **names);

#endif
