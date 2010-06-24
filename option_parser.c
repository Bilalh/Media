#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "option_parser.h"

MediaArgs *new_media_args() {

	MediaArgs *ma = malloc(sizeof(MediaArgs));

	// Selection
	ma->exclude     = false;
	ma->excludes    = NULL;
	ma->newest_only = false;
	ma->sub_dirs    = false;
	ma->types       = T_VIDEO;

	// Playlist
	ma->pl_dir        = ma->pl_name = NULL;
	ma->pl_format     = PL_STDOUT;
	ma->pl_output     = PL_NONE;
	ma->pl_rand       = false;
	ma->write_history = false;
	
	// Prefs
	ma->hash_location = NULL;
	ma->use_hash      = false;
	ma->status        = S_NONE;

	// Player
	ma->player = P_NONE;
	ma->afloat = false;
	ma->prefix_args = ma->postfix_args = NULL;
	ma->nice_repeat=false;
	

	return ma;
}

int main (int argc, char **argv) {
	
	MediaArgs *ma = new_media_args();
	
    int c;
    int digit_optind = 0;
    int aopt = 0, bopt = 0;
    char *copt = 0, *dopt = 0;
    static struct option long_options[] = {
        {"add"     ,required_argument ,0 ,NO_SHORT_OPT},
        {"append"  ,no_argument       ,0 ,NO_SHORT_OPT},
        {"delete"  ,required_argument ,0 ,'d'},        
		{"verbose" ,no_argument       ,0 ,NO_SHORT_OPT},
        {"create"  ,required_argument ,0 ,'c'},
        {"file"    ,required_argument ,0 ,NO_SHORT_OPT},
		{0         ,0                 ,0 ,0}
    };
    int option_index = 0;
    while ((c = getopt_long(argc, argv, "abc:d:012",
                 long_options, &option_index)) != -1) {
	
        int this_option_optind = optind ? optind : 1;
        switch (c) {
        case 0:
            printf ("option %s", long_options[option_index].name);
            if (optarg)
                printf (" with arg %s", optarg);
            printf ("\n");
            break;
        case '0':
        case '1':
        case '2':
            if (digit_optind != 0 && digit_optind != this_option_optind)
              printf ("digits occur in two different argv-elements.\n");
            digit_optind = this_option_optind;
            printf ("option %c\n", c);
            break;
        case 'a':
            printf ("option a\n");
            aopt = 1;
            break;
        case 'b':
            printf ("option b\n");
            bopt = 1;
            break;
        case 'c':
            printf ("option c with value '%s'\n", optarg);
            copt = optarg;
            break;
        case 'd':
            printf ("option d with value '%s'\n", optarg);
            dopt = optarg;
            break;
        case '?':
            break;
        default:
            printf ("?? getopt returned character code 0%o ??\n", c);
        }
    }
    if (optind < argc) {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        printf ("\n");
    }
    exit (0);
}
