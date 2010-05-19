#include <stdio.h>
#include <stdlib.h>
#include "media.h"
#define PATH "/Users/bilalh/Movies/.Movie/mov/"

int main (int argc, char *argv[]) {
	if (argc < 3) {
		puts("Media dir regex");
		exit(0);
	}
	media(PATH,&argv[2],argc-2);
	return 0;
}