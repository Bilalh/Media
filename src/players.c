#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <include/debug.h>
#include <include/players.h>
#include <prefs.h>

// Makes the command for vlc and mplayer
static char *make_command(const char *bin_path, char **filenames, int num_of_files,
						  int total_length, char *prefix_args, char *postfix_args, 
						  char *filepath, bool background);

/// \brief Filenames should end with "", total length the length of all the strings
/// filepath, to the directory to call mplayer from.
void mplayer(char **filenames, int num_of_files, int total_length, 
			 char *prefix_args, char *postfix_args, char *filepath, bool background) 
{
	
	// mplayer binary
	char *command = make_command(PREFS_MPLAYER_BINARY, filenames, num_of_files, total_length, 
								 prefix_args, postfix_args, filepath, background);
	
	// strdup is needed
	system(strdup(command));
}

void mplayer_gui(char **filenames, int num_of_files, int total_length, char *filepath) 
{
	
	// mplayer binary
	char *command = make_command("open -a " PREFS_MPLAYER_GUI_APP_NAME , filenames, num_of_files, total_length, 
								 "", "", filepath, true);
	
	// strdup is needed
	system(strdup(command));
}

/// \brief Filenames should end with "", total length the length of all the strings
/// filepath, to the directory to call vlc from.	
void vlc(char **filenames, int num_of_files, int total_length, 
		 char *prefix_args, char *postfix_args, char *filepath)
{
	
	char *command = make_command(PREFS_VLC_BINARY, filenames, num_of_files, total_length, 
								 prefix_args, postfix_args, filepath, true);
	
	// strdup is needed
	system(strdup(command));
}

static char *make_command(const char *bin_path, char **filenames, int num_of_files,
						  int total_length, char *prefix_args, char *postfix_args, 
						  char *filepath, bool background)
{
	// discards output.
	const char *rid   = background ? "&> /dev/null &" :"&> /dev/null"; 

	if (prefix_args  == NULL) prefix_args  = "";
	if (postfix_args == NULL) postfix_args = "";

	int index        = strlen(filepath);
	int rid_len      = strlen(rid);
	int m_len        = strlen(bin_path);
	int space_quotes = num_of_files * 3; // for space and 2""

	//CHECK 1 from (8 for mplayer)
	char *m_args = malloc(sizeof(char)* 
			   (total_length + 
			    strlen(prefix_args) + strlen(postfix_args) + 
			    index + rid_len + m_len +
			    space_quotes  + 1
			   ));

	sprintf(m_args, "cd %s; %s %s ", filepath, bin_path, prefix_args);
	// 3 for cd 2 for ; 1 for  .
	index += 3 + 2 + (+ 1 + m_len) + strlen(prefix_args) + 1;

	// append filenames
	while (*filenames != NULL) {
		m_args[index++] = '"';

		strcpy(&m_args[index], *filenames);
		index += strlen(*filenames);

		m_args[index++] = '"';
		m_args[index++] = ' ';
		++filenames;
	}
	strcpy(&m_args[index], postfix_args);
	index += strlen(postfix_args);

	strcpy(&m_args[index], rid);
	index += rid_len;

	m_args[index] = '\0';
	dprintf("%s\n", m_args);
	return m_args;	
}

//LATER niceplayer
void niceplayer(char *playlist) {}
