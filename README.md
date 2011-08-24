Meida {#readmeTitle}
=====
Media is used to play video and/or audio files, provide a nice interface to MPlayer and keep track of which episodes of series has been watched. It has a options to show only the latest or unwatched files and update mylist
{#description}

Usage
-----
Usage media start_path [args]

	 args: [-abcdefghijklmnopqrstuvwxyz.ABCDEFGHIJKLMNOPQRSTUVWXYZ,0123456789:/[]{}@%^=]
	 Use -h for help Use -h[num] for a section in the help
	 Use -h,{letters} for details of the arg(s)
	
See the man pages or `media -h` for more details.

Prerequisites
-------------
* sqlite3
* pcre
* libcurl (for mylist)
* libxml  (for mylist and playlists)
* linux has lots extra requirements see Readm

Install 
-------
* Put the scripts in your `$PATH`
	
		make dist
		make install
		make create_database
		-Optional-
		(might need sudo)
		make install_man_pages
		make install_media_aliases

* Note the xcode project is **only** for **debugging** not building 

Issues
------
See man pages