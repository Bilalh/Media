#ifndef HISTORYHEAD
#define HISTORYHEAD 
#include <stdbool.h>
#include "time_util.h"
#include "media_args.h"



/**
 * @brief Adds the filenames to database
 *
 * @param filenames - null term array of strings
 * @return true if addded
**/
bool updateHistory(char **filenames, Status status );


typedef int (*SqlCallback)(void *unused, int argc, char **argv, char **columns);

/**
 * @brief Runs the commands on the datebase and pass the results to the callback
 *
 * @param argc        - The number of commands
 * @param argv        - The commands
 * @param SqlCallback - Fuction that is called for each record
**/
void sql_exec_array (int argc, char **argv, SqlCallback callback );
void sql_exec(char *command, SqlCallback callback);


#define DATABASE "/Users/bilalh/Library/Application Support/Media/Media.db"

#endif
