#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <pcre.h>
#include <Block.h>

#include "../include/history.h"
#include "../include/media.h"
#include "../include/playlist.h"
#include "../include/string_util.h"
#include "../include/time_regex.h"


#define DIRENT(value) (*(struct dirent **) value)
// #define VIDEO  ".*\\.(mkv|mp4|avi)$"
#define VIDEO  ".*\\.(mkv|mp4|mov|avi|ogm|divx|rm|rmvb|flv|part|wmv)$"
#define AUDIO  ".*\\.(mp3|m4a|flac|ogg|m4b|aiff|ac3|aac|wav|wmv|ape)$"

//TODO sub dirs 

void media(char *path, char **args, int argc, MediaArgs *ma) {
	struct dirent **files;
	for(int i = 0; i < argc; ++i){
		printf("%s\n", args[i]);
	}
	
	char *hash_location = ma->use_hash ? ma->hash_location : "";
	char *regex = spilt_args(args, argc, ".*", "(", ")"VIDEO, hash_location);
	printf("regex: %s\n", regex);

	// gets dir listing ignoring case and matching the patten
	int file_num = scandir_b( path, &files,
	^ (struct dirent * s) {
		MAKE_REGEX(at, regex,);
		int res =MATCH_REGEX(at, s->d_name, strlen(s->d_name));
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
	sa[file_num] = "";
	if (ma->pl_output & PL_STDOUT){
		for(int i = 0; i < file_num; ++i){
			printf("[%i] %s\n",i, sa[i]);
		}
	}

	if (ma->write_history)           updateHistory(sa);
	if (ma->pl_output & PL_PLAYLIST) make_playlist(ma->pl_name, ma->pl_dir, sa, ma->pl_format);
	switch (ma->player){
		//TODO players
		case P_MPLAYER: 
			mplayer(sa, total_length, ma->prefix_args.str, ma->postfix_args.str, path);
			break;
		case P_NICEPLAYER:
			niceplayer("");
			break;
		case P_VLC:
			vlc(sa, total_length, ma->prefix_args.str, ma->postfix_args.str, path);
			break;
		case P_NONE: break;
	}
	
}

/// \brief Filenames should end with "", total length the length of all the strings
/// filepath, to the directory to call mplayer from.
void mplayer(char **filenames, int total_length, char *prefix_args, char *postfix_args, char *filepath) {
	if (prefix_args  == NULL) prefix_args  = "";
	if (postfix_args == NULL) postfix_args = "";
	
	int index   = strlen(filepath);
	char *rid   = " &> /dev/null";      // discards output.
	int rid_len = strlen(rid);
	char m_args[total_length + 
	strlen(prefix_args) + strlen(postfix_args) + index + rid_len+ 8];

	sprintf(m_args, "cd %s; mplayer %s ", filepath, prefix_args);
	// 3 for cd 2 for ; 1 for  .
	index += 3 + 2 + 8 + strlen(prefix_args) + 1;

	// append filenames
	while (**filenames != '\0') {
		printf("%s\n", *filenames);
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
	printf("%s\n", m_args);
	system(m_args);
}

//TODO niceplayer
void niceplayer(char *playlist) {
	
}

// TODO vlc
void vlc(char **filenames, int total_length, char *prefix_args, char *postfix_args, char *filepath) {
	
}
