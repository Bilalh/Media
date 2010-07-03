#ifndef OPTIONPHEAD
#define OPTIONPHEAD
#include "media.h"
#include <stdbool.h>
#include <Block.h>

#define NO_SHORT_OPT 0
typedef void (^VoidBlock)();

MediaArgs *new_media_args();
MediaArgs *option_parser(int argc, char **argv);
void print_help(); 
void print_media_args(MediaArgs *ma);

typedef struct{
	const struct option opt;
	const char *arg;
	const char *help;
	VoidBlock block;
	const bool neg;
} Element;

typedef struct {
	const char *grouping;
	const int length;
	const Element *links;
} HelpLink;


#endif