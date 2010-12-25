#include <stdio.h>
#include <stdlib.h>

#include <ftw.h>
#include <fnmatch.h>

#include <pcre.h>

#include <include/string_util.h>
#include <include/time_regex.h>


MAKE_REGEX_VARS(at);

char **string_array;
int current;
int  length;

static int ftw_callback(const char *fpath, const struct stat *sb, int typeflag) {
	
	
	/* if it's a file */
    if (typeflag == FTW_F) {
		int res = MATCH_REGEX(at, fpath, strlen(fpath));
		if (res >0)  {
			
			if (current == length){
				length *=2;
				string_array = realloc(string_array, sizeof(char*) *length);
			}
			
			string_array[current++] =  strdup(fpath); 
			
		}
    }

    return 0;
}

int main() {
	char *regex  = "(.).*\\.(mkv|mp4|mov|avi|ogm|divx|rm|rmvb|flv|part|wmv)$";
	MAKE_REGEX_PREMADE_VARS(at, regex,PCRE_CASELESS)
	string_array = malloc(sizeof(char*) *16);
	current      = 0;
	length       = 16;
	
    ftw("/Users/bilalh/Movies/.Movie/OpeningP", ftw_callback, 10);
	return 0;
}
