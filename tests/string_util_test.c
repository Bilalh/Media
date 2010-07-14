#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "../string_util.h"
int main (int argc, char const *argv[]) {
	char *s = "fma|kga";
	char *r = str_replace(s, strlen(s), "fma", "Fullmetal Alchemist");
	printf("s %lu r %lu \n", strlen(s) , strlen(r));
	printf("%s\n", s);
	printf("%s\n", r);
	
	return 0;
}
