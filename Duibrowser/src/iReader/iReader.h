/*
* Copyright (C) 2010 Patrick Gansterer <paroga@paroga.com>
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

#ifndef _IREADER_H_
#define _IREADER_H_

#include <tchar.h>
#include "auto_fileMapping.h"
//#include "strsharingmanager.h"

__declspec(selectany) LPCTSTR lpszIReaderWindowClassName = _T("iReaderWindowClass");

// this string is used to register window message to communcicate with the host window
#define IREADER_INTERPROC_MSG	_T("HW_INTERPROCESS_MESSAGE")

// this string is used to create the shared file mapping
#define IREADER_DATA			_T("iReader_Data")

// this string is used to create a event indicated the layout is finished
#define IREADER_LAYOUT_EVENT	_T("iReader_Layout_Event")

#define IREADER_SYNC_DATA		_T("iReader_Sync")

#define IREADER_MSG_BASE		(0x80)
#define IREADER_QUIT			(IREADER_MSG_BASE + 1)
#define IREADER_OPEN			(IREADER_MSG_BASE + 2)
#define IREADER_DIVIDEPAGES		(IREADER_MSG_BASE + 3)
#define IREADER_NAVIGATE		(IREADER_MSG_BASE + 4)
#define IREADER_ZOOM			(IREADER_MSG_BASE + 5)
#define IREADER_PAGENUMBYOFFSET	(IREADER_MSG_BASE + 6)
#define IREADER_CUROFFSET		(IREADER_MSG_BASE + 7)

// when the iReader.exe has the input focus, the iReader will post the keyboard event to the Mebook.exe
#define IREADER_KEYEVENT		(IREADER_MSG_BASE + 8)

// when we under the translate mode(m_bTranslate is true), when the end-user select some text, 
// iReader.exe will post this message to let the Mebook.exe translate
#define IREADER_TRANSLATE		(IREADER_MSG_BASE + 9)

// when we under the excerpt mode(m_bExcerpt is true), when the end-user select some text, 
// iReader.exe will post this message to let the Mebook.exe know
#define IREADER_EXCERPT			(IREADER_MSG_BASE + 10)

// when the Mebook.exe excute TTS function, it will post this message to let the iReader.exe prepare the text
#define IREADER_TTS				(IREADER_MSG_BASE + 11)

#define IREADER_CLEARSELECTION	(IREADER_MSG_BASE + 12)

// tell the iReader.exe to do searching task,
// LPARAM 0: start a new searching task
// LPARAM 1: get the previous matched page
// LPARAM 2: get the next matched page
// LPARAM 3: get the first matched page
// LPARAM 4: get the last matched page
#define IREADER_SEARCH			(IREADER_MSG_BASE + 13)

// tell the iReader.exe to clear the cached data
#define IREADER_CLEAR_CACHE		(IREADER_MSG_BASE + 14)

// tell the host that some exceptions occur 
#define IREADER_EXCEPTION_RAISED (IREADER_MSG_BASE + 15)

// tell the host that some exceptions occur 
#define IREADER_CLEAR_POSTIL	(IREADER_MSG_BASE + 16)

#define IREADER_PAGEOFFSET		(IREADER_MSG_BASE + 17)

#define IREADER_GET_BEGIN_TEXT	(IREADER_MSG_BASE + 18)

#define IREADER_INVALIDATE		(IREADER_MSG_BASE + 19)

#define IREADER_DATA_VERSION	(1.0f)

struct iReaderData {
	iReaderData()
		: m_bPostil(false)
		, m_bExcerpt(false)
		, m_bTranslate(false)
		, m_hasSelectionText(false)
		, m_bSelectionTextPos(true)
		, m_bUnderSearch(false)
		, m_bErasePostil(false)
		, m_bCurrentPageHasPostil(false)
		, m_bHasBookmark(false)
		, m_nPagesCount(0)
		, m_nCurPage(0)
		, m_nPageNumByOffset(0)
		, m_nCurOffset(0)
		, m_nMaximumText(0)
		, m_nFontSize(24)
		, m_nFontType(0)
		, m_hProcess(0)
		, m_dwProcessId(0)
		, m_uInterProcMsg(0)
		, m_hNofityWnd(0)
		, m_nNextMatches(0)
		, m_nPrevMatches(0)
		, m_nPostilPenWidth(1)
	{
		ZeroMemory(m_szFilePath, sizeof(m_szFilePath));
		ZeroMemory(m_szFontFamily, sizeof(m_szFontFamily));
		ZeroMemory(m_szSearchText, sizeof(m_szSearchText));
		ZeroMemory(m_szPostilPath, sizeof(m_szPostilPath));
	}

	const float version()
	{
		return IREADER_DATA_VERSION;
	}

	// this parameter is used to indicated whether we are under Postil environment or not
	bool	m_bPostil;
	bool	m_bTranslate;
	bool	m_bExcerpt;

	// indicate whether the webkit has any text selected or not
	bool	m_hasSelectionText;

	// indicate the position of the selected text, true: top, false:down, according to the middle of the view
	bool	m_bSelectionTextPos;

	// indicate whether we are under the select text mode or not
	bool	m_bUnderSearch;

	bool	m_bCurrentPageHasPostil;

	// indicate whether we will erase the postil or not
	bool	m_bErasePostil;

	// indicate whether the current page has Bookmark
	bool	m_bHasBookmark;

	// this parameter is used to indicate the current pen's width for Postil
	int		m_nPostilPenWidth;

	// this parameter is used to indicate how many pages that this file contains
	int		m_nPagesCount;

	// this parameter is used to indicate the current page number
	int		m_nCurPage;

	// this parameter is used to indicate the current font size
	int		m_nFontSize;

	// this parameter is used to indicate which font type that we used
	int		m_nFontType;

	// this parameter is used to indicate the maximum page text number in the pages array
	int		m_nMaximumText;

	// store the page number calculated by the offset
	int		m_nPageNumByOffset;

	// store the current offset
	int		m_nCurOffset;

	// this parameter is used to indicate how many text matched page after the current page
	int		m_nNextMatches;

	// this parameter is used to indicate how many text matched page before the current page
	int		m_nPrevMatches;

	// process related parameters
	HANDLE	m_hProcess;
	DWORD	m_dwProcessId;
	UINT	m_uInterProcMsg;
	HWND	m_hNofityWnd;

	// this buffer is used to contain the font family
	TCHAR	m_szFontFamily[LF_FACESIZE];

	// this buffer is used to contain the searching text
	TCHAR	m_szSearchText[MAX_PATH];

	// this buffer is used to contain the file path to be opened by iReader.exe
	TCHAR	m_szFilePath[MAX_PATH];

	TCHAR	m_szPostilPath[MAX_PATH];
};

class iReader
{
public:
	inline static iReader* GetInstance()
	{
		if (m_iReader == NULL)
		{
			m_iReader = new iReader;
		}

		return m_iReader;
	}

	inline static void ReleaseInstance()
	{
		if (m_iReader != NULL)
		{
			m_iReader->UnInitialize();
			delete m_iReader;
		}
	}

	inline static BOOL IsIReaderInit(){return ((m_iReader != NULL) && m_iReader->IsIReaderWindStart())?TRUE:FALSE;}

	inline BOOL IsIReaderWindStart(){return (m_bIReaderInited && (m_hIReader != NULL))?TRUE:FALSE;}

	static inline BOOL IsPreInstanceExit()
	{
		HWND hWnd = FindWindow(lpszIReaderWindowClassName,NULL);
		UINT uInterprocessMsg = 0;
		DWORD dwProcessId = 0;
		HANDLE hProcess = NULL;

		if (hWnd != NULL) {
			DWORD dwTimeout = 30000;
			{
				CMemroySharePtr<iReaderData,1> pFileMapping(IREADER_DATA);
				if (pFileMapping && pFileMapping->version() >= IREADER_DATA_VERSION) {
					uInterprocessMsg = pFileMapping->m_uInterProcMsg;
					hProcess = pFileMapping->m_hProcess;
					dwProcessId = pFileMapping->m_dwProcessId;
				}
			}

			// the format rendering process is alread active, so we should let it exit;
			::PostMessage(hWnd, uInterprocessMsg, IREADER_QUIT, 0);
			Sleep(1500);

			if (dwProcessId == 0)
				GetWindowThreadProcessId(hWnd, &dwProcessId);

			if (dwProcessId > 0){
				hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,dwProcessId);
				TerminateProcess(hProcess,0);
			}

			int i = 0;
			int nSleepSlice = 1000;
			dwTimeout += 5*nSleepSlice;
			while (FindWindow(lpszIReaderWindowClassName,NULL) != NULL)	{
				Sleep(nSleepSlice);
				if (i > static_cast<int>(dwTimeout / nSleepSlice)) {
					if (hProcess != NULL)
						TerminateProcess(hProcess,0);
					break;
				}
				++i;
			}
			hWnd = NULL;
		}
		return TRUE;
	}

	HWND GetIReaderWindow()
	{
		return m_hIReader;
	}

	BOOL Initialize(HWND hNotifyWnd, LPCTSTR lpszPath, const UINT& privateMsg)
	{
		if (!lpszPath || _tcslen(lpszPath) <= 0)
			return FALSE;

		WIN32_FIND_DATA wfd;
		HANDLE hFile = FindFirstFile(lpszPath, &wfd);	
		if(INVALID_HANDLE_VALUE != hFile)
		{
			FindClose(hFile);
			hFile = NULL;
			if ((0 != (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)))
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}

		m_hNotifyWnd = hNotifyWnd;
#if defined(UNDER_CE)
		_stprintf(m_szExePath, lpszPath);
#else
		_stprintf_s(m_szExePath, MAX_PATH, lpszPath);
#endif
		m_uInterProcMsg = privateMsg;
		if (!Initialize())
		{
			return FALSE;
		}

		return TRUE;
	}

	BOOL OpenFile()
	{
		if (IsIReaderInit())
		{
			CMemroySharePtr<iReaderData,1> pFileMapping(IREADER_DATA);
			if (pFileMapping && pFileMapping->version() >= IREADER_DATA_VERSION)
			{
				pFileMapping->m_uInterProcMsg = m_uInterProcMsg;
				pFileMapping->m_hProcess = m_processsInfo.hProcess;
				pFileMapping->m_dwProcessId = m_processsInfo.dwProcessId;
				pFileMapping->m_hNofityWnd = m_hNotifyWnd;
			}
		}

		ResetEvent(m_hLayout);
		DispatchMessage(IREADER_OPEN, (LPARAM)0);
		if (LayoutWait())
		{
			return DividePages();
		}

		return FALSE;
	}

	BOOL DividePages()
	{
		ResetEvent(m_hSyncData);
		DispatchMessage(IREADER_DIVIDEPAGES, (LPARAM)0);

		return SyncWait();
	}

	BOOL NavigatePage(const int nPage)
	{		
		ShowWindow(m_hIReader, SW_SHOW);
		return DispatchMessage(IREADER_NAVIGATE, (LPARAM)nPage);
	}

	BOOL Zoom()
	{
		DWORD dwOffset = GetCurrentOffset();

		ResetEvent(m_hLayout);
		DispatchMessage(IREADER_ZOOM, (LPARAM)0);

		if (LayoutWait() && DividePages())
		{
			return NavigatePage(GetPageNumberByOffset(dwOffset));
		}

		return FALSE;
	}

	void Invalidate(BOOL bImmediate)
	{
		DispatchMessage(IREADER_INVALIDATE, (LPARAM)bImmediate);
	}

	BOOL ResizeView(const RECT& rcClient)
	{
		if (IsIReaderInit())
		{
			RECT rcWindow = {0};
			GetWindowRect(m_hIReader, &rcWindow);
			if (((rcClient.right - rcClient.left) != (rcWindow.right - rcWindow.left)) || ((rcClient.bottom - rcClient.top) != (rcWindow.bottom - rcWindow.top)))
			{
				ResetEvent(m_hLayout);
				if (MoveWindow(m_hIReader, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, TRUE))
					return TRUE;
			}
		}

		return TRUE;
	}
	
	DWORD GetPageNumberByOffset(DWORD dwOffset)
	{
		ResetEvent(m_hSyncData);
		if (DispatchMessage(IREADER_PAGENUMBYOFFSET, (LPARAM)dwOffset))
		{
			if (SyncWait())
			{
				CMemroySharePtr<iReaderData,1> pFileMapping(IREADER_DATA);
				if (pFileMapping)
				{
					return pFileMapping->m_nPageNumByOffset;
				}
			}
		}
		return 0;
	}

	DWORD GetCurrentOffset()
	{
		ResetEvent(m_hSyncData);
		if (DispatchMessage(IREADER_CUROFFSET, (LPARAM)0))
		{
			if (SyncWait())
			{
				CMemroySharePtr<iReaderData,1> pFileMapping(IREADER_DATA);
				if (pFileMapping)
				{
					return pFileMapping->m_nCurOffset;
				}
			}
		}
		return 0;
	}

	DWORD GetPageOffset(DWORD dwPage)
	{
		DWORD dwOffset = 0;
		ResetEvent(m_hSyncData);
		if (DispatchMessage(IREADER_PAGEOFFSET, (LPARAM)dwPage))
		{
			if (SyncWait())
			{
				CMemroySharePtr<iReaderData,1> pFileMapping(IREADER_DATA);
				if (pFileMapping)
				{
					dwOffset = pFileMapping->m_nCurOffset;
				}
			}
		}
		GetCurrentOffset();
		return dwOffset;
	}

	BOOL GetPageBeginText(DWORD dwPage)
	{
		DWORD dwOffset = 0;
		ResetEvent(m_hSyncData);
		if (DispatchMessage(IREADER_GET_BEGIN_TEXT, (LPARAM)dwPage))
		{
			if (SyncWait())
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL OpenTTS(DWORD dwPage)
	{
		ResetEvent(m_hSyncData);
		if (DispatchMessage(IREADER_TTS, (LPARAM)dwPage))
		{
			return SyncWait();
		}
		return FALSE;
	}

	BOOL ClearTextSelection()
	{
		return DispatchMessage(IREADER_CLEARSELECTION, (LPARAM)0);
	}

	BOOL Search()
	{
		ResetEvent(m_hSyncData);
		if (DispatchMessage(IREADER_SEARCH, (LPARAM)0) && SyncWait())
		{
			CMemroySharePtr<iReaderData,1> pFileMapping(IREADER_DATA);
			if (pFileMapping)
			{
				return ((pFileMapping->m_nNextMatches == 0) && (pFileMapping->m_nPrevMatches == 0))?FALSE:TRUE;
			}
		}
		return FALSE;
	}

	BOOL PrevMatched()
	{
		ResetEvent(m_hSyncData);
		if (DispatchMessage(IREADER_SEARCH, (LPARAM)1))
		{
			return SyncWait();
		}
		return FALSE;
	}

	BOOL NextMatched()
	{
		ResetEvent(m_hSyncData);
		if (DispatchMessage(IREADER_SEARCH, (LPARAM)2))
		{
			return SyncWait();
		}
		return FALSE;
	}

	BOOL FirstMatched()
	{
		ResetEvent(m_hSyncData);
		if (DispatchMessage(IREADER_SEARCH, (LPARAM)3))
		{
			return SyncWait();
		}
		return FALSE;
	}

	BOOL LastMatched()
	{
		ResetEvent(m_hSyncData);
		if (DispatchMessage(IREADER_SEARCH, (LPARAM)4))
		{
			return SyncWait();
		}
		return FALSE;
	}

	void ClearCache()
	{
		DispatchMessage(IREADER_CLEAR_CACHE, 0);
		return;
	}

	void ClearPostilPage(DWORD dwPage)
	{
		DispatchMessage(IREADER_CLEAR_POSTIL, dwPage);
		return;
	}

	void AttachWindow(HWND hParent)
	{
		if (m_hIReader != NULL)
		{
			m_hNotifyWnd = hParent;
			::ShowWindow(m_hIReader, SW_SHOW);
			::SetWindowLong(m_hIReader, GWL_STYLE, WS_CHILD);
			::SetParent(m_hIReader, m_hNotifyWnd);
		}
	}

	void DetachWindow()
	{
		if (m_hIReader != NULL)
		{
			::ShowWindow(m_hIReader,SW_HIDE);
			::SetWindowLong(m_hIReader, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
			::SetParent(m_hIReader,NULL);
		}
	}

private:
	iReader()
		:m_bIReaderInited(FALSE)
		,m_uInterProcMsg(0)
		,m_hIReader(NULL)
		,m_hNotifyWnd(NULL)
	{
		m_hSyncData = CreateEvent(NULL, TRUE, TRUE, IREADER_SYNC_DATA);
		m_hLayout = CreateEvent(NULL, TRUE, TRUE, IREADER_LAYOUT_EVENT);
	}

	iReader(const iReader& opera);
	iReader& operator= (const iReader& opera);

	~iReader()
	{
		if (m_hSyncData != NULL)
		{
			DWORD dwWaitResult = WaitForSingleObject(m_hSyncData, 1);
			switch (dwWaitResult) 
			{
			// The thread got ownership of the mutex
			case WAIT_OBJECT_0:
			// The thread got ownership of an abandoned mutex
			case WAIT_ABANDONED:
			case WAIT_TIMEOUT:
				break;
			}
			SetEvent(m_hSyncData);
			CloseHandle(m_hSyncData);
			m_hSyncData = NULL;
		}

		if (m_hLayout != NULL)
		{
			DWORD dwWaitResult = WaitForSingleObject(m_hLayout, 1);
			switch (dwWaitResult) 
			{
			// The thread got ownership of the mutex
			case WAIT_OBJECT_0:
			// The thread got ownership of an abandoned mutex
			case WAIT_ABANDONED:
			case WAIT_TIMEOUT:
				break;
			}
			SetEvent(m_hLayout);
			CloseHandle(m_hLayout);
			m_hLayout = NULL;
		}
	}

	BOOL LayoutWait(DWORD dwTimeOut = 60*1000)
	{
		BOOL IsSigneded = false;
		if (m_hLayout != NULL)
		{
			DWORD dwWaitResult = WaitForSingleObject(m_hLayout, dwTimeOut);
			switch (dwWaitResult) 
			{
			// The thread got ownership of the mutex
			case WAIT_OBJECT_0:
			// The thread got ownership of an abandoned mutex
			case WAIT_ABANDONED:
				IsSigneded = TRUE;
				break;
			case WAIT_TIMEOUT:
				IsSigneded = FALSE;
				break;
			case WAIT_FAILED:
				IsSigneded = FALSE;
				break;
			default:
				break;
			}
			SetEvent(m_hLayout);
		}
		else
		{
			IsSigneded = TRUE;
		}

		return IsSigneded;
	}

	BOOL SyncWait(DWORD dwTimeOut = 20*1000)
	{
		BOOL IsSigneded = false;
		if (m_hSyncData != NULL)
		{
			DWORD dwWaitResult = WaitForSingleObject(m_hSyncData, dwTimeOut);
			switch (dwWaitResult) 
			{
			// The thread got ownership of the mutex
			case WAIT_OBJECT_0:
			// The thread got ownership of an abandoned mutex
			case WAIT_ABANDONED:
				IsSigneded = TRUE;
				break;
			case WAIT_TIMEOUT:
				IsSigneded = FALSE;
				break;
			case WAIT_FAILED:
				IsSigneded = FALSE;
				break;
			default:
				break;
			}
			SetEvent(m_hSyncData);
		}
		else
		{
			IsSigneded = TRUE;
		}

		return IsSigneded;
	}


	BOOL Initialize()
	{
		if (!m_bIReaderInited)
		{
			IsPreInstanceExit();

			if (FindWindow(lpszIReaderWindowClassName, NULL) == NULL)
			{
				if (!CreateProcess(m_szExePath, _T("-r"), NULL, NULL, FALSE, NULL, NULL, NULL, NULL, &m_processsInfo))
				{
					_tprintf(_T("Fail to create process:%s, error code = %d.\n"), m_szExePath, GetLastError());
					return FALSE;
				}
			}
			m_bIReaderInited = TRUE;
		}

		if (m_bIReaderInited)
		{
			int nCount = 10;
			int i = 0;
			while (m_hIReader == NULL)
			{
				m_hIReader = FindWindow(lpszIReaderWindowClassName, NULL);
				
				if (m_hIReader != NULL)
				{
					ShowWindow(m_hIReader, (m_hNotifyWnd != NULL) ? SW_SHOW : SW_HIDE);
					int nCount = 1;
					while (GetForegroundWindow() != m_hIReader)
					{
						Sleep(1000);
						++nCount;
						if (nCount > 15)
						{
							return false;
						}
					}

					if (m_hNotifyWnd != NULL)
					{
						SetWindowLong(m_hIReader, GWL_STYLE, WS_CHILD);
						SetParent(m_hIReader, m_hNotifyWnd);
					}
					break;
				}

				++i;
				if (i < nCount)
				{
					Sleep(2000);
				}
				else
				{
					break;
				}
			}
		}

		return (m_bIReaderInited && (m_hIReader != NULL));
	}

	BOOL UnInitialize()
	{
		PostMessage(m_hIReader, m_uInterProcMsg, IREADER_QUIT, 0);

		if (m_bIReaderInited)
		{
			if (m_processsInfo.hProcess != NULL)
			{
				CloseHandle(m_processsInfo.hProcess);
			}
			if (m_processsInfo.hThread != NULL)
			{
				CloseHandle(m_processsInfo.hThread);
			}
			m_bIReaderInited = FALSE;
		}

		m_bIReaderInited = FALSE;
		m_uInterProcMsg = 0;
		m_iReader = NULL;
		m_hIReader = NULL;
		ZeroMemory(m_szExePath, sizeof(m_szExePath));

		return TRUE;
	}

	BOOL DispatchMessage(WPARAM wParam, LPARAM lParam)
	{
		if (IsIReaderInit())
		{
			PostMessage(m_hIReader, m_uInterProcMsg, wParam, (LPARAM)lParam);
		}
		else
		{
			return FALSE;
		}
		return TRUE;
	}

private:
	BOOL					m_bIReaderInited;
	UINT					m_uInterProcMsg;
	PROCESS_INFORMATION		m_processsInfo;
	HWND					m_hIReader;
	HWND					m_hNotifyWnd;
	HANDLE					m_hSyncData;
	HANDLE					m_hLayout;
	TCHAR					m_szExePath[MAX_PATH];

	static iReader*			m_iReader;
};

__declspec(selectany) iReader* iReader::m_iReader = NULL;

#endif // _IREADER_H_