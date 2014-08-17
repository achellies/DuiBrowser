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
#include "WebChromeClient.h"

#include "WebElementPropertyBag.h"
#include "WebFrame.h"
#include "WebHistory.h"
#include "WebMutableURLRequest.h"
#include "WebSecurityOrigin.h"
#include "WebView.h"

#include <PlatformString.h>
#include "CString.h"
#include <ContextMenu.h>
#include <FloatRect.h>
#include <FrameLoadRequest.h>
#include <FrameView.h>
#include <Page.h>
#include <WindowFeatures.h>
#include "BAL/Includes/FakedDeepsee.h"
#include <cstdio>
#include <EAWebKit/EAWebKitView.h>
using namespace WebCore;

WebChromeClient::WebChromeClient(WebView* webView)
    : m_webView(webView)
{
    DS_CONSTRUCT();
}

WebChromeClient::~WebChromeClient()
{
    DS_DESTRUCT();
}

void WebChromeClient::chromeDestroyed()
{
    delete this;
}

void WebChromeClient::setWindowRect(const FloatRect& r)
{
}

FloatRect WebChromeClient::windowRect()
{
    return FloatRect();
}

FloatRect WebChromeClient::pageRect()
{
    return m_webView->frameRect();
}

float WebChromeClient::scaleFactor()
{
    // Windows doesn't support UI scaling.
    return 1.0;
}

void WebChromeClient::focus()
{
}

void WebChromeClient::unfocus()
{
}

bool WebChromeClient::canTakeFocus(FocusDirection direction)
{
    return false;
}

void WebChromeClient::takeFocus(FocusDirection direction)
{
}

Page* WebChromeClient::createWindow(Frame*, const FrameLoadRequest& frameLoadRequest, const WindowFeatures& features)
{
    if (features.dialog) {
        /*COMPtr<IWebUIDelegate3> delegate = uiDelegate3();
        if (!delegate)
            return 0;
        COMPtr<IWebMutableURLRequest> request(AdoptCOM, WebMutableURLRequest::createInstance(frameLoadRequest.resourceRequest()));
        COMPtr<IWebView> dialog;
        if (FAILED(delegate->createModalDialog(m_webView, request.get(), &dialog)))
            return 0;
        return core(dialog.get());*/
        return 0;
    }

    Page* page = 0;
    //IWebUIDelegate* uiDelegate = 0;
    /*WebMutableURLRequest* request = WebMutableURLRequest::createInstance(frameLoadRequest.resourceRequest());

    if (SUCCEEDED(m_webView->uiDelegate(&uiDelegate))) {
        IWebView* webView = 0;
        if (SUCCEEDED(uiDelegate->createWebViewWithRequest(m_webView, request, &webView))) {
            page = core(webView);
            webView->Release();
        }
    
        uiDelegate->Release();
    }
	delete request;
	*/
	
	EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
	if(pViewNotification)
	{
		EA::WebKit::CreateViewInfo createViewInfo;
		createViewInfo.mpView = EA::WebKit::GetView(m_webView);

		if(features.xSet)
			createViewInfo.mLeft = (uint16_t)features.x;

		if(features.ySet)
			createViewInfo.mTop	= (uint16_t)features.y;

		if(features.widthSet)
			createViewInfo.mWidth = (uint16_t)features.width;

		if(features.heightSet)
			createViewInfo.mHeight = (uint16_t)features.height;

		createViewInfo.mResizable = features.resizable;
		createViewInfo.mScrollBars = features.scrollbarsVisible;

		createViewInfo.mFromJavaScript = true;


		if(pViewNotification->CreateView(createViewInfo))
		{
			if(createViewInfo.mpCreatedView)
			{
				page = createViewInfo.mpCreatedView->GetPage();
			}
		}
	}


    return page;
}

void WebChromeClient::show()
{
}

bool WebChromeClient::canRunModal()
{
    return false;
}

void WebChromeClient::runModal()
{
}

void WebChromeClient::setToolbarsVisible(bool visible)
{
}

bool WebChromeClient::toolbarsVisible()
{
    return false;
}

void WebChromeClient::setStatusbarVisible(bool visible)
{
}

bool WebChromeClient::statusbarVisible()
{
    return false;
}

void WebChromeClient::setScrollbarsVisible(bool b)
{
    WebFrame* webFrame = m_webView->topLevelFrame();
    if (webFrame) {
        webFrame->setAllowsScrolling(b);
        FrameView* frameView = core(webFrame)->view();
        frameView->setHScrollbarMode(frameView->hScrollbarMode());  // I know this looks weird but the call to v/hScrollbarMode goes to ScrollView
        frameView->setVScrollbarMode(frameView->vScrollbarMode());  // and the call to setV/hScrollbarMode goes to FrameView.
                                                                    // This oddity is a result of matching a design in the mac code.
    }
}

bool WebChromeClient::scrollbarsVisible()
{
    WebFrame* webFrame = m_webView->topLevelFrame();
    bool b = false;
    if (webFrame)
        b = webFrame->allowsScrolling();

    return !!b;
}

void WebChromeClient::setMenubarVisible(bool visible)
{
}

bool WebChromeClient::menubarVisible()
{
    return false;
}

void WebChromeClient::setResizable(bool resizable)
{
}

void WebChromeClient::addMessageToConsole(const String& message, unsigned line, const String& url)
{
    //OWB_PRINTF("CONSOLE MESSAGE: line %d: %s\n", line, message.utf8().data());
}

bool WebChromeClient::canRunBeforeUnloadConfirmPanel()
{
    return false;
}

bool WebChromeClient::runBeforeUnloadConfirmPanel(const String& message, Frame* frame)
{
    //OWB_PRINTF("runBeforeUnloadConfirmPanel: %s\n", message.utf8().data());
    return false;
}

void WebChromeClient::closeWindowSoon()
{
    // We need to remove the parent WebView from WebViewSets here, before it actually
    // closes, to make sure that JavaScript code that executes before it closes
    // can't find it. Otherwise, window.open will select a closed WebView instead of 
    // opening a new one <rdar://problem/3572585>.

    // We also need to stop the load to prevent further parsing or JavaScript execution
    // after the window has torn down <rdar://problem/4161660>.
  
    // FIXME: This code assumes that the UI delegate will respond to a webViewClose
    // message by actually closing the WebView. Safari guarantees this behavior, but other apps might not.
    // This approach is an inherent limitation of not making a close execute immediately
    // after a call to window.close.

    m_webView->setGroupName("");
    m_webView->mainFrame()->stopLoading();
    m_webView->closeWindowSoon();
}

void WebChromeClient::runJavaScriptAlert(Frame *frame, const String& message)
{		
	OWB_PRINTF_FORMATTED("Javascript Alert: %s (from frame %p)\n", message.utf8().data(), frame);	    
}

bool WebChromeClient::runJavaScriptConfirm(Frame *frame, const String& message)
{
	OWB_PRINTF_FORMATTED("Javascript Confirm: %s (from frame %p), answer is 'false' by default.\n", message.utf8().data(), frame);    
	return false;
}

bool WebChromeClient::runJavaScriptPrompt(Frame *frame, const String& message, const String& defaultValue, String& result)
{
	OWB_PRINTF_FORMATTED("Javascript Prompt: %s (from frame %p), answer is 'false' by default.\n", message.utf8().data(), frame);	  
    return false;
}

void WebChromeClient::setStatusbarText(const String& statusText)
{
}

bool WebChromeClient::shouldInterruptJavaScript()
{
    return false;
}

bool WebChromeClient::tabsToLinks() const
{
    // bool enabled = false;
    WebPreferences* preferences = m_webView->preferences();

    return preferences->tabsToLinks();
}

IntRect WebChromeClient::windowResizerRect() const
{
    IntRect intRect;

    return intRect;
}

void WebChromeClient::addToDirtyRegion(const IntRect& dirtyRect)
{
    m_webView->addToDirtyRegion(dirtyRect);
}

void WebChromeClient::scrollBackingStore(int dx, int dy, const IntRect& scrollViewRect, const IntRect& clipRect)
{
    ASSERT(core(m_webView->topLevelFrame()));

    m_webView->scrollBackingStore(core(m_webView->topLevelFrame())->view(), dx, dy, scrollViewRect, clipRect);
}

void WebChromeClient::updateBackingStore()
{
    ASSERT(core(m_webView->topLevelFrame()));

    m_webView->updateBackingStore(core(m_webView->topLevelFrame())->view());
}

void WebChromeClient::mouseDidMoveOverElement(const HitTestResult& result, unsigned modifierFlags)
{

}

void WebChromeClient::setToolTip(const String& toolTip, IntPoint location, const int handleID )
{
    m_webView->setToolTip(toolTip, location, handleID);
}

void WebChromeClient::print(Frame* frame)
{
}

void WebChromeClient::exceededDatabaseQuota(Frame* frame, const String& databaseIdentifier)
{
#if ENABLE(DATABASE)
    WebSecurityOrigin *origin = WebSecurityOrigin::createInstance(frame->document()->securityOrigin());
    const unsigned long long defaultQuota = 5 * 1024 * 1024; // 5 megabytes should hopefully be enough to test storage support.
    origin->setQuota(defaultQuota);
    delete origin;
#endif
}

void WebChromeClient::populateVisitedLinks()
{
    WebHistory *history = WebHistory::sharedHistory();
    if (!history)
        return;
    history->addVisitedLinksToPageGroup(m_webView->page()->group());
}

