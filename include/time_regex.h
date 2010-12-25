
#ifndef TIME_REGEX_HEAD_MA
#define TIME_REGEX_HEAD_MA

#define PCRE_OVECCOUNT 18    /* should be a multiple of 3 */

/**
 * @brief Makes a pcre object for matching regexes
 *
 * @param name   The name 
 * @param regex  The regex
 * @param opts   The pcre options to use (0 none)
 * 
 * Useful options:
 * PCRE_CASELESS   match both upper and  lower case could use (?i) instead
 * PCRE_DOTALL     make . match everything
 * PCRE_MULTILINE  for mutiline pattens
 *
 * @param err_action The action to carry out on error
 */
#define MAKE_REGEX_OPTS(name,regex,opts,err_action)\
	static const pcre *pcre_##name;\
	const char *pcre_error_##name;\
	int  prce_error_pos_##name, pcre_ovector_##name[PCRE_OVECCOUNT];\
	int pcre_res_##name = -1;\
	\
	pcre_##name = pcre_compile(\
			 regex,                  /* the pattern */\
			 opts,                   /* options */\
			 &pcre_error_##name,     /* for error message */\
			 &prce_error_pos_##name, /* for error offset */\
			 NULL                    /* use default character tables */\
	);\
	\
	if (pcre_##name == NULL) {\
		printf("%s\n", "error compiled");\
		err_action\
	}

#define MAKE_REGEX(name,regex,err_action) MAKE_REGEX_OPTS(name,regex,0,err_action)

// Makes the vars as static for a regex 
#define MAKE_REGEX_VARS(name)\
	static const pcre *pcre_##name;\
	static const char *pcre_error_##name;\
	static int  prce_error_pos_##name, pcre_ovector_##name[PCRE_OVECCOUNT];\
	static int pcre_res_##name = -1;\

// make a pcre using pre make varables make by MAKE_REGEX_VARS
#define MAKE_REGEX_PREMADE_VARS(name,regex,opts)\
	pcre_##name = pcre_compile(\
			 regex,                  /* the pattern */\
			 opts,                   /* options */\
			 &pcre_error_##name,     /* for error message */\
			 &prce_error_pos_##name, /* for error offset */\
			 NULL                    /* use default character tables */\
	);\
	

/**
 * @brief Checks if a string matches the a regex compiled by MAKE_REGEX
 *
 * @param name the name of the pcre
 * @param str the string to check
 * @param length the length of the string
 */
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
	
/**
 * @brief Frees all resources of pcre object
 *
 * @param name the name
 */
#define FREE_REGEX(name) pcre_free(pcre_##name)

/**
 * @brief Gets the nth capture (the index in the string that the it starts)
 *
 * @param name the name of pcre 
 * @param index the index to get
 */
#define REGEX_CAPURES(name,index) pcre_ovector_##name[index]

/**
 * @brief Get the result of the last match on the pcre 
 *
 * @param name the name of the pcre
 */
#define REGEX_RESULT(name) pcre_res_##name

#endif