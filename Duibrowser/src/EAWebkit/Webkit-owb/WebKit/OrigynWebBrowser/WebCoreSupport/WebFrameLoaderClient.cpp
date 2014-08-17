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
* This file was modified by Electronic Arts Inc Copyright ?2009-2010
*/

/////////////////////////////////////////////////////////////////////////////
// WebFrameLoaderClient.cpp
//
// Copyright (c) 2008, Electronic Arts Inc. All rights reserved.
// Modifed by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////

#include "config.h"
#include "WebFrameLoaderClient.h"

#include "WebCachedPagePlatformData.h"
#include "WebChromeClient.h"
#include "WebDocumentLoader.h"
#include "WebError.h"
#include "WebFrame.h"
#include "WebHistory.h"
#include "WebHistoryItem.h"
#include "WebMutableURLRequest.h"
#include "WebScriptDebugServer.h"
#include "WebURLAuthenticationChallenge.h"
#include "WebURLResponse.h"
#include "WebView.h"

#include "ObserverService.h"

#include <DocumentLoader.h>
#include <FrameLoader.h>
#include <FrameTree.h>
#include <FrameView.h>
#include <HTMLFrameElement.h>
#include <HTMLFrameOwnerElement.h>
#include <HTMLNames.h>
#include <HistoryItem.h>
#include <Page.h>
#include <PluginPackage.h>
#include <PluginView.h>
#include <RenderPart.h>
#include <ResourceHandle.h>
#include <ResourceResponse.h>
#include "BAL/Includes/FakedDeepsee.h"

#include <stdio.h>

#include <EAWebKit/EAWebKitView.h>
#include <EAWebKit/internal/EAWebKitEASTLHelpers.h>

using namespace WebCore;
using namespace HTMLNames;


static WebDataSource* getWebDataSource(DocumentLoader* loader)
{
    return loader ? static_cast<WebDocumentLoader*>(loader)->dataSource() : 0;
}

WebFrameLoaderClient::WebFrameLoaderClient(WebFrame* webFrame)
    : m_webFrame(webFrame)
    , m_pluginView(0) 
    , m_hasSentResponseToPlugin(false) 
{
    ASSERT_ARG(webFrame, webFrame);
    DS_CONSTRUCT();
}

WebFrameLoaderClient::~WebFrameLoaderClient()
{
    if(m_webFrame)
        m_webFrame = 0;
    if(m_pluginView)
        delete m_pluginView;
    DS_DESTRUCT();
}

bool WebFrameLoaderClient::hasWebView() const
{
    return m_webFrame->webView();
}

bool WebFrameLoaderClient::hasFrameView() const
{
    return core(m_webFrame)->view();
}

void WebFrameLoaderClient::forceLayout()
{
    core(m_webFrame)->forceLayout(true);
}

void WebFrameLoaderClient::assignIdentifierToInitialRequest(unsigned long identifier, DocumentLoader* loader, const WebCore::ResourceRequest& request)
{
/*    WebView* webView = m_webFrame->webView();
    COMPtr<IWebResourceLoadDelegate> resourceLoadDelegate;
    if (FAILED(webView->resourceLoadDelegate(&resourceLoadDelegate)))
        return;

    COMPtr<WebMutableURLRequest> webURLRequest(AdoptCOM, WebMutableURLRequest::createInstance(request));
    resourceLoadDelegate->identifierForInitialRequest(webView, webURLRequest.get(), getWebDataSource(loader), identifier);*/

    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        loadInfo.mLET   = EA::WebKit::kLETPageRequest;

        const WebCore::KURL& kurl = request.mainDocumentURL();
        if(kurl.string().length() != 0)
        {
            GetFixedString(loadInfo.mURI)->assign(kurl.string().characters(), kurl.string().length());
			GetFixedString(loadInfo.mResourceURI)->assign(request.url().string().characters(), request.url().string().length());
            EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
            if(pViewNotification)
                pViewNotification->LoadUpdate(loadInfo);
        }
    }
}

void WebFrameLoaderClient::dispatchDidReceiveAuthenticationChallenge(DocumentLoader* loader, unsigned long identifier, const AuthenticationChallenge& challenge)
{
    // 10/27/10 - Note by Arpit Baldeva: Commented out this assert as illegitimate. The authentication is handled through the ViewNotification::Authenticate
	// callback inside the AuthenticationManager system.

	//ASSERT(challenge.sourceHandle());	
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebResourceLoadDelegate> resourceLoadDelegate;
    if (SUCCEEDED(webView->resourceLoadDelegate(&resourceLoadDelegate))) {
        COMPtr<WebURLAuthenticationChallenge> webChallenge(AdoptCOM, WebURLAuthenticationChallenge::createInstance(challenge));
        if (SUCCEEDED(resourceLoadDelegate->didReceiveAuthenticationChallenge(webView, identifier, webChallenge.get(), getWebDataSource(loader))))
            return;
    }*/

    // If the ResourceLoadDelegate doesn't exist or fails to handle the call, we tell the ResourceHandle
    // to continue without credential - this is the best approximation of Mac behavior
    //challenge.sourceHandle()->receivedRequestToContinueWithoutCredential(challenge);
}

void WebFrameLoaderClient::dispatchDidCancelAuthenticationChallenge(DocumentLoader* loader, unsigned long identifier, const AuthenticationChallenge& challenge)
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebResourceLoadDelegate> resourceLoadDelegate;
    if (FAILED(webView->resourceLoadDelegate(&resourceLoadDelegate)))
        return;

    COMPtr<WebURLAuthenticationChallenge> webChallenge(AdoptCOM, WebURLAuthenticationChallenge::createInstance(challenge));
    resourceLoadDelegate->didCancelAuthenticationChallenge(webView, identifier, webChallenge.get(), getWebDataSource(loader));*/
}

void WebFrameLoaderClient::dispatchWillSendRequest(DocumentLoader* loader, unsigned long identifier, WebCore::ResourceRequest& request, const ResourceResponse& redirectResponse)
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebResourceLoadDelegate> resourceLoadDelegate;
    if (FAILED(webView->resourceLoadDelegate(&resourceLoadDelegate)))
        return;

    COMPtr<WebMutableURLRequest> webURLRequest(AdoptCOM, WebMutableURLRequest::createInstance(request));
    COMPtr<WebURLResponse> webURLRedirectResponse(AdoptCOM, WebURLResponse::createInstance(redirectResponse));

    COMPtr<IWebURLRequest> newWebURLRequest;
    if (FAILED(resourceLoadDelegate->willSendRequest(webView, identifier, webURLRequest.get(), webURLRedirectResponse.get(), getWebDataSource(loader), &newWebURLRequest)))
        return;

    if (webURLRequest == newWebURLRequest)
        return;

    COMPtr<WebMutableURLRequest> newWebURLRequestImpl(Query, newWebURLRequest);
    if (!newWebURLRequestImpl)
        return;

    request = newWebURLRequestImpl->resourceRequest();*/

    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        //Note by Arpit Baldeva - Not sure if following Redirect event indication is correct
		loadInfo.mLET = EA::WebKit::kLETPageRedirect;

        const WebCore::KURL& kurl = request.mainDocumentURL();
        if(kurl.string().length() != 0)
        {
			GetFixedString(loadInfo.mURI)->assign(kurl.string().characters(), kurl.string().length());
			GetFixedString(loadInfo.mResourceURI)->assign(request.url().string().characters(), request.url().string().length());

            EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
            if(pViewNotification)
                pViewNotification->LoadUpdate(loadInfo);
        }
    }
}

void WebFrameLoaderClient::dispatchDidReceiveResponse(DocumentLoader* loader, unsigned long identifier, const ResourceResponse& response)
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebResourceLoadDelegate> resourceLoadDelegate;
    if (FAILED(webView->resourceLoadDelegate(&resourceLoadDelegate)))
        return;

    COMPtr<WebURLResponse> webURLResponse(AdoptCOM, WebURLResponse::createInstance(response));
    resourceLoadDelegate->didReceiveResponse(webView, identifier, webURLResponse.get(), getWebDataSource(loader));*/

    // identifier is a unique id (incrementing integer, actually) for each loading operation. 

    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        loadInfo.mLET   = EA::WebKit::kLETResponseReceived;
		loadInfo.mStatusCode = response.httpStatusCode();

		const WebCore::KURL& kurl = response.url();
		if(kurl.string().length() != 0)
		{			
			GetFixedString(loadInfo.mResourceURI)->assign(kurl.string().characters(), kurl.string().length());
		}

        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
        if(pViewNotification)
            pViewNotification->LoadUpdate(loadInfo);
    }
}


void WebFrameLoaderClient::dispatchDidReceiveContentLength(DocumentLoader* loader, unsigned long identifier, int length)
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebResourceLoadDelegate> resourceLoadDelegate;
    if (FAILED(webView->resourceLoadDelegate(&resourceLoadDelegate)))
        return;

    resourceLoadDelegate->didReceiveContentLength(webView, identifier, length, getWebDataSource(loader));*/

    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        loadInfo.mLET   = EA::WebKit::kLETContentLengthReceived;
        loadInfo.mContentLength = length;

        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
        if(pViewNotification)
            pViewNotification->LoadUpdate(loadInfo);
    }

}

void WebFrameLoaderClient::dispatchDidFinishLoading(DocumentLoader* loader, unsigned long identifier)
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebResourceLoadDelegate> resourceLoadDelegate;
    if (FAILED(webView->resourceLoadDelegate(&resourceLoadDelegate)))
        return;

    resourceLoadDelegate->didFinishLoadingFromDataSource(webView, identifier, getWebDataSource(loader));*/
    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        loadInfo.mLET = EA::WebKit::kLETNone;
        loadInfo.mbCompleted = true;

        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
        if(pViewNotification)
            pViewNotification->LoadUpdate(loadInfo);
    }
}

void WebFrameLoaderClient::dispatchDidFailLoading(DocumentLoader* loader, unsigned long identifier, const ResourceError& error)
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebResourceLoadDelegate> resourceLoadDelegate;
    if (FAILED(webView->resourceLoadDelegate(&resourceLoadDelegate)))
        return;

    COMPtr<WebError> webError(AdoptCOM, WebError::createInstance(error));
    resourceLoadDelegate->didFailLoadingWithError(webView, identifier, webError.get(), getWebDataSource(loader));*/
    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        loadInfo.mLET   = EA::WebKit::kLETLoadFailed;

        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
        if(pViewNotification)
            pViewNotification->LoadUpdate(loadInfo);
   
		// also send an error notification
		EA::WebKit::ErrorInfo errorInfo;
		errorInfo.mErrorId = error.errorCode();
		errorInfo.mpView = pView;

		WebCore::String failingUrl = error.failingURL();
		errorInfo.mpURI = failingUrl.charactersWithNullTermination();

		WebCore::String localizedDescription = error.localizedDescription();
		errorInfo.mpContext1 = localizedDescription.charactersWithNullTermination();
		
        if(pViewNotification)
			pViewNotification->WebError(errorInfo);
	}

}

void WebFrameLoaderClient::dispatchDidHandleOnloadEvents()
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebFrameLoadDelegatePrivate> frameLoadDelegatePriv;
    if (SUCCEEDED(webView->frameLoadDelegatePrivate(&frameLoadDelegatePriv)) && frameLoadDelegatePriv)
        frameLoadDelegatePriv->didHandleOnloadEventsForFrame(webView, m_webFrame);*/
}

void WebFrameLoaderClient::dispatchDidReceiveServerRedirectForProvisionalLoad()
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebFrameLoadDelegate> frameLoadDelegate;
    if (SUCCEEDED(webView->frameLoadDelegate(&frameLoadDelegate)))
        frameLoadDelegate->didReceiveServerRedirectForProvisionalLoadForFrame(webView, m_webFrame);*/
}

void WebFrameLoaderClient::dispatchDidCancelClientRedirect()
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebFrameLoadDelegate> frameLoadDelegate;
    if (SUCCEEDED(webView->frameLoadDelegate(&frameLoadDelegate)))
        frameLoadDelegate->didCancelClientRedirectForFrame(webView, m_webFrame);*/
}

void WebFrameLoaderClient::dispatchWillPerformClientRedirect(const KURL& url, double delay, double fireDate)
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebFrameLoadDelegate> frameLoadDelegate;
    if (SUCCEEDED(webView->frameLoadDelegate(&frameLoadDelegate)))
        frameLoadDelegate->willPerformClientRedirectToURL(webView, BString(url.string()), delay, MarshallingHelpers::CFAbsoluteTimeToDATE(fireDate), m_webFrame);*/
}

void WebFrameLoaderClient::dispatchDidChangeLocationWithinPage()
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebFrameLoadDelegate> frameLoadDelegate;
    if (SUCCEEDED(webView->frameLoadDelegate(&frameLoadDelegate)))
        frameLoadDelegate->didChangeLocationWithinPageForFrame(webView, m_webFrame);*/
}

void WebFrameLoaderClient::dispatchWillClose()
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebFrameLoadDelegate> frameLoadDelegate;
    if (SUCCEEDED(webView->frameLoadDelegate(&frameLoadDelegate)))
        frameLoadDelegate->willCloseFrame(webView, m_webFrame);*/
    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        loadInfo.mLET   = EA::WebKit::kLETWillClose;

        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
        if(pViewNotification)
            pViewNotification->LoadUpdate(loadInfo);
    }
}

void WebFrameLoaderClient::dispatchDidReceiveIcon()
{
#if ENABLE(ICON_DATABASE)
    m_webFrame->webView()->dispatchDidReceiveIconFromWebFrame(m_webFrame);
#endif
}

void WebFrameLoaderClient::dispatchDidStartProvisionalLoad()
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebFrameLoadDelegate> frameLoadDelegate;
    if (SUCCEEDED(webView->frameLoadDelegate(&frameLoadDelegate)))
        frameLoadDelegate->didStartProvisionalLoadForFrame(webView, m_webFrame);*/
}

void WebFrameLoaderClient::dispatchDidReceiveTitle(const String& title)
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebFrameLoadDelegate> frameLoadDelegate;
    if (SUCCEEDED(webView->frameLoadDelegate(&frameLoadDelegate)))
        frameLoadDelegate->didReceiveTitle(webView, BString(title), m_webFrame);*/
    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        loadInfo.mLET   = EA::WebKit::kLETTitleReceived;

		WebCore::KURL kurl(title);
		if(kurl.string().length() != 0)
		{			
			GetFixedString(loadInfo.mPageTitle)->assign(kurl.string().characters(), kurl.string().length());
		}

        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
        if(pViewNotification)
            pViewNotification->LoadUpdate(loadInfo);
    }
}

void WebFrameLoaderClient::dispatchDidCommitLoad()
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebFrameLoadDelegate> frameLoadDelegate;
    if (SUCCEEDED(webView->frameLoadDelegate(&frameLoadDelegate)))
        frameLoadDelegate->didCommitLoadForFrame(webView, m_webFrame);*/
    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
		pView->RebindJavascript();

        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        loadInfo.mLET   = EA::WebKit::kLETLoadCommitted;

        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
        if(pViewNotification)
            pViewNotification->LoadUpdate(loadInfo);
    }
}

void WebFrameLoaderClient::dispatchDidFinishDocumentLoad()
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebFrameLoadDelegatePrivate> frameLoadDelegatePriv;
    if (SUCCEEDED(webView->frameLoadDelegatePrivate(&frameLoadDelegatePriv)) && frameLoadDelegatePriv)
        frameLoadDelegatePriv->didFinishDocumentLoadForFrame(webView, m_webFrame);*/
}

void WebFrameLoaderClient::dispatchDidFinishLoad()
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebFrameLoadDelegate> frameLoadDelegate;
    if (SUCCEEDED(webView->frameLoadDelegate(&frameLoadDelegate)))
        frameLoadDelegate->didFinishLoadForFrame(webView, m_webFrame);*/
}

void WebFrameLoaderClient::dispatchDidFirstLayout()
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebFrameLoadDelegatePrivate> frameLoadDelegatePriv;
    if (SUCCEEDED(webView->frameLoadDelegatePrivate(&frameLoadDelegatePriv)) && frameLoadDelegatePriv)
        frameLoadDelegatePriv->didFirstLayoutInFrame(webView, m_webFrame);*/
}

void WebFrameLoaderClient::dispatchDidLayoutCompleted()
{
    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        loadInfo.mLET   = EA::WebKit::kLETLayoutCompleted;

        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
        if(pViewNotification)
            pViewNotification->LoadUpdate(loadInfo);
    }
}

Frame* WebFrameLoaderClient::dispatchCreatePage()
{
    /*WebView* webView = m_webFrame->webView();

    COMPtr<IWebUIDelegate> ui;
    if (FAILED(webView->uiDelegate(&ui)))
        return 0;

    COMPtr<IWebView> newWebView;
    if (FAILED(ui->createWebViewWithRequest(webView, 0, &newWebView)))
        return 0;

    COMPtr<IWebFrame> mainFrame;
    if (FAILED(newWebView->mainFrame(&mainFrame)))
        return 0;

    COMPtr<WebFrame> mainFrameImpl(Query, mainFrame);
    return core(mainFrameImpl.get());*/
	EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
	if(pViewNotification)
	{
		EA::WebKit::CreateViewInfo createViewInfo;
		createViewInfo.mpView = EA::WebKit::GetView(m_webFrame->webView());
		createViewInfo.mFromJavaScript = false;

		if(pViewNotification->CreateView(createViewInfo))
		{
			if(createViewInfo.mpCreatedView)
			{
				return createViewInfo.mpCreatedView->GetFrame();
			}
		}
	}

	return 0;
	
}

void WebFrameLoaderClient::dispatchShow()
{
    /*WebView* webView = m_webFrame->webView();
    COMPtr<IWebUIDelegate> ui;
    if (SUCCEEDED(webView->uiDelegate(&ui)))
        ui->webViewShow(webView);*/

    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        loadInfo.mLET   = EA::WebKit::kLETWillShow;

        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
        if(pViewNotification)
            pViewNotification->LoadUpdate(loadInfo);
    }
}

void WebFrameLoaderClient::dispatchDidLoadMainResource(DocumentLoader* loader)
{
}

void WebFrameLoaderClient::setMainDocumentError(DocumentLoader*, const ResourceError& error)
{
    // Note by Paul Pedriana: Is this where we should sedn out an EA::WebKit::ErrorInfo?

    if (!m_pluginView)
        return;

    if (m_pluginView->status() == PluginStatusLoadedSuccessfully)
        m_pluginView->didFail(error);
    m_pluginView = 0;
    m_hasSentResponseToPlugin = false;
}

void WebFrameLoaderClient::postProgressStartedNotification()
{
#ifdef BENCH_LOAD_TIME
    gettimeofday(&m_timerStart, NULL);
#endif
    OWBAL::BCObserverService::createBCObserverService()->notifyObserver(WebViewProgressStartedNotification, "", m_webFrame->webView());

   // View::GetURI(FixedString16_256& sURI);

    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());
    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
        loadInfo.mpView = pView;
        loadInfo.mLET = EA::WebKit::kLETLoadStarted;
        loadInfo.mbStarted = true;
        loadInfo.mProgressEstimation = 0.0;

        //+ 8/26/09 CSidhall - Added clearing of title name when loading a new one        
        GetFixedString(loadInfo.mPageTitle)->clear();
        //- CS   

        if(pViewNotification)
            pViewNotification->LoadUpdate(loadInfo);
    
        // 6/20/10 Chris Sidhall - Added mostly for tooltip notification.
        WebView* pWebView = m_webFrame->webView();
        if(pWebView)
        {
            pWebView->ResetToolTip();
        }
    
    }
}

void WebFrameLoaderClient::postProgressEstimateChangedNotification()
{
    OWBAL::BCObserverService::createBCObserverService()->notifyObserver(WebViewProgressEstimateChangedNotification, "", m_webFrame->webView());

    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        loadInfo.mLET   = EA::WebKit::kLETLoadProgressUpdate;
        loadInfo.mProgressEstimation = pView->GetEstimatedProgress();

        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
        if(pViewNotification)
            pViewNotification->LoadUpdate(loadInfo);
    }
}

void WebFrameLoaderClient::postProgressFinishedNotification()
{
    OWBAL::BCObserverService::createBCObserverService()->notifyObserver(WebViewProgressFinishedNotification, "", m_webFrame->webView());
    //FIXME : remove this notification
    OWBAL::BCObserverService::createBCObserverService()->notifyObserver("layoutTestController", "loadDone", NULL);

    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        loadInfo.mLET   = EA::WebKit::kLETLoadCompleted;
        loadInfo.mProgressEstimation = 1.0;

        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
        if(pViewNotification)
            pViewNotification->LoadUpdate(loadInfo);
    }

    #ifdef BENCH_LOAD_TIME
        gettimeofday(&m_timerStop, NULL);

        if (m_timerStart.tv_sec == m_timerStop.tv_sec)
           OWB_PRINTF("load time: %06d us\n", static_cast<uint32_t> (m_timerStop.tv_usec - m_timerStart.tv_usec));
        else {
            int seconds = m_timerStop.tv_sec - m_timerStart.tv_sec;
            int microseconds = m_timerStop.tv_usec - m_timerStart.tv_usec;
            if (microseconds < 0) {
                seconds -= 1;
                microseconds = 1000000 + microseconds;
            }
           OWB_PRINTF("load time: %d s %06d us\n", seconds, microseconds);
        }
    #endif

    //OWB_PRINTF("postProgressFinishedNotification\n");
}

void WebFrameLoaderClient::committedLoad(DocumentLoader* loader, const char* data, int length)
{
    // FIXME: This should probably go through the data source.
    const String& textEncoding = loader->response().textEncodingName();

    if (!m_pluginView)
        receivedData(data, length, textEncoding);

    if (!m_pluginView || m_pluginView->status() != PluginStatusLoadedSuccessfully)
        return;

    if (!m_hasSentResponseToPlugin) {
        m_pluginView->didReceiveResponse(core(m_webFrame)->loader()->documentLoader()->response());
        // didReceiveResponse sets up a new stream to the plug-in. on a full-page plug-in, a failure in
        // setting up this stream can cause the main document load to be cancelled, setting m_pluginView
        // to null
        if (!m_pluginView)
            return;
        m_hasSentResponseToPlugin = true;
    }
    m_pluginView->didReceiveData(data, length);
}

void WebFrameLoaderClient::receivedData(const char* data, int length, const String& textEncoding)
{
    Frame* coreFrame = core(m_webFrame);
    if (!coreFrame)
        return;

    // Set the encoding. This only needs to be done once, but it's harmless to do it again later.
    String encoding = coreFrame->loader()->documentLoader()->overrideEncoding();
    bool userChosen = !encoding.isNull();
    if (encoding.isNull())
        encoding = textEncoding;
    coreFrame->loader()->setEncoding(encoding, userChosen);

    coreFrame->loader()->addData(data, length);
}

void WebFrameLoaderClient::finishedLoading(DocumentLoader* loader)
{
    // Telling the frame we received some data and passing 0 as the data is our
    // way to get work done that is normally done when the first bit of data is
    // received, even for the case of a document with no data (like about:blank)
    if (!m_pluginView) {
        committedLoad(loader, 0, 0);
        return;
    }

    if (m_pluginView->status() == PluginStatusLoadedSuccessfully)
        m_pluginView->didFinishLoading();
    m_pluginView = 0;
    m_hasSentResponseToPlugin = false;
}

void WebFrameLoaderClient::updateGlobalHistory(const KURL& url)
{
    WebHistory* history = WebHistory::sharedHistory();
    if (!history)
        return;
    history->addItem(url, core(m_webFrame)->loader()->documentLoader()->title());
}

bool WebFrameLoaderClient::shouldGoToHistoryItem(HistoryItem*) const
{
    return true;
}

PassRefPtr<DocumentLoader> WebFrameLoaderClient::createDocumentLoader(const WebCore::ResourceRequest& request, const SubstituteData& substituteData)
{
    // Deleted by Paul Pedriana as part of updated to owb change list 413 (http://www.sand-labs.org/owb/changeset/413)
    //
    // RefPtr<WebDocumentLoader> loader = new WebDocumentLoader(request, substituteData);
    // 
    // WebDataSource* dataSource = WebDataSource::createInstance(loader.get());
    // 
    // loader->setDataSource(dataSource);
    // return loader.release();

    return DocumentLoader::create(request, substituteData);
}

void WebFrameLoaderClient::setTitle(const String& title, const KURL& url)
{
    bool privateBrowsingEnabled = false;
    WebPreferences* preferences = m_webFrame->webView()->preferences();
    if (preferences)
        privateBrowsingEnabled = preferences->privateBrowsingEnabled();
    if (privateBrowsingEnabled)
        return;

    // update title in global history
    WebHistory* history = webHistory();
    if (!history)
        return;

    WebHistoryItem* item = history->itemForURL(url.string());
    if (!item)
        return;

    item->setTitle(title);
}

void WebFrameLoaderClient::savePlatformDataToCachedPage(CachedPage* cachedPage)
{
    Frame* coreFrame = core(m_webFrame);
    if (!coreFrame)
        return;

    ASSERT(coreFrame->loader()->documentLoader() == cachedPage->documentLoader());

    WebCachedPagePlatformData* webPlatformData = new WebCachedPagePlatformData(static_cast<WebDataSource*>(getWebDataSource(coreFrame->loader()->documentLoader())));
    cachedPage->setCachedPagePlatformData(webPlatformData);
}

void WebFrameLoaderClient::transitionToCommittedForNewPage()
{
    Frame* frame = core(m_webFrame);
    ASSERT(frame);

    Page* page = frame->page();
    ASSERT(page);

    bool isMainFrame = frame == page->mainFrame();

/*    if (isMainFrame && frame->view())
        frame->view()->detachFromWindow();*/

    frame->setView(0);

    WebView* webView = m_webFrame->webView();

    FrameView* frameView;
    if (isMainFrame) {
        IntRect rect = webView->frameRect();
        frameView = new FrameView(frame, rect.size());
    } else
        frameView = new FrameView(frame);

    frame->setView(frameView);
    frameView->deref(); // FrameViews are created with a ref count of 1. Release this ref since we've assigned it to frame.

    BalWidget* viewWindow = webView->viewWindow();
    if (viewWindow)
        frameView->setContainingWindow(viewWindow);

    /*if (isMainFrame)
        frameView->attachToWindow();*/

    if (frame->ownerRenderer())
        frame->ownerRenderer()->setWidget(frameView);

    if (HTMLFrameOwnerElement* owner = frame->ownerElement())
        frame->view()->setScrollbarsMode(owner->scrollingMode());
}

bool WebFrameLoaderClient::canCachePage() const
{
    return true;
}

PassRefPtr<Frame> WebFrameLoaderClient::createFrame(const KURL& url, const String& name, HTMLFrameOwnerElement* ownerElement, const String& referrer, bool /*allowsScrolling*/, int /*marginWidth*/, int /*marginHeight*/)
{
    RefPtr<Frame> result = createFrame(url, name, ownerElement, referrer);
    if (!result)
        return 0;

    // Propagate the marginwidth/height and scrolling modes to the view.
    if (ownerElement->hasTagName(frameTag) || ownerElement->hasTagName(iframeTag)) {
        HTMLFrameElement* frameElt = static_cast<HTMLFrameElement*>(ownerElement);
        if (frameElt->scrollingMode() == ScrollbarAlwaysOff)
            result->view()->setScrollbarsMode(ScrollbarAlwaysOff);
        int marginWidth = frameElt->getMarginWidth();
        int marginHeight = frameElt->getMarginHeight();
        if (marginWidth != -1)
            result->view()->setMarginWidth(marginWidth);
        if (marginHeight != -1)
            result->view()->setMarginHeight(marginHeight);
    }

    return result.release();
}

PassRefPtr<Frame> WebFrameLoaderClient::createFrame(const KURL& URL, const String& name, HTMLFrameOwnerElement* ownerElement, const String& referrer)
{
    Frame* coreFrame = core(m_webFrame);
    ASSERT(coreFrame);

    WebFrame* webFrame = WebFrame::createInstance();

    RefPtr<Frame> childFrame = webFrame->init(m_webFrame->webView(), coreFrame->page(), ownerElement);
    childFrame->m_pClientWebFrame = webFrame;       // 3/31/09 CSidhall - Added so that the secondary webFrame can be released to fix leak

    coreFrame->tree()->appendChild(childFrame);
    childFrame->tree()->setName(name);
    childFrame->init();

    loadURLIntoChild(URL, referrer, webFrame);

    // The frame's onload handler may have removed it from the document.
    if (!childFrame->tree()->parent())
        return 0;

    return childFrame.release();
}

void WebFrameLoaderClient::loadURLIntoChild(const KURL& originalURL, const String& referrer, WebFrame* childFrame)
{
    ASSERT(childFrame);
    ASSERT(core(childFrame));

    Frame* coreFrame = core(m_webFrame);
    ASSERT(coreFrame);

    HistoryItem* parentItem = coreFrame->loader()->currentHistoryItem();
    FrameLoadType loadType = coreFrame->loader()->loadType();
    FrameLoadType childLoadType = FrameLoadTypeRedirectWithLockedHistory;

    KURL url = originalURL;

    // If we're moving in the backforward list, we might want to replace the content
    // of this child frame with whatever was there at that point.
    // Reload will maintain the frame contents, LoadSame will not.
    if (parentItem && parentItem->children().size() &&
        (isBackForwardLoadType(loadType)
         || loadType == FrameLoadTypeReload
         || loadType == FrameLoadTypeReloadAllowingStaleData))
    {
        if (HistoryItem* childItem = parentItem->childItemWithName(core(childFrame)->tree()->name())) {
            // Use the original URL to ensure we get all the side-effects, such as
            // onLoad handlers, of any redirects that happened. An example of where
            // this is needed is Radar 3213556.
            url = childItem->originalURL();
            // These behaviors implied by these loadTypes should apply to the child frames
            childLoadType = loadType;

            if (isBackForwardLoadType(loadType))
                // For back/forward, remember this item so we can traverse any child items as child frames load
                core(childFrame)->loader()->setProvisionalHistoryItem(childItem);
            else
                // For reload, just reinstall the current item, since a new child frame was created but we won't be creating a new BF item
                core(childFrame)->loader()->setCurrentHistoryItem(childItem);
        }
    }

    // FIXME: Handle loading WebArchives here

    core(childFrame)->loader()->load(url, referrer, childLoadType, String(), 0, 0);
}

Widget* WebFrameLoaderClient::createPlugin(const IntSize& pluginSize, Element* element, const KURL& url, const Vector<String>& paramNames, const Vector<String>& paramValues, const String& mimeType, bool loadManually)
{
    Frame* frame = core(m_webFrame);
    PluginView* pluginView = PluginView::create(frame, pluginSize, element, url, paramNames, paramValues, mimeType, loadManually);

    if (pluginView->status() == PluginStatusLoadedSuccessfully)
        return pluginView;

    /*COMPtr<IWebResourceLoadDelegate> resourceLoadDelegate;

    WebView* webView = m_webFrame->webView();
    if (FAILED(webView->resourceLoadDelegate(&resourceLoadDelegate)))
        return pluginView;

    RetainPtr<CFMutableDictionaryRef> userInfo(AdoptCF, CFDictionaryCreateMutable(0, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks));

    unsigned count = (unsigned)paramNames.size();
    for (unsigned i = 0; i < count; i++) {
        if (paramNames[i] == "pluginspage") {
            static CFStringRef key = MarshallingHelpers::LPCOLESTRToCFStringRef(WebKitErrorPlugInPageURLStringKey);
            RetainPtr<CFStringRef> str(AdoptCF, paramValues[i].createCFString());
            CFDictionarySetValue(userInfo.get(), key, str.get());
            break;
        }
    }

    if (!mimeType.isNull()) {
        static CFStringRef key = MarshallingHelpers::LPCOLESTRToCFStringRef(WebKitErrorMIMETypeKey);

        RetainPtr<CFStringRef> str(AdoptCF, mimeType.createCFString());
        CFDictionarySetValue(userInfo.get(), key, str.get());
    }

    String pluginName;
    if (pluginView->plugin())
        pluginName = pluginView->plugin()->name();
    if (!pluginName.isNull()) {
        static CFStringRef key = MarshallingHelpers::LPCOLESTRToCFStringRef(WebKitErrorPlugInNameKey);
        RetainPtr<CFStringRef> str(AdoptCF, pluginName.createCFString());
        CFDictionarySetValue(userInfo.get(), key, str.get());
    }

    COMPtr<CFDictionaryPropertyBag> userInfoBag(AdoptCOM, CFDictionaryPropertyBag::createInstance());
    userInfoBag->setDictionary(userInfo.get());
 
    int errorCode = 0;
    switch (pluginView->status()) {
        case PluginStatusCanNotFindPlugin:
            errorCode = WebKitErrorCannotFindPlugIn;
            break;
        case PluginStatusCanNotLoadPlugin:
            errorCode = WebKitErrorCannotLoadPlugIn;
            break;
        default:
            ASSERT_NOT_REACHED();
    }

    ResourceError resourceError(String(WebKitErrorDomain), errorCode, url.string(), String());
    COMPtr<IWebError> error(AdoptCOM, WebError::createInstance(resourceError, userInfoBag.get()));
     
    resourceLoadDelegate->plugInFailedWithError(webView, error.get(), getWebDataSource(frame->loader()->documentLoader()));*/

    return pluginView;
}

void WebFrameLoaderClient::redirectDataToPlugin(Widget* pluginWidget)
{
    // Ideally, this function shouldn't be necessary, see <rdar://problem/4852889>

    m_pluginView = static_cast<PluginView*>(pluginWidget);
}

WebHistory* WebFrameLoaderClient::webHistory() const
{
    if (m_webFrame != m_webFrame->webView()->topLevelFrame())
        return 0;

    return WebHistory::sharedHistory();
}

