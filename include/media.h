#ifndef MEDIAHEAD
#define MEDIAHEAD
#include <stdbool.h>
#include "media_args.h"

void media(char *path, char **args,int argc, MediaArgs *opts);

void mplayer(char **filenames, int num_of_files, int total_length, char *prefix_args, char *postfix_args, char *filepath);
void niceplayer(char *playlist);

void vlc(char **filenames, int total_length, char *prefix_args, char *postfix_args, char *filepath);

#endif
