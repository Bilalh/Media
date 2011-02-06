#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include <include/history.h>
#include <include/string_util.h>
#include <include/string_buffer.h>
#include <include/debug.h>

#define USAGE "hists Series Score [lower_num|t] [time] [upper_num] [sep|t] [t]"

static void print_data(char *series, int lower, int upper, struct tm *tm, int sep, int score);

//usage hists Series Score [lower_num|t] [time] [upper_num] [sep|t] [t]
int main (int argc, char *argv[]) {

	if( argc < 3 ) {
		
		printf("%s\n", USAGE);
		exit(0);
	}

	char *series  = argv[1];
	int sep       = 27;
	int lower     = 0;
	int upper     = -1;
	struct tm *tm = currentTime();
	
	String s, s2;
	new_string(&s, 120);
	new_string(&s2, 120);
	
	int score = (int) strtol(argv[2], NULL, 10);
	if( score == 0 || score > 10 ) {
		
		printf("%s %s\n", "Invalid score", argv[2] );
		printf("%s\n", USAGE);
		exit(1);
	}
	
	
		
	if( score > 0 ){
		
		string_sprintf(&s, 18+ 17 + 21  + strlen(series) +1,
			" UPDATE SeriesInfo"
			" SET Score   = %d"
			" WHERE Title = \"%s\"; ",
			score, series
		);
	}
	
	if (argc >= 4 && *argv[3] == 't'){
		
		printf("  Title: %s\n", series );
		printf("  Score: %d\n", score );
		exit(0);
	}else if (argc >= 4 ){
		// hists Series Score lower_num
		lower = (int) strtol(argv[3], NULL, 10);
		if( lower == 0 ) {
			printf("%s %s\n", "Invalid number", argv[3] );
			printf("%s\n", USAGE);
			exit(1);
		}
		upper = lower;
	}
	
	if( argc >= 5 ) {
		// hists Series Score lower_num time
		int  length = 0;
		char **args  = spilt_string_m(argv[4], &length);
		tm = parse_time(args, length);
		free(args);
	}
	
	if( argc >= 6 ){
		// hists Series Score lower_num time upper_num
		upper = 0;
		upper = (int) strtol(argv[5], NULL, 10);
		if( upper == 0 ) {
			printf("%s %s\n", "Invalid number", argv[5] );
			printf("%s\n", USAGE);
			exit(2);
		}
		if (upper < lower){
			printf("Upper has to be >= then lower\n");
			printf("%s\n", USAGE);
			exit(3);
		}
		
	}
	
	if( (argc == 7  && *argv[6] == 't') || argc >= 8 ){
		// hists Series Score lower_num time
		print_data(series, lower, upper, tm, sep, score);
	}else if( argc >= 7 ){
		// hists Series Score lower_num time sep
		sep = (int) strtol(argv[6], NULL, 10);
		if( sep == 0 ) {
			printf("%s %s\n", "Invalid number", argv[6] );
			printf("%s\n", USAGE);
			exit(1);
		}
	}
	
	for(int i = lower; i <=upper; ++i){
		char time_buff[20];
		MAKE_TIME_STR(time_buff,tm);
		string_sprintf(&s2, 87 + strlen(series) + 1,
			"INSERT INTO History(Series,Date,Number) "
			"VALUES(\"%s\", '%s', %d); ",
			 series, time_buff, i
		);
		tm->tm_min += sep;
		timegm(tm);
	}
	
	printf("%s\n", s.str);
	// The insert into history should be done before the updates
	sql_exec(s2.str, NULL); 
	sql_exec(s.str, NULL); 
	free(tm);
	return 0;
}

static void print_data(char *series, int lower, int upper, struct tm *tm, int sep, int score){
	char buff[28];
	strftime(buff, 28, "%Y-%m-%d %H:%M %a %d %b", tm);
	printf("  Title: %s\n", series );
	printf("  Score: %d\n", score );
	printf("  Num_L: %d\n", lower );
	printf("  Num_U: %d\n", upper );
	printf("   Time: %s\n", buff );
	printf("    Sep: %d\n", sep );
	exit(0);
}
