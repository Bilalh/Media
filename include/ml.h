#ifndef MLLLHEAD
#define MLLLHEAD
#include <stdlib.h>

typedef struct _MLOpts MLOpts;
char *get_search_xml(char *name);
void get_id_and_total(char *xml, MLOpts *opts);

char *add_anime(MLOpts *opts);
char *update_anime(MLOpts *opts);
char *delete_anime(int id);


typedef enum { 
	ML_NONE   = 0, ML_WATCHING = 1, ML_COMPLETED   = 2, 
	ML_ONHOLD = 3, ML_DROPPED  = 4, ML_PLANTOWATCH = 5
} ML_status;

struct _MLOpts{
	char      episode[6];
	ML_status status; 
	int       score;          /// 1 - 10
	char      date_start[9];  /// mmddyyyy
	char      date_finish[9]; /// mmddyyyy
	
	char      id[6];
	char      total[6];
	char     *title;
};

#define ML_FIND               "http://myanimelist.net/api/anime/search.xml?q="
#define ML_ADD_ANIME          "http://myanimelist.net/api/animelist/add/"
#define ML_UPDATE_ANIME       "http://myanimelist.net/api/animelist/update/"
#define ML_DELETE_ANIME       "http://myanimelist.net/api/animelist/delete/"


#endif