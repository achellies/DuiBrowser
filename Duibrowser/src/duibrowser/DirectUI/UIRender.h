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
#ifndef __UIRENDER_H__
#define __UIRENDER_H__

#ifdef _MSC_VER
#pragma once
#endif

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CRenderClip
{
public:
    ~CRenderClip();
    RECT rcItem;
    HDC hDC;
    HRGN hRgn;
    HRGN hOldRgn;

    static void GenerateClip(void* ctx, RECT rcItem, CRenderClip& clip);
	static void GenerateRoundClip(void* ctx, RECT rc, RECT rcItem, int width, int height, CRenderClip& clip);
    static void UseOldClipBegin(void* ctx, CRenderClip& clip);
    static void UseOldClipEnd(void* ctx, CRenderClip& clip);
};

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CRenderEngine
{
public:
    static TImageInfo* LoadImage(STRINGorID bitmap, CControlUI* pControl, CPaintManagerUI* pManager, STRINGorID type = (UINT) 0, DWORD mask = 0);
	static TImageInfo* LoadImage(STRINGorID bitmap, CPaintManagerUI* pManager, STRINGorID type = (UINT) 0, DWORD mask = 0);
    static void DrawImage(void* ctx, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, \
        const RECT& rcBmpPart, const RECT& rcCorners, bool alphaChannel, BYTE uFade = 255, 
        bool hole = false, bool xtiled = false, bool ytiled = false);
    static bool DrawImageString(void* ctx, CControlUI* pControl, CPaintManagerUI* pManager, const RECT& rcItem, const RECT& rcPaint, 
        LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);

	static bool DrawImage(void* ctx, CControlUI* pControl, CPaintManagerUI* pManager, const RECT& rcItem, const RECT& rcPaint, 
        LPCTSTR pStrImage);

	// 计算图片的大小
	static bool CaculateImageRect(LPCTSTR pStrImage, CControlUI* pControl, CPaintManagerUI* pManager, RECT& rcImage);

    static void DrawColor(void* ctx, const RECT& rc, DWORD color);
	static void DrawGradient(void* ctx, const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps);

    // 以下函数中的颜色参数alpha值无效    
    static void DrawLine(void* ctx, const RECT& rc, int nSize, DWORD dwPenColor);
    static void DrawRect(void* ctx, const RECT& rc, int nSize, DWORD dwPenColor);
	static void DrawRoundRect(void* ctx, const RECT& rc, int nSize, DWORD dwPenColor, DWORD nWidth, DWORD nHeight);
    static void DrawText(void* ctx, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, \
        DWORD dwTextColor, int iFont, UINT uStyle);
    static void DrawHtmlText(void* ctx, CControlUI* pControl, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, 
        DWORD dwTextColor, RECT* pLinks, CStdString* sLinks, int& nLinkRects, UINT uStyle);
    static HBITMAP GenerateBitmap(CPaintManagerUI* pManager, CControlUI* pControl, RECT rc);
};

} // namespace DuiLib

#endif // __UIRENDER_H__
