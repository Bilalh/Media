#ifndef MLLLHEAD
#define MLLLHEAD
#include <stdlib.h>

char *get_search_xml(char *name);
long getId(char *xml, char *name);

typedef struct _MLOpts MLOpts;
char *add_anime(int id, MLOpts *opts);
char *update_anime(int id, MLOpts *opts);
char *delete_anime(int id);


typedef enum { 
	ML_NONE   = 0, ML_WATCHING = 1, ML_COMPLETED   = 2, 
	ML_ONHOLD = 3, ML_DROPPED  = 4, ML_PLANTOWATCH = 5
} ML_status;

struct _MLOpts{
	int       episode;
	ML_status status; 
	int       score;          /// 1 - 10
	char      date_start[9];  /// mmddyyyy
	char      date_finish[9]; /// mmddyyyy
};

#define ML_FIND               "http://myanimelist.net/api/anime/search.xml?q="
#define ML_ADD_ANIME          "http://myanimelist.net/api/animelist/add/"
#define ML_UPDATE_ANIME       "http://myanimelist.net/api/animelist/update/"
#define ML_DELETE_ANIME       "http://myanimelist.net/api/animelist/delete/"


#endif