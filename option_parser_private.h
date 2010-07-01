
const Element H_filetype[] ={
	      
};        
const Element H_filepath[] ={
	      
};        

const Element H_playlist[] ={
	
};
const Element H_player[] ={
	
	{{"mplayer", no_argument, 0, 'm'}, "", "plays file(s) using mplayer",
		^ (MediaArgs *ma){
			ma->player = P_MPLAYER;
		}
	},
	
	{{"niceplayer", no_argument, 0, 'N'}, "", "plays file(s) using niceplayer",
		^ (MediaArgs *ma){
			ma->player = P_NICEPLAYER;
		}
	},
	
	{{"vlc", no_argument, 0, 'V'}, "", "plays file(s) using vlc",
		^ (MediaArgs *ma){
			ma->player = P_VLC;
		}
	}
	
};

const Element H_mplayer[] = { 
	
	{{"fs", no_argument, 0, 'f'}, "", "plays file(s) in fullscreen",
		^ (MediaArgs *ma ) {
			string_push(&ma->prefix_args,"-fs");
		}
	},
	
	{{"top", no_argument, 0, 't'}, "", "Add profile t, afloat and 360p",
		^ (MediaArgs *ma){
			string_push(&ma->prefix_args, "-profile t");
			ma->afloat = true;
		}
	}
	
};

const Element H_output[] ={
		
};

const Element H_other[] ={
	
	{{"help", no_argument, 0, 'h'}, "", "Shows the help",
		^ (MediaArgs *ma){
			print_help();
		}
	},
	
	{{"print_opt", no_argument, 0, 'Z'}, "", "Shows the opt struct"	,
		^ (MediaArgs *ma){
			print_media_args(ma);
		}
	}
	
};