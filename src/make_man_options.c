#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <include/media_args.h>

void print_help(char *arg){}
void print_media_args(MediaArgs *ma){}

#include <include/option_parser_private.h>

static void sub_print_help2(const Element *ele);
void print_help2();

int main (int argc, char const *argv[]) {
	print_help2();
	return 0;
}

void print_help2(){
	size_t length = sizeof(HELP_LINK) / sizeof(HelpLink), start = 0;
	
	for(int i = start; i < length; ++i){
		printf("\n.I %i  %s\n.BR\n",i,  HELP_LINK[i].grouping);
		for(int j = 0; j < HELP_LINK[i].length; j++){
			sub_print_help2(&HELP_LINK[i].links[j]);
		}
	}
 
}

// prints the element
static void sub_print_help2(const Element *ele){
	printf("%s\n", "\n.TP 0.5i");
	const char *s_exp = ".BR \"%-3s\", \"  %-20s\" \n";
	
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
	

	int ts_cols = 8000;
	const char *ho = ele->help; 
	int h_num = ts_cols - 33, h_cur = h_num;
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
	
	printf("%s\n",hh );
	// while(h_len - h_cur > 0){
	// 	strncpy(hh, &ho[h_cur], h_num);
	// 	hh[h_num] = '\0';
	// 	printf(h_exp, "", "",hh);
	// 	h_cur += h_num;
	// } 
}

