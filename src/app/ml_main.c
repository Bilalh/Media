#include <time.h>
#include <stdio.h>
#include <string.h>

#include <sqlite3.h>

#include <include/ml.h>
#include <include/history.h>







int main (int argc, char  *argv[]) {
	sql_exec("SELECT Title, ID, Current, Total, StartDate, EndDate, Finished"
	         " FROM SeriesInfo WHERE (updated == 0 AND skip == 0 AND Title = 'Angel Beats!')", 
			 update_new);
	return 0;
}


// int main (int argc, char  *argv[]) {
// 
// 	MLOpts opts = {
// 		.title       = "Azumanga Daioh",
// 		.episodes    = "1",
// 		.status      = ML_PLANTOWATCH,
// 		.score       = 6,
// 		.date_start  = "02092010",
// 		.date_finish = "01102010",
// 	};
// 	
// 	// char *ures = update_anime(&opts);
// 	// // char *ures = add_anime(&opts);
// 	// printf("%s\n", ures);
// 	// exit(0);
// 
// 	char *xml = get_search_xml(opts.title);
// 	get_id_and_total(xml, &opts);
// 
// 	printf("%15s    id = %s\n%15s total = %s\n", opts.title, opts.id,"", opts.episodes);
// 	return 0;
// }