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
#include <include/string_util.h>
#include <include/debug.h>

typedef struct {
	char *ptr;
	size_t len;
} String_m;

static void init_string(String_m *s);
static size_t writefunc(void *ptr, size_t size, size_t nmemb, String_m *s);
static char *mal_api(char *url, MLOpts *opts);

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
		if (opts){
			char buff[10];
			if(opts->episode){
				dprintf("%s\n", "ep");
				// sprintf(buff, "%d", opts->episode );
				new_text_node(temp, "episode", opts->episode, root);
			}
			if(opts->status){
				dprintf("%s\n", "status");
				sprintf(buff, "%d", opts->status );
				new_text_node(temp, "status", buff, root);
			}
			if(opts->score){
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

char *delete_anime (int id){
	char url[strlen(ML_DELETE_ANIME) + 4 + 10 + 1];
	sprintf(url, "%s%d.xml\n", ML_DELETE_ANIME, id );
	return mal_api(url, NULL);
}

void get_id_and_total(char *xml,MLOpts *opts) {
	if (strlen(opts->total) > 0 && strlen(opts->id) >0 ) return;
	
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

	const int length = 20 + 14 + 15 + 5 + strlen(xml) * 3 + 1;

	const char *t  = "translate(";
	const char *t2  = ",'ABCDEFGHIJKLMNOPQRSTUVWXYZ','abcdefghijklmnopqrstuvwxyz')";
	char buf[length];
	char *lower = str_lower(opts->title, strlen(opts->title));
	sprintf(buf,
			"/anime/entry[%stitle%s='%s' or %senglish%s='%s' or %ssynonyms%s='%s']/id",
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
	int size = (nodes) ? nodes->nodeNr : 0;
	xmlNodePtr cur;
	if(size > 0 && nodes->nodeTab[0]->type == XML_ELEMENT_NODE &&
			(cur = nodes->nodeTab[0]->last) ) {
		char *num = (char*) cur->content;
		strncpy(opts->id, num, 6);
	}


	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);
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


// command line version
//update

// curl -u bhterra:bhterramai# -d data="<?xml version=\"1.0\" encoUTF-8\"?><entry><status>1</status><score></score></entry>" http://myanimelist.net/api/animelist/update/101.xml

//add

// curl -u bhterra:bhterramai# -d data="<?xml version=\"1.0\" encoding=\"UTF-8\"?><entry><status>1</status><score>6</score></entry>" http://myanimelist.net/api/animelist/add/3750.xml
