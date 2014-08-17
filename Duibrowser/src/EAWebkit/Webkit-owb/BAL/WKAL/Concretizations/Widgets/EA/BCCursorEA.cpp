/*
Copyright (C) 2008-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


///////////////////////////////////////////////////////////////////////////////
// BCCursorEA.cpp
// By Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include <wtf/Assertions.h>
#include "stdio.h"
#include "Cursor.h"
#include "BALBase.h"
#include <EABase/eabase.h>
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/internal/EAWebKitViewHelper.h> // For multiview support

#if EAWEBKIT_DEFAULT_CURSOR_HANDLING_ENABLED && defined(EA_PLATFORM_WINDOWS)
    #include <windows.h>
#elif EAWEBKIT_DEFAULT_CURSOR_HANDLING_ENABLED && defined(EA_PLATFORM_WINCE)
	#include <windows.h>
#endif


namespace WKAL {


Cursor gCursors[kCursorIdCount];


Cursor::Cursor(const Cursor& other)
    : m_impl(other.m_impl)
{
    // Do we need to check that the cursor isn't already set?
    Set();
}


Cursor::Cursor(PlatformCursor c)
    : m_impl(c)
{
    m_impl = c;
    Set();         // It seems odd that we set the cursor in its ctor like this, but it's what WebKit expects.
}


void Cursor::Set() const
{
    bool bHandled = false;
    EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();

    if(pVN)
    {
        EA::WebKit::CursorChangeInfo cci = {EA::WebKit::AutoSetActiveView::GetActiveView(), m_impl }; // m_impl is an int, the cursor id.
		cci.mpView->GetCursorPosition(cci.mX, cci.mY);
        bHandled = pVN->CursorChanged(cci);
    }

    if(!bHandled)
    {
        #if EAWEBKIT_DEFAULT_CURSOR_HANDLING_ENABLED && defined(EA_PLATFORM_WINDOWS)
            LPCTSTR name = IDC_ARROW;

            switch (m_impl)
            {
                case kCursorIdPointer:
                    name = IDC_ARROW;
                    break;

                case kCursorIdCross:
                    name = IDC_CROSS;
                    break;

                case kCursorIdHand:
                    name = IDC_HAND;
                    break;

                case kCursorIdMove:

                case kCursorIdIBeam:
                    name = IDC_IBEAM;
                    break;

                case kCursorIdWait:
                    name = IDC_WAIT;
                    break;

                case kCursorIdHelp:
                    name = IDC_HELP;
                    break;

                case kCursorIdResizeE:
                case kCursorIdResizeW:
                    name = IDC_SIZEWE;
                    break;

                case kCursorIdResizeN:
                case kCursorIdResizeS:
                    name = IDC_SIZENS;
                    break;

                case kCursorIdResizeNE:
                case kCursorIdResizeNW:
                case kCursorIdResizeSE:
                case kCursorIdResizeSW:
                case kCursorIdResizeNS:
                case kCursorIdResizeEW:
                    // No simple solution.
                    break;

                case kCursorIdResizeNESW:
                    name = IDC_SIZENESW;
                    break;

                case kCursorIdResizeNWSE:
                    name = IDC_SIZENWSE;
                    break;

                case kCursorIdContextMenu:
                    // No simple solution.
                    break;

                case kCursorIdNoDrop:
                case kCursorIdNotAllowed:
                    name = IDC_NO;
                    break;

                case kCursorIdProgress:
                case kCursorIdZAlias:
                case kCursorIdZoomIn:
                case kCursorIdZoomOut:
                case kCursorIdCopy:
                case kCursorIdNone:
                    // No simple solution.
                    break;

                case kCursorIdResizeColumn:
                case kCursorIdResizeRow:
                case kCursorIdVerticalText:
                case kCursorIdCell:
                    // No simple solution.
                    break;
            }

            SetCursor(LoadCursor(NULL, name));
		#elif EAWEBKIT_DEFAULT_CURSOR_HANDLING_ENABLED && defined(EA_PLATFORM_WINCE)
            LPCTSTR name = IDC_ARROW;

            switch (m_impl)
            {
                case kCursorIdPointer:
                    name = IDC_ARROW;
                    break;

                case kCursorIdCross:
                    name = IDC_CROSS;
                    break;

                case kCursorIdHand:
                    name = IDC_HAND;
                    break;

                case kCursorIdMove:

                case kCursorIdIBeam:
                    name = IDC_IBEAM;
                    break;

                case kCursorIdWait:
                    name = IDC_WAIT;
                    break;

                case kCursorIdHelp:
                    name = IDC_HELP;
                    break;

                case kCursorIdResizeE:
                case kCursorIdResizeW:
                    name = IDC_SIZEWE;
                    break;

                case kCursorIdResizeN:
                case kCursorIdResizeS:
                    name = IDC_SIZENS;
                    break;

                case kCursorIdResizeNE:
                case kCursorIdResizeNW:
                case kCursorIdResizeSE:
                case kCursorIdResizeSW:
                case kCursorIdResizeNS:
                case kCursorIdResizeEW:
                    // No simple solution.
                    break;

                case kCursorIdResizeNESW:
                    name = IDC_SIZENESW;
                    break;

                case kCursorIdResizeNWSE:
                    name = IDC_SIZENWSE;
                    break;

                case kCursorIdContextMenu:
                    // No simple solution.
                    break;

                case kCursorIdNoDrop:
                case kCursorIdNotAllowed:
                    name = IDC_NO;
                    break;

                case kCursorIdProgress:
                case kCursorIdZAlias:
                case kCursorIdZoomIn:
                case kCursorIdZoomOut:
                case kCursorIdCopy:
                case kCursorIdNone:
                    // No simple solution.
                    break;

                case kCursorIdResizeColumn:
                case kCursorIdResizeRow:
                case kCursorIdVerticalText:
                case kCursorIdCell:
                    // No simple solution.
                    break;
            }

            SetCursor(LoadCursor(NULL, name));
        #else
            // To do: Implement a user-callback mechanism whereby the user handles this in their graphic/UI system.
        #endif
    }
}


const Cursor& enumeratedCursor(CursorId id)
{
    gCursors[id].m_impl = id;  // gCursors[id].m_impl is always equal to id by defintion.
    return gCursors[id];
}



} // namespace
