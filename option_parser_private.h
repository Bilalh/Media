// uses val s > 256 & < MAX_OPT_BLOCKS for long only options
#define MAX_OPT_BLOCKS 300

#define TRUTH_VALUE(ch)  ((ch < 128) ? true : false)
#define TRUTH_ARG(ch,istrue, isfalse)  ((ch < 128) ? istrue : isfalse)


const Element H_filetype[] ={
	      
};        
const Element H_filepath[] ={
	      
};        

const Element H_playlist[] ={
	
};
const Element H_player[] ={
	
	{  
		.opt   = {.name =  "mplayer", .val = 'm', .has_arg = no_argument, .flag = 0}, 
		.help  = "plays the files using mplayer.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch  ) {
			ma->player = P_MPLAYER;
		}
	},
	{  
		.opt   = {.name =  "niceplayer", .val = 'N', .has_arg = no_argument, .flag = 0}, 
		.help  = "plays the files using niceplayer.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch  ) {
			ma->player = P_NICEPLAYER;
		}
	},
	{  
		.opt   = {.name =  "vlc", .val = 'V', .has_arg = no_argument, .flag = 0}, 
		.help  = "plays the files using vlc.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch  ) {
			ma->player = P_VLC;
		}
	},
	{  
		.opt   = {.name =  "none", .val = 257, .has_arg = no_argument, .flag = 0}, 
		.help  = "does not play the files.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch  ) {
			ma->player = P_NONE;
		}
	}
};

const Element H_mplayer[] = { 
	
	{  
		.opt   = {.name =  "fs", .val = 'f', .has_arg = no_argument, .flag = 0, }, 
		.help  = "plays file(s) in fullscreen.",
		.arg   = "", .neg = true,
		.block = ^(MediaArgs *ma, int ch  ) {
			string_push(&ma->prefix_args, TRUTH_ARG(ch,"-fs", "-nofs") );
		}
	},
	{  
		.opt   = {.name =  "top", .val = 't', .has_arg = no_argument, .flag = 0}, 
		.help  = "Adds profile t  - afloat, all spaces and 360p.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch  ) {
			string_push(&ma->prefix_args, "-profile t");
			ma->afloat = true;
		}
	},
	{  
		.opt   = {.name =  "mtop", .val = 'T', .has_arg = no_argument, .flag = 0}, 
		.help  = "Adds profile T  - ontop and 360p.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch  ) {
			string_push(&ma->prefix_args, "-profile t");
			ma->afloat = true;
		}
	}
	
};

const Element H_output[] ={
		
};

const Element H_other[] ={
	
	{  
		.opt   = {.name =  "history", .val = '[', .has_arg = no_argument, .flag = 0}, 
		.help  = "Adds the files to the history, "
		 	"which is stored in a sql database.",
		.arg   = "", .neg = true, 
		.block = ^(MediaArgs *ma, int ch  ) {
			ma->write_history =  TRUTH_VALUE(ch) ;
		},
	},
	{  
		.opt   = {.name =  "help", .val = 'h', .has_arg = no_argument, .flag = 0}, 
		.help  = "Displays the help.",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch  ) {
			print_help();
		}
	},
	{  
		.opt   = {.name =  "print_opt", .val = 'Z', .has_arg = no_argument, .flag = 0}, 
		.help  = "Shows the opt struct",
		.arg   = "", .neg = false,
		.block = ^(MediaArgs *ma, int ch  ) {
			print_media_args(ma);
		}
	}
};