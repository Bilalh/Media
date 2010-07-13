#include "string_util.h"
// uses val s > 256 & < MAX_OPT_BLOCKS for long only options
#define LONG_OPT_START_VALUE 257
#define MAX_OPT_BLOCKS LONG_OPT_START_VALUE + 50 + 128

#define ASCII 128
#define TRUTH_VALUE(ch)  ((ch < ASCII) ? true : false)
#define TRUTH_ARG(ch,istrue, isfalse)  ((ch < ASCII) ? istrue : isfalse)
#define TRUTH_STATE(ch) (ch < ASCII)
#define VAILD_ASCII(ch) ch < ASCII && ch > 0 


const Element H_filetype[] ={
	
	
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
	}
	
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
	MAKE_PLAYLISTT("m3u",'3',F_M3U,"Outputs file as a m3u playlist"),
	MAKE_PLAYLISTT("plist",'x',F_PLIST,"Outputs file as a plist"),
	MAKE_PLAYLISTT("pls",'P',F_PLS, "Outputs file as a pls playlist"),
	MAKE_PLAYLISTT("xspf",'X',F_XSPF,"Outputs file as a xspf playlist"),
	#undef MAKE_PLAYLISTT
	
	{  
		.opt   = {.name =  "shuffle", .val = 'Y', .has_arg = no_argument}, 
		.help  = "shuffles the playlist",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->pl_shuffle = TRUTH_VALUE(ch);
		}
	},
	
};

const Element H_player[] ={
	
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
	{  
		.opt   = {.name =  "none", .val = 256, .has_arg = no_argument}, 
		.help  = "does not play the files.",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->player = P_NONE;
		}
	}
};

const Element H_mplayer[] = { 
	
	{  
		.opt   = {.name =  "fs", .val = 'f', .has_arg = no_argument}, 
		.help  = "Plays the file(s) in fullscreen.",
		.arg   = "", .neg = true,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push(&ma->prefix_args, TRUTH_ARG(ch,"-fs", "-nofs") );
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
		.opt   = {.name =  "aspect", .val = 'A', .has_arg = required_argument}, 
		.help  = "Sets the aspect ratio",
		.arg   = "W:H", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			int a, b, res;
			res = sscanf(arg, "%8i:%8i",&a,&b);
			if (res != 2){
				printf("Invalid aspect ratio\n");
				exit(1);
			}else{
				string_push_m(&ma->prefix_args, 2,"-aspect", arg);
			}
		}
	},
	{  
		.opt   = {.name =  "fast", .val = 'F', .has_arg = no_argument}, 
		.help  = "Plays the with op",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push_m(&ma->prefix_args, 2, 
				"-lavdopts", "skipframe=nonref:skiploopfilter=all:fast=1" );
		}
	},
	{  
		.opt   = {.name =  "loop0", .val = 'k', .has_arg = no_argument}, 
		.help  = "Adds -loop 0 -- meaning loops forever",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push(&ma->prefix_args, "loop 0");
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
		.help  = "Does not write Any output",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->pl_output = PL_NONE;
		}
	}
};

const Element H_other[] ={
	
	{  
		.opt   = {.name =  "last", .val = 'l', .has_arg = no_argument}, 
		.help  = "Choose the latest file of each series",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->newest_only = TRUTH_VALUE(ch);
		}
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
		.opt   = {.name =  "done", .val = 'D', .has_arg = no_argument}, 
		.help  = "Set the status to done ",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			// FIXME add ma->done
			// ma->done =  TRUTH_VALUE(ch) ;
		},
	},
	{  
		.opt   = {.name =  "shortcuts", .val = 'S', .has_arg = no_argument}, 
		.help  = "Uses shortcuts from the hash, on by default",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			// FIXME add ma->shortcuts
			// ma->shortcuts =  TRUTH_VALUE(ch) ;
		},
	},
	{  
		.opt   = {.name =  "nicerandom", .val = 'M', .has_arg = no_argument}, 
		.help  = "Sets random in niceplayer",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			// FIXME add ma->nicerandom
			// ma->nicerandom =  TRUTH_VALUE(ch) ;
		},
	},
	{  
		.opt   = {.name =  "nicereapeat", .val = 'y', .has_arg = no_argument}, 
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
		}
	},
	{  
		.opt   = {.name =  "print_opt", .val = 'Z', .has_arg = no_argument}, 
		.help  = "Shows the opt struct",
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			print_media_args(ma);
		}
	}
	
};

const Element H_mplayer_extra[] = { 
	
	{  
		.opt   = {.name =  "profile", .val = 257, .has_arg = required_argument}, 
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
				string_push_m(&ma->prefix_args, 2,  "loop", arg);
			}
		}
	},
	{  
		.opt   = {.name =  "rnd", .val = 'R', .has_arg = no_argument}, 
		.help  = "Uses mplayer random unction ",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			if (TRUTH_STATE(ch)){
				string_push(&ma->prefix_args, "shuffle");
			}else{
				string_push(&ma->prefix_args, "noshuffle");
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

const Element H_mplayer_geom[] = { 
	{  
		.opt   = {.name =  "geometry", .val = 'G', .has_arg = required_argument}, 
		.help  = "Palaces the player at (x,y)",
		.arg   = "x:y", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			// TODO stricter geometry?
			if (match(arg,"^([0-9][0-9]?|100)%?:([0-9][0-9]?|100)%?$") == 0){
				printf("Invalid geometry \n");
				exit(1);
			}
			string_push_m(&ma->prefix_args, 2,"-geometry", arg);
		}
	},
	
	#define M_GEO(_name,_val,_geo,_help) \
	{  \
		.opt   = {.name =  _name, .val = _val, .has_arg = no_argument}, \
		.help  = _help,\
		.arg   = "", .neg = false,\
		.block = ^(MediaArgs *ma, int ch, char *arg ) {\
			string_push(&ma->prefix_args, _geo);\
		}\
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
	{ "Filepath",          sizeof(H_filepath)      / sizeof(Element), &H_filepath[0]      },
	{ "Mplayer",           sizeof(H_mplayer)       / sizeof(Element), &H_mplayer[0]       },
	{ "Playlist",          sizeof(H_playlist)      / sizeof(Element), &H_playlist[0]      },
	{ "Player",            sizeof(H_player)        / sizeof(Element), &H_player[0]        },
	{ "Output",            sizeof(H_output)        / sizeof(Element), &H_output[0]        },
	{ "Filetype",          sizeof(H_filetype)      / sizeof(Element), &H_filetype[0]      },
	{ "Other",             sizeof(H_other)         / sizeof(Element), &H_other[0]         },
	{ "Mplayer extra",     sizeof(H_mplayer_extra) / sizeof(Element), &H_mplayer_extra[0] },
	{ "Mplayer geometry",  sizeof(H_mplayer_geom)  / sizeof(Element), &H_mplayer_geom[0]  }
};


#define HELP_L_LEN sizeof(HELP_LINK) / sizeof(HelpLink)