#include "UrlInputBox.h"

using namespace std;

#define URLWINDOW_CLASS		L"kUrlWindowClass"
#define LISTWINDOW_CLASS	L"kListWindowClass"
#define URLBAR_HEIGHT		24
#define MAX_DISP_NUM		6
#define GRIPPER_HEIGHT		20
#define MAX_SEARCH_NUM		40

#define ENAC_UPDATE			(WM_USER + 0x200)
#define TID_INSTALL			1

#define ScrollBarWidth()	(IsWindowVisible(m_hVerBar) ? GetSystemMetrics(SM_CYVSCROLL) : 0)

// 补全URL内容，如果不是以http://、ftp://、file://、https://开始，则补http://
// 注意最后的'/'要去掉，否则影响匹配
static bool FillUrl(LPCWSTR urlStr, int urlLen, LPWSTR fillStr)
{
	if ((urlLen >= 7 &&
		towlower(urlStr[0]) == L'h' &&
		towlower(urlStr[1]) == L't' &&
		towlower(urlStr[2]) == L't' &&
		towlower(urlStr[3]) == L'p' &&
		towlower(urlStr[4]) == L':' &&
		towlower(urlStr[5]) == L'/' &&
		towlower(urlStr[6]) == L'/') ||
		
		(urlLen >= 6 &&
		towlower(urlStr[0]) == L'f' &&
		towlower(urlStr[1]) == L't' &&
		towlower(urlStr[2]) == L'p' &&
		towlower(urlStr[3]) == L':' &&
		towlower(urlStr[4]) == L'/' &&
		towlower(urlStr[5]) == L'/') ||

		(urlLen >= 7 &&
		towlower(urlStr[0]) == L'f' &&
		towlower(urlStr[1]) == L'i' &&
		towlower(urlStr[2]) == L'l' &&
		towlower(urlStr[3]) == L'e' &&
		towlower(urlStr[4]) == L':' &&
		towlower(urlStr[5]) == L'/' &&
		towlower(urlStr[6]) == L'/') ||

		(urlLen >= 8 &&
		towlower(urlStr[0]) == L'h' &&
		towlower(urlStr[1]) == L't' &&
		towlower(urlStr[2]) == L't' &&
		towlower(urlStr[3]) == L'p' &&
		towlower(urlStr[4]) == L's' &&
		towlower(urlStr[5]) == L':' &&
		towlower(urlStr[6]) == L'/' &&
		towlower(urlStr[7]) == L'/'))
	{
		return (urlStr[urlLen - 1] == L'/');
	}
	else
	{
		wcscpy(fillStr, L"http://");
		return (urlStr[urlLen - 1] == L'/');
	}
}

UrlData::UrlData(LPCWSTR str, int len)
{
	m_refCount = 0;
	if (len <= 0)
		len = wcslen(str);

	WCHAR szFill[10] = {0};
	bool bTrimChar = FillUrl(str, len, szFill);
	int flen = wcslen(szFill);
	
	m_urlLen = len + flen + (bTrimChar ? -1 : 0);
	int msize = m_urlLen * sizeof(WCHAR);
	m_urlData = (LPWSTR)malloc(msize + sizeof(WCHAR));
	if (flen > 0)
	{
		flen = flen * sizeof(WCHAR);
		memcpy(m_urlData, szFill, flen);
	}
	memcpy((LPBYTE)m_urlData + flen, str, msize - flen);
	m_urlData[m_urlLen] = 0;
}

static POINT GetPointFromLPARAM(LPARAM lParam)
{
	POINT pt;
	pt.x = GET_X_LPARAM(lParam); 
	pt.y = GET_Y_LPARAM(lParam); 
	return pt;
}

static UrlInputBox *GetUrlContext(HWND hWnd, int nIndex);
static void SetUrlContext(HWND hWnd, int nIndex, UrlInputBox *uc);

static LRESULT CALLBACK 
ListWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK 
CBWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK 
EditWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static BOOL UrlRegisterClass(HINSTANCE hInstance)
{
	static bool registered = false;
	if (registered)
		return TRUE;
	registered = true;

	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = ListWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = sizeof(void *);
	wc.hInstance     = hInstance;
	wc.hIcon         = NULL;
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName  = 0;
	wc.lpszClassName = LISTWINDOW_CLASS;
	if (RegisterClass(&wc) > 0)
		return TRUE;
	return FALSE;
}

UrlInputBox::UrlInputBox(HINSTANCE hInstance, HWND hParent)
: m_urlItems(), m_urlDisps()
, m_topIndex(-1), m_currIndex(-1)
, m_visibleCount(0), m_itemHeight(16)
, m_hComboWnd(NULL), m_hEditWnd(NULL)
, m_hListWnd(NULL), m_hVerBar(NULL)
, m_defCBProc(NULL), m_defEditProc(NULL)
, nIDInstall(0)
{
	UrlRegisterClass(hInstance);
	m_hComboWnd = CreateWindow(L"COMBOBOX", 0,
		WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_AUTOHSCROLL | CBS_SORT | CBS_DROPDOWN | CBS_HASSTRINGS,
		0, 0, 0, 0,
		hParent,
		0,
		hInstance, 0);
	if (m_hComboWnd)
	{
		SetUrlContext(m_hComboWnd, GWL_USERDATA, this);
		m_defCBProc = GetWindowLong(m_hComboWnd, GWL_WNDPROC);
		SetWindowLong(m_hComboWnd, GWL_WNDPROC, (LONG_PTR)CBWndProc);

		/*
		// disabled by achellies
		COMBOBOXINFO cbi;
		memset(&cbi, 0, sizeof(COMBOBOXINFO));
		cbi.cbSize = sizeof(COMBOBOXINFO);
		SendMessage(m_hComboWnd, CB_GETCOMBOBOXINFO, 0, (LPARAM)&cbi);
		if (cbi.hwndItem)
		{
			m_hEditWnd = cbi.hwndItem;
			SetUrlContext(m_hEditWnd, GWL_USERDATA, this);
			m_defEditProc = GetWindowLong(m_hEditWnd, GWL_WNDPROC);
			SetWindowLong(m_hEditWnd, GWL_WNDPROC, (LONG_PTR)EditWndProc);
		}
		*/
	}
#ifdef _WIN32_WCE
	m_hListWnd = CreateWindow(
		LISTWINDOW_CLASS, 0,
		WS_CHILD | WS_BORDER, 
		0, 0, 0, 0,
		hParent,
		0,
		hInstance, this); // 初始化为不可见
	m_hAfterWnd = HWND_TOP;
#else
	m_hListWnd = CreateWindowEx(WS_EX_TOOLWINDOW,
		LISTWINDOW_CLASS, 0,
		WS_THICKFRAME | WS_CHILD | WS_BORDER | WS_CLIPSIBLINGS,
		0, 0, 0, 0,
		GetDesktopWindow(),
		0,
		hInstance, this); // 初始化为不可见
	m_hAfterWnd = HWND_TOPMOST;
#endif
	if (NULL == m_hListWnd)
		return;
	// 创建滚动条
	m_hVerBar = CreateWindow(L"SCROLLBAR", 0,
		WS_CHILD | WS_VISIBLE | SBS_VERT, 
		0, 0, GetSystemMetrics(SM_CYVSCROLL), 84,
		m_hListWnd,
		0,
		hInstance, 0); // 初始化为不可见
	SetScrollPos(m_hVerBar, SB_VERT, 0, FALSE);

	SetRectEmpty(&rcLastSize);
	SetRectEmpty(&rcParentRect);
}

UrlInputBox::~UrlInputBox()
{
	DestroyWindow(m_hComboWnd);
	m_urlDisps.clear();
	m_urlItems.clear();
}

void UrlInputBox::Resize()
{
	int nCount = GetCount();
	RECT rcCombo;
	GetWindowRect(m_hComboWnd, &rcCombo);
	MapWindowPoints(NULL, GetParent(m_hComboWnd), (LPPOINT)&rcCombo, 2);
	int h = SendMessage(m_hComboWnd, CB_GETITEMHEIGHT, 0, 0);
	if (nCount >= MAX_DISP_NUM)
		rcCombo.bottom = rcCombo.top + MAX_DISP_NUM * h + URLBAR_HEIGHT;
	else
		rcCombo.bottom = rcCombo.top + nCount * h + URLBAR_HEIGHT;
	::MoveWindow(m_hComboWnd, rcCombo.left, rcCombo.top, _rw(rcCombo), _rh(rcCombo), TRUE);
}

void UrlInputBox::SetFocus()
{
	::SetFocus(m_hComboWnd);
}

void UrlInputBox::AddString(LPCWSTR wszUrl)
{
	UrlString us(wszUrl);
	int nFind = -1;
	for (int nLoops = m_urlItems.size(), i = 0; i < nLoops; i++)
	{
		if (us < m_urlItems[i])
		{
			nFind = i;
			break;
		}
	}
	if (nFind < 0)
		m_urlItems.push_back(us);
	else
	{
		vector<UrlString>::iterator pos = m_urlItems.begin() + nFind;
		m_urlItems.insert(pos, us);
	}
	// 加入到ComboBox
	if (m_hComboWnd)
		SendMessage(m_hComboWnd, CB_ADDSTRING, 0, (LPARAM)wszUrl);
}

int UrlInputBox::FindStringExact(int nStartAfter, LPCWSTR wszUrl)
{
	UrlString us(wszUrl);
	return FindStringExact(nStartAfter, us);
}

int UrlInputBox::FindStringExact(int nStartAfter, const UrlString &us)
{
	if (m_urlItems.size() > 0 && nStartAfter >= -1 && nStartAfter < (int)m_urlItems.size())
	{
		int nPos = nStartAfter + 1;
		for (vector<UrlString>::iterator i = m_urlItems.begin() + nPos; i != m_urlItems.end(); i++, nPos++)
		{
			if (us == *i)
				return nPos;
		}
	}
	return -1;
}

void UrlInputBox::DeleteString(LPCWSTR wszUrl)
{
	int nPos = FindStringExact(-1, wszUrl);
	if (nPos >= 0)
	{
		m_urlItems.erase(m_urlItems.begin() + nPos);
	}
}

int UrlInputBox::strcaseinstr(LPCWSTR str, LPCWSTR search)
{
	int sl = wcslen(str);
	int fl = wcslen(search);
	int r = -1;
	if (fl == 0)
		r = 0;
	else if (fl <= sl)
	{
		int cnt = sl - fl, i;
		for (i = 0; i < cnt; i++)
		{
			if (wcsnicmp(str + i, search, fl) == 0)
				break;
		}
		if (i < cnt)
			r = i;
	}

	return r;
}

void UrlInputBox::ShowUrlDropDown(BOOL bHide)
{
	if (bHide)
		ShowWindow(false);
	else
	{
		WCHAR szInput[MAX_URL_LEN];
		::GetWindowText(m_hComboWnd, szInput, MAX_URL_LEN - 1);
		if (*szInput == 0)
		{
			m_urlDisps.clear();
			ShowWindow(false);
			return;
		}
		m_urlDisps.clear();
		for (vector<UrlString>::iterator i = m_urlItems.begin(); i != m_urlItems.end(); i++)
		{
			if (strcaseinstr((*i).url(), szInput) >= 0)
			{
				m_urlDisps.push_back(*i);
			}
		}
		int nCount = m_urlDisps.size() > MAX_DISP_NUM ? MAX_DISP_NUM : m_urlDisps.size();
		if (0 == nCount)
		{
			ShowWindow(false);
			return;
		}
		m_topIndex = 0;
		m_currIndex = -1;
		m_visibleCount = nCount;

		UINT dispH = m_itemHeight * nCount + GetSystemMetrics(SM_CYBORDER) * 2;
		RECT rcWnd;
		::GetClientRect(m_hComboWnd, &rcWnd);
		rcWnd.top = rcWnd.bottom;
		ShowWindow(true);
		::SetWindowPos(m_hListWnd, m_hAfterWnd, rcWnd.left, rcWnd.top, _rw(rcWnd) + GetSystemMetrics(SM_CXBORDER) * 2, dispH, 0);
		::UpdateWindow(m_hListWnd);
	}
}

void UrlInputBox::DoPaintMessageLoop()
{
	MSG msg;
	while(::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

void UrlInputBox::InvalidateAndScroll()
{
	SetScrollPos(m_hVerBar, SB_VERT, m_topIndex, true);
	InvalidateRect(m_hListWnd, NULL, FALSE);
	DoPaintMessageLoop();
}

int UrlInputBox::HitTest(POINT point)
{
	RECT rcWnd, rcItem;
	GetClientRect(m_hListWnd, &rcWnd);
	long width = _rw(rcWnd) - ScrollBarWidth();
	int nCount = m_urlDisps.size();

	for(int i = m_topIndex, k = 0; i < nCount; i++, k++)
	{
		SetRect(&rcItem, 2, k * m_itemHeight, width, (k + 1) * m_itemHeight);
		if(PtInRect(&rcItem, point))
			return (m_currIndex = (k + m_topIndex));
	}

	return -1;
}

bool UrlInputBox::EnsureVisible(int item, bool bWait)
{
	if(item > m_topIndex && item < m_topIndex + m_visibleCount)
		return false; // ist visible

	int nItemCount = m_urlDisps.size();
	if(item > m_topIndex)	// scroll down
	{
		long len = item;
		for(int i = m_topIndex; i < len; i++)
		{
			if(i >= nItemCount - m_visibleCount)
				break;
			if(i >= nItemCount - m_visibleCount || i + m_visibleCount > item)
			{
				break;
			}

			m_topIndex++;

			if(bWait)
			{
				InvalidateAndScroll();
				Sleep(10);
				DoPaintMessageLoop();
			}
		}
		InvalidateAndScroll();
		return true;
	}
	else if(item < m_topIndex)	// scroll up
	{
		while(item < m_topIndex)
		{
			if(m_topIndex > 0)
				m_topIndex--;
			else
			{
				break;
			}

			if(bWait)
			{
				InvalidateAndScroll();
				Sleep(10);
				DoPaintMessageLoop();
			}
		}

		InvalidateAndScroll();
		return true;
	}

	return false;
}

void UrlInputBox::OnLButtonDown(POINT point, UINT uFlags)
{
	int nSel = HitTest(point);

	if(nSel >= 0)
	{
		if(!EnsureVisible(nSel, true))
			InvalidateRect(m_hListWnd, NULL, FALSE);
		SendMessage(m_hComboWnd, ENAC_UPDATE, WM_KEYDOWN, 0);
		DoPaintMessageLoop();
		Sleep(100);
		ShowWindow(false);
	}
	else
	{
		RECT rc;
		GetClientRect(m_hListWnd, &rc);
		if (!PtInRect(&rc, point))
		{
			GetClientRect(m_hEditWnd, &rc);
			MapWindowPoints(m_hListWnd, m_hEditWnd, &point, 1);
			if (PtInRect(&rc, point))
			{	// 重新定向鼠标到编辑框
				SendMessage(m_hEditWnd, WM_LBUTTONDOWN, uFlags, MAKELONG(point.x, point.y));
				// 重新设置鼠标窗口到ListWnd
				SetCapture(m_hListWnd);
			}
			else
			{
				ShowWindow(false);
				// 重新定向鼠标事件到ComboBox
				GetClientRect(m_hComboWnd, &rc);
				MapWindowPoints(m_hEditWnd, m_hComboWnd, &point, 1);
				if (PtInRect(&rc, point))
					SendMessage(m_hComboWnd, WM_LBUTTONDOWN, uFlags, MAKELONG(point.x, point.y));
			}
		}
	}
}

void UrlInputBox::OnRButtonDown(POINT point, UINT) 
{
	ShowWindow(false);
}

void UrlInputBox::OnMouseMove(POINT point, UINT) 
{
	int nSel = HitTest(point);
	if(nSel >= 0)
	{
		InvalidateRect(m_hListWnd, NULL, FALSE);
	}
}

BOOL UrlInputBox::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message)
{
	RECT rcWnd;
	GetClientRect(m_hListWnd, &rcWnd);
	POINT ptCursor;

	rcWnd.left = rcWnd.right - GetSystemMetrics(SM_CYVSCROLL);
	rcWnd.top = rcWnd.bottom - GetSystemMetrics(SM_CXVSCROLL);

	GetCursorPos(&ptCursor);
	ScreenToClient(m_hListWnd, &ptCursor);

	if(PtInRect(&rcWnd, ptCursor))
	{
		return FALSE;
	}

	::SetCursor(LoadCursor(NULL, IDC_ARROW));
	return TRUE;		
}

void UrlInputBox::SetScroller()
{
	RECT rcWnd, rcBar;
	GetClientRect(m_hListWnd, &rcWnd);
	CopyRect(&rcBar, &rcWnd);
	rcBar.top = -1;
	rcBar.left = _rw(rcWnd) - GetSystemMetrics(SM_CYVSCROLL);
	rcBar.bottom -= GetSystemMetrics(SM_CYHSCROLL);
	::MoveWindow(m_hVerBar, rcBar.left, rcBar.top, rcBar.right - rcBar.left, rcBar.bottom - rcBar.top, true);

	SetScrollPos(m_hVerBar, SB_VERT, m_topIndex, true);
}

void UrlInputBox::SetProp()
{
	int nItemCount = m_urlDisps.size();
	if(nItemCount == 0)
		return;

	RECT rcWnd;
	GetClientRect(m_hListWnd, &rcWnd);

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask =  SIF_PAGE | SIF_RANGE;
	si.nMin = 0;
	si.nMax = nItemCount - 1;
	si.nTrackPos = 2;
	m_visibleCount = si.nPage = _rh(rcWnd) / m_itemHeight;
	SetScrollRange(m_hVerBar, SB_VERT, 0, nItemCount - 1, TRUE);
	SetScrollInfo(m_hVerBar, SB_VERT, &si, TRUE);

	if(m_visibleCount > nItemCount - 1)
		::ShowWindow(m_hVerBar, SW_HIDE);
	else
		::ShowWindow(m_hVerBar, SW_SHOW);

	if(m_topIndex + m_visibleCount > nItemCount)
	{
		m_topIndex = nItemCount - m_visibleCount;
		if(m_topIndex < 0)
			m_topIndex = 0;
		SetScrollPos(m_hVerBar, SB_VERT, m_topIndex, true);
	}
}

void UrlInputBox::ShowWindow(bool bShow)
{
	if (bShow)
	{
		SetCapture(m_hListWnd);
		::ShowWindow(m_hListWnd, SW_SHOW);
	}
	else
	{
		if (GetCapture() == m_hListWnd)
			ReleaseCapture();
		::ShowWindow(m_hListWnd, SW_HIDE);
	}
}

void UrlInputBox::Move(int x, int y, int w, int h)
{
	RECT rcWnd;
	::GetClientRect(m_hComboWnd, &rcWnd);
	if (w == -1)
		w = _rw(rcWnd);
	if (h == -1)
	{
		int ih = SendMessage(m_hComboWnd, CB_GETITEMHEIGHT, 0, 0);
		if (ih <= 0) ih = m_itemHeight;
		h = MAX_DISP_NUM * ih + URLBAR_HEIGHT;
	}
	::MoveWindow(m_hComboWnd, x, y, w, h, TRUE);
	if (IsWindowVisible(m_hListWnd))
	{
		int nCount = m_urlDisps.size() > MAX_DISP_NUM ? MAX_DISP_NUM : m_urlDisps.size();
		if (0 == nCount)
		{
			ShowWindow(false);
			return;
		}
		UINT dispH = m_itemHeight * nCount + GetSystemMetrics(SM_CYBORDER) * 2;
		::GetClientRect(m_hComboWnd, &rcWnd);
		rcWnd.top = rcWnd.bottom;
		::SetWindowPos(m_hListWnd, m_hAfterWnd, rcWnd.left, rcWnd.top, _rw(rcWnd) + GetSystemMetrics(SM_CXBORDER) * 2, dispH, 0);
		ShowWindow(true);
		::UpdateWindow(m_hListWnd);
	}
}

void UrlInputBox::Show(bool bShow)
{
	if (bShow)
		::ShowWindow(m_hComboWnd, SW_SHOW);
	else
	{
		ShowWindow(false);
		::ShowWindow(m_hComboWnd, SW_HIDE);
	}
}

bool UrlInputBox::SelectItem(int nItem)
{
	if(nItem > (int)m_urlDisps.size())
		return false;

	if(nItem == -1)
	{
		EnsureVisible(m_currIndex, false);
		InvalidateRect(m_hListWnd, NULL, FALSE);
		return false;
	}

	m_currIndex = nItem;

	if(!EnsureVisible(nItem, true))
		InvalidateRect(m_hListWnd, NULL, FALSE);

	return true;
}

bool UrlInputBox::GetItemText(int nItem, LPWSTR wszUrl, int nMaxLen)
{
	if(nItem < 0 || nItem >= m_urlItems.size())
		return false;
	const UrlString &us = *(m_urlItems.begin() + nItem);
	wcsncpy(wszUrl, us.url(), nMaxLen - 1);
	wszUrl[nMaxLen - 1] = 0;
	return true;
}

int UrlInputBox::FindString(int nStartAfter, LPCWSTR szString, bool bDisplayOnly)
{
	int nAktCount = m_urlDisps.size();

	if(!bDisplayOnly)
	{
		if(!m_hEditWnd || !szString || *szString == 0)
		{
			ShowWindow(false);
			return -1;
		}

		if(nStartAfter < -1 || nStartAfter > m_urlItems.size())
		{
			ShowWindow(false);
			return -1;
		}

		m_urlDisps.clear();
		if (m_urlItems.size() > 0)
		{
			for (vector<UrlString>::iterator i = m_urlItems.begin() + nStartAfter + 1; i != m_urlItems.end(); i++)
			{
				if (strcaseinstr(szString, (*i).url()) >= 0)
				{
					m_urlDisps.push_back(*i);
				}
			}
		}
	}

	int nCount = m_urlDisps.size() > MAX_DISP_NUM ? MAX_DISP_NUM : m_urlDisps.size();
	if (0 == nCount)
	{
		ShowWindow(false);
		return -1;
	}
	m_topIndex = 0;
	m_currIndex = -1;
	m_visibleCount = nCount;

	UINT dispH = m_itemHeight * nCount + GetSystemMetrics(SM_CYBORDER) * 2;
	RECT rcWnd;
	::GetClientRect(m_hComboWnd, &rcWnd);
	rcWnd.top = rcWnd.bottom;
	::SetWindowPos(m_hListWnd, m_hAfterWnd, rcWnd.left, rcWnd.top, _rw(rcWnd) + GetSystemMetrics(SM_CXBORDER) * 2, dispH, 0);
	ShowWindow(true);
	::UpdateWindow(m_hListWnd);

	return 0;
}

int UrlInputBox::SelectString(LPCWSTR lpszString)
{
	int nItem = FindString(-1, lpszString);
	SelectItem(nItem);
	return nItem;
}

void UrlInputBox::OnSize(UINT nType, int cx, int cy) 
{
	SetScroller();
	SetProp();

	if(!IsRectEmpty(&rcLastSize))
		GetWindowRect(m_hListWnd, &rcLastSize);
}

void UrlInputBox::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	if(bShow)
	{
		nIDInstall = SetTimer(m_hListWnd, TID_INSTALL, 200, NULL);
		::GetWindowRect(GetParent(m_hComboWnd), &rcParentRect);
	}
	else
	{
		if(nIDInstall)
			KillTimer(m_hListWnd, TID_INSTALL);
		nIDInstall = 0;
		m_currIndex = -1;
		m_topIndex = 0;
	}

	::SetCursor(LoadCursor(NULL, IDC_ARROW));
}

int UrlInputBox::GetInputText(LPWSTR wszUrl, int nMaxLen)
{
	int nLen = ::GetWindowText(m_hComboWnd, wszUrl, nMaxLen - 1);
	wszUrl[nMaxLen - 1] = 0;
	return nLen;
}

LPWSTR ConvertUrl2Str(LPCWSTR wszUrl, int &nLen, LPWSTR wszRes)
{
	LPWSTR wszTmp = wszRes;
	if (wszUrl == NULL || nLen == 0)
		return NULL;
	if (nLen >= MAX_URL_LEN)
	{
		wszTmp = (WCHAR *)malloc(sizeof(WCHAR) * (nLen + 1));
		if (NULL == wszTmp)
			return NULL;
	}

	LPCWSTR pr = wszUrl;
	LPWSTR pw = wszTmp;
	while (*pr && nLen > 0)
	{
		if (*pr == L'%')
		{
			int c;
			WCHAR esc[3];
			esc[0] = pr[1];
			esc[1] = pr[2];
			esc[2] = 0;
			swscanf(esc, L"%x", &c);
			*pw++ = (WCHAR)c;
			pr += 3;
			nLen -= 3;
		}
		else if (*pr == '+')
		{
			*pw = L' ';
			pr++;
			nLen -= 1;
		}
		else
		{
			*pw++ = *pr++;
			nLen -= 1;
		}
	}
	*pw = 0;

	nLen = pw - wszTmp;
	return wszTmp;
}

void UrlInputBox::SetInputText(LPCWSTR wszUrl, int nLen)
{
	WCHAR str[MAX_URL_LEN];
	LPWSTR wszTmp = ConvertUrl2Str(wszUrl, nLen, str);
	if (wszTmp == NULL)
		return;

	::SetWindowText(m_hComboWnd, wszTmp);
	::SendMessage(m_hComboWnd, CB_SETEDITSEL, 0, MAKELPARAM(0, -1));

	if (wszTmp != str)
		free(wszTmp);
}

void UrlInputBox::GetString(LPWSTR wszUrl, int nMaxLen)
{
	int nItem = m_urlDisps.size();

	if(nItem <= 0)
	{
		*wszUrl = 0;
		return;
	}
	else if(nItem <= m_currIndex || m_currIndex == -1)
		nItem = 0;
	else
		nItem = m_currIndex;

	const UrlString &us = *(m_urlDisps.begin() + nItem);
	wcsncpy(wszUrl, us.url(), nMaxLen - 1);
	wszUrl[nMaxLen - 1] = 0;
}

void UrlInputBox::GetNextString(int nChar, LPWSTR wszUrl, int nMaxLen)
{
	switch(nChar)
	{
	case VK_DOWN:
		m_currIndex++;
		break;

	case VK_UP:
		m_currIndex--;
		break;

	case VK_PRIOR:
		m_currIndex -= m_visibleCount;
		if(m_currIndex < 0)
			m_currIndex = 0;
		break;

	case VK_NEXT:
		m_currIndex += m_visibleCount;
		if(m_currIndex >= m_urlDisps.size() - 1)
			m_currIndex = m_urlDisps.size() - 1;
		break;

	case VK_HOME:
		m_currIndex = 0;
		break;

	case VK_END:
		m_currIndex = m_urlDisps.size() - 1;
		break;
	}

	if(m_currIndex < 0)
		m_currIndex = m_urlDisps.size() - 1;

	if(m_currIndex >= m_urlDisps.size())
		m_currIndex = 0;

	if(EnsureVisible(m_currIndex, (m_urlDisps.size() > MAX_SEARCH_NUM) ? false : true))
		InvalidateAndScroll();

	return GetString(wszUrl, nMaxLen);
}

void UrlInputBox::OnVScroll(UINT nSBCode, UINT nPos)
{
	int oldlTopIndex = m_topIndex;

	switch(nSBCode)
	{
	case SB_ENDSCROLL:
		break;

	case SB_PAGEUP:
		m_topIndex -= m_visibleCount;
		if(m_topIndex < 0)
			m_topIndex = 0;
		break;

	case SB_PAGEDOWN:
		m_topIndex += m_visibleCount;
		if(m_topIndex >= m_urlDisps.size() - m_visibleCount)
			m_topIndex = m_urlDisps.size() - m_visibleCount;
		break;

	case SB_LINEUP:
		m_topIndex--;
		if(m_topIndex < 0)
			m_topIndex = 0;
		break;

	case SB_LINEDOWN:
		m_topIndex++;
		if(m_topIndex >= m_urlDisps.size() - m_visibleCount)
			m_topIndex = m_topIndex - m_visibleCount;
		break;

	case SB_THUMBTRACK:
		m_topIndex = nPos;
		break;
	}

	SetScrollPos(m_hVerBar, SB_VERT, m_topIndex, TRUE);

	if(oldlTopIndex != m_topIndex)
		InvalidateRect(m_hListWnd, NULL, FALSE);
}

void UrlInputBox::OnTimer(UINT nIDEvent) 
{
	RECT rcWnd;
	::GetWindowRect(GetParent(m_hComboWnd), &rcWnd);
	if(!EqualRect(&rcWnd, &rcParentRect))
		ShowWindow(false);
}

#ifndef _WIN32_WCE
void UrlInputBox::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	int nCount = m_urlDisps.size();

	long lMinY1 = GetSystemMetrics(SM_CYHSCROLL) * 2 + GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CXHTHUMB);
	long lMinY2 = nCount * m_itemHeight + (GetSystemMetrics(SM_CYBORDER) * 2);

	if(m_visibleCount > nCount - 1 && lMinY2 < lMinY1)
		lpMMI->ptMinTrackSize.y = lMinY2;
	else
		lpMMI->ptMinTrackSize.y = lMinY1;

	lpMMI->ptMinTrackSize.x = GetSystemMetrics(SM_CXHSCROLL) * 4;

	if (m_hComboWnd) 
	{
		RECT rc;
		GetWindowRect(m_hComboWnd, &rc);
		lpMMI->ptMinTrackSize.x = _rw(rc);
	}
}
#endif

static UrlInputBox *GetUrlContext(HWND hWnd, int nIndex)
{
	UrlInputBox *uc = (UrlInputBox *)
#ifdef _WIN32_WCE
		GetWindowLong(hWnd, nIndex);
#else
		GetWindowLongPtr(hWnd, nIndex);
#endif
	return uc;
}

static void SetUrlContext(HWND hWnd, int nIndex, UrlInputBox *uc)
{
#ifdef _WIN32_WCE
	SetWindowLong(hWnd, nIndex, (LONG)uc);
#else
	SetWindowLongPtr(hWnd, nIndex, (LONG_PTR)uc);
#endif
}

static int GetDrawLen(LPCWSTR szDraw, long width)
{
	return wcslen(szDraw);
}

void UrlInputBox::DrawItem(HDC hdc, int nIdx, long width)
{
	RECT rcLabel;
	SetRect(&rcLabel, 2, nIdx * m_itemHeight, width, (nIdx + 1) * m_itemHeight);
	if(nIdx == m_currIndex)
	{
		rcLabel.left = 0;
		FillRect(hdc, &rcLabel, GetSysColorBrush(COLOR_HIGHLIGHT));
		SetTextColor(hdc, ::GetSysColor(COLOR_HIGHLIGHTTEXT));
		rcLabel.left = 2;
	}
	else
		SetTextColor(hdc, ::GetSysColor(COLOR_WINDOWTEXT));

	LPCWSTR szDraw = (*(m_urlDisps.begin() + nIdx)).url();
	DrawText(hdc, szDraw, GetDrawLen(szDraw, width), &rcLabel, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
}

void UrlInputBox::OnPaint(HDC hdc)
{
	HDC dcMem;
	HBITMAP hBmp;
	RECT rcWnd, rcBar;

	GetClientRect(m_hListWnd, &rcWnd);
	CopyRect(&rcBar, &rcWnd);

	rcBar.left = rcBar.right - GetSystemMetrics(SM_CXHSCROLL);
	rcBar.top = rcBar.bottom - GetSystemMetrics(SM_CYVSCROLL);

	hBmp = CreateCompatibleBitmap(hdc, _rw(rcWnd), _rh(rcWnd));
	dcMem = CreateCompatibleDC(hdc);
	SelectObject(dcMem, hBmp);

	FillRect(dcMem, &rcWnd, GetSysColorBrush(COLOR_WINDOW));
	SelectObject(dcMem, GetStockObject(SYSTEM_FONT));
	SetBkMode(dcMem, TRANSPARENT);

	int width = _rw(rcWnd) - ScrollBarWidth();
	for (int i = m_topIndex, k = 0; k < m_visibleCount; i++, k++)
	{
		DrawItem(dcMem, i, width);
	}

	if (IsWindowVisible(m_hVerBar))
		FillRect(dcMem, &rcBar, GetSysColorBrush(COLOR_BTNFACE));

	HPEN hPen1 = CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOW));
	HPEN hPen2 = CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNFACE));
	HPEN hPen3 = CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

	width = GetSystemMetrics(SM_CXHSCROLL);
	// gripper
	for (int i = 0, a = 1; i < width; i++, a++)
	{
		if(a == 1)
			SelectObject(dcMem, hPen1);
		if(a == 2)
			SelectObject(dcMem, hPen2);
		if(a == 3)
			SelectObject(dcMem, hPen3);
		if(a > 3)
			a = 0;
		MoveToEx(dcMem, rcBar.left + i - 1, rcWnd.bottom, NULL);
		LineTo(dcMem, rcBar.right, rcBar.top + i - 1);
	}

	DeleteObject(hPen1);
	DeleteObject(hPen2);
	DeleteObject(hPen3);
	
	BitBlt(hdc, 0, 0, _rw(rcWnd), _rh(rcWnd), dcMem, 0, 0, SRCCOPY);

	DeleteDC(dcMem);
	DeleteObject(hBmp);
}

LRESULT CALLBACK ListWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		{
			LPCREATESTRUCT cs = (LPCREATESTRUCT)lParam;
			SetUrlContext(hWnd, 0, (UrlInputBox *)cs->lpCreateParams);
		}
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			GetUrlContext(hWnd, 0)->ShowUrlDropDown(TRUE);
		}
		break;

	case WM_LBUTTONDOWN:
		GetUrlContext(hWnd, 0)->OnLButtonDown(GetPointFromLPARAM(lParam), (UINT)wParam);
		break;
	case WM_RBUTTONDOWN:
		GetUrlContext(hWnd, 0)->OnRButtonDown(GetPointFromLPARAM(lParam), (UINT)wParam);
		break;
	case WM_MOUSEMOVE:
		GetUrlContext(hWnd, 0)->OnMouseMove(GetPointFromLPARAM(lParam), (UINT)wParam);
		break;

	case WM_TIMER:
		GetUrlContext(hWnd, 0)->OnTimer((UINT)wParam);
		break;
#ifndef _WIN32_WCE
	case WM_GETMINMAXINFO:
		if (UrlInputBox *uc = GetUrlContext(hWnd, 0))
			uc->OnGetMinMaxInfo((MINMAXINFO *)lParam);
		break;
#endif

	case WM_SETCURSOR:
		GetUrlContext(hWnd, 0)->OnSetCursor((HWND)wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_SETFOCUS:
		break;

	case WM_VSCROLL:
		GetUrlContext(hWnd, 0)->OnVScroll(LOWORD(wParam), HIWORD(wParam));
		break;

	case WM_SHOWWINDOW:
		GetUrlContext(hWnd, 0)->OnShowWindow((BOOL)wParam, (UINT)lParam);
		break;

	case WM_SIZE:
		GetUrlContext(hWnd, 0)->OnSize((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetUrlContext(hWnd, 0)->OnPaint(hdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		{
			UrlInputBox *uc = GetUrlContext(hWnd, 0);
			if (uc)
			{
				DestroyWindow(uc->m_hVerBar);
				uc->m_hVerBar = NULL;
			}
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 使用在ComboBox&EditBox的事件中
void UrlInputBox::OnKillfocus() 
{
	ShowWindow(false);
}

bool UrlInputBox::OnEditKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return HandleKey(nChar, true);
}

bool UrlInputBox::HandleKey(UINT nChar, bool bFromChild)
{
	if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		ShowWindow(false);
		return true;
	}

	if (nChar == VK_DOWN || nChar == VK_UP 
		|| nChar == VK_PRIOR || nChar == VK_NEXT
		|| nChar == VK_HOME || nChar == VK_END) 
	{
		if(!IsWindowVisible(m_hListWnd))
			return false;
		if (m_currIndex == -1) // 未选中任何项目
		{
			if (nChar == VK_UP)
				return true;
			else if (nChar != VK_DOWN)
				return false;
		}

		WCHAR szEditText[MAX_URL_LEN];
		if (bFromChild)
			GetNextString(nChar, szEditText, MAX_URL_LEN);
		else
			GetString(szEditText, MAX_URL_LEN);
		szEditText[MAX_URL_LEN - 1] = 0;
		SelectItem(-1);

		SetWindowText(m_hComboWnd, szEditText);
		SendMessage(m_hComboWnd, CB_SETEDITSEL, 0, MAKELONG(0, -1));
		SendMessage(GetParent(m_hComboWnd), ENAC_UPDATE, EN_UPDATE, 0);
		return true;
	}
	return false;
}

bool UrlInputBox::OnDropDown(HWND hWnd)
{
	if (hWnd == m_hComboWnd)
	{
		ShowWindow(false);
		return true;
	}
	return false;
}

void UrlInputBox::OnSelchange(HWND hWnd)
{
	if (hWnd != m_hComboWnd)
		return;
	int nIndex = SendMessage(m_hComboWnd, CB_GETCURSEL, 0, 0);
	if (nIndex == CB_ERR)
		return;
	
	int nLen = SendMessage(m_hComboWnd, CB_GETLBTEXTLEN, nIndex, 0);
	if (nLen == CB_ERR)
		return;
	
	WCHAR wszUrl[MAX_URL_LEN], *str = wszUrl;
	if (nLen >= MAX_URL_LEN)
	{
		str = (WCHAR *)malloc((nLen + 1) * sizeof(WCHAR));
		if (str == NULL)
			return;
	}

	nLen = SendMessage(m_hComboWnd, CB_GETLBTEXT, nIndex, (LPARAM)str);
	if (nLen != CB_ERR)
		SendMessage(GetParent(m_hComboWnd), WM_LOAD_URL, nLen, (LPARAM)str);

	if (str != wszUrl)
		free(str);
}

LRESULT CALLBACK 
CBWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	UrlInputBox *uc = GetUrlContext(hWnd, GWL_USERDATA);
	if (NULL == uc)
		return 0;

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch(wmEvent)
		{
		case EN_UPDATE:
			GetUrlContext(hWnd, GWL_USERDATA)->ShowUrlDropDown();
			break;
		case EN_KILLFOCUS:
			GetUrlContext(hWnd, GWL_USERDATA)->OnKillfocus();
			break;
		default:
			break;
		}
		break;

	case ENAC_UPDATE:
		if (wParam == WM_KEYDOWN)
		{
			GetUrlContext(hWnd, GWL_USERDATA)->HandleKey(VK_DOWN, false);
		}
		return 0;

	case WM_DESTROY:
		if (uc->m_hComboWnd)
			DestroyWindow(uc->m_hComboWnd);
		if (uc->m_hListWnd)
			DestroyWindow(uc->m_hListWnd);
		break;
	}
	return (LRESULT)CallWindowProc((WNDPROC)uc->m_defCBProc, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK
EditWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UrlInputBox *uc = GetUrlContext(hWnd, GWL_USERDATA);
	if (NULL == uc)
		return 0;

	switch (message)
	{
	case WM_CHAR:
		if (wParam == 13)
		{	// Enter Key
			WCHAR strPtr[MAX_URL_LEN];
			*((LPWORD)strPtr) = MAX_URL_LEN;
			int strLen = SendMessage(hWnd, EM_GETLINE, 0, (LPARAM)strPtr);
			strPtr[strLen] = 0;
			SendMessage(GetParent(uc->m_hComboWnd), WM_LOAD_URL, strLen, (LPARAM)strPtr);
			return 0;
		}
		break;
	case WM_KEYDOWN:
		if (uc->OnEditKeyDown((UINT)wParam, LOWORD(lParam), HIWORD(lParam)))
			return 0;
		break;
	default:
		break;
	}
	return (LRESULT)CallWindowProc((WNDPROC)uc->m_defEditProc, hWnd, message, wParam, lParam);
}
