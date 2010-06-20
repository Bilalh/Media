#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "time_util.h"
void test_day_diff();


#define PARSE_ERROR(test,string,structtmPtr) \
	if (test){ \
		fprintf(stderr, "invalid type %s\n",string); \
		return structtmPtr;\
	}

#define PARSE_ERR(str) fprintf(stderr, "invalid type %s\n",str);

struct tm* currentTime() {
	time_t tt;
	time(&tt);
	// struct tm *timeinfo = gmtime(&tt);
	struct tm  *timeinfo  = malloc(sizeof(struct tm));
	gmtime_r(&tt, timeinfo);
	return timeinfo;
}

// calulates the near specifed 'other_day' from 'day'
int day_diff(int day, int other_day) {
	int res = 0;
	if (day != other_day ) {
		for(res = 1; res <= 3; ++res) {
			if ((day + res) % 7 ==  other_day) {
				break;
			}
			if((other_day + res) % 7 ==  day ) {
				res = -res;
				break;
			}
		}
	}
	return res;
}

int day_last(int day, int other_day) {
	if (day >= other_day) return -(day - other_day);
	else                  return -(day + 7 - other_day);
}

int day_future(int day, int other_day) {
	if (other_day >= day) return (other_day - day);
	else                  return (other_day + 7 - day);
}

struct tm *parse_time(char **str, int length) {
	printf("\n%s\n", "start");
	struct tm* tm = currentTime();

	if (isdigit(**str)) {
		printf("%s\n", "type A - n time ago/after ");
		long num = strtol(str[0], NULL, 10);
		PARSE_ERROR(length < 3 || num == 0, "A", tm)

		T_Spec spec; T_Pos pos;
		for (spec = 0 ; spec < 3; spec++) {
			char *temp = strcasestr(time_spec[spec], str[1]);
			if (temp != NULL && strlen(temp) > 3) break;
		}
		printf("spec: %s\n", time_spec[spec] );

		if (strcasecmp(str[2], "ago") == 0)       pos = T_BEFORE;
		else if(strcasecmp(str[2], "after") == 0) pos = T_AFTER;
		else PARSE_ERR("A - t-pos");
		printf("pos: %i\n", pos);
		
		switch(spec){
			case T_DAYS:
				tm->tm_mday += num *pos;
				break;
			case T_HOURS:
				tm->tm_hour += num *pos;
				break;
			case T_MINS:
				tm->tm_min += num *pos;
				break;
			case T_SECS:
				tm->tm_sec += num *pos;
				break;
		}
		
	}
	
	mktime(tm);
	return tm;
}

int main (int argc, char *argv[]) {
	struct tm* timeinfo = currentTime();
	struct tm* newinfo = argc > 1 ? parse_time(&argv[1], argc - 1) : timeinfo;

	char now[20], after[20];
	MAKE_TIME_STR(now, timeinfo);
	MAKE_TIME_STR(after, newinfo);

	char *str_days[7] = {"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat"};
	printf("\n\n%s %s\n%s %s\n",
		   now,   str_days[timeinfo->tm_wday],
		   after, str_days[newinfo->tm_wday]);

	return 0;
}

void test_day_diff() {
	struct tm* timeinfo = currentTime();
	char now[20], after[20];
	MAKE_TIME_STR(now, timeinfo);

	timegm(timeinfo);
	MAKE_TIME_STR(after, timeinfo);

	char *str_days[7] = {"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat"};

	for (int day = 0 ; day < 7; day++) {
		printf("	day %i %s\n", day, str_days[day]);
		for (int j = 0 ; j < 7; j++) {
			int k = day_diff(day, j);
			printf(" %i %s is %i\n", j, str_days[j], k );
		}
	}
}
