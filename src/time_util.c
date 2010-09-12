//Includes
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <pcre.h>

#include "../include/string_util.h"
#include "../include/time_util.h"
#include "../include/time_regex.h"

struct tm* currentTime() {
	time_t tt;
	time(&tt);
	// struct tm *timeinfo = gmtime(&tt);
	struct tm  *timeinfo  = malloc(sizeof(struct tm));
	gmtime_r(&tt, timeinfo);
	return timeinfo;
}

// calulates the near specifed 'other_day' from 'day'
int day_diff(int day, int other_day) {
	int res = 0;
	if (day != other_day ) {
		for(res = 1; res <= 3; ++res) {
			if ((day + res) % 7 ==  other_day) {
				break;
			}
			if((other_day + res) % 7 ==  day ) {
				res = -res;
				break;
			}
		}
	}
	return res;
}

// calulates the last specifed 'other_day' from 'day'
int day_last(int day, int other_day) {
	if      (day == other_day) return -7;
	else if (day  > other_day) return -(day - other_day);
	else                       return -(day + 7 - other_day);
}

// calulates the next specifed 'other_day' from 'day'
int day_future(int day, int other_day) {
	if      (day == other_day) return +7;
	else if (day  < other_day) return (other_day - day);
	else                       return (other_day + 7 - day);
}

struct tm *parse_time(char **str, int length) {
	
	struct tm* tm = currentTime();
    
	int total =  1, str_index = 0, index =0;
	for(int i = 0; i < length; ++i){
		total += strlen(str[i]) +1 ; //for space
	}
	
	// strl contains the whole string
	// strarr is an array of pointer to the parts of the string
	char strl[total], *strarr[length];
	for(int i = 0; i < length; ++i){
		strarr[i] = &strl[str_index];
		strncpy(&strl[str_index], str[i], strlen(str[i]));
		str_index += strlen(str[i]);
		strl[str_index++] = ' ';
	}
	if (str_index > 0) --str_index;
	strl[str_index] = '\0';
	
	#define REGEX_ERR \
		printf("%s\n", "error in pcre_compile");\
		return tm;

	MAKE_REGEX(at,   "^at ([0-1]?[0-9]|2[0-3]):([0-5]?[0-9])",REGEX_ERR);
	MAKE_REGEX(date, "^\\d{4}-\\d{2}-\\d{2}",REGEX_ERR);
	MAKE_REGEX(ago_after, "^(\\d+ (min(ute)?|hour|day)s? )+(ago|after)",REGEX_ERR);
	MAKE_REGEX(date_time, "^\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}",REGEX_ERR);
	MAKE_REGEX(week_day, "^(last|near|next) [a-zA-Z]{2}[deinrtu]([a-zA-Z]{3,6})?",REGEX_ERR);
	MAKE_REGEX(n_month  , 
			"^\\d{1,2}(th|st|nd|rd) [a-zA-Z]{2}[bcglnprtvy][a-zA-Z]{0,6}( \\d{4})?"
		,REGEX_ERR);

	#define STRARR_MATCH_REGEX(regex) (MATCH_REGEX(regex, strarr[index], index_len) >= 0)

	while (index < length){
		int index_len = strlen(strarr[index]);
		
		/*printf("\n\t%s\n%10s %i  %10s %i  %10s %i\n%10s %i  %10s %i  %10s %i\n",  
			strarr[index],
			"at",        STRARR_MATCH_REGEX(at),
			"date",      STRARR_MATCH_REGEX(date),
			"ago_after", STRARR_MATCH_REGEX(ago_after), 
			"date_time", STRARR_MATCH_REGEX(date_time), 
			"week_day",  STRARR_MATCH_REGEX(week_day),
			"n_month",   STRARR_MATCH_REGEX(n_month)
		);    */
		
		if( *strarr[index] == 'a' && STRARR_MATCH_REGEX(at)){
			char s_hour[3]; 
			
			// separates  the hour part
			int h_len = 2;
			if (strarr[index+1][1] == ':') h_len--;
			strncpy(s_hour,strarr[index+1],h_len);
			s_hour[h_len] ='\0';
			
			tm->tm_hour = strtol(s_hour, NULL, 10);
			tm->tm_min  = strtol(&strarr[index+1][h_len+1], NULL, 10);
			index      += 2;
			continue; 
		}else if ( STRARR_MATCH_REGEX(week_day) ){
			
			int day = MONDAY; 
			int (*day_func)(int,int) = day_last;
			
			// finds the day
			switch( strarr[index+1][2] ){
				case 'e': day = TUESDAY; 
				break;
				case 'd': day = WEDNESDAY; 
				break;
				case 'u': day = THURSDAY; 
				break;
				case 'i': day = FRIDAY; 
				break;
				case 't': day = SATURDAY; 
				break;
				case 'n':   
					if ( strarr[index+1][1] == 'u' ) 
						day = SUNDAY; 
				break;
			}
			
			// choses the funtion to use 
			switch( strarr[index][2] ){
				case 'a': day_func = day_diff; // TODO day_near
				break;             
				case 'x': day_func = day_future;
				break;
 			}
			tm->tm_mday += day_func(tm->tm_wday, day);
			
			index++;
			continue;
		        
		// saves using regex if it does not start with a digit5	
		}else if (isdigit(*strarr[index])){
			
			//n (day|hour|minute) ago 
			if( STRARR_MATCH_REGEX(ago_after) ){
				int multiplier = 1, i = index;
				char second    = 'f'; 
				// compares the second letter to see if it is 'g' of ago
				if (strarr[index][REGEX_CAPURES(ago_after,3)+1] == 'g'){
					multiplier = -1;
					second     = 'g';
				}
				
				//checks if we are at the end 
				while ( strarr[i][1] != second ){
					parse_time_spec(strarr,  tm, &index, multiplier);
					i+=2 ;
				}
				
				// for the ago|after
				index++;
				continue;
				
			}else if( STRARR_MATCH_REGEX(date_time) ){
				strptime(strarr[index], "%FT%H:%M:%S", tm);
				index++;
				continue;
				
			}else if( STRARR_MATCH_REGEX(date) ){
				strptime(strarr[index], "%F", tm);
				index++;
				continue;
			}else if( STRARR_MATCH_REGEX(n_month) ){
				// parses the month
				strptime(strarr[index+1], "%b", tm);
				// parses the month's day
				char day[2];
				int  day_len = (strarr[index+1] - strarr[index] - 3);
				strncpy(day, strarr[index], day_len);
				tm->tm_mday = strtol(day, NULL, 10);
				
				if (REGEX_RESULT(n_month) == 3){
					strptime(strarr[index+2], " %Y", tm);
				}
					
				index += REGEX_RESULT(n_month);
				continue;
			}
		}
		
		index++;
	}
	
	timegm(tm); // corrects the time using gmtd
	return tm;
	
}

// parse n (days|hours|minutes) and increments index by 2; 
void parse_time_spec(char** strarr, struct tm* tm, int *index, int multiplier){
	long num = strtol(strarr[*index], NULL, 10);			
	switch (*strarr[*index+1]){
		case 'm': tm->tm_min  += multiplier * num;
		break;
		case 'h': tm->tm_hour += multiplier * num;
		break;
		case 'd': tm->tm_mday += multiplier * num;
		break;
	}
	*index +=2;
}

