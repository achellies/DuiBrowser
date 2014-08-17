#ifndef _ISEEDEFS_H
#define _ISEEDEFS_H

// Main window messages
#define WM_ADD_TABWIN		(WM_USER + 0x101)
#define WM_DEL_TABWIN		(WM_USER + 0x102)
#define WM_SET_TITLE		(WM_USER + 0x103)
#define WM_SET_URL			(WM_USER + 0x104)
#define WM_LOAD_URL			(WM_USER + 0x201)
#define WM_FULL_SCREEN		(WM_USER + 0x202)
#define WM_UPDATE_STATE		(WM_USER + 0x203)

#define MAX_URL_LEN			1024

#define SHOTCUT_NEW			0
#define SHOTCUT_RELOAD		1
#define SHOTCUT_STOP		2
#define SHOTCUT_FORWARD		3
#define SHOTCUT_BACK		4

#define TOOL_MENU_FAVOR		11

#define _rw(r)              ((r).right - (r).left)
#define _rh(r)              ((r).bottom - (r).top)

#endif // _ISEEDEFS_H
