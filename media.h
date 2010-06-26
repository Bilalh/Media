#ifndef MEDIAHEAD
#define MEDIAHEAD
#include <stdbool.h>

typedef struct _MediaArgs MediaArgs;

void media(char *path, char **args,int argc, MediaArgs *opts);

void mplayer(char **filenames, int total_length, char *prefix_args, char *postfix_args, char *filepath);

void niceplayer(char *playlist);

void vlc(char **filenames, int total_length, char *prefix_args, char *postfix_args, char *filepath);

// enums
typedef enum {
	F_M3U = 0x1 , F_PLIST = 0x2 , F_PLS = 0x4 , F_XSPF = 0x8
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
	char *str;
	int length;
	int index;
} String;

struct  _MediaArgs{
	
	// Selection  
	bool  exclude;
	char *excludes;
	bool  newest_only;
	bool  sub_dirs;
	Types types;
	
	// Playlist (pl)
	char    *pl_dir;
	char    *pl_name;
	Pformat  pl_format;
	Poutput  pl_output;
	bool     pl_rand;
	
	// Prefs
	char   *hash_location;
	bool    use_hash;
	Status  status;
	bool    write_history;
	
	// Players
	Player  player;
	bool    afloat;
	String  prefix_args;
	String  postfix_args;
		
	//Nice Player
	NiceRepeat nice_repeat;
	
};

#endif
