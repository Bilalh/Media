// TODO opt mmap and malloc error when adding new field when forking (getopt fault)
#include "include/opt_test.h"
OptVar

// Setup that is done once before the tests are run
#define OptSetup\

	
// Setup that is done before each test
#define OptBefore \
	MediaArgs *exp = new_media_args();

// test_result must be a true value for true and false otherwise
#define OptAfter(name) \
	test_result = opt_test_start (name, exp);\
	free(exp->prefix_args.str);\
	free(exp->postfix_args.str);\
	free(exp->pl_name);\
	free(exp->hash_location);\
	free(exp);

TestResult opt_test_main(int test_no) {
OptSetup	
Sections {
Section("Basic Features"){
	OptTest("-f", {
		string_push_m(&exp->prefix_args, 2, "-fs", "-aspect 16:10");
	})
	OptTest("-m",{
		exp->player = P_MPLAYER;
	})
	OptTest("-k",{
		string_push(&exp->prefix_args, "-loop 0");
	})
}OptEndSection


Section("Filepath"){
	OptTest("--rootpath media",{
		exp->root_dir = "media";
	})
}OptEndSection


Section("other"){
	OptTest("--done",{
		exp->status |= S_UPDATED;
	})
	OptTest("-[",{
		exp->write_history = true;
	})
	OptTest("-l",{
		exp->newest_only = true;
	})
}OptEndSection

//LATER  opt  test and imp excludes -e
//-e  --exclude {dir}      Sub directories to exclude

Section("output"){
	OptTest("-o",{
		exp->pl_output |= PL_STDOUT;
	})
	OptTest("-x",{
		exp->pl_output |= PL_PLAYLIST;
		exp->pl_format |= F_PLIST;
	})
	OptTest("-o -x",{
		exp->pl_output |= PL_STDOUT;
		exp->pl_output |= PL_PLAYLIST;
		exp->pl_format |= F_PLIST;
	})
}OptEndSection


Section("aspect "){
	OptTest("--fast",{
		string_push_m(&exp->prefix_args, 2, 
			"-lavdopts", "skipframe=nonref:skiploopfilter=all:fast=1" );
	})
	OptTest("-A 16:9",{
		string_push(&exp->prefix_args, "-aspect 16:9");
	})
	OptTest("-A 16:9 --fast",{
		string_push(&exp->prefix_args, "-aspect 16:9");
		string_push_m(&exp->prefix_args, 2, 
			"-lavdopts", "skipframe=nonref:skiploopfilter=all:fast=1" );
	})
	OptTest("--fast -A 16:9",{
		string_push(&exp->prefix_args, "-aspect 16:9");
		string_push_m(&exp->prefix_args, 2, 
			"-lavdopts", "skipframe=nonref:skiploopfilter=all:fast=1" );
	})
}OptEndSection

Section("Advanced"){
	OptTest("-mNV --rootpath aaa -fat --fast -k",{
		exp->player   = P_MPLAYER;
		exp->player   = P_NICEPLAYER;
		exp->player   = P_VLC;
		exp->root_dir = "aaa";
		string_push_m(&exp->prefix_args, 2, "-fs", "-aspect 16:10");
		exp->afloat=true;
		string_push(&exp->prefix_args, "-profile t");
		string_push(&exp->prefix_args, "-nofs");
		string_push_m(&exp->prefix_args, 2, 
			"-lavdopts", "skipframe=nonref:skiploopfilter=all:fast=1" );
		string_push(&exp->prefix_args, "-loop 0");
	})
	OptTest("-mNVA 16:9 --rootpath aaa -fat --fast -k",{
		exp->player   = P_MPLAYER;
		exp->player   = P_NICEPLAYER;
		exp->player   = P_VLC;
		string_push(&exp->prefix_args, "-aspect 16:9");
		exp->root_dir = "aaa";
		string_push_m(&exp->prefix_args, 2, "-fs", "-aspect 16:10");
		exp->afloat=true;
		string_push(&exp->prefix_args, "-profile t");
		string_push(&exp->prefix_args, "-nofs");
		string_push_m(&exp->prefix_args, 2, 
			"-lavdopts", "skipframe=nonref:skiploopfilter=all:fast=1" );
		string_push(&exp->prefix_args, "-loop 0");
	})
	OptTest("-mNV --rootpath aaa -A 16:9 -fat --fast -k",{
		exp->player   = P_MPLAYER;
		exp->player   = P_NICEPLAYER;
		exp->player   = P_VLC;
		exp->root_dir = "aaa";
		string_push(&exp->prefix_args, "-aspect 16:9");
		string_push_m(&exp->prefix_args, 2, "-fs", "-aspect 16:10");
		exp->afloat=true;
		string_push(&exp->prefix_args, "-profile t");
		string_push(&exp->prefix_args, "-nofs");
		string_push_m(&exp->prefix_args, 2, 
			"-lavdopts", "skipframe=nonref:skiploopfilter=all:fast=1" );
		string_push(&exp->prefix_args, "-loop 0");
	})
	OptTest("-mNV --rootpath aaa -fa -A 16:9 -t --fast -k",{
		exp->player   = P_MPLAYER;
		exp->player   = P_NICEPLAYER;
		exp->player   = P_VLC;
		exp->root_dir = "aaa";
		string_push_m(&exp->prefix_args, 2, "-fs", "-aspect 16:10");
		exp->afloat=true;
		string_push(&exp->prefix_args, "-aspect 16:9");
		string_push(&exp->prefix_args, "-profile t");
		string_push(&exp->prefix_args, "-nofs");
		string_push_m(&exp->prefix_args, 2, 
			"-lavdopts", "skipframe=nonref:skiploopfilter=all:fast=1" );
		string_push(&exp->prefix_args, "-loop 0");
	})
	OptTest("-mNV --rootpath aaa -fat -A 16:9  --fast -k",{
		exp->player   = P_MPLAYER;
		exp->player   = P_NICEPLAYER;
		exp->player   = P_VLC;
		exp->root_dir = "aaa";
		string_push_m(&exp->prefix_args, 2, "-fs", "-aspect 16:10");
		exp->afloat=true;
		string_push(&exp->prefix_args, "-profile t");
		string_push(&exp->prefix_args, "-nofs");
		string_push(&exp->prefix_args, "-aspect 16:9");
		string_push_m(&exp->prefix_args, 2, 
			"-lavdopts", "skipframe=nonref:skiploopfilter=all:fast=1" );
		string_push(&exp->prefix_args, "-loop 0");
	})
	OptTest("-x -ol -[ --profile t -W 22 -L4 -r",{
		exp->pl_format |= F_PLIST;
		exp->pl_output |= PL_PLAYLIST;
		exp->pl_output |= PL_STDOUT;
		
		exp->newest_only    = true;
		exp->write_history  = true;
		string_push_m(&exp->prefix_args, 2,"-profile", "t");
		string_push_m(&exp->prefix_args, 2,"-xy", "22");
		string_push_m(&exp->prefix_args, 2, "-loop", "4");
		string_push(&exp->prefix_args, "-shuffle");
	})
	OptTest("-57 -F -nofs -n plys --done",{
		string_push(&exp->prefix_args, "-geometry 0%:50%");
		string_push(&exp->prefix_args, "-geometry 50%:0%");
		string_push(&exp->postfix_args, "-nofs");
		exp->pl_name = "plys";
		exp->status |= S_UPDATED;
	})
	OptTest("-mNV --rootpath aaa -fatA 16:9 -k"
			" -x -ol -[ --profile t -W 22 -L4 -r "
			"-57 -F -nofs -n plys --done",{
		exp->player   = P_MPLAYER;
		exp->player   = P_NICEPLAYER;
		exp->player   = P_VLC;
		exp->root_dir = "aaa";
		string_push_m(&exp->prefix_args, 2, "-fs", "-aspect 16:10");
		exp->afloat=true;
		string_push(&exp->prefix_args, "-profile t");
		string_push(&exp->prefix_args, "-nofs");
		string_push(&exp->prefix_args, "-aspect 16:9");
		string_push(&exp->prefix_args, "-loop 0");
		exp->pl_format |= F_PLIST;
		exp->pl_output |= PL_PLAYLIST;
		exp->pl_output |= PL_STDOUT;
		exp->newest_only    = true;
		exp->write_history  = true;
		string_push_m(&exp->prefix_args, 2,"-profile", "t");
		string_push_m(&exp->prefix_args, 2,"-xy", "22");
		string_push_m(&exp->prefix_args, 2, "-loop", "4");
		string_push(&exp->prefix_args, "-shuffle");
		string_push(&exp->prefix_args, "-geometry 0%:50%");
		string_push(&exp->prefix_args, "-geometry 50%:0%");
		string_push(&exp->postfix_args, "-nofs");
		exp->pl_name = "plys";
		exp->status |= S_UPDATED;
	})
}OptEndSection

};

TestRun
OptPrintTestResults
OptMakeResult;
return OptResults;
}
