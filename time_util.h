#ifndef TIMEUTILHEAD
#define TIMEUTILHEAD

#define MAKE_TIME_STR(str,stucttmPtr) strftime(str, 20, "%F %H:%M:%S", stucttmPtr);
#define DATE_TIME_REGEX "[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}"
#define DATE_TIME_UK_REGEX "[0-9]{2}-[0-9]{2}-[0-9]{4} [0-9]{2}:[0-9]{2}:[0-9]{2}"

#define DATE_REGEX "[0-9]{4}-[0-9]{2}-[0-9]{2}"
#include "time.h"

struct tm* currentTime();

int day_diff(int day, int other_day);
int day_last(int day, int other_day);
int day_future(int day, int other_day);

struct tm *parse_time(char **str, int length);

// parse n (days|hours|minutes) and increments index by 2; 
void parse_time_spec(char** strarr, struct tm* tm, int *index, int multiplier);


void type_a(char **str, int length, struct tm* tm );
void type_b(char **str, int length, struct tm* tm );
void type_c(char **str, int length, struct tm* tm );

typedef enum {T_DAYS, T_HOURS, T_MINS, T_SECS} T_Spec;
typedef enum {T_AFTER = 1, T_BEFORE = -1} T_Pos;
#endif