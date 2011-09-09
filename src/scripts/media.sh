#!/bin/bash
function mediah(){
	hashfile=${HASHFILE:-$HOME/Library/Application Support/Media/zzhash}	
	dir="$1";
	shift;
	MEDIA=${MEDIA_BINARY:-media}
	${MEDIA} "$dir" --hashlocation "${hashfile}" $*
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

alias  _ax='mediah --ax --out --colour-ep --label-watched --conf input_no_enter.conf'
alias   ax='_ax --dot-default'
alias   ay='_ax --mplayer'
alias   ml='ay --last'
alias  mll='ml --none --dot-default'
alias   mw='ay --un-watched --first '
alias  mww='mw --none --dot-default ' 
alias  mwg='ay --un-watched  --menu --top --history'
alias   ag='ay --menu'
alias   mx='ax --only-menu'

alias   op='mediah --op --out --mplayer'
alias  opp='op --none --dot-default'
alias  opr="op --top --169 --rnd --framedrop --fast --no-out --dot-default --conf input_next_on_click.conf"
alias oprs='opr -sps -k0W1440'

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


