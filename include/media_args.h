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
	S_NONE, S_SKIP, S_UPDATED
} Status;

typedef enum{
	N_NO_REPEAT, N_REPEAT_ONE, N_REPEAT_ALL
} NiceRepeat;



typedef struct {
	bool exclude;
	char **str_arr;
	int  length;
	int  index;
} Excludes;

typedef struct {
	
	// Selection  
	Excludes excludes;
	bool     newest_only;
	bool     sub_dirs;
	Types    types; //TODO types
	
	char     *root_dir;
	
	// Playlist (pl)
	char    *pl_dir;
	char    *pl_name;
	Pformat  pl_format;
	Poutput  pl_output;
	bool     pl_shuffle;
	
	// Prefs
	char   *hash_location;
	bool    use_hash;
	Status  status; //TODO status
	bool    write_history;
	bool	updated;
	bool    shortcuts;
	
	
	// Players
	Player   player;
	bool     afloat;
	String   prefix_args;
	String   postfix_args;
		
	//Nice Player
	NiceRepeat nice_repeat;
	bool       nice_random;
} MediaArgs;

#endif
