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

typedef struct _str {
	char *ptr;
	size_t len;
} String;

void print_xpath_nodes(xmlNodeSetPtr nodes, FILE* output);
static void init_string(String *s);
static size_t writefunc(void *ptr, size_t size, size_t nmemb, String *s);

char *get_search_xml(char *name) {
	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_NOTHING);
	curl = curl_easy_init();
	String *str = malloc(sizeof(String));
	if(curl) {
		init_string(str);

		const size_t ml_len = strlen(ML_API), n_len = strlen(name);
		char url[ml_len + n_len];
		strncpy(url, ML_API, ml_len);
		strncpy(&url[ml_len], name , n_len + 1);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_USERPWD, "bhterra:bhterramai#");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);
		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
	}
	return str->ptr;
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

static size_t writefunc(void *ptr, size_t size, size_t nmemb, String *s) {
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

void init_string(String *s) {
	s->len = 0;
	s->ptr = malloc(s->len + 1);
	s->ptr[0] = '\0';
}

// int main (int argc, char const *argv[]) {
// 	char *xml = get_search_xml("Azumanga Daioh");
// 	long id = getId(xml, "Azumanga Daioh");
// 	printf("id = %li\n", id);
// 	return 0;
// }
