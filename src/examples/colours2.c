#include <include/colours.h>
#include <stdio.h>

int main (int argc, char const *argv[]) {
	char *s = "r";
	printf("%s%s%s\n", COLOUR(s,BLUE) );
	return 0;
}