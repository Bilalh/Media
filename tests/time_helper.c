#include <stdlib.h>
#include <string.h>

#include "include/time_helper.h"


#define TM_ERROR_MSG(args...) printf("   !%-10s expected %-3i is %-3i\n",args);
#define VAILDATE_TM(exp,act,type) \
	if (act != exp){\
		if (error == false){\
			error = true;\
			puts("WRONG");\
		}\
		TM_ERROR_MSG( type, exp, act);\
	}

#define printTimeTest(title,after,day)  printf("%-31s %-19s, %-3s : ", title,after,day )

bool time_test_start(char *given, struct tm* tm ){
	
	// spilts the string with pointers to parts of the string
	char *to_spilt = strdup(given), *sep = " ", *s_ptr, *word;
	int slength = 7;
	char **strs = malloc(sizeof(char*) * slength);
	int i = 0;
	
	for(word = strtok_r(to_spilt, sep, &s_ptr); word != NULL; 
			word = strtok_r(NULL, sep, &s_ptr)) {
		strs[i++] = word;
		if (i >= slength ){
			slength = slength * 2 + 1;
			void * temp = realloc(strs, sizeof(char*) * slength);
			if (temp != NULL){
				strs = temp;
			}else{
				fprintf(stderr, "realloc  failed in time test start");
				exit(4);
			}
			
		}
	}
	
	struct tm *newt = parse_time(strs, i);
	free(to_spilt);
	bool error = false;
	
	char now[20], after[20];
	MAKE_TIME_STR(now, tm);
	MAKE_TIME_STR(after, newt);
	char *str_days[7] = {"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat"};
	
	printTimeTest(given,after,str_days[tm->tm_wday]);
	
	VAILDATE_TM(tm->tm_min,  newt->tm_min,  "tm_min")	
	VAILDATE_TM(tm->tm_hour, newt->tm_hour, "tm_hour")	
	VAILDATE_TM(tm->tm_mday, newt->tm_mday, "tm_mday")	
	VAILDATE_TM(tm->tm_mon,  newt->tm_mon,  "tm_mon")	
	VAILDATE_TM(tm->tm_year, newt->tm_year, "tm_year")	
	VAILDATE_TM(tm->tm_wday, newt->tm_wday, "tm_wday")	
	VAILDATE_TM(tm->tm_yday, newt->tm_yday, "tm_yday")	

	if (error){
		printf("\n    expected :%s %s UTC \n    actual   :%s %s UTC\n\n",
				now,   str_days[tm->tm_wday],
				after, str_days[newt->tm_wday]
			  );
	}else{
		puts("correct");			
	}
	return !error;
}

#undef printTimeTest