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
// WebViewPrivate.cpp
// By Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////

#include "config.h" 
#include "WebViewPrivate.h"
#include "IntRect.h"
#include "FrameView.h"
#include "Frame.h"
#include "GraphicsContext.h"
#include "Page.h"
#include "EventHandler.h"
#include "FocusController.h"
#include <PlatformKeyboardEvent.h>
#include <PlatformMouseEvent.h>
#include <PlatformWheelEvent.h>
#include "SelectionController.h"
#include "Editor.h"
#include "owb-config.h"
#include "PopupMenu.h"
#include "CString.h"
#include "FileIO.h"
#include <EAWebKit/EAWebKitInput.h>
#include <EAWebKit/EAWebKit.h>
#include "BAL/Includes/FakedDeepsee.h"


using namespace WebCore;


void WebViewPrivate::onExpose(BalEventExpose eventExpose)
{
    //OWB_PRINTF("WebViewPrivate::onExpose\n");
    Frame* frame = core(m_webView->mainFrame());
    if (!frame)
        return;

    if(!isInitialized) {
        //OWB_PRINTF("not isInitialized\n");
        isInitialized = true;
        frame->view()->resize(m_rect.width(), m_rect.height());
        frame->forceLayout();
        frame->view()->adjustViewSize();
    }

    GraphicsContext ctx(m_webView->viewWindow());
    ctx.setBalExposeEvent(&eventExpose);
    if (frame->contentRenderer() && frame->view() && !m_webView->dirtyRegion().isEmpty()) {
        frame->view()->layoutIfNeededRecursive();
        IntRect dirty = m_webView->dirtyRegion();
        
        // Notify the user of the draw start event.
        EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();

        EA::WebKit::View* pView = EA::WebKit::GetView(m_webView);
        
        // Clip to the main view surface and not just the dirty rect for notification.    
        int w=0;
        int h=0;
        if(pView)
            pView->GetSize(w,h);
        IntRect rect(0,0,w,h);
        rect.intersect(dirty);

        EA::WebKit::ViewUpdateInfo vui = { pView, rect.x(), rect.y(), rect.width(), rect.height(), EA::WebKit::ViewUpdateInfo::kViewDrawStart};
        if(pVN)
            pVN->DrawEvent(vui);

        frame->view()->paint(&ctx, dirty);

       // Notify the user of end draw event     
        if(pVN) {
            vui.mDrawEvent = EA::WebKit::ViewUpdateInfo::kViewDrawEnd;
            pVN->DrawEvent(vui);
            
           // TODO: Deprecate this view update 
          pVN->ViewUpdate(vui);
        }

        m_webView->clearDirtyRegion();
    }
}

void WebViewPrivate::onKeyDown(BalEventKey eventKey)
{
    using namespace EA::WebKit;
    
    // 7/20/09 CSidhall -Added for crash fix
    if(!m_webView || !m_webView->page() || !m_webView->page()->focusController() )
        return;

    Frame* frame = m_webView->page()->focusController()->focusedOrMainFrame();
    if (!frame)
        return;
    PlatformKeyboardEvent keyboardEvent(&eventKey);

    if (frame->eventHandler()->keyEvent(keyboardEvent))
        return;

    FrameView* view = frame->view();
    SelectionController::EAlteration alteration;
    if (eventKey.mId == kShift)
        alteration = SelectionController::EXTEND;
    else
        alteration = SelectionController::MOVE;

    switch (eventKey.mId) {
        case kArrowDown:
            view->scrollBy(0, LINE_STEP);
            break;
        case kArrowUp:
            view->scrollBy(0, -LINE_STEP);
            break;
        case kArrowRight:
            view->scrollBy(LINE_STEP, 0);
            break;
        case kArrowLeft:
            view->scrollBy(-LINE_STEP, 0);
            break;
        case kHome:
            frame->selection()->modify(alteration, SelectionController::BACKWARD, DocumentBoundary, true);
            break;
        case kEnd:
            frame->selection()->modify(alteration, SelectionController::FORWARD, DocumentBoundary, true);
            break;
        case kEscape:
            // On FireFox, esc causes animations to stop.
            break;
        case kF1:
            m_webView->goBack();
            view->update();
            break;
        case kF2:
            m_webView->goForward();
            view->update();
            break;
        default:
            break;
    }
}

void WebViewPrivate::onKeyUp(BalEventKey eventKey)
{
    Frame* frame = m_webView->page()->focusController()->focusedOrMainFrame();
    if (!frame)
        return;

    PlatformKeyboardEvent keyboardEvent(&eventKey);

    if (frame->eventHandler()->keyEvent(keyboardEvent))
        return ;
}

void WebViewPrivate::onMouseMotion(BalEventMotion eventMotion)
{
    Frame* frame = core(m_webView->mainFrame());
    if (!frame)
        return;

    frame->eventHandler()->mouseMoved(PlatformMouseEvent(&eventMotion, 0));
}

void WebViewPrivate::onMouseButtonDown(BalEventButton eventButton)
{
    using namespace EA::WebKit;

    Frame* frame = core(m_webView->mainFrame());

    if (!frame)
        return;

    if (eventButton.mId == kMouseMiddle)
        return; //webkit_web_view_forward_context_menu_event(webView, PlatformMouseEvent(event));

    frame->eventHandler()->handleMousePressEvent(PlatformMouseEvent(&eventButton));
}

void WebViewPrivate::onMouseButtonUp(BalEventButton eventButton)
{
    // 12/3/10 CSidhall - Have the release event use the same frame as the press event so the scroll can be notified.

    using namespace EA::WebKit;
    
    Frame* frame = core(m_webView->mainFrame());
     if (!frame)
        return; 
    
    if (eventButton.mId == kMouseMiddle)
        return; 

    frame->eventHandler()->handleMouseReleaseEvent(PlatformMouseEvent(&eventButton));
}

void WebViewPrivate::onScroll(BalEventScroll eventScroll)
{
    Frame* frame = core(m_webView->mainFrame());
    if (!frame)
        return;
    PlatformWheelEvent wheelEvent(&eventScroll);
    frame->eventHandler()->handleWheelEvent(wheelEvent);
}

void WebViewPrivate::onResize(BalResizeEvent eventResize)
{
    Frame* frame = core(m_webView->mainFrame());
    if (!frame)
        return;
    m_rect.setWidth(eventResize.w);
    m_rect.setHeight(eventResize.h);
    frame->view()->resize(eventResize.w, eventResize.h);
    frame->forceLayout();
    frame->view()->adjustViewSize();
}

void WebViewPrivate::onQuit(BalQuitEvent)
{
}

void WebViewPrivate::onUserEvent(BalUserEvent)
{
}

void WebViewPrivate::popupMenuHide()
{
    // This code is empty, as we currently handle popup menus from BCPopupMenuEA.h/cpp
    // Previously that code was calling this code and assuming that this code would
    // implement some platform-specific popup menu or implement some kind of JavaScript
    // which would implement a similar thing.
    // To consider: Move the code from BCPopupMenuEA.h/cpp to here.
}


void WebViewPrivate::popupMenuShow(void *popupInfo)
{
    // This code is empty, as we currently handle popup menus from BCPopupMenuEA.h/cpp
    // Previously that code was calling this code and assuming that this code would
    // implement some platform-specific popup menu or implement some kind of JavaScript
    // which would implement a similar thing.
    // To consider: Move the code from BCPopupMenuEA.h/cpp to here.
}




