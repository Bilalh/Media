#ifndef PREFERENCES_HEADER_MEDIA_
#define PREFERENCES_HEADER_MEDIA_

// Binaries paths
#define PREFS_MPLAYER_BINARY    "\"/Users/bilalh/Library/Application Support/MPlayer OSX Extended/Binaries/mplayer-git.mpBinaries/Contents/MacOS/mplayer\""
#define PREFS_MPLAYER_BINARY_UN_Q "/Users/bilalh/bin/mplayer_osx/mplayer"

#define PREFS_MPLAYER_APP_NAME  "\"mplayer-git.mpBinaries\""
#define PREFS_VLC_BINARY        "\"/Applications/VLC.app/Contents/MacOS/VLC\""

// Nmae of Mplayer GUI app 
#define PREFS_MPLAYER_GUI_APP_NAME "\"MPlayer OSX Extended\""

// The label colour to set watched files to 
// none, orange, red, yellow, blue, purple, green or grey
#define PREFS_WATCHED_COLOUR "orange"

// The Colour to highlight episodes numbers 
// colours:
// BLACK RED GREEN YELLOW BLUE MAGENTA CYAN WHITE BOLD 
// BOLD_BLACK BOLD_RED BOLD_GREEN BOLD_YELLOW BOLD_BLUE 
// BOLD_MAGENTA BOLD_CYAN BOLD_WHITE 
#define PREFS_EP_COLOUR BLUE

// Regexes for matching Video and audio files
#define PREFS_VIDEO_REGEX       ".*\\.(mkv|mp4|avi|mov|part|ogm|divx|rm|rmvb|flv|wmv|webm)$"
#define PREFS_AUDIO_REGEX       ".*.(mp3|m4a|flac|ac3|ogg|aiff|m4b|aac|wav|wmv|ape|mka)$"
#define PREFS_AUDIO_VIDEO_REGEX ".*.(mkv|mp4|mp3|m4a|mov|avi|flac|ogm|ogg|aiff|divx|rm|rmvb|flv|part|wmv|ac3|aac|wav|wmv|ape|mka)$"

// comment to disable.  Replaces all (the char (default ^)) with | to save '' the args 
#define PREFS_SECOND_BAR '@'

// comment to get rid of COLOUR
#define PREFS_COLOUR


#define DATABASE "/Users/bilalh/Library/Application Support/Media/Media.db"

// paths for --ax and --op,  mainly for testing
#define PREFS_AX "/Users/bilalh/Movies/.Movie/divx"
#define PREFS_OP "/Users/bilalh/Movies/.Movie/OpeningP"

// lengths for tiles in ongoing
#define PREFS_MIN_TITLE_LENGTH 42
#define PREFS_MAX_TITLE_LENGTH 55

#endif