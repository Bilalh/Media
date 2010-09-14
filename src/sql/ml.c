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

typedef struct{
	char *ptr;
	size_t len;
} String_m;

static void init_string(String_m *s);
static size_t writefunc(void *ptr, size_t size, size_t nmemb, String_m *s);

char *get_search_xml(char *o_name) {
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

char * mal_api(char *url){
	CURL *curl;
	CURLcode res;

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
		
		char buff[10];
		
		sprintf(buff, "%d\n",1 );
		new_text_node(temp, "status",buff, root);
		sprintf(buff, "%d\n",6 );
		new_text_node(temp, "score",buff, root);
		
		xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 0);
		char xml[buffersize + 7 + 1];
		sprintf(xml, "data=%s", (char *) xmlbuff);
		
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

char *add_anime(int id) {
	char url[strlen(ML_ADD_ANIME) + 4 + 10 + 1];
	sprintf(url, "%s%d.xml\n", ML_ADD_ANIME, id );
	return mal_api(url);
}

char *update_anime(int id) {
	char url[strlen(ML_UPDATE_ANIME) + 4 + 10 + 1];
	sprintf(url, "%s%d.xml\n", ML_UPDATE_ANIME, id );
	return mal_api(url);
}

char *delete_anime(int id) {
	char url[strlen(ML_DELETE_ANIME) + 4 + 10 + 1];
	sprintf(url, "%s%d.xml\n", ML_DELETE_ANIME, id );
	return mal_api(url);
}

long getId(char *xml, char *name) {
	int result  = -1;
	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;
	
	doc = xmlParseDoc(XC xml);
	xpathCtx = xmlXPathNewContext(doc);

	if(xpathCtx == NULL) {
		fprintf(stderr, "Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return(result);
	}

	const int length = 20 + 14 + 15 + 5 + strlen(xml) * 3 + 1;

	const char *t  = "translate(";
	const char *t2  = ",'ABCDEFGHIJKLMNOPQRSTUVWXYZ','abcdefghijklmnopqrstuvwxyz')";
	char buf[length];
	char *lower = str_lower(name, strlen(name));
	sprintf(buf,
			"/anime/entry[%stitle%s='%s' or %senglish%s='%s' or %ssynonyms%s='%s']/id",
			t, t2, lower,   t, t2, lower,   t, t2, lower
		   );

	xpathObj = xmlXPathEvalExpression(XC buf, xpathCtx);
	if(xpathObj == NULL) {
		fprintf(stderr, "Error: unable to evaluate xpath expression \"...\"\n");
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return(result);
	}

	xmlNodeSetPtr nodes =  xpathObj->nodesetval;
	int size = (nodes) ? nodes->nodeNr : 0;
	xmlNodePtr cur;
	if(size > 0 && nodes->nodeTab[0]->type == XML_ELEMENT_NODE &&
			(cur = nodes->nodeTab[0]->last) ) {
		char *num = (char*) cur->content;
		result = strtol(num, NULL, 10);
	}

	/* Cleanup */
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);
	return result;
}

static size_t writefunc(void *ptr, size_t size, size_t nmemb, String_m *s) {
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

void init_string(String_m *s) {
	s->len = 0;
	s->ptr = malloc(s->len + 1);
	s->ptr[0] = '\0';
}

int main (int argc, char  *argv[]) {
	
	char *ures = update_anime(3750);
	// char *ures = add_anime(3750);
	printf("%s\n", ures);
	exit(0);
	char *name = argv[1];
	if (!name) name ="Azumanga Daioh";
	
	char *xml = get_search_xml(name);
	long id = getId(xml, name);
	
	printf("%s id = %li\n", name, id);
	return 0;
}


// command line version
//update

// curl -u bhterra:bhterramai# -d data="<?xml version=\"1.0\" encoUTF-8\"?><entry><status>1</status><score></score></entry>" http://myanimelist.net/api/animelist/update/101.xml 

//add

// curl -u bhterra:bhterramai# -d data="<?xml version=\"1.0\" encoding=\"UTF-8\"?><entry><status>1</status><score>6</score></entry>" http://myanimelist.net/api/animelist/add/3750.xml
