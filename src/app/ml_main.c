#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#include <include/ml.h>
#include <include/history.h>
#include <include/string_buffer.h>

#include <include/debug.h>


extern String *sql_commands;

int main (int argc, char  *argv[]) {
	sql_exec("SELECT Title, Id, Current, Total, StartDate, EndDate, Finished, Date, Score"
	         " FROM SeriesData WHERE (updated == 0 AND skip == 0 AND Rewatching =0 )", 
			 update_new);
	 if (sql_commands){
		dprintf("%s\n", sql_commands->str);
		sql_exec(sql_commands->str, NULL);
	}
	return 0;
}