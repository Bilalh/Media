#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "string_util.h"
#include "media.h"
#include "hashmap.h"

char** ep_num(char *s) {
	char *start  = s;
	char **ans = calloc(2, sizeof(size_t));
	int index = 0;

	while (*s != '\0' ) s++;

	while (*s != *start) {
		if (*s == '-' || *s == 'Z' || *s == '_' ) {
			ans[index]  = s;
			if (index == 0) index++;
		};
		s--;
	}
	return ans;
}

int main (int argc, char  *argv[]) {
	char *strings[] = {
		"index - 01", "index - 02", "index - 03", 
			"index - 5"
	};

	hashmap* h = mk_hmap(str_hash_fn, str_eq_fn, int_del_fn);

	char **ans = ep_num(strings[3]);
	char *s = malloc(sizeof(char)*1024);
	
	long l = 1;
	if (ans[0] != NULL) {
		l = strtol(ans[0]+1, NULL, 10);
		if (l == 0 ) l++;
		
		int index = ans[1] != NULL ? 1 : 0;
		strncpy(s, strings[3], ans[index]-strings[3]);
	}else{
		strcpy(s,strings[3]);
	}
		
	printf("num: %li\n", l);
	printf("str: %s\n", s);

	hmap_add(h,s,&l);
	
	
	int *last = hmap_get(h,s);
	printf("final: %i\n", *last);
	free(s);
	free(ans);
	
	return 0;
}