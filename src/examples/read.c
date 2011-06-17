#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
	char *s;
	read_history("ddd");
 
	s = readline(NULL);

	printf("characters read: %s\n", s);
	
	add_history(s);
	write_history("ddd");        
	
	free(s);

	return 0;
}
