#ifndef MLLLHEAD
#define MLLLHEAD
#include <stdlib.h>

char *get_search_xml(char *name);
long getId(char *xml, char *name);

char *add_anime(int id);
char *update_anime(int id);
char *delete_anime(int id);

typedef struct {
	int episode;
	int status;
	int score;
	char date_start[9]; /// mmddyyyy
	char date_finish[9]; /// mmddyyyy
} MLOpts

enum { ML_NONE =0, ML_WATCHING = 1, ML_COMPLETED = 2, 
	   ML_ONHOLD = 3, ML_DROPPED = 4, ML_PLANTOWATCH = 5 };

#define ML_FIND               "http://myanimelist.net/api/anime/search.xml?q="
#define ML_ADD_ANIME          "http://myanimelist.net/api/animelist/add/"
#define ML_UPDATE_ANIME       "http://myanimelist.net/api/animelist/update/"
#define ML_DELETE_ANIME       "http://myanimelist.net/api/animelist/delete/"


#endif