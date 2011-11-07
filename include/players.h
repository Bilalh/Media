#ifndef PLAYER_MEDIA_H
#define PLAYER_MEDIA_H

#include <stdbool.h>

/// \brief Filenames should end with "", total length the length of all the strings
/// filepath, to the directory to call mplayer from.
void mplayer(char **filenames, size_t num_of_files, size_t total_length, 
	         char *prefix_args, char *postfix_args, char *filepath, bool background, bool output_file);

/// \brief Filenames should end with "", total length the length of all the strings
/// filepath, to the directory to call vlc from.
void vlc(char **filenames, size_t num_of_files, size_t total_length, 
	     char *prefix_args, char *postfix_args, char *filepath);

void mplayer_gui(char **filenames, size_t num_of_files, size_t total_length, char *filepath);

void niceplayer(char *playlist);

#endif