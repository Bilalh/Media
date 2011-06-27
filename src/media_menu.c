#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#include <sqlite3.h>

#include <include/colours.h>
#include <include/media_menu.h>
#include <include/string_buffer.h>
#include <include/string_array.h>
#include <include/string_util.h>
#include <include/uthash.h>

#include <prefs.h>


#define EPS_ARR(eps_ptr,index) ((Ep*) (eps_ptr)->eps->arr[index])

typedef struct {
	char *full;
	long   num;
} Ep;

typedef struct {
	char        *series;
	ArrayList   *eps;
	UT_hash_handle hh;
} Eps;


static inline int longcmp(long a, long b) {
	return  a > b ? 1 : (a < b ? -1 : 0) ;
}


void show_menu(char **filenames, size_t *length, bool free_unused){
	
	size_t file_num = *length;
	Eps* hash = NULL, *h;
	
	for(size_t i = 0;  i < file_num; i++){
		
		
		char *s = basename(filenames[i]);
		
		// Spilt the name and the ep num
		char **ans = ep_num(s);
		if (ans[0] == NULL){
			efprintf("Number not found for file '%s'\n", filenames[i]);
			exit(44);
		}
		
		EP_GET_NAME_M(ans, name, s);
		EP_GET_NUMBER(ans,num);
		
		// Check if it in the hash
		HASH_FIND_STR(hash, name, h);
		
		
		if (h == NULL){ // add it 
			h = (Eps*) malloc(sizeof(Eps));
			h->eps = new_arraylist((int)file_num/2);
			h->series = name;
			HASH_ADD_KEYPTR( hh, hash, h->series, (int) strlen(h->series), h );
		}
		
		// add to the end of list 
		Ep *ep = malloc(sizeof(Ep));
		ep->full = filenames[i];
		ep->num  = num;
		arraylist_add(h->eps, ep);
		
	}
	
	bool ordered = true;
	int index = 0;
	const unsigned len = HASH_COUNT(hash);
	Eps *eps_ptr[len];
	
	
	for(Eps *e=hash; e != NULL; e=e->hh.next, ++index) {
		
		eps_ptr[index] =  e;
		printf("%-2d :", index);
		
		//		qsort_b(e->eps->arr, e->eps->index, sizeof(size_t),
		//			^(const void *a, const void *b){
		//				const Ep *ea = *((Ep**)a), *eb = *((Ep**)b);
		////				dprintf("%ld %ld res: %d\n", ea->num, eb->num, longcmp( ea->num, eb->num ) );
		//				return longcmp( ea->num , eb->num );
		//			}
		//		);
		
		if (ordered && e->eps->index > 1 ){
			printf("%4ld-%-4ld", EPS_ARR(e,0)->num, EPS_ARR(e,e->eps->index-1)->num   );
			ordered = false;
		}else{
			const int min =  e->eps->index < 3 ? e->eps->index :3;
			for(int i = 0; i<min;i++){
				printf("%2ld ", EPS_ARR(e,i)->num );
			}
			// line up
			const int extra = (3-min)*3;
			for(int i = 0; i<extra;i++){
				putchar(' ');
			}
		}
		
		printf(" %s\n", e->series);
    }
	
	int res = -1;

	while (res < 0 || res >=len){
		printf("%s [%d,%u]\n", "Choose an Episode to watch in", 0, len-1);
		
		// use readline and regex for opts like  3:3
		scanf("%d", &res);
		char f_buff[4096];
		fgets(f_buff, 4096, stdin);
	}
	
	// use a array indexing index to series then use that to select the right series
	Eps *selected = eps_ptr[res];
	
	int j;
	file_num = selected->eps->index;
	for(j = 0; j <file_num;j++){
		filenames[j] = ((Ep*) selected->eps->arr[j])->full;
	}
	filenames[j] = '\0';
		
	*length = file_num;
}



void show_menu2(char **filenames, size_t *length, bool free_unused){
	
	sqlite3 *db;
	int result;
	
	result = sqlite3_open(DATABASE, &db);
	if( result ) {
		efprintf("show_menu failed: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(23);
	}
	
	sqlite3_stmt *statement_h;
	const char *query_h = "select current from SeriesData where Title = ?";
	sqlite3_prepare_v2(db, query_h, (int) strlen(query_h), &statement_h, NULL);
	
	size_t file_num = *length;
	for(int i = 0; i < file_num; ++i){
		
		char **ep_num_ans  = ep_num(filenames[i]);
		if (ep_num_ans[0] != NULL){
			EP_GET_NUMBER(ep_num_ans,num);
			EP_GET_NAME(ep_num_ans, s_title, filenames[i]);
			sqlite3_bind_text(statement_h, 1, s_title, -1, SQLITE_TRANSIENT);
			
			result = sqlite3_step(statement_h);
			mmprintf("r:%i Row:%i Ok:%i done:%i \n", result, SQLITE_ROW, SQLITE_OK, SQLITE_DONE );
			
			// check latest
			if (result == SQLITE_ROW||  result == SQLITE_OK  || result == SQLITE_DONE){
				
				int current = sqlite3_column_int(statement_h, 0);
				mmprintf("current:%d\n",current );
				
				printf(SSS("%-2d") ":  N: " SSS("%2ld") "  P: "  SSS("%2d") " %s \n", 
					   COLOUR(i,GREEN),COLOUR(num,BLUE),COLOUR(current,BLUE), s_title );
				
				
			}else{
				efprintf(  "SQL error %s : %s\n", *filenames, sqlite3_errmsg(db));
				exit(12);
			}
			
			result = sqlite3_reset(statement_h);
		}else{
			efprintf("ep_num null in media (%s)\n", filenames[i]);
			exit(22);
		}
	}
	
	sqlite3_finalize(statement_h);
	sqlite3_close(db);
	
	int res = -1;
	while (res < 0 || res >= file_num){
		printf("%s [%d,%zu]\n", "Choose an Episode to watch in", 0, file_num-1);
		scanf("%d", &res);
		//TODO better line length
		char f_buff[4096];
		fgets(f_buff, 4096, stdin);
	}
	
	// Picks only the chosen entry 
	// TODO free other entries
	filenames[0] = filenames[res];
	if (file_num >1){
		filenames[1] = '\0';
	}
	file_num = 1;
	*length = file_num;
}
