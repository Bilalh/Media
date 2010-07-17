
#include <stdio.h>
#include "../time_util.h"
#include <stdbool.h>

#define TM_ERROR_MSG(args...) printf("   !%-10s expected %-3i is %-3i\n",args);
#define VAILDATE_TM(exp,act,type) \
	if (act != exp){\
		if (error == false){\
			error = true;\
			puts("wrong");\
		}\
		TM_ERROR_MSG( type, exp, act);\
	}

#define SEP "---------------------------------------------------------"
#define printTitle(title) printf("\n%s\n%s\n%s\n",SEP,title,SEP)
#define printTest(title,after,day)  printf("%-15s %-26s %-3s : ", title,after,day )

void ttest(char **given, struct tm* tm ){
	struct tm *newt = parse_time(given,3);
	bool error = false;
	
	char now[20], after[20];
	MAKE_TIME_STR(now, tm);
	MAKE_TIME_STR(after, newt);
	char *str_days[7] = {"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat"};

	printTest(*given,after,str_days[tm->tm_wday]);
	
	VAILDATE_TM(tm->tm_min,  newt->tm_min,  "tm_min")	
	VAILDATE_TM(tm->tm_hour, newt->tm_hour, "tm_hour")	
	VAILDATE_TM(tm->tm_mday, newt->tm_mday, "tm_mday")	
	VAILDATE_TM(tm->tm_mon,  newt->tm_mon,  "tm_mon")	
	VAILDATE_TM(tm->tm_year, newt->tm_year, "tm_year")	
	VAILDATE_TM(tm->tm_wday, newt->tm_wday, "tm_wday")	
	VAILDATE_TM(tm->tm_yday, newt->tm_yday, "tm_yday")	
	VAILDATE_TM(tm->tm_min,  newt->tm_min,  "tm_min")	

	if (error){
		printf("\n    expected :%s %s UTC \n    actual   :%s %s UTC\n\n",
				now,   str_days[tm->tm_wday],
				after, str_days[newt->tm_wday]
			  );
	}else{
		puts("correct");			
	}

}

// TODO Make spilt function
int main (int argc, char const *argv[]) {
	
	printTitle("Rel time");

	{
		struct tm *tm = currentTime();
		tm->tm_hour -= 2;
		timegm(tm);
		char *ss[] = {"2", "hours", "ago"};
		ttest(ss,tm);
	}
	return 0;
}
