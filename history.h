#ifndef HISTORYHEAD
#define HISTORYHEAD 
#include <stdbool.h>
#include <time.h>
bool updateHistory(char **filenames);
struct tm* currentTime();
#endif