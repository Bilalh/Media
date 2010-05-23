#include <stdbool.h>

void media(char *path,char **args, int length);

/**
 *  Plays the file using mplayer.
**/
void mplayer(char **filenames, int total_length, char *prefix_args, char *postfix_args, char *filepath);

/**
 * Match string against the extended regular expression in
 * pattern, treating errors as no match.
 *
 * non zero on match, 0 on any error
**/
int match(const char *string, char *pattern);


typedef enum {
	F_M3U, F_PLIST, F_PLS, F_XSPF
} Pformat;

typedef enum {
	PL_NONE = 0x0, PL_STDOUT = 0x1, PL_PLAYLIST = 0x2, PL_BOTH = 0x3
} Poutput;

typedef enum {
	P_MPLAYER, P_NICEPLAYER,  P_VLC
} Player;

typedef enum {
	T_NONE = 0x0, T_VIDEO = 0x1, T_AUDIO = 0x2, T_BOTH = 0x3
} Types;

typedef enum {
	S_DONE
} Status;

typedef struct {
	
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
	
	// Pefs
	char   *hash_location;
	bool    use_hash;
	Status  status;
	Player  player;
	bool    write_history;
	
	// Players
	bool   afloat;
	char   *prefix_args;
	char   *postfix_args;
	
	//Nice Player
	bool repeat;
	
} MediaArgs;
