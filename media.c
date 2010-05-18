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
#define PATTEN ".*\\.mkv"
void media(char *path) {
	struct dirent **files;

	// gets dir listing ignoring case and hidden files
	int file_num = scandir_b( path, &files,
		^ (struct dirent * s) { 
			// return (s->d_name[0] != '.'); 
			return match(s->d_name ,PATTEN);
		},
		^ (const void * a, const void * b) {
			return strcasecmp( DIRENT(a)->d_name, DIRENT(b)->d_name);
		}
	);
	
	int path_len = strlen(path)+3 ;// 3 extra for,cd ; 
	int cur_len  = 50 * file_num + path_len;
	
	// builds the argument fro mplayer
	char m_args[cur_len];              // args for mplayer
	int index = path_len;              // index in string
	strcpy(m_args,"cd ");              // cd command
	strcpy(&m_args[3],path);	       
	m_args[index++] = ';';             
	m_args[index++] = ' ';             
	strcpy(&m_args[index],"mplayer "); // mplayer 
	index+=8;                                   
	
	for(int i = 0; i < file_num; i++) {
		printf("%s	\n", files[i]->d_name);
		// copies each filename into m_args
		m_args[index++]='"';
		strcpy(&m_args[index], files[i]->d_name);
		index += strlen(files[i]->d_name);
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
	if (status != 0) {
		return(0);      
	}
	return(1);
}
