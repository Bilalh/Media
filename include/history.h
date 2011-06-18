#ifndef HISTORYHEAD
#define HISTORYHEAD 
#include <stdbool.h>

#include <sqlite3.h>

#include "time_util.h"
#include "media_args.h"


/**
 * @brief Adds the filenames to database
 *
 * @param filenames - null term array of strings
 * @param sep       - The time in minutes to use as the sep
 * @return true if addded
**/
bool updateHistory(char **filenames, Status status, int sep);

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

// printf the latest files
void print_latest(char *num);
void print_latest_with_finished(char *num);
void print_latest_with_finished_and_skipped(char *num);
/**
 * @brief prints ongoing series 
 *
 * @param colour prints in colour if true
**/
void print_ongoing(bool colour);

/**
 * @brief Prints data for ongoing series 
**/
void print_ongoing_data();

/**
 * @brief Sets the score of a series 
 *
 * @param title The title of the series
 * @param score The score in the range 1 - 10
**/
void set_score(char *title, int score);


/**
 * @brief Sets the total of the series
 *
 * @param series The title of the series
 * @param total The total to use
**/
void set_total(char *series, int total);

/**
 * @brief Sets the total of the series to 1 and sets finished to 1
 *
 * @param series The title of the series
**/
void set_movie(char *series);

/**
 * @brief Finds the files that have not been watched 
 * @return the files that have not be watched 
**/
char** find_unwatched(char **filenames, size_t *length, bool free_unused);


void show_menu(char **filenames, size_t *length, bool free_unused);

#define DATABASE "/Users/bilalh/Library/Application Support/Media/Media.db"

#endif
