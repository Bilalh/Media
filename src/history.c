#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>
#include <libxml/tree.h>

#include "../include/history.h"
#include "../include/string_util.h"

#define DATABASE "/Users/bilalh/Library/Application Support/Media/Media.db"
bool updateHistory(char **filenames, Status status) {
	//FIXME add done skip
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
		printf("%s\n", *filenames);
		char **ans = ep_num(*filenames);
		if (ans[0] != NULL) {
			EP_GET_NUMBER(ans, num);
			EP_GET_NAME(ans, s, *filenames)
			
			printf("title %s.\n", s);
			printf("num   %li.\n", num);
			MAKE_TIME_STR(now,timeinfo);
			printf("time  %s.\n", now);
			
			sqlite3_bind_text(statement_h, 1, s, -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement_h, 2, num);
			sqlite3_bind_text(statement_h, 3, now, -1, SQLITE_STATIC);
			
			result = sqlite3_step(statement_h);
			printf("r:%i Ok:%i done:%i \n", result,SQLITE_OK,SQLITE_DONE );
			if( !(result == SQLITE_OK  || result == SQLITE_DONE) ){
				fprintf(stderr, "SQL error %s : %s\n",*filenames, sqlite3_errmsg(db));
			}
			printf("reset: %i\n\n", sqlite3_reset(statement_h));
			timeinfo->tm_min+=27;
			timegm(timeinfo);
			
			// for status
			if (status == S_SKIP_UPDATED){
				if(! si_su){
					sqlite3_prepare_v2(db, query_si_su, strlen(query_si_su), &statement_si_su, NULL);
					si_su = true;
				}
				sqlite3_bind_text(statement_si_su, 3, s, -1, SQLITE_TRANSIENT);
				sqlite3_bind_int(statement_si_su, 1, 1);
				sqlite3_bind_int(statement_si_su, 2, 1);
				result = sqlite3_step(statement_si_su);
				printf("su r:%i Ok:%i done:%i \n", result,SQLITE_OK,SQLITE_DONE );
				if( !(result == SQLITE_OK  || result == SQLITE_DONE) ){
					fprintf(stderr, "SQL error %s : %s\n",*filenames, sqlite3_errmsg(db));
				}
				printf("sk reset: %i\n\n", sqlite3_reset(statement_si_su));
				
			}else if (status == S_UPDATED){
				if(! si_u){
					sqlite3_prepare_v2(db, query_si_u, strlen(query_si_u), &statement_si_u, NULL);
					si_u = true;
				}
				sqlite3_bind_text(statement_si_u, 2, s, -1, SQLITE_TRANSIENT);
				sqlite3_bind_int(statement_si_u, 1, 1);
				result = sqlite3_step(statement_si_u);
				printf("up r:%i Ok:%i done:%i \n", result,SQLITE_OK,SQLITE_DONE );
				if( !(result == SQLITE_OK  || result == SQLITE_DONE) ){
					fprintf(stderr, "SQL error %s : %s\n",*filenames, sqlite3_errmsg(db));
				}
				printf("up reset: %i\n\n", sqlite3_reset(statement_si_u));
			}else if(status == S_SKIP){
				if(! si_s){
					sqlite3_prepare_v2(db, query_si_s, strlen(query_si_s), &statement_si_s, NULL);
					si_s = true;
				}
				sqlite3_bind_text(statement_si_s, 2, s, -1, SQLITE_TRANSIENT);
				sqlite3_bind_int(statement_si_s, 1, 1);
				result = sqlite3_step(statement_si_s);
				printf("sk r:%i Ok:%i done:%i \n", result, SQLITE_OK, SQLITE_DONE );
				if( !(result == SQLITE_OK  || result == SQLITE_DONE) ){
					fprintf(stderr, "SQL error %s : %s\n",*filenames, sqlite3_errmsg(db));
				}
				printf("sk reset: %i\n\n", sqlite3_reset(statement_si_s));	
			}
			
		}
		filenames++;
	}

	sqlite3_finalize(statement_h);
	sqlite3_close(db);
	return true;
}
