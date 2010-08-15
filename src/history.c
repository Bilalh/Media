#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>
#include <libxml/tree.h>

#include "history.h"
#include "string_util.h"

#define DATABASE "/Users/bilalh/Library/Application Support/Media/Media.db"
bool updateHistory(char **filenames) {

	sqlite3 *db;
	int result;

	result = sqlite3_open(DATABASE, &db);
	if( result ) {
		fprintf(stderr, "No history written: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return false;
	}

	sqlite3_stmt *statement;
	const char *query = "Insert Into History(Series,Number,Date) Values(?, ? ,?)";
	sqlite3_prepare_v2(db, query, strlen(query), &statement, NULL);

	struct tm* timeinfo = currentTime();
	char now[20];
	
	
	while(**filenames != '\0') {
		printf("%s\n", *filenames);
		char **ans = ep_num(*filenames);
		if (ans[0] != NULL) {
			long num = strtol(ans[0] + 1, NULL, 10);
			if (num == 0 ) num++;

			int index = ans[1] != NULL ? 1 : 0;
			char s[ans[index] - *filenames + 1]; // 1 for \0
			strncpy(s, *filenames, ans[index] - *filenames);
			s[ans[index] - *filenames] = '\0';

			printf("title %s.\n", s);
			printf("num   %li.\n", num);
			MAKE_TIME_STR(now,timeinfo);
			printf("time  %s.\n", now);
			
			sqlite3_bind_text(statement, 1, s, -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(statement, 2, num);
			sqlite3_bind_text(statement, 3, now, -1, SQLITE_STATIC);
			
			result = sqlite3_step(statement);
			printf("r:%i Ok:%i done:%i \n", result,SQLITE_OK,SQLITE_DONE );
			if( !(result == SQLITE_OK  || result == SQLITE_DONE) ) 
				fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
				
			printf("reset: %i\n\n", sqlite3_reset(statement));
			timeinfo->tm_min+=30;
			timegm(timeinfo);
			
		}
		filenames++;
	}

	sqlite3_finalize(statement);
	sqlite3_close(db);
	return true;
}