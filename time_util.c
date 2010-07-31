#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <pcre.h>

#include "string_util.h"
#include "time_util.h"

void test_day_diff();

const char* time_spec[][5] = {
	{"day", "days",""}, {"hour", "hours",""},
	{"min", "mins", "minute", "minutes",""}, {"sec", "secs","second","second",""}
};

#define PARSE_ERR(str) fprintf(stderr, "invalid type %s\n",str);
#define OVECCOUNT 21    /* should be a multiple of 3 */

// Makes a pcre object for matching regexes
#define MAKE_REGEX(name,regex,err_action)\
	pcre *r_##name;\
	const char *e_##name;\
	int  eo_##name, ovector_##name[OVECCOUNT], res_##name = -1;\
	\
	r_##name = pcre_compile(\
			 regex,         /* the pattern */\
			 0,             /* default options */\
			 &e_##name,     /* for error message */\
			 &eo_##name,    /* for error offset */\
			 NULL           /* use default character tables */\
	);\
	if (r_##name == NULL) {\
		printf("%s\n", "error compiled");\
		err_action\
	}

// checks if a string matches the a regex compiled by MAKE_REGEX
#define MATCH_REGEX(name, str, length)\
	(res_##name = pcre_exec(\
			 r_##name,         /* the compiled pattern */\
			 NULL,             /* no extra data*/\
			 str,              /* the subject string */\
			 length,           /* the length of the subject */\
			 0,                /* start at the offset in the subject */\
			 0,                /* default options */\
			 ovector_##name,   /* output vector for substring information */\
			 OVECCOUNT         /* number of elements in the output vector */\
	))
	
// Frees all resources of the regex
#define FREE_REGEX(name) pcre_free(r_##name)

// Get the nth capture (the index in the string that the it starts)
#define REGEX_CAPURES(name,index) ovector_##name[index]
#define REGEX_RESULT(name) res_##name


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

int day_last(int day, int other_day) {
	if (day >= other_day) return -(day - other_day);
	else                  return -(day + 7 - other_day);
}

int day_future(int day, int other_day) {
	if (other_day >= day) return (other_day - day);
	else                  return (other_day + 7 - day);
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
	
	//n SPEC ago
	MAKE_REGEX(ago_after, "^\\d+ (min(ute)?|hour|day)s? (ago|after)",REGEX_ERR);
	// at hh:mm
	MAKE_REGEX(at, "^at ([0-1]?[0-9]|2[0-3]):([0-5]?[0-9])",REGEX_ERR);
	// compound test
	MAKE_REGEX(ago_after_m, "^(\\d+ (min(ute)?|hour|day)s? )+(ago|after)",REGEX_ERR);
	
	
	int times = 0;
	while (index < length){
		int index_len = strlen(strarr[index]);
		if ( MATCH_REGEX(ago_after, strarr[index], index_len) >= 0) {
			int multiplier = 1;
			// compares second letter 'g' of ago
			if (strarr[index+2][1] == 'g') multiplier = -1;
			
			parse_time_spec(strarr,  tm, &index, multiplier);
			// since 'n WORD ago' is three parts
			index ++;
			
		}else if( *strarr[index] == 'a' && 
				MATCH_REGEX(at, strarr[index], index_len) >= 0){
			char s_hour[3]; 
			
			// separates  the hour part
			int h_len = 2;
			if (strarr[index+1][1] == ':') h_len--;
			strncpy(s_hour,strarr[index+1],h_len);
			s_hour[h_len] ='\0';
			
			tm->tm_hour = strtol(s_hour, NULL, 10);
			tm->tm_min  = strtol(&strarr[index+1][h_len+1], NULL, 10);
			index      += 2;
			
		}else if( MATCH_REGEX(ago_after_m, strarr[index], index_len) >= 0){
			int multiplier = 1, i = index;
			char second    = 'f';
			// compares the second letter to see if it is 'g' of ago
			if (strarr[index][REGEX_CAPURES(ago_after_m,3)+1] == 'g'){
				multiplier = -1;
				second     = 'g';
			} 
			//checks if we are at the end 
			while ( strarr[i][1] != second ){
				parse_time_spec(strarr,  tm, &index, multiplier);
				i+=2;
			}
			// for the ago|after
			index++;
			
		}else{
			times++;
		}
		 
		// to stop loop if not matching 
		if (times >= 2){ 
			index++;
			times = 0;
		}
	}
	
	timegm(tm); // corrects the time using gmtd
	FREE_REGEX(ago_after);
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

//OLD CODE

struct tm *parse_time2(char **str, int length) {
	
	struct tm* tm = currentTime();

	int times = 0;
	int oldlength = length;
	while (length > 0){
		
		// for(int i = 0; i < length; ++i){
		// 	printf("	str[%i] %s\n",i, str[i]);
		// }
		
		// time formats 
		if( match(*str, "[0-9]{1,2} [a-zA-Z]{3}") > 0) {
			strptime(*str, "%d %b", tm);
			str++;
			length--;
		
		} else if( match(*str, "[0-9]{1,2}(th|st|nd)") > 0) {
			strptime(*str, "%dth", tm);
			str++;
			length--;
		
		} else if ( match(*str, DATE_TIME_REGEX) > 0) {
			strptime(*str, "%F %H:%M:%S", tm);
			return tm;
		
		} else if( match(*str, DATE_REGEX) > 0) {
			strptime(*str, "%F", tm);
			str++;
			length--;
		
		//FIXME time bug
		} else if ( match(*str, DATE_TIME_UK_REGEX) > 0) {
			strptime(*str, "%d/%m/%y %H:%M:%S", tm);
			return tm;
		}

		// n (day/hour/minute) ago/after
		if (length >= 3 && isdigit(**str)) {
			type_a(str, length, tm);
			str    += 3;
			length -= 3;
		}

		// at hh:mm
		if (length >= 2) {
			if (strcasecmp(*str, "at") == 0) {
				type_b(str, length, tm);
				str    += 2;
				length -= 2;
			}
		}
		
		if (length == oldlength) times++;
		else oldlength = length;
		
		if (times >= 2){ // to stop 
			str++;
			length--;
			times = 0;
		}
		
	}

	return tm;
}

// n (day/hour/minute) ago/after
void type_a(char **str, int length, struct tm* tm ) {
	//printf("%s\n", "type A - n time ago/after ");
	long num = strtol(str[0], NULL, 10);
	if (num == 0 || *str[1] == '\0') {
		PARSE_ERR("a");
		return;
	}

	T_Spec spec = -1; T_Pos pos;
	const char **arr = NULL;
	#define TIME_SPEC_TEST(arr,str,spec_v) \
		arr = time_spec[spec_v]; \
		while(**arr != '\0'){ \
			if(strcasecmp(str, *arr) == 0) spec = spec_v; \
			arr++; \
		}

	switch(*str[1]) {
	case 'D':
	case 'd': TIME_SPEC_TEST(arr, str[1], 0);
		break;

	case 'H':
	case 'h': TIME_SPEC_TEST(arr, str[1], 1);
		break;

	case 'M':
	case 'm': TIME_SPEC_TEST(arr, str[1], 2);
		break;

	case 'S':
	case 's': TIME_SPEC_TEST(arr, str[1], 3);
		break;
	}

	if (arr == NULL || spec == -1 ) {PARSE_ERR("a-spec"); return;}
	//printf("spec: %i\n", spec);

	if (strcasecmp(str[2], "ago") == 0)       pos = T_BEFORE;
	else if(strcasecmp(str[2], "after") == 0) pos = T_AFTER;
	else { PARSE_ERR("A - t-pos"); return; }
	//printf("pos: %i num: %li \n", pos, num);

	switch(spec) {
	case T_DAYS:
		tm->tm_mday += num * pos;
		break;
	case T_HOURS:
		tm->tm_hour += num * pos;
		break;
	case T_MINS:
		tm->tm_min += num * pos;
		break;
	case T_SECS:
		tm->tm_sec += num * pos;
		break;
	}
	timegm(tm);
}

// at hh:mm
void type_b(char **str, int length, struct tm* tm ) {
	//printf("%s\n", "type B - at nn:nn ");
	char temp[5];
	long hours, minutes, len = strlen(str[1]);

	if(len < 3 || len > 5) return;
	char *sep = strrchr(str[1], ':');
	if (sep == NULL) return;

	hours = atol(str[1]);
	strncpy(temp, (sep + 1),  (len - (sep - str[1]) - 1) );
	minutes = atol(temp);

	tm->tm_hour = hours;
	tm->tm_min  = minutes;

}


