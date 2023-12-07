/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "fibonacci.c"
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert  inner gap between windows */
static const unsigned int gappoh    = 15;       /* vert  outer gap between windows and screen edge */
static const unsigned int gappov    = 15;       /* horiz outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *barlayout        = "ltn|s";
static const int user_bh            = 27;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          ={"EnvyCodeR Nerd Font:size=10","monospace:size=10"};
static const char dmenufont[]       = "EnvyCodeR Nerd Font:size=9";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
//   /---/ CUSTOM COLORS /---/
static const char main_col[]        = "#005577";
static const char white[]           = 	"#c8ccd6";
static const char black[]           = 	"#151515";
static const char bg[]              = 	"#080808";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	//[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	//[SchemeSel]  = { col_gray4, main_col,  main_col  },
	[SchemeNorm] = { white,     black,  bg },
	[SchemeSel]  = { black,     white,  white },
};
static const unsigned int alphas[][3]      = {
    /*               fg      bg        border*/
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
//static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tags[] = { " ", "󰘳 ", " ", " ", "󰊴 ", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "firefox",  NULL,       NULL,       1 << 0,       0,           -1 },
	{ "Spotify",  NULL,       NULL,       1 << 2,       0,           -1 },	//1 << 8  
	{ "discord",  NULL,       NULL,       1 << 3,       0,           -1 },	//1 << 8  
	{ "Steam",    NULL,       NULL,       1 << 4,       0,           -1 },	//1 << 8  
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[\\]",     dwindle },
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[@]",      spiral },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "[M]",      monocle },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]  = { "dmenu_run", "-m", dmenumon,"-p","Dmenu:_ ",  "-fn", dmenufont, "-nb", black, "-nf", white, "-sb", white, "-sf", black, NULL };
static const char *termcmd[]   = { "st", NULL };
static const char *turnoff[]   = { "/home/bbasic/.suckless/dmenu/scripts/turnoff.sh", NULL };
static const char *wifi[]      = { "/home/bbasic/.suckless/dmenu/scripts/wifi.sh", NULL };
static const char *screenshot[]= { "flameshot","gui", NULL };
/* VOLUME AND BRIGHTNESS CONTROL */
static const char *vol_up[]    = { "/home/bbasic/.config/dunst/vol_bri.sh", "volume_up", NULL};
static const char *vol_dn[]    = { "/home/bbasic/.config/dunst/vol_bri.sh", "volume_down", NULL};
static const char *vol_mt[]    = { "/home/bbasic/.config/dunst/vol_bri.sh", "volume_mute", NULL};
static const char *bri_up[]    = { "/home/bbasic/.config/dunst/vol_bri.sh", "brightness_up", NULL};
static const char *bri_dn[]    = { "/home/bbasic/.config/dunst/vol_bri.sh", "brightness_down", NULL};
/* VOLUME AND BRIGHTNESS CONTROL */
//static const char *upvol[]     = { "/usr/bin/pactl",   "set-sink-volume", "0",      "+5%",      NULL };
//static const char *downvol[]   = { "/usr/bin/pactl",   "set-sink-volume", "0",      "-5%",      NULL };
//static const char *mutevol[]   = { "/usr/bin/pactl",   "set-sink-mute",   "0",      "toggle",   NULL };
//static const char *light_up[]  = { "/usr/bin/brightnessctl",   	   "set",    "5%+", NULL };
//static const char *light_down[]= { "/usr/bin/brightnessctl",   	   "set",    "5%-", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,	                XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,	                XK_x,	   spawn,          {.v = turnoff } },
	{ MODKEY,	                XK_z,	   spawn,          {.v = wifi } },
	{ MODKEY,	                XK_s,	   spawn,          {.v = screenshot} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	//{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_u,	   zoom,           {0} },
	{ MODKEY|Mod4Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,	                XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	//{ 0,XF86XK_AudioRaiseVolume, 		   spawn, 	   {.v = upvol   } },
	//{ 0,XF86XK_AudioLowerVolume, 		   spawn, 	   {.v = downvol } },
	//{ 0,XF86XK_AudioMute, 			   spawn, 	   {.v = mutevol } },
	//{ 0,XF86XK_MonBrightnessUp,  		   spawn, 	   {.v = light_up} },
	//{ 0,XF86XK_MonBrightnessDown,		   spawn, 	   {.v = light_down} },
	{ 0,XF86XK_AudioRaiseVolume, 		   spawn, 	   {.v = vol_up   } },
	{ 0,XF86XK_AudioLowerVolume, 		   spawn, 	   {.v = vol_dn } },
	{ 0,XF86XK_AudioMute, 			   spawn, 	   {.v = vol_mt } },
	{ 0,XF86XK_MonBrightnessUp,  		   spawn, 	   {.v = bri_up} },
	{ 0,XF86XK_MonBrightnessDown,		   spawn, 	   {.v = bri_dn} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
