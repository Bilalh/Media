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
#include <include/string_util.h>
#include <include/debug.h>

typedef struct {
	char *ptr;
	size_t len;
} String_m;

String *sql_commands = NULL;

static void init_string(String_m *s);
static size_t writefunc(void *ptr, size_t size, size_t nmemb, String_m *s);
static char *mal_api(char *url, MLOpts *opts);
void update_id_total(MLOpts *opts);

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
			if(opts->episodes) {
				dprintf("%s\n", "ep");
				// sprintf(buff, "%d", opts->episodes );
				new_text_node(temp, "episode", opts->episodes, root);
			}
			if(opts->status) {
				dprintf("%s\n", "status");
				sprintf(buff, "%d", opts->status );
				new_text_node(temp, "status", buff, root);
			}
			if(opts->score) {
				dprintf("%s\n", "score");
				sprintf(buff, "%d", opts->score );
				new_text_node(temp, "score", buff, root);
			}
			if(opts->date_start) {
				dprintf("%s\n", "date_start");
				new_text_node(temp, "date_start", opts->date_start, root);
			}
			if(opts->date_finish) {
				dprintf("%s\n", "date_finish");
				new_text_node(temp, "date_finish", opts->date_finish, root);
			}
		}

		xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 0);
		char xml[buffersize + 7 + 1];
		sprintf(xml, "data=%s", (char *) xmlbuff);
		dprintf("%s\n", xml);
		curl_easy_setopt(curl, CURLOPT_POST, true);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, xml);
		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_USERPWD, "bhterra:bhterramai#");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);

		res = curl_easy_perform(curl);
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

char *delete_anime (int id) {
	char url[strlen(ML_DELETE_ANIME) + 4 + 10 + 1];
	sprintf(url, "%s%d.xml\n", ML_DELETE_ANIME, id );
	return mal_api(url, NULL);
}

void get_id_and_total(char *xml, MLOpts *opts) {
	bool done_id = false, done_total = false;
	if(strlen(opts->total) > 0) done_total = true;
	if(strlen(opts->id)    > 0) done_id = true;

	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	doc = xmlParseDoc(XC xml);
	xpathCtx = xmlXPathNewContext(doc);

	if(xpathCtx == NULL) {
		fprintf(stderr, "Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return;
	}

	printf("%s\n", "after xml doc");
	const int length = 20 + 14 + 15 + 5 + strlen(xml) * 3 + 1;

	const char *t  = "translate(";
	const char *t2  = ",'ABCDEFGHIJKLMNOPQRSTUVWXYZ','abcdefghijklmnopqrstuvwxyz')";
	char buf[length];
	char *lower = str_lower(opts->title, strlen(opts->title));
	

	// xpath to the entry
	sprintf(buf,
			"/anime/entry[%stitle%s='%s' or %senglish%s='%s' or %ssynonyms%s='%s']",
			t, t2, lower,   t, t2, lower,   t, t2, lower
		   );

	xpathObj = xmlXPathEvalExpression(XC buf, xpathCtx);
	if(xpathObj == NULL) {
		fprintf(stderr, "Error: unable to evaluate xpath expression \"...\"\n");
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return;
	}
	
	xmlNodeSetPtr nodes =  xpathObj->nodesetval;
	if (nodes->nodeNr == 0){
		fprintf(stderr, "%s\n", "no nodes found");
		return;
	}
	printf("%s\n", "after xpath get nodes");
	
	
	if (! nodes) {
		fprintf(stderr, "%s\n", "nodes failed");
		return;
	}
	xmlNodePtr entry_t = nodes->nodeTab[0];
	printf("%s\n", "after xpath get nodeTab");
	
	
	xmlNodePtr entry_c = nodes->nodeTab[0]->children;
	if (! entry_c ){
		fprintf(stderr, "%s\n", "entry_c failed");
		return;
	}
	

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
	printf("%s\n", "start update_new");
	// for(int i = 0; i < argc; ++i){
	// 	printf("%s = %s\n", columns[i], argv[i] ? argv[i] : "NULL");
	// }
	// printf("\n");



	MLOpts opts = {
		.status      = strcmp("1", argv[6]) == 0 ? ML_COMPLETED : ML_WATCHING
	};


	// argv[0] becames invaild after strptime is called unless it is duped.
	char *t = strdup(argv[0]);
	strncpy(opts.title,   t, 100);
	free(t);

	bool have_id = false, have_total = false;


	if (argv[2]) strncpy(opts.episodes, argv[2], 6);
	if (argv[1]) {
		strncpy(opts.id, argv[1], 7);
		have_id = true;
	}
	
	if (argv[3]){ 
		strncpy(opts.total,argv[3], 6);
		have_total = true;
	}
	
	
	// strptime seg fault in argv 4 or 5
	struct tm* tm;
	if( argv[4] ) {
		strncpy(&opts.date_start[0],  &argv[4][5], 2);
		strncpy(&opts.date_start[2], &argv[4][8], 2);
		strncpy(&opts.date_start[4], &argv[4][0], 4);
		
	}

	if( argv[5] ) {
		strncpy(&opts.date_start[0],  &argv[4][5], 2);
		strncpy(&opts.date_start[2], &argv[4][8], 2);
		strncpy(&opts.date_start[4], &argv[4][0], 4);
	}


	printf("%12s: '%s'\n", "title", opts.title);
	printf("%12s: '%s'\n", "id", opts.id);
	printf("%12s: '%s'\n", "episodes", opts.episodes);
	printf("%12s: '%s'\n", "total", opts.total);
	printf("%12s: '%s'\n", "date_start", opts.date_start);
	printf("%12s: '%s'\n", "date_finish", opts.date_finish);

	if (!have_total || ! have_id  ) {
		char *xml = get_search_xml(opts.title);
		printf("%s\n", "have xml");
		get_id_and_total(xml, &opts);
		
		if (!have_id && !have_total && *opts.id != '\0' && *opts.total != '\0'  ){
			printf("%s id '%s' total '%s'\n", "have both", opts.id, opts.total );
			update_id_total(&opts);
		}else{
			if (!have_id && *opts.id != '\0' ) {
				printf("%s\n", "have id");
			}
			if (!have_total && *opts.total != '\0' ){
				printf("%s\n", "have total");	
			} 
			printf("%s\n", "end if");
		}
	}

	printf("%12s: '%s'\n", "title", opts.title);
	printf("%12s: '%s'\n", "id", opts.id);
	printf("%12s: '%s'\n", "episodes", opts.episodes);
	printf("%12s: '%s'\n", "total", opts.total);
	printf("%12s: '%s'\n", "date_start", opts.date_start);
	printf("%12s: '%s'\n", "date_finish", opts.date_finish);
	return 0;
}

void update_id_total(MLOpts *opts){
	int len = 64 + strlen(opts->title) + 1;
	if (sql_commands == NULL){
		sql_commands = string_new(len);
	}
	printf("beg %s\n", sql_commands->str);
	
	string_sprintf(sql_commands, len,
		"Update SeriesInfo Set Total = %s, Id = %s where Title = '%s'; ", 
		opts->total, opts->id, opts->title
	);
	printf("end %s\n", sql_commands->str);
	
}

// writes all data to the String_m struct
static size_t writefunc (void *ptr, size_t size, size_t nmemb, String_m *s) {
	size_t new_len = s->len + size * nmemb;
	s->ptr = realloc(s->ptr, new_len + 1);
	if (s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}
	memcpy(s->ptr + s->len, ptr, size * nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;
	return size * nmemb;
}

void init_string (String_m *s) {
	s->len = 0;
	s->ptr = malloc(s->len + 1);
	s->ptr[0] = '\0';
}

