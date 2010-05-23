#include <Block.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

#include "media.h"
#include "string_util.h"

#define DIRENT(value) (*(struct dirent **) value)
#define VIDEO ".*\\.mkv|mp4|mov|ogm|avi|divx|rm|rmvb|flv|part|wmv$"
#define AUDIO  ".*\\.mp3|m4a|flac|ogg|m4b|aiff|ac3|aac|wav|wmv|ape$"				
                              

void media(char *path, char **args,int argc) {
	struct dirent **files;
	
	// gets dir listing ignoring case and matching the patten
	int file_num = scandir_b( path, &files,
		^ (struct dirent * s) { 
			return match(s->d_name ,VIDEO);
		},
		^ (const void * a, const void * b) {
			return strcasecmp( DIRENT(a)->d_name, DIRENT(b)->d_name);
		}
	);
	

	char *sa[file_num+1];
	int total_length = 0;
	for(int i =0 ; i<file_num-1;++i){ 
		sa[i] = files[i]->d_name;
		total_length += strlen(sa[i]);
	}
	sa[file_num-1] ="";
	mplayer(sa,total_length,"","",path);
}

/// \brief Filenames should end with "", total length the length of all the strings
/// filepath, to the directory to call mplayer from.  
void mplayer(char **filenames, int total_length, 
		char *prefix_args, char *postfix_args, char *filepath) {

	int index   = strlen(filepath) + 3; // 3 for cd 
	char *rid   = " &> /dev/null"; // discards output
	int rid_len = strlen(rid);
	char m_args[total_length +  // 8 for mplayer
		strlen(prefix_args) + strlen(postfix_args) + index + rid_len+ 8];
	
	strcpy(m_args, "cd ");
	strcpy(&m_args[3], filepath);
	m_args[index++] = ';';
	m_args[index++] = ' ';
	strcpy(&m_args[index], "mplayer "); 
	index += 8;
	
	// append filenames
	while (**filenames != '\0'){
		printf("%s\n", *filenames);
		m_args[index++] = '"';
		
		strcpy(&m_args[index], *filenames);
		index += strlen(*filenames);
		
		m_args[index++] = '"';
		m_args[index++] = ' ';
		++filenames;
	}
	
	strcpy(&m_args[index], rid);
	index +=rid_len;
	
	m_args[index] = '\0';
	printf("%s\n", m_args);
	system(m_args);
}                                            
