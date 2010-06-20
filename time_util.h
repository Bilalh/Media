#ifndef TIMEUTILHEAD
#define TIMEUTILHEAD

#include "time.h"
#define MAKE_TIME_STR(str,stucttmPtr) strftime(str, 20, "%F %H:%M:%S", stucttmPtr);
int day_diff(int day, int other_day);
int day_last(int day, int other_day);
int day_future(int day, int other_day);
struct tm *parse_time(char **str, int length);
struct tm* currentTime();


const char* time_spec[] = {"days", "hours", "minutes", "seconds"};
typedef enum {T_DAYS, T_HOURS, T_MINS, T_SECS} T_Spec;
typedef enum {T_AFTER = 1, T_BEFORE = -1} T_Pos;


#endif