#include "include/strutil_test.h"
StrutilVar

// Setup that is done once before the tests are run
#define StrutilSetup 

// Setup that is done before each test
#define StrutilBefore \
	char* actual;\
	char* expected; 
	
// test_result must be a true value for true and false otherwise
#define StrutilAfter(name) \
	test_result = strutil_test_start (name, actual, expected);\
	free(actual);


#define StrutilTestEp(_name, tBLOCK)                \
	MakeTest(Strutil, _name, tBLOCK,                \
		char **actual;                              \
		char  *exp_str;                             \
		int    exp_int;                             \
		,                                           \
		test_result =                               \
		ep_num_test(name, actual, exp_int, exp_str);\
		free(actual);                               \
	);

#define StrutilTestSp(_name, tBLOCK)                \
	MakeTest(Strutil, _name, tBLOCK,                \
		char **actual;                              \
		int    alen = 0;                            \
		int    elen = 0;                            \
		,                                           \
 		elen = sizeof(e)/sizeof(e[0]);              \
		actual = spilt_string(name, &alen);         \
		test_result =                               \
		spilt_words_test                            \
			(name,actual,alen,e,elen);              \
		free(actual);                               \
	);


TestResult strutil_test_main(int test_no) {
StrutilSetup

Sections {
Section("str_lower"){
	StrutilTest("ABC", {
		actual   = str_lower(name, strlen(name));
		expected = "abc";
	})
	StrutilTest("dsdsDDKDKDsdsdsdsa", {
		actual   = str_lower(name, strlen(name));
		expected = "dsdsddkdkdsdsdsdsa";
	})
	StrutilTest("Actual = Str_Lower(Name);", {
		actual   = str_lower(name, strlen(name));
		expected = "actual = str_lower(name);";
	})
	StrutilTest("dsadasdascxczxc", {
		actual   = str_lower(name, strlen(name));
		expected = "dsadasdascxczxc";
	})
	StrutilTest("sad≈≈sa", {
		actual = str_lower(name, strlen(name));
		expected = "sad≈≈sa";
	})
}StrutilEndSection

Section("str_replace"){
	StrutilTest("str",{
		actual   = str_replace(name, strlen(name), "str", "string");
		expected = "string";
	})
	StrutilTest("_str_",{
		actual   = str_replace(name, strlen(name), "str", "string");
		expected = "_string_";
	})
	StrutilTest("_rep_REP_rpp_ree_ReP_rep",{
		actual   = str_replace(name, strlen(name), "rep", "new");
		expected = "_new_REP_rpp_ree_ReP_new";
	})
	StrutilTest("cpp|rb",{
		actual   = str_replace(name, strlen(name),"rb","ruby");
		actual   = str_replace(actual, strlen(actual),"cpp", "c plus plus");
		expected = "c plus plus|ruby";
	})
}StrutilEndSection

Section("ep_num") {
	StrutilTestM("Toindex - 4.webm", {
		test_result = ep_num_test(name, ep_num(name), 4, "Toindex");
	})
	StrutilTestEp("AAA - 1.mkv", {
		actual  = ep_num(name);
		exp_str = "AAA";
		exp_int = 1;
	})
	StrutilTestEp("aaa - 1.webm", {
		actual  = ep_num(name);
		exp_str = "aaa";
		exp_int = 1;
	})
	StrutilTestEp("dsdsd - 32.mp4", {
		actual  = ep_num(name);
		exp_str = "dsdsd";
		exp_int = 32;
	})
	StrutilTestEp("Zs - 322.divx", {
		actual  = ep_num(name);
		exp_str = "Zs";
		exp_int = 322;
	})
	StrutilTestEp("zs - 322.wmv", {
		actual  = ep_num(name);
		exp_str = "zs";
		exp_int = 322;
	})
	StrutilTestEp("T - 32.part.mkv", {
		actual  = ep_num(name);
		exp_str = "T";
		exp_int = 32;
	})
	StrutilTestEp("Katanagatari_-_2.divx", {
		actual  = ep_num(name);
		exp_str = "Katanagatari";
		exp_int = 2;
	})
	StrutilTestEp("To Aru Kagaku no Railgun 06.avi", {
		actual  = ep_num(name);
		exp_str = "To Aru Kagaku no Railgun";
		exp_int = 6;
	})
	StrutilTestEp("ToS Tethe'alla 4.mkv", {
		actual  = ep_num(name);
		exp_str = "ToS Tethe'alla";
		exp_int = 4;
	})
	StrutilTestEp("Shangri-La - 2.part.mkv", {
		actual  = ep_num(name);
		exp_str = "Shangri-La";
		exp_int = 2;
	})
	StrutilTestEp("At Ir 3 - schwarzweiß - 1.v", {
		actual  = ep_num(name);
		exp_str = "At Ir 3 - schwarzweiß";
		exp_int = 1;
	})
	StrutilTestEp("hakanaku_mo-towa_no_kanashi 1", {
		actual  = ep_num(name);
		exp_str = "hakanaku_mo-towa_no_kanashi";
		exp_int = 1;
	})
	StrutilTestEp("K-ON!! - 122.flv", {
		actual  = ep_num(name);
		exp_str = "K-ON!!";
		exp_int = 122;
	})
	StrutilTestEp("X - 1", {
		actual  = ep_num(name);
		exp_str = "X";
		exp_int = 1;
	})
	StrutilTestEp("X - 0009", {
		actual  = ep_num(name);
		exp_str = "X";
		exp_int = 9;
	})
	StrutilTestEp("XY - 9", {
		actual  = ep_num(name);
		exp_str = "XY";
		exp_int = 9;
	})
	StrutilTestEp("X 1", {
		actual  = ep_num(name);
		exp_str = "X";
		exp_int = 1;
	})
	StrutilTestEp("XY", {
		actual  = ep_num(name);
		exp_str = "XY";
		exp_int = 1;
	})
	StrutilTestEp("X", {
		actual  = ep_num(name);
		exp_str = "X";
		exp_int = 1;
	})
	StrutilTestEp("Kaichou wa-    01.mkv", {
		actual  = ep_num(name);
		exp_str = "Kaichou wa";
		exp_int = 1;
	})
	StrutilTestEp("Kaichou wa-01.mkv", {
		actual  = ep_num(name);
		exp_str = "Kaichou wa";
		exp_int = 1;
	})
	StrutilTestEp("Kaichou wa    ~13.mkv", {
		actual  = ep_num(name);
		exp_str = "Kaichou wa";
		exp_int = 13;
	})
	StrutilTestEp("Noien~~~01.mkv", {
		actual  = ep_num(name);
		exp_str = "Noien";
		exp_int = 1;
	})
	StrutilTestEp("Fate Stay Night~~01.mkv", {
		actual  = ep_num(name);
		exp_str = "Fate Stay Night";
		exp_int = 1;
	})
	StrutilTestEp("Terra e~_09.mkv", {
		actual  = ep_num(name);
		exp_str = "Terra e";
		exp_int = 9;
	})
	StrutilTestEp("Mai-Otome_~_01.mkv", {
		actual  = ep_num(name);
		exp_str = "Mai-Otome";
		exp_int = 1;
	})
	StrutilTestEp("Sora no Woto~05.mkv", {
		actual  = ep_num(name);
		exp_str = "Sora no Woto";
		exp_int = 5;
	})
	StrutilTestEp("Senjou no Valkyria ~ 01.mkv", {
		actual  = ep_num(name);
		exp_str = "Senjou no Valkyria";
		exp_int = 1;
	})
	StrutilTestEp("Ar to- Ut o ~Ha FRELIA~ - 22.mkv", {
		actual  = ep_num(name);
		exp_str = "Ar to- Ut o ~Ha FRELIA~";
		exp_int = 22;
	})
	StrutilTestEp("Ar to- Ut o ~Ha FREI~ - 22.mkv", {
		actual  = ep_num(name);
		exp_str = "Ar to- Ut o ~Ha FREI~";
		exp_int = 22;
	})
	StrutilTestEp("Tales of Vesperia - The First Strike.mkv", {
		actual  = ep_num(name);
		exp_str = "Tales of Vesperia - The First Strike";
		exp_int = 1;
	})
	StrutilTestEp("To.Aru.mkv", {
		actual  = ep_num(name);
		exp_str = "To.Aru";
		exp_int = 1;
	})
	// TODO X     3 ep_num
	// StrutilTestEp("X     3",{
	// 	actual  = ep_num(name);
	// 	exp_str = "X";
	// 	exp_int = 3;
	// })
} StrutilEndSection

#define e(unstring) e[elen++] = #unstring

Section("spilt_string") {
	StrutilTestSp("char*dasd asds dasd", 
		char * e[3];
		e[0] = "char*dasd";
		e[1] = "asds";
		e[2] = "dasd";
	)
	StrutilTestSp("-fs -m --fast", 
		char * e[3];
		e[0] = "-fs";
		e[1] = "-m";
		e[2] = "--fast";
	)
	StrutilTestSp("--G 32:33 -1R", 
		char * e[3];
		e[0] = "--G";
		e[1] = "32:33";
		e[2] = "-1R";
	)
	StrutilTestSp("-A 16:9 --fast", 
		char *e[3];
		e(-A);  e(16:9);  e(--fast);
	)
	StrutilTestSp("--fast -A 16:9", 
		char *e[3];
		e(--fast);  e(-A);  e(16:9);
	)
	StrutilTestSp("  -A 16:9 --fast", 
		char *e[4];
		e( );  e(-A);  e(16:9);  e(--fast);
	)
	StrutilTestSp("  --fast -A 16:9", 
		char *e[4];
		e( );  e(--fast);  e(-A);  e(16:9);
	)
	StrutilTestSp("-mNV --rootpath / -e opt -fatA 16:9 --fast -k "
				  "--no-m3u -x -ol -[ -no-done --profile t -W 22 -L4 -R "
				  "-57 -F -nofs -Z", 
		char * e[24];
		e(-mNV)     ; e(--rootpath) ; e(/)          ;
	    e(-e)       ; e(opt)        ; e(-fatA)      ; 
		e(16:9)     ; e(--fast)     ; e(-k)         ; 
		e(--no-m3u) ; e(-x)         ; e(-ol)        ; 
		e(-[)       ; e(-no-done)   ; e(--profile)  ; 
		e(t)        ; e(-W)         ; e(22)         ; 
		e(-L4)      ; e(-R)         ; e(-57)        ; 
		e(-F)       ; e(-nofs)      ; e(-Z)         ;
	)
} StrutilEndSection

Section("Hash"){
	char *vals[][2] = {
		{"11e", "11 eyes"},
		{"ai", "Atelier Iris"},
		{"na2", "Mahou Shoujo Lyrical Nanoha A's"}
	};
	
	StrutilTestM("Testing Hash",{
		Mapping *hash = load_hash(TEST_DIR"hashes/h0");
		Mapping *h;
		
		int count;
		if ( (count = HASH_COUNT(hash)) != 3){
			test_result = FAIL;
			PRINT_NAME_FAIL(name);
			eprintf(" Hash count exp:%d act:%d\n", 3,count);
		}else{
			int i = 0;
			for(h = hash; h != NULL; h = h->hh.next, i++) {
				if (strcmp(h->key,vals[i][0]) != 0 || strcmp(h->val,vals[i][1])  != 0){
					test_result = FAIL;
					PRINT_NAME_FAIL(name);
					eprintf(" Hash key exp:'%s' act:'%s'\n", vals[i][0], h->key );
					eprintf(" Hash val\n%s exp: '%s'\n%s act: '%s'\n",
						ERROR_SEP,vals[i][1],ERROR_SEP, h->val 
					);
					
					break;
				}
			}	
		}

		
		while (hash){
			h = hash;
			HASH_DEL(hash,h);
			free(h->key);
			free(h->val);
			free(h); 
		}
		free(hash);
		
		if (test_result){
			PRINT_NAME_PASS(name);
		} 
	})
}StrutilEndSection

Section("Spilt args"){
	
	struct spilt_test{
		char *out;
		int len;
		char *sep;
		char *start;
		char *end;
		char *hash;
	};
	

	struct spilt_test tests[] =
	{
		{.out = "abc_def_ghiEND",
		 .len = 3, .sep = "_", .start ="", .end = "END",.hash= "no_hashs"
		},
		{.out = "Atelier Iris 2_e",
		 .len = 1, .sep = "_", .start ="", .end = "_e",.hash= TEST_DIR"hashes/h1"
		},
		{.out = "11 eyes|Atelier Iris.mkv",
		 .len = 1, .sep = ",", .start ="", .end = ".mkv",.hash= TEST_DIR"hashes/h2"
		},
		{.out = "jMahou|Atelier,a|11 eyesa",
		 .len = 1, .sep = "_", .start ="j", .end = "a",.hash= TEST_DIR"hashes/h3"
		},
		{.out = "|",
		 .len = 1, .sep = "", .start ="", .end = "",.hash= TEST_DIR"hashes/none"
		},
		{.out = "|aa|",
		 .len = 1, .sep = "", .start ="", .end = "",.hash= TEST_DIR"hashes/none"
		},
		{.out = "aaa_bbb|ccc",
		 .len = 2, .sep = "_", .start ="", .end = "",.hash= TEST_DIR"hashes/h4"
		},
		{.out = "(aaa.*bb|cccc.*aaa|bb).mkv",
		 .len = 3, .sep = ".*", .start ="(", .end = ").mkv",.hash= TEST_DIR"hashes/h5"
		},
		{.out = "(def|ghi|abd).mkv",
		 .len = 1, .sep = "_", .start ="(", .end = ").mkv",.hash= TEST_DIR"hashes/h6"
		}
	};

	char *inputs[][10] = {
		{"abc", "def", "ghi"},
		{"ai2"},
		{"11e|ai"},
		{"na2|ai|11e"},
		{"|"},
		{"|aa|"},
		{"abc", "def|ghi"},
		{"abc","def|ghi","abc|def"},
		{"abc|def|ghi"}
	};
	
	int total_length = sizeof(inputs)/sizeof(inputs[0]);
	
	for(int i = 0; i < total_length; ++i){
		
		StrutilTestM(tests[i].out,{
			
			char *res = spilt_args(inputs[i], tests[i].len, 
								   tests[i].sep, tests[i].start, 
								   tests[i].end, tests[i].hash);
			if( strcmp(res, tests[i].out) != 0 ){
				test_result = false;
				PRINT_NAME_FAIL(name);
				eprintf("exp: '%s'\n", tests[i].out );
				eprintf("act: '%s'\n", res );
				eprintf("sep: '%s'  end:  '%s' file:  '%s'\n","_", "END", "no_hash" );
			}
			PRINT_IF_PASSED_a(name);
		})
		
	}
	
}StrutilEndSection

Section("newest_only"){
		
	char *in[][40] = {
		{
			"To index - 4.flv",
			"To index - 6.divx",
			"To index - 24.webm",
			"To index - 2.mkv",
			"To index - 22.avi",
			"To index - 1.part.avi",
			"To index - 20.mkv",
			"T of Ve  - 2.mkv"
		},
		{
			"A_b 1.mkv",
			"A_b - 2.mkv",
			"A_b   3.mkv",
			"aaa - 11.mkv",
			"aaa - 9.mkv"
		},
		{
			"Sora no ~05.mkv",
			"Sora no ~ 22.mkv",
			"Sora no - 0009",
			"Sora no 4.mkv",
			"Sora no~~~01.mkv",
			"Sora no_-_21.mkv",
			
			"na a's ~05.mkv",
			"na a's ~ 21.mkv",
			"na a's - 0009",
			"na a's 4.mkv",
			"na a's~~~01.mkv",
			"na a's_-_24.mkv",

			"ta s ~05.mkv",
			"ta s ~ 21.mkv",
			"ta s - 0309",
			"ta s 4.mkv",
			"ta s~~~01.mkv",
			"ta s_-_21.mkv",

			"Noien ~05.mkv",
			"Noien ~ 21.mkv",
			"Noien - 0009",
			"Noien 4.mkv",
			"Noien~~~41.mkv",
			"Noien_-_51.mkv",

			"ar to ~05.mkv",
			"ar to ~ 41.mkv",
			"ar to - 0009",
			"ar to 4.mkv",
			"ar to~~~01.mkv",
			"ar to_-_21.mkv",
		},
		{
			"na a's 4.mkv",
			"ta s_-_21.mkv",
			"Sora no ~05.mkv",
			"ta s - 0309",
			"ar to_-_21.mkv",
			"Noien_-_51.mkv",
			"Sora no 4.mkv",
			"na a's ~ 21.mkv",
			"ar to ~ 41.mkv",
			"ta s ~05.mkv",
			"na a's~~~01.mkv",
			"ar to~~~01.mkv",
			"Sora no - 0009",
			"Noien ~05.mkv",
			"ar to ~05.mkv",
			"Sora no ~ 22.mkv",
			"ar to - 0009",
			"Sora no~~~01.mkv",
			"Noien 4.mkv",
			"na a's - 0009",
			"Noien~~~41.mkv",
			"Noien - 0009",
			"Sora no_-_21.mkv",
			"ar to 4.mkv",
			"na a's ~05.mkv",
			"Noien ~ 21.mkv",
			"ta s~~~01.mkv",
			"ta s ~ 21.mkv",
			"ta s 4.mkv",
			"na a's_-_24.mkv",
		}
	};

	char *out[][10] = {
		{
			"To index - 24.webm",
			"T of Ve  - 2.mkv"
		},
		{
			"A_b   3.mkv",
			"aaa - 11.mkv",
			NULL
		},
		{
			"Sora no ~ 22.mkv",
			"na a's_-_24.mkv",
			"ta s - 0309",
			"Noien_-_51.mkv",
			"ar to ~ 41.mkv"
		},
		{
			"na a's_-_24.mkv",
			"ta s - 0309",
			"Sora no ~ 22.mkv",
			"ar to ~ 41.mkv",
			"Noien_-_51.mkv",
		}
	};
	
	int lens[][2]={
		{8,2},
		{5,3},
		{30,5},
		{30,5}
	};
	
	for(int i = 0; i < sizeof(in) / sizeof(in[0]); ++i) {
		char title[9];
		sprintf(title, "Test %2d",i );
		StrutilTestM(title, {
			int act_len  = lens[i][0];
			int exp_len  = lens[i][1];
			char **act  = newest_only(in[i], &act_len, false, i == 1 ?true: false);

			if (exp_len != act_len ){
				test_result = false;
			}else{
				for(int j = 0; j < act_len; ++j){
					if (!strcmp_null(out[i][j],act[j])) {
						test_result = false;
						break;
					}
				}
			}
			
			if (test_result) {
				PRINT_NAME_PASS(name);
			} else{
				PRINT_NAME_FAIL(name);
				int min;
				int max;
				char *extra_name;
				char **extra;
				if(exp_len >= act_len) {
					min        = act_len;
					max        = exp_len;
					extra_name = "exp";
					extra      = out[i];
				} else{
					min        = exp_len;
					max        = act_len;
					extra_name = "act";
					extra      = act;
				}

				eprintf("length exp: %d act: %d \n", exp_len, act_len );
				for(int j = 0; j < min; ++j) {
					eprintf("[%2d] act: %-30s   exp: %-30s \n", j, act[j], out[i][j] );
				}
				for(int j = min; j < max; ++j) {
					eprintf("%9s [%2d] %s: %s\n", "", j, extra_name, extra[j] );
				}
			}

		})
	}

}StrutilEndSection

};


TestRun
StrutilPrintTestResults
StrutilMakeResult;
return StrutilResults;
}
