#include <stdio.h>

#include "time_util.h"

struct tm* currentTime() {
	time_t tt;
	time(&tt);
	struct tm *timeinfo = gmtime(&tt);
	return timeinfo;
}

// calulates the near specifed 'other_day' from 'day'
signed int day_diff(int day, int other_day) {
	int res = 0;
	if (day != other_day ) {
		res = 1;
		while(res <= 3) {
			if ((day + res) % 7 ==  other_day) {
				break;
			}
			if((other_day + res) % 7 ==  day ) {
				res = -res;
				break;
			}
			res++;
		}
	}

	return res;
}

int main (int argc, char const *argv[]) {
	struct tm* timeinfo = currentTime();
	char now[20], after[20];
	MAKE_TIME_STR(now, timeinfo);

	int day = timeinfo->tm_wday;
	int other_day = 3;
	int diff = day_diff(day, other_day);

	timeinfo->tm_mday += diff;
	timegm(timeinfo);
	MAKE_TIME_STR(after, timeinfo);

	char *str_days[7] = {"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat"};
	printf("%s %s\n%s %s\n", now, str_days[day], after, str_days[timeinfo->tm_wday]);

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


