//
// UIWebkit.cpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2011 achellies (achellies at 163 dot com)
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//
#include "stdafx.h"
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebkitSTLWrapper.h>
#include <EAWebkit/WebkitLayout.h>
#include "UIlib.h"
#if defined(UNDER_CE)
#include "PostilMaker.h"
#endif
#include "UIWebkit.hpp"

namespace DuiLib {
	
CWebkitUI::CWebkitUI(
#if defined(UNDER_CE)
					 CPostilMaker& postilMaker
#endif
					 )
: raster_(NULL)
, view_(NULL)
, bitmap_bits_(NULL)
, webkit_layout_(NULL)
, using_webkit_layout_(false)
, did_first_layout_(false)
, left_button_down_(false)
, webkit_layout_height_(0)
#if defined(UNDER_CE)
, m_postilMaker(postilMaker)
#endif
{
    last_mouse_point_.x = 0;
    last_mouse_point_.y = 0;
}

CWebkitUI::~CWebkitUI()
{
	if (bitmap_bits_ != NULL)
		delete[] bitmap_bits_;
}

void CWebkitUI::UpdateWebkitLayoutInfo(bool using_webkit_layout, int webkit_layout_height, int font_size)
{
	if (g_iReaderData->m_nCurPage + 1 == g_iReaderData->m_nPagesCount)
		if (g_iReaderData->m_nPagesCount > 1)
			webkit_layout_height += font_size;
	using_webkit_layout_ = using_webkit_layout;
	webkit_layout_height_ = webkit_layout_height;
	
	LayoutChanged(CRect());	
}

UINT CWebkitUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}

bool CWebkitUI::LayoutChanged(RECT rc)
{
	did_first_layout_ = true;

	CRect invalidateRect = m_rcItem;
	if (!invalidateRect.IsNull())
	{
		invalidateRect.left += rc.left;
		invalidateRect.top += rc.top;
		invalidateRect.right = invalidateRect.left + rc.right;
		invalidateRect.bottom = invalidateRect.top + rc.bottom;
		InvalidateRect(GetManager()->GetPaintWindow(), &invalidateRect, TRUE);
	}

	return did_first_layout_;
}

void CWebkitUI::RestoreSurfaceBuffer()
{
	if (bitmap_bits_ != NULL)
		delete[] bitmap_bits_;
	bitmap_bits_ = NULL;

	if (view_ != NULL)
	{
		int mWidth = (m_rcItem.right - m_rcItem.left);
		mWidth = (mWidth + 3) / 4 * 4;
		int mHeight = (m_rcItem.bottom - m_rcItem.top);

		bitmap_header_info_.biSize			= sizeof(BITMAPINFOHEADER);
		bitmap_header_info_.biWidth			= mWidth;
		bitmap_header_info_.biHeight		= mHeight;
		bitmap_header_info_.biPlanes		= 1;
		bitmap_header_info_.biBitCount		= 32;
		bitmap_header_info_.biCompression	= 0;
		bitmap_header_info_.biSizeImage		= bitmap_header_info_.biWidth * bitmap_header_info_.biHeight * 4;
		bitmap_header_info_.biXPelsPerMeter	= 0;
		bitmap_header_info_.biYPelsPerMeter	= 0;
		bitmap_header_info_.biClrUsed		= 0;
		bitmap_header_info_.biClrImportant	= 0;

		bitmap_bits_ = new BYTE[bitmap_header_info_.biSizeImage];
		memset(bitmap_bits_, 0xFF, bitmap_header_info_.biSizeImage);
	}
}

void CWebkitUI::SetEARasterAndView(IEARaster* raster, View* view, IWebKitLayout* webkit_layout)
{
	if (view_ == NULL)
	{
		webkit_layout_ = webkit_layout;
		raster_ = raster;
		view_ = view;

		ViewParameters vp = view_->GetParameters();
		vp.mbTransparentBackground = false;
		vp.mWidth = (m_rcItem.right - m_rcItem.left);
		vp.mWidth = (vp.mWidth + 3) / 4 * 4;
		vp.mHeight = (m_rcItem.bottom - m_rcItem.top);
		view_->InitView(vp);

		RestoreSurfaceBuffer();
	}

	if (bitmap_bits_ == NULL)
	{
		bitmap_bits_ = new BYTE[bitmap_header_info_.biSizeImage];
		memset(bitmap_bits_, 0xFF, bitmap_header_info_.biSizeImage);
	}
}

void CWebkitUI::SetPos(RECT rc)
{
	CControlUI::SetPos(rc);

	int mWidth = (m_rcItem.right - m_rcItem.left);
	mWidth = (mWidth + 3) / 4 * 4;
	int mHeight = (m_rcItem.bottom - m_rcItem.top);

	if ((view_ != NULL) && ((bitmap_header_info_.biWidth != mWidth) || (bitmap_header_info_.biHeight != mHeight)))
	{
		view_->SetSize(mWidth, mHeight);
		RestoreSurfaceBuffer();
	}
}

void CWebkitUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->DoEvent(event);
        else CControlUI::DoEvent(event);
        return;
    }

	if( event.Type == UIEVENT_TIMER )
    {
        m_pManager->SendNotify(this, kTimer, event.wParam, event.lParam);
        return;
    }

	if (bitmap_bits_ == NULL) return;

    if( event.Type == UIEVENT_SETFOCUS ) 
    {
        view_->OnFocusChangeEvent(true);
    }
    if( event.Type == UIEVENT_KILLFOCUS ) 
    {
        view_->OnFocusChangeEvent(false);
    }
    if( event.Type == UIEVENT_CHAR )
    {
		if(using_webkit_layout_)
			return;

        KeyboardEvent keyEvent = {0};
        keyEvent.mId = event.chKey;
        keyEvent.mbChar = true;
        keyEvent.mbDepressed = false;
        keyEvent.mbShift = ((event.wKeyState & MK_SHIFT) != 0);
        keyEvent.mbControl = ((event.wKeyState & MK_CONTROL) != 0);
        keyEvent.mbAlt = ((event.wKeyState & MK_ALT) != 0);
        view_->OnKeyboardEvent(keyEvent);
    }
    if( event.Type == UIEVENT_KEYDOWN )
    {
		if(using_webkit_layout_)
		{
#if defined(UNDER_CE)
			if (g_iReaderData)
				PostMessage(g_iReaderData->m_hNofityWnd, g_uInterProcMsg, IREADER_KEYEVENT, event.chKey);
#endif
			return;
		}
		
        KeyboardEvent keyEvent = {0};
        keyEvent.mId = event.chKey;
        keyEvent.mbChar = false;
        keyEvent.mbDepressed = true;
        keyEvent.mbShift = ((event.wKeyState & MK_SHIFT) != 0);
        keyEvent.mbControl = ((event.wKeyState & MK_CONTROL) != 0);
        keyEvent.mbAlt = ((event.wKeyState & MK_ALT) != 0);
        view_->OnKeyboardEvent(keyEvent);
    }
    if( event.Type == UIEVENT_KEYUP )
    {
		if(using_webkit_layout_)
		{
#if defined(UNDER_CE)
			if (g_iReaderData)
				PostMessage(g_iReaderData->m_hNofityWnd, g_uInterProcMsg, IREADER_KEYEVENT, event.chKey);
#endif
			return;
		}

        KeyboardEvent keyEvent = {0};
        keyEvent.mId = event.chKey;
        keyEvent.mbChar = false;
        keyEvent.mbDepressed = false;
        keyEvent.mbShift = ((event.wKeyState & MK_SHIFT) != 0);
        keyEvent.mbControl = ((event.wKeyState & MK_CONTROL) != 0);
        keyEvent.mbAlt = ((event.wKeyState & MK_ALT) != 0);
        view_->OnKeyboardEvent(keyEvent);
    }
    if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_BUTTONUP )
    {
		if (event.Type == UIEVENT_BUTTONDOWN)
			left_button_down_ = true;

		if (using_webkit_layout_)
		{
			if (g_iReaderData->m_nCurPage + 1 == g_iReaderData->m_nPagesCount)
			{
				event.ptMouse.y += bitmap_header_info_.biHeight - webkit_layout_height_;
			}
		}

		MouseButtonEvent mouseEvent = {0};
		mouseEvent.mX = event.ptMouse.x - m_rcItem.left;
		mouseEvent.mY = event.ptMouse.y - m_rcItem.top;
		mouseEvent.mId = kMouseLeft;
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
			mouseEvent.mbDepressed = true;
		else
			mouseEvent.mbDepressed = false;

		mouseEvent.mbShift = ((event.wKeyState & MK_SHIFT) != 0);
		mouseEvent.mbControl = ((event.wKeyState & MK_CONTROL) != 0);
		mouseEvent.mbAlt = ((event.wKeyState & MK_ALT) != 0);
#if defined(UNDER_CE)
		if (g_iReaderData && g_iReaderData->m_bPostil && ((event.Type == UIEVENT_BUTTONDOWN) || (event.Type == UIEVENT_DBLCLICK) || (event.Type == UIEVENT_BUTTONUP)))
		{
			// do nothing
		}
		else if (g_iReaderData && (g_iReaderData->m_bTranslate || g_iReaderData->m_bExcerpt) )
#endif
		{
			view_->OnMouseButtonEvent(mouseEvent);
		}

#if defined(UNDER_CE)
		if (g_iReaderData && g_iReaderData->m_bPostil && (event.Type == UIEVENT_BUTTONDOWN)) {
			webkit_layout_->ClearTextSelection(view_);
			CPoint clickedPoint(mouseEvent.mX, mouseEvent.mY);
			m_postilMaker.TraceStart(clickedPoint, g_iReaderData->m_nPostilPenWidth, g_iReaderData->m_bErasePostil);
		}

		if (event.Type == UIEVENT_BUTTONUP)
		{
			if (left_button_down_ && g_iReaderData && !g_iReaderData->m_bPostil && (event.Type == UIEVENT_BUTTONUP))
			{
				LPWSTR buffer = webkit_layout_->GetSelectedText(view_);
				if (buffer != NULL)
				{
					g_iReaderData->m_hasSelectionText = true;
					LocalFree(buffer);

					if (g_iReaderData->m_bTranslate) {
						g_iReaderData->m_bSelectionTextPos = (mouseEvent.mY <= (m_rcItem.top + (m_rcItem.bottom - m_rcItem.top)/2))?true:false;
						PostMessage(g_iReaderData->m_hNofityWnd, g_uInterProcMsg, IREADER_TRANSLATE, (LPARAM)0);
					}
				}
			}
			else if (g_iReaderData)
				g_iReaderData->m_hasSelectionText = false;
		}	

		if (left_button_down_ && g_iReaderData && g_iReaderData->m_bPostil && (event.Type == UIEVENT_BUTTONUP)) {
			CPoint clickedPoint(mouseEvent.mX, mouseEvent.mY);
			m_postilMaker.TraceEnd(clickedPoint, g_iReaderData->m_nPostilPenWidth, g_iReaderData->m_bErasePostil);
			left_button_down_ = false;
			return;
		}

		if (event.Type == UIEVENT_BUTTONDOWN)
			left_button_down_ = true;
		else
			left_button_down_ = false;
#endif
        return;
    }
    if( event.Type == UIEVENT_MOUSEMOVE )
    {
#if defined(UNDER_CE)
		CPoint clickedPoint(event.ptMouse.x - m_rcItem.left, event.ptMouse.y - m_rcItem.top);
#endif
		if(using_webkit_layout_)
		{
			if (left_button_down_ && (event.ptMouse.y > m_rcItem.top + webkit_layout_height_))
				return;

			if (left_button_down_ && (event.ptMouse.y < m_rcItem.top))
				return;

			if (g_iReaderData->m_nCurPage + 1 == g_iReaderData->m_nPagesCount)
			{
				event.ptMouse.y += bitmap_header_info_.biHeight - webkit_layout_height_;

				if (left_button_down_)
					Invalidate();
			}
		}

		MouseMoveEvent mouseEvent = {0};
		mouseEvent.mX = event.ptMouse.x - m_rcItem.left;
		mouseEvent.mY = event.ptMouse.y - m_rcItem.top;
		mouseEvent.mDX = event.ptMouse.x - last_mouse_point_.x;
		mouseEvent.mDY = event.ptMouse.y - last_mouse_point_.y;
		mouseEvent.mbShift = ((event.wKeyState & MK_SHIFT) != 0);
		mouseEvent.mbControl = ((event.wKeyState & MK_CONTROL) != 0);
		mouseEvent.mbAlt = ((event.wKeyState & MK_ALT) != 0);
#if defined(UNDER_CE)
		if (g_iReaderData && g_iReaderData->m_bPostil)
		{
			// do nothing
		}
		else if (g_iReaderData && (g_iReaderData->m_bTranslate || g_iReaderData->m_bExcerpt) )
#endif
		{
			view_->OnMouseMoveEvent(mouseEvent);
		}
		last_mouse_point_.x = mouseEvent.mX;
		last_mouse_point_.y = mouseEvent.mY;

#if defined(UNDER_CE)
		if (left_button_down_ && g_iReaderData && g_iReaderData->m_bPostil) {
			m_postilMaker.AddPoint(clickedPoint, g_iReaderData->m_nPostilPenWidth, g_iReaderData->m_bErasePostil);
		}
#endif
        return;
    }
    if( event.Type == UIEVENT_SCROLLWHEEL )
    {
		if(using_webkit_layout_)
			return;

		MouseWheelEvent mouseEvent = {0};
		mouseEvent.mX = event.ptMouse.x - m_rcItem.left;
		mouseEvent.mY = event.ptMouse.y - m_rcItem.top;

		// default value is 3
		int uLineDelta = 3;
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &uLineDelta, 0);

        if( LOWORD(event.wParam) == SB_LINEUP ) mouseEvent.mLineDelta = static_cast<float>(uLineDelta);
        else mouseEvent.mLineDelta = static_cast<float>(-uLineDelta);
        mouseEvent.mbShift = ((event.wKeyState & MK_SHIFT) != 0);
        mouseEvent.mbControl = ((event.wKeyState & MK_CONTROL) != 0);
        mouseEvent.mbAlt = ((event.wKeyState & MK_ALT) != 0);
        view_->OnMouseWheelEvent(mouseEvent);
        return;
    }
}

void CWebkitUI::DoPaint(void* ctx, const RECT& rcPaint)
{
	if ((ctx == NULL) || (raster_ == NULL) || (bitmap_bits_ == NULL))
		return;
	HDC hDC = reinterpret_cast<HDC>(ctx);

    if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;

	if (did_first_layout_ && bitmap_bits_ != NULL)
	{
		memcpy_s(bitmap_bits_, bitmap_header_info_.biSizeImage, view_->GetSurface()->GetData(), bitmap_header_info_.biSizeImage);
	}
	else if (bitmap_bits_ != NULL)
	{
		memset(bitmap_bits_, 0xFF, bitmap_header_info_.biSizeImage);
	}

	::SetStretchBltMode(hDC, COLORONCOLOR);
	BITMAPINFOHEADER bmiHeader = bitmap_header_info_;
	bmiHeader.biHeight = -bmiHeader.biHeight;

	if (using_webkit_layout_)
	{
		bool IsEmbedded = false;
		CRect rcWindow;
		CRect rcParentWindow;

		if (::GetParent(m_pManager->GetPaintWindow()))
		{
			IsEmbedded = true;
			GetWindowRect(::GetParent(m_pManager->GetPaintWindow()), &rcWindow);
		}

		GetWindowRect(m_pManager->GetPaintWindow(), &rcWindow);		

		CRect rcPaintArea = m_rcItem;

		FillRect(hDC, &rcPaintArea, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));

		if (g_iReaderData->m_nCurPage + 1 == g_iReaderData->m_nPagesCount)
		{
#if 1
#if defined(UNDER_CE)
			StretchDIBits(hDC, rcPaintArea.left, rcPaintArea.top, rcPaintArea.right - rcPaintArea.left, webkit_layout_height_, 
				0, bitmap_header_info_.biHeight - webkit_layout_height_, bitmap_header_info_.biWidth, webkit_layout_height_, bitmap_bits_, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
#else
			StretchDIBits(hDC, rcPaintArea.left, rcPaintArea.top, rcPaintArea.right - rcPaintArea.left, webkit_layout_height_, 
				0, 1, bitmap_header_info_.biWidth, webkit_layout_height_, bitmap_bits_, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
#endif
			//StretchDIBits(hDC, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top, 
			//	0, 0, bitmap_header_info_.biWidth, bitmap_header_info_.biHeight, bitmap_bits_, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);

			//MoveToEx(hDC, m_rcItem.left, m_rcItem.top + bitmap_header_info_.biHeight - webkit_layout_height_, NULL);
			//LineTo(hDC, m_rcItem.right, m_rcItem.top + bitmap_header_info_.biHeight - webkit_layout_height_);
#else
			HDC doubleBufferDC = CreateCompatibleDC(hDC);
			HBITMAP doubleBufferBitmap = CreateCompatibleBitmap(hDC, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);

			SelectObject(doubleBufferDC, doubleBufferBitmap);

			StretchDIBits(doubleBufferDC, 0, 0, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top, 
				0, 0, bitmap_header_info_.biWidth, bitmap_header_info_.biHeight, bitmap_bits_, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);

			BitBlt(hDC, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, webkit_layout_height_, doubleBufferDC, 0, m_rcItem.bottom - m_rcItem.top - webkit_layout_height_, SRCCOPY);

			DeleteDC(doubleBufferDC);
			DeleteObject(doubleBufferBitmap);
#endif
		}
		else
			StretchDIBits(hDC, rcPaintArea.left, rcPaintArea.top, rcPaintArea.right - rcPaintArea.left, webkit_layout_height_, 
				0, 0, bitmap_header_info_.biWidth, webkit_layout_height_, bitmap_bits_, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);

		//if (g_iReaderData->m_nPagesCount > 1)
		//{
		//	CRect rcVerticalScrollbar = rcPaintArea;
		//	rcVerticalScrollbar.left = rcVerticalScrollbar.right - 15;
		//	FillRect(hDC, &rcVerticalScrollbar, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
		//}

		if (webkit_layout_height_ < bitmap_header_info_.biHeight)
		{
			CRect rcUnPaintArea = rcPaintArea;
			rcUnPaintArea.top = rcUnPaintArea.bottom - (bitmap_header_info_.biHeight - webkit_layout_height_);
			FillRect(hDC, &rcUnPaintArea, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
		}

#if defined(UNDER_CE)
		m_postilMaker.Draw(hDC);

		if (g_iReaderData->m_bHasBookmark) {
			RECT rcLabel;
			rcLabel.left = m_rcItem.right - 40;
			rcLabel.right = m_rcItem.right - 20;
			rcLabel.top = m_rcItem.top;
			rcLabel.bottom = rcLabel.top + 20;

			HBRUSH hGrayBrush = (HBRUSH) GetStockObject(GRAY_BRUSH); 

			POINT pt[4];
			pt[0].x = rcLabel.left;
			pt[0].y = rcLabel.top;
			pt[1].x = rcLabel.right;
			pt[1].y = rcLabel.top;
			pt[2].x = rcLabel.right;
			pt[2].y = rcLabel.bottom;
			pt[3] = pt[0];
			SelectObject(hDC,hGrayBrush);
			Polygon(hDC,pt,4);

			HPEN hGrayPen = (HPEN)CreatePen(PS_SOLID,1,RGB(128,128,128));
			HPEN HOldPen = (HPEN)SelectObject(hDC,hGrayPen);
			MoveToEx(hDC,rcLabel.left,rcLabel.top,NULL);
			LineTo(hDC,rcLabel.left,rcLabel.bottom);

			LineTo(hDC,rcLabel.right,rcLabel.bottom);

			SelectObject(hDC,HOldPen);
			DeleteObject(hGrayPen);
		}
#endif
	}
	else
		StretchDIBits(hDC, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top, 
		0, 0, bitmap_header_info_.biWidth, bitmap_header_info_.biHeight, bitmap_bits_, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
}

} // namespace DuiLib