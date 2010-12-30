#ifndef OPTIONPHEAD
#define OPTIONPHEAD
#include <getopt.h>
#include <stdbool.h>
#include <Block.h>
#include "media_args.h"

#define NO_SHORT_OPT 0
typedef void (^VoidBlock)();

MediaArgs *new_media_args();

/**
 * @brief parses the options, can only be used once
 *
 * @return  A Media Args object with the opts
**/
MediaArgs *option_parser(int *p_argc, char ***p_argv);

// prints the help, section or letter if specifed
void print_help(); 

void print_usage();

// prints the media_args struct
void print_media_args(MediaArgs *ma);

typedef struct{
	const struct option opt;
	const char *arg;
	const char *help;
	const VoidBlock block;
	const bool neg;
} Element;

typedef struct {
	const char *grouping;
	const int length;
	const Element *links;
} HelpLink;

#endif