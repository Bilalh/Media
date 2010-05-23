#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "string_util.h"

/// \brief non zero on match, 0 on any error
int match(const char *string, char *pattern) {
	int    status;
	regex_t    re;

	if (regcomp(&re, pattern, REG_EXTENDED | REG_ICASE | REG_NOSUB) != 0) {
		return(0);
	}
	status = regexec(&re, string, (size_t) 0, NULL, 0);
	regfree(&re);

	return !(status != 0);

}

char *str_replace(char *s, size_t len,  char *sub, char *rep) {
	int rep_len = strlen(rep);
	int sub_len = strlen(sub);
	char *r     = malloc(len * 2 + rep_len * 2 + 100 );

	// counters for s and r
	int is = 0, ir = 0;
	while(is < len) {
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


char *str_spilt_replace1(char *s) {
	char *start = s, *r = s;
	while(*s != '|' && *s != '\0' ) {
		s++;
	}

	if(s != start) r = str_replace(start,  s - start , "fma", "full me");
	start = s;
	s++;
	while(*s != '|' && *s != '\0' ) {
		s++;
	}
	printf("%s\n", s);
	if(s != start) r = str_replace(start,  s - start , "kga", "kid");

	return r;
}


char *str_spilt_replace(char *s) {
	char *start = s;
	char **res = malloc(sizeof(size_t) * 5);
	int  *res_len = malloc(sizeof(int) * 5);
	int total = 0;
	int i = 0;

	while(*s != '\0' ) {
		while(*s != '|' && *s != '\0' ) {
			++s;
		}
		if(s != start) {
			res[i] = str_replace(start,  s - start , "fma", "full metal");
			res_len[i] = strlen(res[i]);
			total += res_len[i];
			++i;
			++s;
		}
		start = s;
	}

	char *r  = malloc (total + 1);
	int j = 0, k = 0;
	for(j = 0; j < i; ++j) {
		if (j != 0) r[k++] = '|';
		strncpy(&r[k], res[j], res_len[j] );
		k += res_len[j];
		free(res[i]);
	}

	free(res);
	free(res_len);
	return r;
}

int main (int argc, char const *argv[]) {
	char *s = "fma|fma";
	char *r = str_spilt_replace(s);
	puts("");
	printf("s %lu r %lu \n", strlen(s) , strlen(r));
	printf("%s\n", s);
	printf("%s\n", r);

	return 0;
}


char *strrep2(char *s, char *sub, char *rep) {
	int len     = strlen(s);
	int rep_len = strlen(rep);
	int sub_len = strlen(sub);
	char *r     = malloc(len + 100);

	// counters for s and r
	int is = 0, ir = 0, i = 0;
	while(is < len) {
		if (s[is] == *sub && strncmp(&s[is], sub, sub_len) == 0 ) {
			strncpy(&r[ir], &s[i], is - i );
			strncpy(&r[ir], rep , rep_len);
			ir += rep_len;
			is += sub_len;
		} else {
			is++;
		}
	}
	r[ir] = '\0';
	return r;
}
