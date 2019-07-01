/* modifier 0 means no modifier */
static int surfuseragent	= 1;  /* Append Surf version to default WebKit user agent */
static char *fulluseragent	= ""; /* Or override the whole user agent string */
static char *scriptfile		= "~/.surf/script.js";
static char *styledir		= "~/.surf/styles/";
static char *certdir		= "~/.surf/certificates/";
static char *cachedir		= "~/.surf/cache/";
static char *cookiefile		= "~/.surf/cookies.txt";
static char *dmenuhandlerpath	= "/home/gavinok/.scripts/dmenu/dmenuhandler";
static char *linkhandlerpath	= "/home/gavinok/.scripts/tools/linkhandler";
static char *searchurl		= "duckduckgo.com/?q=%s";
/* Webkit default features */
/* Highest priority value will be used.
 * Default parameters are priority 0
 * Per-uri parameters are priority 1
 * Command parameters are priority 2
 */
static Parameter defconfig[ParameterLast] = {
    /* parameter                    Arg value       priority */
    [AcceleratedCanvas]   =       { { .i = 1 },     },
    [AccessMicrophone]    =       { { .i = 0 },     },
    [AccessWebcam]        =       { { .i = 0 },     },
    [Certificate]         =       { { .i = 0 },     },
    [CaretBrowsing]       =       { { .i = 0 },     },
    [CookiePolicies]      =       { { .v = "@Aa" }, },
    [DefaultCharset]      =       { { .v = "UTF-8" }, },
    [DiskCache]           =       { { .i = 1 },     },
    [DNSPrefetch]         =       { { .i = 0 },     },
    [FileURLsCrossAccess] =       { { .i = 0 },     },
    [FontSize]            =       { { .i = 12 },    },
    [FrameFlattening]     =       { { .i = 0 },     },
    [Geolocation]         =       { { .i = 0 },     },
    [HideBackground]      =       { { .i = 0 },     },
    [Inspector]           =       { { .i = 0 },     },
    [Java]                =       { { .i = 0 },     },
    [JavaScript]          =       { { .i = 1 },     },
    [KioskMode]           =       { { .i = 0 },     },
    [LoadImages]          =       { { .i = 1 },     },
    [MediaManualPlay]     =       { { .i = 1 },     },
    [Plugins]             =       { { .i = 0 },     },
    [PreferredLanguages]  =       { { .v = (char *[]){ NULL } }, },
    [RunInFullscreen]     =       { { .i = 0 },     },
    [ScrollBars]          =       { { .i = 1 },     },
    [ShowIndicators]      =       { { .i = 1 },     },
    [SiteQuirks]          =       { { .i = 1 },     },
    [SmoothScrolling]     =       { { .i = 1 },     },
    [SpellChecking]       =       { { .i = 1 },     },
    [SpellLanguages]      =       { { .v = ((char *[]){ "en_US", NULL }) }, },
    [StrictTLS]           =       { { .i = 1 },     },
    [Style]               =       { { .i = 1 },     },
    [WebGL]               =       { { .i = 1 },     },
    [ZoomLevel]           =       { { .f = 1.0 },   },
};

static UriParameters uriparams[] = {
    { "(://|\\.)suckless\\.org(/|$)", {
      [JavaScript] = { { .i = 1 }, 1 },
      [Plugins]    = { { .i = 0 }, 1 },
    }, },
};

/* default window size: width, height */
static int winsize[] = { 800, 600 };

static WebKitFindOptions findopts = WEBKIT_FIND_OPTIONS_CASE_INSENSITIVE |
                                    WEBKIT_FIND_OPTIONS_WRAP_AROUND;

#define PROMPT_GO   "Open:"
#define PROMPT_FIND "Find:"

/* SETPROP(readprop, setprop, prompt)*/
#define SETPROP(r, s, p) { \
        .v = (const char *[]){ "/bin/sh", "-c", \
             "prop=\"$(printf '%b' \"$(xprop -id $1 $2 " \
             "| sed \"s/^$2(STRING) = //;s/^\\\"\\(.*\\)\\\"$/\\1/\" && cat ~/.config/bookmarks)\" " \
             "| dmenu -l 10 -p \"$4\" -w $1)\" && " \
             "xprop -id $1 -f $3 8s -set $3 \"$prop\"", \
             "surf-setprop", winid, r, s, p, NULL \
        } \
}

/* SETSEARCHPROP(readprop, setprop, prompt)*/
#define SETSEARCHPROP(r, s, p) { \
        .v = (const char *[]){ "/bin/sh", "-c", \
             "prop=\"$(printf '%b' \"$(xprop -id $1 $2 " \
             "| sed \"s/^$2(STRING) = //;s/^\\\"\\(.*\\)\\\"$/\\1/\")\" " \
             "| dmenu -p \"$4\" -w $1)\" && xprop -id $1 -f $3 8s -set $3 \"$prop\"", \
             "surf-setsearchprop", winid, r, s, p, NULL \
        } \
}

#define PASS(p) { \
        .v = (const char *[]){ "/bin/sh", "-c", \
             "mypassmenu --type -l 10 -p \"$1\" -w \"$0\"", \
              winid, p, NULL \
        } \
}

/* DOWNLOAD(URI, referer) */
#define DOWNLOAD(d, r) { \
	.v = (char *[]){ "/bin/sh", "-c", \
		"cd ~/Telechargements;"\
		"st -g \"=40-10+0\" -t float -e /bin/sh -c \"aria2c -x 3 -U '$1' -d ~/Downloads " \
		" --referer '$2' --load-cookies $3 --save-cookies $3 '$0' || notify-send \"DOWNLOAD Failed\"" \
		" sleep 3;\"", \
		d, useragent, r, cookiefile, NULL \
	} \
}

/* BM_ADD(readprop) */
#define BM_ADD(r) {\
        .v = (const char *[]){ "/bin/sh", "-c", \
             "(echo $(xprop -id $0 $1) | cut -d '\"' -f2 " \
             "| sed '' && cat ~/.config/bookmarks) " \
             "| awk '!seen[$0]++' > ~/.surf/bookmarks.tmp && " \
             "mv ~/.surf/bookmarks.tmp ~/.config/bookmarks", \
             winid, r, NULL \
        } \
}

/* BM_EDIT() */
#define BM_EDIT() {\
        .v = (const char *[]){ "st", "-e", "/bin/sh", "-c",\
             "$EDITOR ~/.config/bookmarks", \
              NULL \
        } \
}

#define LINKBOX() {\
        .v = (const char *[]){ "st", "-e", "/bin/sh", "-c",\
             "$EDITOR ~/Dropbox/LinkBox.html", \
              NULL \
        } \
}

#define VIMANYWHERE() {\
        .v = (const char *[]){ "/bin/sh", "-c",\
             "~/.scripts/tools/vim-anywhere.sh", \
              NULL \
        } \
}

#define RCLICK() {\
        .v = (const char *[]){ "/bin/sh", "-c",\
             "~/.scripts/9menu/globalrightclick.sh", \
              NULL \
        } \
}
/* PLUMB(URI) */
/* This called when some URI which does not begin with "about:",
 * "http://" or "https://" should be opened.
 */
#define PLUMB(u) {\
        .v = (const char *[]){ "/bin/sh", "-c", \
             "xdg-open \"$0\"", u, NULL \
        } \
}

/* VIDEOPLAY(URI) */
#define VIDEOPLAY(u) {\
        .v = (const char *[]){ "/bin/sh", "-c", \
             "mpv --really-quiet \"$0\"", u, NULL \
        } \
}

static char *linkselect_curwin [] = { "/bin/sh", "-c",
	"~/.scripts/surf/surf_linkselect.sh $0 'Link' | xargs -r xprop -id $0 -f _SURF_GO 8s -set _SURF_GO",
	winid, NULL
};

static char *linkselect_linkhandeler [] = { "/bin/sh", "-c",
	"~/.scripts/surf/surf_linkselect.sh $0 'Link' | sed 's*^*https://*g' | xargs linkhandler",
	winid, NULL
};

// static char *linkselect_newwin [] = { "/bin/sh", "-c",
// 	"~/.scripts/surf/surf_linkselect.sh $0 'Link (new window)' | xargs -r surf",
// 	winid, NULL
// };

static char *editscreen[] = { "/bin/sh", "-c", "~/.scripts/surf/edit_screen.sh", NULL };

/* styles */
/*
 * The iteration will stop at the first match, beginning at the beginning of
 * the list.
 */
static SiteSpecific styles[] = {
    /* regexp               file in $styledir */
    { ".*.suckless.org.*",	"suckless.css" },
};

/* certificates */
/*
 * Provide custom certificate for urls
 */
static SiteSpecific certs[] = {
    /* regexp               file in $certdir */
    { "://suckless\\.org/", "suckless.org.crt" },
};

#define MODKEY GDK_CONTROL_MASK

/* hotkeys */
/*
 * If you use anything else but MODKEY and GDK_SHIFT_MASK, don't forget to
 * edit the CLEANMASK() macro.
 */
static Key keys[] = {
    /* modifier              keyval          function    arg */
    { 0|GDK_SHIFT_MASK, GDK_KEY_o,      spawn,		SETPROP("_SURF_URI", "_SURF_GO", PROMPT_GO) },
    { 0,		GDK_KEY_o,      spawn,		SETPROP("_SURF_SEARCH", "_SURF_SEARCH", PROMPT_GO) },
    { 0,		GDK_KEY_s,      spawn,		LINKBOX() },
    { 0,		GDK_KEY_f,	externalpipe,	{ .v = linkselect_curwin } },
    { ShiftMask,        GDK_KEY_f,	externalpipe,	{ .v = linkselect_linkhandeler } },
    // { GDK_SHIFT_MASK|0, GDK_KEY_semicolon, externalpipe,	{ .v = linkselect_newwin } },
    { 0,		GDK_KEY_e,	externalpipe,	{ .v = editscreen } },
    { 0,                GDK_KEY_w,	spawn,              PASS("Select_Password") },
    { 0,                GDK_KEY_slash,	spawn,              SETSEARCHPROP("_SURF_FIND", "_SURF_FIND", PROMPT_FIND) },
    { 0,                GDK_KEY_m,	spawn,              BM_ADD("_SURF_URI") },
    { 0,		GDK_KEY_x,	spawn,		VIMANYWHERE() },
    { 0|GDK_SHIFT_MASK, GDK_KEY_m,	spawn,              BM_EDIT() },
    // { 0,                GDK_KEY_i,     lhandler,		{ .i = 0 } },
    { 0,		GDK_KEY_i,      insert,     { .i = 1 } },
    { 0,		GDK_KEY_Escape, insert,     { .i = 0 } },
    { 0,		GDK_KEY_Return, lhandler,           { .i = 1 } },
    { 0|GDK_SHIFT_MASK, GDK_KEY_r,	reload,             { .i = 1 } },
    { 0,                GDK_KEY_r,	reload,             { .i = 0 } },

    { 0,                GDK_KEY_l,	navigate,           { .i = +1 } },
    { 0,                GDK_KEY_h,	navigate,           { .i = -1 } },

    /* vertical and horizontal scrolling, in viewport percentage */
    { 0,                GDK_KEY_j,	scrollv,            { .i = +10 } },
    { 0,                GDK_KEY_k,     	scrollv,            { .i = -10 } },
    { MODKEY,           GDK_KEY_u,     	scrollv,            { .i = -50 } },
    { MODKEY,           GDK_KEY_d,     	scrollv,            { .i = +50 } },
    { MODKEY,           GDK_KEY_b,     	scrollv,            { .i = -100 } },
    { MODKEY,           GDK_KEY_f,     	scrollv,            { .i = +100 } },
    { 0,                GDK_KEY_g,     	scrolltolim,        { .i = -1 } },
    { 0|GDK_SHIFT_MASK, GDK_KEY_g,     	scrolltolim,        { .i = +1 } },
    { 0|GDK_SHIFT_MASK, GDK_KEY_h,     	scrollh,            { .i = -10 } },
    { 0|GDK_SHIFT_MASK, GDK_KEY_l,     	scrollh,            { .i = +10 } },


    { 0|GDK_SHIFT_MASK, GDK_KEY_j,     	zoom,               { .i = -1 } },
    { 0|GDK_SHIFT_MASK, GDK_KEY_k,     	zoom,               { .i = +1 } },
    { 0|GDK_SHIFT_MASK, GDK_KEY_q,     	zoom,               { .i = 0  } },
    { 0,                GDK_KEY_minus, 	zoom,               { .i = -1 } },
    { 0,                GDK_KEY_plus,  	zoom,               { .i = +1 } },

    { 0,                GDK_KEY_p,     	clipboard,          { .i = 1 } },
    { 0,                GDK_KEY_y,     	clipboard,          { .i = 0 } },
    { 0,                GDK_KEY_n,     	find,               { .i = +1 } },
    { 0|GDK_SHIFT_MASK, GDK_KEY_n,     	find,               { .i = -1 } },

    { 0|GDK_SHIFT_MASK, GDK_KEY_p,     	print,              { 0 } },
    { 0,                GDK_KEY_t,     	spawnnewclient,     { 0 } },
    { 0|GDK_SHIFT_MASK, GDK_KEY_a,     	togglecookiepolicy, { 0 } },
    { 0,                GDK_KEY_F11,	togglefullscreen,   { 0 } },
    { 0|GDK_SHIFT_MASK, GDK_KEY_i,	toggleinspector,    { 0 } },

    { 0,		GDK_KEY_grave,	navigate,		{ .i = -1 } },
    { 0|GDK_SHIFT_MASK, GDK_KEY_f,      toggle,     { .i = FrameFlattening } },
    // { 0,                GDK_KEY_v,      toggle,     { .i = CaretBrowsing } },
    // { 0|GDK_SHIFT_MASK, GDK_KEY_g,      toggle,     { .i = Geolocation } },
    // { 0|GDK_SHIFT_MASK, GDK_KEY_s,      toggle,     { .i = JavaScript } },
    /* { 0|GDK_SHIFT_MASK, GDK_KEY_i,      toggle,     { .i = LoadImages } }, */
    // { 0|GDK_SHIFT_MASK, GDK_KEY_a,      toggle,     { .i = Plugins } },
    // { 0|GDK_SHIFT_MASK, GDK_KEY_x,      toggle,     { .i = ScrollBars } },
    // { 0|GDK_SHIFT_MASK, GDK_KEY_t,      toggle,     { .i = StrictTLS } },
    // { 0|GDK_SHIFT_MASK, GDK_KEY_m,      toggle,     { .i = Style } },
};

/* button definitions */
/* target can be OnDoc, OnLink, OnImg, OnMedia, OnEdit, OnBar, OnSel, OnAny */
static Button buttons[] = {
    /* target       event mask      button  function        argument        stop event */
    { OnLink,	    0,		    2,	    clickspecial,      	{ .i =  0  },	1 },
    { OnDoc,	    0,		    2,      spawn,		SETPROP("_SURF_SEARCH", "_SURF_SEARCH", PROMPT_GO)},
    // { OnDoc,	    0,		    1,      insert,		{ .i =  1  },	1 },
    { OnLink,  	    MODKEY,	    2, 	    clickspecial, 	{ .i =  1  },	1 },
    { OnSel,  	    0,		    2, 	    spawn,		RCLICK(),	1 },
    { OnLink,  	    MODKEY,	    1, 	    clicknewwindow,    	{ .i =  1  },	1 },
    { OnAny,   	    0,		    8, 	    clicknavigate,    	{ .i =  -1 },	1 },
    { OnAny,   	    0,		    9, 	    clicknavigate,     	{ .i =  +1 },	1 },
    { OnMedia, 	    MODKEY,	    1, 	    clickexternplayer, 	{ 0  },		1  },
};
