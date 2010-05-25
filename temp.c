#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "string_util.h"
#include "media.h"



int main (int argc, char  *argv[]) {
	int i = match("Kidou Senshi Gundam I Gekijouban part 1.mkv", argv[1]);
	printf("%i\n", i);
	
	return 0;
}