#ifndef MLLLHEAD
#define MLLLHEAD
#include <stdlib.h>

char *get_search_xml(char *name);
long getId(char *xml,char *name);

#define ML_API  "http://myanimelist.net/api/anime/search.xml?q="

#endif