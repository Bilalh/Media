#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <include/history.h>

int main (int argc, char const *argv[]) {
	
	if (argc > 4 || argc < 3){
		printf("%s\n", "set_id series id [total]");
		exit(0);
	}
	
	char sql[161 + strlen(argv[1]) + strlen(argv[2]) + (argc == 4 ? strlen(argv[3]) :0) ];
	
	if (argc == 3 ){
		sprintf(sql, "Update SeriesInfo Set id = %s Where Title =\"%s\"", 
			argv[2], argv[1]
		);
	}else{
		sprintf(sql, "Update SeriesInfo Set id = %s, total = %s Where Title =\"%s\"", 
			argv[2], argv[3], argv[1]
		);
	}
	
	printf("%s\n", sql);
	sql_exec(strdup(sql), NULL);
	return 0;
}