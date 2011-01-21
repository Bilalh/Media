#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <curl/curl.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>


#include <include/ml.h>
#include <include/xml.def>
#include <include/string_buffer.h>
#include <include/string_buffer_extra.h>
#include <include/string_util.h>
#include <include/debug.h>
#include <include/colours.h>

typedef struct {
	char *ptr;
	size_t len;
} String_m;

String *sql_commands = NULL;

static void init_string(String_m *s);
static size_t writefunc(void *ptr, size_t size, size_t nmemb, String_m *s);

static char *mal_api(char *url, MLOpts *opts);
static void update_id_total(MLOpts *opts);
static void update_id(MLOpts *opts);
static void update_total(MLOpts *opts);
static void update_total_only(MLOpts *opts);
static void update_updated(MLOpts *opts);
static void update_end_date_fin(MLOpts *opts, const char *date);
static void update_fin(MLOpts *opts);

// does not like update_anime in the sql callback
char *update_anime2 (MLOpts *opts);

char *get_search_xml (char *o_name) {
	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_NOTHING);
	curl = curl_easy_init();
	String_m *str = malloc(sizeof(String_m));
	if(curl) {
		init_string(str);
		char *name = curl_easy_escape( curl, o_name , 0 );

		const size_t ml_len = strlen(ML_FIND), n_len = strlen(name);
		char url[ml_len + n_len];
		strncpy(url, ML_FIND, ml_len);
		strncpy(&url[ml_len], name , n_len + 1);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_USERPWD, "bhterra:bhterramai#");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);

		res = curl_easy_perform(curl);
		curl_free(name);
		curl_easy_cleanup(curl);
	}
	
	return str->ptr;
}

static char *mal_api (char *url, MLOpts *opts) {
	CURL *curl;
	CURLcode res;
	dprintf("url %s\n", url);
	
	// Sets up the connection
	curl_global_init(CURL_GLOBAL_NOTHING);
	curl = curl_easy_init();
	String_m *str = malloc(sizeof(String_m));
	if(curl) {
		init_string(str);

		xmlDocPtr doc;
		xmlNodePtr root, temp;
		xmlChar *xmlbuff;
		int buffersize;

		// makes document
		doc  = xmlNewDoc ((xmlChar*) "1.0");
		root = xmlNewNode(NULL, XC"entry");
		xmlDocSetRootElement(doc, root);

		// Makes the xml of the options
		if (opts) {
			char buff[10];
			if( *opts->episodes ) {
				dprintf("%s\n", "ep");
				// sprintf(buff, "%d", opts->episodes );
				new_text_node(temp, "episode", opts->episodes, root);
			}
			if( opts->status ) {
				dprintf("%s\n", "status");
				sprintf(buff, "%d", opts->status );
				new_text_node(temp, "status", buff, root);
			}
			if( *opts->score ) {
				dprintf("%s\n", "score");
				dprintf("%s\n", opts->score );
				
				sprintf(buff, "%s", opts->score );
				new_text_node(temp, "score", buff, root);
			}
			if( *opts->date_start ) {
				dprintf("%s\n", "date_start");
				new_text_node(temp, "date_start", opts->date_start, root);
			}
			if( *opts->date_finish ) {
				dprintf("%s\n", "date_finish");
				new_text_node(temp, "date_finish", opts->date_finish, root);
			}
		}
		
		// make xml arg 
		xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 0);
		
		char xml[buffersize + 7 + 1];
		sprintf(xml, "data=%s", (char *) xmlbuff);
		dprintf("%s\n", xml);
		
		// does post 
		curl_easy_setopt(curl, CURLOPT_POST, true);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, xml);
		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_USERPWD, "bhterra:bhterramai#");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);

		res = curl_easy_perform(curl);
		dprintf("curl res %i\n", res);
		
		curl_easy_cleanup(curl);
		xmlFree(xmlbuff);
		xmlFreeDoc(doc);
	}
	return str->ptr;
}

char *add_anime (MLOpts *opts) {
	char url[strlen(ML_ADD_ANIME) + 4 + 10 + 1];
	sprintf(url, "%s%s.xml\n", ML_ADD_ANIME, opts->id );
	return mal_api(url, opts);
}

char *update_anime (MLOpts *opts) {
	char url[strlen(ML_UPDATE_ANIME) + 4 + 10 + 1];
	sprintf(url, "%s%s.xml\n", ML_UPDATE_ANIME, opts->id );
	return mal_api(url, opts);
}

char *update_anime2 (MLOpts *opts) {
	char url[strlen(ML_UPDATE_ANIME) + 4 + 10 + 1];
	sprintf(url, "%s%s.xml\n", ML_UPDATE_ANIME, opts->id );
	return mal_api(url, opts);
}

char *delete_anime (int id) {
	char url[strlen(ML_DELETE_ANIME) + 4 + 10 + 1];
	sprintf(url, "%s%d.xml\n", ML_DELETE_ANIME, id );
	return mal_api(url, NULL);
}

void get_id_and_total(char *xml, MLOpts *opts) {
	if (xml == NULL || *xml == '\0'){
		efprintf("%s\n", "XMl null or empty");
		return;
	}
	
	bool done_id = false, done_total = false;
	if(strlen(opts->total) > 0) done_total = true;
	if(strlen(opts->id)    > 0) done_id = true;

	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	doc = xmlReadDoc(xmlCharStrdup(xml), NULL, NULL, 
		XML_PARSE_NOWARNING | XML_PARSE_NOERROR
	);
	xpathCtx = xmlXPathNewContext(doc);

	if(xpathCtx == NULL) {
		efprintf( "Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		printf("%s\n", xml);
		return;
	}

	dprintf("%s\n", "after xml doc");
	// TODO change 1024 to actual amount for the synonyms
	const int length = 20 + 14 + 15 + 5 +  1024;

	const char *t  = "translate(";
	const char *t2  = ",'ABCDEFGHIJKLMNOPQRSTUVWXYZ','abcdefghijklmnopqrstuvwxyz')";
	char buf[length];
	char *lower = str_lower(opts->title, strlen(opts->title));
	
	dprintf("%s\n", "after lowering ");
	
	
	// xpath to the entry
	sprintf(buf,
			"/anime/entry[%stitle%s='%s' or %senglish%s='%s' or %ssynonyms%s='%s']",
			t, t2, lower,   t, t2, lower,   t, t2, lower
		   );

	// xpath to the entry
	sprintf(buf,
			"/anime/entry[%stitle%s='%s' or %senglish%s='%s' or "
				"substring(%ssynonyms%s, string-length(synonyms) - string-length('%s')+1) = '%s' or "
				"contains(%ssynonyms%s, '%s;')"
			"]",
			t, t2, lower,   t, t2, lower,   t, t2, lower, lower,  t, t2, lower
		   );

	dprintf("xpath: %s\n", buf);

	dprintf("%s\n", "after buf spf ");
	//FIXME might seg fault on getting no vaild data (fixed?)
	xpathObj = xmlXPathEvalExpression(xmlCharStrdup(buf), xpathCtx);
	dprintf("%s\n\n", "after xpath eval ");
	if(xpathObj == NULL) {
		efprintf( "Error: unable to evaluate xpath expression \"...\"\n");
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		printf("%s\n", xml);
		return;
	}
	
	dprintf("%s\n", "after xpath ObJ");
	dprintf("type:%d\n", xpathObj->type);
	
	xmlNodeSetPtr nodes =  xpathObj->nodesetval;
	
	if ( ! nodes ) {
		efprintf( "%s\n", "nodes NULL");
		printf("%s\n", xml);
		return;
	}
	
	if (nodes->nodeNr == 0){
		efprintf( "%s\n", "no nodes found");
		printf("%s\n", xml);
		return;
	}
	
	dprintf("%s\n", "after xpath nodes");
	
	xmlNodePtr entry_c = nodes->nodeTab[0]->children;
	if (! entry_c ){
		efprintf( "%s\n", "entry_c failed");
		printf("%s\n", xml);
		return;
	}

	dprintf("%s\n", "after xpath get nodeTab");

	// gets the id and total number of episodes
	for(; (!done_id || !done_total) && entry_c != NULL; entry_c = entry_c->next  ) {
		if( strcmp((char *) entry_c->name, "id") == 0 ) {
			char *num = (char*) entry_c->last->content;
			strncpy(opts->id, num, 6);
			dprintf("id %s\n", opts->id);
			done_id = true;
		} else if( strcmp((char *) entry_c->name, "episodes") == 0 ) {
			char *eps = (char*) entry_c->last->content;
			strncpy(opts->total, eps, 6);
			dprintf("total %s\n", opts->total);
			done_total = true;
		}
	}


	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);
}

// sql exec callback function
int update_new(void *unused, int argc, char **argv, char **columns) {
	const char *title_arg     =  argv[0];
	const char *id_arg        =  argv[1];
	const char *ep_arg        =  argv[2];
	const char *total_arg     =  argv[3];
	const char *date_start_a  =  argv[4];
	const char *date_fin_a    =  argv[5];
	const char *fin_arg       =  argv[6];
	const char *date_a        =  argv[7];
	const char *score_arg     =  argv[8];
	
	dprintf("%s title: %s\n", "start update_new", title_arg);
	
	// sets to zero
	MLOpts opts = {};

	if( strcmp("1", fin_arg ) == 0 ) {
		opts.status = ML_COMPLETED;
		if( score_arg ) {
			dprintf("%s\n", "adding score beg");
			strncpy(opts.score, score_arg, 3);
		}	
	}else{
		opts.status = ML_WATCHING;
	}
	

	// title_arg becames invaild after strptime is called unless it is duped.
	strncpy(opts.title, title_arg, 100);
	bool have_id = false, have_total = false;

	if( ep_arg ) {
		dprintf("eps %s\n", ep_arg);
		strncpy(opts.episodes, ep_arg, 6);
	} 
	if( id_arg ) {
		dprintf("id %s\n", id_arg);
		strncpy(opts.id, id_arg, 7);
		have_id = true;
	}  
	if( total_arg ){ 
		dprintf("total %s\n", total_arg);
		strncpy(opts.total, total_arg, 6);
		have_total = true;
	}
	
	// strptime seg fault in argv 4 or 5
	if( date_start_a ) {
		strncpy(&opts.date_start[0], &date_start_a[5], 2);
		strncpy(&opts.date_start[2], &date_start_a[8], 2);
		strncpy(&opts.date_start[4], &date_start_a[0], 4);
	}

	if( date_fin_a ) {
		strncpy(&opts.date_finish[0], &date_fin_a[5], 2);
		strncpy(&opts.date_finish[2], &date_fin_a[8], 2);
		strncpy(&opts.date_finish[4], &date_fin_a[0], 4);
	}

	// Updates the ml list  and creates the corect sql querry 
	bool update_anime = false;
	
	if (!have_total || ! have_id  ) {
		dprintf("%s\n", "id or total missing");
		char *xml = get_search_xml(opts.title);
		dprintf("%s\n", "have xml");
		get_id_and_total(xml, &opts);
		dprintf("%s\n", "after get_id_and_total");
		if (!have_id && !have_total && *opts.id != '\0' && *opts.total != '\0'  ){
			dprintf("%s id '%s' total '%s'\n", "have both", opts.id, opts.total );
			update_id_total(&opts);
			update_anime = true;
		}else{
			if (!have_id && *opts.id != '\0' ) {
				dprintf("%s\n", "have id");
				update_id(&opts);
				update_anime = true;
			}
			if (!have_total && *opts.total != '\0' ){
				dprintf("%s\n", "have total");
				if (*opts.id != '\0'){
					update_total(&opts);
					update_anime = true;
				}else{
					update_total_only(&opts);	
				}
			} 
			if (*opts.total == '\0' ||  *opts.total == '\0'){
				printf("%-36s %s\n",opts.title, COLOURJ("id &| total not found",BOLD_RED));
			}
			
		}
	}else{
		update_anime = true;
		update_updated(&opts);
	}
	dprintf("%12s: '%s'\n", "title", opts.title);
	dprintf("%12s: '%s'\n", "id", opts.id);
	dprintf("%12s: '%s'\n", "episodes", opts.episodes);
	dprintf("%12s: '%s'\n", "total", opts.total);
	dprintf("%12s: '%s'\n", "date_start", opts.date_start);

	if( strcmp(opts.episodes,opts.total) == 0 ){
		if( *opts.date_finish == '\0' ){
			strncpy(&opts.date_finish[0], &date_a[5], 2);
			strncpy(&opts.date_finish[2], &date_a[8], 2);
			strncpy(&opts.date_finish[4], &date_a[0], 4);
			update_end_date_fin(&opts, date_a);
		}else{
			update_fin(&opts);
		}
		if( score_arg ) {
			dprintf("%s\n", "adding score end");
			strncpy(opts.score, score_arg, 3);	
		}
		opts.status = ML_COMPLETED;
	}

	dprintf("%12s: '%s'\n\n", "date_finish", opts.date_finish);
	dprintf("%12s: '%s'\n\n", "score", opts.score);
	
	
	if (update_anime){
		add_anime(&opts);
		printf("%-36s " SSS("%s") "\n",opts.title, COLOUR(update_anime2(&opts),BLUE) );
	}
	
	return 0;
}

static void update_fin(MLOpts *opts){

	if (sql_commands == NULL){
		const int len = 53 + strlen(opts->title) + 1;
		sql_commands = string_new(len);
	}
	
	string_mprintf(sql_commands,
		"Update SeriesInfo Set Finished = 1 where Title = %Q; ", 
		opts->title
	);
}

static void update_end_date_fin(MLOpts *opts, const char *date){

	if (sql_commands == NULL){
		const int len = 67 + 20 + strlen(opts->title) + 1;
		sql_commands = string_new(len);
	}
	
	string_mprintf(sql_commands,
		"Update SeriesInfo Set EndDate = %Q, Finished = 1 where Title = %Q; ", 
		date, opts->title
	);
}

// makes sql statements
static void update_updated(MLOpts *opts){
	
	if (sql_commands == NULL){
		const int len = 54 + strlen(opts->title) + 1;
		sql_commands = string_new(len);
	}
	
	string_mprintf(sql_commands,
		"Update SeriesInfo Set Updated = 1 where Title = %Q; ", 
		opts->title
	);
}

static void update_id_total(MLOpts *opts){
	
	if (sql_commands == NULL){
		const int len = 75 + strlen(opts->title) + 1;
		sql_commands = string_new(len);
	}
	
	string_mprintf(sql_commands,
		"Update SeriesInfo Set Updated = 1, Total = %s, Id = %s where Title = %Q; ", 
		opts->total, opts->id, opts->title
	);
	
}

static void update_id(MLOpts *opts){
	if (sql_commands == NULL){
		const int len = 63 + strlen(opts->title) + 1;
		sql_commands = string_new(len);
	}
	
	string_mprintf(sql_commands,
		"Update SeriesInfo Set Updated = 1, Id = %s where Title = %Q; ", 
		opts->id, opts->title
	);
	
}

static void update_total(MLOpts *opts){
	
	if (sql_commands == NULL){
		const int len = 67 + strlen(opts->title) + 1;
		sql_commands = string_new(len);
	}
	
	string_mprintf(sql_commands,
		"Update SeriesInfo Set Updated == 1, Total = %s where Title = %Q; ", 
		opts->total, opts->title
	);
	
}

static void update_total_only(MLOpts *opts){
	
	if (sql_commands == NULL){
		const int len = 53 + strlen(opts->title) + 1;
		sql_commands = string_new(len);
	}
	
	string_mprintf(sql_commands,
		"Update SeriesInfo Set Total = %s where Title =  %Q; ", 
		opts->total, opts->title
	);
	
}

// writes all data to the String_m struct
static size_t writefunc (void *ptr, size_t size, size_t nmemb, String_m *s) {
	size_t new_len = s->len + size * nmemb;
	s->ptr = realloc(s->ptr, new_len + 1);
	if (s->ptr == NULL) {
		efprintf( "realloc() failed writefunc in ml.c\n");
		exit(EXIT_FAILURE);
	}
	memcpy(s->ptr + s->len, ptr, size * nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;
	return size * nmemb;
}

static void init_string (String_m *s) {
	s->len = 0;
	s->ptr = malloc(s->len + 1);
	s->ptr[0] = '\0';
}

