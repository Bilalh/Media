#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "option_parser.h"

MediaArgs *new_media_args() {

	MediaArgs *ma = malloc(sizeof(MediaArgs));

	// Selection
	ma->exclude     = false;
	ma->excludes    = NULL;
	ma->newest_only = false;
	ma->sub_dirs    = false;
	ma->types       = T_VIDEO;

	// Playlist
	ma->pl_dir    = ma->pl_name = NULL;
	ma->pl_format = PL_STDOUT;
	ma->pl_output = PL_NONE;
	ma->pl_rand   = false;

	// Prefs
	ma->hash_location = NULL;
	ma->use_hash      = false;
	ma->status        = S_NONE;

	// Player
	ma->player = P_NONE;
	ma->afloat = false;
	ma->prefix_args = ma->postfix_args = NULL;
	ma->nice_repeat=false;
	

	return ma;
}