Media
=====
Media is used to play video and/or audio files, provide a nice interface to mplayer and keep track of which episodes of series has been watched. It has a options to show only the latest or unwatched files and update mylist

There also other binaries see the `man` pages in man for details 

installing 
----------
By default the program install to `~/bin` and the database is created in `~/Library/Application Support/Media/`

from `build`
<code>
	make
	make install
	make create_database
</code>

if you want to install the man pages
<code>
	sudo make install\_man\_pages
</code>

if you want to install the media alias & functions
<code>
	make install\_media\_alias
</code>
then add `. bin/media.sh` to your `.bashrc`

requirements
----------
* sqlite3
* libxml
* libcurl
* pcre
* linux has lots extra requirements see Readme