#include "include/opt_helper.h"

#define opt_intcmp(NAME)	if ( actual->NAME != expected->NAME ) goto fail;
#define opt_strcmp(NAME)	if ( ! strcmp_null(actual->NAME, expected->NAME) ) goto fail;

#define opt_sbufcmp(NAME)\
	strcmp_null(actual->NAME.str, expected->NAME.str);\
	opt_intcmp(NAME.index);


bool opt_test_start ( char *name, MediaArgs *expected ){
	int length     = 0;
	char **args    = spilt_string_m(name,&length);
	// saves pointers for freeing 
	char **keeps   = args; int olength = length;

	MediaArgs *actual = option_parser(&length, &args);

	for(int i = 0; i < olength; ++i){
		free(keeps[i]);
	}
	free(keeps);



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
	opt_intcmp(sep);
	
	opt_intcmp(player);
	opt_intcmp(afloat);
	opt_intcmp(all_spaces);
	opt_sbufcmp(prefix_args);
	opt_sbufcmp(postfix_args);

	opt_intcmp(nice_repeat);
	opt_intcmp(nice_random);

	free(actual); 
	PRINT_NAME_PASS_a(name);
	return PASS;

fail:
	PRINT_NAME_FAIL_a(name);
	
	#define print_args(title,value) printf("%20s: '%s'\n",  title, NULLCHECK(value));
	#define print_str(NAME) if ( ! strcmp_null(actual->NAME, expected->NAME))\
		printf("%20s:\t act: '%s'\n%20s \t exp: '%s' \n",\
			#NAME, NULLCHECK(actual->NAME),"", NULLCHECK(expected->NAME));
	#define print_int(NAME)\
		printf("%20s:\t act: %-6i exp: %-6i \n", #NAME, actual->NAME, expected->NAME);
	#define print_hex(NAME)  if (actual->NAME != expected->NAME) \
		printf("%20s:\t act: 0x%-4x exp: 0x%-4x \n", #NAME, actual->NAME, expected->NAME);
	#define print_bool(NAME) if (actual->NAME != expected->NAME) \
		printf("%20s:\t act: %-6s exp: %-6s \n",\
			#NAME, TRUTH(actual->NAME), TRUTH(expected->NAME));
	
	String *strs[][3] = { 
		{&actual->prefix_args,  &expected->prefix_args,   }, 
		{&actual->postfix_args, &expected->postfix_args,  }
	};
	
	char *names[] = {"prefix_args","postfix_args"};
	
	for(int i = 0; i < sizeof(strs)/ sizeof(strs[0]); ++i){
		if (strs[i][0]->index  != strs[i][1]->index   || 
			 strs[i][0]->length != strs[i][1]->length  ||
			! strcmp_null(strs[i][0]->str, strs[i][1]->str ) 
		){
			printf(" %s %-15s: '%s'\n","act", names[i] ,strs[i][0]->str  );
			printf(" %s %-15s: '%s'\n","exp", names[i] ,strs[i][1]->str  );
			printf(" act %s len: %-3i %10s: %-3i \n", 
				names[i], strs[i][0]->length,
				"index", strs[i][0]->index 
			);
			printf(" exp %s len: %-3i %10s: %-3i \n", 
				names[i], strs[i][1]->length,
				"index", strs[i][1]->index 
			);
			puts("");
		}
	}
	
	print_hex(newest_only);
	print_bool(sub_dirs);
	print_bool(types);
	print_str(root_dir);
	
	print_str(pl_dir);
	print_str(pl_name);
	print_hex(pl_format);
	print_hex(pl_output);
	print_bool(pl_shuffle);
	
	print_str(hash_location);
	print_bool(use_hash);
	print_hex(status);
	print_bool(write_history);
	print_int(sep);
	
	print_hex(player);
	print_bool(afloat);
	print_hex(all_spaces);
	
	print_bool(nice_repeat);
	print_bool(nice_random);
	free(actual);
	
	#undef print_args
	#undef print_str
	#undef print_hex
	#undef print_int
	#undef print_bool
	return FAIL;

}
