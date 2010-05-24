#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "string_util.h"
#include "media.h"



int main (int argc, char  *argv[]) {
	char *s_arr[] = {"fma|bbb", "q", "c"};
	
	int length = sizeof(s_arr) / sizeof(size_t);
	char *result = spilt_args(s_arr, length,".*");
	
	printf("%s\n", result);
	return 0;
}