#include <time.h>
#include <stdio.h>
#include <string.h>

#include <sqlite3.h>

#include <include/ml.h>
#include <include/history.h>



int update_new(void *unused, int argc, char **argv, char **columns) {

	for(int i = 0; i < argc; i++) {
		printf("%s = %s\n", columns[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");

	MLOpts opts = {
		.status      = strcmp("1", argv[6]) == 0 ? ML_COMPLETED : ML_WATCHING,
	};
	
	strncpy(opts.id,      argv[1], 6);
	strncpy(opts.episode, argv[2], 6);
	strncpy(opts.total,   argv[4], 6);

	struct tm* tm;
	if( argv[4] ){
		strptime(argv[4], "%F %H:%M:%S", tm);
		strftime(opts.date_start, 9, "%d%m%Y", tm);
	}

	if( argv[5] ){
		strptime(argv[5], "%F %H:%M:%S", tm);
		strftime(opts.date_finish, 9, "%d%m%Y", tm);
	}

	
	return 0;
}



// int main (int argc, char  *argv[]) {
// 
// 
// 	sql_exec("SELECT Title, ID, Current, Total, max(StartDate), EndDate, Finished"
// 	         " FROM SeriesInfo WHERE (updated == 0 AND skip == 0)", 
// 			 update_new);
// 	return 0;
// }


int main (int argc, char  *argv[]) {

	MLOpts opts = {
		.title       = "Azumanga Daioh",
		.episode     = "1",
		.status      = ML_PLANTOWATCH,
		.score       = 6,
		.date_start  = "02092010",
		.date_finish = "01102010",
	};
	
	// char *ures = update_anime(&opts);
	// // char *ures = add_anime(&opts);
	// printf("%s\n", ures);
	// exit(0);

	char *name = argv[1];
	if (!name) name = "Azumanga Daioh";

	char *xml = get_search_xml(name);
	get_id_and_total(xml, &opts);

	printf("%s id = %s\n", name, opts.id);
	return 0;
}