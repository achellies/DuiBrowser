/*
 * Copyright (C) 2008 Pleyo.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Pleyo nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY PLEYO AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PLEYO OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#include "config.h"
#include "WebDropSource.h"

#include "WebView.h"

#include <DragActions.h>
#include <EventHandler.h>
#include <Frame.h>
#include <Page.h>
#include <PlatformMouseEvent.h>
#include <SystemTime.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;


WebDropSource* WebDropSource::createInstance(WebView* webView)
{
    return new WebDropSource(webView);
}

WebDropSource::WebDropSource(WebView* webView)
: m_dropped(false) 
, m_webView(webView)
{
    DS_CONSTRUCT();
}

WebDropSource::~WebDropSource()
{
    DS_DESTRUCT();
}

PlatformMouseEvent generateMouseEvent(WebView* webView, bool isDrag) {
/*    POINTL pt;
    ::GetCursorPos((LPPOINT)&pt);
    POINTL localpt = pt;
    HWND viewWindow;
    if (SUCCEEDED(webView->viewWindow((OLE_HANDLE*)&viewWindow)))
        ::ScreenToClient(viewWindow, (LPPOINT)&localpt);
    return PlatformMouseEvent(IntPoint(localpt.x, localpt.y), IntPoint(pt.x, pt.y),
        isDrag ? LeftButton : NoButton, MouseEventMoved, 0, false, false, false, false, currentTime());*/
    return PlatformMouseEvent();
}

/*STDMETHODIMP WebDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
    if(fEscapePressed)
        return DRAGDROP_S_CANCEL;

    if(!(grfKeyState & (MK_LBUTTON|MK_RBUTTON))) {
        m_dropped = true;
        if (Page* page = m_webView->page())
            if (Frame* frame = page->mainFrame()) 
                //FIXME: We need to figure out how to find out what actually happened in the drag <rdar://problem/5015961>
                frame->eventHandler()->dragSourceEndedAt(generateMouseEvent(m_webView.get(), false), DragOperationCopy);
        return DRAGDROP_S_DROP;
    } else if (Page* page = m_webView->page())
        if (Frame* frame = page->mainFrame()) 
            frame->eventHandler()->dragSourceMovedTo(generateMouseEvent(m_webView.get(), true));

    return S_OK;

}

STDMETHODIMP WebDropSource::GiveFeedback(DWORD)
{
    return DRAGDROP_S_USEDEFAULTCURSORS;
}*/
