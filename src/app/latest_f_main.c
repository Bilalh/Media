#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <include/history.h>

int main (int argc, char *argv[]) {
	
	char *num = "11";
	bool use_num = true;
	if (argc == 2){
		char *t = argv[1];
		while (*t != '\0'){
			if( isdigit(*t) == 0 ){
				use_num = false;
				break;
			}
			t++;
		}
		if (use_num) num = argv[1];
	}
	
	print_latest_with_finished(num);
	
	return 0;
}
