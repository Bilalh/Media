#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#include <pcre.h>
#include <pcreposix.h>
#include "string_util.h"
#include "debug.h"

static char *test_hash(char *s);

char** ep_num(char *s) {
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

int match(const char *string, char *pattern) {
	int    status;
	regex_t    re;
	if ( regcomp(&re, pattern, REG_EXTENDED | REG_ICASE | REG_NOSUB) != 0) {
		return(0);
	}
	status = regexec(&re, string, (size_t) 0, NULL, 0);
	regfree(&re);
	return !(status != 0);

}

char *spilt_args(char **arr, size_t length, char *separator, char *ending ) {
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

char *str_replace(char *s, size_t len,  char *sub, char *rep) {
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

SpiltData *str_spilt_func(char *s) {
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

// not used
char *str_spilt_replace(char *s) {
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

char *str_lower(char *s, size_t length) {
	
	char *re = malloc(sizeof(char) * length +1);
	char *r  = re;
	while(( *r++ = tolower(*s++) )) ;
	*r = '\0';

	return re;
}

static char *test_hash(char *s) {
	if (strcmp(s, "fma" ) == 0) {
		return strdup("full metal");
	} else {
		return strdup(s);
	}

}
