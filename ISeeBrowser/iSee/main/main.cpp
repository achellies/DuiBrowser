#ifdef _WIN32_WCE
#include <ceconfig.h>
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#define SHELL_AYGSHELL
#endif

#ifdef _CE_DCOM
#define _ATL_APARTMENT_THREADED
#endif

#ifdef SHELL_AYGSHELL
#include <aygshell.h>
#pragma comment(lib, "aygshell.lib") 
#endif // SHELL_AYGSHELL
#endif

#include <Windows.h>
#include <tchar.h>
#include <commctrl.h>

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#ifndef _DEVICE_RESOLUTION_AWARE
#define _DEVICE_RESOLUTION_AWARE
#endif
#endif

#ifdef _DEVICE_RESOLUTION_AWARE
#include "DeviceResolutionAware.h"
#endif

#if _WIN32_WCE < 0x500 && ( defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP) )
#pragma comment(lib, "ccrtrtti.lib")
#ifdef _X86_	
#if defined(_DEBUG)
#pragma comment(lib, "libcmtx86d.lib")
#else
#pragma comment(lib, "libcmtx86.lib")
#endif
#endif
#endif

#ifdef _WIN32_WCE
#include <altcecrt.h>
#endif

#include "iSeeDefs.h"
#include "browser.h"

#ifdef _WIN32_WCE
#include "resourceppc.h"
#else
#include "Resource.h"
#endif

#include "UrlInputBox.h"
#include "ManageUrls.h"

#define MAX_LOADSTRING		100
#define URLBAR_HEIGHT		20
#define TID_START_BROWSER	1
#define CMDBAR_NUM			5

#define GOTO_BTN_NUM		3
#define MAX_GOTO_WIDTH		54
#define GOTO_WIDTH			16
#define GOTO_HEIGHT			16
#define GOTO_DRAW_GAP		2
#define GOTO_Y_OFFSET		1

#define GOTO_CMD_LOAD		0
#define GOTO_CMD_RELOAD		1
#define GOTO_CMD_STOP		2

HINSTANCE ghInstance = NULL;
HWND ghMainWnd = NULL;

static UrlInputBox  *gUrlInputBox = NULL;
static HWND ghViewWindow = NULL;

static TCHAR gszTitle[MAX_LOADSTRING];                    // The title bar text
static TCHAR gszWindowClass[MAX_LOADSTRING];            // the main window class name

#ifdef SHELL_AYGSHELL // Wince
static HWND	ghWndMenuBar = NULL; // menu bar handle
static bool g_bSipChange = true;
static UINT g_nMenuHeight = 0;
#endif
static HMENU ghMenuWindow = NULL; // Windows menu handle
static HMENU ghMenuTools = NULL; // Tools menu handle
static UINT g_nCurrWindow = 0;

static UINT gGotoPosX = 0;
static UINT gGotoPosY = 0;
static HIMAGELIST gGotoImage = NULL;
static int gGotoSel = -1;
static RECT gGotoRect;
static int gGotoPress = -1;

#ifdef _WIN32_WCE
typedef struct _MenuItem
{
	const WCHAR *name;
	int idx;
} MenuItem;

MenuItem g_allMenus[] = 
{
	{L"New", 0},
	{L"Close", 1},
	{L"FullScreen", 2},
	{L"EnableProxy", 3},
	{L"Refresh", 5},
	{L"Stop", 6},
	{L"Back", 7},
	{L"Forward", 8},
	{L"AddFavorite", 10},
	{L"ManageFavorites", 11},
	{L"GoFavorite", 12},
	{L"About", 14},
	{L"Exit", 15},

	{NULL, -1}
};

extern "C"
{

void WalkMenuBars(WCHAR *name, WCHAR **attrs)
{
	if (name == NULL || attrs == NULL ||
		attrs[0] == NULL || attrs[1] == NULL)
		return;

	int idx = -1;
	MenuItem *mi = NULL;
	for (int i = 0; g_allMenus[i].name; i++)
	{
		if (wcsicmp(name, g_allMenus[i].name) == 0)
		{
			idx = g_allMenus[i].idx;
			break;
		}
	}
	if (idx >= 0)
	{
		MENUITEMINFO mii;
		memset(&mii, 0, sizeof(MENUITEMINFO));
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_TYPE;
		mii.fType = MFT_STRING;

		if (GetMenuItemInfo(ghMenuTools, idx, TRUE, &mii))
		{
			mii.dwTypeData = attrs[1];
			mii.cch = wcslen(attrs[1]);
			SetMenuItemInfo(ghMenuTools, idx, TRUE, &mii);
		}
	}
}

};
#endif

static void resizeSubViews()
{
	RECT rcClient;
	GetClientRect(ghMainWnd, &rcClient);
	if (gUrlInputBox)
	{
		if (!IsFullScreen())
		{
			gGotoPosX = rcClient.right - MAX_GOTO_WIDTH;
			gGotoPosY = 1;
			SetRect(&gGotoRect, gGotoPosX, gGotoPosY, gGotoPosX + MAX_GOTO_WIDTH, gGotoPosY + GOTO_HEIGHT);
			gUrlInputBox->Move(0, 0, gGotoPosX, -1);
			gUrlInputBox->Show(true);
		}
		else
			gUrlInputBox->Show(false);
	}
	if (ghViewWindow)
	{
		UINT h = rcClient.bottom - (!IsFullScreen() ? URLBAR_HEIGHT : 0);
#ifdef SHELL_AYGSHELL
		static UINT nLastHeight = 0;
		if (!IsFullScreen())
		{
			if (g_bSipChange)
			{
				g_bSipChange = false;
				if (nLastHeight != _rh(rcClient))
				{
					nLastHeight = _rh(rcClient);
					if (rcClient.bottom > (nLastHeight - (g_nMenuHeight >> 1)))
						h -= g_nMenuHeight + 2;
				}
			}
		}
		else
			nLastHeight = 0;
#endif
		::MoveWindow(ghViewWindow, 0, (!IsFullScreen() ? URLBAR_HEIGHT : 0), rcClient.right, h, TRUE);
		if (IsFullScreen())
			SetFocus(ghViewWindow);
	}
}

// Message handler for about box.
static
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
#ifdef SHELL_AYGSHELL
		{
			// Create a Done button and size it.  
			SHINITDLGINFO shidi;
			shidi.dwMask = SHIDIM_FLAGS;
			shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_EMPTYMENU;
			shidi.hDlg = hDlg;
			SHInitDialog(&shidi);
		}
#endif // SHELL_AYGSHELL
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, message);
		return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}

static void UpdateHistoryButtons()
{
#if defined(_WIN32_WCE) && defined(SHELL_AYGSHELL)
	TBBUTTONINFO tbi;
	memset(&tbi, 0, sizeof(TBBUTTONINFO));
	tbi.cbSize = sizeof(TBBUTTONINFO);
	tbi.dwMask = TBIF_COMMAND | TBIF_STATE;

	if (SendMessage(ghWndMenuBar, TB_GETBUTTONINFO, IDM_BACKWARD, (LPARAM)&tbi) && tbi.idCommand == IDM_BACKWARD)
	{
		tbi.fsState = IsHistoryBegin() ? 0 : TBSTATE_ENABLED;
		SendMessage(ghWndMenuBar, TB_SETBUTTONINFO, IDM_BACKWARD, (LPARAM)&tbi);
	}

	if (SendMessage(ghWndMenuBar, TB_GETBUTTONINFO, IDM_FORWARD, (LPARAM)&tbi) && tbi.idCommand == IDM_FORWARD)
	{
		tbi.fsState = IsHistoryEnd() ? 0 : TBSTATE_ENABLED;
		SendMessage(ghWndMenuBar, TB_SETBUTTONINFO, IDM_FORWARD, (LPARAM)&tbi);
	}
#endif
}

static void LoadUrl(LPCWSTR urlStr, int nLen)
{
	UrlString us(urlStr, nLen);
	if  (-1 == gUrlInputBox->FindStringExact(-1, us))
	{
		gUrlInputBox->AddString(us.url());
		SaveHistoryUrl(us.url(), us.length());
	}
	LoadTabView(us.url(), us.length());
}

static void DoFullScreen()
{
#if defined(_WIN32_WCE) && defined(SHELL_AYGSHELL)
	bool fs = IsFullScreen();
	SetFullScreen(!fs);
	g_bSipChange = true;
	if (fs)
	{
		SHFullScreen(ghMainWnd, SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON);
		ShowWindow(ghWndMenuBar, SW_SHOW);
		RECT	rcw;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcw, 0);
		::SetWindowPos(ghMainWnd, NULL,
			rcw.left, rcw.top,
			_rw(rcw), _rh(rcw),
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
	else
	{
		SHSipPreference(ghMainWnd, SIP_FORCEDOWN);
		SHFullScreen(ghMainWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON);
		
		ShowWindow(ghWndMenuBar, SW_HIDE);
		::SetWindowPos(ghMainWnd, NULL, 0, 0,
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN),
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
#endif
}

static void EnableProxy()
{
	SetProxyEnable(!ProxyEnabled());
	CheckMenuItem(ghMenuTools, IDM_PROXY, MF_BYCOMMAND | (ProxyEnabled() ? MF_CHECKED : MF_UNCHECKED));
}

static void OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef SHELL_AYGSHELL
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);

	SHMENUBARINFO mbi;

	memset(&mbi, 0, sizeof(SHMENUBARINFO));
	mbi.cbSize     = sizeof(SHMENUBARINFO);
	mbi.hwndParent = hWnd;
	mbi.nToolBarId = IDR_MENU;
	mbi.hInstRes   = ghInstance;

	if (!SHCreateMenuBar(&mbi)) 
	{
		ghWndMenuBar = NULL;
	}
	else
	{
		ghWndMenuBar = mbi.hwndMB;

		// 加入图片按钮
		{
			HIMAGELIST hImageList = (HIMAGELIST)ImageList_LoadBitmap(ghInstance, MAKEINTRESOURCE(IDR_MENU), 16, CMDBAR_NUM, CLR_DEFAULT);
			::SendMessage(ghWndMenuBar, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
			if(hImageList != NULL)
			{
				TBBUTTON button; memset(&button, 0, sizeof(TBBUTTON));
				button.iString = -1;
				UINT idArray[] = {IDM_NEW, IDM_CLOSE, IDM_BACKWARD, IDM_FORWARD, IDM_FULL_SCREEN};
				int iImage = 0;
				for (int i = 0; i < CMDBAR_NUM; i++)
				{
					button.idCommand = idArray[i];
					button.fsState = TBSTATE_ENABLED;
					// a command button with image
					button.fsStyle = TBSTYLE_BUTTON;
					button.iBitmap = iImage++;
					SendMessage(ghWndMenuBar, TB_ADDBUTTONS, 1, (LPARAM)&button);
				}
			}
		}

		// 更改顶层菜单
		{
			TBBUTTONINFO tbi;
			memset(&tbi, 0, sizeof(TBBUTTONINFO));
			tbi.cbSize = sizeof(TBBUTTONINFO);
			tbi.dwMask = TBIF_COMMAND | TBIF_TEXT;

			LPWSTR szTool = GetConfigAttr(L"VALUE", L"MENU", L"TOOL");
			if (szTool)
			{
				if (SendMessage(ghWndMenuBar, TB_GETBUTTONINFO, IDM_TOOLS, (LPARAM)&tbi) && tbi.idCommand == IDM_TOOLS)
				{
					tbi.pszText = szTool;
					tbi.cchText = wcslen(szTool);
					SendMessage(ghWndMenuBar, TB_SETBUTTONINFO, IDM_TOOLS, (LPARAM)&tbi);
				}
			}

			LPWSTR szWindow = GetConfigAttr(L"VALUE", L"MENU", L"WINDOW");
			if (szWindow)
			{
				if (SendMessage(ghWndMenuBar, TB_GETBUTTONINFO, IDM_WINDOWS, (LPARAM)&tbi) && tbi.idCommand == IDM_WINDOWS)
				{
					tbi.pszText = szWindow;
					tbi.cchText = wcslen(szWindow);
					SendMessage(ghWndMenuBar, TB_SETBUTTONINFO, IDM_WINDOWS, (LPARAM)&tbi);
				}
			}
		}

		// 定制菜单资源
		{
			ghMenuTools = (HMENU)SendMessage(ghWndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_TOOLS);
			if (ghMenuTools)
			{
				WalkConfigNode(WalkMenuBars, L"MENU", L"TOOL");
			}
			ghMenuWindow = (HMENU)SendMessage(ghWndMenuBar, SHCMBM_GETSUBMENU, 0, IDM_WINDOWS);
		}

		GetClientRect(ghWndMenuBar, &rcClient);
		g_nMenuHeight = rcClient.bottom;
	}
#elif !defined(_WIN32_WCE)
	ghMenuTools = GetSubMenu(GetMenu(hWnd), 0);
	ghMenuWindow = GetSubMenu(GetMenu(hWnd), 1);
#endif // SHELL_AYGSHELL
	// 控制proxy代理菜单的状态
	CheckMenuItem(ghMenuTools, IDM_PROXY, MF_BYCOMMAND | (ProxyEnabled() ? MF_CHECKED : MF_UNCHECKED));
}

static bool DoMenuCommands(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	wmId    = LOWORD(wParam);
	wmEvent = HIWORD(wParam);
	
	// 主要处理Url输入框通知事件
	switch(wmEvent)
	{
	case CBN_DROPDOWN:
		gUrlInputBox->OnDropDown((HWND)lParam);
		break;
	case CBN_SELCHANGE:
		gUrlInputBox->OnSelchange((HWND)lParam);
		break;
	default:
		break;
	}
	
	// Parse the menu selections:
	switch (wmId)
	{
	case IDM_ABOUT:
		DialogBox(ghInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	case IDM_NEW:
		NewTabWindow(L"");
		break;
	case IDM_CLOSE:
		CloseTabWindow(ghMenuWindow, g_nCurrWindow);
		break;
	case IDM_FULL_SCREEN:
		DoFullScreen();
		break;
	case IDM_PROXY:
		EnableProxy();
		break;
	case IDM_REFRESH:
		ReloadTabView();
		break;
	case IDM_STOP:
		StopTabView();
		break;
	case IDM_FORWARD:
		GoTabForward();
		break;
	case IDM_BACKWARD:
		GoTabBack();
		break;
#ifdef _WIN32_WCE
	case IDM_ADD_FAVORITE:
		AddFavorite(ghMenuTools);
		break;
	case IDM_MANAGE_FAVORITES:
		ManageFavorites(ghMenuTools);
		break;
#endif
	default:
		if (wmId >= IDM_TAB_WINDOW)
		{	// Tab窗口列表
			if (wmId == g_nCurrWindow)
				break;
			// 处理选中状态
			MENUITEMINFO mii;
			memset(&mii, 0, sizeof(MENUITEMINFO));
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask = MIIM_ID | MIIM_DATA;
			int nLoops = GetViewsCount();
			int nBreaks = 0;
			UINT nOldWindow = g_nCurrWindow;
			for (int i = 0; i < nLoops; i++)
			{
				if (GetMenuItemInfo(ghMenuWindow, i, TRUE, &mii))
				{
					if (mii.wID == nOldWindow)
					{
						CheckMenuItem(ghMenuWindow, i, MF_BYPOSITION | MF_UNCHECKED);
						++nBreaks;
					}
					else if (mii.wID == wmId)
					{
						CheckMenuItem(ghMenuWindow, i, MF_BYPOSITION | MF_CHECKED);
						g_nCurrWindow = wmId;
						ActiveTabView((void *)mii.dwItemData);
						++nBreaks;
					}
					if (nBreaks >= 2)
						break;
				}
			}
			if (nBreaks >= 2)
				break;
		}
#ifdef _WIN32_WCE
		else if (wmId >= IDM_GO_FAVORITE)
		{	// 收藏夹的内容
			UINT nLen = MAX_URL_LEN;
			WCHAR wszUrl[MAX_URL_LEN];
			if (GetFavoriteUrl(wmId, wszUrl, nLen))
			{
				LoadUrl(wszUrl, nLen);
				SendMessage(hWnd, WM_SET_URL, nLen, (LPARAM)wszUrl);
			}
			break;
		}
#endif
		return false;
	}
	return true;
}

static POINT MakeCursorPoint(HWND hWnd, LPARAM lParam)
{
	POINT point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);
	return point;
}

static void OnLButtonDown(HWND hWnd, UINT nFlags, POINT point)
{
	if (gGotoPress < 0)
	{
		RECT rcGoto;
		for (int i = 0; i < GOTO_BTN_NUM; i++)
		{
			int x = gGotoPosX + 1 + i * (GOTO_WIDTH + GOTO_DRAW_GAP);
			SetRect(&rcGoto, x, gGotoPosY, x + GOTO_WIDTH, gGotoPosY + GOTO_HEIGHT);
			if (PtInRect(&rcGoto, point))
			{
				gGotoPress = gGotoSel = i;
				InvalidateRect(hWnd, &rcGoto, FALSE);
				SetCapture(hWnd);
				break;
			}
		}
	}
}

static void OnLButtonUp(HWND hWnd, UINT nFlags, POINT point)
{
	if (gGotoPress >= 0)
	{
		bool btnOn = false;
		RECT rcGoto;
		gGotoSel = -1;
		for (int i = 0; i < GOTO_BTN_NUM; i++)
		{
			int x = gGotoPosX + 1 + i * (GOTO_WIDTH + GOTO_DRAW_GAP);
			SetRect(&rcGoto, x, gGotoPosY, x + GOTO_WIDTH, gGotoPosY + GOTO_HEIGHT);
			if (PtInRect(&rcGoto, point) && i == gGotoPress)
			{
				btnOn = true;
				InvalidateRect(hWnd, &rcGoto, FALSE);
				if (GOTO_CMD_LOAD == i)
				{
					WCHAR szUrl[MAX_URL_LEN];
					int nLen = gUrlInputBox->GetInputText(szUrl, MAX_URL_LEN);
					if (nLen > 0)
						LoadUrl(szUrl, nLen);
				}
				else if (GOTO_CMD_RELOAD == i)
				{
					ReloadTabView();
				}
				else if (GOTO_CMD_STOP == i)
				{
					StopTabView();
				}
				break;
			}
		}
		if (!btnOn)
		{
			int x = gGotoPosX + 1 + gGotoPress * (GOTO_WIDTH + GOTO_DRAW_GAP);
			SetRect(&rcGoto, x, gGotoPosY, x + GOTO_WIDTH, gGotoPosY + GOTO_HEIGHT);
			InvalidateRect(hWnd, &rcGoto, FALSE);
		}
		gGotoPress = -1;
		ReleaseCapture();
	}
}

static void OnMouseMove(HWND hWnd, UINT nFlags, POINT point)
{
	if (gGotoPress >= 0)
	{
		RECT rcGoto;
		if (PtInRect(&gGotoRect, point))
		{
			int nCurSel = -1;
			for (int i = 0; i < GOTO_BTN_NUM; i++)
			{
				int x = gGotoPosX + 1 + i * (GOTO_WIDTH + GOTO_DRAW_GAP);
				SetRect(&rcGoto, x, gGotoPosY, x + GOTO_WIDTH, gGotoPosY + GOTO_HEIGHT);
				if (PtInRect(&rcGoto, point))
				{
					nCurSel = i;
					break;
				}
			}
			if (nCurSel == gGotoPress)
			{
				if (gGotoSel < 0)
				{
					gGotoSel = nCurSel;
					int x = gGotoPosX + 1 + gGotoPress * (GOTO_WIDTH + GOTO_DRAW_GAP);
					SetRect(&rcGoto, x, gGotoPosY, x + GOTO_WIDTH, gGotoPosY + GOTO_HEIGHT);
					InvalidateRect(hWnd, &rcGoto, FALSE);
				}
			}
			else
			{
				if (gGotoSel >= 0)
				{
					gGotoSel = -1;
					int x = gGotoPosX + 1 + gGotoPress * (GOTO_WIDTH + GOTO_DRAW_GAP);
					SetRect(&rcGoto, x, gGotoPosY, x + GOTO_WIDTH, gGotoPosY + GOTO_HEIGHT);
					InvalidateRect(hWnd, &rcGoto, FALSE);
				}
			}
		}
		else
		{
			if (gGotoSel >= 0)
			{
				gGotoSel = -1;
				int x = gGotoPosX + 1 + gGotoPress * (GOTO_WIDTH + GOTO_DRAW_GAP);
				SetRect(&rcGoto, x, gGotoPosY, x + GOTO_WIDTH, gGotoPosY + GOTO_HEIGHT);
				InvalidateRect(hWnd, &rcGoto, FALSE);
			}
		}
	}
}

static void OnPaint(HWND hWnd, HDC hdc)
{
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, MAX_GOTO_WIDTH, URLBAR_HEIGHT);
	HDC dcMem = CreateCompatibleDC(hdc);
	SelectObject(dcMem, hBmp);
	SetBkMode(dcMem, TRANSPARENT);

	RECT rcFill;
	SetRect(&rcFill, 0, 0, MAX_GOTO_WIDTH, URLBAR_HEIGHT);
	FillRect(dcMem, &rcFill, GetSysColorBrush(COLOR_BTNFACE));
	
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	SelectObject(dcMem, hPen);
	MoveToEx(dcMem, 0, 0, NULL);
	LineTo(dcMem, MAX_GOTO_WIDTH, 0);
	MoveToEx(dcMem, 0, URLBAR_HEIGHT - 1, NULL);
	LineTo(dcMem, MAX_GOTO_WIDTH, URLBAR_HEIGHT - 1);
	
	for (int i = 0, x = 1; i < GOTO_BTN_NUM; i++, x += GOTO_WIDTH + GOTO_DRAW_GAP)
	{
		ImageList_Draw(gGotoImage, i, dcMem, x, GOTO_Y_OFFSET, i == gGotoSel ? ILD_SELECTED : ILD_NORMAL);
	}
	BitBlt(hdc, gGotoPosX, 0, MAX_GOTO_WIDTH, URLBAR_HEIGHT, dcMem, 0, 0, SRCCOPY);

	DeleteObject(hBmp);
	DeleteDC(dcMem);
	DeleteObject(hPen);
}

static
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#if defined(SHELL_AYGSHELL) && !defined(WIN32_PLATFORM_WFSP)
	static SHACTIVATEINFO s_sai;
#endif // SHELL_AYGSHELL && !WIN32_PLATFORM_WFSP

	switch (message)
	{
	case WM_COMMAND:
		if (!DoMenuCommands(hWnd, message, wParam, lParam))
			return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_DESTROY:
#ifdef SHELL_AYGSHELL
		CommandBar_Destroy(ghWndMenuBar);
#endif // SHELL_AYGSHELL
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		resizeSubViews();
		break;

	case WM_CREATE:
		OnCreate(hWnd, message, wParam, lParam);
#ifdef SHELL_AYGSHELL
#ifndef WIN32_PLATFORM_WFSP
		// Initialize the shell activate info structure
		memset(&s_sai, 0, sizeof (s_sai));
		s_sai.cbSize = sizeof (s_sai);
#endif // !WIN32_PLATFORM_WFSP
#endif // SHELL_AYGSHELL
		break;

	case WM_ACTIVATE:
#if defined(SHELL_AYGSHELL) && !defined(WIN32_PLATFORM_WFSP)
		// Notify shell of our activate message
		SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
#endif
		setViewFocus(ghMenuWindow, g_nCurrWindow);
		break;
#if defined(SHELL_AYGSHELL) && !defined(WIN32_PLATFORM_WFSP)
	case WM_SETTINGCHANGE:
		SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
		break;
#endif // SHELL_AYGSHELL && !WIN32_PLATFORM_WFSP

	case WM_ADD_TABWIN: // 仅在菜单条上操作
		g_nCurrWindow = AddTabView(ghMenuWindow, (void *)lParam, g_nCurrWindow);
		break;
	case WM_DEL_TABWIN: // 仅在菜单条上操作
		g_nCurrWindow = DelTabView(ghMenuWindow, (void *)lParam);
		break;
	case WM_SET_TITLE:
		SetViewTitle(ghMenuWindow, (void *)lParam);
		break;
	case WM_LOAD_URL:
		LoadUrl((LPCWSTR)lParam, (int)wParam);
		break;
	case WM_SET_URL: // 设置URL输入框内容
		gUrlInputBox->SetInputText((LPCWSTR)lParam, (int)wParam);
		break;
	case WM_FULL_SCREEN:
		DoFullScreen();
		break;
	case WM_UPDATE_STATE:
		UpdateHistoryButtons();
		break;

	case WM_LBUTTONDOWN:
		OnLButtonDown(hWnd, (UINT)wParam, MakeCursorPoint(hWnd, lParam));
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(hWnd, (UINT)wParam, MakeCursorPoint(hWnd, lParam));
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(hWnd, (UINT)wParam, MakeCursorPoint(hWnd, lParam));
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hWnd, &ps);
			OnPaint(hWnd, hdc);
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_TIMER:
		if (TID_START_BROWSER == wParam)
		{
			KillTimer(hWnd, TID_START_BROWSER);
			// 启动缺省浏览器页面
			if (!StartBrowser())
				PostQuitMessage(0);
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

static ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASS wc;

	wc.style          = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc    = WndProc;
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = 0;
	wc.hInstance      = hInstance;
	wc.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ISEE));
	wc.hCursor        = 0;
	wc.hbrBackground  = (HBRUSH) GetStockObject(WHITE_BRUSH);
#ifdef _WIN32_WCE
	wc.lpszMenuName   = NULL;
#else
	wc.lpszMenuName   = MAKEINTRESOURCE(IDC_ISEE);
#endif
	wc.lpszClassName  = gszWindowClass;

	return RegisterClass(&wc);
}

static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
	// SHInitExtraControls should be called once during your application's initialization to initialize any
	// of the device specific controls such as CAPEDIT and SIPPREF.
	SHInitExtraControls();
#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
	//If it is already running, then focus on the window, and exit
	HWND hWnd = FindWindow(gszWindowClass, gszTitle);	
	if (hWnd) 
	{
		// set focus to foremost child window
		// The "| 0x00000001" is used to bring any owned windows to the foreground and
		// activate them.
		SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
		return 0;
	} 
#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP

	DWORD styles = 
#ifdef _WIN32_WCE
		WS_VISIBLE;
#else
		WS_OVERLAPPED     |
		WS_CAPTION        |
		WS_SYSMENU        |
		WS_THICKFRAME     |
		WS_MINIMIZEBOX    |
		WS_MAXIMIZEBOX;
#endif
	ghMainWnd = CreateWindow(gszWindowClass, gszTitle, styles,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!ghMainWnd)
		return FALSE;

#ifdef WIN32_PLATFORM_PSPC
	// When the main window is created using CW_USEDEFAULT the height of the menubar (if one
	// is created is not taken into account). So we resize the window after creating it
	// if a menubar is present
	if (ghWndMenuBar)
	{
		RECT rc;
		RECT rcMenuBar;

		GetWindowRect(hWnd, &rc);
		GetWindowRect(ghWndMenuBar, &rcMenuBar);
		rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);

		MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
	}
#endif // WIN32_PLATFORM_PSPC

	ShowWindow(ghMainWnd, nCmdShow);
	UpdateWindow(ghMainWnd);

	return TRUE;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	ghInstance = hInstance;
	SetRectEmpty(&gGotoRect);
	InitManageUrls();

	// 初始化浏览器，配置文件读取等
	if (!InitBrowser())
		return 0;
	
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, gszTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ISEE, gszWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
		return 0;

	gGotoImage = (HIMAGELIST)ImageList_LoadBitmap(ghInstance, MAKEINTRESOURCE(IDB_GOTO), 16, 3, CLR_DEFAULT);
	if (NULL == gGotoImage)
	{
		DestroyWindow(ghMainWnd);
		return FALSE;
	}

	gUrlInputBox = new UrlInputBox(hInstance, ghMainWnd);
	if (NULL == gUrlInputBox)
	{
		ImageList_Destroy(gGotoImage);
		DestroyWindow(ghMainWnd);
		return FALSE;
	}
	gUrlInputBox->SetFocus();

	// 从注册表载入URL列表（目前暂时放在注册表中）
	ReadHistoryUrls(gUrlInputBox);
#ifdef _WIN32_WCE
	// 载入Favorites
	ReadFavoriteUrls(ghMenuTools);
#endif

	// 获取WebView显示窗口
	ghViewWindow = GetViewWindow();
	if (!ghViewWindow)
	{
		delete gUrlInputBox;
		ImageList_Destroy(gGotoImage);
		DestroyWindow(ghMainWnd);
		return 0;
	}
	// 显示WebView窗口
	resizeSubViews();
	ShowWindow(ghViewWindow, SW_SHOWNORMAL);
	UpdateWindow(ghViewWindow);

	// 显示Goto图标
	InvalidateRect(ghMainWnd, NULL, FALSE);

	// 50ms后启动缺省浏览器页面
	SetTimer(ghMainWnd, TID_START_BROWSER, 50, NULL);

	// 消息循环
#ifndef WIN32_PLATFORM_WFSP
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ISEE));
#endif

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
#ifndef WIN32_PLATFORM_WFSP
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
#endif
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	ExitBrowser();
	delete gUrlInputBox;
	ImageList_Destroy(gGotoImage);
	DestroyWindow(ghMainWnd);

	return 0;
}
