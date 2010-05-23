#include <stdlib.h>
#include <string.h>
char *strrep(char *s, char *sub, char *rep) {
	int len     = strlen(s);
	int rep_len = strlen(rep);
	int sub_len = strlen(sub);
	char *r     = malloc(len + 100);

	// counters for s and r
	int is = 0, ir = 0;
	while(is < len) {
		if (s[is] == *sub && strncmp(&s[is], sub, sub_len) == 0 ) {
			strncpy(&r[ir], rep , rep_len);
			ir += rep_len;
			is += sub_len;
		} else { // move 
			r[ir++] = s[is++];
		}
	}
	r[ir] = '\0';
	return r;
}

char *strrep2(char *s, char *sub, char *rep) {
	int len     = strlen(s);
	int rep_len = strlen(rep);
	int sub_len = strlen(sub);
	char *r     = malloc(len + 100);

	// counters for s and r
	int is = 0, ir = 0, i=0;
	while(is < len) {
		if (s[is] == *sub && strncmp(&s[is], sub, sub_len) == 0 ) {
			strncpy(&r[ir],&s[i],is-i );
			strncpy(&r[ir], rep , rep_len);
			ir += rep_len;
			is += sub_len;
		} else { 
			is++;
		}
	}
	r[ir] = '\0';
	return r;
}

int main (int argc, char const *argv[]) {
	char *s = "123456789";
	char *r = strrep(s, "4", "abc");
	printf("s %lu r %lu \n", strlen(s) , strlen(r));
	printf("%s\n", s);
	printf("%s\n", r);
	
	return 0;
}