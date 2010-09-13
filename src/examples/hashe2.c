// example of hash

#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */
#include <include/uthash.h>

typedef struct  {
	char *key;
	char *val;
	UT_hash_handle hh;
} Mapping;


int main(int argc, char *argv[]) {


	char *keys[] = { "joe", "bob", "betty", "betty", NULL };
	char *vals[] = { "doe", "dob", "detty", "new"  , NULL };

	Mapping *hash = NULL; // must be null
	Mapping *s;

	char **k, **v;
	for (k = keys, v = vals ; *k != NULL; k++, v++) {

		HASH_FIND_STR( hash, *k, s);
		if ( s == NULL) {
			s = (Mapping*) malloc(sizeof(Mapping));
			s->key  = *k;
			s->val  = *v;

			HASH_ADD_KEYPTR( hh, hash, s->key, strlen(s->key), s );
		} else {
			s->val = *v;
		}

	}

	for(s = hash; s != NULL; s = s->hh.next) {
		printf("key:%s val:%s\n", s->key, s->val );
	}


	return 0;
}

