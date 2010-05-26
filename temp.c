#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "hashtable.h"
#include "hashtable_itr.h"


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

struct key {
	unsigned char *str;
	char *full;
};

unsigned long str_hash(unsigned char *str) {
	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

unsigned int key_hash(void *ky) {
	struct key *k = (struct key *)ky;
	return (str_hash(k->str));
}

int equal_keys(void *k1, void *k2) {
	struct key *ka = (struct key *)k1;
	struct key *kb = (struct key *)k2;
	
	return (strcmp(ka->str, kb->str) ==0);
}

int main (int argc, char  *argv[]) {
	char *strings[] = {
		"index - 01", "index - 06", "index - 03"
	};
	int length = sizeof(strings) / sizeof(size_t);

	struct hashtable *h;
	h = create_hashtable(16, str_hash, equal_keys);

	char *str_last;
	for (int i = 0 ; i < length; ++i) {
		char **ans = ep_num(strings[i]);
		char *s = malloc(sizeof(char) * 1024);
		struct key k;
		
		long l = 1;
		if (ans[0] != NULL) {
			l = strtol(ans[0] + 1, NULL, 10);
			if (l == 0 ) l++;

			int index = ans[1] != NULL ? 1 : 0;
			strncpy(s, strings[i], ans[index] - strings[i]);
		} else {
			strcpy(s, strings[i]);
		}
		str_last = s;
		k.str  = (unsigned char*)s;
		k.full = strings[i];
		printf("num: %li\n", l);
		printf("str: %s\n", s);


		int *m = malloc(sizeof(int));
		*m  = l;
		printf("m: %i\n", *m);
		hashtable_insert(h,&k,m);
		free(ans);
		puts("");
	}


	printf("%s\n", str_last);
	struct key kk;
	kk.str =(unsigned char*) str_last;
	int *last = hashtable_search(h,  &kk);
	if (last == NULL){
		printf("%s\n", "NULL");
	}else{
		printf("final: %i\n", *last);
	}


	return 0;
}