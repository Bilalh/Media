#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#include <pcre.h>
#include <pcreposix.h>
#include "../include/string_util.h"
#include "../include/debug.h"

static char *test_hash(char *s);

char** ep_num (char *s) {
	char *start  = s;
	char **ans = calloc(2, sizeof(size_t));
	int index = 0;
	int num   = 0;
	int dashes = 0;
	
	// finds the end of the string
	while (*s != '\0' ) {
		if (num ==0 && isdigit(*s) ) num++;
		else if (*s == '-' ) dashes++;
		s++;	
	}
	
	// if there is no number (e.g movie) the whole string is the name.
	if (num == 0 ){
		dprintf("no number\n");
		ans[0] = ans[1] = s;
		char *temp = s - 1;
		// TODO no ext on name only
		while ( temp != start ){
			if ( *temp == '.'){
				ans[1] = temp;
				break;
			}
			temp--;
		}
		return ans;
	}
	
	while (*s != *start) {
		if (index == 0 && (*s == '-' || *s == ' ' || *s == '_'  || *s  == '~' ) ) {
			ans[index]  = s;
			index++;
			// CHECK quick fix for - types
			if( (s - start) >=2 ) ans[index] = s-2;
		}
		
		dprintf("i1  x:%d s :%c: \ts+1 :%c: \n",index, *s, *(s + 1));
		if(index == 1 && !(*s == ' ' || *s == '-' || *s == '_' || *s  == '~'  ) ) {
			char *t = (s + 1);
			dprintf("1i  s :%c: \ts+1 :%c: \n", *s, *(s + 1));
			if( *t == ' ' || *t == '-' || *t == '_' || *t  == '~' ) {
				dprintf("ii  s :%c: \ts+1 :%c: \n", *s, *(s + 1));
				if (*t == '~' && dashes > 0 ) t++;
				ans[index] = t;
				break;
			}
		}

		s--;
	}
	return ans;
}


char** spilt_string_malloc (char *str, int *res_length, bool malloces){
	char *to_spilt = strdup(str), *sep = " ", *s_ptr, *word;
	int slength = 7;
	if ( *res_length > 1 ) slength = *res_length;	
	char **strs = malloc(sizeof(char*) * slength);
	int i = 0;
	
	for(word = strtok_r(to_spilt, sep, &s_ptr); word != NULL;
			word = strtok_r(NULL, sep, &s_ptr)) {
		strs[i++] = malloces ? strdup(word) : word;
		if (i >= slength ) {
			slength = slength * 2 + 1;
			void * temp = realloc(strs, sizeof(char*) * slength);
			if (temp != NULL) {
				strs = temp;
			} else {
				fprintf(stderr, "realloc  failed in time spilt");
				exit(6);
			}
		}

	}
	
	free(to_spilt);
	*res_length = i;
	return strs;
	
}

inline char** spilt_string_m (char *str, int *res_length){
	return spilt_string_malloc(str,res_length,true);
}

inline char** spilt_string (char *str, int *res_length){
	return spilt_string_malloc(str,res_length,false);
}


char *spilt_args (char **arr, size_t length, char *separator, char *ending ) {
	SpiltData *sd_arr[length];
	int total = 0; // memory needed for final string
	// expand each element
	for (int i = 0; i < length; ++i) {
		sd_arr[i] = str_spilt_func(arr[i]);
		total += sd_arr[i]->total;
	}

	int sep_len   = strlen(separator);
	int end_len   = strlen(ending);
	size_t memory = // 1 for \0
		(sizeof(char) * total) + (sep_len * (length - 1)) + end_len + 1;
	char *final_str = malloc(memory);
	int index = 0;

	// builds final string
	for(int i = 0;  i < length; ++i) {
		for(int j = 0; j < sd_arr[i]->length; ++j) {
			strncpy(&final_str[index], sd_arr[i]->args[j], sd_arr[i]->lengths[j]);
			index += sd_arr[i]->lengths[j];
		}
		if (i != length - 1) {
			strncpy(&final_str[index], separator, sep_len);
			index += sep_len;
		} else {
			strncpy(&final_str[index], ending, end_len);
			index += end_len;
		}
	}

	//CHECK might sem fault on returned string with printf?
	final_str[index] = '\0';
	return final_str;
}

SpiltData *str_spilt_func (char *s) {
	char *start   = s;
	char **res    = malloc(sizeof(size_t) * 5);
	int  *res_len = malloc(sizeof(int) * 5);
	int total = 0;
	int i     = 0;

	while(*s != '\0' ) {
		while(*s != '|' && *s != '\0' ) {
			++s;
		}

		int length = s - start;

		char in[length+1];
		strncpy(in, start, length);
		in[length] = '\0';

		res[i]     = str_replace(start,  length , in, test_hash(in) );
		res_len[i] = strlen(res[i]);
		total     += res_len[i];

		if (*s == '|') ++s;
		start = s;
		++i;

	}

	SpiltData *sd = malloc(sizeof(SpiltData));
	sd->args      = res;
	sd->length    = i;
	sd->lengths   = res_len;
	sd->total     = total;
	return sd; 
}

char *str_replace (char *s, size_t len,  char *sub, char *rep) {
	int rep_len = strlen(rep);
	int sub_len = strlen(sub);
	char *r     = malloc(len * 2 + rep_len + 25 );

	// counters for s and r
	int is = 0, ir = 0;
	while(is < len) {
		// checks if sub is a sub string og s
		if (s[is] == *sub && strncmp(&s[is], sub, sub_len) == 0 ) {
			strncpy(&r[ir], rep , rep_len);
			ir += rep_len;
			is += sub_len;
		} else { // move
			r[ir++] = s[is++];
		}
	}

	r[ir] = '\0';
	return r;
}

Mapping *load_hash(const char *filename){
	Mapping *hash = NULL, *h;
	FILE *hfile = fopen(filename, "r");

	char lens[6];
	int key_len, val_len;
	
	while (fgets(lens, 6, hfile) != NULL ){
		key_len = lens[0] - 48;
		val_len = strtol(&lens[2], NULL , 10);
		// printf("kl:%i vl:%i\n", key_len,val_len);
		
		// +3 r - \t and \n
		char key[key_len+1], val[val_len+3];
		fgets(key, key_len+1, hfile);
		fgets(val, val_len+3, hfile);
		// gets rid of newline
		val[val_len+1] ='\0'; 
		
		// printf("k:'%s' v:'%s'\n", key,&val[1]);
		h = (Mapping*) malloc(sizeof(Mapping));
		h->key = strdup(key);
		h->val = strdup(&val[1]);
		HASH_ADD_KEYPTR( hh, hash, h->key, strlen(h->key), h );
	}
	return hash;
}

bool strcmp_null(const char *s1, const char *s2 ){
	if ( (s1 == NULL && s2 != NULL) || (s1 != NULL && s2 == NULL) ) return false;
	if ( s1 != NULL && s2 != NULL && strcmp( s1, s2 ) != 0 ) return false;
	return true;
}


// not used

// CHECK broken?
int match (const char *string, char *pattern) {
	int    status;
	regex_t    re;
	if ( regcomp(&re, pattern, REG_EXTENDED | REG_ICASE | REG_NOSUB) != 0) {
		return(0);
	}
	status = regexec(&re, string, (size_t) 0, NULL, 0);
	regfree(&re);
	return !(status != 0);

}

char *str_spilt_replace (char *s) {
	char *start   = s;
	char **res    = malloc(sizeof(size_t) * 5);
	char *res_len = malloc(sizeof(int) * 5);

	int i_res = 0, total = 0;

	while(*s != '\0') {

		while (*s != '|' && *s != '\0' ) {
			++s;
		}

		res[i_res] = str_replace(start, s - start, "char*ub", "char*p");
		res_len[i_res] = strlen(res[i_res]);
		total += res_len[i_res];
		++i_res;
		if (*s == '|') ++s;

		start = s;
	}

	return NULL;
}

char *str_lower (char *s, size_t length) {
	
	char *re = malloc(sizeof(char) * length +1);
	char *r  = re;
	while(( *r++ = tolower(*s++) )) ;
	*r = '\0';

	return re;
}

static char *test_hash (char *s) {
	if (strcmp(s, "fma" ) == 0) {
		return strdup("full metal");
	} else {
		return strdup(s);
	}

}

