// If we don't define these, they get defined in windef.h. 
// We want to use std::min and std::max.
#include "config.h"

#include <windows.h>
#include <commctrl.h>

#include "iSeeDefs.h"
#include "UrlInputBox.h"

#ifdef _WIN32_WCE
#include "resourceppc.h"
#else
#include "Resource.h"
#endif

#include "WebView.h"
#include "browser.h"

using namespace WebCore;

#define HISTORY_URL_NUM		100
#define FAVORITE_URL_NUM	20
#define MAX_NAME_LEN		40
#define INVALID_POS			0xFFFFFFFF
#define MAX_TITLE_LEN		30
#define BLANK_STR_LEN		5

static BYTE gHistoryUrls[HISTORY_URL_NUM];
static UINT gHisUrlNum = 0;
static UINT gRewindHisUrl = 0;

static UINT favorCommands[FAVORITE_URL_NUM];
static UINT gRewindFavorite = 0;

static DWORD ReadDwordValue(LPCWSTR wszPath, LPCWSTR wszName)
{
	HKEY hKey;
	if (ERROR_SUCCESS != RegOpenKeyExW(HKEY_LOCAL_MACHINE, wszPath, 0, KEY_ALL_ACCESS, &hKey))
		return 0;

	DWORD dwValue, dwType, dwDataLen;
	if (ERROR_SUCCESS != RegQueryValueExW(hKey, wszName, NULL, &dwType, (LPBYTE)&dwValue, &dwDataLen))
		dwValue = 0;

	RegCloseKey(hKey);
	return dwValue;
}

static void WriteDwordValue(LPCWSTR wszPath, LPCWSTR wszName, DWORD dwValue)
{
	DWORD dwDisposition;
	HKEY hKey;
	if (ERROR_SUCCESS != RegCreateKeyExW(HKEY_LOCAL_MACHINE, wszPath, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition))
		return;

	RegSetValueExW(hKey, wszName, 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));
	RegCloseKey(hKey);
}

void InitManageUrls()
{
	memset(gHistoryUrls, 0, sizeof(gHistoryUrls));
	memset(favorCommands, 0, sizeof(favorCommands));
}

void ReadHistoryUrls(UrlInputBox *urlInputBox)
{
	DWORD dwDisposition;
	HKEY hKeyUrls;
	if (ERROR_SUCCESS != RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\iSee\\history\\urls", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyUrls, &dwDisposition))
		return;
	if (dwDisposition & REG_CREATED_NEW_KEY)
	{
		RegCloseKey(hKeyUrls);
		return;
	}

	DWORD dwIndex = 0;
	DWORD dwRes = ERROR_SUCCESS;
	WCHAR wszName[MAX_NAME_LEN];
	WCHAR wszData[MAX_URL_LEN];
	DWORD dwNameLen, dwDataLen;
	gHisUrlNum = 0;
	while (dwRes == ERROR_SUCCESS)
	{
		dwNameLen = MAX_NAME_LEN, dwDataLen = MAX_URL_LEN;
		dwRes = RegEnumValueW(hKeyUrls, dwIndex, wszName, &dwNameLen, NULL, NULL, (LPBYTE)wszData, &dwDataLen);
		if (dwRes == ERROR_SUCCESS)
		{
			UINT nSlot;
			swscanf(wszName, L"U%u", &nSlot);
			if (nSlot < HISTORY_URL_NUM)
			{
				gHistoryUrls[nSlot] = 1;
				++gHisUrlNum;
			}

			wszData[dwDataLen >> 1] = 0;
			urlInputBox->AddString(wszData);
			dwIndex++;
		}
	}

	RegCloseKey(hKeyUrls);

	gRewindHisUrl = ReadDwordValue(L"SOFTWARE\\iSee\\history", L"rewindurl");
}

void SaveHistoryUrl(LPCWSTR url, int nLen)
{
	DWORD dwDisposition;
	HKEY hKeyUrls;
	if (ERROR_SUCCESS != RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\iSee\\history\\urls", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyUrls, &dwDisposition))
		return;

	WCHAR wszName[MAX_NAME_LEN];
	if (gHisUrlNum < HISTORY_URL_NUM)
		swprintf(wszName, L"U%02u", gHisUrlNum++);
	else
	{
		swprintf(wszName, L"U%02u", gRewindHisUrl++);
		if (gRewindHisUrl >= HISTORY_URL_NUM)
			gRewindHisUrl = 0;
		WriteDwordValue(L"SOFTWARE\\iSee\\history", L"rewindurl", gRewindHisUrl);
	}

	RegSetValueExW(hKeyUrls, wszName, 0, REG_SZ, (LPBYTE)url, nLen * sizeof(WCHAR));

	RegCloseKey(hKeyUrls);
}

#ifdef _WIN32_WCE
static void AddFavoriteMenu(HMENU hMenuTools, UINT nPos, LPCWSTR wszUrl, int nUrlLen)
{
	HMENU hMenuFavor = GetSubMenu(hMenuTools, TOOL_MENU_FAVOR);
	if (NULL == hMenuFavor)
		return;
	if (nPos == INVALID_POS)
	{
		for (UINT i = 0; i < FAVORITE_URL_NUM; i++)
		{
			if (favorCommands[i] == 0)
			{
				nPos = i;
				break;
			}
		}
		if (nPos == INVALID_POS)
			return;
	}

	MENUITEMINFO mii;
	memset(&mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_TYPE;
	mii.fType = MFT_STRING;
	mii.dwItemData = 0;
	mii.wID = 0;
	WCHAR wszMenuStr[MAX_TITLE_LEN + 5]; // &0 + .. + 结束字符
	mii.dwTypeData = GetViewShotcut(nPos, wszUrl[nUrlLen + 1] ? (wszUrl + nUrlLen + 1) : wszUrl, MAX_TITLE_LEN, wszMenuStr, &mii.cch);
	SetMenuItemInfo(hMenuFavor, nPos, TRUE, &mii);
}

// 加入空白菜单
static void AddBlankFavorites(HMENU hMenuTools)
{
	HMENU hMenuFavor = GetSubMenu(hMenuTools, TOOL_MENU_FAVOR);
	if (NULL == hMenuFavor)
		return;

	WCHAR wszMenuStr[MAX_TITLE_LEN + 5];
	UINT nLen;
	for (UINT i = 1; i < FAVORITE_URL_NUM; i++)
	{
		AppendMenu(hMenuFavor, MF_STRING, IDM_GO_FAVORITE + i, GetViewShotcut(i, L"Blank", MAX_TITLE_LEN, wszMenuStr, &nLen));
	}
	// 设置第一个菜单项
	AddFavoriteMenu(hMenuTools, 0, L"Blank\0", BLANK_STR_LEN);
}

void ReadFavoriteUrls(HMENU hMenuTools)
{
	// 加入空菜单
	AddBlankFavorites(hMenuTools);

	// 读入Favorites
	DWORD dwDisposition;
	HKEY hKeyUrls;
	if (ERROR_SUCCESS != RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\iSee\\favorites", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyUrls, &dwDisposition))
		return;
	if (dwDisposition & REG_CREATED_NEW_KEY)
	{
		RegCloseKey(hKeyUrls);
		return;
	}

	DWORD dwIndex = 0;
	DWORD dwRes = ERROR_SUCCESS;
	WCHAR wszName[MAX_NAME_LEN];
	WCHAR wszData[MAX_URL_LEN * 2];
	DWORD dwNameLen, dwDataLen;
	while (dwRes == ERROR_SUCCESS)
	{
		dwNameLen = MAX_NAME_LEN, dwDataLen = MAX_URL_LEN << 1;
		dwRes = RegEnumValueW(hKeyUrls, dwIndex, wszName, &dwNameLen, NULL, NULL, (LPBYTE)wszData, &dwDataLen);
		if (dwRes == ERROR_SUCCESS)
		{
			UINT nSlot;
			swscanf(wszName, L"F%u", &nSlot);
			if (nSlot < FAVORITE_URL_NUM)
			{
				favorCommands[nSlot] = IDM_GO_FAVORITE + nSlot;
				AddFavoriteMenu(hMenuTools, nSlot, wszData, wcslen(wszData));
			}
			dwIndex++;
		}
	}

	RegCloseKey(hKeyUrls);

	gRewindFavorite = ReadDwordValue(L"SOFTWARE\\iSee", L"rewindfavorite");
}

void AddFavorite(HMENU hMenuTools)
{
	int nUrlLen = 0, nTitleLen = 0;
	LPCWSTR wszUrl = NULL, wszTitle = NULL;
	WebView *webView = WebView::currentView();
	if (webView)
	{
		wszUrl = webView->getUrl(nUrlLen);
		wszTitle = webView->getTitle(nTitleLen);
	}
	if (NULL == wszUrl)
		return;
	WCHAR wszRes[MAX_URL_LEN];
	LPWSTR wszTmp = ConvertUrl2Str(wszUrl, nUrlLen, wszRes);
	if ((nUrlLen + nTitleLen) >= MAX_URL_LEN * 2)
	{
		if (wszTmp != wszRes)
			free(wszTmp);
		return;
	}

	DWORD dwDisposition;
	HKEY hKeyUrls;
	if (ERROR_SUCCESS != RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\iSee\\favorites", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyUrls, &dwDisposition))
	{
		if (wszTmp != wszRes)
			free(wszTmp);
		return;
	}

	UINT nPos;
	for (nPos = 0; nPos < FAVORITE_URL_NUM; nPos++)
	{
		if (favorCommands[nPos] == 0)
			break;
	}

	WCHAR wszName[MAX_NAME_LEN];
	if (nPos < FAVORITE_URL_NUM)
		swprintf(wszName, L"F%02u", nPos);
	else
	{
		nPos = gRewindFavorite;
		swprintf(wszName, L"F%02u", gRewindFavorite++);
		if (gRewindFavorite >= FAVORITE_URL_NUM)
			gRewindFavorite = 0;
		WriteDwordValue(L"SOFTWARE\\iSee", L"rewindfavorite", gRewindFavorite);
	}

	// 设置数据
	DWORD dwDataLen = (nUrlLen + nTitleLen + 3) * sizeof(WCHAR);
	LPWSTR wszValue = (LPWSTR)malloc(dwDataLen);
	if (wszValue)
	{
		memcpy(wszValue, wszTmp, (nUrlLen + 1) * sizeof(WCHAR));
		if (nTitleLen > 0)
		{
			memcpy(wszValue + nUrlLen + 1, wszTitle, (nTitleLen + 1) * sizeof(WCHAR));
			wszValue[nUrlLen + nTitleLen + 2] = 0;
		}
		else
			wszValue[nUrlLen + 1] = 0;
		RegSetValueExW(hKeyUrls, wszName, 0, REG_MULTI_SZ, (LPBYTE)wszValue, dwDataLen);
		favorCommands[nPos] = IDM_GO_FAVORITE + nPos;
	}
	RegCloseKey(hKeyUrls);
	// 释放内存
	if (wszTmp != wszRes)
		free(wszTmp);

	// 加入到快捷菜单
	if (wszValue)
	{
		AddFavoriteMenu(hMenuTools, nPos, wszValue, nUrlLen);
		free(wszValue);
	}
}

static void LoadFavorites(HWND hUrls)
{
	// 读入Favorites
	DWORD dwDisposition;
	HKEY hKeyUrls;
	if (ERROR_SUCCESS != RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\iSee\\favorites", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyUrls, &dwDisposition))
		return;
	if (dwDisposition & REG_CREATED_NEW_KEY)
	{
		RegCloseKey(hKeyUrls);
		return;
	}

	DWORD dwIndex = 0;
	DWORD dwRes = ERROR_SUCCESS;
	WCHAR wszName[MAX_NAME_LEN];
	WCHAR wszData[MAX_URL_LEN];
	DWORD dwNameLen, dwDataLen;
	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
	while (dwRes == ERROR_SUCCESS)
	{
		dwNameLen = MAX_NAME_LEN, dwDataLen = MAX_URL_LEN;
		dwRes = RegEnumValueW(hKeyUrls, dwIndex, wszName, &dwNameLen, NULL, NULL, (LPBYTE)wszData, &dwDataLen);
		if (dwRes == ERROR_SUCCESS)
		{
			UINT nSlot;
			swscanf(wszName, L"F%u", &nSlot);
			wszData[dwDataLen >> 1] = 0;

			int nUrlLen = ::wcslen(wszData);
			lvi.mask = LVIF_PARAM | LVIF_TEXT;
			lvi.iItem = dwIndex;
			lvi.iSubItem = 0;
			lvi.pszText = wszData + nUrlLen + 1;
			lvi.cchTextMax = ::wcslen(lvi.pszText);
			lvi.lParam = nSlot;
			SendMessage(hUrls, LVM_INSERTITEM, 0, (LPARAM)&lvi);
			lvi.mask = LVIF_TEXT;
			lvi.iSubItem = 1;
			lvi.pszText = wszData;
			lvi.cchTextMax = nUrlLen;
			SendMessage(hUrls, LVM_SETITEM, 0, (LPARAM)&lvi);

			dwIndex++;
		}
	}

	RegCloseKey(hKeyUrls);
}

static void DeleteFavorites(HMENU hMenuTools, HWND hUrls)
{
	// 读入Favorites
	DWORD dwDisposition;
	HKEY hKeyUrls;
	if (ERROR_SUCCESS != RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\iSee\\favorites", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeyUrls, &dwDisposition))
		return;
	if (dwDisposition & REG_CREATED_NEW_KEY)
	{
		RegCloseKey(hKeyUrls);
		return;
	}

	int nPos = (int)SendMessage(hUrls, LVM_GETNEXTITEM, -1, MAKELPARAM(LVNI_SELECTED, 0));
	if (nPos == -1)
	{
		RegCloseKey(hKeyUrls);
		return;
	}
	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
	lvi.mask = LVIF_PARAM;
	do {
		lvi.iItem = nPos;
		if (SendMessage(hUrls, LVM_GETITEM, 0, (LPARAM)&lvi) && lvi.lParam < FAVORITE_URL_NUM)
		{
			WCHAR wszName[MAX_NAME_LEN];
			swprintf(wszName, L"F%02u", lvi.lParam);
			RegDeleteValue(hKeyUrls, wszName);
			favorCommands[lvi.lParam] = 0;
			// 删除菜单
			AddFavoriteMenu(hMenuTools, lvi.lParam, L"Blank\0", BLANK_STR_LEN);
		}
		SendMessage(hUrls, LVM_DELETEITEM, nPos, 0);
		nPos = (int)SendMessage(hUrls, LVM_GETNEXTITEM, nPos - 1, MAKELPARAM(LVNI_SELECTED, 0));
	} while (nPos >= 0);

	RegCloseKey(hKeyUrls);
}

static
INT_PTR CALLBACK FavoriteProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hUrls;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
#ifdef _WIN32_WCE
		SetWindowLong(hDlg, DWL_USER, (LONG)lParam);
#else
		SetWindowLongPtr(hDlg, DWL_USER, (LONG_PTR)lParam);
#endif
		SetWindowText(hDlg, GetConfigAttr(L"DIALOG", L"UI", L"FAVORITE"));
		SetDlgItemText(hDlg, IDC_DELETE, GetConfigAttr(L"DELETE", L"UI", L"FAVORITE"));
		SetDlgItemText(hDlg, IDCANCEL, GetConfigAttr(L"CLOSE", L"UI", L"FAVORITE"));
		hUrls = GetDlgItem(hDlg, IDC_FAVORITES);
		if (hUrls == NULL)
			break;
		// 设置抬头信息
		{
			LVCOLUMN lvc;
			lvc.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
			lvc.fmt = LVCFMT_LEFT;
			lvc.cx = 80;
			lvc.pszText = GetConfigAttr(L"TITLE", L"UI", L"FAVORITE");
			lvc.cchTextMax = ::wcslen(lvc.pszText);
			lvc.iSubItem = 0;
			lvc.iImage = 0;
			lvc.iOrder = 0;
			SendMessage(hUrls, LVM_INSERTCOLUMNW, 0, (LPARAM)&lvc);
			lvc.cx = 200;
			lvc.pszText = GetConfigAttr(L"URL", L"UI", L"FAVORITE");
			lvc.cchTextMax = ::wcslen(lvc.pszText);
			lvc.iSubItem = 1;
			SendMessage(hUrls, LVM_INSERTCOLUMNW, 1, (LPARAM)&lvc);
		}
		// 载入收藏家内容
		LoadFavorites(hUrls);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_DELETE)
		{	// 删除一条收藏
			HMENU hMenuTools = (HMENU)
#ifdef _WIN32_WCE
				GetWindowLong(hDlg, DWL_USER);
#else
				GetWindowLongPtr(hDlg, DWL_USER);
#endif
			DeleteFavorites(hMenuTools, GetDlgItem(hDlg, IDC_FAVORITES));
		}
		else if (LOWORD(wParam) == IDCANCEL)
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
void ManageFavorites(HMENU hMenuTools)
{
	DialogBoxParam(ghInstance, MAKEINTRESOURCE(IDD_FAVORITES), ghMainWnd, FavoriteProc, (LPARAM)hMenuTools);
}

bool GetFavoriteUrl(UINT nCmdId, LPWSTR wszUrl, UINT &nLen)
{
	if (nCmdId >= IDM_GO_FAVORITE && nCmdId <= (IDM_GO_FAVORITE + FAVORITE_URL_NUM))
	{
		UINT nPos = nCmdId - IDM_GO_FAVORITE;
		if (favorCommands[nPos] > 0)
		{
			HKEY hKey;
			if (ERROR_SUCCESS == RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\iSee\\favorites", 0, KEY_ALL_ACCESS, &hKey))
			{
				WCHAR wszName[MAX_NAME_LEN];
				swprintf(wszName, L"F%02u", nPos);
				DWORD dwType, dwDataLen = nLen << 1;
				if (ERROR_SUCCESS == RegQueryValueExW(hKey, wszName, NULL, &dwType, (LPBYTE)wszUrl, &dwDataLen))
				{
					nLen = dwDataLen >> 1;
					wszUrl[nLen] = 0;
					return true;
				}
			}
		}
	}
	return false;
}
#endif
