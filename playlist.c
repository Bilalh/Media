#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "playlist.h"
#include <libxml/encoding.h>

#define XC (xmlChar*)

#define new_node(node,name,parent) \
 node = xmlNewNode(NULL,XC name); \
 node = xmlAddChild(parent,node);

#define new_text_node(node,name,value,parent) \
 node = xmlNewTextChild(parent,NULL,XC name, XC value); \
 node = xmlAddChild(parent,node);

bool make_playlist(char *filename, char *dir,  char **names, Pformat formats) {

	const int extra  =  2 + 6; // 2 for \0 and / 6 for a 5 char exe
	char fullpath[strlen(filename) + strlen(dir)+extra];
	printf(" m3u %i plist %i pls %i xspf %i\n",
		   (formats & F_M3U),
		   (formats & F_PLIST),
		   (formats & F_PLS),
		   (formats & F_XSPF)
		  );
	bool result  = true;

	if ( formats & F_M3U ) {
		snprintf(fullpath, sizeof(fullpath), "%s/%s.m3u", dir, filename );
		printf("%s\n", fullpath);
		result &= make_m3u(fullpath, names);
	}
	if (formats & F_PLIST) {
		snprintf(fullpath, sizeof(fullpath), "%s/%s.plist", dir, filename );
		printf("%s\n", fullpath);
		result &= make_plist(fullpath, names);
	}
	if (formats & F_PLS) {
		snprintf(fullpath, sizeof(fullpath), "%s/%s.pls", dir, filename );
		printf("%s\n", fullpath);
		result &= make_plist(fullpath, names);
	}
	if (formats & F_XSPF) {
		snprintf(fullpath, sizeof(fullpath), "%s/%s.xspf", dir, filename );
		printf("%s\n", fullpath);
		result &= make_xspf(fullpath, names);
	}

	return result;
}

bool make_m3u(char *filepath, char **names) {
	FILE *out = fopen(filepath, "w");

	while(**names != '\0') {
		fprintf(out, "%s\n", *names);
		names++;
	}

	fclose(out);
	return true;
}

//TODO string names
bool make_plist(char *filepath, char **names) {
	xmlDocPtr doc;
	xmlNodePtr root, rr, cur,dict, dict2, array,t1;
	xmlAttrPtr attr;

	// makes document
	doc  = xmlNewDoc ((xmlChar*) "1.0");
	root = xmlNewNode(NULL, XC"temp");
	rr   = xmlDocSetRootElement(doc, root);

	// makes root node
	cur = xmlNewTextChild (root, NULL, XC"plist", NULL);
	attr = xmlNewProp(cur, XC"version", XC "1.0");
	xmlReplaceNode(root, cur);
	root = cur;

	new_node(dict, "dict", root);
	new_text_node(t1, "key","Contents", dict);
	
	new_node(dict2, "dict", dict);
	new_text_node(t1, "key","Playlist", dict2);
	
	// <array> 
	new_node(array, "array", dict2);
	while (**names){
		new_text_node(t1, "string",*names, array);
		names++;
	}
	
	// dict2 children - Random
	new_text_node(t1, "key","Random", dict2);
	new_node(t1, "false", dict2);
	// Repeat
	new_text_node(t1, "key","Repeat", dict2);
	new_text_node(t1, "integer","1", dict2);
	// Volume
	new_text_node(t1, "key","Volume", dict2);
	new_text_node(t1, "integer","1", dict2);
	
	// dict children - MajorVersion
	new_text_node(t1, "key","MajorVersion", dict);
	new_text_node(t1, "integer","0", dict);
	// MinorVersion
	new_text_node(t1, "key","MinorVersion", dict);
	new_text_node(t1, "integer","1", dict);
	
	// writes to file
	puts("");
	xmlSaveFormatFile( "-", doc, 1);
	return false;
}

bool make_pls(char *filepath, char **names) {
	fprintf(stderr, "%s\n", "pls not impermented");
	return false;
}

bool make_xspf(char *filepath, char **names) {
	fprintf(stderr, "%s\n", "xspf not impermented");
	return false;
}
