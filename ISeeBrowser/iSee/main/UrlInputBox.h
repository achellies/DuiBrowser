#ifndef _URL_INPUTBOX_H
#define _URL_INPUTBOX_H

#include <windows.h>
#include <windowsx.h>

#include <vector>

#include "iSeeDefs.h"

class UrlData
{
public:
	friend class UrlString;
	UrlData()
	{
		m_refCount = 0;
		m_urlData = NULL;
		m_urlLen = -1;
	}
	UrlData(LPCWSTR str, int len = -1);
	~UrlData() {}

	void addRef()
	{
		++m_refCount;
	}

	void releaseRef()
	{
		if (m_refCount > 0)
			--m_refCount;
		if (0 == m_refCount)
		{
			if (m_urlData)
				free(m_urlData);
			delete this;
		}
	}

	LPWSTR data() { return m_urlData; }
	int length() { return m_urlLen; }

private:
	int m_refCount;
	LPWSTR m_urlData;
	int m_urlLen;
};

class UrlString
{
public:
	UrlString() : m_urlData(NULL) {}
	UrlString(const UrlString &other)
	{
		m_urlData = other.m_urlData;
		m_urlData->addRef();
	}
	UrlString(LPCWSTR str, int len = -1)
	{
		m_urlData = new UrlData(str, len);
		if (m_urlData)
			m_urlData->addRef();
	}
	~UrlString()
	{
		if (m_urlData)
			m_urlData->releaseRef();
	}

	bool operator<(const UrlString& other) const
	{
		return (wcsicmp(m_urlData->data(), other.m_urlData->data()) < 0);
	}

	bool operator>(const UrlString& other) const
	{
		return (wcsicmp(m_urlData->data(), other.m_urlData->data()) > 0);
	}

	bool operator==(const UrlString& other) const
	{
		return (wcsicmp(m_urlData->data(), other.m_urlData->data()) == 0);
	}

	UrlString &operator=(const UrlString & other)
	{
		if (m_urlData)
			m_urlData->releaseRef();
		m_urlData = other.m_urlData;
		m_urlData->addRef();
		return *this;
	}

	LPCWSTR url() const { return m_urlData ? m_urlData->data() : NULL; }
	int length() const { return m_urlData ? m_urlData->length() : -1; }

private:
	UrlData *m_urlData;
};

class UrlInputBox
{
public:
	UrlInputBox(HINSTANCE hInstance, HWND hParent);
	~UrlInputBox();

	void Resize();
	void SetFocus();

	void AddString(LPCWSTR szUrl);
	int FindStringExact(int nStartAfter, LPCWSTR szUrl);
	int FindStringExact(int nStartAfter, const UrlString &us);
	void DeleteString(LPCWSTR szUrl);
	int GetCount() { return m_urlItems.size(); }

	void ShowUrlDropDown(BOOL bHide = FALSE);
	void DoPaintMessageLoop();
	void InvalidateAndScroll();

	int HitTest(POINT point);
	bool EnsureVisible(int item, bool bWait);
	
	void SetScroller();
	void SetProp();

	void ShowWindow(bool bShow);
	void Move(int x, int y, int w, int h);
	void Show(bool bShow);

	bool SelectItem(int nItem);
	bool GetItemText(int nItem, LPWSTR szUrl, int nMaxLen);
	int FindString(int nStartAfter, LPCWSTR szString, bool bDisplayOnly = false);
	int SelectString(LPCWSTR lpszString);
	
	int GetInputText(LPWSTR szUrl, int nMaxLen);
	void SetInputText(LPCWSTR szUrl, int nLen);

	void GetString(LPWSTR szUrl, int nMaxLen);
	void GetNextString(int nChar, LPWSTR szUrl, int nMaxLen);

	void OnLButtonDown(POINT point, UINT uFlags);
	void OnRButtonDown(POINT point, UINT uFlags);
	void OnMouseMove(POINT point, UINT uFlags);
	BOOL OnSetCursor(HWND hWnd, UINT nHitTest, UINT message);
	void OnSize(UINT nType, int cx, int cy);

	void OnShowWindow(BOOL bShow, UINT nStatus);
	void OnVScroll(UINT nSBCode, UINT nPos);
	void OnTimer(UINT nIDEvent);
#ifndef _WIN32_WCE
	void OnGetMinMaxInfo(MINMAXINFO * lpMMI);
#endif
	void DrawItem(HDC hdc, int nIdx, long width);
	void OnPaint(HDC hdc);

	void OnKillfocus();
	bool OnEditKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool HandleKey(UINT nChar, bool bFromChild);

	bool OnDropDown(HWND hWnd);
	void OnSelchange(HWND hWnd);

	std::vector<UrlString> m_urlItems;
	std::vector<UrlString> m_urlDisps;
	int m_topIndex;
	int m_currIndex;
	int m_visibleCount;
	int m_itemHeight;

	HWND m_hComboWnd;
	HWND m_hEditWnd;
	HWND m_hListWnd;
	HWND m_hVerBar;

	LONG_PTR m_defCBProc;
	LONG_PTR m_defEditProc;

private:
	int strcaseinstr(LPCWSTR str, LPCWSTR search);

	HWND m_hAfterWnd;
	RECT rcLastSize;
	RECT rcParentRect;
	UINT nIDInstall;
};

#endif // _URL_INPUTBOX_H
