#!/bin/bash
function mediah(){
	hashfile=${HASHFILE:-$HOME/Library/Application Support/Media/zzhash}	
	dir="$1";
	shift;
	MEDIA=${MEDIA_BINARY:-media}
	${MEDIA} "$dir" --hashlocation "${hashfile}" -E -input -E file=~/.mplayer/pipe $*
}

alias mg='mediaf'
function mediaf(){
	full="`gf`";

	name=`basename "${full}"`;
	ext="${name##*.}"

	search="";
	dir="${full}";
	
	extra="";
	if [ ${#ext} -lt ${#name} ]; then
		search=${name%.*};
		dir="`dirname \"${full}\"`";
		extra='--no-hash'
	fi

	mediah "${dir}" "${search}" --dot-default ${extra} --audio --out --mplayer $*

}

alias  _ax='mediah --ax --out --colour-ep --label-watched --conf input_no_enter.conf -E -really-quiet'
alias   ax='_ax --dot-default'
alias   ay='_ax --mplayer'
alias   ml='ay --last'
alias  mls='ml -sps -0W1440'
alias  mll='ml --none --dot-default'
alias   mw='ay --un-watched --first '
alias  mww='mw --none --dot-default ' 
alias  mwg='ay --un-watched  --menu --top --history'
alias mwgs='mwg -sps -0W1440'
alias   ag='ay --menu'
alias   mx='ax --only-menu'

alias  _op='mediah --op --out --mplayer'
alias  opp='_op --none --dot-default'
alias  opr="op --top --169 --rnd --framedrop --fast --no-out --dot-default --conf input_with_last_fm.conf"
alias oprs='opr -fs#1 -0W1440'
alias oprf='opr -fs#0 -0W1920'
function op(){
	#if ( ! ps | grep last_fm_scrobble_on_mplayer_playedd_50 > /dev/null); then echo ; else last_fm_scrobble_on_mplayer_played_50 &; fi
	killall last_fm_scrobble_on_mplayer_played_50 &> /dev/null
	last_fm_scrobble_on_mplayer_played_50 &
	_op --tee $* | grep '^# '  
}

alias shash='shash "$HOME/Library/Application Support/Media/zzhash"'
alias numsfs='numss'
alias on='ongoing'
alias onm='histm'

alias last_eps='media . . --last -o | sort | uniq'

function hget () {
	shash "$1"
	shash "$1" v | pbcopy
}

function hadd() {
	hashfile=${HASHFILE:-$HOME/Library/Application Support/Media/zzhash}	
	
	if [ $#  -ne 2  ]; then
		echo "hadd key val"
	elif [[ ${#1} -lt 10 && ${#1} -gt 0 && ${#2} -gt 0 ]]; then
		key=$1
		val=$2
		printf "%d %2d ${key}\t${val}\n" ${#key} ${#val}  >> "${hashfile}";
	else 
		echo "len(key) < 10 and > 0  len(val) > 0"
	fi
}

alias mdb='open -a base ~/Library/Application\ Support/Media/Media.db'

alias au="ax -U"
alias mu="ml -U"
alias mlu="mll -U"
alias opu='op -U'
alias oppu='opp -U'

function media_binary_here(){
	MEDIA_BINARY=`pwd`/media
}

function  media_binary_clear() {
	unset MEDIA_BINARY
}
