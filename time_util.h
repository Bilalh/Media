#ifndef TIMEUTILHEAD
#define TIMEUTILHEAD

#define MAKE_TIME_STR(str,stucttmPtr) strftime(str, 20, "%F %H:%M:%S", stucttmPtr);
#define DATE_TIME_UK_REGEX "[0-9]{2}-[0-9]{2}-[0-9]{4} [0-9]{2}:[0-9]{2}:[0-9]{2}"

#include "time.h"

struct tm* currentTime();

int day_diff(int day, int other_day);
int day_last(int day, int other_day);
int day_future(int day, int other_day);

struct tm *parse_time(char **str, int length);

// parse n (days|hours|minutes) and increments index by 2; 
void parse_time_spec(char** strarr, struct tm* tm, int *index, int multiplier);

enum{ SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY};

#endif