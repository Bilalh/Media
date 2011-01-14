#ifndef PREFERENCES_HEADER_MEDIA_
#define PREFERENCES_HEADER_MEDIA_

// Binaries paths
#define PREFS_VLC_BINARY        "\"/Applications/VLC.app/Contents/MacOS/VLC\""
#define PREFS_MPLAYER_BINARY    "\"/Users/bilalh/Library/Application Support/MPlayer OSX Extended/Binaries/mplayer-pigoz.mpBinaries/Contents/MacOS/mplayer\""


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
#define PREFS_VIDEO_REGEX       "(.*)?\\.(mkv|mp4|mov|avi|ogm|divx|rm|rmvb|flv|part|wmv)$"
#define PREFS_AUDIO_REGEX       "(.*)?\\.(mp3|m4a|flac|ogg|m4b|aiff|ac3|aac|wav|wmv|ape)$"
#define PREFS_AUDIO_VIDEO_REGEX "(.*)?\\.(mkv|mp4|mp3|m4a|mov|avi|flac|ogm|ogg|aiff|divx|rm|rmvb|flv|part|wmv|ac3|aac|wav|wmv|ape)$"



#endif