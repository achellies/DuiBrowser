#ifndef ChromeClientISee_h
#define ChromeClientISee_h

#include "ChromeClient.h"
#include "KURL.h"

class WebView;

namespace WebKit {

class ChromeClient : public WebCore::ChromeClient {
public:
	ChromeClient(WebView *webView);
    virtual ~ChromeClient() {}
	
	WebView* webView() const { return m_webView; }

    virtual void chromeDestroyed();

    virtual void setWindowRect(const WebCore::FloatRect&);
    virtual WebCore::FloatRect windowRect();

    virtual WebCore::FloatRect pageRect();

    virtual float scaleFactor();

    virtual void focus();
    virtual void unfocus();

	virtual bool canTakeFocus(WebCore::FocusDirection);
    virtual void takeFocus(WebCore::FocusDirection);

    virtual WebCore::Page* createWindow(WebCore::Frame*, const WebCore::FrameLoadRequest&, const WebCore::WindowFeatures&);
    virtual void show();

    virtual bool canRunModal();
    virtual void runModal();

    virtual void setToolbarsVisible(bool);
    virtual bool toolbarsVisible();

    virtual void setStatusbarVisible(bool);
    virtual bool statusbarVisible();

    virtual void setScrollbarsVisible(bool);
    virtual bool scrollbarsVisible();

    virtual void setMenubarVisible(bool);
    virtual bool menubarVisible();

    virtual void setResizable(bool);

    virtual void addMessageToConsole(const WebCore::String& message, unsigned int lineNumber,
                                     const WebCore::String& sourceID);

    virtual bool canRunBeforeUnloadConfirmPanel();
    virtual bool runBeforeUnloadConfirmPanel(const WebCore::String& message, WebCore::Frame* frame);

    virtual void closeWindowSoon();

    virtual void runJavaScriptAlert(WebCore::Frame*, const WebCore::String&);
    virtual bool runJavaScriptConfirm(WebCore::Frame*, const WebCore::String&);
    virtual bool runJavaScriptPrompt(WebCore::Frame*, const WebCore::String& message, const WebCore::String& defaultValue, WebCore::String& result);
    virtual void setStatusbarText(const WebCore::String&);
    virtual bool shouldInterruptJavaScript();
    virtual bool tabsToLinks() const;

    virtual WebCore::IntRect windowResizerRect() const;
    virtual void addToDirtyRegion(const WebCore::IntRect&);
    virtual void scrollBackingStore(int dx, int dy, const WebCore::IntRect& scrollViewRect, const WebCore::IntRect& clipRect);
    virtual void updateBackingStore();

    virtual void mouseDidMoveOverElement(const WebCore::HitTestResult&, unsigned modifierFlags);

    virtual void setToolTip(const WebCore::String&);

	virtual void print(WebCore::Frame*);

	virtual void exceededDatabaseQuota(WebCore::Frame*, const WebCore::String&);

private:
	WebView *m_webView;
	WebCore::KURL m_hoveredLinkURL;
};

} // namespace WebKit

#endif // ChromeClientISee_h
