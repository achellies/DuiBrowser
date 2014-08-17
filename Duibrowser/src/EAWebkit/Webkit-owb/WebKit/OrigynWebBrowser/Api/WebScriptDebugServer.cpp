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
#include "WebScriptDebugServer.h"

#include "WebScriptCallFrame.h"
#include "WebView.h"

#include <DOMWindow.h>
#include <JSDOMWindow.h>
#include <JavaScriptDebugServer.h>

#include <kjs/ExecState.h>
#include <wtf/Assertions.h>
#include <wtf/Vector.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace KJS;
using namespace WebCore;

//typedef HashSet<WebScriptDebugListener*> ListenerSet;

//static ListenerSet s_Listeners;
// static unsigned s_ListenerCount = 0;
static bool s_dying = false;

/*static Frame* frame(ExecState* exec)
{
    JSDOMWindow* window = static_cast<JSDOMWindow*>(exec->dynamicGlobalObject());
    return window->impl()->frame();
}*/

/*static WebFrame* webFrame(ExecState* exec)
{
    return kit(frame(exec));
}

static WebView* webView(ExecState* exec)
{
    return kit(frame(exec)->page());
}*/

//unsigned WebScriptDebugServer::listenerCount() { return s_ListenerCount; };

WebScriptDebugServer::WebScriptDebugServer()
    : m_paused(false)
    , m_step(false)
{
    DS_CONSTRUCT();
}

WebScriptDebugServer::~WebScriptDebugServer()
{
    DS_DESTRUCT();
}

WebScriptDebugServer* WebScriptDebugServer::createInstance()
{
    WebScriptDebugServer* instance = new WebScriptDebugServer;
    return instance;
}

WebScriptDebugServer* WebScriptDebugServer::sharedWebScriptDebugServer()
{
    static WebScriptDebugServer* server;

    if (!server) {
        s_dying = false;
        server = WebScriptDebugServer::createInstance();
    }

    return server;
}


void WebScriptDebugServer::addListener(WebScriptDebugListener* listener)
{
/*
    if (!s_ListenerCount)
        JavaScriptDebugServer::shared().addListener(this);

    ++s_ListenerCount;
    s_Listeners.add(listener);*/
}

void WebScriptDebugServer::removeListener(WebScriptDebugListener* listener)
{
    /*if (!s_Listeners.contains(listener))
        return ;

    s_Listeners.remove(listener);

    ASSERT(s_ListenerCount >= 1);
    if (--s_ListenerCount == 0) {
        JavaScriptDebugServer::shared().removeListener(this);
        resume();
    }*/
}

void WebScriptDebugServer::step()
{
    m_step = true;
    m_paused = false;
}

void WebScriptDebugServer::pause()
{
    m_paused = true;
    m_step = false;
}

void WebScriptDebugServer::resume()
{
    m_paused = false;
    m_step = false;
}

bool WebScriptDebugServer::isPaused()
{
    return m_paused;
}


/*void WebScriptDebugServer::suspendProcessIfPaused()
{
    static bool alreadyHere = false;

    if (alreadyHere)
        return;

    alreadyHere = true;

    // We only deliver messages to COM's message window to pause the process while still allowing RPC to work.
    // FIXME: It would be nice if we could keep delivering WM_[NC]PAINT messages to all windows to keep them painting on XP.

//     HWND messageWindow = comMessageWindow();
// 
//     MSG msg;
//     while (m_paused && GetMessage(&msg, messageWindow, 0, 0)) {
//         TranslateMessage(&msg);
//         DispatchMessage(&msg);
//     }

    if (m_step) {
        m_step = false;
        m_paused = true;
    }

    alreadyHere = false;
}

void WebScriptDebugServer::didLoadMainResourceForDataSource(WebView* webView, WebDataSource* dataSource)
{
    if (!webView || !dataSource)
        return;

    ListenerSet listenersCopy = s_Listeners;
    ListenerSet::iterator end = listenersCopy.end();
    for (ListenerSet::iterator it = listenersCopy.begin(); it != end; ++it)
        (*it)->didLoadMainResourceForDataSource(webView, dataSource);
}

void WebScriptDebugServer::didParseSource(ExecState* exec, const UString& source, int startingLineNumber, const UString& sourceURL, int sourceID)
{
    ListenerSet listenersCopy = s_Listeners;
    ListenerSet::iterator end = listenersCopy.end();
    for (ListenerSet::iterator it = listenersCopy.begin(); it != end; ++it)
        (*it)->didParseSource(webView(exec), source, startingLineNumber, sourceURL, sourceID, webFrame(exec));
}

void WebScriptDebugServer::failedToParseSource(ExecState* exec, const UString& source, int startingLineNumber, const UString& sourceURL, int errorLine, const UString& errorMessage)
{
    // FIXME: the error var should be made with the information in the errorMsg.  It is not a simple
    // UString to BSTR conversion there is some logic involved that I don't fully understand yet.
    String error("An Error Occurred.");

    ListenerSet listenersCopy = s_Listeners;
    ListenerSet::iterator end = listenersCopy.end();
    for (ListenerSet::iterator it = listenersCopy.begin(); it != end; ++it)
        (*it)->failedToParseSource(webView(exec), source, startingLineNumber, sourceURL, error, webFrame(exec));
}

void WebScriptDebugServer::didEnterCallFrame(ExecState* exec, int sourceID, int lineNumber)
{
    WebScriptCallFrame* callFrame = WebScriptCallFrame::createInstance(exec);
    ListenerSet listenersCopy = s_Listeners;
    ListenerSet::iterator end = listenersCopy.end();
    for (ListenerSet::iterator it = listenersCopy.begin(); it != end; ++it)
        (*it)->didEnterCallFrame(webView(exec), callFrame, sourceID, lineNumber, webFrame(exec));

    suspendProcessIfPaused();
}

void WebScriptDebugServer::willExecuteStatement(ExecState* exec, int sourceID, int lineNumber)
{
    WebScriptCallFrame* callFrame = WebScriptCallFrame::createInstance(exec);
    ListenerSet listenersCopy = s_Listeners;
    ListenerSet::iterator end = listenersCopy.end();
    for (ListenerSet::iterator it = listenersCopy.begin(); it != end; ++it)
        (*it)->willExecuteStatement(webView(exec), callFrame, sourceID, lineNumber, webFrame(exec));

    suspendProcessIfPaused();
}

void WebScriptDebugServer::willLeaveCallFrame(ExecState* exec, int sourceID, int lineNumber)
{
    WebScriptCallFrame* callFrame = WebScriptCallFrame::createInstance(exec->callingExecState());
    ListenerSet listenersCopy = s_Listeners;
    ListenerSet::iterator end = listenersCopy.end();
    for (ListenerSet::iterator it = listenersCopy.begin(); it != end; ++it)
        (*it)->willLeaveCallFrame(webView(exec), callFrame, sourceID, lineNumber, webFrame(exec));

    suspendProcessIfPaused();
}

void WebScriptDebugServer::exceptionWasRaised(ExecState* exec, int sourceID, int lineNumber)
{
    WebScriptCallFrame* callFrame = WebScriptCallFrame::createInstance(exec);
    ListenerSet listenersCopy = s_Listeners;
    ListenerSet::iterator end = listenersCopy.end();
    for (ListenerSet::iterator it = listenersCopy.begin(); it != end; ++it)
        (*it)->exceptionWasRaised(webView(exec), callFrame, sourceID, lineNumber, webFrame(exec));

    suspendProcessIfPaused();
}

void WebScriptDebugServer::serverDidDie()
{
    s_dying = true;

    ListenerSet listenersCopy = s_Listeners;
    ListenerSet::iterator end = listenersCopy.end();
    for (ListenerSet::iterator it = listenersCopy.begin(); it != end; ++it) {
        (*it)->serverDidDie();
        s_Listeners.remove((*it));
    }
}*/
