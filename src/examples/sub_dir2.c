#include <stdio.h>
#include <stdlib.h>

#include <ftw.h>
#include <fnmatch.h>

#include <pcre.h>

#include <include/string_util.h>
#include <include/time_regex.h>
#include <include/string_array.h>


MAKE_REGEX_VARS(at);
StringArray *file_name_buffer;

static int ftw_callback(const char *fpath, const struct stat *sb, int typeflag) {
	
	
	/* if it's a file */
    if (typeflag == FTW_F) {
		int res = MATCH_REGEX(at, fpath, strlen(fpath));
		if (res >0)  {
			printf("%s\n", fpath );
			string_array_add(file_name_buffer, strdup(fpath));
			
		}
    }

    return 0;
}

int main() {
	char *regex  = "(.).*\\.(mkv|mp4|mov|avi|ogm|divx|rm|rmvb|flv|part|wmv)$";
	MAKE_REGEX_PREMADE_VARS(at, regex,PCRE_CASELESS)
	file_name_buffer = string_array_new(160);
	
	for(int i = 0; i < file_name_buffer->index; ++i){
		printf("%s\n", file_name_buffer->arr[i]);
	}
	
    ftw("/Users/bilalh/Movies/.Movie/. アニメ/divx", ftw_callback, 10);
	return 0;
}
