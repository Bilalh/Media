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

#define DIRENT(value) (*(struct dirent **) value)
// #define VIDEO  ".*\\.(mkv|mp4|avi)$"
#define VIDEO  ".*\\.(mkv|mp4|mov|avi|ogm|divx|rm|rmvb|flv|part|wmv)$"
#define AUDIO  ".*\\.(mp3|m4a|flac|ogg|m4b|aiff|ac3|aac|wav|wmv|ape)$"
#define VID_AUD ".*\\.(mkv|mp4|mp3|m4a|mov|avi|flac|ogm|ogg|aiff|divx|rm|rmvb|flv|part|wmv|ac3|aac|wav|wmv|ape)$"

//TODO sub dirs 
void media(char *path, char **args, int argc, const MediaArgs *ma) {
	
	if (argc == 0) {
		fprintf(stderr, "%s\n", "NO file args (use . for all files)");
		exit(2);
	}
	
	struct dirent **files;
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
	
	// gets dir listing ignoring case and matching the patten
	int file_num = scandir_b( path, &files,
	^ (struct dirent * s) {
		MAKE_REGEX_OPTS(at, regex,PCRE_CASELESS,);
		int res = MATCH_REGEX(at, s->d_name, strlen(s->d_name));
		if (ma->safe && res > 0 ){
			MAKE_REGEX_OPTS(safe_r, 
				"Haruhi|Mai-(Otome|Hime)|K-on|Kaichou wa|Hime Chen! Otogi|gundam 00 S2 op 2|"
				"aw.mp4|Ar tonelico|11 eyes|Disgaea 3|Kampfer|fantasy|To ?aru Majutsu no Index|Princess |"
				"Nogizaka Haruka| eng|Sora no Woto Op 2|Saki ed|moon|Myself;|Kiddy Girl-and|Turn a moon|"
				"Rave|Dragonaut The Resonance|Shining Tears|Sakuranbo K|Tales of Symphonia  lyn|"
				"Gundam Seed Destiny|Shugo Chara",
				PCRE_CASELESS,);
			res = MATCH_REGEX(safe_r, s->d_name, strlen(s->d_name));
			res = res < 0 ? true : false;
		}
		return res > 0;
	},
	^ (const void * a, const void * b) {
		return strcasecmp( DIRENT(a)->d_name, DIRENT(b)->d_name);
	}
	);

	if (file_num == 0) {
		printf("%s\n", "NO files found");
		exit(EXIT_FAILURE);
	}

	char *sa[file_num+1];
	int total_length = 0;
	for(int i = 0 ; i < file_num; ++i) {
		sa[i] = files[i]->d_name;
		total_length += strlen(sa[i]);
	}
	sa[file_num] = NULL;
	
	char **s_arr = sa;
	if(ma->newest_only){
		//CHECK recalc length ? save mem, cost time
		s_arr = newest_only(sa, &file_num, false, true);
		file_num--;
	}
	
	if(ma->pl_shuffle) shuffle((void**) s_arr, file_num);
	
	if(ma->pl_output & PL_STDOUT){
		for(int i = 0; i < file_num; ++i){
			printf("%s\n",s_arr[i]);
		}
	}

	if(ma->write_history)           updateHistory(s_arr, ma->status, ma->sep);
	if(ma->pl_output & PL_PLAYLIST) make_playlist(ma->pl_name, ma->pl_dir, s_arr, ma->pl_format);
	
	pid_t pid =  fork();
	if ( pid != 0 ){
		switch (ma->player){
			//TODO players
			case P_MPLAYER: 
				mplayer(s_arr,file_num, total_length, ma->prefix_args.str, ma->postfix_args.str, path);
			
				break;
			case P_NICEPLAYER:
				niceplayer("");
				break;
			case P_VLC:
				vlc(s_arr, total_length, ma->prefix_args.str, ma->postfix_args.str, path);
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
void mplayer(char **filenames, int num_of_files, int total_length, char *prefix_args, char *postfix_args, char *filepath) {
	
	
	// mplayer binary
	const char* mplayer = "\"/Users/bilalh/Library/Application Support/MPlayer OSX Extended/Binaries/mplayer-pigoz.mpBinaries/Contents/MacOS/mplayer\"";
	const char *rid   = "&> /dev/null";      // discards output.
	
	if (prefix_args  == NULL) prefix_args  = "";
	if (postfix_args == NULL) postfix_args = "";
	
	int index        = strlen(filepath);
	int rid_len      = strlen(rid);
	int m_len        = strlen(mplayer);
	int space_quotes = num_of_files * 3; // for space and 2""
	
	//CHECK 1 from (8 for mplayer)
	char m_args[total_length + 
			    strlen(prefix_args) + strlen(postfix_args) + 
			    index + rid_len + m_len +
			    space_quotes  + 1
			   ];

	sprintf(m_args, "cd %s; %s %s ", filepath, mplayer, prefix_args);
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
	system(m_args);
}

//TODO niceplayer
void niceplayer(char *playlist) {
	
}
	
// TODO vlc
void vlc(char **filenames, int total_length, char *prefix_args, char *postfix_args, char *filepath) {
	
}
