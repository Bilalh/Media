#ifndef TIMEUTILHEAD
#define TIMEUTILHEAD

#define MAKE_TIME_STR(str,stucttmPtr) strftime(str, 20, "%F %H:%M:%S", stucttmPtr);
#include "time.h"

struct tm* currentTime();

int day_diff(int day, int other_day);
int day_last(int day, int other_day);
int day_future(int day, int other_day);

struct tm *parse_time(char **str, int length);
void type_a(char **str, int length, struct tm* tm );
void type_b(char **str, int length, struct tm* tm );
void type_c(char **str, int length, struct tm* tm );

const char* time_spec[][5] = {
	{"day", "days",""}, {"hour", "hours",""},
	{"min", "mins", "minute", "minutes",""}, {"sec", "secs","second","second",""}
};
typedef enum {T_DAYS, T_HOURS, T_MINS, T_SECS} T_Spec;
typedef enum {T_AFTER = 1, T_BEFORE = -1} T_Pos;
#endif