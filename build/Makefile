# Uses make dist to make everything and make install to install

                           ### Variables ###
CC             = gcc
CFLAGS         = -g -Wall
PERL           = perl
.DEFAULT_GOAL := dist


BIN            = ${HOME}/bin
SCRIPT_DST     = ${HOME}/bin
MAN_DST        = /usr/local/share/man/man7
SUPPORT_DIR    = ~/Library/Application\ Support/Media/
# Inculdes when libs headers  (pcre for example) are not installed in /usr/local/include
EXTRA_INCLUDES = 

compile        = ${CC} -std=gnu99 -fblocks
flags          = ${CFLAGS} ${includes} -I../src -I../
includes       = -I/usr/include/libxml2 ${EXTRA_INCLUDES}
libs           = -lsqlite3 -lxml2  -lcurl -lpcre -lreadline
script_src     = ../src/scripts
config_src     = ../config
plot_src       = ../src/plot
                           ### Includes ###
vpath %.c ../src 
vpath %.h ../
# unused ?
vpath %.sh %.rb %.r %.py %.pl %.applescript ../src/scripts

# Rules for tests and requiments
include ../tests/tests.mk

                           ### Distribution ###
dist: CFLAGS = -O2 -Wall 
dist: clean version all oclean

install: to_bin to_scripts

to_bin:
	mkdir -p ${BIN};
	cp ${objs} ${BIN};

                           ### Objects ###

objs       = media mal nums numsf numss ongoing hista hists shash set_id ongoingnc ongoingdata
examples   = optt hashe2 subdir_test colours2 mplayer_fork_input
c_scripts  = make_man_options 

media_req    = app/media_main.o media.o players.o media_menu.o ${opt_req} ${hist_req} ${play_req} ${strutil_req} ${sub_dirs_req}
mal_req      = app/ml_main.o ${mlt_req} ${hist_req}
sub_dirs_req = sub_dirs.o ${strutil_req} ${strarr_req}

nums_req        = app/latest_main.o ${hist_req} 
numsf_req       = app/latest_f_main.o ${hist_req} 
numss_req       = app/latest_fs_main.o ${hist_req} 
ongoing_req     = app/ongoing_main.o ${hist_req}
ongoingnc_req   = app/ongoing_no_colour_main.o ${hist_req} 
ongoingdata_req = app/history_menu_main.o ${hist_req} 
set_id_req      = app/set_id_total_main.o ${hist_req}


hista_req = app/history_add_main.o ${hist_req} ${time_req}  ${strutil_req} ${string_req}
hists_req = app/set_score_main.o ${hist_req} ${time_req}  ${strutil_req} ${string_req}
shash_req = app/hash_lookup.o ${strutil_req}


optt_req        = examples/optm.o ${opt_req}
hashe2_req      = examples/hashe2.o
colours2_req    = examples/colours2.o
subdir_test_req = ${sub_dirs_req}

mplayer_fork_input: libs+=-lreadline
mplayer_fork_input_req = examples/mplayer_fork_input.o
make_man_options_req   = make_man_options.o ${string_req} ${strutil_req}

                           ### Scripts ###

scripts_keep_name = get_number.sh get_titles.sh hide_extension.applescript onRewatchedFinished.rb find_title_and_id.rb

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
	mkdir -p ${SCRIPT_DST};\
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

list:     list_all
list_all: list_targets list_objs list_scripts list_cscripts list_tests

list_all:
	@${PERL} -e 'print join "\n", qw{plot create_database install_man_pages install_media_aliases}, ""'

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
	mkdir -p ${MAN_DST} && cp ../man/*.7 ${MAN_DST}/
	ln -fs ${MAN_DST}/nums.7 ${MAN_DST}/numss.7
	ln -fs ${MAN_DST}/nums.7 ${MAN_DST}/numsf.7
	ln -fs ${MAN_DST}/ongoing.7 ${MAN_DST}/ongoingnc.7
	

	
install_media_aliases:
	mkdir -p ${SCRIPT_DST} && cp ../src/scripts/media.sh  ${SCRIPT_DST}/.
	
install_mplayer_options:
	cp -r ${config_src}/mplayer/* ~/.mplayer/
		
create_database:
	mkdir -p ${SUPPORT_DIR} && touch ${SUPPORT_DIR}/zzhash && cat ${script_src}/media.sql | sqlite3  ${SUPPORT_DIR}/media.db

	                           ### Plot rules ###

# list of plot to be made
# plot_files        = plot_years.pdf plot_ep_scores.pdf plot_days.pdf 
plot_files        = $(subst make_,,$(patsubst %.r,%.pdf,$(notdir $(wildcard ${plot_src}/*.r))))
plot_temp_files   =  $(addprefix data_, $(basename ${plot_files}))

plot: ${plot_files}
pplot: pclean ${plot_files}

plot_%.pdf: data_plot_%
	${plot_src}/make_$(basename $@).r 

data_plot_%: ${plot_src}/make_plot_%.rb
	$^ > "$@"	

.PRECIOUS: data_plot_%

                               ### Util ###
%.o: %.c
	${compile} ${flags} -c $^ -o ${@F}

clean:
	rm -f *.o *.out test_* *_wrapper.c all_tests ${objs} ${examples} ${c_scripts} ${plot_temp_files} ${plot_files} plot*_chart.pdf plot*_small.pdf *~
                                       
oclean:                                
	rm -f *.o *.out test_* *_wrapper.c all_tests ${examples} ${c_scripts} ${plot_temp_files} *~

pclean:
	rm -f ${plot_files} plot*_chart.pdf plot*_small.pdf *~

help: 
	@${PERL} -e 'print join "\n", qw{dist install create_database -Optional- sudo install_man_pages install_mplayer_options install_media_aliases}, ""'

version:
	[ -d ../.git ] && ./make_version.sh
	
	
# Syntax
# http://www.gnu.org/s/hello/manual/make/Automatic-Variables.html

# $^
# The names of all the prerequisites, with spaces between them. For prerequisites which are archive members, only the
# named member is used (see Archives). A target has only one prerequisite on each other file it depends on, no matter
# how many times each file is listed as a prerequisite. So if you list a prerequisite more than once for a target, the
# value of $^ contains just one copy of the name. This list does not contain any of the order-only prerequisites; for
# those see the ‘$|’ variable, below.

# $?
# The names of all the prerequisites that are newer than the target, with spaces between them. For prerequisites
# which are archive members, only the named member is used (see Archives).

# $(@F)
# The file-within-directory part of the file name of the target. If the value of ‘$@’ is dir/foo.o then ‘$(@F)’ is
# foo.o. ‘$(@F)’ is equivalent to ‘$(notdir $@)’.


# $+
# This is like ‘$^’, but prerequisites listed more than once are duplicated in the order they were listed in the
# makefile. This is primarily useful for use in linking commands where it is meaningful to repeat library file names
# in a particular order.

# $@
# The file name of the target of the rule. If the target is an archive member, then ‘$@’ is the name of the archive
# file. In a pattern rule that has multiple targets (see Introduction to Pattern Rules), ‘$@’ is the name of
# whichever target caused the rule's recipe to be run.

# $%
# The target member name, when the target is an archive member. See Archives. For example, if the target is
# foo.a(bar.o) then ‘$%’ is bar.o and ‘$@’ is foo.a. ‘$%’ is empty when the target is not an archive member.
