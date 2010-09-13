// example of hash

#include <stdio.h>
#include <stdlib.h>
#include "../../include/uthash.h"

// reads the hash from file and prints it out

typedef struct  {
	char *key;
	char *val;
	UT_hash_handle hh;
} Mapping;

int main (int argc, char const *argv[]) {

	Mapping *hash = NULL, *h;
	FILE *hfile = fopen("../build/zzhash", "r");

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
	
	// iteration 
	for(h = hash; h != NULL; h = h->hh.next) {
		printf("key:'%s' \tval:'%s'\n", h->key, h->val );
	}
	
	// deletion 
	while (hash){
		h = hash;
		HASH_DEL(hash,h);
		printf("key:'%s' \tval:'%s'\n", h->key, h->val );
		free(h->key);
		free(h->val);
		free(h); 
	}
	
	return 0;
}
