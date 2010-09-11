#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/media.h"
#include "../include/option_parser.h"

// #define PATH "/Users/bilalh/Movies/.Movie/divx"
#define PATH "/Users/bilalh/Movies/.Movie/OpeningP"
// #define PATH "/Users/bilalh/Movies/.Movie/mov"

int main (int argc, char **argv) {
	if (argc < 3) {
		puts("Media dir regex");
		exit(0);
	}
	
	// gets the path
	char *path  = strdup(argv[1]);
	argc--;
	argv++;
	
	MediaArgs *opt = option_parser(&argc,&argv);
	
	media(PATH, argv, argc , opt);
	return 0;
}