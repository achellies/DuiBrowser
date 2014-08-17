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
#include "WebInspector.h"

#include "WebView.h"
#include <InspectorController.h>
#include <Page.h>
#include <wtf/Assertions.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

WebInspector* WebInspector::createInstance(WebView* webView)
{
    WebInspector* inspector = new WebInspector(webView);
    return inspector;
}

WebInspector::WebInspector(WebView* webView)
    : m_webView(webView)
{
    ASSERT_ARG(webView, webView);
    DS_CONSTRUCT();
}

WebInspector::~WebInspector()
{
    DS_DESTRUCT();
}

void WebInspector::webViewClosed()
{
    m_webView = 0;
}


void WebInspector::show()
{
    if (m_webView)
        if (Page* page = m_webView->page())
            page->inspectorController()->show();
}

void WebInspector::showConsole()
{
    if (m_webView)
        if (Page* page = m_webView->page())
            page->inspectorController()->showPanel(InspectorController::ConsolePanel);
}

void WebInspector::close()
{
    if (m_webView)
        if (Page* page = m_webView->page())
            page->inspectorController()->close();
}

void WebInspector::attach()
{
    if (m_webView)
        if (Page* page = m_webView->page())
            page->inspectorController()->attachWindow();
}

void WebInspector::detach()
{
    if (m_webView)
        if (Page* page = m_webView->page())
            page->inspectorController()->detachWindow();
}
