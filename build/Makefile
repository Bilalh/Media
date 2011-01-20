# Uses make dist to make everything and make install to install

                           ### Variables ###
CC             = gcc
CFLAGS         = -g -Wall
PERL           = perl

SCRIPT_DST     = ${HOME}/scripts
BIN            = ${HOME}/bin
MAN_DST        = /usr/local/share/man/man7/
.DEFAULT_GOAL := media

compile        = ${CC} -std=gnu99 -fblocks
flags          = ${CFLAGS} ${includes} -I../src -I../
includes       = -I/opt/local/include -I/usr/include/libxml2 
libs           = -lsqlite3 -lxml2  -lcurl -L/opt/local/lib -lpcre
script_src     = ../src/scripts

                           ### Includes ###
vpath %.c ../src 
vpath %.h ../
vpath %.sh %.rb %.r %.py %.pl %.applescript ../src/scripts
# Rules for tests and requiments
include ../tests/tests.mk


                           ### Distribution ###
dist: CFLAGS = -O2 -Wall 
dist: clean all oclean

install: to_bin to_scripts

to_bin:
	cp ${objs} ${BIN}

                           ### Objects ###

objs       = media mal nums numsf numss ongoing ongoingnc hista hists shash set_id
examples   = optt hashe2 subdir_test colours2
c_scripts  = make_man_options 

media_req    = app/media_main.o media.o players.o ${opt_req} ${hist_req} ${play_req} ${strutil_req} ${sub_dirs_req}
mal_req      = app/ml_main.o ${ml_req} ${hist_req}
sub_dirs_req = sub_dirs.o ${strutil_req} ${strarr_req}

nums_req      = app/latest_main.o ${hist_req} 
numsf_req     = app/latest_f_main.o ${hist_req} 
numss_req     = app/latest_fs_main.o ${hist_req} 
ongoing_req   = app/ongoing_main.o ${hist_req}
ongoingnc_req = app/ongoing_no_colour_main.o ${hist_req} 
set_id_req    = app/set_id_total.o ${hist_req}


hista_req  = app/history_add_main.o ${hist_req} ${time_req}  ${strutil_req} ${string_req}
hists_req  = app/set_score_main.o ${hist_req} ${time_req}  ${strutil_req} ${string_req}
shash_req  = app/hash_lookup.o ${strutil_req}


optt_req        = examples/optm.o ${opt_req}
hashe2_req      = examples/hashe2.o
colours2_req    = examples/colours2.o
subdir_test_req = ${sub_dirs_req}

make_man_options_req = make_man_options.o ${string_req} ${strutil_req}

                           ### Scripts ###
scripts_keep_name = get_number.sh get_titles.sh hide_extension.applescript

scripts                   = histm gf setLabel setLabelByNumber histc
histm_location            = history_add_menu.rb
gf_location               = get_selected_finder_item.sh
setLabel_location         = set_label.rb
setLabelByNumber_location = set_label_by_number.applescript
histc_location            = history_copy.rb

                           ### Objects rules ###
# makes name : ${name_req}
$(foreach name,${objs} ${examples} ${c_scripts} ,$(eval ${name}:${${name}_req}))

.PHONY: ${scripts} ${scripts_keep_name}
${scripts}:
	cp "${script_src}/${${@}_location}" "${SCRIPT_DST}/$@"; \
	chmod +x "${SCRIPT_DST}/$@";
	
${scripts_keep_name}:
	cp "${script_src}/$@" "${SCRIPT_DST}/$@"; \
	chmod +x "${SCRIPT_DST}/$@";

${objs} ${examples} ${c_scripts}: 
	${compile} ${libs} ${flags} ${^F} -o $@

all:         ${objs}
examples:    ${examples}  list_examples
cscripts:    ${c_scripts} list_cscripts	
to_scripts:  ${scripts} ${scripts_keep_name}

#TODO put mweb.rb in scripts
to_scripts:
	cp ../src/history/mweb.rb ${SCRIPT_DST}/histw; \
	chmod +x ${SCRIPT_DST}/histw;
	cp ../src/history/mwebc.rb ${SCRIPT_DST}/histwc; \
	chmod +x ${SCRIPT_DST}/histwc;

                           ### Help ###
list_objs:
	@${PERL} -e 'print join "\n", qw{${objs}}, ""'

list_scripts:
	@${PERL} -e 'print join "\n", qw{${scripts}}, ""'

list_cscripts:
	@${PERL} -e 'print join "\n", qw{${c_scripts}}, ""'

list_examples:
	@${PERL} -e 'print join "\n", qw{${examples}}, ""'

list_targets:
	@${PERL} -e 'print join "\n", qw{dist all cscripts to_scripts run_tests examples}, ""'

list_all: list_targets list_objs list_scripts list_cscripts 
list:     list_all
                           ### Tests ###
run_tests: all_tests
	./all_tests

all_tests: ${tests} ../tests/all_tests.o
	${compile} ${libs} ${flags} $(filter %.o,${^F}) -o $@
	@echo "Built all tests : ${tests}"

test_%: ../tests/%_test.o ../tests/%_helper.o %_wrapper.o
	${compile} ${libs} ${flags}  ${^F} -o $@

list_tests:
	@${PERL} -e 'print join "\n", qw{${tests} all_tests run_tests}, ""'

# makes main method for tests
%_wrapper.c:
	./make_wrapper.sh "$@" "../tests/include/"

#needs sudo
install_man_pages:
	mkdir -p ${MAN_DST} && cp ../man/*.7 ${MAN_DST}/.

	                           ### Scripts rules ###
temp_files   = years days
result_files = years.pdf days.pdf

plot:
	../src/scripts/make_plot_years.rb > years
	../src/scripts/make_plot_days.rb > days
	../src/scripts/make_plot_years.r          && \
	mv Rplots.pdf years.pdf                   && \
	../src/scripts/make_plot_days.r           && \
	mv Rplots.pdf days.pdf

                           ### util ###
%.o: %.c
	${compile} ${flags} -c $^ -o ${@F}

clean:
	rm -f *.o *.out test_* *_wrapper.c all_tests ${objs} ${examples} ${c_scripts} ${temp_files} ${result_files} *~
                                       
oclean:                                
	rm -f *.o *.out test_* *_wrapper.c all_tests ${examples} ${c_scripts} ${temp_files} *~

help: 
	@${PERL} -e 'print join "\n", qw{dist install install_man_pages}, ""'
