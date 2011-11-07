#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <unistd.h>
#include <include/debug.h>
#include <include/players.h>
#include <prefs.h>

// Makes the command for vlc and mplayer
static char *make_command(const char *bin_path, char **filenames, size_t num_of_files,
						  size_t total_length, char *prefix_args, char *postfix_args, 
						  char *filepath, bool background, bool output_file);

/// \brief Filenames should end with "", total length the length of all the strings
/// filepath, to the directory to call mplayer from.
void mplayer(char **filenames, size_t num_of_files, size_t total_length, 
			 char *prefix_args, char *postfix_args, char *filepath, bool background, bool output_file) 
{
	// mplayer binary
	char *command = make_command(PREFS_MPLAYER_BINARY, filenames, num_of_files, total_length, 
								 prefix_args, postfix_args, filepath, background,output_file);
	// printf("%s\n", command);
	// strdup is needed
	system(strdup(command));
	
	// exec
	// chdir(filepath);
	// execvp( PREFS_MPLAYER_BINARY_UN_Q, "mplayer", filenames );
}

void mplayer_gui(char **filenames, size_t num_of_files, size_t total_length, char *filepath) 
{
	// mplayer binary
	char *command = make_command("open -a " PREFS_MPLAYER_GUI_APP_NAME , filenames, num_of_files, total_length, 
								 "", "", filepath, true, false);
	
	// strdup is needed
	system(strdup(command));
}

/// \brief Filenames should end with "", total length the length of all the strings
/// filepath, to the directory to call vlc from.	
void vlc(char **filenames, size_t num_of_files, size_t total_length, 
		 char *prefix_args, char *postfix_args, char *filepath)
{
	char *command = make_command(PREFS_VLC_BINARY, filenames, num_of_files, total_length, 
								 prefix_args, postfix_args, filepath, true, false);
	
	// strdup is needed
	system(strdup(command));
}

static char *make_command(const char *bin_path, char **filenames, size_t num_of_files,
						  size_t total_length, char *prefix_args, char *postfix_args, 
						  char *filepath, bool background, bool output_file)
{
	assert(bin_path); assert(filenames); assert(prefix_args);
	assert(postfix_args); assert(filepath); 
	
	// discards output.
	// const char *rid   = background ? "&> /dev/null &" :"&> /dev/null"; 

	const char *rid   = background ? "&" :""; 
	const char *out   = output_file ? (!background ? " 2>&1  | tee ~/.mplayer/output | grep '# '"\
	 								: "2>/dev/null | tee ~/.mplayer/output &>/dev/null" ) : "";

	if (prefix_args  == NULL) prefix_args  = "";
	if (postfix_args == NULL) postfix_args = "";

	size_t index              = strlen(filepath);
	const size_t rid_len      = strlen(rid);
	const size_t m_len        = strlen(bin_path);
	const size_t space_quotes = num_of_files * 3; // for space and 2""
	const size_t out_len      = output_file ? strlen(out) : 0;

	//CHECK 1 from (8 for mplayer)  3 for  '-- '
	char *m_args = malloc(sizeof(char)* 
			   (total_length + 
			    strlen(prefix_args) + strlen(postfix_args) + 
			    index + rid_len + m_len +
			    space_quotes  + 1 + 3 +
				out_len+2
			   ));

	sprintf(m_args, "cd %s; %s %s -- ", filepath, bin_path, prefix_args);
	// 3 for cd 2 for ; 1 for  .
	index += 3 + 2 + (+ 1 + m_len) + strlen(prefix_args) + 1 + 3;

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

	if (output_file){
		strcpy(&m_args[index], out);
		index += out_len;
	}

	strcpy(&m_args[index], rid);
	index += rid_len;


	m_args[index] = '\0';
	dprintf("%s\n", m_args);
	return m_args;	
}

//LATER niceplayer
void niceplayer(char *playlist) {}
