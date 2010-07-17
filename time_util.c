#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "string_util.h"
#include "time_util.h"

void test_day_diff();
#define PARSE_ERR(str) fprintf(stderr, "invalid type %s\n",str);

const char* time_spec[][5] = {
	{"day", "days",""}, {"hour", "hours",""},
	{"min", "mins", "minute", "minutes",""}, {"sec", "secs","second","second",""}
};

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
	//printf("\n%s\n", "start");
	struct tm* tm = currentTime();

	if (length >= 1) {

		if( match(*str, "[0-9]{1,2} [a-zA-Z]{3}") > 0) {
			strptime(*str, "%d %b", tm);
			str++;
			length--;
			
		} else if( match(*str, "[0-9]{1,2}(th|st|nd)") > 0) {
			strptime(*str, "%dth", tm);
			str++;
			length--;
			
		} else if ( match(*str, DATE_TIME_REGEX) > 0) {
			strptime(*str, "%F %H:%M:%S", tm);
			return tm;
			
		} else if( match(*str, DATE_REGEX) > 0) {
			strptime(*str, "%F", tm);
			str++;
			length--;
			
		//FIXME time bug
		} else if ( match(*str, DATE_TIME_UK_REGEX) > 0) {
			strptime(*str, "%d/%m/%y %H:%M:%S", tm);
			return tm;
		}
	}

	if (length >= 3 && isdigit(**str)) {
		type_a(str, length, tm);
		str    += 3;
		length -= 3;
	}

	if (length >= 2) {
		if (strcasecmp(*str, "at") == 0) {
			type_b(str, length, tm);
			str    += 2;
			length -= 2;
		}
	}


	return tm;
}

// n (day/hour/minute) ago/after
void type_a(char **str, int length, struct tm* tm ) {
	//printf("%s\n", "type A - n time ago/after ");
	long num = strtol(str[0], NULL, 10);
	if (num == 0 || *str[1] == '\0') {
		PARSE_ERR("a");
		return;
	}

	T_Spec spec = -1; T_Pos pos;
	const char **arr = NULL;
#define TIME_SPEC_TEST(arr,str,spec_v) \
		arr = time_spec[spec_v]; \
		while(**arr != '\0'){ \
			if(strcasecmp(str, *arr) == 0) spec = spec_v; \
			arr++; \
		}

	switch(*str[1]) {
	case 'D':
	case 'd': TIME_SPEC_TEST(arr, str[1], 0);
		break;

	case 'H':
	case 'h': TIME_SPEC_TEST(arr, str[1], 1);
		break;

	case 'M':
	case 'm': TIME_SPEC_TEST(arr, str[1], 2);
		break;

	case 'S':
	case 's': TIME_SPEC_TEST(arr, str[1], 3);
		break;
	}

	if (arr == NULL || spec == -1 ) {PARSE_ERR("a-spec"); return;}
	//printf("spec: %i\n", spec);

	if (strcasecmp(str[2], "ago") == 0)       pos = T_BEFORE;
	else if(strcasecmp(str[2], "after") == 0) pos = T_AFTER;
	else { PARSE_ERR("A - t-pos"); return; }
	//printf("pos: %i num: %li \n", pos, num);

	switch(spec) {
	case T_DAYS:
		tm->tm_mday += num * pos;
		break;
	case T_HOURS:
		tm->tm_hour += num * pos;
		break;
	case T_MINS:
		tm->tm_min += num * pos;
		break;
	case T_SECS:
		tm->tm_sec += num * pos;
		break;
	}
	timegm(tm);
}

// at hh:mm
void type_b(char **str, int length, struct tm* tm ) {
	//printf("%s\n", "type B - at nn:nn ");
	char temp[5];
	long hours, minutes, len = strlen(str[1]);

	if(len < 3 || len > 5) return;
	char *sep = strrchr(str[1], ':');
	if (sep == NULL) return;

	hours = atol(str[1]);
	strncpy(temp, (sep + 1),  (len - (sep - str[1]) - 1) );
	minutes = atol(temp);

	tm->tm_hour = hours;
	tm->tm_min  = minutes;

}

// int main (int argc, char *argv[]) {
// 	struct tm* timeinfo = currentTime();
// 	struct tm* newinfo = argc > 1 ? parse_time(&argv[1], argc - 1) : timeinfo;
// 
// 	char now[20], after[20];
// 	MAKE_TIME_STR(now, timeinfo);
// 	MAKE_TIME_STR(after, newinfo);
// 
// 	char *str_days[7] = {"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat"};
// 	//printf("\n\nnow  :%s %s UTC \nafter:%s %s UTC\n",
// 		   now,   str_days[timeinfo->tm_wday],
// 		   after, str_days[newinfo->tm_wday]);
// 
// 	return 0;
// }

void test_day_diff() {
	struct tm* timeinfo = currentTime();
	char now[20], after[20];
	MAKE_TIME_STR(now, timeinfo);

	timegm(timeinfo);
	MAKE_TIME_STR(after, timeinfo);

	char *str_days[7] = {"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat"};

	for (int day = 0 ; day < 7; day++) {
		//printf("	day %i %s\n", day, str_days[day]);
		for (int j = 0 ; j < 7; j++) {
			int k = day_diff(day, j);
			//printf(" %i %s is %i\n", j, str_days[j], k );
		}
	}
}
