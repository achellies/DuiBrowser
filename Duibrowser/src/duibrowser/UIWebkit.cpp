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
#include "UIlib.h"
#include "UIWebkit.hpp"

namespace DuiLib {
	
CWebkitUI::CWebkitUI()
: raster_(NULL)
, view_(NULL)
, bitmap_bits_(NULL)
, did_first_layout_(false)
{
    last_mouse_point_.x = 0;
    last_mouse_point_.y = 0;
}

CWebkitUI::~CWebkitUI()
{
	if (bitmap_bits_ != NULL)
		delete[] bitmap_bits_;
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

void CWebkitUI::SetEARasterAndView(IEARaster* raster, View* view)
{
	if (view_ == NULL)
	{
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
        view_->OnMouseButtonEvent(mouseEvent);
        return;
    }
    if( event.Type == UIEVENT_MOUSEMOVE )
    {
        MouseMoveEvent mouseEvent = {0};
        mouseEvent.mX = event.ptMouse.x - m_rcItem.left;
        mouseEvent.mY = event.ptMouse.y - m_rcItem.top;
        mouseEvent.mDX = event.ptMouse.x - last_mouse_point_.x;
        mouseEvent.mDY = event.ptMouse.y - last_mouse_point_.y;
        mouseEvent.mbShift = ((event.wKeyState & MK_SHIFT) != 0);
        mouseEvent.mbControl = ((event.wKeyState & MK_CONTROL) != 0);
        mouseEvent.mbAlt = ((event.wKeyState & MK_ALT) != 0);
        view_->OnMouseMoveEvent(mouseEvent);
        last_mouse_point_.x = mouseEvent.mX;
        last_mouse_point_.y = mouseEvent.mY;
        return;
    }
    if( event.Type == UIEVENT_SCROLLWHEEL )
    {
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
	StretchDIBits(hDC, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top, 
		0, 0, bitmap_header_info_.biWidth, bitmap_header_info_.biHeight, bitmap_bits_, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
}

} // namespace DuiLib