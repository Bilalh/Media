#include "include/opt_helper.h"

#define opt_intcmp(NAME)	if ( actual->NAME != expected->NAME ) goto fail;
#define opt_strcmp(NAME)	opt_strcheck(actual->NAME, expected->NAME);
#define opt_strcheck(t1,t2)\
	if ( (t1 == NULL && t2 != NULL) || (t1 != NULL && t2 == NULL) ) goto fail;\
	if ( t1 != NULL && t2 != NULL && strcmp( t1, t2 ) != 0 ) goto fail;

#define opt_sbufcmp(NAME)\
	opt_strcheck(actual->NAME.str, expected->NAME.str);\
	opt_intcmp(NAME.length);\
	opt_intcmp(NAME.index);



bool opt_test_start ( char *name, MediaArgs *expected ){
	int length     = 0;
	char **args    = spilt_string(name,&length);
	MediaArgs *actual = option_parser(length, args);

	opt_intcmp(newest_only);
	opt_intcmp(sub_dirs);
	opt_intcmp(types);
	opt_strcmp(root_dir);
	
	opt_strcmp(pl_dir);
	opt_strcmp(pl_name);
	opt_intcmp(pl_format);
	opt_intcmp(pl_output);
	opt_intcmp(pl_shuffle);
	
	opt_strcmp(hash_location);
	opt_intcmp(use_hash);
	opt_intcmp(status);
	opt_intcmp(write_history);
	opt_intcmp(updated);
	opt_intcmp(shortcuts);
	
	opt_intcmp(player);
	opt_intcmp(afloat);
	opt_sbufcmp(prefix_args);
	opt_sbufcmp(postfix_args);

	opt_intcmp(nice_repeat);
	opt_intcmp(nice_random);


	free(actual); 
	PRINT_NAME_PASS(name);
	return PASS;

fail:
	PRINT_NAME_FAIL(name);	
	#define print_args(title,value) printf("%20s: '%s'\n",  title, value);
	#define print_hex(title,value) printf("%20s: 0x%x\n",  title, value);
	#define print_int(title,value) printf("%20s: %i\n",  title, value);
	#define print_2int(t1,v1,t2,v2) printf("%20s: %-3i %10s: %-3i \n",  t1, v1,t2,v2);

	print_args("act prefix_args",  NULLCHECK(actual->prefix_args.str));	
	print_args("exp prefix_args",  NULLCHECK(expected->prefix_args.str));
	print_2int("act prefix_args len", actual->prefix_args.length,
               "index", actual->prefix_args.index 
	)	
	print_2int("exp prefix_args len", expected->prefix_args.length,
               "index", expected->prefix_args.index 
	)
	
	#undef print_args
	#undef print_hex
	#undef print_int
	#undef print_2int
	free(actual);
	return FAIL;

}

