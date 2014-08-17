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
#include <DragClient.h>
#include "BALBase.h"

class WebView;

typedef enum WebDragDestinationAction {
    WebDragDestinationActionNone    = 0,
    WebDragDestinationActionDHTML   = 1,
    WebDragDestinationActionEdit    = 2,
    WebDragDestinationActionLoad    = 4,
    WebDragDestinationActionAny     = (unsigned long)-1
} WebDragDestinationAction;


typedef enum WebDragSourceAction {
    WebDragSourceActionNone         = 0,
    WebDragSourceActionDHTML        = 1,
    WebDragSourceActionImage        = 2,
    WebDragSourceActionLink         = 4,
    WebDragSourceActionSelection    = 8,
    WebDragSourceActionAny          = (unsigned long)-1
} WebDragSourceAction;

class WebDragClient : public WebCore::DragClient {
public:
    WebDragClient(WebView*);
    virtual ~WebDragClient();
    virtual WebCore::DragDestinationAction actionMaskForDrag(WebCore::DragData*);
    virtual void willPerformDragDestinationAction(WebCore::DragDestinationAction, WebCore::DragData*);
    virtual void dragControllerDestroyed();
    virtual WebCore::DragSourceAction dragSourceActionMaskForPoint(const WebCore::IntPoint&);
    virtual void willPerformDragSourceAction(WebCore::DragSourceAction, const WebCore::IntPoint&, WebCore::Clipboard*);
    virtual void startDrag(WebCore::DragImageRef, const WebCore::IntPoint&, const WebCore::IntPoint&, WebCore::Clipboard*, WebCore::Frame*, bool);
    virtual WebCore::DragImageRef createDragImageForLink(WebCore::KURL&, const WebCore::String&, WebCore::Frame*);
private:
    WebView* m_webView;
};

