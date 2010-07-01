#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Block.h>
#include "option_parser.h"
#include "string.h"
typedef void (^voidBlock)();

#define csElement const static Element
csElement H_filetype[] ={
	      
};        
csElement H_filepath[] ={
	      
};        
csElement H_mplayer[] = { 
	{{"fs", no_argument, 0, 'f'}, "", "plays file(s) in fullscreen"},
	{{"top", no_argument, 0, 't'}, "", "Add profile t - afloat + 360p"}
};
csElement H_playlist[] ={
	
};
csElement H_player[] ={
	{{"mplayer", no_argument, 0, 'm'}, "", "plays file(s) using mplayer"}
};
csElement H_output[] ={
		
};
csElement H_other[] ={
	{{"help", no_argument, 0, 'h'}, "", "Shows the help"},
	{{"print_opt", no_argument, 0, 'Z'}, "", "Shows the opt struct"}
};

 
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

MediaArgs *option_parser(int argc, char **argv) {

	int c, digit_optind = 0, option_index =0;
	MediaArgs *ma = new_media_args();
	
	// array of lengths
	int e_len = sizeof(Element), t_len = 0, index = 0, s_index = 0;
	int lens[] = {
		sizeof(H_filetype) / e_len,
		sizeof(H_filepath) / e_len,
		sizeof(H_mplayer)  / e_len,
		sizeof(H_playlist) / e_len,
		sizeof(H_player)   / e_len,
		sizeof(H_output)   / e_len,
		sizeof(H_other)    / e_len
	};
	const static Element *ele[] = {
		 &H_filetype[0],
		 &H_filepath[0],
		 &H_mplayer[0] ,
		 &H_playlist[0],
		 &H_player[0]  ,
		 &H_output[0]  ,
		 &H_other[0]   
	};
	
	// calc total length
	for(int i = 0; i < sizeof(lens)/sizeof(int); ++i) t_len += lens[i];
	struct option opts[t_len+1];
	char letters[t_len*2+1]; // since opt with arg needs a : after it
	// builds the options array.
	for(int i = 0; i < sizeof(ele) / sizeof(size_t); ++i) {
		for(int j = 0; j < lens[i]; ++j){
			opts[index++] = ele[i][j].opt;
			letters[s_index++] = ele[i][j].opt.val;
		}
	}
	letters[s_index] = '\0';
	voidBlock test = ^ (MediaArgs *ma ) {
		printf("%s\n", "abc");
	};
	
	
	while ((c = getopt_long(argc, argv, letters, opts, &option_index)) != -1) {
		int this_option_optind = optind ? optind : 1;
		switch (c) {
		case 'h':
			print_help();
			exit(0);
			break;			
		case 0:
			printf("%s\n", "jj");
			printf ("option %s", opts[option_index].name);
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
			string_push(&ma->prefix_args,"-fs");
			break;
		case 't':
			string_push(&ma->prefix_args, "-profile t");
			break;
			
		// playlist options
		
		// player options
		case 'm':
			ma->player = P_MPLAYER;
			break;
			
		// output options
		
		// other options 
		case 'Z':
			print_media_args(ma);
			exit(0);
		// default:
		// 	exit(0);
		}
	}
	return ma;
}

void print_help(){
	const static HelpLink help[] = {
		{ "Filetype", sizeof(H_filetype) / sizeof(Element), &H_filetype[0] },
		{ "Filepath", sizeof(H_filepath) / sizeof(Element), &H_filepath[0] },
		{ "Mplayer",  sizeof(H_mplayer)  / sizeof(Element), &H_mplayer[0]  },
		{ "Playlist", sizeof(H_playlist) / sizeof(Element), &H_playlist[0] },
		{ "Player",   sizeof(H_player)   / sizeof(Element), &H_player[0]   },
		{ "Output",   sizeof(H_output)   / sizeof(Element), &H_output[0]   },
		{ "Other",    sizeof(H_other)    / sizeof(Element), &H_other[0]    },
	};
	
	size_t length = sizeof(help) / sizeof(HelpLink);
	const char *s_exp = "\t%-3s %-15s %s\n";
	
	for(int i = 0; i < length; ++i){
		printf("\n%s\n", help[i].grouping);
		for(int j = 0; j < help[i].length; j++){
			
			const struct option *optr = &help[i].links[j].opt;
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
}

void print_media_args(MediaArgs *ma) {
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

int main (int argc, char **argv) {

	option_parser(argc, argv);
	
	if (optind < argc) {
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		printf ("\n");
	}
	
	return 0;
}
