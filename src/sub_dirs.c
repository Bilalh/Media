#include <dirent.h>
#include <fnmatch.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <pcre.h>

#include <include/string_util.h>
#include <include/time_regex.h>
#include <include/string_array.h>
#include <include/sub_dirs.h>

#define DIRENT(value) (*(struct dirent **) value)
#define GET_FILES_REC_SAFE_REGEX \
	"Haruhi|Mai-(Otome|Hime)|K-on|Kaichou wa|Hime Chen! Otogi|gundam 00 S2 op 2|Rave|" \
	"aw.mp4|Ar tonelico|11 eyes|Disgaea 3|Kampfer|To ?aru Majutsu no Index|Princess |" \
	"Nogizaka Haruka| eng|So ?ra ?no ?Wo ?to Op 2|Saki ed|moon|Myself;|Shining Tears|" \
	"Kiddy Girl-and|Turn a moon|Dragonaut The Resonance|Sakuranbo|Starry Heavens|"     \
	"Gundam Seed Destiny|Shugo Chara|EX OVA|Atelier Totori op 1|frontier|Azumanga|"    \
	"fantasy|Kidou Tenshi"

static int get_files_recursive_callback(const char *fpath, const struct stat *sb, int typeflag);

static StringArray *file_name_buffer;
static int getFilesRec_at_total_length;
static bool getFilesRec_at_safe;
MAKE_REGEX_VARS(getFilesRec_at);
MAKE_REGEX_VARS(getFilesRec_safe);


static int get_files_recursive_callback(const char *fpath, const struct stat *sb, int typeflag) {
	
	/* if it's a file */
    if (typeflag == FTW_F) {
		int res = MATCH_REGEX(getFilesRec_at, fpath, strlen(fpath));
		if (getFilesRec_at_safe && res > 0 ){
			res = MATCH_REGEX(getFilesRec_safe, fpath, strlen(fpath));
			res = res < 0 ? true : false;
		}
		if (res >0)  {
			string_array_add(file_name_buffer, fpath);
			getFilesRec_at_total_length += strlen(fpath);
			
		}
    }

    return 0;
}

StringsPlusMeta* get_files_recursive(char *dir, char *regex, bool safe) {
	
	// Sets up vars
	MAKE_REGEX_PREMADE_VARS(getFilesRec_at, regex,PCRE_CASELESS)
	file_name_buffer = string_array_new(16);
	getFilesRec_at_total_length = 0;
	getFilesRec_at_safe = safe;
	
	MAKE_REGEX_PREMADE_VARS(getFilesRec_safe, 
		GET_FILES_REC_SAFE_REGEX,
		PCRE_CASELESS);
	
	// get file listing
    ftw(dir, get_files_recursive_callback, 10);
	file_name_buffer->arr[file_name_buffer->index] = NULL;
	
	if (file_name_buffer->index == 0) {
		return NULL;
	}
	
	StringsPlusMeta spm =
	{
		.str_arr      = file_name_buffer->arr,
		.length       = file_name_buffer->index,
		.total_length = getFilesRec_at_total_length
	};
	
	StringsPlusMeta *spm_malloc = malloc(sizeof(StringsPlusMeta));
	*spm_malloc = spm;
	
	return spm_malloc;
}

StringsPlusMeta* get_files(char *dir, char *regex, bool safe) {
	// gets dir listing ignoring case and matching the patten
	
	struct dirent **files;
	int file_num = scandir_b( dir, &files,
	^ (struct dirent * s) {
		MAKE_REGEX_OPTS(at, regex,PCRE_CASELESS,);
		int res = MATCH_REGEX(at, s->d_name, strlen(s->d_name));
		if (safe && res > 0 ){
			MAKE_REGEX_OPTS(safe_r, 
				GET_FILES_REC_SAFE_REGEX,
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
		return NULL;
	}

	char **sa = malloc(sizeof(char*) * file_num+1);
	int total_length = 0;
	for(int i = 0 ; i < file_num; ++i) {
		sa[i] = strdup(files[i]->d_name);
		total_length += strlen(sa[i]);
	}
	sa[file_num] = NULL;
	
	StringsPlusMeta spm =
	{
		.str_arr      = sa,
		.length       = file_num,
		.total_length = total_length
	};
	
	StringsPlusMeta *spm_malloc = malloc(sizeof(StringsPlusMeta));
	*spm_malloc = spm;
	
	return spm_malloc;
}
