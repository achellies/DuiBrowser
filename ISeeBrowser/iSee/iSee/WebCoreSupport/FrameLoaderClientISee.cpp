/*
 *  Copyright (C) 2007 Alp Toker <alp@atoker.com>
 *  Copyright (C) 2007, 2008 Holger Hans Peter Freyther
 *  Copyright (C) 2007 Christian Dywan <christian@twotoasts.de>
 *  Copyright (C) 2008 Collabora Ltd.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "FrameLoaderClientISee.h"

#include "DocumentLoader.h"
#include "FrameLoader.h"
#include "FrameView.h"
#include "FrameTree.h"
#include "HTMLFormElement.h"
#include "HTMLFrameElement.h"
#include "HTMLFrameOwnerElement.h"
#include "HTMLNames.h"
#include "JSDOMWindow.h"
#include "Language.h"
#include "MIMETypeRegistry.h"
#include "NotImplemented.h"
#include "PlatformString.h"
#include "PluginDatabase.h"
#include "RenderPart.h"
#include "ResourceRequest.h"
#include "CString.h"
#include "ProgressTracker.h"
#include "JSDOMBinding.h"
#include "ScriptController.h"
#include "Page.h"

#include "WebView.h"

#include <JavaScriptCore/APICast.h>
#include <stdio.h>
#if PLATFORM(UNIX)
#include <sys/utsname.h>
#endif

#define CURLE_OK	0	// FIXME：from <curl/curl.h>

using namespace WebCore;

namespace WebKit {

FrameLoaderClient::FrameLoaderClient(WebView *webView)
: m_webView(webView)
, m_frame(0)
, m_userAgent("")
, m_pluginView(0)
, m_hasSentResponseToPlugin(false)
{
}

static String agentPlatform()
{
#ifdef GDK_WINDOWING_X11
    return "X11";
#elif defined(GDK_WINDOWING_WIN32)
    return "Windows";
#elif defined(GDK_WINDOWING_QUARTZ)
    return "Macintosh";
#elif defined(GDK_WINDOWING_DIRECTFB)
    return "DirectFB";
#elif defined(WIN32)
	return "Windows";
#else
    notImplemented();
    return "Unknown";
#endif
}

static String agentOS()
{
#if PLATFORM(DARWIN)
#if PLATFORM(X86)
    return "Intel Mac OS X";
#else
    return "PPC Mac OS X";
#endif
#elif PLATFORM(UNIX)
    struct utsname name;
    if (uname(&name) != -1)
        return String::format("%s %s", name.sysname, name.machine);
    else
        return "Unknown";
#elif PLATFORM(WIN_OS)
    // FIXME: Compute the Windows version
#ifdef _WIN32_WCE
	return "Windows CE";
#else
    return "Windows NT";
#endif
#else
    notImplemented();
    return "Unknown";
#endif
}

static String composeUserAgent()
{
    // This is a liberal interpretation of http://www.mozilla.org/build/revised-user-agent-strings.html
    // See also http://developer.apple.com/internet/safari/faq.html#anchor2

    String ua;

    // Product
    ua += "Mozilla/5.0";

    // Comment
    ua += " (";
    ua += agentPlatform(); // Platform
    ua += "; U; "; // Security
    ua += agentOS(); // OS-or-CPU
    ua += "; ";
    ua += defaultLanguage(); // Localization information
    ua += ") ";

    // WebKit Product
    // FIXME: The WebKit version is hardcoded
    static const String webKitVersion = "525.1+";
    ua += "AppleWebKit/" + webKitVersion;
    ua += " (KHTML, like Gecko, ";
    // We mention Safari since many broken sites check for it (OmniWeb does this too)
    // We re-use the WebKit version, though it doesn't seem to matter much in practice
    ua += "Safari/" + webKitVersion;
    ua += ") ";

    // Vendor Product
    // ua += g_get_prgname();

    return ua;
}

String FrameLoaderClient::userAgent(const KURL&)
{
	if (m_userAgent.isEmpty())
        m_userAgent = composeUserAgent();

    return m_userAgent;
}

WTF::PassRefPtr<WebCore::DocumentLoader> FrameLoaderClient::createDocumentLoader(const WebCore::ResourceRequest& request, const SubstituteData& substituteData)
{
    return DocumentLoader::create(request, substituteData);
}

void FrameLoaderClient::dispatchWillSubmitForm(FramePolicyFunction policyFunction,  PassRefPtr<FormState>)
{
    // FIXME: This is surely too simple
    ASSERT(frame() && policyFunction);
    if (!frame() || !policyFunction)
        return;
    (frame()->loader()->*policyFunction)(PolicyUse);
}

void FrameLoaderClient::committedLoad(DocumentLoader* loader, const char* data, int length)
{
	if (!m_pluginView) {
        ASSERT(loader->frame());
        // Setting the encoding on the frame loader is our way to get work done that is normally done
        // when the first bit of data is received, even for the case of a document with no data (like about:blank).
        String encoding = loader->overrideEncoding();
        bool userChosen = !encoding.isNull();
        if (!userChosen)
            encoding = loader->response().textEncodingName();

        FrameLoader* frameLoader = loader->frameLoader(); // = frame()->loader()
        frameLoader->setEncoding(encoding, userChosen);
        if (data)
            frameLoader->addData(data, length);
    }

    if (m_pluginView) {
        if (!m_hasSentResponseToPlugin) {
            m_pluginView->didReceiveResponse(loader->response());
            m_hasSentResponseToPlugin = true;
        }
        m_pluginView->didReceiveData(data, length);
    }
}

void FrameLoaderClient::dispatchDidReceiveAuthenticationChallenge(DocumentLoader*, unsigned long  identifier, const AuthenticationChallenge&)
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidReceiveAuthenticationChallenge\n");
    notImplemented();
}

void FrameLoaderClient::dispatchDidCancelAuthenticationChallenge(DocumentLoader*, unsigned long  identifier, const AuthenticationChallenge&)
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidCancelAuthenticationChallenge\n");
    notImplemented();
}

void FrameLoaderClient::dispatchWillSendRequest(DocumentLoader*, unsigned long , WebCore::ResourceRequest&, const ResourceResponse&)
{
	iSeeLogW(L"FrameLoaderClient::dispatchWillSendRequest\n");
    notImplemented();
}

void FrameLoaderClient::assignIdentifierToInitialRequest(unsigned long identifier, DocumentLoader*, const WebCore::ResourceRequest&)
{
	iSeeLogW(L"FrameLoaderClient::assignIdentifierToInitialRequest\n");
    notImplemented();   
}

void FrameLoaderClient::postProgressStartedNotification()
{
	iSeeLogW(L"FrameLoaderClient::postProgressStartedNotification\n");
}

void FrameLoaderClient::postProgressEstimateChangedNotification()
{
	iSeeLogW(L"FrameLoaderClient::postProgressEstimateChangedNotification : %d\n", int(m_webView->page()->progress()->estimatedProgress() * 100 + 0.5));
}

void FrameLoaderClient::postProgressFinishedNotification()
{
	iSeeLogW(L"FrameLoaderClient::postProgressFinishedNotification\n");
}

void FrameLoaderClient::frameLoaderDestroyed()
{
    m_webView = 0;
	m_frame = 0;
    delete this;
}

void FrameLoaderClient::dispatchDidReceiveResponse(DocumentLoader*, unsigned long, const ResourceResponse& response)
{
    m_response = response;
	if (frame() && frame()->document())
		frame()->document()->setCookie(response.httpHeaderField("Set-Cookie"));
}

void FrameLoaderClient::dispatchDecidePolicyForMIMEType(FramePolicyFunction policyFunction, const String& mimeType, const WebCore::ResourceRequest&)
{
    ASSERT(frame() && policyFunction);
    if (!frame() || !policyFunction)
        return;

	// 如果不能确定MIME，则先执行PolicyUse
	if (mimeType.isEmpty())
	{
		(frame()->loader()->*policyFunction)(PolicyUse);
		return;
	}

	if (MIMETypeRegistry::isSupportedImageMIMEType(mimeType) ||
		MIMETypeRegistry::isSupportedNonImageMIMEType(mimeType))
        (frame()->loader()->*policyFunction)(PolicyUse);
    else
        (frame()->loader()->*policyFunction)(PolicyDownload);
}

void FrameLoaderClient::dispatchDecidePolicyForNewWindowAction(FramePolicyFunction policyFunction, const NavigationAction& action, const WebCore::ResourceRequest& request, PassRefPtr<FormState> , const String& frameName)
{
	ASSERT(frame() && policyFunction);
	if (!frame() || !policyFunction || !action.event()) // FIXME：要求必须要是手动触发
        return;
	iSeeLogW(L"FrameLoaderClient::dispatchDecidePolicyForNewWindowAction: %08X\n", action.event());

	// 增加一个新窗口，同时访问新的URL
	if (WebView *view = WebView::newWebView(request, frameName, String("")))
	{
		// FIXME: Add a sane default implementation
		(view->frame()->loader()->*policyFunction)(PolicyUse);
	}
	else
		(frame()->loader()->*policyFunction)(PolicyIgnore);
}

void FrameLoaderClient::dispatchDecidePolicyForNavigationAction(FramePolicyFunction policyFunction, const NavigationAction& action, const WebCore::ResourceRequest& request, PassRefPtr<FormState> formState)
{
	// FIXME
    ASSERT(frame() && policyFunction);
    if (!frame() || !policyFunction)
        return;
    (frame()->loader()->*policyFunction)(PolicyUse);
}

Widget* FrameLoaderClient::createPlugin(const IntSize& pluginSize, Element* element, const KURL& url, const Vector<String>& paramNames, const Vector<String>& paramValues, const String& mimeType, bool loadManually)
{
	PluginView* pluginView = PluginView::create(frame() /* = gtk::core(m_frame) */, pluginSize, element, url, paramNames, paramValues, mimeType, loadManually);

    if (pluginView->status() == PluginStatusLoadedSuccessfully)
        return pluginView;

    return 0;
}

PassRefPtr<Frame> FrameLoaderClient::createFrame(const KURL& url, const String& name, HTMLFrameOwnerElement* ownerElement,
                                        const String& referrer, bool allowsScrolling, int marginWidth, int marginHeight)
{
	return m_webView->createFrame(url, name, ownerElement, referrer, allowsScrolling, marginWidth, marginHeight);
}

void FrameLoaderClient::redirectDataToPlugin(Widget* pluginWidget)
{
    ASSERT(!m_pluginView);
    m_pluginView = static_cast<PluginView*>(pluginWidget);
    m_hasSentResponseToPlugin = false;
}

Widget* FrameLoaderClient::createJavaAppletWidget(const IntSize&, Element*, const KURL& baseURL,
                                                    const Vector<String>& paramNames, const Vector<String>& paramValues)
{
    notImplemented();
    return 0;
}

ObjectContentType FrameLoaderClient::objectContentType(const KURL& url, const String& inMimeType)
{
	String type = inMimeType;
    // We don't use MIMETypeRegistry::getMIMETypeForPath() because it returns "application/octet-stream" upon failure
    if (type.isEmpty())
        type = MIMETypeRegistry::getMIMETypeForExtension(url.path().substring(url.path().reverseFind('.') + 1));

    if (type.isEmpty())
        return WebCore::ObjectContentFrame;

    if (MIMETypeRegistry::isSupportedImageMIMEType(type))
        return WebCore::ObjectContentImage;

    if (PluginDatabase::installedPlugins()->isMIMETypeRegistered(inMimeType))
        return WebCore::ObjectContentNetscapePlugin;

    if (MIMETypeRegistry::isSupportedNonImageMIMEType(type))
        return WebCore::ObjectContentFrame;

    return WebCore::ObjectContentNone;
}

String FrameLoaderClient::overrideMediaType() const
{
    notImplemented();
    return String();
}

void FrameLoaderClient::windowObjectCleared()
{
	// FIXME
	iSeeLogW(L"FrameLoaderClient::windowObjectCleared\n");
    notImplemented();
}

void FrameLoaderClient::didPerformFirstNavigation() const
{
	iSeeLogW(L"FrameLoaderClient::didPerformFirstNavigation\n");
}

void FrameLoaderClient::registerForIconNotification(bool) 
{ 
	notImplemented(); 
}

void FrameLoaderClient::setMainFrameDocumentReady(bool) 
{
	iSeeLogW(L"FrameLoaderClient::setMainFrameDocumentReady\n");
    // this is only interesting once we provide an external API for the DOM
}

bool FrameLoaderClient::hasWebView() const
{
    return true;
}

bool FrameLoaderClient::hasFrameView() const
{
    return true;
}

void FrameLoaderClient::dispatchDidFinishLoad() 
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidFinishLoad\n");
}

void FrameLoaderClient::frameLoadCompleted() 
{
	iSeeLogW(L"FrameLoaderClient::frameLoadCompleted\n");
}

void FrameLoaderClient::saveViewStateToItem(HistoryItem*)
{
    notImplemented(); 
}

void FrameLoaderClient::restoreViewState()
{
    notImplemented(); 
}

bool FrameLoaderClient::shouldGoToHistoryItem(HistoryItem* item) const 
{
    // FIXME: This is a very simple implementation. More sophisticated
    // implementation would delegate the decision to a PolicyDelegate.
    // See mac implementation for example.
    return item != 0;
}

void FrameLoaderClient::makeRepresentation(DocumentLoader*)
{
	notImplemented();
}

void FrameLoaderClient::forceLayout()
{
	notImplemented();
}

void FrameLoaderClient::forceLayoutForNonHTML()
{
	notImplemented();
}

void FrameLoaderClient::setCopiesOnScroll()
{
	notImplemented();
}

void FrameLoaderClient::detachedFromParent1()
{
	notImplemented();
}

void FrameLoaderClient::detachedFromParent2()
{
	notImplemented();
}

void FrameLoaderClient::detachedFromParent3()
{
	notImplemented();
}

void FrameLoaderClient::detachedFromParent4()
{
	m_frame = 0;
	m_webView = 0;
}

void FrameLoaderClient::loadedFromCachedPage()
{
	notImplemented();
}

void FrameLoaderClient::dispatchDidHandleOnloadEvents()
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidHandleOnloadEvents\n");
}

void FrameLoaderClient::dispatchDidReceiveServerRedirectForProvisionalLoad()
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidReceiveServerRedirectForProvisionalLoad\n");
}

void FrameLoaderClient::dispatchDidCancelClientRedirect()
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidCancelClientRedirect\n");
}

void FrameLoaderClient::dispatchWillPerformClientRedirect(const KURL&, double, double)
{
	iSeeLogW(L"FrameLoaderClient::dispatchWillPerformClientRedirect\n");
}

void FrameLoaderClient::dispatchDidChangeLocationWithinPage()
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidChangeLocationWithinPage\n");
}

void FrameLoaderClient::dispatchWillClose()
{
	iSeeLogW(L"FrameLoaderClient::dispatchWillClose\n");
}

void FrameLoaderClient::dispatchDidReceiveIcon()
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidReceiveIcon\n");
}

void FrameLoaderClient::dispatchDidStartProvisionalLoad()
{
	notImplemented();
}

void FrameLoaderClient::dispatchDidReceiveTitle(const String& title)
{
	if (frame() && frame()->document() && !frame()->document()->parentDocument())
		m_webView->didReceiveTitle(title);
	String copyTitle = title;
	iSeeLogW(L"FrameLoaderClient::dispatchDidReceiveTitle : %s\n", copyTitle.charactersWithNullTermination());
}

void FrameLoaderClient::dispatchDidCommitLoad()
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidCommitLoad\n");
	// FIXME:需要在结束的时候提示
	m_webView->dispatchDidCommitLoad();
}

void FrameLoaderClient::dispatchDidFinishDocumentLoad()
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidFinishDocumentLoad\n");
}

void FrameLoaderClient::dispatchDidFirstLayout()
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidFirstLayout\n");
}

void FrameLoaderClient::dispatchShow()
{
	iSeeLogW(L"FrameLoaderClient::dispatchShow\n");
}

void FrameLoaderClient::cancelPolicyCheck()
{
	notImplemented();
}

void FrameLoaderClient::dispatchDidLoadMainResource(DocumentLoader*)
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidLoadMainResource\n");
}

void FrameLoaderClient::revertToProvisionalState(DocumentLoader*)
{
	notImplemented();
}

void FrameLoaderClient::willChangeTitle(DocumentLoader*)
{
	notImplemented();
}

void FrameLoaderClient::didChangeTitle(DocumentLoader *l)
{
	setTitle(l->title(), l->url());
}

bool FrameLoaderClient::canHandleRequest(const WebCore::ResourceRequest&) const
{
	notImplemented();
	return true;
}

bool FrameLoaderClient::canShowMIMEType(const String&) const
{
	notImplemented();
	return true;
}

bool FrameLoaderClient::representationExistsForURLScheme(const String&) const
{
	notImplemented();
	return false;
}

String FrameLoaderClient::generatedMIMETypeForURLScheme(const String&) const
{
	notImplemented();
	return String();
}

void FrameLoaderClient::finishedLoading(DocumentLoader* documentLoader)
{
	iSeeLogW(L"FrameLoaderClient::finishedLoading\n");
	if (!m_pluginView)
        committedLoad(documentLoader, 0, 0);
    else {
        m_pluginView->didFinishLoading();
        m_pluginView = 0;
        m_hasSentResponseToPlugin = false;
    }
}

void FrameLoaderClient::provisionalLoadStarted()
{
	notImplemented();
}

void FrameLoaderClient::didFinishLoad()
{ 
	iSeeLogW(L"FrameLoaderClient::didFinishLoad\n");
}

void FrameLoaderClient::prepareForDataSourceReplacement()
{
	notImplemented();
}

void FrameLoaderClient::setTitle(const String& title, const KURL& kurl)
{
	String copyTitle = title;
	String url = kurl.string();
	iSeeLogW(L"FrameLoaderClient::setTitle : %s, %s\n", copyTitle.charactersWithNullTermination(), url.charactersWithNullTermination());
}

void FrameLoaderClient::dispatchDidReceiveContentLength(DocumentLoader*, unsigned long  identifier, int lengthReceived)
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidReceiveContentLength : %d\n", lengthReceived);
}

void FrameLoaderClient::dispatchDidFinishLoading(DocumentLoader*, unsigned long  identifier)
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidFinishLoading\n");
}

void FrameLoaderClient::dispatchDidFailLoading(DocumentLoader* docLoader, unsigned long  identifier, const ResourceError& error)
{
	if (!docLoader->frame() || frame() != docLoader->frame())
	{
		const WebCore::ResourceRequest& request = docLoader->request();
		if (request.url() == request.mainDocumentURL())
			m_webView->didFailLoadingWithError(identifier, error);
	}
	iSeeLogW(L"FrameLoaderClient::dispatchDidFailLoading\n");
}

bool FrameLoaderClient::dispatchDidLoadResourceFromMemoryCache(DocumentLoader*, const WebCore::ResourceRequest&, const ResourceResponse&, int length)
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidLoadResourceFromMemoryCache\n");
	return false;
}

void FrameLoaderClient::dispatchDidFailProvisionalLoad(const ResourceError&)
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidFailProvisionalLoad\n");
}

void FrameLoaderClient::dispatchDidFailLoad(const ResourceError&)
{
	iSeeLogW(L"FrameLoaderClient::dispatchDidFailLoad\n");
}

void FrameLoaderClient::download(ResourceHandle*, const WebCore::ResourceRequest&, const WebCore::ResourceRequest&, const ResourceResponse&)
{
	iSeeLogW(L"FrameLoaderClient::download\n");
}

ResourceError FrameLoaderClient::cancelledError(const WebCore::ResourceRequest&)
{
	iSeeLogW(L"FrameLoaderClient::cancelledError\n");
	return ResourceError();
}

ResourceError FrameLoaderClient::blockedError(const WebCore::ResourceRequest&)
{
	iSeeLogW(L"FrameLoaderClient::blockedError\n");
	return ResourceError();
}

ResourceError FrameLoaderClient::cannotShowURLError(const WebCore::ResourceRequest&)
{
	iSeeLogW(L"FrameLoaderClient::cannotShowURLError\n");
	return ResourceError();
}

ResourceError FrameLoaderClient::interruptForPolicyChangeError(const WebCore::ResourceRequest&)
{
	iSeeLogW(L"FrameLoaderClient::interruptForPolicyChangeError\n");
	return ResourceError();
}

ResourceError FrameLoaderClient::cannotShowMIMETypeError(const ResourceResponse&)
{
	iSeeLogW(L"FrameLoaderClient::cannotShowMIMETypeError\n");
	return ResourceError();
}

ResourceError FrameLoaderClient::fileDoesNotExistError(const ResourceResponse&)
{
	iSeeLogW(L"FrameLoaderClient::fileDoesNotExistError\n");
	return ResourceError();
}

ResourceError FrameLoaderClient::pluginWillHandleLoadError(const ResourceResponse&)
{
    iSeeLogW(L"FrameLoaderClient::pluginWillHandleLoadError\n");
    return ResourceError();
}

bool FrameLoaderClient::shouldFallBack(const ResourceError& error)
{
	return (error.errorCode() != CURLE_OK);
}

bool FrameLoaderClient::canCachePage() const
{
    return true;
}

Frame* FrameLoaderClient::dispatchCreatePage()
{
	iSeeLogW(L"FrameLoaderClient::dispatchCreatePage\n");
	return 0;
}

void FrameLoaderClient::dispatchUnableToImplementPolicy(const ResourceError&)
{
	iSeeLogW(L"FrameLoaderClient::dispatchUnableToImplementPolicy\n");
}

void FrameLoaderClient::setMainDocumentError(DocumentLoader*, const ResourceError& error)
{
	if (m_pluginView) {
        m_pluginView->didFail(error);
        m_pluginView = 0;
        m_hasSentResponseToPlugin = false;
    }
}

void FrameLoaderClient::startDownload(const WebCore::ResourceRequest&)
{
	notImplemented();
}

void FrameLoaderClient::updateGlobalHistory(const KURL&)
{
    notImplemented();
}

void FrameLoaderClient::savePlatformDataToCachedPage(CachedPage*)
{
	notImplemented();
}

void FrameLoaderClient::transitionToCommittedFromCachedPage(CachedPage*)
{
	notImplemented();
}

void FrameLoaderClient::transitionToCommittedForNewPage()
{
	Frame* frame = this->frame(); // 此处进行了改动
	ASSERT(frame);

	Page* page = frame->page();
	ASSERT(page);

	bool isMainFrame = frame == page->mainFrame();

	if (isMainFrame && frame->view())
		frame->view()->detachFromWindow();

	frame->setView(0);

	FrameView* frameView;
	if (isMainFrame) {
		RECT rect;
		m_webView->frameRect(&rect); // 此处有所改动
		frameView = new FrameView(frame, IntRect(rect).size());
	} else
		frameView = new FrameView(frame);

	frame->setView(frameView);
	frameView->deref(); // FrameViews are created with a ref count of 1. Release this ref since we've assigned it to frame.

	// 此处有所改动
	frameView->setContainingWindow(m_webView->getViewWindow());

	if (isMainFrame)
		frameView->attachToWindow();

	if (frame->ownerRenderer())
		frame->ownerRenderer()->setWidget(frameView);

	if (HTMLFrameOwnerElement* owner = frame->ownerElement())
		frame->view()->setScrollbarsMode(owner->scrollingMode());
}

} // namespace WebKit
