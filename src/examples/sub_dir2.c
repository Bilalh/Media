#include <stdio.h>
#include <stdlib.h>

#include <ftw.h>
#include <fnmatch.h>

#include <pcre.h>

#include <include/string_util.h>
#include <include/time_regex.h>
#include <include/string_array.h>


MAKE_REGEX_VARS(getFilesRec_at);
static StringArray *file_name_buffer;
static int getFilesRec_at_total_length;

typedef struct{
	StringArray *str_arr;
	int total_length;
} StringArrayWithTotalLength;

static int ftw_callback(const char *fpath, const struct stat *sb, int typeflag) {
	
	/* if it's a file */
    if (typeflag == FTW_F) {
		int res = MATCH_REGEX(getFilesRec_at, fpath, strlen(fpath));
		if (res >0)  {
			string_array_add(file_name_buffer, fpath);
			getFilesRec_at_total_length += strlen(fpath);
		}
    }

    return 0;
}

StringArrayWithTotalLength getFilesRec(char *dir, char *regex) {

	MAKE_REGEX_PREMADE_VARS(getFilesRec_at, regex,PCRE_CASELESS)
	file_name_buffer = string_array_new(16);
	getFilesRec_at_total_length = 0;
	
    ftw(dir, ftw_callback, 10);
	
	StringArrayWithTotalLength sawtl =
	{
		.str_arr = file_name_buffer,
		.total_length = getFilesRec_at_total_length
	};
	
	return sawtl;
}


int main (int argc, char const *argv[]) {
	
	StringArrayWithTotalLength sta = getFilesRec(
	"/Users/bilalh/Movies/.Movie/. アニメ/divx", 
	"(.).*\\.(mkv|mp4|mov|avi|ogm|divx|rm|rmvb|flv|part|wmv)$");
	
	char **ptr  = sta.str_arr->arr;
	int new_length = sta.str_arr->index;
	
	ptr = newest_only(ptr, &new_length, false, true);
	
	while (*ptr != NULL){
		printf("%s\n", *ptr );
		++ptr;
	}
	
	return 0;
}