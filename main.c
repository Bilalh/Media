#include <stdio.h>
#include <stdlib.h>
#include "media.h"
#include "string_util.h"
// #define PATH "/Users/bilalh/Movies/.Movie/mov/"
#define PATH "/Users/bilalh/Movies/.Movie/divx/"
// #define PATH "/Users/bilalh/Movies/.Movie/OpeningP/"


int main (int argc, char **argv) {
	if (argc < 3) {
		puts("Media dir regex");
		exit(0);
	}
	media(PATH,&argv[2],argc-2);
	return 0;
}