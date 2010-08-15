#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

//#include <regex.h>
#include <pcre.h>
#include <pcreposix.h>

#include "string_util.h"

/// \brief return the char before the number and the last char before after the text
char** ep_num(char *s) {
	char *start  = s;
	char **ans = calloc(2, sizeof(size_t));
	int index = 0;

	while (*s != '\0' ) s++;

	while (*s != *start) {
		if (index == 0 && (*s == '-' || *s == ' ' || *s == '_' ) ) {
			ans[index]  = s;
			index++;
		} else if(index == 1 && !(*s == ' ' || ispunct(*s) ) ) {
			if( *(s + 1) == ' ' || ispunct(*(s + 1)) ) {
				printf("s :%c: \tc2 :%c: \n", *s, *(s + 1));
				// so that  (ans[index] - s) give number of char in the name
				ans[index] = s + 1;
				break;
			}
		}
		s--;
	}
	return ans;
}

// could always be 1
/// \brief Non zero on match, 0 on any error
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

/// \brief Concatenates the array with separator and adds the ending to the end
char *spilt_args(char **arr, int length, char *separator, char *ending ) {
	SpiltData *sd_arr[length];
	int total = 0; // memory needed for final string
	// expand each element
	for (int i = 0; i < length; ++i) {
		sd_arr[i] = spilt_func(arr[i]);
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

/// \brief Replaces the substring sub with rep in the first len charaters of s.
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

static char *test_hash(char *s) {
	if (strcmp(s, "fma" ) == 0) {
		return strdup("full metal");
	} else {
		return strdup(s);
	}

}

SpiltData *spilt_func(char *s) {
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

char *str_lower(char *s, int length) {
	char *re = malloc(sizeof(char) * length);
	char *r  = re;
	while(( *r++ = tolower(*s++) )) ;
	*r = '\0';
	return re;
}