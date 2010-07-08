#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include "option_parser.h"
#include "string.h"
#include "option_parser_private.h"

static Element *Element_ptr[128];
static void sub_print_help(const Element *ele);

// creates a new MediaArgs with the default values. 
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
	
	struct option opts[t_len+1 * 2];
	char letters[t_len * 3 + 1]; // since opt with arg needs a : after it
	// builds the options array.
	for(int i = 0; i <HELP_L_LEN; ++i) {
	    for(int j = 0; j < HELP_LINK[i].length; ++j){
	    	opts[index++] = ele(i)[j].opt;
	    	
	    	if (ele(i)[j].neg == true){
	    		struct option o2 = ele(i)[j].opt;
	    		o2.val +=128;
	    		char *c2 = malloc(strlen(o2.name +1 +3));
	    		sprintf(c2,"no-%s",o2.name);
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
	    	if (ele(i)[j].neg) blocks[ele(i)[j].opt.val+128] = &ele(i)[j].block;
	    }
	}
	
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

// prints the help, by section or letter if specifed
void print_help(char *arg){
	// TODO for letter only
	
	size_t length = sizeof(HELP_LINK) / sizeof(HelpLink), start = 0;
	
	// print only the specified section by name or number.  
	if (arg != NULL && *arg != '\0'){
		int number = -1;
		if ( *arg++ == ',' ){
			// FIXME invaild ascii chars
			while (( *arg != '\0' && isascii(*arg) )){
				sub_print_help(Element_ptr[(int)*arg++]);
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
	const char *s_exp = "\t%-3s %-18s ";
	const char *h_exp = "\t%-3s %-18s %-s\n";
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
	int h_len = strlen(ho), h_num = ts.ts_cols - 31, h_cur = h_num;
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
