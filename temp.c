#include "hashtable.h"
#include "hashtable_itr.h"
#include "string_util.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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

	return (strcmp( (char*) ka->str, (char*) kb->str) == 0);
}

int main (int argc, char  *argv[]) {
	char *strings[] = {
		"index - 01.mkv", "index - 02.mkv", "index - 06.mkv", "index - 04.mkv"
	};
	int length = sizeof(strings) / sizeof(size_t);

	struct hashtable *h;
	h = create_hashtable(16, key_hash, equal_keys);

	char *str_last;
	for (int i = 0 ; i < length; ++i) {
		char **ans = ep_num(strings[i]);
		char *s = malloc(sizeof(char) * 1024);
		struct key *k = malloc(sizeof(struct key));

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

		printf("num: %li\n", l);
		printf("str: %s\n", s);

		k->str  = (unsigned char*)s;
		k->full = strings[i];

		int *result = hashtable_search(h, k);
		if (result == NULL || l > *result ) {
			free(result);
			int *m = malloc(sizeof(int));
			*m  = l;
			if (result == NULL){
				hashtable_insert(h, k, m);
			}else{
				hashtable_remove(h, k);
				hashtable_insert(h, k, m);
			}
		}
		free(ans);
		puts("");
	}


	struct hashtable_itr *itr;
	struct key *kk;
	int *v, i = 0;
	itr = hashtable_iterator(h);
	if (hashtable_count(h) > 0) {
		do {
			kk = hashtable_iterator_key(itr);
			v = hashtable_iterator_value(itr);
			i++;
			printf("%i: %i %s \n",i, *v, kk->full );
		} while (hashtable_iterator_advance(itr));
	}


	hashtable_destroy(h, true);

}