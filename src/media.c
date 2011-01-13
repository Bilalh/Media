#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <pcre.h>
#include <Block.h>

#include <include/history.h>
#include <include/media.h>
#include <include/playlist.h>
#include <include/string_util.h>
#include <include/time_regex.h>
#include <include/debug.h>
#include <include/sub_dirs.h>
#include <include/colours.h>

#define DIRENT(value) (*(struct dirent **) value)
// #define VIDEO  ".*\\.(mkv|mp4|avi)$"
#define VIDEO   "(.*)?\\.(mkv|mp4|mov|avi|ogm|divx|rm|rmvb|flv|part|wmv)$"
#define AUDIO   "(.*)?\\.(mp3|m4a|flac|ogg|m4b|aiff|ac3|aac|wav|wmv|ape)$"
#define VID_AUD "(.*)?\\.(mkv|mp4|mp3|m4a|mov|avi|flac|ogm|ogg|aiff|divx|rm|rmvb|flv|part|wmv|ac3|aac|wav|wmv|ape)$"

// Makes the command for vlc and mplayer
static char *make_command(const char *bin_path, char **filenames, int num_of_files,
						  int total_length, char *prefix_args, char *postfix_args, 
						  char *filepath, bool background);

void media(char *path, char **args, int argc, const MediaArgs *ma) {
	
	char *dot_default[] = {"."};  
	
	if (argc == 0) {
		if (ma->dot_default){
			argc = 1;
			args = dot_default;
		}else{
			fprintf(stderr, "%s\n", "NO file args (use . for all files)");
			exit(2);
		}
	}
	
	#ifndef DEBUG
	for(int i = 0; i < argc; ++i){
		dprintf("args[%i]%s\n",i, args[i]);
	}
	#endif
	
	char *types = "";
	switch (ma->types){
		case T_VIDEO: types = ")"VIDEO;
			break;
		case T_AUDIO: types = ")"AUDIO;
			break;
		case T_BOTH : types = ")"VID_AUD;
			break;
		case T_NONE : types = ")";
			break;
	}
	char *hash_location = ma->use_hash ? ma->hash_location : "";
	char *regex = spilt_args(args, argc, ma->regex_sep, "(", types, hash_location);
	dprintf("regex: %s\n", regex);
	if(ma->regex_print) printf("regex: %s\n", regex);
	
	StringsPlusMeta* (*get_files_func)(char *dir, char *regex, bool safe);
	if (ma->sub_dirs){
		get_files_func = get_files_recursive;
	}else{
		get_files_func = get_files;
	}
	
	StringsPlusMeta *sta = get_files_func(path, regex, ma->safe);
	
	if (sta == NULL){
		puts("NO files found");
		exit(EXIT_FAILURE);
	}
	
	// so that the code does not have to be changed
	char **s_arr = sta->str_arr;
	int file_num = sta->length;
	int total_length = sta->total_length;
	
	if(ma->newest_only){
		s_arr = newest_only(s_arr, &file_num, true, true);
		file_num--;
	}
	
	if(ma->pl_shuffle) {
		shuffle((void**) s_arr, file_num);
	}

	if(ma->pl_reverse) {
		reverse((void**) s_arr, file_num);
	}
	
	if(ma->pl_output & PL_STDOUT){
		for(int i = 0; i < file_num; ++i){
			
			char *print = strdup(basename(s_arr[i]));
			
			if (ma->colour_ep){
				char **ep_num_ans = ep_num(s_arr[i]);
				if (ep_num_ans[0] != NULL){
					EP_GET_NUMBER(ep_num_ans,num);
					char buff[20+3]; //  . 0 at start and \0
					// ? for highlighting 01 
					// LATER check for 001 
					sprintf(buff,"%s%ld", *(ep_num_ans[0]+1) == '0' ? "0" : "", num);
					
					int length = strlen(buff);
					char rep[length + strlen(RESET)*2 + strlen(BLUE) + 1];
					sprintf(rep, SSS("%s") ".", COLOUR(buff,BLUE));

					// so that we only match the ep_num
					buff[length] = '.';
					buff[length+1] = '\0';

					print = str_replace(print, strlen(print), buff, rep);
				}
			}
			
			printf("%s\n", print);
		}
	}

	if(ma->write_history) updateHistory(s_arr, ma->status, ma->sep);
	if(ma->pl_output & PL_PLAYLIST) {
		make_playlist(ma->pl_name, ma->pl_dir, s_arr, ma->pl_format);
	}
	// Forks to allow the progam to set the options of the player e.g. afloat
	pid_t pid =  fork();
	if ( pid != 0 ){
		switch (ma->player){
			case P_MPLAYER: 
				mplayer(s_arr,file_num, total_length, ma->prefix_args.str, ma->postfix_args.str, 
				path,ma->background);
				break;
			case P_NICEPLAYER:
				niceplayer("");
				break;
			case P_VLC:
				vlc(s_arr,file_num, total_length, ma->prefix_args.str, ma->postfix_args.str, path);
				break;
			case P_NONE: break;
		}
	}else{
		
		#define all_afloat \
		" -e 'tell application \"mplayer-pigoz.mpBinaries\" to activate' " \
		" -e 'tell application \"Afloat Scripting\" to set"                \
		" topmost window kept afloat to true without badge shown'"         
		
		#define all_spaces_manual \
		" -e 'tell application \"System Events\"'"                         \
		" -e 'keystroke (ASCII character of 32)'"                          \
		" -e 'keystroke \"f\" using {command down, control down}'"         
		
		#define all_spaces_auto \
		all_spaces_manual                                                                  \
		" -e 'delay 0.3'"                                                                  \
		" -e 'tell window \"Afloat — Adjust Effects\" of application process \"mplayer\"'" \
		" -e 'click checkbox \"Keep this window on the screen on all Spaces\"'"            \
		" -e 'end tell' -e 'keystroke \"f\" using {command down, control down}'"           \
		" -e 'keystroke (ASCII character of 32)' -e 'end tell'"                           
				
		if (ma->afloat && ma->all_spaces == SPACES_MANUAL ){
			sleep(1);
			system("osascript " all_afloat all_spaces_manual " -e 'end tell'");
		}else if (ma->afloat && ma->all_spaces == SPACES_AUTO ){
			sleep(1);
			system("osascript " all_afloat all_spaces_auto);
		}else if (ma->afloat){
			sleep(1);
			system("osascript " all_afloat);
		}else if (ma->all_spaces== SPACES_MANUAL){
			sleep(1);
			system("osascript " all_spaces_manual);
		}else if (ma->all_spaces== SPACES_AUTO){
			sleep(1);
			system("osascript " all_spaces_auto);
		}
	}
	
}

/// \brief Filenames should end with "", total length the length of all the strings
/// filepath, to the directory to call mplayer from.
void mplayer(char **filenames, int num_of_files, int total_length, 
			 char *prefix_args, char *postfix_args, char *filepath, bool background) {
	
	// mplayer binary
	const char* mplayer = "\"/Users/bilalh/Library/Application Support/MPlayer OSX Extended/Binaries/mplayer-pigoz.mpBinaries/Contents/MacOS/mplayer\"";
	char *command = make_command(mplayer, filenames, num_of_files, total_length, 
								 prefix_args, postfix_args, filepath, background);
	system(strdup(command));
}

/// \brief Filenames should end with "", total length the length of all the strings
/// filepath, to the directory to call vlc from.	
void vlc(char **filenames, int num_of_files, int total_length, 
		 char *prefix_args, char *postfix_args, char *filepath) {
	
	// vlc binary
	const char* vlc = "\"/Applications/VLC.app/Contents/MacOS/VLC\"";
	char *command = make_command(vlc, filenames, num_of_files, total_length, 
								 prefix_args, postfix_args, filepath, true);
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

//TODO niceplayer
void niceplayer(char *playlist) {
	
}