#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "option_parser.h"
#include "string.h"


const static struct option Long_options[] = {
	{"fs",      no_argument,           0, 'f'},
	{"mplayer", no_argument,           0, 'm'},
	{"",        no_argument,           0, 'a'},
	{0, 0, 0, 0}
};

 
void print_help(){
	 
	// structs to print nice
	#define csElement const static Element
	csElement filetype[] ={
		
	};
	csElement filepath[] ={
		
	};
	csElement mplayer[] = { 
		{&Long_options[0], "", "plays file(s) in fullscreen"}, //--fs
		{&Long_options[2], "TEST", "TEST"} //--fs
	};
	csElement playlist[] ={
		
	};
	csElement player[] ={
		{&Long_options[1], "", "plays file(s) using mplayer"} //--mplayer
	};
	csElement output[] ={
		
	};
	csElement other[] ={
		
	};
	
	const static HelpLink help[] = {
		{ "Filetype", sizeof(filetype) / sizeof(Element), &filetype[0] },
		{ "Filepath", sizeof(filepath) / sizeof(Element), &filepath[0] },
		{ "Mplayer",  sizeof(mplayer)  / sizeof(Element), &mplayer[0]  },
		{ "Playlist", sizeof(playlist) / sizeof(Element), &playlist[0] },
		{ "Player",   sizeof(player)   / sizeof(Element), &player[0]   },
		{ "Output",   sizeof(output)   / sizeof(Element), &output[0]   },
		{ "Other",    sizeof(other)    / sizeof(Element), &other[0]    },
	};
	
	size_t length = sizeof(help) / sizeof(HelpLink);
	const char *s_exp = "\t%-3s %-15s %s\n";
	
	for(int i = 0; i < length; ++i){
		printf("\n%s\n", help[i].grouping);
		for(int j = 0; j < help[i].length; j++){
			
			const struct option *optr = help[i].links[j].opt;
			// makes the space for the short arg
			char short_opt[3] = ""; 
			if (optr->val != NO_SHORT_OPT) sprintf(short_opt, "-%c",optr->val);
			// makes the space for the long arg
			char long_opt[3+strlen(optr->name)];
			if (*optr->name != '\0') sprintf(long_opt, "--%s",optr->name);
			else long_opt[0] = '\0';
			
			if (optr->has_arg == required_argument){
				// joins long opt and arg to print nicely
				char name_arg[strlen(help[i].links[j].arg) + strlen(optr->name) + 4];
				sprintf(name_arg, "%s [%s]", long_opt, help[i].links[j].arg );
				printf(s_exp, short_opt, name_arg, help[i].links[j].help );
			}else{
				printf(s_exp, short_opt, long_opt, help[i].links[j].help );	
			}
			
		}
	}
	#undef csElement
}
 
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
	
	ma->prefix_args.length = ma->postfix_args.length = 1;
	ma->prefix_args.index  = ma->postfix_args.index  = 0;
	
	ma->prefix_args.str    = malloc(sizeof(char) * ma->prefix_args.length);
	ma->postfix_args.str   = malloc(sizeof(char) * ma->postfix_args.length);
	
	ma->prefix_args.str[0] = ma->postfix_args.str[0] = '\0';
	 
	
	ma->nice_repeat = false;
	 
	return ma;
}

void print_media_args(MediaArgs *ma) {
	return;
#define truth(boolean) (boolean ? "true" : "false" )
#define nullcheck(str) (str == NULL ? "NULL" : str )
#define strcheck(s)    (s.str != NULL ? s.str : "NULL" )
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
	print_args("prefix_args",  strcheck(ma->prefix_args));
	print_args("postfix_args", strcheck(ma->postfix_args));
	print_args("nice_repeat",  truth(ma->nice_repeat));

#undef truth
#undef nullcheck
#undef print_args
}

MediaArgs *option_parser(int argc, char **argv) {

	int c, digit_optind = 0, option_index =0;
	MediaArgs *ma = new_media_args();

	while ((c = getopt_long(argc, argv, ":fm?", Long_options, &option_index)) != -1) {
		int this_option_optind = optind ? optind : 1;
		switch (c) {
		case '?':
			print_help();
			exit(0);
			break;			
		case 0:
			printf("%s\n", "jj");
			printf ("option %s", Long_options[option_index].name);
			if (optarg)
				printf (" with arg %s", optarg);
			printf ("\n");
			break;
		case '1':
		case '2':
			if (digit_optind != 0 && digit_optind != this_option_optind)
				printf ("digits occur in two different argv-elements.\n");
			digit_optind = this_option_optind;
			printf ("option %c\n", c);
			break;
		 
		// filetype option
		
		// filepaths options
		
		// mplayer options
		case 'f':
			printf("%s\n", "fs");
			string_push(&ma->prefix_args,"-fs");
			break;
			
		// playlist options
		
		// player options
		case 'm':
			printf("%s\n", "mp");
			ma->player = P_MPLAYER;
			break;
		// output options
		
		// other options 
		
		default:
			printf ("?? getopt returned character code 0%o ??\n", c);
		}
	}
	return ma;
}

int main (int argc, char **argv) {

	MediaArgs *ma = option_parser(argc, argv);
	
	if (optind < argc) {
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		printf ("\n");
	}
	
	print_media_args(ma);
	return 0;
}
