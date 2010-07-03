#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include "option_parser.h"
#include "string.h"
#include "option_parser_private.h"

 
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

	int c, option_index = 0;
	MediaArgs *ma = new_media_args();
	// pointer to block contain the function for the chararcter.
	const static VoidBlock *blocks[MAX_OPT_BLOCKS]; 
	
	// array of the lengths
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
	
	// calculate  total length
	for(int i = 0; i < sizeof(lens)/sizeof(int); ++i) t_len += lens[i];
	
	struct option opts[t_len+1 * 2];
	char letters[t_len*2+1]; // since opt with arg needs a : after it
	// builds the options array.
	for(int i = 0; i < sizeof(ele) / sizeof(size_t); ++i) {
		for(int j = 0; j < lens[i]; ++j){
			opts[index++] = ele[i][j].opt;
			
			if (ele[i][j].neg == true){
				struct option o2 = ele[i][j].opt;
				o2.val +=128;
				char *c2 = malloc(strlen(o2.name +1 +3));
				sprintf(c2,"no-%s",o2.name);
				o2.name = c2;
				opts[index++] = o2;
			}
			
			letters[s_index++] = ele[i][j].opt.val;
			blocks[ele[i][j].opt.val] = &ele[i][j].block;
			if (ele[i][j].neg) blocks[ele[i][j].opt.val+128] = &ele[i][j].block;
		}
	}
	
	letters[s_index] = '\0';
	// parsers the options
	while ((c = getopt_long(argc, argv, letters, opts, &option_index)) != -1) {
		// int this_option_optind = optind ? optind : 1;
		if (c == '?') exit(1);
		(*blocks[c])(ma, c); // calls the related block
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
	const char *s_exp = "\t%-3s %-15s ";
	const char *h_exp = "\t%-3s %-15s %-s\n";
	
	for(int i = 0; i < length; ++i){
		printf("\n%s\n", help[i].grouping);
		for(int j = 0; j < help[i].length; j++){
			
			const struct option *optr = &help[i].links[j].opt;
			// makes the space for the short arg
			char short_opt[3] = ""; 
			if (optr->val < 128) sprintf(short_opt, "-%c",optr->val);
			// makes the space for the long arg
			char long_opt[3 + 5 + strlen(optr->name)];
			
			if (*optr->name != '\0'){
				if (help[i].links[j].neg == true) 
					 sprintf(long_opt, "--[no-]%s",optr->name);
				else sprintf(long_opt, "--%s",optr->name);
			}else long_opt[0] = '\0';
			
			struct ttysize ts;
		    ioctl(0, TIOCGSIZE, &ts);
		    
			
			const char *ho = help[i].links[j].help; 
			int h_len = strlen(ho), h_num = ts.ts_cols - 28, h_cur = h_num;
			if (h_num < 5) h_num = 5;
			char hh[h_num + 2]; 
			
			// puts short words like 'the', 'or' and 'then' on the next like.
			bool changed = false;
			for(int i = h_cur; i > 0 && (h_cur - i <= 4) ; i--){
				if (ho[i] == ' '){
					h_cur = i;
					changed = true;
					break;
				}
			}
			
			strncpy(hh, help[i].links[j].help, h_cur );
			// if the the word is spilt a =- is used.
			if (changed == true){
				hh[h_cur] = '\0';
			}else{
				hh[h_cur-1]  = '-';
				hh[h_cur] = '\0';
				h_cur--;
			}
			
			if (optr->has_arg == required_argument){
				// joins long opt and arg to print nicely
				char name_arg[strlen(help[i].links[j].arg) + strlen(long_opt) + 4];
				sprintf(name_arg, "%s [%s]", long_opt, help[i].links[j].arg );
				printf(s_exp, short_opt, name_arg);
			}else{
				printf(s_exp, short_opt, long_opt);	
			}
			
			//TODO spaces on multiple  lines
			printf("%s\n",hh );
			while(h_len - h_cur > 0){
				strncpy(hh, &ho[h_cur], h_num);
				hh[h_num] = '\0';
				printf(h_exp, "", "",hh);
				h_cur += h_num;
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
#undef strcheck
#undef print_args
#undef print_hex
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
