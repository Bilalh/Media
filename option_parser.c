#include <stdio.h>    
#include <stdlib.h>   
#include <getopt.h>   

#include "option_parser.h"

MediaArgs *new_media_args(){
	
	MediaArgs *args = malloc(sizeof(MediaArgs)); 
	return args;
	
}