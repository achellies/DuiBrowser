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
#ifndef _UICRACK_H_
#define _UICRACK_H_

#ifdef _MSC_VER
#pragma once
#endif

namespace DuiLib {

/////////////////////////////////////////////////////////////////////////////////////
//
//
// marco defination
#define BEGIN_MSG_MAP_EX(theClass)	\
public: \
	BOOL m_bMsgHandled; \
	/* "handled" management for cracked handlers */ \
	BOOL IsMsgHandled() const \
	{ \
		return m_bMsgHandled; \
	} \
	void SetMsgHandled(BOOL bHandled) \
	{ \
		m_bMsgHandled = bHandled; \
	} \
	BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0) \
	{ \
		BOOL bOldMsgHandled = m_bMsgHandled; \
		BOOL bRet = _ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID); \
		m_bMsgHandled = bOldMsgHandled; \
		return bRet; \
	} \
	BOOL _ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID) \
	{ \
		BOOL bHandled = TRUE; \
		hWnd; \
		uMsg; \
		wParam; \
		lParam; \
		lResult; \
		bHandled; \
		switch(dwMsgMapID) \
		{ \
		case 0:

///////////////////////////////////////////////////////////////////////////////
// Standard Windows message macros

// int OnCreate(LPCREATESTRUCT lpCreateStruct)
#define MSG_WM_CREATE(func) \
	if (uMsg == WM_CREATE) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func((LPCREATESTRUCT)lParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// BOOL OnCopyData(HWND wnd, PCOPYDATASTRUCT pCopyDataStruct)
#define MSG_WM_COPYDATA(func) \
	if (uMsg == WM_COPYDATA) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func((HWND)wParam, (PCOPYDATASTRUCT)lParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnDestroy()
#define MSG_WM_DESTROY(func) \
	if (uMsg == WM_DESTROY) \
	{ \
		SetMsgHandled(TRUE); \
		func(); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// BOOL OnNcActivate(BOOL bActive)
#define MSG_WM_NCACTIVATE(func) \
	if (uMsg == WM_NCACTIVATE) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func((BOOL)wParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
#define MSG_WM_NCCALCSIZE(func) \
	if (uMsg == WM_NCCALCSIZE) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = func((BOOL)wParam, lParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnNcPaint(CRgn rgn)
#define MSG_WM_NCPAINT(func) \
	if (uMsg == WM_NCPAINT) \
	{ \
		SetMsgHandled(TRUE); \
		func((HRGN)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// UINT OnNcHitTest(CPoint point)
#define MSG_WM_NCHITTEST(func) \
	if (uMsg == WM_NCHITTEST) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func(CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnMove(CPoint ptPos)
#define MSG_WM_MOVE(func) \
	if (uMsg == WM_MOVE) \
	{ \
		SetMsgHandled(TRUE); \
		func(CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnSize(UINT nType, CSize size)
#define MSG_WM_SIZE(func) \
	if (uMsg == WM_SIZE) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CSize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}


// void OnSetFocus(HWND wndOld)
#define MSG_WM_SETFOCUS(func) \
	if (uMsg == WM_SETFOCUS) \
	{ \
		SetMsgHandled(TRUE); \
		func((HWND)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnKillFocus(HWND wndFocus)
#define MSG_WM_KILLFOCUS(func) \
	if (uMsg == WM_KILLFOCUS) \
	{ \
		SetMsgHandled(TRUE); \
		func((HWND)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnEnable(BOOL bEnable)
#define MSG_WM_ENABLE(func) \
	if (uMsg == WM_ENABLE) \
	{ \
		SetMsgHandled(TRUE); \
		func((BOOL)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// BOOL OnEraseBkgnd(CDCHandle dc)
#define MSG_WM_ERASEBKGND(func) \
	if (uMsg == WM_ERASEBKGND) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func((HDC)wParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnSysColorChange()
#define MSG_WM_SYSCOLORCHANGE(func) \
	if (uMsg == WM_SYSCOLORCHANGE) \
	{ \
		SetMsgHandled(TRUE); \
		func(); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}


// void OnShowWindow(BOOL bShow, UINT nStatus)
#define MSG_WM_SHOWWINDOW(func) \
	if (uMsg == WM_SHOWWINDOW) \
	{ \
		SetMsgHandled(TRUE); \
		func((BOOL)wParam, (int)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit)
#define MSG_WM_CTLCOLOREDIT(func) \
	if (uMsg == WM_CTLCOLOREDIT) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// HBRUSH OnCtlColorListBox(CDCHandle dc, CListBox listBox)
#define MSG_WM_CTLCOLORLISTBOX(func) \
	if (uMsg == WM_CTLCOLORLISTBOX) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// HBRUSH OnCtlColorBtn(CDCHandle dc, CButton button)
#define MSG_WM_CTLCOLORBTN(func) \
	if (uMsg == WM_CTLCOLORBTN) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// HBRUSH OnCtlColorDlg(CDCHandle dc, HWND wnd)
#define MSG_WM_CTLCOLORDLG(func) \
	if (uMsg == WM_CTLCOLORDLG) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// HBRUSH OnCtlColorScrollBar(CDCHandle dc, CScrollBar scrollBar)
#define MSG_WM_CTLCOLORSCROLLBAR(func) \
	if (uMsg == WM_CTLCOLORSCROLLBAR) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic)
#define MSG_WM_CTLCOLORSTATIC(func) \
	if (uMsg == WM_CTLCOLORSTATIC) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
#define MSG_WM_SETTINGCHANGE(func) \
	if (uMsg == WM_SETTINGCHANGE) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, (LPCTSTR)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}


// UINT OnGetDlgCode(LPMSG lpMsg)
#define MSG_WM_GETDLGCODE(func) \
	if (uMsg == WM_GETDLGCODE) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func((LPMSG)lParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnNcMouseMove(UINT nHitTest, CPoint point)
#define MSG_WM_NCMOUSEMOVE(func) \
	if (uMsg == WM_NCMOUSEMOVE) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnNcLButtonDown(UINT nHitTest, CPoint point)
#define MSG_WM_NCLBUTTONDOWN(func) \
	if (uMsg == WM_NCLBUTTONDOWN) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnNcLButtonUp(UINT nHitTest, CPoint point)
#define MSG_WM_NCLBUTTONUP(func) \
	if (uMsg == WM_NCLBUTTONUP) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnNcLButtonDblClk(UINT nHitTest, CPoint point)
#define MSG_WM_NCLBUTTONDBLCLK(func) \
	if (uMsg == WM_NCLBUTTONDBLCLK) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnNcRButtonDown(UINT nHitTest, CPoint point)
#define MSG_WM_NCRBUTTONDOWN(func) \
	if (uMsg == WM_NCRBUTTONDOWN) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnNcRButtonUp(UINT nHitTest, CPoint point)
#define MSG_WM_NCRBUTTONUP(func) \
	if (uMsg == WM_NCRBUTTONUP) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnNcRButtonDblClk(UINT nHitTest, CPoint point)
#define MSG_WM_NCRBUTTONDBLCLK(func) \
	if (uMsg == WM_NCRBUTTONDBLCLK) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnNcMButtonDown(UINT nHitTest, CPoint point)
#define MSG_WM_NCMBUTTONDOWN(func) \
	if (uMsg == WM_NCMBUTTONDOWN) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnNcMButtonUp(UINT nHitTest, CPoint point)
#define MSG_WM_NCMBUTTONUP(func) \
	if (uMsg == WM_NCMBUTTONUP) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnNcMButtonDblClk(UINT nHitTest, CPoint point)
#define MSG_WM_NCMBUTTONDBLCLK(func) \
	if (uMsg == WM_NCMBUTTONDBLCLK) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_KEYDOWN(func) \
	if (uMsg == WM_KEYDOWN) \
	{ \
		SetMsgHandled(TRUE); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_KEYUP(func) \
	if (uMsg == WM_KEYUP) \
	{ \
		SetMsgHandled(TRUE); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_CHAR(func) \
	if (uMsg == WM_CHAR) \
	{ \
		SetMsgHandled(TRUE); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_DEADCHAR(func) \
	if (uMsg == WM_DEADCHAR) \
	{ \
		SetMsgHandled(TRUE); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_SYSKEYDOWN(func) \
	if (uMsg == WM_SYSKEYDOWN) \
	{ \
		SetMsgHandled(TRUE); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_SYSKEYUP(func) \
	if (uMsg == WM_SYSKEYUP) \
	{ \
		SetMsgHandled(TRUE); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_SYSCHAR(func) \
	if (uMsg == WM_SYSCHAR) \
	{ \
		SetMsgHandled(TRUE); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define MSG_WM_SYSDEADCHAR(func) \
	if (uMsg == WM_SYSDEADCHAR) \
	{ \
		SetMsgHandled(TRUE); \
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnSysCommand(UINT nID, LPARAM lParam)
#define MSG_WM_SYSCOMMAND(func) \
	if (uMsg == WM_SYSCOMMAND) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnTCard(UINT idAction, DWORD dwActionData)
#define MSG_WM_TCARD(func) \
	if (uMsg == WM_TCARD) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, (DWORD)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnTimer(UINT_PTR nIDEvent)
#define MSG_WM_TIMER(func) \
	if (uMsg == WM_TIMER) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT_PTR)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
#define MSG_WM_HSCROLL(func) \
	if (uMsg == WM_HSCROLL) \
	{ \
		SetMsgHandled(TRUE); \
		func((int)LOWORD(wParam), (short)HIWORD(wParam), (HWND)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
#define MSG_WM_VSCROLL(func) \
	if (uMsg == WM_VSCROLL) \
	{ \
		SetMsgHandled(TRUE); \
		func((int)LOWORD(wParam), (short)HIWORD(wParam), (HWND)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}
// void OnGetMinMaxInfo(LPMINMAXINFO lpMMI)
#define MSG_WM_GETMINMAXINFO(func) \
	if (uMsg == WM_GETMINMAXINFO) \
	{ \
		SetMsgHandled(TRUE); \
		func((LPMINMAXINFO)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnIconEraseBkgnd(CDCHandle dc)
#define MSG_WM_ICONERASEBKGND(func) \
	if (uMsg == WM_ICONERASEBKGND) \
	{ \
		SetMsgHandled(TRUE); \
		func((HDC)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}


// void OnInitMenu(CMenu menu)
#define MSG_WM_INITMENU(func) \
	if (uMsg == WM_INITMENU) \
	{ \
		SetMsgHandled(TRUE); \
		func((HMENU)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnInitMenuPopup(CMenu menuPopup, UINT nIndex, BOOL bSysMenu)
#define MSG_WM_INITMENUPOPUP(func) \
	if (uMsg == WM_INITMENUPOPUP) \
	{ \
		SetMsgHandled(TRUE); \
		func((HMENU)wParam, (UINT)LOWORD(lParam), (BOOL)HIWORD(lParam)); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnMenuSelect(UINT nItemID, UINT nFlags, CMenu menu)
#define MSG_WM_MENUSELECT(func) \
	if (uMsg == WM_MENUSELECT) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HMENU)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu menu)
#define MSG_WM_MENUCHAR(func) \
	if (uMsg == WM_MENUCHAR) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = func((TCHAR)LOWORD(wParam), (UINT)HIWORD(wParam), (HMENU)lParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// LRESULT OnNotify(int idCtrl, LPNMHDR pnmh)
#define MSG_WM_NOTIFY(func) \
	if (uMsg == WM_NOTIFY) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = func((int)wParam, (LPNMHDR)lParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnEnterIdle(UINT nWhy, HWND wndWho)
#define MSG_WM_ENTERIDLE(func) \
	if (uMsg == WM_ENTERIDLE) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, (HWND)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnMouseMove(UINT nFlags, CPoint point)
#define MSG_WM_MOUSEMOVE(func) \
	if (uMsg == WM_MOUSEMOVE) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
#define MSG_WM_MOUSEWHEEL(func) \
	if (uMsg == WM_MOUSEWHEEL) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func((UINT)LOWORD(wParam), (short)HIWORD(wParam), CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnLButtonDown(UINT nFlags, CPoint point)
#define MSG_WM_LBUTTONDOWN(func) \
	if (uMsg == WM_LBUTTONDOWN) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnLButtonUp(UINT nFlags, CPoint point)
#define MSG_WM_LBUTTONUP(func) \
	if (uMsg == WM_LBUTTONUP) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnLButtonDblClk(UINT nFlags, CPoint point)
#define MSG_WM_LBUTTONDBLCLK(func) \
	if (uMsg == WM_LBUTTONDBLCLK) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnRButtonDown(UINT nFlags, CPoint point)
#define MSG_WM_RBUTTONDOWN(func) \
	if (uMsg == WM_RBUTTONDOWN) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnRButtonUp(UINT nFlags, CPoint point)
#define MSG_WM_RBUTTONUP(func) \
	if (uMsg == WM_RBUTTONUP) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnRButtonDblClk(UINT nFlags, CPoint point)
#define MSG_WM_RBUTTONDBLCLK(func) \
	if (uMsg == WM_RBUTTONDBLCLK) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnMButtonDown(UINT nFlags, CPoint point)
#define MSG_WM_MBUTTONDOWN(func) \
	if (uMsg == WM_MBUTTONDOWN) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnMButtonUp(UINT nFlags, CPoint point)
#define MSG_WM_MBUTTONUP(func) \
	if (uMsg == WM_MBUTTONUP) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnMButtonDblClk(UINT nFlags, CPoint point)
#define MSG_WM_MBUTTONDBLCLK(func) \
	if (uMsg == WM_MBUTTONDBLCLK) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnParentNotify(UINT message, UINT nChildID, LPARAM lParam)
#define MSG_WM_PARENTNOTIFY(func) \
	if (uMsg == WM_PARENTNOTIFY) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnMDIActivate(HWND wndActivate, HWND wndDeactivate)
#define MSG_WM_MDIACTIVATE(func) \
	if (uMsg == WM_MDIACTIVATE) \
	{ \
		SetMsgHandled(TRUE); \
		func((HWND)wParam, (HWND)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnRenderFormat(UINT nFormat)
#define MSG_WM_RENDERFORMAT(func) \
	if (uMsg == WM_RENDERFORMAT) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnRenderAllFormats()
#define MSG_WM_RENDERALLFORMATS(func) \
	if (uMsg == WM_RENDERALLFORMATS) \
	{ \
		SetMsgHandled(TRUE); \
		func(); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnDestroyClipboard()
#define MSG_WM_DESTROYCLIPBOARD(func) \
	if (uMsg == WM_DESTROYCLIPBOARD) \
	{ \
		SetMsgHandled(TRUE); \
		func(); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnDrawClipboard()
#define MSG_WM_DRAWCLIPBOARD(func) \
	if (uMsg == WM_DRAWCLIPBOARD) \
	{ \
		SetMsgHandled(TRUE); \
		func(); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnPaintClipboard(HWND wndViewer, const LPPAINTSTRUCT lpPaintStruct)
#define MSG_WM_PAINTCLIPBOARD(func) \
	if (uMsg == WM_PAINTCLIPBOARD) \
	{ \
		SetMsgHandled(TRUE); \
		func((HWND)wParam, (const LPPAINTSTRUCT)::GlobalLock((HGLOBAL)lParam)); \
		::GlobalUnlock((HGLOBAL)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnVScrollClipboard(HWND wndViewer, UINT nSBCode, UINT nPos)
#define MSG_WM_VSCROLLCLIPBOARD(func) \
	if (uMsg == WM_VSCROLLCLIPBOARD) \
	{ \
		SetMsgHandled(TRUE); \
		func((HWND)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnContextMenu(HWND wnd, CPoint point)
#define MSG_WM_CONTEXTMENU(func) \
	if (uMsg == WM_CONTEXTMENU) \
	{ \
		SetMsgHandled(TRUE); \
		func((HWND)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnSizeClipboard(HWND wndViewer, const LPRECT lpRect)
#define MSG_WM_SIZECLIPBOARD(func) \
	if (uMsg == WM_SIZECLIPBOARD) \
	{ \
		SetMsgHandled(TRUE); \
		func((HWND)wParam, (const LPRECT)::GlobalLock((HGLOBAL)lParam)); \
		::GlobalUnlock((HGLOBAL)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnAskCbFormatName(UINT nMaxCount, LPTSTR lpszString)
#define MSG_WM_ASKCBFORMATNAME(func) \
	if (uMsg == WM_ASKCBFORMATNAME) \
	{ \
		SetMsgHandled(TRUE); \
		func((DWORD)wParam, (LPTSTR)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnChangeCbChain(HWND wndRemove, HWND wndAfter)
#define MSG_WM_CHANGECBCHAIN(func) \
	if (uMsg == WM_CHANGECBCHAIN) \
	{ \
		SetMsgHandled(TRUE); \
		func((HWND)wParam, (HWND)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnHScrollClipboard(HWND wndViewer, UINT nSBCode, UINT nPos)
#define MSG_WM_HSCROLLCLIPBOARD(func) \
	if (uMsg == WM_HSCROLLCLIPBOARD) \
	{ \
		SetMsgHandled(TRUE); \
		func((HWND)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// BOOL OnQueryNewPalette()
#define MSG_WM_QUERYNEWPALETTE(func) \
	if (uMsg == WM_QUERYNEWPALETTE) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func(); \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnPaletteChanged(HWND wndFocus)
#define MSG_WM_PALETTECHANGED(func) \
	if (uMsg == WM_PALETTECHANGED) \
	{ \
		SetMsgHandled(TRUE); \
		func((HWND)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnPaletteIsChanging(HWND wndPalChg)
#define MSG_WM_PALETTEISCHANGING(func) \
	if (uMsg == WM_PALETTEISCHANGING) \
	{ \
		SetMsgHandled(TRUE); \
		func((HWND)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnDropFiles(HDROP hDropInfo)
#define MSG_WM_DROPFILES(func) \
	if (uMsg == WM_DROPFILES) \
	{ \
		SetMsgHandled(TRUE); \
		func((HDROP)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnWindowPosChanging(LPWINDOWPOS lpWndPos)
#define MSG_WM_WINDOWPOSCHANGING(func) \
	if (uMsg == WM_WINDOWPOSCHANGING) \
	{ \
		SetMsgHandled(TRUE); \
		func((LPWINDOWPOS)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
#define MSG_WM_WINDOWPOSCHANGED(func) \
	if (uMsg == WM_WINDOWPOSCHANGED) \
	{ \
		SetMsgHandled(TRUE); \
		func((LPWINDOWPOS)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnExitMenuLoop(BOOL fIsTrackPopupMenu)
#define MSG_WM_EXITMENULOOP(func) \
	if (uMsg == WM_EXITMENULOOP) \
	{ \
		SetMsgHandled(TRUE); \
		func((BOOL)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnEnterMenuLoop(BOOL fIsTrackPopupMenu)
#define MSG_WM_ENTERMENULOOP(func) \
	if (uMsg == WM_ENTERMENULOOP) \
	{ \
		SetMsgHandled(TRUE); \
		func((BOOL)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
#define MSG_WM_STYLECHANGED(func) \
	if (uMsg == WM_STYLECHANGED) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, (LPSTYLESTRUCT)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void OnStyleChanging(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
#define MSG_WM_STYLECHANGING(func) \
	if (uMsg == WM_STYLECHANGING) \
	{ \
		SetMsgHandled(TRUE); \
		func((UINT)wParam, (LPSTYLESTRUCT)lParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}


// void OnClose()
#define MSG_WM_CLOSE(func) \
	if (uMsg == WM_CLOSE) \
	{ \
		SetMsgHandled(TRUE); \
		func(); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

#define END_MSG_MAP() \
			break; \
		default: \
			break; \
		} \
		return FALSE; \
	}

#define BEGIN_NOTIFY_MAP(theClass) \
	void Notify(TNotifyUI& msg) \
	{

#define NOTIFY_BUTTON_CLICK(controlUIName, func) \
	if (((_tcsicmp(msg.sType, kClick) == 0) || (_tcsicmp(msg.sType, _T("link")) == 0)) && (_tcsicmp(msg.pSender->GetName(),controlUIName) == 0)) {   \
        func(msg); \
    }

#define NOTIFY_RADIOBUTTON_CLICK(controlUIName, func) \
	else if ((_tcsicmp(msg.sType, kSelectChanged) == 0) && (_tcsicmp(msg.pSender->GetName(),controlUIName) == 0) && (_tcsicmp(msg.pSender->GetClass(),_T("OptionUI")) == 0)) {	\
		func(msg); \
	}

#define NOTIFY_EDIT_CHANGE(controlUIName, func) \
    else if ((_tcsicmp(msg.sType, _T("changed")) == 0) && (_tcsicmp(msg.pSender->GetName(),controlUIName) == 0) && ((_tcsicmp(msg.pSender->GetClass(),_T("SingleLineEditUI")) == 0) || (_tcsicmp(msg.pSender->GetClass(),_T("MultiLineEditUI")) == 0))) {   \
		func(msg); \
    }

#define NOTIFY_COMBOX_SELECT(controlUIName, func) \
    else if ((_tcsicmp(msg.sType, kItemSelect) == 0) && (_tcsicmp(msg.pSender->GetName(),controlUIName) == 0)) {   \
        func(msg); \
    }

#define NOTIFY_LIST_ITEMCLICK(controlUIName, func) \
    else if ((_tcsicmp(msg.sType, kItemClick) == 0) && ((msg.pSender->GetParent() != NULL) && (msg.pSender->GetParent()->GetParent() != NULL)&& _tcsicmp(msg.pSender->GetParent()->GetParent()->GetName(),controlUIName) == 0)) {   \
        func(msg); \
    }


#define NOTIFY_WND_INIT(func) \
    else if (_tcsicmp(msg.sType, kWindowInit) == 0) {   \
        func(msg); \
    }

#define END_NOTIFY_MAP() \
		return; \
	}

/////////////////////////////////////////////////////////////////////////////////////
//
//
// constrol class name and interface name

// UIActiveX
extern UILIB_API const TCHAR* const kUIActiveXClassName;// = _T("ActiveXUI");
extern UILIB_API const TCHAR* const kUIActiveXInterfaceName;// = _T("ActiveX");

// ComboUI
extern UILIB_API const TCHAR* const kComboUIClassName;// = _T("ComboUI");
extern UILIB_API const TCHAR* const kComboUIInterfaceName;// = _T("Combo");

// LabelUI
extern UILIB_API const TCHAR* const kLabelUIClassName;// = _T("LabelUI");
extern UILIB_API const TCHAR* const kLabelUIInterfaceName;// = _T("Label");

// ButtonUI
extern UILIB_API const TCHAR* const kButtonUIClassName;// = _T("ButtonUI");
extern UILIB_API const TCHAR* const kButtonUIInterfaceName;// = _T("Button");

// OptionUI
extern UILIB_API const TCHAR* const kOptionUIClassName;// = _T("OptionUI");
extern UILIB_API const TCHAR* const kOptionUIInterfaceName;// = _T("Option");

// TextUI
extern UILIB_API const TCHAR* const kTextUIClassName;// = _T("TextUI");
extern UILIB_API const TCHAR* const kTextUIInterfaceName;// = _T("Text");

// ProgressUI
extern UILIB_API const TCHAR* const kProgressUIClassName;// = _T("ProgressUI");
extern UILIB_API const TCHAR* const kProgressUIInterfaceName;// = _T("Progress");

// SliderUI
extern UILIB_API const TCHAR* const kSliderUIClassName;// = _T("SliderUI");
extern UILIB_API const TCHAR* const kSliderUIInterfaceName;// = _T("Slider");

// EditUI
extern UILIB_API const TCHAR* const kEditUIClassName;// = _T("EditUI");
extern UILIB_API const TCHAR* const kEditUIInterfaceName;// = _T("Edit");

// IEditUI
extern UILIB_API const TCHAR* const kIEditUIInterfaceName;// = _T("Edit");

// ScrollBarUI
extern UILIB_API const TCHAR* const kScrollBarUIClassName;// = _T("ScrollBarUI");
extern UILIB_API const TCHAR* const kScrollBarUIInterfaceName;// = _T("ScrollBar");

// ContainerUI
extern UILIB_API const TCHAR* const kContainerUIClassName;// = _T("ContainerUI");
extern UILIB_API const TCHAR* const kContainerUIInterfaceName;// = _T("Container");

// IContainerUI
extern UILIB_API const TCHAR* const kIContainerUIInterfaceName;// = _T("IContainer");

// VerticalLayoutUI
extern UILIB_API const TCHAR* const kVerticalLayoutUIClassName;// = _T("VerticalLayoutUI");
extern UILIB_API const TCHAR* const kVerticalLayoutUIInterfaceName;// = _T("VerticalLayout");

// HorizontalLayoutUI
extern UILIB_API const TCHAR* const kHorizontalLayoutUIClassName;// = _T("HorizontalLayoutUI");
extern UILIB_API const TCHAR* const kHorizontalLayoutUIInterfaceName;// = _T("HorizontalLayout");

// TileLayoutUI
extern UILIB_API const TCHAR* const kTileLayoutUIClassName;// = _T("TileLayoutUI");
extern UILIB_API const TCHAR* const kTileLayoutUIInterfaceName;// = _T("TileLayout");

// DialogLayoutUI
extern UILIB_API const TCHAR* const kDialogLayoutUIClassName;// = _T("DialogLayoutUI");
extern UILIB_API const TCHAR* const kDialogLayoutUIInterfaceName;// = _T("DialogLayout");

// TabLayoutUI
extern UILIB_API const TCHAR* const kTabLayoutUIClassName;// = _T("TabLayoutUI");
extern UILIB_API const TCHAR* const kTabLayoutUIInterfaceName;// = _T("TabLayout");

// ControlUI
extern UILIB_API const TCHAR* const kControlUIClassName;// = _T("ControlUI");
extern UILIB_API const TCHAR* const kControlUIInterfaceName;// = _T("Control");

// ListUI
extern UILIB_API const TCHAR* const kListUIClassName;// = _T("ListUI");
extern UILIB_API const TCHAR* const kListUIInterfaceName;// = _T("CList");

// IListUI
extern UILIB_API const TCHAR* const kIListUIInterfaceName;// = _T("List");

// IListOwnerUI
extern UILIB_API const TCHAR* const kIListOwnerUIInterfaceName;// = _T("ListOwner");

// ListHeaderUI
extern UILIB_API const TCHAR* const kListHeaderUIClassName;// = _T("ListHeaderUI");
extern UILIB_API const TCHAR* const kListHeaderUIInterfaceName;// = _T("ListHeader");

// ListHeaderItemUI
extern UILIB_API const TCHAR* const kListHeaderItemUIClassName;// = _T("ListHeaderItemUI");
extern UILIB_API const TCHAR* const kListHeaderItemUIInterfaceName;// = _T("ListHeaderItem");

// ListElementUI
extern UILIB_API const TCHAR* const kListElementUIClassName;// = _T("ListElementUI");
extern UILIB_API const TCHAR* const kListElementUIInterfaceName;// = _T("ListElement");

// IListItemUI
extern UILIB_API const TCHAR* const kIListItemUIInterfaceName;// = _T("ListItem");

// ListLabelElementUI
extern UILIB_API const TCHAR* const kListLabelElementUIClassName;// = _T("ListLabelElementUI");
extern UILIB_API const TCHAR* const kListLabelElementUIInterfaceName;// = _T("ListLabelElement");

// ListTextElementUI
extern UILIB_API const TCHAR* const kListTextElementUIClassName;// = _T("ListTextElementUI");
extern UILIB_API const TCHAR* const kListTextElementUIInterfaceName;// = _T("ListTextElement");

// ListExpandElementUI
extern UILIB_API const TCHAR* const kListExpandElementUIClassName;// = _T("ListExpandElementUI");
extern UILIB_API const TCHAR* const kListExpandElementUIInterfaceName;// = _T("ListExpandElement");

// ListContainerElementUI
extern UILIB_API const TCHAR* const kListContainerElementUIClassName;// = _T("ListContainerElementUI");
extern UILIB_API const TCHAR* const kListContainerElementUIInterfaceName;// = _T("ListContainerElement");

// RichEditUI
extern UILIB_API const TCHAR* const kRichEditUIClassName;// = _T("RichEditUI");
extern UILIB_API const TCHAR* const kRichEditUIInterfaceName;// = _T("RichEdit");

/////////////////////////////////////////////////////////////////////////////////////
//
//
// control related message
extern UILIB_API const TCHAR* const kWindowInit;// = _T("windowinit");
extern UILIB_API const TCHAR* const kClick;// = _T("click");
extern UILIB_API const TCHAR* const kSelectChanged;// = _T("selectchanged");
extern UILIB_API const TCHAR* const kItemSelect;// = _T("itemselect");
extern UILIB_API const TCHAR* const kItemActivate;// = _T("itemactivate");
extern UILIB_API const TCHAR* const kItemClick;// = _T("itemclick");
extern UILIB_API const TCHAR* const kDropDown;// = _T("dropdown");
extern UILIB_API const TCHAR* const kTimer;// = _T("timer");
extern UILIB_API const TCHAR* const kMenu;// = _T("menu");
extern UILIB_API const TCHAR* const kReturn;// = _T("return");
extern UILIB_API const TCHAR* const kTextChanged;// = _T("textchanged");

}; // namespace DuiLib

#endif // _UICRACK_H_