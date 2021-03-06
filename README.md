Dependancies:
=============
### surf
- dmenu - Address bar
- webkit2gtk - browser engine
- xprop
- gcr

### recommended
- ca-certificates
- curl ( default download handler )
- st   ( default terminal for curl )
- mpv  ( default video player )
- gdb and gdb-debug ( helps prevent intermittent crashes )

### specific to this fork
- youtube-dl   ( for playing videos )
- [dmenuhandler](https://github.com/Gavinok/scripts/blob/master/dmenu/dmenuhandler) ( for specific interactions with links )
- [linkhandler](https://github.com/Gavinok/scripts/blob/master/linkhandler)  ( for automatically interactions with links )
- pass & passmenu ( convinient embedded password manager )

Notes Form Gavin
================
- hopefully I can remove the dependency on passmenu
- runs into issues when focuse is taken away from surf
- to simply copy the password to clipboard remove the --type arg in PASS()

## Patches
### [suckless.org](suckless.org)
- [Bookmarks](https://surf.suckless.org/patches/bookmarking/surf-bookmarks-20170722-723ff26.diff)
### Personal
- open a web page with linkhandler script
- open a web page with dmenuhandler script
- open middle clicked link with linkhandler
- open control-middle clicked link with dmenuhandler
- scrolling no longer relies on depreciated code
- scroll directly to top and bottom of screen with ctrl-g and ctrl-shift-g respectively
- if the URL is not a full link is used it is searched with duckduckgo 
    - this allows you to use !Bang searching in dmenu
- removed togglestats, and pagestats from title to look better with dwm

	
p.s. For more help checkout [Surf Demystified](http://troubleshooters.com/linux/surf.htm)

surf - simple webkit-based browser
==================================
surf is a simple Web browser based on WebKit/GTK+.

Requirements
------------
In order to build surf you need GTK+ and Webkit/GTK+ header files.

In order to use the functionality of the url-bar, also install dmenu[0].

Installation
------------
Edit config.mk to match your local setup (surf is installed into
the /usr/local namespace by default).

Afterwards enter the following command to build and install surf (if
necessary as root):

    make clean install

Running surf
------------
run
	surf [URI]

See the manpage for further options.

Running surf in tabbed
----------------------
For running surf in tabbed[1] there is a script included in the distribution,
which is run like this:

	surf-open.sh [URI]

Further invocations of the script will run surf with the specified URI in this
instance of tabbed.

[0] http://tools.suckless.org/dmenu
[1] http://tools.suckless.org/tabbed

