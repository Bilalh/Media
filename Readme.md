Media {#readmeTitle}
=====
Media is used to play video and/or audio files, provide a nice interface to MPlayer and keep track of which episodes of series has been watched. It has a options to show only the latest or unwatched files and update mylist
{#description}

Usage
-----
Usage `media start_path [args]`

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
* linux has lots extra requirements see Readme

Install 
-------
* Put the scripts in your `$PATH` e.g `export PATH=$PATH:~/${HOME}/bin` for default settings
	
		make dist
		make install
		make create_database
		-Optional-
		(might need sudo)
		make install_man_pages
		make install_mplayer_options
		make install_media_aliases

* Note the xcode project is **only** for **debugging** not building 

Plots
-----
If use `media` to keep track of which episodes of series has been watched, then you can made nice plots using `make plot`

Issues
------
See man pages

Licence
-------
[Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License](http://creativecommons.org/licenses/by-nc-sa/3.0/ "Full details")

Authors
-------
* Bilal Syed Hussain