#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <include/history.h>
#include <include/time_util.h>
#include <include/string_util.h>


int main (int argc, char *argv[]) {

	if( argc < 3 ) {
		printf("%s\n", "hista Series lower_num [time] [upper_num] [sep] ");
		exit(0);
	}

	char *series = argv[1];
	char base_time[20];
	int sep = 27;
	
	int lower = (int) strtol(argv[2], NULL, 10);
	if( lower == 0 ) {
		printf("%s %s\n", "Invalid number", argv[2] );
		printf("%s\n", "hista Series lower_num [time] [upper_num] [sep] ");
		exit(1);
	}

	struct tm *tm;
	if(argc >= 4) {
		int  length = 0;
		char **args  = spilt_string_m(argv[3], &length);
		tm = parse_time(args, length);
		free(args);
	} else {
		// hista Series lower_num
		tm = currentTime();
		char sql[85 +strlen(series)];
		MAKE_TIME_STR(base_time,tm);
		
		sprintf(sql, 
			"Insert Into History(Series,Date,Number) "
			"Values('%s', '%s', %s)",
			 series, base_time, argv[2] 
		);
		dprintf("%s\n", sql );
		sql_exec(sql, NULL);
		free(tm);
		exit(0);
	}

	MAKE_TIME_STR(base_time,tm);

	int upper = 0;
	if(argc >= 5){
		upper = (int) strtol(argv[4], NULL, 10);
		if( upper == 0 ) {
			printf("%s %s\n", "Invalid number", argv[4] );
			printf("%s\n", "hista Series lower_num [time] [upper_num] [sep] ");
			exit(1);
		}
	}else{
		char sql[85 +strlen(series)];
		sprintf(sql, 
			"Insert Into History(Series,Date,Number) "
			"Values('%s', '%s', %s)",
			 series, base_time, argv[2] 
		);
		dprintf("%s\n", sql );
		sql_exec(sql, NULL);
		free(tm);
		exit(0);
	}


	if(argc == 6  && *argv[5] == 't'){
		char buff[28];
		strftime(buff, 28, "%Y-%m-%d %H:%M %a %d %b", tm);
		printf("  Title: %s\n", series );
		printf("  Num_L: %d\n", lower );
		printf("  Num_U: %d\n", upper );
		printf("   Time: %s\n", buff );
		exit(0);
	}
	
	if(argc >= 6){
		sep = (int) strtol(argv[5], NULL, 10);
		if( sep == 0 ) {
			printf("%s %s\n", "Invalid number", argv[5] );
			printf("%s\n", "hista Series lower_num [time] [upper_num] [sep] ");
			exit(1);
		}
	}
	
	if(argc >= 7){
		char buff[28];
		strftime(buff, 28, "%Y-%m-%d %H:%M %a %d %b", tm);
		printf("  Title: %s\n", series );
		printf("  Num_L: %d\n", lower );
		printf("  Num_U: %d\n", upper );
		printf("   Time: %s\n", buff );
		printf("    Sep: %d\n", sep );
		exit(0);
	}
	
	
	free(tm);
	return 0;
}