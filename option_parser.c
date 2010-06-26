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
	
	// Prefs
	ma->hash_location = NULL;
	ma->use_hash      = false;
	ma->status        = S_NONE;
	ma->write_history = false;

	// Player
	ma->player = P_NONE;
	ma->afloat = false;
	ma->prefix_args = ma->postfix_args = NULL;
	// ma->prefix_index = ma->prefix_length = ma->postfix_index = ma->postfix_length = 0;
	ma->nice_repeat=false;
	

	return ma;
}

void print_media_args(MediaArgs *ma){
	#define truth(boolean) (boolean ? "true" : "false" )
	#define nullcheck(str) (str == NULL ? "NULL" : str )
	#define print_args(title,value) printf("%20s: %s\n",  title, value);
	#define print_hex(title,value) printf("%20s: 0x%x\n",  title, value);
	
	printf("Selection\n");
	print_args("exclude"  ,   truth(ma->exclude));
	print_args("excludes" ,   nullcheck(ma->excludes));
	print_args("newest_only", truth(ma->newest_only));
	print_args("sub_dirs",    truth(ma->sub_dirs));
	print_hex("types",        ma->types);
	
	
	printf("Playlist\n");
	print_args("pl_rand",     truth(ma->pl_rand));
	print_args("pl_dir",      nullcheck(ma->pl_dir));
	print_args("pl_name",     truth(ma->pl_name));
	print_hex("pl_format",    ma->pl_format);
	print_hex("pl_output",    ma->pl_output);
	print_args("pl_rand",     truth(ma->pl_rand));
	
	printf("Prefs\n");
	print_args("hash_location", truth(ma->hash_location));
	print_args("use_hash",      truth(ma->use_hash));
	print_hex("types",          ma->types);
	print_args("write_history", truth(ma->write_history));
	
	printf("Player\n");
	print_hex("player",        ma->player);
	print_args("afloat",       truth(ma->afloat));
	print_args("prefix_args",  nullcheck(ma->prefix_args));
	print_args("postfix_args", nullcheck(ma->postfix_args));
	print_args("nice_repeat",  truth(ma->nice_repeat));
	
	#undef truth
	#undef nullcheck
	#undef print_args
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
	print_media_args(ma);
    exit (0);
}
