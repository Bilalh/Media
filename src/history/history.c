#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <assert.h>
#include <sys/ioctl.h>

#include <sqlite3.h>
#include <libxml/tree.h>

#include <include/history.h>
#include <include/string_util.h>
#include <include/debug.h>
#include <include/colours.h>
#include <include/time.def>
#include <include/colours.h>
#include <include/uthash.h>

#include <prefs.h>

static int print_latest_callback (void *unused, int argc, char **argv, char **columns);
static int print_ongoing_callback(void *unused, int argc, char **argv, char **columns);
static int print_ongoing_callback_no_colour(void *unused, int argc, char **argv, char **columns);
static int print_ongoing_data_callback(void *unused, int argc, char **argv, char **columns);

bool updateHistory(char **filenames, Status status, int sep) {
	assert(filenames);
	sqlite3 *db;
	int result;

	result = sqlite3_open(DATABASE, &db);
	if( result ) {
		 efprintf(  "No history written: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return false;
	}

	sqlite3_stmt *statement_h, *statement_si_u, *statement_si_s, *statement_si_su;
	const char *query_h     = "Insert Into History(Series,Number,Date) Values(?, ? ,?)";
	const char *query_si_u  = "UPDATE SeriesInfo SET Updated =? Where Title = ?";
	const char *query_si_s  = "UPDATE SeriesInfo SET Skip = ? Where Title = ?";
	const char *query_si_su = "UPDATE SeriesInfo SET Skip = ?, Updated = ? Where Title = ?";

	bool si_u = false, si_s = false, si_su = false;

	sqlite3_prepare_v2(db, query_h, (int) strlen(query_h), &statement_h, NULL);

	struct tm* timeinfo = currentTime();
	char now[20];

	while(*filenames != NULL) {
		*filenames = basename(*filenames);
		printf("%s\n", *filenames);
		char **ans = ep_num(*filenames);
		if (ans[0] != NULL) {
			EP_GET_NUMBER(ans, num);
			EP_GET_NAME(ans, s, *filenames)

			hdprintf("title %s.\n", s);
			hdprintf("num   %li.\n", num);
			MAKE_TIME_STR(now, timeinfo);
			hdprintf("time  %s.\n", now);

			sqlite3_bind_text(statement_h, 1, s, -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement_h, 2, num);
			sqlite3_bind_text(statement_h, 3, now, -1, SQLITE_STATIC);

			result = sqlite3_step(statement_h);
			hdprintf("r:%i Ok:%i done:%i \n", result, SQLITE_OK, SQLITE_DONE );
			if( !(result == SQLITE_OK  || result == SQLITE_DONE) ) {
				 efprintf(  "SQL error %s : %s\n", *filenames, sqlite3_errmsg(db));
			}
			result = sqlite3_reset(statement_h);
			hdprintf("reset: %i\n\n", result);

			// for status
			if (status == S_SKIP_UPDATED) {
				if(! si_su) {
					sqlite3_prepare_v2(db, query_si_su, (int) strlen(query_si_su), &statement_si_su, NULL);
					si_su = true;
				}
				sqlite3_bind_text(statement_si_su, 3, s, -1, SQLITE_TRANSIENT);
				sqlite3_bind_int(statement_si_su, 1, 1);
				sqlite3_bind_int(statement_si_su, 2, 1);
				result = sqlite3_step(statement_si_su);
				printf("su r:%i Ok:%i done:%i \n", result, SQLITE_OK, SQLITE_DONE );
				if( !(result == SQLITE_OK  || result == SQLITE_DONE) ) {
					efprintf( "SQL error %s : %s\n", *filenames, sqlite3_errmsg(db));
				}
				result = sqlite3_reset(statement_si_su);
				hdprintf("sk reset: %i\n\n", result);

			} else if (status == S_UPDATED) {
				if(! si_u) {
					sqlite3_prepare_v2(db, query_si_u, (int) strlen(query_si_u), &statement_si_u, NULL);
					si_u = true;
				}
				sqlite3_bind_text(statement_si_u, 2, s, -1, SQLITE_TRANSIENT);
				sqlite3_bind_int(statement_si_u, 1, 1);
				result = sqlite3_step(statement_si_u);
				printf("up r:%i Ok:%i done:%i \n", result, SQLITE_OK, SQLITE_DONE );
				if( !(result == SQLITE_OK  || result == SQLITE_DONE) ) {
					efprintf( "SQL error %s : %s\n", *filenames, sqlite3_errmsg(db));
				}
				result = sqlite3_reset(statement_si_u);
				hdprintf("up reset: %i\n\n", result);
			} else if(status == S_SKIP) {
				if(! si_s) {
					sqlite3_prepare_v2(db, query_si_s, (int) strlen(query_si_s), &statement_si_s, NULL);
					si_s = true;
				}
				sqlite3_bind_text(statement_si_s, 2, s, -1, SQLITE_TRANSIENT);
				sqlite3_bind_int(statement_si_s, 1, 1);
				result = sqlite3_step(statement_si_s);
				hdprintf("sk r:%i Ok:%i done:%i \n", result, SQLITE_OK, SQLITE_DONE );
				if( !(result == SQLITE_OK  || result == SQLITE_DONE) ) {
					efprintf( "SQL error %s : %s\n", *filenames, sqlite3_errmsg(db));
				}
				result = sqlite3_reset(statement_si_s);
				hdprintf("sk reset: %i\n\n", result);
			}

		}
		filenames++;
	}

	sqlite3_finalize(statement_h);
	sqlite3_close(db);
	return true;
}

void set_score(char *series, int score){
	assert(series); assert(score > 0); assert(score <= 10);
	
	char **ans = ep_num(series);
	if (ans[0] != NULL) {
		EP_GET_NAME(ans, title, series);
		
		char *sql = sqlite3_mprintf(
			" UPDATE SeriesInfo"
			" SET Score   = %d"
			" WHERE Title = %Q; ",
			score,title);
		
		sql_exec(sql, NULL);
		sqlite3_free(sql);
	}else{
		efprintf("%s\n", "null ep_num in set_score");
	}
	
}

void set_total(char *series, int total){
	assert(series); assert(total > 0);
	
	char **ans = ep_num(series);
	if (ans[0] != NULL) {
		EP_GET_NAME(ans, title, series);
		
		char *sql = sqlite3_mprintf(
			" UPDATE SeriesInfo"
			" SET Total   = %d"
			" WHERE Title = %Q; ",
			total,title);
		
		sql_exec(sql, NULL);
		sqlite3_free(sql);
	}else{
		efprintf("%s\n", "null ep_num in set_total");
	}
	
}

void set_movie(char *series){
	assert(series);
	
	char **ans = ep_num(series);
	if (ans[0] != NULL) {
		EP_GET_NAME(ans, title, series);
		
		char *sql = sqlite3_mprintf(
			" UPDATE SeriesInfo"
			" SET Total   = 1,"
			" Finished    = 1 "
			" WHERE Title = %Q; ",
			title);
		
		sql_exec(sql, NULL);
		sqlite3_free(sql);
	}else{
		efprintf("%s\n", "null ep_num in set_movie");
	}
	
}

char** find_unwatched(char **filenames, size_t *length, bool free_unused) {
	assert(filenames);
	
	char **new_filenames = malloc(sizeof(char*) * (*length +1) );
	int index =0;
	
	sqlite3 *db;
	int result;

	result = sqlite3_open(DATABASE, &db);
	if( result ) {
		efprintf(  "find_unwatched failed: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(33);
	}

	sqlite3_stmt *statement_h;
	const char *query_h = "select current from SeriesData where Title = ?";
	sqlite3_prepare_v2(db, query_h, (int) strlen(query_h), &statement_h, NULL);

	while(*filenames != NULL) {
		
		char *filename = strdup(basename(*filenames));
		char **ans = ep_num(filename);
		if (ans[0] != NULL) {
			
			EP_GET_NUMBER(ans, num);
			EP_GET_NAME(ans, title, filename)

			hdprintf("title %s. num %ld\n", title, num);
			sqlite3_bind_text(statement_h, 1, title, -1, SQLITE_TRANSIENT);

			result = sqlite3_step(statement_h);
			hdprintf("r:%i Row:%i Ok:%i done:%i \n", result, SQLITE_ROW, SQLITE_OK, SQLITE_DONE );
			
			// check latest
			if (result == SQLITE_ROW){
				
				int current = sqlite3_column_int(statement_h, 0);
				hdprintf("current:%d\n",current );
				
				// not watched
				if (num > current ){
					hdprintf("added %s\n", filename);
					new_filenames[index++] = *filenames;
				}else if (free_unused) {
					free(*filenames);
				}
				
			// never watched
			}else if( result == SQLITE_OK  || result == SQLITE_DONE ) {
				
				hdprintf("added %s\n", filename);
				new_filenames[index++] = *filenames;
			}else{
				efprintf(  "SQL error %s : %s\n", *filenames, sqlite3_errmsg(db));
				exit(18);
			}
			
			
			result = sqlite3_reset(statement_h);
			hdprintf("reset:%d index:%d\n", result, index);
			free(filename);
		}
		filenames++;
	}

	sqlite3_finalize(statement_h);
	sqlite3_close(db);
	
	new_filenames[index] = strdup("");
	
	hdprintf("index:%d\n", index);
	
	*length = index;
	return new_filenames;
}

void sql_exec(char *command, SqlCallback callback ) {
	assert(command);
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(DATABASE, &db);
	if( rc ) {
		efprintf( "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
	
	rc = sqlite3_exec(db, command, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ) {
		efprintf( "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(db);
}

void sql_exec_array (int argc, char **argv, SqlCallback callback ) {
	assert(argv); assert(callback);
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(DATABASE, &db);
	if( rc ) {
		efprintf( "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}


	for(int i = 0; i < argc; ++i) {
		rc = sqlite3_exec(db, argv[i], callback, 0, &zErrMsg);
		if( rc != SQLITE_OK ) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
	}

	sqlite3_close(db);
}

void print_latest(char *num){
	assert(num);
	
	char buff[202 + strlen(num)*2];
	sprintf(buff, 
		"SELECT Title, Current, Total, Date, Finished, Rewatching, Dropped"
		" FROM SeriesData"
		" WHERE("
		"	strftime('%%s',Date) > strftime('%%s', 'now','-%s day','localtime')"
		"	AND Finished = 0 AND (Skip = 0 AND Dropped = 0)"
		")",
		num
	);
	sql_exec(buff, print_latest_callback);
}

void print_latest_with_finished(char *num){
	assert(num);
	
	char buff[200 + strlen(num)]; // few extra 332? 
	sprintf(buff, 
		"SELECT Title, Current, Total, Date, Finished, Rewatching, Dropped"
		" FROM SeriesData"
		" WHERE("
		"	strftime('%%s',Date) > strftime('%%s', 'now','-%s day','localtime')"
		"	AND (Dropped = 0)"
		")",
		num
	);
	sql_exec(buff, print_latest_callback);
}

void print_latest_with_finished_and_skipped(char *num){
	assert(num);
	
	char buff[200 + strlen(num)*2];
	sprintf(buff, 
		"SELECT Title, Current, Total, Date, Finished, Rewatching, Dropped"
		" FROM SeriesData"
		" WHERE("
		"	strftime('%%s',Date) > strftime('%%s', 'now','-%s day','localtime')"
		")",
		num
	);
	sql_exec(buff, print_latest_callback);
}

// Prints the data from the  print_latest functions
static int print_latest_callback(void *unused, int argc, char **argv, char **columns){
	assert(argv);assert(columns);
	
	const char* title = argv[0];
	const char* current = argv[1];
	const char* total = argv[2] ? argv[2] : "?";
	const char* date_data  = argv[3];
	
	char *status;
	
	if ( *argv[4] == '1' ){       // finished
		status = COLOURJ("F ",CYAN); 
	}else if ( *argv[5] == '1' ){ // rewatching 
		status = COLOURJ("R ",GREEN);
	}else if ( *argv[6] == '1' ){ // dropped 
		status = COLOURJ("D ",WHITE);
	}else{                        // ongoing
		status = COLOURJ("O ",YELLOW); 
	}
	
	// Makes the date
	struct tm tm = {}; char date[COLOUR_TIME_LENGTH];
	strptime(date_data, SQL_DATE, &tm);
	time_t temp = mktime(&tm);
	tm = *localtime(&temp);
	strftime(date, COLOUR_TIME_LENGTH, COLOUR_TIME_STRING, &tm);
	
	const int  length = 40;
	// prints the data 
	printf("%s%-*s " SSS("%3s") "/" SSS("%-3s") " %17s\n", 
		status, length, title, 
		COLOUR(current,BLUE) , COLOUR(total,RED) , date
	);

	return 0;
}

void print_ongoing(bool colour){
	char *buff = "Select * From OngoingSeries Where Rewatching = 0";
	sql_exec(buff, colour ? print_ongoing_callback : print_ongoing_callback_no_colour);
	puts("");
	buff = "Select * From OngoingSeries Where Rewatching = 1";
	sql_exec(buff, colour ? print_ongoing_callback : print_ongoing_callback_no_colour);
}

void print_ongoing_data(){
	
	char *buff = "Select * From OngoingSeries";
	sql_exec(buff, print_ongoing_data_callback);
}

// Prints the data from the print_ongoing_data functions
static int print_ongoing_data_callback(void *unused, int argc, char **argv, char **columns){
	
	const char* title   = argv[0];
	const char *current = argv[1];
	
	printf("%s\t%s\n", 
		title, current
	);

	return 0;
}

// Prints the data from the  print_ongoing functions
static int print_ongoing_callback(void *unused, int argc, char **argv, char **columns){
	
	const char* title   = argv[0];
	const char *current = argv[1];
	const char* total   = argv[2] ? argv[2] : "?";
	
	const int rest_length = 37;
	
	struct ttysize ts; 
	ioctl(0, TIOCGSIZE, &ts);
	
	int title_length = ts.ts_cols - rest_length;
	if      (title_length >= PREFS_MAX_TITLE_LENGTH) title_length = PREFS_MAX_TITLE_LENGTH;
	else if (title_length <= PREFS_MIN_TITLE_LENGTH) title_length = PREFS_MIN_TITLE_LENGTH;
	 
	
	// Makes the date
	struct tm tm = {}; char date[COLOUR_TIME_LENGTH];
	strptime(argv[3], SQL_DATE, &tm);
	time_t temp = mktime(&tm);
	tm = *localtime(&temp);
	strftime(date, COLOUR_TIME_LENGTH, COLOUR_TIME_STRING, &tm);
	
	// prints the data 
	printf("%-*s " SSS("%3s") "/" SSS("%-3s") " %28s\n", 
		   title_length,title, 
		   COLOUR(current,BLUE) , COLOUR(total,RED) , date
	);

	return 0;
}

// Prints the data from the  print_ongoing functions
static int print_ongoing_callback_no_colour(void *unused, int argc, char **argv, char **columns){
	
	const char* title   = argv[0];
	const char *current = argv[1];
	const char* total   = argv[2] ? argv[2] : "?";
	
	
	// Makes the date
	struct tm tm = {}; char date[TIME_LENGTH];
	strptime(argv[3], SQL_DATE, &tm);
	time_t temp = mktime(&tm);
	tm = *localtime(&temp);
	strftime(date, TIME_LENGTH, TIME_STRING, &tm);
	
	// prints the data 
	printf("%-42s " "%3s" "/" "%-3s" " %17s\n", 
		title, current, total , date
	);

	return 0;
}


