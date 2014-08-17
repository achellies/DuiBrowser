/*
* Copyright (C) 2010, 2011 achellies <achellies@163.com>
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _POSTIL_MAKER_H
#define _POSTIL_MAKER_H
#include <windows.h>
#include <tchar.h>
#include <vector>
using namespace std;

typedef struct tagTrace {
	DWORD	dwWidth;
	DWORD	dwColor;
	DWORD	dwPress;
	DWORD	dwCount;
	vector<POINT>	points;
}Trace;

typedef struct tagTraceArray
{
	int Count()
	{
		return static_cast<int>(data.size());
	}

	void Clear()
	{
		data.clear();
	}

	BOOL IsEmpty()
	{
		return data.empty();
	}

	vector<Trace> data;
}TraceArray;

class CPostilMaker
{
public:
	// constructor
	CPostilMaker();

	// destructor
	~CPostilMaker();

	void SetInfo(HWND hHost, LPCTSTR lpszFile);

	BOOL Draw(HDC hDCDest);

	void Save();

	void AddPoint(POINT point, DWORD dwWidth, bool clear = false, COLORREF clr = RGB(0,0,0), DWORD dwPress = 0);
	void TraceStart(POINT point, DWORD dwWidth, bool clear = false, COLORREF clr = RGB(0,0,0), DWORD dwPress = 0);
	void TraceEnd(POINT point, DWORD dwWidth, bool clear = false, COLORREF clr = RGB(0,0,0), DWORD dwPress = 0);

	void SetZoom(DWORD dwFontSize, DWORD dwPageCounts);

	void ClearTrace();

	void ClearPageData(DWORD dwPage);

	BOOL GetPageData(DWORD dwPage);

protected:
	BOOL LoadFile();

	void SavePageData();

private:
	BOOL				m_bPostilExisted;
	BOOL				m_bPostilChanged;
	HWND				m_hHostWnd;
	DWORD				m_dwCurPage;
	DWORD				m_dwPageCount;
	DWORD				m_dwFontSize;
	TCHAR				m_szFile[MAX_PATH];
	TraceArray			m_currentTraceArray;
	Trace				m_currentTrace;
	Trace				m_clearTrace;
};

#endif // _POSTIL_MAKER_H