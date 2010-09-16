#ifndef MEDIA_ARGS_HEAD
#define MEDIA_ARGS_HEAD
#include "string_buffer.h"


typedef enum {
	F_NONE = 0x0, F_M3U = 0x1 , F_PLIST = 0x2 , F_PLS = 0x4 , F_XSPF = 0x8
} Pformat;

typedef enum {
	PL_NONE = 0x0, PL_STDOUT = 0x1, PL_PLAYLIST = 0x2, PL_BOTH = 0x3
} Poutput;

typedef enum {
	P_NONE, P_MPLAYER, P_NICEPLAYER,  P_VLC
} Player;

typedef enum {
	T_NONE = 0x0, T_VIDEO = 0x1, T_AUDIO = 0x2, T_BOTH = 0x3
} Types;

typedef enum {
	S_NONE, S_SKIP = 0x1, S_UPDATED = 0x2, S_SKIP_UPDATED = 0x3
} Status;

typedef enum{
	N_NO_REPEAT, N_REPEAT_ONE, N_REPEAT_ALL
} NiceRepeat;

typedef enum{
	SPACES_NONE, SPACES_MANUAL, SPACES_AUTO
} Spaces;

typedef struct {
	bool exclude;
	char **str_arr;
	int  length;
	int  index;
} Excludes;

typedef struct {
	
	// Selection  
	Excludes excludes;
	bool     newest_only;  // DONE
	bool     sub_dirs;
	Types    types;        // DONE
	
	char     *root_dir;    // DONE
	
	// Playlist (pl)
	char    *pl_dir;       // DONE
	char    *pl_name;      // DONE
	Pformat  pl_format;    // nDONE - use full paths
	Poutput  pl_output;    // DONE
	bool     pl_shuffle;   // DONE
	
	// Prefs
	char   *hash_location; // DONE
	bool    use_hash;      // DONE
	Status  status;        // DONE
	bool    write_history; // DONE
	int     sep;           // DONE
	
	// Players
	Player   player;       // DONE
	bool     afloat;       // DONE
	Spaces   all_spaces;
	String   prefix_args;  // DONE
	String   postfix_args; // DONE
		
	//Nice Player
	NiceRepeat nice_repeat;
	bool       nice_random;
} MediaArgs;

#endif
