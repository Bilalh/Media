#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <pcre.h>
#include <Block.h>

#include <include/colours.h>
#include <include/debug.h>
#include <include/history.h>
#include <include/media.h>
#include <include/players.h>
#include <include/playlist.h>
#include <include/string_util.h>
#include <include/sub_dirs.h>
#include <include/time_regex.h>

#include <prefs.h>


#define DIRENT(value) (*(struct dirent **) value)
// #define VIDEO  ".*\\.(mp4)$"


void media(char *path, char **args, int argc, const MediaArgs *ma) {
	
	assert(path); assert(ma); assert(args);
	
	char *dot_default[] = {"."};  
	
	if (argc == 0) {
		if (ma->dot_default){
			argc = 1;
			args = dot_default;
		}else{
			efprintf("%s\n", "No file args (use . for all files)");
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
		case T_VIDEO: types = ")"PREFS_VIDEO_REGEX;
			break;
		case T_AUDIO: types = ")"PREFS_AUDIO_REGEX;
			break;
		case T_BOTH : types = ")"PREFS_AUDIO_VIDEO_REGEX;
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
			
			char **ep_num_ans =  NULL;
			if (ma->colour_ep){
				ep_num_ans = ep_num(s_arr[i]);
			}
				
			if (ma->colour_ep){
				if (ep_num_ans[0] != NULL){
					EP_GET_NUMBER(ep_num_ans,num);
					char buff[20+3]; //  .   0 at start and \0
					// LATER check for 001 
					// ? for highlighting 01 
					sprintf(buff,"%s%ld", *(ep_num_ans[0]+1) == '0' ? "0" : "", num);
					
					int length = strlen(buff);
					char rep[length + strlen(RESET)*2 + strlen(PREFS_EP_COLOUR) + 1];
					sprintf(rep, SSS("%s") ".", COLOUR(buff,PREFS_EP_COLOUR));

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
			case P_MPLAYER_GUI:
				mplayer_gui(s_arr,file_num, total_length, path);
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
		" -e 'tell application " PREFS_MPLAYER_APP_NAME " to activate' " \
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
	
		if(ma->write_history && ma->label_watched){
			const char* set_label = "hide_extension.applescript";
			int len_len           = strlen(set_label);
			int path_len          = strlen(path);
			int colour_len        = strlen(PREFS_WATCHED_COLOUR);
			for(int i = 0; i < file_num; ++i){
				char args[strlen(s_arr[i]) + len_len + path_len + colour_len + 5 + 11+ 1 ];
				sprintf(args, "SetLabel %s \"%s/%s\" &>/dev/null", PREFS_WATCHED_COLOUR, path, s_arr[i] );
				system(args);
				sprintf(args, "hide_extension.applescript \"%s/%s\" &>/dev/null", path, s_arr[i] );
				system(args);
			}
			
		}
		
		if (ma->score > 0){
			for(int i = 0; i < file_num; ++i){
				set_score(s_arr[i], ma->score);
			}
		}
		
	}
	
}

