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

#ifndef WebFrame_H
#define WebFrame_H


#include "BALBase.h"
#include "WebFrameLoaderClient.h"
#include "WebFrameLoaderClient.h"

#include "WebDataSource.h"

#include <Frame.h>
#include <KURL.h>
#include <PlatformString.h>
#include <ResourceHandleClient.h>

#include <wtf/RefPtr.h>
#include <wtf/HashMap.h>
#include <wtf/OwnPtr.h>
#include "ObserverAddons.h"

namespace WebCore {
    class AuthenticationChallenge;
    class DocumentLoader;
    class Element;
    class Frame;
    class HTMLFrameOwnerElement;
    class IntRect;
    class Page;
    class ResourceError;
    class ResourceRequest;
    class SharedBuffer;
    class String;
}

typedef const struct OpaqueJSContext* JSContextRef;
typedef struct OpaqueJSValue* JSObjectRef;
typedef struct OpaqueJSContext* JSGlobalContextRef;

class WebFrame;
class WebFramePolicyListener;
class WebHistory;
class WebView;
class WebMutableURLRequest;

#ifdef __BINDING_JS__
class BindingJS;
#endif

typedef enum {
    WebFrameLoadTypeStandard,
    WebFrameLoadTypeBack,
    WebFrameLoadTypeForward,
    WebFrameLoadTypeIndexedBackForward, // a multi-item hop in the backforward list
    WebFrameLoadTypeReload,
    WebFrameLoadTypeReloadAllowingStaleData,
    WebFrameLoadTypeSame,               // user loads same URL again (but not reload button)
    WebFrameLoadTypeInternal,           // maps to WebCore::FrameLoadTypeRedirectWithLockedHistory
    WebFrameLoadTypeReplace
} WebFrameLoadType;

typedef enum _WebURLRequestCachePolicy
{
    WebURLRequestUseProtocolCachePolicy,
    WebURLRequestReloadIgnoringCacheData,
    WebURLRequestReturnCacheDataElseLoad,
    WebURLRequestReturnCacheDataDontLoad
} WebURLRequestCachePolicy;


WebFrame* kit(WebCore::Frame*);
WebCore::Frame* core(WebFrame*);

class WebFrame: public WebFrameLoaderClient, public OWBAL::BCObserverAddons
{
public:
    static WebFrame* createInstance();

    virtual ~WebFrame();

protected:
    WebFrame();

public:
    virtual WebCore::String name();
    virtual WebView *webView();
    virtual WebCore::Document* DOMDocument();
    virtual void loadRequest(WebMutableURLRequest *request);
    virtual void loadRequest(const WebCore::ResourceRequest& request);
    virtual void loadURL(WebCore::String url, double timeoutSeconds = 0);
    virtual void loadData(PassRefPtr<WebCore::SharedBuffer>data, WebCore::String mimeType, WebCore::String textEncodingName, WebCore::String url);
    virtual void loadHTMLString(WebCore::String string, WebCore::String baseURL);
    virtual void loadAlternateHTMLString(WebCore::String str, WebCore::String baseURL, WebCore::String unreachableURL);
    virtual void loadArchive(WebArchive *archive);
    virtual WebDataSource *dataSource();
    virtual WebDataSource *provisionalDataSource();
    virtual void stopLoading();
    virtual void reload();
    virtual WebFrame* findFrameNamed(WebCore::String name);
    virtual WebFrame* parentFrame();
    virtual WebCore::HTMLFormElement* currentForm();
    virtual JSGlobalContextRef globalContext();

    virtual WebCore::String renderTreeAsExternalRepresentation();
    virtual WebCore::IntSize scrollOffset();
    virtual void layout();
    virtual bool firstLayoutDone();
    virtual WebFrameLoadType loadType();
    //virtual void setInPrintingMode(bool value, HDC printDC);  
    //virtual unsigned int getPrintedPageCount(HDC printDC);
    //virtual void* spoolPages(HDC printDC, UINT startPage, UINT endPage);

    virtual bool isFrameSet();
    virtual WebCore::String string();
    virtual IntSize size();
    virtual bool hasScrollBars();
    virtual WebCore::IntRect contentBounds();
    virtual WebCore::IntRect frameBounds();

    virtual bool isDescendantOfFrame(WebFrame *ancestor);
    virtual void setAllowsScrolling(bool flag);
    virtual bool allowsScrolling();

    virtual bool supportsTextEncoding();
    virtual WebCore::String selectedString();
    virtual void selectAll();
    virtual void deselectAll();
    
    // FrameLoaderClient
    virtual void frameLoaderDestroyed();
    virtual void makeRepresentation(WebCore::DocumentLoader*);
    virtual void forceLayoutForNonHTML();
    virtual void setCopiesOnScroll();
    virtual void detachedFromParent2();
    virtual void detachedFromParent3();
    virtual void detachedFromParent4();
    virtual void cancelPolicyCheck();
    virtual void dispatchWillSubmitForm(WebCore::FramePolicyFunction, PassRefPtr<WebCore::FormState>);
    virtual void revertToProvisionalState(WebCore::DocumentLoader*);
    virtual void setMainFrameDocumentReady(bool);
    virtual void willChangeTitle(WebCore::DocumentLoader*);
    virtual void didChangeTitle(WebCore::DocumentLoader*);
    virtual bool canHandleRequest(const WebCore::ResourceRequest&) const;
    virtual bool canShowMIMEType(const WebCore::String& MIMEType) const;
    virtual bool representationExistsForURLScheme(const WebCore::String& URLScheme) const;
    virtual WebCore::String generatedMIMETypeForURLScheme(const WebCore::String& URLScheme) const;
    virtual void frameLoadCompleted();
    virtual void restoreViewState();
    virtual void provisionalLoadStarted();
    virtual bool shouldTreatURLAsSameAsCurrent(const WebCore::KURL&) const;
    virtual void addHistoryItemForFragmentScroll();
    virtual void didFinishLoad();
    virtual void prepareForDataSourceReplacement();
    virtual WebCore::String userAgent(const WebCore::KURL&);
    virtual void transitionToCommittedFromCachedPage(WebCore::CachedPage*);
    virtual void saveViewStateToItem(WebCore::HistoryItem *);
    virtual WebCore::ResourceError cancelledError(const WebCore::ResourceRequest&);
    virtual WebCore::ResourceError blockedError(const WebCore::ResourceRequest&);
    virtual WebCore::ResourceError cannotShowURLError(const WebCore::ResourceRequest&);
    virtual WebCore::ResourceError interruptForPolicyChangeError(const WebCore::ResourceRequest&);
    virtual WebCore::ResourceError cannotShowMIMETypeError(const WebCore::ResourceResponse&);
    virtual WebCore::ResourceError fileDoesNotExistError(const WebCore::ResourceResponse&);
    virtual bool shouldFallBack(const WebCore::ResourceError&);
    virtual void dispatchDecidePolicyForMIMEType(WebCore::FramePolicyFunction, const WebCore::String& MIMEType, const WebCore::ResourceRequest&);
    virtual void dispatchDecidePolicyForNewWindowAction(WebCore::FramePolicyFunction, const WebCore::NavigationAction&, const WebCore::ResourceRequest&, PassRefPtr<WebCore::FormState>, const WebCore::String& frameName);
    virtual void dispatchDecidePolicyForNavigationAction(WebCore::FramePolicyFunction, const WebCore::NavigationAction&, const WebCore::ResourceRequest&, PassRefPtr<WebCore::FormState>);
    virtual void dispatchUnableToImplementPolicy(const WebCore::ResourceError&);
    virtual void download(WebCore::ResourceHandle*, const WebCore::ResourceRequest&, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&);

    virtual bool dispatchDidLoadResourceFromMemoryCache(WebCore::DocumentLoader*, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&, int length);
    virtual void dispatchDidFailProvisionalLoad(const WebCore::ResourceError&);
    virtual void dispatchDidFailLoad(const WebCore::ResourceError&);
    virtual void startDownload(const WebCore::ResourceRequest&);
        
    virtual WebCore::Widget* createJavaAppletWidget(const WebCore::IntSize&, WebCore::Element*, const WebCore::KURL& baseURL, const Vector<WebCore::String>& paramNames, const Vector<WebCore::String>& paramValues);

    virtual WebCore::ObjectContentType objectContentType(const WebCore::KURL& url, const WebCore::String& mimeType);
    virtual WebCore::String overrideMediaType() const;

    virtual void windowObjectCleared();
    virtual void didPerformFirstNavigation() const;

    virtual void registerForIconNotification(bool listen);

    // WebFrame
    PassRefPtr<WebCore::Frame> init(WebView*, WebCore::Page*, WebCore::HTMLFrameOwnerElement*);
    WebCore::Frame* impl();
    void invalidate();
    void unmarkAllMisspellings();
    void unmarkAllBadGrammar();

    // WebFrame (matching WebCoreFrameBridge)
    void setTextSizeMultiplier(float multiplier);
    bool inViewSourceMode();
    void setInViewSourceMode(bool flag);
    //HRESULT elementWithName(BSTR name, IDOMElement* form, IDOMElement** element);
    //HRESULT formForElement(IDOMElement* element, IDOMElement** form);
    //HRESULT elementDoesAutoComplete(IDOMElement* element, bool* result);
    //HRESULT controlsInForm(IDOMElement* form, IDOMElement** controls, int* cControls);
    //HRESULT elementIsPassword(IDOMElement* element, bool* result);
    //HRESULT searchForLabelsBeforeElement(const BSTR* labels, int cLabels, IDOMElement* beforeElement, BSTR* result);
    //HRESULT matchLabelsAgainstElement(const BSTR* labels, int cLabels, IDOMElement* againstElement, BSTR* result);
    //HRESULT canProvideDocumentSource(bool* result);

    WebFramePolicyListener* setUpPolicyListener(WebCore::FramePolicyFunction function);
    void receivedPolicyDecision(WebCore::PolicyAction);

    WebCore::KURL url() const;

    WebView* webView() const;

    /** 
     * addToJSWindowObject 
     */  
    void addToJSWindowObject(const char* name, void *object); 
    //BCObserverAddons 
    virtual void observe(const OWBAL::String &topic, BalObject *obj);

    bool isDocLoading();   // Added to find if doc loader is loading

protected:
    void loadData(PassRefPtr<WebCore::SharedBuffer>, WebCore::String mimeType, WebCore::String textEncodingName, WebCore::String baseURL, WebCore::String failingURL);
    void loadHTMLString(WebCore::String string, WebCore::String baseURL, WebCore::String unreachableURL);
    //const Vector<WebCore::IntRect>& computePageRects(HDC printDC);
    //void setPrinting(bool printing, float minPageWidth, float maxPageWidth, bool adjustViewSize);
    //void headerAndFooterHeights(float*, float*);
    //WebCore::IntRect printerMarginRect(HDC);

protected:
    class WebFramePrivate;
    WebFramePrivate*    d;
    bool                m_quickRedirectComing;
    WebCore::KURL       m_originalRequestURL;
    bool                m_inPrintingMode;
    Vector<WebCore::IntRect> m_pageRects;
    int m_pageHeight;   // height of the page adjusted by margins
#ifdef __BINDING_JS__ 
    BindingJS *m_bindingJS; 
#endif
};

#endif
