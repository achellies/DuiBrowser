//
//
// DirectUI - UI Library
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2006-2007 Bjarke Viksoe.
//
// This code may be used in compiled form in any way you desire. These
// source files may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
////
// Acknowledgements :
// Bjarke Viksoe (http://www.viksoe.dk/code/windowless1.htm)
//
//
//
// Beware of bugs.
//
//
//
////////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// UIBase.h
//  Declaration of the following classes:
//  CDirectUIException, CCriticalSection,
//  CPoint, CRect, and CSize
//	CStdPtrArray, CStdStringPtrMap, CStdString, CStdValArray
//	CWindowWndImpl
//
// This file contains the declarations for the core set of classes required to
// create simple windows using DirectUI.  Seven classes are declared here:
//
// 1) CCriticalSection: This class is used internally to manage thread access
//            to shared resources. You can also use this class to lock and
//            release your own critical sections.
//
// 2) CDirectUIException: This class is used internally by DirectUI to handle
//            exceptions. You can also use it to throw and catch exceptions.
//
// 3) CWindowWndImpl: this class is a wrapper for native win32 window
//
// 4) CPoint, CRect and CSize:
//            These class can be used in place of a POINT, RECT or SIZE
//            repectively.
//
// Note: This header file (or another DirectUI header file which includes it)
//       should be included before all other header files. It sets some
//       important macros which need to be set before including Windows.h
//       Including this file first also allows it to disable some pointless
//       warning messages (see below).

#ifndef __UIBASE_H__
#define __UIBASE_H__

namespace DuiLib {

/////////////////////////////////////////////////////////////////////////////////////
//

#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif

#ifndef TRACE
#ifdef _DEBUG
#define TRACE __Trace
#define TRACEMSG __TraceMsg
#else
#define TRACE
#define TRACEMSG _T("")
#endif
#endif

// Define min and max for Dev-C++ compatibility
#ifndef max
  #define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
  #define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

void UILIB_API __Trace(LPCTSTR pstrFormat, ...);
LPCTSTR UILIB_API __TraceMsg(UINT uMsg);

enum Constants
{
	MAX_MENU_STRING = 80,
	MAX_STRING_SIZE = 255,
};

#if defined(UI_BUILD_FOR_WIN32) || defined(UI_BUILD_FOR_WINCE)

#if defined(UI_BUILD_FOR_WINCE)

#ifndef WS_OVERLAPPEDWINDOW
// Although WS_OVERLAPPEDWINDOW is not supported in Windows CE, the functionality can still be achieved by obtaining a bitwise OR of the style flags WS_OVERLAPPED, WS_CAPTION, WS_SYSMENU, WS_THICKFRAME, WS_MINIMIZEBOX, and WS_MAXIMIZEBOX.
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
#endif

#ifndef WS_POPUPWINDOW
#define WS_POPUPWINDOW (WS_BORDER | WS_POPUP | WS_SYSMENU)
#endif 

#endif

#define UI_WNDSTYLE_CONTAINER  (0)
#define UI_WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW)
#define UI_WNDSTYLE_CHILD      (WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define UI_WNDSTYLE_DIALOG     (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define UI_WNDSTYLE_DIALOGEX                (WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

#define UI_WNDSTYLE_EX_FRAME   (WS_EX_WINDOWEDGE)
#define UI_WNDSTYLE_EX_DIALOG  (WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME)

#define UI_CLASSSTYLE_CONTAINER  (0)
#define UI_CLASSSTYLE_FRAME      (CS_VREDRAW | CS_HREDRAW)
#define UI_CLASSSTYLE_CHILD      (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)
#define UI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

#endif

/////////////////////////////////////////////////////////////////////////////////////
// Declaration of the STRINGorID class

class STRINGorID
{
public:
    STRINGorID(LPCTSTR lpString) : m_lpstr(lpString)
    { }
    STRINGorID(UINT nID) : m_lpstr(MAKEINTRESOURCE(nID))
    { }
    LPCTSTR m_lpstr;
};

/////////////////////////////////////////////////////////////////////////////////////
// Declaration of the CPoint class
//
class UILIB_API CPoint : public tagPOINT
{
public:
    CPoint();
    CPoint(const POINT& src);
    CPoint(int x, int y);
    CPoint(LPARAM lParam);
	CPoint(DWORD dw){x = (short)LOWORD(dw); y = (short)HIWORD(dw);}
	void Offset(int dx, int dy)	{ x += dx; y += dy; }
	void SetPoint(int X, int Y)	{ x = X; y = Y; }
	BOOL operator == (POINT pt)	{ return ((x == pt.x) && (y == pt.y)); }
	BOOL operator != (POINT pt)	{ return ((x != pt.x) || (y != pt.y)); }
};

inline CPoint GetCursorPos()
{
	CPoint pt;
	::GetCursorPos(&pt);
	return pt;
}

/////////////////////////////////////////////////////////////////////////////////////
// Declaration of the CSize class
//
class UILIB_API CSize : public tagSIZE
{
public:
    CSize();
    CSize(const SIZE& src);
    CSize(const RECT rc);
    CSize(int cx, int cy);
	CSize(POINT pt)				{ cx = pt.x;  cx = pt.y; }
	CSize(DWORD dw)				{ cx = (short)LOWORD(dw); cx = (short)HIWORD(dw); }
	void SetSize(int CX, int CY){ cx = CX; cy = CY; }
	BOOL operator == (SIZE sz)	{ return (cx == sz.cx && cy == sz.cy); }
	BOOL operator != (SIZE sz)	{ return (cx != sz.cx || cy != sz.cy); }
};


/////////////////////////////////////////////////////////////////////////////////////
// Declaration of the CRect class
//
class UILIB_API CRect : public tagRECT
{
public:
    CRect();
    CRect(const RECT& src);
    CRect(int iLeft, int iTop, int iRight, int iBottom);

	CRect(POINT pt, SIZE sz)
	{ right = (left = pt.x) + sz.cx; bottom = (top = pt.y) + sz.cy; }

	CRect(POINT topLeft, POINT bottomRight)
	{ left = topLeft.x; top = topLeft.y; right = bottomRight.x; bottom = bottomRight.y; }

	operator LPRECT()
	{ return this; }

	operator LPCRECT() const
	{ return this; }

	BOOL operator == (const RECT& rc) const
	{ return ::EqualRect(this, &rc); }

	BOOL operator != (const RECT& rc) const
	{ return !::EqualRect(this, &rc); }

	void  operator=( RECT& srcRect)
	{ ::CopyRect(this, &srcRect); }

	int Height()
	{ return bottom - top; }

	int Width()
	{ return right - left; }

	void CopyRect(RECT* pRect)
	{ ::CopyRect(pRect, this); }

	BOOL EqualRect(const RECT* pRect)
	{ return ::EqualRect(pRect, this); }

	BOOL InflateRect(int dx, int dy)
	{ return ::InflateRect(this, dx, dy); }

	BOOL IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2)
	{ return ::IntersectRect(this, lpRect1, lpRect2); }

	BOOL IsRectEmpty()
	{ return ::IsRectEmpty(this);}

	BOOL OffsetRect(int dx, int dy)
	{ return ::OffsetRect(this, dx, dy); }

	BOOL PtInRect(POINT pt)
	{ return ::PtInRect(this, pt); }

	BOOL SetRectEmpty()
	{ return ::SetRectEmpty(this); }

	BOOL SubtractRect(const RECT* pRcSrc1, const RECT* pRcSrc2)
	{ return ::SubtractRect(this, pRcSrc1, pRcSrc2); }

    int GetWidth() const;
    int GetHeight() const;
    void Empty();
    bool IsNull() const;
    void Join(const RECT& rc);
    void ResetOffset();
    void Normalize();
    void Offset(int cx, int cy);
    void Inflate(int cx, int cy);
    void Deflate(int cx, int cy);
    void Union(CRect& rc);
};

/////////////////////////////////////////
// Declarations for the CCriticalSection class
//

// This class is used for thread synchronisation
class CCriticalSection
{
	public:
	CCriticalSection()	{ ::InitializeCriticalSection(&m_cs); }
	~CCriticalSection()	{ ::DeleteCriticalSection(&m_cs); }

	void Lock() 	{ ::EnterCriticalSection(&m_cs); }
	void Release()	{ ::LeaveCriticalSection(&m_cs); }

	private:
	CRITICAL_SECTION m_cs;
};

#ifndef lstrcpyn
// Required for WinCE
inline LPTSTR lstrcpyn(LPTSTR lpstrDest, LPCTSTR lpstrSrc, int nLength)
{
	if(NULL == lpstrDest || NULL == lpstrSrc || nLength <= 0)
		return NULL;
	int nLen = min((int)lstrlen(lpstrSrc), nLength - 1);
	LPTSTR lpstrRet = (LPTSTR)memcpy(lpstrDest, lpstrSrc, nLen * sizeof(TCHAR));
	lpstrDest[nLen] = 0;
	return lpstrRet;
}
#endif // !lstrcpyn

//
//////////////////////////////////////////
//// Declaration of the CDirectUIException class
////
//class CDirectUIException
//{
//public:
//	CDirectUIException (LPCTSTR msg) : m_err (::GetLastError()), m_msg(msg) {}
//	LPCTSTR What() const {return m_msg;}
//	void MessageBox() const
//	{
//		TCHAR buf1 [MAX_STRING_SIZE/2 -10] = _T("");
//		TCHAR buf2 [MAX_STRING_SIZE/2 -10] = _T("");
//		TCHAR buf3 [MAX_STRING_SIZE]       = _T("");
//
//		lstrcpyn(buf1, m_msg, MAX_STRING_SIZE/2 -10);
//
//		// Display Last Error information if it's useful
//		if (m_err != 0)
//		{
//			::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, m_err,
//				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf2, MAX_STRING_SIZE/2 -10, NULL);
//
//			::wsprintf(buf3, _T("%s\n\n     %s\n\n"), buf1, buf2);
//		}
//		else
//			::wsprintf(buf3, _T("%s"), buf1);
//
//		TRACE(_T("*** ERROR: An Exception occured ***\n"));
//		TRACE(buf3);
//		TRACE(_T("\n\n"));
//
//		::MessageBox (0, buf3, _T("Error"), MB_ICONEXCLAMATION | MB_OK);
//	}
//
//private:
//	DWORD  m_err;
//	LPCTSTR m_msg;
//
//};

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CStdPtrArray
{
public:
    CStdPtrArray(int iPreallocSize = 0);
	CStdPtrArray(const CStdPtrArray& src);
    ~CStdPtrArray();

    void Empty();
    void Resize(int iSize);
    bool IsEmpty() const;
    int Find(LPVOID iIndex) const;
    bool Add(LPVOID pData);
    bool SetAt(int iIndex, LPVOID pData);
    bool InsertAt(int iIndex, LPVOID pData);
    bool Remove(int iIndex);
    int GetSize() const;
    LPVOID* GetData();

    LPVOID GetAt(int iIndex) const;
    LPVOID operator[] (int nIndex) const;

protected:
    LPVOID* m_ppVoid;
    int m_nCount;
    int m_nAllocated;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CStdValArray
{
public:
    CStdValArray(int iElementSize, int iPreallocSize = 0);
    ~CStdValArray();

    void Empty();
    bool IsEmpty() const;
    bool Add(LPCVOID pData);
    bool Remove(int iIndex);
    int GetSize() const;
    LPVOID GetData();

    LPVOID GetAt(int iIndex) const;
    LPVOID operator[] (int nIndex) const;

protected:
    LPBYTE m_pVoid;
    int m_iElementSize;
    int m_nCount;
    int m_nAllocated;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CStdString
{
public:
    enum { MAX_LOCAL_STRING_LEN = 63 };

    CStdString();
    CStdString(const TCHAR ch);
    CStdString(const CStdString& src);
    CStdString(LPCTSTR lpsz, int nLen = -1);
    ~CStdString();

    void Empty();
    int GetLength() const;
    bool IsEmpty() const;
    TCHAR GetAt(int nIndex) const;
    void Append(LPCTSTR pstr);
    void Assign(LPCTSTR pstr, int nLength = -1);
    LPCTSTR GetData();

    void SetAt(int nIndex, TCHAR ch);
    operator LPCTSTR() const;

    TCHAR operator[] (int nIndex) const;
    const CStdString& operator=(const CStdString& src);
    const CStdString& operator=(const TCHAR ch);
    const CStdString& operator=(LPCTSTR pstr);
#ifndef _UNICODE
    const CStdString& CStdString::operator=(LPCWSTR lpwStr);
#endif
    CStdString operator+(const CStdString& src);
    CStdString operator+(LPCTSTR pstr);
    const CStdString& operator+=(const CStdString& src);
    const CStdString& operator+=(LPCTSTR pstr);
    const CStdString& operator+=(const TCHAR ch);

    bool operator == (LPCTSTR str) const;
    bool operator != (LPCTSTR str) const;
    bool operator <= (LPCTSTR str) const;
    bool operator <  (LPCTSTR str) const;
    bool operator >= (LPCTSTR str) const;
    bool operator >  (LPCTSTR str) const;

    int Compare(LPCTSTR pstr) const;
    int CompareNoCase(LPCTSTR pstr) const;

    void MakeUpper();
    void MakeLower();

    CStdString Left(int nLength) const;
    CStdString Mid(int iPos, int nLength = -1) const;
    CStdString Right(int nLength) const;

    int Find(TCHAR ch, int iPos = 0) const;
    int Find(LPCTSTR pstr, int iPos = 0) const;
    int ReverseFind(TCHAR ch) const;
    int Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo);

    int __cdecl Format(LPCTSTR pstrFormat, ...);
    int __cdecl SmallFormat(LPCTSTR pstrFormat, ...);

protected:
    LPTSTR m_pstr;
    TCHAR m_szBuffer[MAX_LOCAL_STRING_LEN + 1];
};


/////////////////////////////////////////////////////////////////////////////////////
//

struct TITEM
{
    CStdString Key;
    LPVOID Data;
    struct TITEM* pNext;
};

class UILIB_API CStdStringPtrMap
{
public:
    CStdStringPtrMap(int nSize = 83);
    ~CStdStringPtrMap();

    void Resize(int nSize = 83);
    LPVOID Find(LPCTSTR key) const;
    bool Insert(LPCTSTR key, LPVOID pData);
    LPVOID Set(LPCTSTR key, LPVOID pData);
    bool Remove(LPCTSTR key);
    int GetSize() const;
    LPCTSTR GetAt(int iIndex) const;
    LPCTSTR operator[] (int nIndex) const;

protected:
    TITEM** m_aT;
    int m_nBuckets;
};

} // namespace DuiLib

#endif // __UIBASE_H__
