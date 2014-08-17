#ifndef FrameLoaderClientISee_H
#define FrameLoaderClientISee_H

#include "FrameLoaderClient.h"
#include "ResourceResponse.h"
#include "PluginView.h"

namespace WebCore {
    class Frame;
}

class WebView;

namespace WebKit {

class FrameLoaderClient : public WebCore::FrameLoaderClient {
public:
    FrameLoaderClient(WebView *webView);
    virtual ~FrameLoaderClient() {}
	virtual void frameLoaderDestroyed();
    
    virtual bool hasWebView() const;
    virtual bool hasFrameView() const;

    virtual void makeRepresentation(WebCore::DocumentLoader*);
    virtual void forceLayout();
    virtual void forceLayoutForNonHTML();

    virtual void setCopiesOnScroll();

    virtual void detachedFromParent1();
    virtual void detachedFromParent2();
    virtual void detachedFromParent3();
    virtual void detachedFromParent4();

    virtual void loadedFromCachedPage();

    virtual void assignIdentifierToInitialRequest(unsigned long identifier, WebCore::DocumentLoader*, const WebCore::ResourceRequest&);

    virtual void dispatchWillSendRequest(WebCore::DocumentLoader*, unsigned long  identifier, WebCore::ResourceRequest&, const WebCore::ResourceResponse& redirectResponse);
    virtual void dispatchDidReceiveAuthenticationChallenge(WebCore::DocumentLoader*, unsigned long identifier, const WebCore::AuthenticationChallenge&);
    virtual void dispatchDidCancelAuthenticationChallenge(WebCore::DocumentLoader*, unsigned long  identifier, const WebCore::AuthenticationChallenge&);
    virtual void dispatchDidReceiveResponse(WebCore::DocumentLoader*, unsigned long  identifier, const WebCore::ResourceResponse&);
    virtual void dispatchDidReceiveContentLength(WebCore::DocumentLoader*, unsigned long identifier, int lengthReceived);
    virtual void dispatchDidFinishLoading(WebCore::DocumentLoader*, unsigned long  identifier);
    virtual void dispatchDidFailLoading(WebCore::DocumentLoader*, unsigned long  identifier, const WebCore::ResourceError&);
    virtual bool dispatchDidLoadResourceFromMemoryCache(WebCore::DocumentLoader*, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&, int length);

    virtual void dispatchDidHandleOnloadEvents();
    virtual void dispatchDidReceiveServerRedirectForProvisionalLoad();
    virtual void dispatchDidCancelClientRedirect();
    virtual void dispatchWillPerformClientRedirect(const WebCore::KURL&, double, double);
    virtual void dispatchDidChangeLocationWithinPage();
    virtual void dispatchWillClose();
    virtual void dispatchDidReceiveIcon();
    virtual void dispatchDidStartProvisionalLoad();
    virtual void dispatchDidReceiveTitle(const WebCore::String&);
    virtual void dispatchDidCommitLoad();
    virtual void dispatchDidFailProvisionalLoad(const WebCore::ResourceError&);
    virtual void dispatchDidFailLoad(const WebCore::ResourceError&);
    virtual void dispatchDidFinishDocumentLoad();
    virtual void dispatchDidFinishLoad();
    virtual void dispatchDidFirstLayout();

    virtual WebCore::Frame* dispatchCreatePage();
    virtual void dispatchShow();

    virtual void dispatchDecidePolicyForMIMEType(WebCore::FramePolicyFunction, const WebCore::String& MIMEType, const WebCore::ResourceRequest&);
	virtual void dispatchDecidePolicyForNewWindowAction(WebCore::FramePolicyFunction, const WebCore::NavigationAction&, const WebCore::ResourceRequest&, PassRefPtr<WebCore::FormState>, const WebCore::String&);
    virtual void dispatchDecidePolicyForNavigationAction(WebCore::FramePolicyFunction, const WebCore::NavigationAction&, const WebCore::ResourceRequest&, PassRefPtr<WebCore::FormState>);
    virtual void cancelPolicyCheck();

    virtual void dispatchUnableToImplementPolicy(const WebCore::ResourceError&);

    virtual void dispatchWillSubmitForm(WebCore::FramePolicyFunction, WTF::PassRefPtr<WebCore::FormState>);

    virtual void dispatchDidLoadMainResource(WebCore::DocumentLoader*);
    virtual void revertToProvisionalState(WebCore::DocumentLoader*);
    virtual void setMainDocumentError(WebCore::DocumentLoader*, const WebCore::ResourceError&);
    
    virtual void postProgressStartedNotification();
    virtual void postProgressEstimateChangedNotification();
    virtual void postProgressFinishedNotification();

    virtual PassRefPtr<WebCore::Frame> createFrame(const WebCore::KURL& url, const WebCore::String& name, WebCore::HTMLFrameOwnerElement* ownerElement,
                                   const WebCore::String& referrer, bool allowsScrolling, int marginWidth, int marginHeight);
    virtual WebCore::Widget* createPlugin(const WebCore::IntSize&, WebCore::Element*, const WebCore::KURL&, const WTF::Vector<WebCore::String>&, const WTF::Vector<WebCore::String>&, const WebCore::String&, bool);
    virtual void redirectDataToPlugin(WebCore::Widget* pluginWidget);
    virtual WebCore::Widget* createJavaAppletWidget(const WebCore::IntSize&, WebCore::Element*, const WebCore::KURL& baseURL, const WTF::Vector<WebCore::String>& paramNames, const WTF::Vector<WebCore::String>& paramValues);
    virtual WebCore::String overrideMediaType() const;
	virtual void windowObjectCleared();
	virtual void didPerformFirstNavigation() const;

	virtual void registerForIconNotification(bool);

    virtual WebCore::ObjectContentType objectContentType(const WebCore::KURL& url, const WebCore::String& mimeType);

    virtual void setMainFrameDocumentReady(bool);

    virtual void startDownload(const WebCore::ResourceRequest&);

    virtual void willChangeTitle(WebCore::DocumentLoader*);
    virtual void didChangeTitle(WebCore::DocumentLoader*);

    virtual void committedLoad(WebCore::DocumentLoader*, const char*, int);
    virtual void finishedLoading(WebCore::DocumentLoader*);
    
	virtual void updateGlobalHistory(const WebCore::KURL&);
    virtual bool shouldGoToHistoryItem(WebCore::HistoryItem*) const;

    virtual WebCore::ResourceError cancelledError(const WebCore::ResourceRequest&);
    virtual WebCore::ResourceError blockedError(const WebCore::ResourceRequest&);
    virtual WebCore::ResourceError cannotShowURLError(const WebCore::ResourceRequest&);
    virtual WebCore::ResourceError interruptForPolicyChangeError(const WebCore::ResourceRequest&);

    virtual WebCore::ResourceError cannotShowMIMETypeError(const WebCore::ResourceResponse&);
    virtual WebCore::ResourceError fileDoesNotExistError(const WebCore::ResourceResponse&);
	virtual WebCore::ResourceError pluginWillHandleLoadError(const WebCore::ResourceResponse&);

    virtual bool shouldFallBack(const WebCore::ResourceError&);

    virtual bool canHandleRequest(const WebCore::ResourceRequest&) const;
    virtual bool canShowMIMEType(const WebCore::String&) const;
    virtual bool representationExistsForURLScheme(const WebCore::String&) const;
    virtual WebCore::String generatedMIMETypeForURLScheme(const WebCore::String&) const;

    virtual void frameLoadCompleted();
    virtual void saveViewStateToItem(WebCore::HistoryItem*);
    virtual void restoreViewState();
    virtual void provisionalLoadStarted();
    virtual void didFinishLoad();
    virtual void prepareForDataSourceReplacement();

    virtual WTF::PassRefPtr<WebCore::DocumentLoader> createDocumentLoader(const WebCore::ResourceRequest&, const WebCore::SubstituteData&);
    virtual void setTitle(const WebCore::String& title, const WebCore::KURL&);

    virtual WebCore::String userAgent(const WebCore::KURL&);

	virtual void savePlatformDataToCachedPage(WebCore::CachedPage*);
	virtual void transitionToCommittedFromCachedPage(WebCore::CachedPage*);
	virtual void transitionToCommittedForNewPage();

    virtual bool canCachePage() const;
    virtual void download(WebCore::ResourceHandle*, const WebCore::ResourceRequest&, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&);

public: // 该类特有的
	void setFrame(WebCore::Frame *frame) { m_frame = frame; }
	WebCore::Frame *frame() { return m_frame; }

private:
	WebView *m_webView;
	WebCore::Frame *m_frame;
    WebCore::ResourceResponse m_response;
    WebCore::String m_userAgent;

    // Plugin view to redirect data to
    WebCore::PluginView* m_pluginView;
    bool m_hasSentResponseToPlugin;
};

}

#endif
