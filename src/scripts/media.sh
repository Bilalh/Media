#!/bin/bash
function mediah(){
	hashfile="$HOME/Library/Application Support/Media/zzhash"
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

alias  _ax='mediah --ax --out --colour-ep --label-watched'
alias   ax='_ax --dot-default'
alias   ay='_ax --mplayer'
alias   ml='ay --last'
alias  mll='ml --none --dot-default'
alias  mlg='ml --dot-default --menu --top '
alias   mw='ay --un-watched --first '
alias  mww='mw --none --dot-default ' 
alias  mwg='mw --menu --top --history'
alias  mgw='mwg'
alias mlwg='mwg'

alias   op='mediah --op --out --mplayer'
alias  opp='op --none --dot-default'
alias  opr="op --top --169 --rnd --framedrop --fast --no-out --dot-default"
alias opr1='opr -f -E -vo -E corevideo:device_id=1'

alias shash='shash "$HOME/Library/Application Support/Media/zzhash"'
alias numsfs='numss'
alias on='ongoing'
alias onm='histm'

function hget () {
	shash "$1"
	shash "$1" v | pbcopy
}

function hadd() {
	
	hashfile="$HOME/Library/Application Support/Media/zzhash"
	
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

alias au="ax -u"
alias mu="ml -u"
alias mlu="mll -u"
alias opu='op -u'
alias oppu='opp -u'

function media_binary_here(){
	MEDIA_BINARY=`pwd`/media
}

function  media_binary_clear() {
	unset MEDIA_BINARY
}