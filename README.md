Media
=====
Media is used to play video and/or audio files, provide a nice interface to mplayer and keep track of which episodes of series has been watched. It has a options to show only the latest or unwatched files and update mylist

There also other binaries see the `man` pages in man for details 

installing 
----------
By default the program install to ~/bin


from `build`
<code>
	make
	make install
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
