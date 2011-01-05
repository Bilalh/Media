#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include <sqlite3.h>
#include <libxml/tree.h>

#include <include/history.h>
#include <include/string_util.h>
#include <include/debug.h>


static int print_latest_callback (void *unused, int argc, char **argv, char **columns);
static int print_ongoing_callback(void *unused, int argc, char **argv, char **columns);

bool updateHistory(char **filenames, Status status, int sep) {
	sqlite3 *db;
	int result;

	result = sqlite3_open(DATABASE, &db);
	if( result ) {
		fprintf(stderr, "No history written: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return false;
	}

	sqlite3_stmt *statement_h, *statement_si_u, *statement_si_s, *statement_si_su;
	const char *query_h     = "Insert Into History(Series,Number,Date) Values(?, ? ,?)";
	const char *query_si_u  = "UPDATE SeriesInfo SET Updated =? Where Title = ?";
	const char *query_si_s  = "UPDATE SeriesInfo SET Skip = ? Where Title = ?";
	const char *query_si_su = "UPDATE SeriesInfo SET Skip = ?, Updated = ? Where Title = ?";

	bool si_u = false, si_s = false, si_su = false;

	sqlite3_prepare_v2(db, query_h, strlen(query_h), &statement_h, NULL);

	struct tm* timeinfo = currentTime();
	char now[20];

	while(*filenames != NULL) {
		*filenames = basename(*filenames);
		printf("%s\n", *filenames);
		char **ans = ep_num(*filenames);
		if (ans[0] != NULL) {
			EP_GET_NUMBER(ans, num);
			EP_GET_NAME(ans, s, *filenames)

			dprintf("title %s.\n", s);
			dprintf("num   %li.\n", num);
			MAKE_TIME_STR(now, timeinfo);
			dprintf("time  %s.\n", now);

			sqlite3_bind_text(statement_h, 1, s, -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement_h, 2, num);
			sqlite3_bind_text(statement_h, 3, now, -1, SQLITE_STATIC);

			result = sqlite3_step(statement_h);
			dprintf("r:%i Ok:%i done:%i \n", result, SQLITE_OK, SQLITE_DONE );
			if( !(result == SQLITE_OK  || result == SQLITE_DONE) ) {
				fprintf(stderr, "SQL error %s : %s\n", *filenames, sqlite3_errmsg(db));
			}
			result = sqlite3_reset(statement_h);
			dprintf("reset: %i\n\n", reset);

			// for status
			if (status == S_SKIP_UPDATED) {
				if(! si_su) {
					sqlite3_prepare_v2(db, query_si_su, strlen(query_si_su), &statement_si_su, NULL);
					si_su = true;
				}
				sqlite3_bind_text(statement_si_su, 3, s, -1, SQLITE_TRANSIENT);
				sqlite3_bind_int(statement_si_su, 1, 1);
				sqlite3_bind_int(statement_si_su, 2, 1);
				result = sqlite3_step(statement_si_su);
				printf("su r:%i Ok:%i done:%i \n", result, SQLITE_OK, SQLITE_DONE );
				if( !(result == SQLITE_OK  || result == SQLITE_DONE) ) {
					fprintf(stderr, "SQL error %s : %s\n", *filenames, sqlite3_errmsg(db));
				}
				result = sqlite3_reset(statement_si_su)
				dprintf("sk reset: %i\n\n", result);

			} else if (status == S_UPDATED) {
				if(! si_u) {
					sqlite3_prepare_v2(db, query_si_u, strlen(query_si_u), &statement_si_u, NULL);
					si_u = true;
				}
				sqlite3_bind_text(statement_si_u, 2, s, -1, SQLITE_TRANSIENT);
				sqlite3_bind_int(statement_si_u, 1, 1);
				result = sqlite3_step(statement_si_u);
				printf("up r:%i Ok:%i done:%i \n", result, SQLITE_OK, SQLITE_DONE );
				if( !(result == SQLITE_OK  || result == SQLITE_DONE) ) {
					fprintf(stderr, "SQL error %s : %s\n", *filenames, sqlite3_errmsg(db));
				}
				result = sqlite3_reset(statement_si_u);
				dprintf("up reset: %i\n\n", result);
			} else if(status == S_SKIP) {
				if(! si_s) {
					sqlite3_prepare_v2(db, query_si_s, strlen(query_si_s), &statement_si_s, NULL);
					si_s = true;
				}
				sqlite3_bind_text(statement_si_s, 2, s, -1, SQLITE_TRANSIENT);
				sqlite3_bind_int(statement_si_s, 1, 1);
				result = sqlite3_step(statement_si_s);
				dprintf("sk r:%i Ok:%i done:%i \n", result, SQLITE_OK, SQLITE_DONE );
				if( !(result == SQLITE_OK  || result == SQLITE_DONE) ) {
					fprintf(stderr, "SQL error %s : %s\n", *filenames, sqlite3_errmsg(db));
				}
				result = sqlite3_reset(statement_si_s);
				dprintf("sk reset: %i\n\n", result);
			}

		}
		filenames++;
	}

	sqlite3_finalize(statement_h);
	sqlite3_close(db);
	return true;
}

void sql_exec(char *command, SqlCallback callback ) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(DATABASE, &db);
	if( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
	
	rc = sqlite3_exec(db, command, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(db);
}

void sql_exec_array (int argc, char **argv, SqlCallback callback ) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(DATABASE, &db);
	if( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
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
	
	char buff[200 + strlen(num)*2];
	sprintf(buff, 
		"SELECT Title, Current, Total, Date, Finished, Rewatching"
		" FROM SeriesData"
		" WHERE("
		"	strftime('%%s',Date) > strftime('%%s', 'now','-%s day','localtime')"
		"	AND Finished = 0 AND (Skip = 0 OR Dropped = 0)"
		")",
		num
	);
	sql_exec(buff, print_latest_callback);
}

void print_latest_with_finished(char *num){
	
	char buff[200 + strlen(num)]; // few extra 332? 
	sprintf(buff, 
		"SELECT Title, Current, Total, Date, Finished, Rewatching"
		" FROM SeriesData"
		" WHERE("
		"	strftime('%%s',Date) > strftime('%%s', 'now','-%s day','localtime')"
		"	AND (Skip = 0 OR Dropped = 0)"
		")",
		num
	);
	sql_exec(buff, print_latest_callback);
}

void print_latest_with_finished_and_skipped(char *num){
	char buff[200 + strlen(num)*2];
	sprintf(buff, 
		"SELECT Title, Current, Total, Date, Finished, Rewatching"
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
	
	const char* title = argv[0];
	const char* current = argv[1];
	const char* total = argv[2] ? argv[2] : "?";
	const char* date_data  = argv[3];
	
	char *status;
	
	if       ( *argv[5] == '1' ){ // rewatching 
		status = "R "; 
	}else if ( *argv[4] == '1' ){ // finished
		status = "F ";
	}else{
		status = "O ";
	}
	
	// Makes the date
	struct tm tm = {}; char date[28];
	strptime(date_data, "%F %H:%M:%S", &tm);
	time_t temp = mktime(&tm);
	tm = *localtime(&temp);
	strftime(date, 28, "%Y-%m-%d %H:%M %a %d %b", &tm);
	
	const int  length = 40;
	// prints the data 
	printf("%s%-*s %3s/%-3s %17s\n", status, length, title, current ,total, date);

	return 0;
}

void print_ongoing(){
	char *buff = "Select * From OngoingSeries Where Rewatching = 0";
	sql_exec(buff, print_ongoing_callback);
	puts("");
	buff = "Select * From OngoingSeries Where Rewatching = 1";
	sql_exec(buff, print_ongoing_callback);
}

// Prints the data from the  print_ongoing functions
static int print_ongoing_callback(void *unused, int argc, char **argv, char **columns){
	
	const char* title   = argv[0];
	const char *current = argv[1];
	const char* total   = argv[2] ? argv[2] : "?";
	
	
	// Makes the date
	struct tm tm = {}; char date[28];
	strptime(argv[3], "%F %H:%M:%S", &tm);
	time_t temp = mktime(&tm);
	tm = *localtime(&temp);
	strftime(date, 28, "%Y-%m-%d %H:%M %a %d %b", &tm);
	
	// prints the data 
	printf("%-42s %3s/%-3s %17s\n", title, current ,total, date);

	return 0;
}

