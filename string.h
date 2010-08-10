#ifndef STRING_STURCT_HEAD
#define STRING_STURCT_HEAD

typedef struct {
	char *str;
	int length;
	int index;
} String;

void new_string(String *s, int length );
void string_push(String *s, char *str);
void string_push_m(String *s,int length, ...);
#endif