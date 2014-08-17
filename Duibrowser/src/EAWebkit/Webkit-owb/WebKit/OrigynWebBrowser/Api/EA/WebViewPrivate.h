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
// WebViewPrivate.h
// By Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#ifndef WebViewPrivate_H
#define WebViewPrivate_H

#include <wtf/FastAllocBase.h>

#include "WebView.h"
#include "IntRect.h"
#include "FrameView.h"
#include "Frame.h"
#include "BALBase.h"
#include "BAL/Includes/FakedDeepsee.h"


class WebViewPrivate/*: public WTF::FastAllocBase*/ {
public:
    WebViewPrivate(WebView *webView):m_webView(webView), isInitialized(false)
    {
        DS_CONSTRUCT();
    }

    ~WebViewPrivate() 
    {
        DS_DESTRUCT();
    }

    void show()
    {
    }
    
    void hide()
    {
    }

    void setFrameRect(WebCore::IntRect r)
    {
        m_rect = r;
    }

    WebCore::IntRect frameRect()
    { 
        return m_rect; 
    }
    
    BalWidget *createWindow(int x, int y, int width, int height)
    {
        const WebCore::IntRect rect(x, y, width, height);

        if(rect != m_rect)
            m_rect = rect;

        return 0;
    }

    void initWithFrameView(WebCore::FrameView *frameView)
    {
    }

    void onExpose(BalEventExpose eventExpose);
    void onKeyDown(BalEventKey eventKey);
    void onKeyUp(BalEventKey eventKey);
    void onMouseMotion(BalEventMotion eventMotion);
    void onMouseButtonDown(BalEventButton eventMotion);
    void onMouseButtonUp(BalEventButton eventButton);
    void onScroll(BalEventScroll eventScroll);
    void onResize(BalResizeEvent eventResize);
    void onQuit(BalQuitEvent);
    void onUserEvent(BalUserEvent);
    void popupMenuHide();
    void popupMenuShow(void *popupInfo);
private:
    WebCore::IntRect m_rect;
    WebView*         m_webView;
    bool             isInitialized;
};


#endif
