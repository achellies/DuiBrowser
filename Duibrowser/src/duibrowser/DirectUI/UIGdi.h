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
#ifndef _UIGDI_H_
#define _UIGDI_H_

namespace DuiLib {

// protect template members from windowsx.h macros
#ifdef _INC_WINDOWSX
  #undef CopyRgn
  #undef CreateBrush
  #undef CreatePen
  #undef SelectBrush
  #undef SelectPen
  #undef SelectFont
  #undef SelectBitmap
#endif // _INC_WINDOWSX


#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif


///////////////////////////////////////////////////////////////////////////////
// CRgn

template <bool t_bManaged>
class CRgnT
{
public:
// Data members
	HRGN m_hRgn;

// Constructor/destructor/operators
	CRgnT(HRGN hRgn = NULL) : m_hRgn(hRgn)
	{ }

	~CRgnT()
	{
		if(t_bManaged && m_hRgn != NULL)
			DeleteObject();
	}

	CRgnT<t_bManaged>& operator =(HRGN hRgn)
	{
		Attach(hRgn);
		return *this;
	}

	void Attach(HRGN hRgn)
	{
		if(t_bManaged && m_hRgn != NULL && m_hRgn != hRgn)
			::DeleteObject(m_hRgn);
		m_hRgn = hRgn;
	}

	HRGN Detach()
	{
		HRGN hRgn = m_hRgn;
		m_hRgn = NULL;
		return hRgn;
	}

	operator HRGN() const { return m_hRgn; }

	bool IsNull() const { return (m_hRgn == NULL); }

// Create methods
	HRGN CreateRectRgn(int x1, int y1, int x2, int y2)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreateRectRgn(x1, y1, x2, y2);
		return m_hRgn;
	}

	HRGN CreateRectRgnIndirect(LPCRECT lpRect)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreateRectRgnIndirect(lpRect);
		return m_hRgn;
	}

#if defined(UI_BUILD_FOR_WIN32) && !defined(UI_BUILD_FOR_WINCE)
	HRGN CreateEllipticRgn(int x1, int y1, int x2, int y2)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreateEllipticRgn(x1, y1, x2, y2);
		return m_hRgn;
	}

	HRGN CreateEllipticRgnIndirect(LPCRECT lpRect)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreateEllipticRgnIndirect(lpRect);
		return m_hRgn;
	}

	HRGN CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreatePolygonRgn(lpPoints, nCount, nMode);
		return m_hRgn;
	}

	HRGN CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount, int nPolyFillMode)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreatePolyPolygonRgn(lpPoints, lpPolyCounts, nCount, nPolyFillMode);
		return m_hRgn;
	}

	HRGN CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3);
		return m_hRgn;
	}

	HRGN CreateFromPath(HDC hDC)
	{
		ASSERT(m_hRgn == NULL);
		ASSERT(hDC != NULL);
		m_hRgn = ::PathToRegion(hDC);
		return m_hRgn;
	}

	HRGN CreateFromData(const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData)
	{
		ASSERT(m_hRgn == NULL);
		m_hRgn = ::ExtCreateRegion(lpXForm, nCount, pRgnData);
		return m_hRgn;
	}
#endif // !_WIN32_WCE

	BOOL DeleteObject()
	{
		ASSERT(m_hRgn != NULL);
		BOOL bRet = ::DeleteObject(m_hRgn);
		if(bRet)
			m_hRgn = NULL;
		return bRet;
	}

// Operations
	void SetRectRgn(int x1, int y1, int x2, int y2)
	{
		ASSERT(m_hRgn != NULL);
		::SetRectRgn(m_hRgn, x1, y1, x2, y2);
	}

	void SetRectRgn(LPCRECT lpRect)
	{
		ASSERT(m_hRgn != NULL);
		::SetRectRgn(m_hRgn, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
	}

	int CombineRgn(HRGN hRgnSrc1, HRGN hRgnSrc2, int nCombineMode)
	{
		ASSERT(m_hRgn != NULL);
		return ::CombineRgn(m_hRgn, hRgnSrc1, hRgnSrc2, nCombineMode);
	}

	int CombineRgn(HRGN hRgnSrc, int nCombineMode)
	{
		ASSERT(m_hRgn != NULL);
		return ::CombineRgn(m_hRgn, m_hRgn, hRgnSrc, nCombineMode);
	}

	int CopyRgn(HRGN hRgnSrc)
	{
		ASSERT(m_hRgn != NULL);
		return ::CombineRgn(m_hRgn, hRgnSrc, NULL, RGN_COPY);
	}

	BOOL EqualRgn(HRGN hRgn) const
	{
		ASSERT(m_hRgn != NULL);
		return ::EqualRgn(m_hRgn, hRgn);
	}

	int OffsetRgn(int x, int y)
	{
		ASSERT(m_hRgn != NULL);
		return ::OffsetRgn(m_hRgn, x, y);
	}

	int OffsetRgn(POINT point)
	{
		ASSERT(m_hRgn != NULL);
		return ::OffsetRgn(m_hRgn, point.x, point.y);
	}

	int GetRgnBox(LPRECT lpRect) const
	{
		ASSERT(m_hRgn != NULL);
		return ::GetRgnBox(m_hRgn, lpRect);
	}

	BOOL PtInRegion(int x, int y) const
	{
		ASSERT(m_hRgn != NULL);
		return ::PtInRegion(m_hRgn, x, y);
	}

	BOOL PtInRegion(POINT point) const
	{
		ASSERT(m_hRgn != NULL);
		return ::PtInRegion(m_hRgn, point.x, point.y);
	}

	BOOL RectInRegion(LPCRECT lpRect) const
	{
		ASSERT(m_hRgn != NULL);
		return ::RectInRegion(m_hRgn, lpRect);
	}

	int GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const
	{
		ASSERT(m_hRgn != NULL);
		return (int)::GetRegionData(m_hRgn, nDataSize, lpRgnData);
	}
};

typedef CRgnT<false>   CRgnHandle;
typedef CRgnT<true>    CRgn;


}; // namespace DuiLib

#endif // _UIGDI_H_