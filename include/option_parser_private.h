#include "string_util.h"

// uses val s > 256 && < MAX_OPT_BLOCKS for long only options
#define LONG_OPT_START_VALUE 257
#define LONG_OPT_END_VALUE LONG_OPT_START_VALUE + 51

#define MAX_OPT_BLOCKS LONG_OPT_END_VALUE - 1 + 128

#define ASCII 128
#define VAILD_ASCII(ch) ch < ASCII && ch > 0 

#define TRUTH_VALUE(ch)  ((ch < ASCII) ? true : false)
#define TRUTH_ARG(ch,istrue, isfalse)  ((ch < ASCII) ? istrue : isfalse)
#define TRUTH_STATE(ch) (ch < ASCII)
#define TRUTH_STATE_l(ch) (ch < LONG_OPT_END_VALUE)

//LATER usee fprintf stderr 

const Element H_filetype[] ={

	{  
		.opt   = {.name =  "all", .val = 260, .has_arg = no_argument}, 
		.help  = "Display all files.",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->types = T_NONE;
		}
	},
	{  
		.opt   = {.name =  "audio", .val = 259, .has_arg = no_argument}, 
		.help  = "Displays audio as well.",
		.arg   = "", .neg = true,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if ( TRUTH_STATE_l(ch) ){
				ma->types |= T_AUDIO;
			}else{
				ma->types &= ~T_AUDIO;
			}
		}
	},
	{  
		.opt   = {.name =  "video", .val = 258, .has_arg = no_argument}, 
		.help  = "Displays videos as well.",
		.arg   = "", .neg = true,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if ( TRUTH_STATE_l(ch) ){
				ma->types |= T_VIDEO;
			}else{
				ma->types &= ~T_VIDEO;
			}
		}
	},	
};

const Element H_filepath[] ={
	
	{  
		.opt   = {.name =  "exclude", .val = 'e', .has_arg = required_argument}, 
		.help  = "Sub directories to exclude",
		.arg   = "dir", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			Excludes *e = &ma->excludes;
			if ( (e->length - e->length ) < 1 ){
				e->length  = e->length * 2 + 1;
				e->str_arr = realloc(e->str_arr,  e->length );
			}
			e->str_arr[e->index++] = strdup(arg);
		}
	},
	{
		.opt   = {.name =  "hash", .val = 'S', .has_arg = no_argument},
		.help  = "Uses shortcuts from the hash, on by default",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->use_hash = TRUTH_VALUE(ch);
		},
	},
	{  
		.opt   = {.name =  "hashlocation", .val = 'I', .has_arg = required_argument}, 
		.help  = "Filepath of the hash",
		.arg   = "file", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->hash_location = strdup(arg);
		}
	},
	{  
		.opt   = {.name =  "playlistpath", .val = 'P', .has_arg = required_argument}, 
		.help  = "Directory to write playlist in default .",
		.arg   = "dir", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->pl_dir = strdup(arg);
		}
	},
	{  
		.opt   = {.name =  "rootpath", .val = 'R', .has_arg = required_argument}, 
		.help  = "Directory to start searching from",
		.arg   = "dir", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->root_dir = strdup(arg);
		}
	},
	{  
		.opt   = {.name =  "subdirectories", .val = 's', .has_arg = no_argument}, 
		.help  = "Look in sub directories if true, default false",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->sub_dirs = TRUTH_VALUE(ch);
		}
	},
};

const Element H_playlist[] ={

	#define MAKE_PLAYLISTT(_name,_val,_ftype,_help){\
		.opt   = {.name =  _name, .val = _val, .has_arg = no_argument},\
		.help  = _help,\
		.arg   = "", .neg = true,\
		.block = ^(MediaArgs *ma, int ch, char *arg ) {\
			if (TRUTH_STATE(ch)){\
				ma->pl_output |= PL_PLAYLIST;\
				ma->pl_format |= _ftype;\
			}else{\
				ma->pl_format &= ~_ftype;\
				if (ma->pl_format == F_NONE){\
					ma->pl_output &= ~PL_PLAYLIST;\
				}\
			}\
		}\
	}
	MAKE_PLAYLISTT("m3u"   ,'3', F_M3U,   "Outputs file as a m3u playlist"),
	MAKE_PLAYLISTT("plist" ,'x', F_PLIST, "Outputs file as a plist"),
	MAKE_PLAYLISTT("pls"   ,'U', F_PLS,   "Outputs file as a pls playlist"),
	MAKE_PLAYLISTT("xspf"  ,'X', F_XSPF,  "Outputs file as a xspf playlist"),
	#undef MAKE_PLAYLISTT
	{  
		.opt   = {.name =  "filename", .val = 'n', .has_arg = required_argument}, 
		.help  = "The filename of the playlist",
		.arg   = "name", .neg = false, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->pl_name = strdup(arg);
		}
	},
	{  
		.opt   = {.name =  "shuffle", .val = 'y', .has_arg = no_argument}, 
		.help  = "shuffles the playlist",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->pl_shuffle = TRUTH_VALUE(ch);
		}
	}
	
};

const Element H_player[] ={
	
	{  
		.opt   = {.name =  "none", .val = 'M', .has_arg = no_argument}, 
		.help  = "does not play the files.",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->player = P_NONE;
		}
	},
	{  
		.opt   = {.name =  "mplayer", .val = 'm', .has_arg = no_argument}, 
		.help  = "plays the files using mplayer.",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->player = P_MPLAYER;
		}
	},
	{  
		.opt   = {.name =  "niceplayer", .val = 'N', .has_arg = no_argument}, 
		.help  = "plays the files using niceplayer.",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->player = P_NICEPLAYER;
		}
	},
	{  
		.opt   = {.name =  "vlc", .val = 'V', .has_arg = no_argument}, 
		.help  = "plays the files using vlc.",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->player = P_VLC;
		}
	},

};

const Element H_mplayer[] = { 
	
	{  
		.opt   = {.name =  "1610", .val = '0', .has_arg = no_argument}, 
		.help  = "Uses 16:10 aspect ratio",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push(&ma->prefix_args, "-aspect 16:10");
		}
	},
	{  
		.opt   = {.name =  "afloat", .val = 'a', .has_arg = no_argument}, 
		.help  = "Makes the window afloat",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->afloat = true;
		}
	},
	{  
		.opt   = {.name =  "fast", .val = 'd', .has_arg = no_argument}, 
		.help  = "Makes it fast",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push_m(&ma->prefix_args, 2, 
				"-lavdopts", "skipframe=nonref:skiploopfilter=all:fast=1" );
		}
	},
	{  
		.opt   = {.name =  "fs", .val = 'f', .has_arg = no_argument}, 
		.help  = "Plays the file(s) in fullscreen. ",
		.arg   = "", .neg = true,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push(&ma->prefix_args, TRUTH_ARG(ch,"-fs", "-nofs") );
			string_push(&ma->prefix_args, "-aspect 16:10" );
			
		}
 	},
	{  
		.opt   = {.name =  "loop0", .val = 'k', .has_arg = no_argument}, 
		.help  = "Adds -loop 0 -- meaning loops forever",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push(&ma->prefix_args, "-loop 0");
		}
	},
	{  
		.opt   = {.name =  "top", .val = 't', .has_arg = no_argument}, 
		.help  = "Adds profile t  - afloat and 360p.",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if (TRUTH_STATE(ch)){
				string_push_m(&ma->prefix_args, 2, "-profile t", "-nofs");
				ma->afloat = true;
			}else{
				string_push(&ma->prefix_args, "-xy 1");
				ma->afloat = false;
			}
		}
	},
	{  
		.opt   = {.name =  "mtop", .val = 'T', .has_arg = no_argument}, 
		.help  = "Adds profile T  - ontop and 360p.",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if (TRUTH_STATE(ch)){
				string_push(&ma->prefix_args, "-profile T");
			}else{
				string_push_m(&ma->prefix_args, 2,  "-noontop", "-xy 1" );
			}
		}
	},
	{  
		.opt   = {.name =  "framedrop", .val = 'j', .has_arg = no_argument}, 
		.help  = "Enable frame dropping",
		.arg   = "", .neg = true,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if (TRUTH_STATE(ch)){
				string_push(&ma->prefix_args, "-framedrop");
			}else{
				string_push(&ma->prefix_args, "-noframedrop");
			}
		}
	},
	{  
		.opt   = {.name =  "hframedrop", .val = 'J', .has_arg = no_argument}, 
		.help  = "Enable hard frame dropping",
		.arg   = "", .neg = true,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if (TRUTH_STATE(ch)){
				string_push(&ma->prefix_args, "-hardframedrop");
			}else{
				string_push(&ma->prefix_args, "-noframedrop");
			}
		}
	},
	{  
		.opt   = {.name =  "allspaces", .val = 'c', .has_arg = no_argument}, 
		.help  = "Brings up the afloat menu",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->all_spaces = TRUTH_ARG(ch, SPACES_MANUAL, SPACES_NONE);
		}
	},
	{  
		.opt   = {.name =  "autospaces", .val = 'C', .has_arg = no_argument}, 
		.help  = "Make the player appear on spaces",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->all_spaces = TRUTH_ARG(ch, SPACES_AUTO, SPACES_NONE);
		}
	},	
};

const Element H_output[] ={
	
	{  
		.opt   = {.name =  "out", .val = 'o', .has_arg = no_argument}, 
		.help  = "Outputs file list to stdout",
		.arg   = "", .neg = true,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if (TRUTH_STATE(ch)){
				ma->pl_output |= PL_STDOUT;
			}else{
				ma->pl_output &= ~PL_STDOUT;
			}
		}
	},
	{  
		.opt   = {.name =  "nooutput", .val = 'O', .has_arg = no_argument}, 
		.help  = "Does not write any output (including playlists)",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->pl_output = PL_NONE;
		}
	}
};

const Element H_History[]={
	
	{  
		.opt   = {.name =  "done", .val = 'D', .has_arg = no_argument}, 
		.help  = "Set the status to done ",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if (TRUTH_STATE(ch)){
				ma->status |=  S_UPDATED;
			}else{
				ma->status &= ~S_UPDATED;
			}
		},
	},
	{  
		.opt   = {.name =  "history", .val = '[', .has_arg = no_argument}, 
		.help  = "Adds the files to the history, "
		 	"which is stored in a sql database.",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->write_history =  TRUTH_VALUE(ch) ;
		},
	},
	{  
		.opt   = {.name =  "skip", .val = 'K', .has_arg = no_argument}, 
		.help  = "Set the status to skip ",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if (TRUTH_STATE(ch)){
				ma->status |=  S_SKIP;
			}else{
				ma->status &= ~S_SKIP;
			}
		},
	}
};

const Element H_other[] ={
	
	{  
		.opt   = {.name =  "last", .val = 'l', .has_arg = no_argument}, 
		.help  = "Choose the latest file of each series",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->newest_only = TRUTH_VALUE(ch);
		}
	},
	{  
		.opt   = {.name =  "sep", .val = 'w', .has_arg = required_argument}, 
		.help  = "Use {num} as the separator between episodes",
		.arg   = "num", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			int temp;
			int res = sscanf(arg, "%4d",&temp);
			if (res == -1){
				fprintf(stderr, "Invalid separator  %s\n", arg );
				exit(4);
			}
			ma->sep = temp;
		}
	},
	{  
		.opt   = {.name =  "nicerandom", .val = ':', .has_arg = no_argument}, 
		.help  = "Sets random in niceplayer",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->nice_random =  TRUTH_VALUE(ch) ;
		},
	},
	{  
		.opt   = {.name =  "nicereapeat", .val = 'Y', .has_arg = no_argument}, 
		.help  = "Sets repeat in niceplayer",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->nice_repeat =  TRUTH_VALUE(ch) ;
		},
	},
	{  
		.opt   = {.name =  "help", .val = 'h', .has_arg = optional_argument}, 
		.help  = "Displays the help.",
		.arg   = "part", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			print_help(arg);
			exit(0);
		}
	},
	{  
		.opt   = {.name =  "print_opt", .val = 'Z', .has_arg = no_argument}, 
		.help  = "Shows the opt struct",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			print_media_args(ma);
		}
	},
	{  
		.opt   = {.name =  "ax", .val = 261, .has_arg = no_argument}, 
		.help  = "Uses AX as root directory",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->root_dir = strdup("/Users/bilalh/Movies/.Movie/divx");
		}
	},
	{  
		.opt   = {.name =  "op", .val = 262, .has_arg = no_argument}, 
		.help  = "Uses OP as root directory",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->root_dir = strdup("/Users/bilalh/Movies/.Movie/OpeningP");
		}
	},
};

const Element H_mplayer_extra[] = { 
	
	{  
		.opt   = {.name =  "profile", .val = 'p', .has_arg = required_argument}, 
		.help  = "Adds profile ",
		.arg   = "name", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if (*arg == '-'){
				puts("opt: option `--profile' requires an argument");
				exit(1);
			}
			string_push_m(&ma->prefix_args, 2,"-profile", arg);
		}
	},
	{  
		.opt   = {.name =  "width", .val = 'W', .has_arg = required_argument}, 
		.help  = "Set the width",
		.arg   = "width", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			int a, res; char temp[1];
			res = sscanf(arg, "%8i%1s",&a,temp);
			if (res != 1){
				printf("Invalid Width \n");
				exit(1);
			}else{
				string_push_m(&ma->prefix_args, 2,"-xy", arg);
			}
		}
	},
	{  
		.opt   = {.name =  "height", .val = 'H', .has_arg = required_argument}, 
		.help  = "Set the height using 16:9",
		.arg   = "height", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			int y, res; char temp[20];
			double x;
			res = sscanf(arg, "%8i%1s",&y,temp);
			if (res != 1){
				exit(1);
			}else{
				x = y * 16.0/9.0;
				sprintf(temp, "%lf", x);
				string_push_m(&ma->prefix_args, 2,"-xy", temp);
			}
		}
	},
	{  
		.opt   = {.name =  "loop", .val = 'L', .has_arg = required_argument}, 
		.help  = "Adds -loop -- meaning loops forever",
		.arg   = "num", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			int res, x; char temp[1];
			res = sscanf(arg, "%8i%1s",&x,temp);
			if (res != 1){
				exit(1);
			}else{
				string_push_m(&ma->prefix_args, 2,  "-loop", arg);
			}
		}
	},
	{  
		.opt   = {.name =  "rnd", .val = 'r', .has_arg = no_argument}, 
		.help  = "Uses mplayer random unction ",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if (TRUTH_STATE(ch)){
				string_push(&ma->prefix_args, "-shuffle");
			}else{
				string_push(&ma->prefix_args, "-noshuffle");
			}
		},
	},
	{  
		.opt   = {.name =  "prefix", .val = 'E', .has_arg = required_argument}, 
		.help  = "Set mplayer prefix options, can be used multiple times",
		.arg   = "args", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if (arg != NULL && *arg != '\0'){
				string_push(&ma->prefix_args,arg);
			}
		}
	},
	{  
		.opt   = {.name =  "postfix", .val = 'F', .has_arg = required_argument}, 
		.help  = "Set mplayer postfix options, can be used multiple times",
		.arg   = "args", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if (arg != NULL && *arg != '\0'){
				string_push(&ma->postfix_args,arg);
			}
		}
	}
};

const Element H_mplayer_aspect[] = {
	{  
		.opt   = {.name =  "aspect", .val = 'A', .has_arg = required_argument}, 
		.help  = "Sets the aspect ratio",
		.arg   = "W:H", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			int a, b, res;
			res = sscanf(arg, "%8i:%8i",&a,&b);
			// printf("    Abeg aspect %s\n", arg);
			if (res != 2){
				printf("Invalid aspect ratio\n");
				exit(1);
			}else{
				// printf("    Aend aspect %s\n",arg );
				// string_push_m(&ma->prefix_args, 2,"-aspect", arg);
				string_push(&ma->prefix_args,"-aspect");
				string_push(&ma->prefix_args, arg);
				// string_push(&ma->prefix_args,arg);
			}
		}
	},
	{  
		.opt   = {.name =  "169", .val = '9', .has_arg = no_argument}, 
		.help  = "Uses 16:9 aspect ratio",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push(&ma->prefix_args, "-aspect 16:9");
		}
	},
	{  
		.opt   = {.name =  "43", .val = '4', .has_arg = no_argument}, 
		.help  = "Uses 4:3 aspect ratio",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push(&ma->prefix_args, "-aspect 4:3");
		}
	},
	{  
		.opt   = {.name =  "original", .val = 'i', .has_arg = no_argument}, 
		.help  = "Uses original size",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push(&ma->prefix_args, "-xy 1");
		}
	},
};

const Element H_mplayer_geom[] = {
	{  
		.opt   = {.name =  "geometry", .val = 'G', .has_arg = required_argument}, 
		.help  = "Palaces the player at (x,y)",
		.arg   = "x:y", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			//LATER stricter geometry?
			if (match(arg,"^([0-9][0-9]?|100)%?:([0-9][0-9]?|100)%?$") == 0){
				printf("Invalid geometry \n");
				exit(1);
			}
			string_push_m(&ma->prefix_args, 2,"-geometry", arg);
		}
	},
	
	#define M_GEO(_name,_val,_geo,_help)                                \
	{                                                                   \
		.opt   = {.name =  _name, .val = _val, .has_arg = no_argument}, \
		.help  = _help,                                                 \
		.arg   = "", .neg = false,                                      \
		.block = ^(MediaArgs *ma, int ch, char *arg ) {                 \
		string_push(&ma->prefix_args, _geo);                            \
	}                                                                   \
	}
	M_GEO("tl", '1', "-geometry 0%:0%"   ,"Places the player at the top left"),
	M_GEO("tr", '=', "-geometry 100%:0%" ,"Places the player at the top right"),
	M_GEO("bl", 'z', "-geometry 0%:93%"  ,"Places the player at the bottom right"),
	M_GEO("br", '/', "-geometry 100%:93%","Places the player at the bottom left"),
	M_GEO("lc", '5', "-geometry 0%:50%"  ,"Places the player at the left centre"),
	M_GEO("rc", '8', "-geometry 100%:50%","Places the player at the right centre"),
	M_GEO("tc", '7', "-geometry 50%:0%"  ,"Places the player at the top centre"),
	M_GEO("bc", '6', "-geometry 50%:93%" ,"Places the player at the bottom centre")
	#undef M_GEO
};

const HelpLink HELP_LINK[] = {
	{ "Filepath",          sizeof(H_filepath)       / sizeof(Element), &H_filepath[0]       },
	{ "Mplayer",           sizeof(H_mplayer)        / sizeof(Element), &H_mplayer[0]        },
	{ "Playlist",          sizeof(H_playlist)       / sizeof(Element), &H_playlist[0]       },
	{ "Player",            sizeof(H_player)         / sizeof(Element), &H_player[0]         },
	{ "Output",            sizeof(H_output)         / sizeof(Element), &H_output[0]         },
	{ "History",           sizeof(H_History)        / sizeof(Element), &H_History[0]        },
	{ "Filetype",          sizeof(H_filetype)       / sizeof(Element), &H_filetype[0]       },
	{ "Other",             sizeof(H_other)          / sizeof(Element), &H_other[0]          },
	{ "Mplayer extra",     sizeof(H_mplayer_extra)  / sizeof(Element), &H_mplayer_extra[0]  },
	{ "Mplayer aspect",    sizeof(H_mplayer_aspect) / sizeof(Element), &H_mplayer_aspect[0] },
	{ "Mplayer geometry",  sizeof(H_mplayer_geom)   / sizeof(Element), &H_mplayer_geom[0]   },
};


#define HELP_L_LEN sizeof(HELP_LINK) / sizeof(HelpLink)
