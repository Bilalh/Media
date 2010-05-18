#include <Block.h>
#include <dirent.h>
#include <regex.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <xlocale.h>
#include "Media.h"

#define DIRENT(value) (*(struct dirent **) value)
#define VIDEO ".*\\.mkv|mp4|mov|ogm|avi|divx|rm|rmvb|wmv|flv|part$"
#define AUDIO  ".*\\.mp3|m4a|flac|ogg|m4b|aiff|ac3|aac|wma|wav"				
                              
void 
	mplayer(char **filenames, int length, char *prefix_args, char *postfix_args, char *filepath);

void media(char *path) {
	struct dirent **files;

	// gets dir listing ignoring case and hidden files
	int file_num = scandir_b( path, &files,
		^ (struct dirent * s) { 
			return match(s->d_name ,VIDEO);
		},
		^ (const void * a, const void * b) {
			return strcasecmp( DIRENT(a)->d_name, DIRENT(b)->d_name);
		}
	);
	

	char *sa[file_num];
	for(int i =0 ; i<file_num;i++) sa[i] = files[i]->d_name; 
	mplayer(sa,4,"","",path);
}

//FIXME add length  
//TODO use struct
void 
mplayer(char **filenames, int length, char *prefix_args, char *postfix_args, char *filepath){
   	int index = strlen(filepath)+3;
	char m_args[strlen(filenames[0])*10+strlen(prefix_args)+strlen(postfix_args)+index+8];  
	strcpy(m_args,"cd ");              
	strcpy(&m_args[3],filepath);	       
	m_args[index++] = ';';             
	m_args[index++] = ' ';
	strcpy(&m_args[index],"mplayer "); // mplayer 
	index+=8;

	// copies each filename into m_args
	for(int i = 0; i < length; i++) {
		printf("%s	\n", filenames[i]); 		
		m_args[index++]='"';
		strcpy(&m_args[index], filenames[i]);
		index += strlen(filenames[i]);
		m_args[index++]='"';
		m_args[index++]=' ';
	}
	m_args[index] = '\0';
	printf("%s\n", m_args);
	system(m_args);
}                                            

/// \brief 1 on match, 0 on any error s
int match(const char *string, char *pattern) {
	int    status;
	regex_t    re;

	if (regcomp(&re, pattern, REG_EXTENDED | REG_ICASE | REG_NOSUB) != 0) {
		return(0);      
	}
	status = regexec(&re, string, (size_t) 0, NULL, 0);
	regfree(&re);
	
	return !(status !=0);
	
}
