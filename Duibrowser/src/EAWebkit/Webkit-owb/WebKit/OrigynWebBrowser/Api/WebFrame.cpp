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

#include "config.h"
#include "WebFrame.h"

#include "DefaultPolicyDelegate.h"
#include "FormValuesPropertyBag.h"
#include "WebActionPropertyBag.h"
#include "WebChromeClient.h"
#include "WebDocumentLoader.h"
#include "WebDownload.h"
#include "WebError.h"
#include "WebMutableURLRequest.h"
#include "WebEditorClient.h"
#include "WebFramePolicyListener.h"
#include "WebHistory.h"
#include "WebView.h"
#include "WebDataSource.h"
#include "WebHistoryItem.h"
#include "WebURLResponse.h"

#include <PlatformString.h>
#include <Cache.h>
#include <Document.h>
#include <DocumentLoader.h>
#include <DOMImplementation.h>
#include <DOMWindow.h>
#include <Event.h>
#include <FormState.h>
#include <FrameLoader.h>
#include <FrameLoadRequest.h>
#include <FrameTree.h>
#include <FrameView.h>
#include <Frame.h>
#include <GraphicsContext.h>
#include <HistoryItem.h>
#include <HTMLFormElement.h>
#include <HTMLFormControlElement.h>
#include <HTMLInputElement.h>
#include <HTMLNames.h>
#include <JSDOMWindow.h>
#include <KeyboardEvent.h>
#include <MIMETypeRegistry.h>
#include <MouseRelatedEvent.h>
#include <NotImplemented.h>
#include <Page.h>
#include <PlatformKeyboardEvent.h>
#include <PluginInfoStore.h>
#include <PluginDatabase.h>
#include <PluginView.h>
#include <ResourceHandle.h>
#include <ResourceHandle.h>
#include <ResourceRequest.h>
#include <RenderView.h>
#include <RenderTreeAsText.h>
#include <Settings.h>
#include <TextIterator.h>
#include <JSDOMBinding.h>
#include <ScriptController.h>
#include <API/APICast.h>
#include <wtf/MathExtras.h>
#include "BAL/Includes/FakedDeepsee.h"
#include "ObserverService.h"
#include "bal_instance.h"
#include "runtime.h"
#include "runtime_root.h"
#include "runtime_object.h"
#include "ExecState.h"
#include "JSGlobalObject.h"
#include "JSObject.h"
#include "JSDOMWindowBase.h"
#include "JSDOMWindow.h"
#include "bal_object.h"
#ifdef __BINDING_JS__ 
#include "BindingJS.h" 
#endif 

using namespace WebCore;
using namespace HTMLNames;

#define FLASH_REDRAW 0


// By imaging to a width a little wider than the available pixels,
// thin pages will be scaled down a little, matching the way they
// print in IE and Camino. This lets them use fewer sheets than they
// would otherwise, which is presumably why other browsers do this.
// Wide pages will be scaled down more than this.
const float PrintingMinimumShrinkFactor = 1.25f;

// This number determines how small we are willing to reduce the page content
// in order to accommodate the widest line. If the page would have to be
// reduced smaller to make the widest line fit, we just clip instead (this
// behavior matches MacIE and Mozilla, at least)
const float PrintingMaximumShrinkFactor = 2.0f;

//-----------------------------------------------------------------------------
// Helpers to convert from WebCore to WebKit type
WebFrame* kit(Frame* frame)
{
    if (!frame)
        return 0;

    FrameLoaderClient* frameLoaderClient = frame->loader()->client();
    if (frameLoaderClient)
        return static_cast<WebFrame*>(frameLoaderClient);  // eek, is there a better way than static cast?
    return 0;
}

Frame* core(WebFrame* webFrame)
{
    if (!webFrame)
        return 0;
    return webFrame->impl();
}

// This function is not in WebFrame.h because we don't want to advertise the ability to get a non-const Frame from a const WebFrame
Frame* core(const WebFrame* webFrame)
{
    if (!webFrame)
        return 0;
    return const_cast<WebFrame*>(webFrame)->impl();
}

//-----------------------------------------------------------------------------

#include <wtf/FastAllocBase.h>

class WebFrame::WebFramePrivate/*: public WTF::FastAllocBase*/ {
public:
    WebFramePrivate() 
        : frame(0)
        , webView(0)
        , m_policyFunction(0)
        , m_policyListener(0)
    {
        DS_CONSTRUCT();
    }

    ~WebFramePrivate() 
    {
        delete frame;
        webView = 0;
        delete m_policyListener;
        DS_DESTRUCT();
    }
    FrameView* frameView() { return frame ? frame->view() : 0; }

    Frame* frame;
    WebView* webView;
    FramePolicyFunction m_policyFunction;
    WebFramePolicyListener* m_policyListener;
};

// WebFrame ----------------------------------------------------------------

WebFrame::WebFrame()
    : WebFrameLoaderClient(this)
    , d(new WebFrame::WebFramePrivate)
    , m_quickRedirectComing(false)
    , m_inPrintingMode(false)
    , m_pageHeight(0)
// #ifdef __BINDING_JS__
//     , m_bindingJS(new BindingJS())
// #endif
{
    DS_CONSTRUCT();
#ifdef __BINDING_JS__
    m_bindingJS = new BindingJS();
#endif
    OWBAL::BCObserverService::createBCObserverService()->registerObserver("AddonRegister", this); 
}

WebFrame::~WebFrame()
{
    OWBAL::BCObserverService::createBCObserverService()->removeObserver("AddonRegister", this); 
#ifdef __BINDING_JS__ 
    if (m_bindingJS)
        delete m_bindingJS;
    m_bindingJS = 0;
#endif

    delete d;
    DS_DESTRUCT();
}

WebFrame* WebFrame::createInstance()
{
    WebFrame* instance = new WebFrame();
    return instance;
}

void WebFrame::setAllowsScrolling(bool flag)
{
    /*if (Frame* frame = core(this))
        if (FrameView* view = frame->view())
            view->setAllowsScrolling(!!flag);*/
}

bool WebFrame::allowsScrolling()
{
    /*if (flag)
        if (Frame* frame = core(this))
            if (FrameView* view = frame->view())
                return view->allowsScrolling();*/

    return false;
}

String WebFrame::name()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return String();

    return coreFrame->tree()->name();
}

WebView* WebFrame::webView()
{
    return d->webView;
}

WebCore::Document* WebFrame::DOMDocument()
{
    if (Frame* coreFrame = core(this))
        return coreFrame->document();

    return 0;
}

WebCore::HTMLFormElement* WebFrame::currentForm()
{
    if (Frame* coreFrame = core(this))
        return coreFrame->currentForm();

    return 0;
}

JSGlobalContextRef WebFrame::globalContext()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return 0;

    return toGlobalRef(coreFrame->script()->globalObject()->globalExec());
}

void WebFrame::loadURL(WebCore::String url, double timeoutSeconds)
{
    String method("GET");

    WebMutableURLRequest* request = WebMutableURLRequest::createInstance();
    request->initWithURL(url, (WebCore::ResourceRequestCachePolicy)WebURLRequestUseProtocolCachePolicy, timeoutSeconds);
    request->setHTTPMethod(method);
    loadRequest(request);
    delete request;
}

void WebFrame::loadRequest(WebMutableURLRequest* request)
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return;

    coreFrame->loader()->load(request->resourceRequest());
}

void WebFrame::loadRequest(const WebCore::ResourceRequest& request)
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return;

    coreFrame->loader()->load(request);
}

void WebFrame::loadData(PassRefPtr<WebCore::SharedBuffer> data, String mimeType, String textEncodingName, String url)
{
    loadData(data, mimeType, textEncodingName, url, "");
}


void WebFrame::loadData(PassRefPtr<WebCore::SharedBuffer> data, String mimeType, String textEncodingName, String baseURL, String failingURL)
{
    if (!mimeType)
        mimeType = "text/html";

    KURL baseKURL(baseURL);
    KURL failingKURL(failingURL);

    WebCore::ResourceRequest request(baseKURL);
    SubstituteData substituteData(data, mimeType, textEncodingName, failingKURL);

    // This method is only called from IWebFrame methods, so don't ASSERT that the Frame pointer isn't null.
    if (Frame* coreFrame = core(this))
        coreFrame->loader()->load(request, substituteData);
}


void WebFrame::loadHTMLString(String string, String baseURL, String unreachableURL)
{
    RefPtr<SharedBuffer> sharedBuffer = SharedBuffer::create(string.utf8().data(), string.length());
    String utf16Encoding("utf-16");
    loadData(sharedBuffer.release(), "", utf16Encoding, baseURL, unreachableURL);
}

void WebFrame::loadHTMLString(String string, String baseURL)
{
    loadHTMLString(string, baseURL, "");
}

void WebFrame::loadAlternateHTMLString(String str, String baseURL, String unreachableURL)
{
    loadHTMLString(str, baseURL, unreachableURL);
}

void WebFrame::loadArchive(WebArchive* /*archive*/)
{
    BalNotImplemented();
}

static inline WebDataSource *getWebDataSource(DocumentLoader* loader)
{
    return loader ? static_cast<WebDocumentLoader*>(loader)->dataSource() : 0;
}

WebDataSource* WebFrame::dataSource()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return 0;

    return getWebDataSource(coreFrame->loader()->documentLoader());
}

WebDataSource* WebFrame::provisionalDataSource()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return 0;

    return getWebDataSource(coreFrame->loader()->provisionalDocumentLoader());
}

KURL WebFrame::url() const
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return KURL();

    return coreFrame->loader()->url();
}

void WebFrame::stopLoading()
{
    if (Frame* coreFrame = core(this))
        coreFrame->loader()->stopAllLoaders();
}

void WebFrame::reload()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return;

    coreFrame->loader()->reload();
}

WebFrame* WebFrame::findFrameNamed(String name)
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return 0;

    Frame* foundFrame = coreFrame->tree()->find(name);
    if (!foundFrame)
        return 0;

    WebFrame* foundWebFrame = kit(foundFrame);
    if (!foundWebFrame)
        return 0;

    return foundWebFrame;
}

WebFrame* WebFrame::parentFrame()
{
    if (Frame* coreFrame = core(this))
        if (WebFrame* webFrame = kit(coreFrame->tree()->parent()))
            return webFrame;

    return 0;
}

String WebFrame::renderTreeAsExternalRepresentation()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return String();

    return externalRepresentation(coreFrame->contentRenderer());
}

IntSize WebFrame::scrollOffset()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return IntSize();

    FrameView* view = coreFrame->view();
    if (!view)
        return IntSize();

    return view->scrollOffset();
}

void WebFrame::layout()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return ;

    FrameView* view = coreFrame->view();
    if (!view)
        return ;

    view->layout();
}

bool WebFrame::firstLayoutDone()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return false;

    return coreFrame->loader()->firstLayoutDone();
}

WebFrameLoadType WebFrame::loadType()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return WebFrameLoadTypeStandard;

    return (WebFrameLoadType)coreFrame->loader()->loadType();
}

bool WebFrame::supportsTextEncoding()
{
    return false;
}

String WebFrame::selectedString()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return String();

    String text = coreFrame->selectedText();
    text.replace('\\', coreFrame->backslashAsCurrencySymbol());

    return text;
}

void WebFrame::selectAll()
{
    BalNotImplemented();
}

void WebFrame::deselectAll()
{
    BalNotImplemented();
}

PassRefPtr<Frame> WebFrame::init(WebView* webView, Page* page, HTMLFrameOwnerElement* ownerElement)
{
    d->webView = webView;

    RefPtr<Frame> frame = Frame::create(page, ownerElement, this);
    d->frame = frame.get();
    return frame.release();
}

Frame* WebFrame::impl()
{
    return d->frame;
}

void WebFrame::invalidate()
{
    Frame* coreFrame = core(this);
    ASSERT(coreFrame);

    if (Document* document = coreFrame->document())
        document->recalcStyle(Node::Force);
}

void WebFrame::setTextSizeMultiplier(float multiplier)
{
    Frame* coreFrame = core(this);
    ASSERT(coreFrame);
    coreFrame->setZoomFactor(multiplier, true);
}

bool WebFrame::inViewSourceMode()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return false;

    return coreFrame->inViewSourceMode() ? true : false;
}

void WebFrame::setInViewSourceMode(bool flag)
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return ;

    coreFrame->setInViewSourceMode(!!flag);
}

/*HRESULT WebFrame::elementWithName(BSTR name, IDOMElement* form, IDOMElement** element)
{
    if (!form)
        return E_INVALIDARG;

    HTMLFormElement *formElement = formElementFromDOMElement(form);
    if (formElement) {
        Vector<HTMLGenericFormElement*>& elements = formElement->formElements;
        AtomicString targetName((UChar*)name, SysStringLen(name));
        for (unsigned int i = 0; i < elements.size(); i++) {
            HTMLGenericFormElement *elt = elements[i];
            // Skip option elements, other duds
            if (elt->name() == targetName) {
                *element = DOMElement::createInstance(elt);
                return S_OK;
            }
        }
    }
    return E_FAIL;
}

HRESULT WebFrame::formForElement(IDOMElement* element, IDOMElement** form)
{
    if (!element)
        return E_INVALIDARG;

    HTMLInputElement *inputElement = inputElementFromDOMElement(element);
    if (!inputElement)
        return E_FAIL;

    HTMLFormElement *formElement = inputElement->form();
    if (!formElement)
        return E_FAIL;

    *form = DOMElement::createInstance(formElement);
    return S_OK;
}

HRESULT WebFrame::elementDoesAutoComplete(IDOMElement *element, bool *result)
{
    *result = false;
    if (!element)
        return E_INVALIDARG;

    HTMLInputElement *inputElement = inputElementFromDOMElement(element);
    if (!inputElement)
        *result = false;
    else
        *result = (inputElement->inputType() == HTMLInputElement::TEXT && inputElement->autoComplete());

    return S_OK;
}

HRESULT WebFrame::controlsInForm(IDOMElement* form, IDOMElement** controls, int* cControls)
{
    if (!form)
        return E_INVALIDARG;

    HTMLFormElement *formElement = formElementFromDOMElement(form);
    if (!formElement)
        return E_FAIL;

    int inCount = *cControls;
    int count = (int) formElement->formElements.size();
    *cControls = count;
    if (!controls)
        return S_OK;
    if (inCount < count)
        return E_FAIL;

    *cControls = 0;
    Vector<HTMLGenericFormElement*>& elements = formElement->formElements;
    for (int i = 0; i < count; i++) {
        if (elements.at(i)->isEnumeratable()) { // Skip option elements, other duds
            controls[*cControls] = DOMElement::createInstance(elements.at(i));
            (*cControls)++;
        }
    }
    return S_OK;
}

HRESULT WebFrame::elementIsPassword(IDOMElement *element, bool *result)
{
    HTMLInputElement *inputElement = inputElementFromDOMElement(element);
    *result = inputElement != 0
        && inputElement->inputType() == HTMLInputElement::PASSWORD;
    return S_OK;
}

HRESULT WebFrame::searchForLabelsBeforeElement(const BSTR* labels, int cLabels, IDOMElement* beforeElement, BSTR* result)
{
    if (!result) {
        ASSERT_NOT_REACHED();
        return E_POINTER;
    }

    *result = 0;

    if (!cLabels)
        return S_OK;
    if (cLabels < 1)
        return E_INVALIDARG;

    Frame* coreFrame = core(this);
    if (!coreFrame)
        return E_FAIL;

    Vector<String> labelStrings(cLabels);
    for (int i=0; i<cLabels; i++)
        labelStrings[i] = String(labels[i], SysStringLen(labels[i]));
    Element *coreElement = elementFromDOMElement(beforeElement);
    if (!coreElement)
        return E_FAIL;

    String label = coreFrame->searchForLabelsBeforeElement(labelStrings, coreElement);
    
    *result = SysAllocStringLen(label.characters(), label.length());
    if (label.length() && !*result)
        return E_OUTOFMEMORY;
    return S_OK;
}

HRESULT WebFrame::matchLabelsAgainstElement(const BSTR* labels, int cLabels, IDOMElement* againstElement, BSTR* result)
{
    if (!result) {
        ASSERT_NOT_REACHED();
        return E_POINTER;
    }

    *result = 0;

    if (!cLabels)
        return S_OK;
    if (cLabels < 1)
        return E_INVALIDARG;

    Frame* coreFrame = core(this);
    if (!coreFrame)
        return E_FAIL;

    Vector<String> labelStrings(cLabels);
    for (int i=0; i<cLabels; i++)
        labelStrings[i] = String(labels[i], SysStringLen(labels[i]));
    Element *coreElement = elementFromDOMElement(againstElement);
    if (!coreElement)
        return E_FAIL;

    String label = coreFrame->matchLabelsAgainstElement(labelStrings, coreElement);
    
    *result = SysAllocStringLen(label.characters(), label.length());
    if (label.length() && !*result)
        return E_OUTOFMEMORY;
    return S_OK;
}

HRESULT WebFrame::canProvideDocumentSource(bool* result)
{
    HRESULT hr = S_OK;
    *result = false;

    COMPtr<IWebDataSource> dataSource;
    hr = WebFrame::dataSource(&dataSource);
    if (FAILED(hr))
        return hr;

    COMPtr<IWebURLResponse> urlResponse;
    hr = dataSource->response(&urlResponse);
    if (SUCCEEDED(hr) && urlResponse) {
        BSTR mimeTypeBStr;
        if (SUCCEEDED(urlResponse->MIMEType(&mimeTypeBStr))) {
            String mimeType(mimeTypeBStr, SysStringLen(mimeTypeBStr));
            *result = mimeType == "text/html" || WebCore::DOMImplementation::isXMLMIMEType(mimeType);
            SysFreeString(mimeTypeBStr);
        }
    }
    return hr;
}*/

void WebFrame::frameLoaderDestroyed()
{
    // The FrameLoader going away is equivalent to the Frame going away,
    // so we now need to clear our frame pointer.
    d->frame = 0;
}

void WebFrame::makeRepresentation(DocumentLoader*)
{
    BalNotImplemented();
}

void WebFrame::forceLayoutForNonHTML()
{
    BalNotImplemented();
}

void WebFrame::setCopiesOnScroll()
{
    BalNotImplemented();
}

void WebFrame::detachedFromParent2()
{
    BalNotImplemented();
}

void WebFrame::detachedFromParent3()
{
    BalNotImplemented();
}

void WebFrame::detachedFromParent4()
{
    BalNotImplemented();
}

void WebFrame::cancelPolicyCheck()
{
    if (d->m_policyListener) {
        d->m_policyListener->invalidate();
        d->m_policyListener = 0;
    }

    d->m_policyFunction = 0;
}

void WebFrame::dispatchWillSubmitForm(FramePolicyFunction function, PassRefPtr<FormState> formState)
{
    Frame* coreFrame = core(this);
    ASSERT(coreFrame);

    (coreFrame->loader()->*function)(PolicyUse);
}

void WebFrame::revertToProvisionalState(DocumentLoader*)
{
    BalNotImplemented();
}

void WebFrame::setMainFrameDocumentReady(bool)
{
    BalNotImplemented();
}

void WebFrame::willChangeTitle(DocumentLoader*)
{
    BalNotImplemented();
}

void WebFrame::didChangeTitle(DocumentLoader*)
{
    BalNotImplemented();
}

bool WebFrame::canHandleRequest(const WebCore::ResourceRequest& request) const
{
    return WebView::canHandleRequest(request);
}

bool WebFrame::canShowMIMEType(const String& /*MIMEType*/) const
{
    BalNotImplemented();
    return true;
}

bool WebFrame::representationExistsForURLScheme(const String& /*URLScheme*/) const
{
    BalNotImplemented();
    return false;
}

String WebFrame::generatedMIMETypeForURLScheme(const String& /*URLScheme*/) const
{
    BalNotImplemented();
    ASSERT_NOT_REACHED();
    return String();
}

void WebFrame::frameLoadCompleted()
{
    if (Frame* coreFrame = core(this))
        coreFrame->loader()->setPreviousHistoryItem(0);
}

void WebFrame::restoreViewState()
{
}

void WebFrame::provisionalLoadStarted()
{
    BalNotImplemented();
}

bool WebFrame::shouldTreatURLAsSameAsCurrent(const KURL&) const
{
    BalNotImplemented();
    return false;
}

void WebFrame::addHistoryItemForFragmentScroll()
{
    BalNotImplemented();
}

void WebFrame::didFinishLoad()
{
  BalNotImplemented();
}

//+ 11/23/09 CSidhall - added
bool WebFrame::isDocLoading()
{
    const bool loading = false;    

    Frame* coreFrame = core(this);
    if( (!coreFrame) && (!coreFrame->loader()) )
        return loading;

    DocumentLoader* pDocLoader = coreFrame->loader()->documentLoader();    
    if(!pDocLoader)
        return loading;

    if( (pDocLoader->isLoading()) ||
        (pDocLoader->isLoadingSubresources()) ||
        (pDocLoader->isLoadingMainResource()) ||
        (pDocLoader->isLoadingPlugIns()) )
    {
        return true;
    }

    return loading;
}
//-CS

void WebFrame::prepareForDataSourceReplacement()
{
    BalNotImplemented();
}

String WebFrame::userAgent(const KURL& url)
{
    return d->webView->userAgentForKURL(url);
}

void WebFrame::transitionToCommittedFromCachedPage(CachedPage*)
{
}

void WebFrame::saveViewStateToItem(HistoryItem*)
{
}

ResourceError WebFrame::cancelledError(const WebCore::ResourceRequest& request)
{
    // FIXME: Need ChickenCat to include CFNetwork/CFURLError.h to get these values
    // Alternatively, we could create our own error domain/codes.
    return ResourceError(String(WebURLErrorDomain), WebURLErrorCancelled, request.url().string(), GetWebkitErrorString(WebURLErrorCancelled));	
}

ResourceError WebFrame::blockedError(const WebCore::ResourceRequest& request)
{
    // FIXME: Need to implement the String descriptions for errors in the WebKitErrorDomain and have them localized
    return ResourceError(String(WebKitErrorDomain), WebKitErrorCannotUseRestrictedPort, request.url().string(), GetWebkitErrorString(WebKitErrorCannotUseRestrictedPort));	
	
}

ResourceError WebFrame::cannotShowURLError(const WebCore::ResourceRequest& request)
{
	// 09/03/10 Gautam Narain : ResourceError was an empty constructor. As we are logging the errors we need to send meaningful errors
	// for this error
    //BalNotImplemented();	
    return ResourceError(String(WebKitErrorDomain), WebKitErrorCannotShowURL, request.url().string(), GetWebkitErrorString(WebKitErrorCannotShowURL));	
}

ResourceError WebFrame::interruptForPolicyChangeError(const WebCore::ResourceRequest& request)
{
    // FIXME: Need to implement the String descriptions for errors in the WebKitErrorDomain and have them localized
    return ResourceError(String(WebKitErrorDomain), WebKitErrorFrameLoadInterruptedByPolicyChange, request.url().string(), GetWebkitErrorString(WebKitErrorFrameLoadInterruptedByPolicyChange));		
}

ResourceError WebFrame::cannotShowMIMETypeError(const ResourceResponse& response)
{
    //BalNotImplemented();
    //return ResourceError();
	return ResourceError(String(WebKitErrorDomain), WebKitErrorCannotShowMIMEType, response.url().string(), GetWebkitErrorString(WebKitErrorCannotShowMIMEType));	
}

ResourceError WebFrame::fileDoesNotExistError(const ResourceResponse& response)
{
    //BalNotImplemented();
    //return ResourceError();
	return ResourceError(String(WebKitErrorDomain), WebURLErrorFileDoesNotExist, response.url().string(), GetWebkitErrorString(WebURLErrorFileDoesNotExist));		
}

bool WebFrame::shouldFallBack(const ResourceError& error)
{
	return (error.errorCode() != WebURLErrorCancelled);	
}

WebFramePolicyListener* WebFrame::setUpPolicyListener(WebCore::FramePolicyFunction function)
{
    // FIXME: <rdar://5634381> We need to support multiple active policy listeners.

    if (d->m_policyListener)
        d->m_policyListener->invalidate();

    Frame* coreFrame = core(this);
    ASSERT(coreFrame);

    d->m_policyListener = WebFramePolicyListener::createInstance(coreFrame);
    d->m_policyFunction = function;

    return d->m_policyListener;
}

void WebFrame::receivedPolicyDecision(PolicyAction action)
{
    ASSERT(d->m_policyListener);
    ASSERT(d->m_policyFunction);

    FramePolicyFunction function = d->m_policyFunction;

    d->m_policyListener = 0;
    d->m_policyFunction = 0;

    Frame* coreFrame = core(this);
    ASSERT(coreFrame);

    (coreFrame->loader()->*function)(action);
}

void WebFrame::dispatchDecidePolicyForMIMEType(FramePolicyFunction function, const String& mimeType, const WebCore::ResourceRequest& request)
{
    Frame* coreFrame = core(this);
    ASSERT(coreFrame);

    DefaultPolicyDelegate* policyDelegate = d->webView->policyDelegate();
    if (!policyDelegate)
        policyDelegate = DefaultPolicyDelegate::sharedInstance();

    WebMutableURLRequest* urlRequest = WebMutableURLRequest::createInstance(request);

//    policyDelegate->decidePolicyForMIMEType(d->webView, mimeType, urlRequest, this, setUpPolicyListener(function));

    (coreFrame->loader()->*function)(PolicyUse);
    delete urlRequest;
}

void WebFrame::dispatchDecidePolicyForNewWindowAction(FramePolicyFunction function, const NavigationAction& action, const WebCore::ResourceRequest& request, PassRefPtr<FormState> formState, const String& frameName)
{
    Frame* coreFrame = core(this);
    ASSERT(coreFrame);

    DefaultPolicyDelegate* policyDelegate = d->webView->policyDelegate();
    if (!policyDelegate)
        policyDelegate = DefaultPolicyDelegate::sharedInstance();

    WebMutableURLRequest* urlRequest =  WebMutableURLRequest::createInstance(request);
    WebActionPropertyBag* actionInformation = WebActionPropertyBag::createInstance(action, coreFrame);

    //policyDelegate->decidePolicyForNewWindowAction(d->webView, actionInformation, urlRequest, frameName, setUpPolicyListener(function));

    (coreFrame->loader()->*function)(PolicyUse);
    delete urlRequest;
    delete actionInformation;
}

void WebFrame::dispatchDecidePolicyForNavigationAction(FramePolicyFunction function, const NavigationAction& action, const WebCore::ResourceRequest& request, PassRefPtr<FormState> formState)
{
    Frame* coreFrame = core(this);
    ASSERT(coreFrame);

    DefaultPolicyDelegate* policyDelegate = d->webView->policyDelegate();
    if (!policyDelegate) {
        d->webView->setPolicyDelegate(DefaultPolicyDelegate::sharedInstance());
        policyDelegate = d->webView->policyDelegate();
    }

    WebMutableURLRequest* urlRequest =  WebMutableURLRequest::createInstance(request);
    WebActionPropertyBag* actionInformation = WebActionPropertyBag::createInstance(action, coreFrame);

//FIXME
//    policyDelegate->decidePolicyForNavigationAction(d->webView, actionInformation, urlRequest, this, setUpPolicyListener(function));

    (coreFrame->loader()->*function)(PolicyUse);
    delete urlRequest;
    delete actionInformation;
}

void WebFrame::dispatchUnableToImplementPolicy(const ResourceError& error)
{
    DefaultPolicyDelegate* policyDelegate = d->webView->policyDelegate();
    if (!policyDelegate)
        policyDelegate = DefaultPolicyDelegate::sharedInstance();

    WebError* webError = WebError::createInstance(error);
    policyDelegate->unableToImplementPolicyWithError(d->webView, webError, this);

    delete webError;
}

void WebFrame::download(ResourceHandle* handle, const WebCore::ResourceRequest& request, const WebCore::ResourceRequest&, const ResourceResponse& response)
{
    DefaultDownloadDelegate* downloadDelegate;
    WebView* webView = this->webView();
    downloadDelegate = webView->downloadDelegate();
    if(!downloadDelegate)
    {
        // If the WebView doesn't successfully provide a download delegate we'll pass a null one
        // into the WebDownload - which may or may not decide to use a DefaultDownloadDelegate
        //LOG_ERROR("Failed to get downloadDelegate from WebView");
        downloadDelegate = 0;
    }

    // Its the delegate's job to ref the WebDownload to keep it alive - otherwise it will be destroyed
    // when this method returns
    WebDownload* download = WebDownload::createInstance(handle, request, response, downloadDelegate);
    delete download;
}

bool WebFrame::dispatchDidLoadResourceFromMemoryCache(DocumentLoader*, const WebCore::ResourceRequest&, const ResourceResponse&, int /*length*/)
{
    BalNotImplemented();
    return false;
}

void WebFrame::dispatchDidFailProvisionalLoad(const ResourceError& error)
{
    BalNotImplemented();
}

void WebFrame::dispatchDidFailLoad(const ResourceError& error)
{
    BalNotImplemented();
}

void WebFrame::startDownload(const WebCore::ResourceRequest&)
{
    BalNotImplemented();
}

Widget* WebFrame::createJavaAppletWidget(const IntSize& pluginSize, Element* element, const KURL& /*baseURL*/, const Vector<String>& paramNames, const Vector<String>& paramValues)
{
    PluginView* pluginView = PluginView::create(core(this), pluginSize, element, KURL(), paramNames, paramValues, "application/x-java-applet", false);

    // Check if the plugin can be loaded successfully
    if (pluginView->plugin() && pluginView->plugin()->load())
        return pluginView;

    return pluginView;
}

ObjectContentType WebFrame::objectContentType(const KURL& url, const String& mimeTypeIn)
{
    String mimeType = mimeTypeIn;
    if (mimeType.isEmpty())
        mimeType = MIMETypeRegistry::getMIMETypeForExtension(url.path().substring(url.path().reverseFind('.') + 1));

    if (mimeType.isEmpty())
        return ObjectContentFrame; // Go ahead and hope that we can display the content.

    if (MIMETypeRegistry::isSupportedImageMIMEType(mimeType))
        return WebCore::ObjectContentImage;

    if (PluginDatabase::installedPlugins()->isMIMETypeRegistered(mimeType))
        return WebCore::ObjectContentNetscapePlugin;

    if (MIMETypeRegistry::isSupportedNonImageMIMEType(mimeType))
        return WebCore::ObjectContentFrame;

    return WebCore::ObjectContentNone;
}

String WebFrame::overrideMediaType() const
{
    BalNotImplemented();
    return String();
}

void WebFrame::windowObjectCleared()
{
    Frame* coreFrame = core(this);
    ASSERT(coreFrame);

    Settings* settings = coreFrame->settings();
    if (!settings || !settings->isJavaScriptEnabled())
        return;

//     COMPtr<IWebFrameLoadDelegate> frameLoadDelegate;
//     if (SUCCEEDED(d->webView->frameLoadDelegate(&frameLoadDelegate))) {
//         COMPtr<IWebFrameLoadDelegate2> frameLoadDelegate2(Query, frameLoadDelegate);
// 
//         JSContextRef context = toRef(coreFrame->scriptProxy()->globalObject()->globalExec());
//         JSObjectRef windowObject = toRef(coreFrame->scriptProxy()->globalObject());
//         ASSERT(windowObject);
// 
//         if (!frameLoadDelegate2 || 
//             FAILED(frameLoadDelegate2->didClearWindowObject(d->webView, context, windowObject, this)))
//             frameLoadDelegate->windowScriptObjectAvailable(d->webView, context, windowObject);
//     }
#ifdef __BINDING_JS__
    if (m_bindingJS)
        m_bindingJS->registerBinding();
#endif
}

void WebFrame::didPerformFirstNavigation() const
{
    WebPreferences* preferences = d->webView->preferences();
    if(!preferences)
        return;

    bool automaticallyDetectsCacheModel = preferences->automaticallyDetectsCacheModel();
    
    WebCacheModel cacheModel = preferences->cacheModel();

    if (automaticallyDetectsCacheModel && cacheModel < WebCacheModelDocumentBrowser)
        preferences->setCacheModel(WebCacheModelDocumentBrowser);
}

void WebFrame::registerForIconNotification(bool listen)
{
#if ENABLE(ICON_DATABASE)
    d->webView->registerForIconNotification(listen);
#endif
}

/*static IntRect printerRect(HDC printDC)
{
    return IntRect(0, 0, 
                   GetDeviceCaps(printDC, PHYSICALWIDTH)  - 2 * GetDeviceCaps(printDC, PHYSICALOFFSETX),
                   GetDeviceCaps(printDC, PHYSICALHEIGHT) - 2 * GetDeviceCaps(printDC, PHYSICALOFFSETY));
}

void WebFrame::setPrinting(bool printing, float minPageWidth, float maxPageWidth, bool adjustViewSize)
{
    Frame* coreFrame = core(this);
    ASSERT(coreFrame);
    coreFrame->setPrinting(printing, minPageWidth, maxPageWidth, adjustViewSize);
}

HRESULT STDMETHODCALLTYPE WebFrame::setInPrintingMode( BOOL value, HDC printDC)
{
    if (m_inPrintingMode == !!value)
        return S_OK;

    Frame* coreFrame = core(this);
    if (!coreFrame)
        return E_FAIL;

    m_inPrintingMode = !!value;

    // If we are a frameset just print with the layout we have onscreen, otherwise relayout
    // according to the paper size
    float minLayoutWidth = 0.0f;
    float maxLayoutWidth = 0.0f;
    if (m_inPrintingMode && !coreFrame->isFrameSet()) {
        if (!printDC) {
            ASSERT_NOT_REACHED();
            return E_POINTER;
        }

        const int desiredHorizontalPixelsPerInch = 72;
        int paperHorizontalPixelsPerInch = ::GetDeviceCaps(printDC, LOGPIXELSX);
        int paperWidth = printerRect(printDC).width() * desiredHorizontalPixelsPerInch / paperHorizontalPixelsPerInch;
        minLayoutWidth = paperWidth * PrintingMinimumShrinkFactor;
        maxLayoutWidth = paperWidth * PrintingMaximumShrinkFactor;
    }

    setPrinting(m_inPrintingMode, minLayoutWidth, maxLayoutWidth, true);

    if (!m_inPrintingMode)
        m_pageRects.clear();

    return S_OK;
}

void WebFrame::headerAndFooterHeights(float* headerHeight, float* footerHeight)
{
    if (headerHeight)
        *headerHeight = 0;
    if (footerHeight)
        *footerHeight = 0;
    float height = 0;
    COMPtr<IWebUIDelegate> ui;
    if (FAILED(d->webView->uiDelegate(&ui)))
        return;
    COMPtr<IWebUIDelegate2> ui2;
    if (FAILED(ui->QueryInterface(IID_IWebUIDelegate2, (void**) &ui2)))
        return;
    if (headerHeight && SUCCEEDED(ui2->webViewHeaderHeight(d->webView, &height)))
        *headerHeight = height;
    if (footerHeight && SUCCEEDED(ui2->webViewFooterHeight(d->webView, &height)))
        *footerHeight = height;
}

IntRect WebFrame::printerMarginRect(HDC printDC)
{
    IntRect emptyRect(0, 0, 0, 0);

    COMPtr<IWebUIDelegate> ui;
    if (FAILED(d->webView->uiDelegate(&ui)))
        return emptyRect;
    COMPtr<IWebUIDelegate2> ui2;
    if (FAILED(ui->QueryInterface(IID_IWebUIDelegate2, (void**) &ui2)))
        return emptyRect;

    RECT rect;
    if (FAILED(ui2->webViewPrintingMarginRect(d->webView, &rect)))
        return emptyRect;

    rect.left = MulDiv(rect.left, ::GetDeviceCaps(printDC, LOGPIXELSX), 1000);
    rect.top = MulDiv(rect.top, ::GetDeviceCaps(printDC, LOGPIXELSY), 1000);
    rect.right = MulDiv(rect.right, ::GetDeviceCaps(printDC, LOGPIXELSX), 1000);
    rect.bottom = MulDiv(rect.bottom, ::GetDeviceCaps(printDC, LOGPIXELSY), 1000);

    return IntRect(rect.left, rect.top, (rect.right - rect.left), rect.bottom - rect.top);
}

const Vector<WebCore::IntRect>& WebFrame::computePageRects(HDC printDC)
{
    ASSERT(m_inPrintingMode);
    
    Frame* coreFrame = core(this);
    ASSERT(coreFrame);
    ASSERT(coreFrame->document());

    if (!printDC)
        return m_pageRects;

    // adjust the page rect by the header and footer
    float headerHeight = 0, footerHeight = 0;
    headerAndFooterHeights(&headerHeight, &footerHeight);
    IntRect pageRect = printerRect(printDC);
    IntRect marginRect = printerMarginRect(printDC);
    IntRect adjustedRect = IntRect(
        pageRect.x() + marginRect.x(),
        pageRect.y() + marginRect.y(),
        pageRect.width() - marginRect.x() - marginRect.right(),
        pageRect.height() - marginRect.y() - marginRect.bottom());

    computePageRectsForFrame(coreFrame, adjustedRect, headerHeight, footerHeight, 1.0,m_pageRects, m_pageHeight);
    
    return m_pageRects;
}

UINT WebFrame::getPrintedPageCount( HDC printDC)
{
    if (!pageCount || !printDC) {
        ASSERT_NOT_REACHED();
        return E_POINTER;
    }

    *pageCount = 0;

    if (!m_inPrintingMode) {
        ASSERT_NOT_REACHED();
        return E_FAIL;
    }

    Frame* coreFrame = core(this);
    if (!coreFrame || !coreFrame->document())
        return E_FAIL;

    const Vector<IntRect>& pages = computePageRects(printDC);
    *pageCount = (UINT) pages.size();
    
    return S_OK;
}

void* WebFrame::spoolPages(HDC printDC, UINT startPage, UINT endPage)
{
    if (!printDC || !ctx) {
        ASSERT_NOT_REACHED();
        return E_POINTER;
    }

    if (!m_inPrintingMode) {
        ASSERT_NOT_REACHED();
        return E_FAIL;
    }

    Frame* coreFrame = core(this);
    if (!coreFrame || !coreFrame->document())
        return E_FAIL;

    UINT pageCount = (UINT) m_pageRects.size();
    PlatformGraphicsContext* pctx = (PlatformGraphicsContext*)ctx;

    if (!pageCount || startPage > pageCount) {
        ASSERT_NOT_REACHED();
        return E_FAIL;
    }

    if (startPage > 0)
        startPage--;

    if (endPage == 0)
        endPage = pageCount;

    COMPtr<IWebUIDelegate> ui;
    if (FAILED(d->webView->uiDelegate(&ui)))
        return E_FAIL;
    // FIXME: we can return early after the updated app is released
    COMPtr<IWebUIDelegate2> ui2;
    if (FAILED(ui->QueryInterface(IID_IWebUIDelegate2, (void**) &ui2)))
        ui2 = 0;

    float headerHeight = 0, footerHeight = 0;
    headerAndFooterHeights(&headerHeight, &footerHeight);
    GraphicsContext spoolCtx(pctx);

    for (UINT ii = startPage; ii < endPage; ii++) {
        IntRect pageRect = m_pageRects[ii];

        CGContextSaveGState(pctx);

        IntRect printRect = printerRect(printDC);
        CGRect mediaBox = CGRectMake(CGFloat(0),
                                     CGFloat(0),
                                     CGFloat(printRect.width()),
                                     CGFloat(printRect.height()));

        CGContextBeginPage(pctx, &mediaBox);

        CGFloat scale = (float)mediaBox.size.width/ (float)pageRect.width();
        CGAffineTransform ctm = CGContextGetBaseCTM(pctx);
        ctm = CGAffineTransformScale(ctm, -scale, -scale);
        ctm = CGAffineTransformTranslate(ctm, CGFloat(-pageRect.x()), CGFloat(-pageRect.y()+headerHeight)); // reserves space for header
        CGContextScaleCTM(pctx, scale, scale);
        CGContextTranslateCTM(pctx, CGFloat(-pageRect.x()), CGFloat(-pageRect.y()+headerHeight));   // reserves space for header
        CGContextSetBaseCTM(pctx, ctm);

        coreFrame->paint(&spoolCtx, pageRect);

        if (ui2) {
            CGContextTranslateCTM(pctx, CGFloat(pageRect.x()), CGFloat(pageRect.y())-headerHeight);

            int x = pageRect.x();
            int y = 0;
            if (headerHeight) {
                RECT headerRect = {x, y, x+pageRect.width(), y+(int)headerHeight};
                ui2->drawHeaderInRect(d->webView, &headerRect, (OLE_HANDLE)(LONG64)pctx);
            }

            if (footerHeight) {
                y = max((int)headerHeight+pageRect.height(), m_pageHeight-(int)footerHeight);
                RECT footerRect = {x, y, x+pageRect.width(), y+(int)footerHeight};
                ui2->drawFooterInRect(d->webView, &footerRect, (OLE_HANDLE)(LONG64)pctx, ii+1, pageCount);
            }
        }

        CGContextEndPage(pctx);
        CGContextRestoreGState(pctx);
    }
 
    return S_OK;
}*/

bool WebFrame::isFrameSet()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return false;

    return coreFrame->isFrameSet() ? true : false;
}

String WebFrame::string()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return String();

    RefPtr<Range> allRange(rangeOfContents(coreFrame->document()));
    return plainText(allRange.get());
}

IntSize WebFrame::size()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return IntSize();
    FrameView* view = coreFrame->view();
    if (!view)
        return IntSize();
    return IntSize(view->width(), view->height());
}

bool WebFrame::hasScrollBars()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return false;

    FrameView* view = coreFrame->view();
    if (!view)
        return false;

    if (view->vScrollbarMode() == ScrollbarAlwaysOn || view->visibleHeight() < view->contentsHeight() ||
            view->hScrollbarMode() == ScrollbarAlwaysOn || view->visibleWidth() < view->contentsWidth())
        return true;
    return false;
}

IntRect WebFrame::contentBounds()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return IntRect();

    FrameView* view = coreFrame->view();
    if (!view)
        return IntRect();

    return IntRect(0, 0, view->contentsWidth(), view->contentsHeight());
}

IntRect WebFrame::frameBounds()
{
    Frame* coreFrame = core(this);
    if (!coreFrame)
        return IntRect();

    FrameView* view = coreFrame->view();
    if (!view)
        return IntRect();

    FloatRect bounds = view->visibleContentRectConsideringExternalScrollers();
    return IntRect(0, 0, (int)bounds.width(), (int)bounds.height());
}

bool WebFrame::isDescendantOfFrame(WebFrame *ancestor)
{
    Frame* coreFrame = core(this);

    return (coreFrame && coreFrame->tree()->isDescendantOf(core(ancestor))) ? true : false;
}

void WebFrame::unmarkAllMisspellings()
{
    Frame* coreFrame = core(this);
    for (Frame* frame = coreFrame; frame; frame = frame->tree()->traverseNext(coreFrame)) {
        Document *doc = frame->document();
        if (!doc)
            return;

        doc->removeMarkers(DocumentMarker::Spelling);
    }
}

void WebFrame::unmarkAllBadGrammar()
{
    Frame* coreFrame = core(this);
    for (Frame* frame = coreFrame; frame; frame = frame->tree()->traverseNext(coreFrame)) {
        Document *doc = frame->document();
        if (!doc)
            return;

        doc->removeMarkers(DocumentMarker::Grammar);
    }
}

WebView* WebFrame::webView() const
{
    return d->webView;
}

void WebFrame::observe(const String &topic, BalObject *obj)
{
    ASSERT(obj);
    ASSERT(obj->getName() != "");
    if (topic == "AddonRegister")
        addToJSWindowObject(obj->getName().utf8().data(), (void *)obj);
}

void WebFrame::addToJSWindowObject(const char* name, void *object)
{
    KJS::JSLock lock;
    JSDOMWindow *window = toJSDOMWindow(core(this));
    KJS::Bindings::RootObject *root = core(this)->bindingRootObject();
    if (!window) {
        return;
    }

    KJS::ExecState* exec = window->globalExec();
    KJS::JSObject *runtimeObject = KJS::Bindings::Instance::createRuntimeObject(exec, KJS::Bindings::BalInstance::create(static_cast<BalObject*>(object), root));

    window->put(exec, KJS::Identifier(exec, name), runtimeObject);
}



