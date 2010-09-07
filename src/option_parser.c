#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#include "option_parser.h"
#include "option_parser_private.h"
#include "string.h"

static Element *Element_ptr[128];
static void sub_print_help(const Element *ele);

// creates a new MediaArgs with the default values. 
MediaArgs *new_media_args() {

	MediaArgs *ma = malloc(sizeof(MediaArgs));
	// all unmentioned  set to NULL.
	MediaArgs m = {
		// Selection
		.excludes.exclude = false,
		.excludes.length  = 1,
		.excludes.index   = 0,
		.excludes.str_arr = malloc(sizeof(char*) * 1 ),
		
		.newest_only = false,
		.sub_dirs    = false,
		.types       = T_VIDEO,
		.root_dir    = NULL,
		
		// Playlist
		// .pl_name       = "zzplaylist",
		.pl_dir        = NULL,
		.pl_format     = F_NONE,
		.pl_output     = PL_NONE,
		.pl_shuffle    = false,
		
		// Prefs
		.hash_location = NULL,
		.use_hash      = false,
		.status        = S_NONE,
		.write_history = false,
		.updated       = false,
		.shortcuts     = true,
		
		// Player
		.player = P_NONE,
		.afloat = false,
		
		.prefix_args.length = m.postfix_args.length = 16,
		.prefix_args.index  = m.postfix_args.index  = 0,
		
		.prefix_args.str    = malloc(sizeof(char) * m.prefix_args.length),
		.postfix_args.str   = malloc(sizeof(char) * m.postfix_args.length),
		
		.nice_repeat = false,
		.nice_random = false
	};
	
	m.prefix_args.str[0]  = '\0',
	m.postfix_args.str[0] = '\0',
	
	*ma = m;
	return ma;
}

// Parser the options and returns a MediaArgs struct
MediaArgs *option_parser(int argc, char **argv) {

	int c, option_index = 0;
	MediaArgs *ma = new_media_args();
	// pointer to block contain the function for the chararcter.
	const static VoidBlock *blocks[MAX_OPT_BLOCKS]; 
	// index counters
	int t_len = 0, index = 0, s_index = 0;
	// calculate  total length
	for(int i = 0; i < HELP_L_LEN; ++i) t_len +=  HELP_LINK[i].length;
	
	#define ele(i) HELP_LINK[i].links
	
	struct option opts[t_len *2 + 1 ];
	char letters[t_len * 3 + 1]; // since opt with arg needs a : after it
	// builds the options array.
	for(int i = 0; i < HELP_L_LEN; ++i) {
		for(int j = 0; j < HELP_LINK[i].length; ++j) {
			opts[index++] = ele(i)[j].opt;

			if (ele(i)[j].neg == true) {
				struct option o2 = ele(i)[j].opt;
				o2.val += 128;
				// 3 for 'no-' 1 for \0
				char *c2 = malloc(strlen(o2.name + 3 + 1));
				sprintf(c2, "no-%s", o2.name);
				o2.name = c2;
				opts[index++] = o2;
			}

			if ( VAILD_ASCII(ele(i)[j].opt.val) ) {
				letters[s_index++] = ele(i)[j].opt.val;
				Element_ptr[ele(i)[j].opt.val] = (Element*) &ele(i)[j];
				if (ele(i)[j].opt.has_arg == required_argument) letters[s_index++] = ':';
				if (ele(i)[j].opt.has_arg == optional_argument) {
					letters[s_index++] = ':';
					letters[s_index++] = ':';
				}

			}
			blocks[ele(i)[j].opt.val] = &ele(i)[j].block;
			if (ele(i)[j].neg) {
				blocks[ele(i)[j].opt.val + 128] = &ele(i)[j].block;
			}
		}
	}

	// last have to be all zeros
	opts[index].name    = 0;
	opts[index].val     = 0;
	opts[index].has_arg = 0;
	opts[index].flag    = 0;
	letters[s_index] = '\0';
	
	// parsers the options
	while ((c = getopt_long(argc, argv, letters, opts, &option_index)) != -1) {
		// int this_option_optind = optind ? optind : 1;
		if (c == '?') exit(1);
		(*blocks[c])(ma, c,optarg); // calls the related block
	}
	
	return ma;
	#undef ele
}

// prints the help, section or letter if specifed
void print_help(char *arg){
	
	size_t length = sizeof(HELP_LINK) / sizeof(HelpLink), start = 0;
	
	// print only the specified section by name or number.  
	if (arg != NULL && *arg != '\0'){
		int number = -1;
		if ( *arg == ',' ){
			++arg;
			while (( *arg != '\0' && isascii(*arg) )){
				if (Element_ptr[(int)*arg] != NULL ){
					sub_print_help(Element_ptr[(int)*arg]);
				}else{
					printf("\t-%c %31s\n",*arg,"NOT defined");
				}
				++arg;
			}
			return;
		}else if ( sscanf(arg, "%i", &number ) == 1 && number < length && number >= 0 ){
			start = number;
			length = start + 1;
		}else{
			for (int i = 0; i < length;i++ ){
				if (strncasecmp(arg, HELP_LINK[i].grouping, strlen(arg)) == 0 ){
					start  = i; 
					length = start + 1;
					break;
				}
			}
		}
	}
	
	for(int i = start; i < length; ++i){
		printf("\n%i. %s\n",i, HELP_LINK[i].grouping);
		for(int j = 0; j < HELP_LINK[i].length; j++){
			sub_print_help(&HELP_LINK[i].links[j]);
		}
	}
 
}

// prints the element
static void sub_print_help(const Element *ele){
	const char *s_exp = "\t%-3s %-20s ";
	const char *h_exp = "\t%-3s %-20s %-s\n";
	const struct option *optr = &ele->opt;
	// makes the space for the short arg
	char short_opt[3] = ""; 
	if (optr->val < 128) sprintf(short_opt, "-%c",optr->val);
	// makes the space for the long arg
	char long_opt[3 + 5 + strlen(optr->name)];
	
	if (*optr->name != '\0'){
		if (ele->neg == true) 
			 sprintf(long_opt, "--[no-]%s",optr->name);
		else sprintf(long_opt, "--%s",optr->name);
	}else long_opt[0] = '\0';
	
	// gets the term size.
	struct ttysize ts; 
	ioctl(0, TIOCGSIZE, &ts);
	
	const char *ho = ele->help; 
	int h_len = strlen(ho), h_num = ts.ts_cols - 33, h_cur = h_num;
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
	
	strncpy(hh, ele->help, h_cur );
	// if the the word is spilt a - is used.
	if (changed == true){
		hh[h_cur] = '\0';
	}else{
		hh[h_cur-1]  = '-';
		hh[h_cur] = '\0';
		h_cur--;
	}
	
	if (optr->has_arg != no_argument){
		// joins long opt and arg to print nicely
		char name_arg[strlen(ele->arg) + strlen(long_opt) + 4];
		char cS = '{', cE = '}';
		if (optr->has_arg == optional_argument){
			cS = '['; cE = ']';
		}
		sprintf(name_arg, "%-9s %c%s%c", long_opt,cS, ele->arg, cE );
		printf(s_exp, short_opt, name_arg);
	}else{
		printf(s_exp, short_opt, long_opt);	
	}
	
	//TODO spacing on multiple lines
	printf("%s\n",hh );
	while(h_len - h_cur > 0){
		strncpy(hh, &ho[h_cur], h_num);
		hh[h_num] = '\0';
		printf(h_exp, "", "",hh);
		h_cur += h_num;
	} 
}

// prints the media_args struct
void print_media_args(MediaArgs *ma) {
#define truth(boolean) (boolean ? "true" : "false" )
#define nullcheck(val) (val == NULL ? "NULL" : val )
#define strcheck(s)    (s.str != NULL ? s.str : "NULL" )
#define print_args(title,value) printf("%20s: %s\n",  title, value);
#define print_hex(title,value) printf("%20s: 0x%x\n",  title, value);
#define print_int(title,value) printf("%20s: %i\n",  title, value);

	printf("Selection\n");
	print_args("exclude",      truth(ma->excludes.exclude));
	print_int("exclude index",  ma->excludes.index);
	print_int("exclude length", ma->excludes.length);
	print_args("excludes arr",  "" );
	for(int i = 0; i < ma->excludes.index; ++i){
		printf("%17s[%i]: %s\n","", i, ma->excludes.str_arr[i] );
	}
	
	
	print_args("root_dir",    ma->root_dir)
	print_args("newest_only", truth(ma->newest_only));
	print_args("sub_dirs",    truth(ma->sub_dirs));
	print_hex("types",        ma->types);

	printf("Playlist\n");
	print_args("pl_dir",     nullcheck(ma->pl_dir));
	print_args("pl_name",    truth(ma->pl_name));
	print_hex ("pl_format",  ma->pl_format);
	print_hex ("pl_output",  ma->pl_output);
	print_args("pl_shuffle", truth(ma->pl_shuffle));
                              
	printf("Prefs\n");
	print_args("hash_location", nullcheck(ma->hash_location));
	print_args("use_hash",      truth(ma->use_hash));
	print_hex ("types",         ma->types);
	print_args("write_history", truth(ma->write_history));
	print_args("updated",       truth(ma->updated));
	print_args("shortcuts",     truth(ma->shortcuts));

	printf("Player\n");
	print_hex ("player",       ma->player);
	print_args("afloat",       truth(ma->afloat));
	print_args("prefix_args",  strcheck(ma->prefix_args));
	print_args("postfix_args", strcheck(ma->postfix_args));
	print_args("nice_repeat",  truth(ma->nice_repeat));
	print_args("nice_random",  truth(ma->nice_random));

#undef truth
#undef nullcheck
#undef strcheck
#undef print_args
#undef print_hex
}

