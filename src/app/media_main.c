#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <include/media.h>
#include <include/option_parser.h>

// #define PATH "/Users/bilalh/Movies/.Movie/divx"
#define PATH "/Users/bilalh/Movies/.Movie/OpeningP"

int main (int argc, char **argv) {

	if ( argc == 2) {
		if( strncmp(argv[1], "-h", 2) == 0) {
			print_help("");
		} else if( strncmp(argv[1], "--help", 6) == 0 ) {
			print_help("");
		} else if( strncmp(argv[1], "-help", 5) == 0 ) {
			print_help("");
		}
		exit(0);
	}

	if (argc < 3) {
		print_usage();
		exit(1);
	}

	// gets the path
	char *g_path  = strdup(argv[1]);
	argc--; argv++;

	MediaArgs *opt = option_parser(&argc, &argv);
	char *path =  opt->root_dir ?  opt->root_dir : g_path;

	media(path, argv, argc , opt);
	free(g_path);
	// CHECK free opt?
	return 0;
}