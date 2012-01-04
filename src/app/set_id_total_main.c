#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <include/history.h>

char *date;

int get_date_callback(void *unused, int argc, char **argv, char **columns){
	date   = strdup(argv[0]);
	printf("date:%s\n", date);
	return 0;
}

int main (int argc, char const *argv[]) {
	
	if (argc > 5 || argc < 3){
		printf("%s\n", "set_id series id [total] [finished]");
		exit(0);
	}
	
	// char sql[161 + strlen(argv[1]) + strlen(argv[2]) + (argc >= 4 ? strlen(argv[3]) :0) + (argv >= 5 ?  :0 ) ];
	char *sql = NULL;
	
	if (argc == 3 ){
		sql = sqlite3_mprintf("Update SeriesInfo Set id = %s Where Title =%Q",
			argv[2], argv[1]);
	}else if (argc == 4 ){
		sql = sqlite3_mprintf("Update SeriesInfo Set id = %s, total = %s Where Title =%Q",
			argv[2], argv[3], argv[1]);
	}else{
		
		char *sql_date = sqlite3_mprintf("Select date from SeriesInfo  Where Title =%Q", argv[1]);
		sql_exec(sql_date, get_date_callback);
		
		sql = sqlite3_mprintf("Update SeriesInfo Set id = %s, total = %s, endDate = %Q, finished = 1  Where Title =%Q",
			argv[2], argv[3],date,  argv[1]);
	}
	
	printf("%s\n", sql);
	sql_exec(sql, NULL);
	return 0;
}