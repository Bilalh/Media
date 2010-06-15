#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"
#include "string_util.h"
bool updateHistory(char **filenames){
    
    while(**filenames){
        printf("%s\n", *filenames);
        char **ans = ep_num(*filenames);
        if (ans[0] != NULL) {
			long l = strtol(ans[0] + 1, NULL, 10);
			if (l == 0 ) l++;
			
			int index = ans[1] != NULL ? 1 : 0;
            // char *s = malloc(sizeof(char) * (ans[index] - *filenames) );
            char s[ans[index] - *filenames+1]; // 1 for \0
			printf("len %i\n", (ans[index] - *filenames) );
			strncpy(s, *filenames, ans[index] - *filenames);
            s[ans[index] - *filenames] ='\0';
            
			printf("title %s.\n", s);
			printf("num   %li.\n", l);
			
		}
        filenames++;
    }
    return false;
}
