#include <stdio.h>
#include <stdlib.h>
#include "media.h"
#include "option_parser.h"

#define PATH "/Users/bilalh/Movies/.Movie/divx"
// #define PATH "/Users/bilalh/Movies/.Movie/OpeningP"
// #define PATH "/Users/bilalh/Movies/.Movie/mov"

int main (int argc, char **argv) {
	if (argc < 3) {
		puts("Media dir regex");
		exit(0);
	}
	
	MediaArgs *opt = new_media_args();
	media(PATH,&argv[2],argc-2,opt);
	return 0;
}