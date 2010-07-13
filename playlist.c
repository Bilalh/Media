#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/encoding.h>

#include "playlist.h"
#include "xml.def"

#define PLAYLIST_INT_LENGTH 10

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
		result &= make_pls(fullpath, names);
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

	while(**names) {
		fprintf(out, "%s\n", *names);
		names++;
	}

	fclose(out);
	return true;
}

//TODO string path names
//TODO apple top
bool make_plist(char *filepath, char **names) {
	xmlDocPtr doc;
	xmlNodePtr root, rr, cur,dict, dict2, array,temp;
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
	new_text_node(temp, "key","Contents", dict);
	
	new_node(dict2, "dict", dict);
	new_text_node(temp, "key","Playlist", dict2);
	
	// <array> 
	new_node(array, "array", dict2);
	while (**names){
		new_text_node(temp, "string",*names, array);
		names++;
	}
	
	// dict2 children - Random
	new_text_node(temp, "key","Random", dict2);
	new_node(temp, "false", dict2);
	// Repeat
	new_text_node(temp, "key","Repeat", dict2);
	new_text_node(temp, "integer","1", dict2);
	// Volume
	new_text_node(temp, "key","Volume", dict2);
	new_text_node(temp, "integer","1", dict2);
	
	// dict children - MajorVersion
	new_text_node(temp, "key","MajorVersion", dict);
	new_text_node(temp, "integer","0", dict);
	// MinorVersion
	new_text_node(temp, "key","MinorVersion", dict);
	new_text_node(temp, "integer","1", dict);
	
	// writes to file
	puts("");
	xmlSaveFormatFile( "-", doc, 1);
	return false;
}

//TODO string path names
bool make_pls(char *filepath, char **names) {
	FILE *out = fopen(filepath, "w");
	fprintf(out, "%s\n", "[playlist]");
	int i = 1;
	while(**names) {
		fprintf(out, "File%i=%s\nTitle%i=%s\n", 
			i,*names,i,*names
		);
		names++;
	}

	fclose(out);
	return false;
}
//CHECK xmlCleanupParser?

//TODO string path names
bool make_xspf(char *filepath, char **names) {
	xmlDocPtr doc;
	xmlNodePtr root, rr, trackList,track, temp;
	xmlAttrPtr attr;

	// makes document
	doc  = xmlNewDoc ((xmlChar*) "1.0");
	root = xmlNewNode(NULL, XC"temp");
	rr   = xmlDocSetRootElement(doc, root);

	// makes root node
	temp = xmlNewTextChild (root, NULL, XC"playlist", NULL);
	attr = xmlNewProp(temp, XC"version", XC "1");
	attr = xmlNewProp(temp, XC"xmlns", XC "http://xspf.org/ns/0/");
	xmlReplaceNode(root, temp);
	root = temp;
	// tracklist
	new_node(trackList, "trackList", root);
	
	int i = 1;
	while (**names){
		new_node(track, "track", trackList);
		new_text_node(temp, "location",*names, track);
		new_text_node(temp, "title",*names, track);
		char num[PLAYLIST_INT_LENGTH];
		sprintf(num, "%i", i);
		new_text_node(temp, "trackNum",num, track);
		names++; i++;
	}
	
	// writes to file
	puts("");
	xmlSaveFormatFile( "-", doc, 1);
	return false;
}
