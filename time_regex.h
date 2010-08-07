
#ifndef TIME_REGEX_HEAD_MA
#define TIME_REGEX_HEAD_MA

#define PCRE_OVECCOUNT 18    /* should be a multiple of 3 */

// Makes a pcre object for matching regexes
#define MAKE_REGEX(name,regex,err_action)\
	static const pcre *pcre_##name;\
	const char *pcre_error_##name;\
	int  prce_error_pos_##name, pcre_ovector_##name[PCRE_OVECCOUNT];\
	int pcre_res_##name = -1;\
	\
	pcre_##name = pcre_compile(\
			 regex,                  /* the pattern */\
			 0,                      /* default options */\
			 &pcre_error_##name,     /* for error message */\
			 &prce_error_pos_##name, /* for error offset */\
			 NULL                    /* use default character tables */\
	);\
	\
	if (pcre_##name == NULL) {\
		printf("%s\n", "error compiled");\
		err_action\
	}

// checks if a string matches the a regex compiled by MAKE_REGEX
#define MATCH_REGEX(name, str, length)\
	(pcre_res_##name = pcre_exec(\
			 pcre_##name,           /* the compiled pattern */\
			 NULL,                  /* no extra data*/\
			 str,                   /* the subject string */\
			 length,                /* the length of the subject */\
			 0,                     /* start at the offset in the subject */\
			 0,                     /* default options */\
			 pcre_ovector_##name,   /* output vector for substring information */\
			 PCRE_OVECCOUNT         /* number of elements in the output vector */\
	))
	
// Frees all resources of the regex
#define FREE_REGEX(name) pcre_free(pcre_##name)

// Get the nth capture (the index in the string that the it starts)
#define REGEX_CAPURES(name,index) pcre_ovector_##name[index]
#define REGEX_RESULT(name) pcre_res_##name

#endif