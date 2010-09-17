#include <stdio.h>
#include <stdlib.h>
#include <include/string_util.h>

// display details from the hash
int main (int argc, char const *argv[]) {
	
	if (argc < 2) {
		printf("%s\n", "shash hash_location");
		exit(0);
	}
	
	const char *hash_location = argv[1];
	Mapping *hash = load_hash(hash_location);
	Mapping *h;
	
	if (argc >= 3){
		HASH_FIND_STR( hash, argv[2], h);
		if (h != NULL){
			if (argc >= 4 && *argv[3] == 'v' ){
				printf("%s", h->val );
				
			}else{
				printf("  %-9s %s\n", h->key, h->val );
				
			}
		}
		exit(0);
	}
	
	for(h = hash; h != NULL; h = h->hh.next) {
		printf("  %-9s %s\n", h->key, h->val );
	}
	
	return 0;
}