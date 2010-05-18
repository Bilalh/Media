#include <stdio.h>
#include <stdlib.h>
#include "media.h"
#define PATH "/Users/bilalh/Movies/.Movie/mov/"

static char *s = PATH;


int main (int argc, char const *argv[]) {
	argc = 4;
	if (argc < 3) {
		puts("Media dir regex");
		exit(0);
	}

	media(s );

	return 0;
}