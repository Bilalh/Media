// uses val s > 256 & < MAX_OPT_BLOCKS for long only options
#define LONG_OPT_START_VALUE 257
#define MAX_OPT_BLOCKS LONG_OPT_START_VALUE + 50 + 128

#define TRUTH_VALUE(ch)  ((ch < 128) ? true : false)
#define TRUTH_ARG(ch,istrue, isfalse)  ((ch < 128) ? istrue : isfalse)
#define ASCII 128
#define VAILD_ASCII(ch) ch < ASCII && ch > 0 


const Element H_filetype[] ={
	      
};        
const Element H_filepath[] ={
	      
};        

const Element H_playlist[] ={
	
};
const Element H_player[] ={
	
	{  
		.opt   = {.name =  "mplayer", .val = 'm', .has_arg = no_argument}, 
		.help  = "plays the files using mplayer.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->player = P_MPLAYER;
		}
	},
	{  
		.opt   = {.name =  "niceplayer", .val = 'N', .has_arg = no_argument}, 
		.help  = "plays the files using niceplayer.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->player = P_NICEPLAYER;
		}
	},
	{  
		.opt   = {.name =  "vlc", .val = 'V', .has_arg = no_argument}, 
		.help  = "plays the files using vlc.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->player = P_VLC;
		}
	},
	{  
		.opt   = {.name =  "none", .val = 256, .has_arg = no_argument}, 
		.help  = "does not play the files.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->player = P_NONE;
		}
	}
};



const Element H_mplayer[] = { 
	
	{  
		.opt   = {.name =  "fs", .val = 'f', .has_arg = no_argument, }, 
		.help  = "Plays file(s) in fullscreen.",
		.arg   = "", .neg = true,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push(&ma->prefix_args, TRUTH_ARG(ch,"-fs", "-nofs") );
		}
	},
	{  
		.opt   = {.name =  "afloat", .val = 'a', .has_arg = no_argument}, 
		.help  = "Makes the window afloat",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			ma->afloat = true;
		}
	},
	{  
		.opt   = {.name =  "top", .val = 't', .has_arg = no_argument}, 
		.help  = "Adds profile t  - afloat, all spaces and 360p.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push(&ma->prefix_args, "-profile t");
			ma->afloat = true;
		}
	},
	{  
		.opt   = {.name =  "mtop", .val = 'T', .has_arg = no_argument}, 
		.help  = "Adds profile T  - ontop and 360p.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push(&ma->prefix_args, "-profile t");
			ma->afloat = true;
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
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			string_push_m(&ma->prefix_args, 2, "-lavdopts", "skipframe=nonref:skiploopfilter=all:fast=1" );
		}
	},
};

const Element H_output[] ={
		
};

const Element H_other[] ={
	
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
		.opt   = {.name =  "help", .val = 'h', .has_arg = no_argument}, 
		.help  = "Displays the help.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch, char *arg ) {
			print_help();
		}
	},
	{  
		.opt   = {.name =  "print_opt", .val = 'Z', .has_arg = no_argument}, 
		.help  = "Shows the opt struct",
		.arg   = "", .neg = false,
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
				printf("Invalid Width \n");
				exit(1);
			}else{
				x = y * 16.0/9.0;
				sprintf(temp, "%lf", x);
				string_push_m(&ma->prefix_args, 2,"-xy", temp);
			}
		}
	},
};


const HelpLink HELP_LINK[] = {
	{ "Filetype",         sizeof(H_filetype)      / sizeof(Element), &H_filetype[0]      },
	{ "Filepath",         sizeof(H_filepath)      / sizeof(Element), &H_filepath[0]      },
	{ "Mplayer",          sizeof(H_mplayer)       / sizeof(Element), &H_mplayer[0]       },
	{ "Playlist",         sizeof(H_playlist)      / sizeof(Element), &H_playlist[0]      },
	{ "Player",           sizeof(H_player)        / sizeof(Element), &H_player[0]        },
	{ "Output",           sizeof(H_output)        / sizeof(Element), &H_output[0]        },
	{ "Other",            sizeof(H_other)         / sizeof(Element), &H_other[0]         },
	{ "Mplayer extra",    sizeof(H_mplayer_extra) / sizeof(Element), &H_mplayer_extra[0] }
};


#define HELP_L_LEN sizeof(HELP_LINK) / sizeof(HelpLink)