#ifndef PLAYER_MEDIA_H
#define PLAYER_MEDIA_H

#include <stdbool.h>

/// \brief Filenames should end with "", total length the length of all the strings
/// filepath, to the directory to call mplayer from.
void mplayer(char **filenames, int num_of_files, int total_length, 
	char *prefix_args, char *postfix_args, char *filepath, bool background);

/// \brief Filenames should end with "", total length the length of all the strings
/// filepath, to the directory to call vlc from.
void vlc(char **filenames, int num_of_files, int total_length, 
	char *prefix_args, char *postfix_args, char *filepath);

void niceplayer(char *playlist);

#endif