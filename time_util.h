#ifndef TIMEUTILHEAD
#define TIMEUTILHEAD
#include "time.h"
#define MAKE_TIME_STR(str,stucttmPtr) strftime(str, 20, "%F %H:%M:%S", stucttmPtr);
struct tm* currentTime();
#endif