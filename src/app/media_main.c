#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>


#include <include/media.h>
#include <include/option_parser.h>
#include <include/colours.h>

int main (int argc, char **argv) {

	if (argc == 1 ) {
		print_usage();
		exit(0);
	}

	MediaArgs *opt = option_parser(&argc, &argv);
	char *path =  opt->root_dir;

	if (argc == 0 && !path) {
		efprintf("%s\n", "A Start Path MUST be given");
		print_usage();
		exit(1);
	} else if (!path) {
		path = argv[0];
		argc--; argv++;
	}
	
	struct stat st;
	if(stat(path, &st) != 0) {
		efprintf("%s does not exist or not readable\n", path);
		exit(2);
	}


	media(path, argv, argc , opt);
	return 0;
}