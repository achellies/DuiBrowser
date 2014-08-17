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


#ifndef WebView_H
#define WebView_H


#include "BALBase.h"
#include "WebFrame.h"
#include "WebPreferences.h"
#include <IntRect.h>
#include <Timer.h>
#include <ObserverData.h>
#include <wtf/OwnPtr.h>
#include "GraphicsContext.h"
#include "BALBase.h"
#include "BAL/WKAL/Concretizations/Widgets/EA/BCTextPopupEA.h"

class WebFrame;
class WebBackForwardList;
class WebInspector;
class WebInspectorClient;
class WebViewPrivate;
class DefaultDownloadDelegate;
class DefaultPolicyDelegate;
class WebHistoryItem;
class WebScriptObject;
class WebElementPropertyBag;


WebView* kit(WebCore::Page*);
WebCore::Page* core(WebView*);

namespace WebCore {
    class String;
    class IntRect;
    class Image;
    class IntPoint;
}


#define WebViewProgressStartedNotification         "WebProgressStartedNotification"
#define WebViewProgressEstimateChangedNotification "WebProgressEstimateChangedNotification"
#define WebViewProgressFinishedNotification        "WebProgressFinishedNotification"

class MouseEventPrivate;

class WebView 
    : public OWBAL::BCObserverData
{
public:
    static WebView* createInstance();

//+daw ca 28/07/2008 static and global management
    // 2/17/09 CSidhall - Split up uninit and finalize to fix dependencies (staticFinalize needs idAttr)
    void static unInitPart1();
    void static unInitPart2();
    void static staticFinalizePart1();
    void static staticFinalizePart2();
    static WebBackForwardList* m_ptrWebBack;
//-daw ca

    virtual ~WebView();

protected:
    WebView();

public:
    virtual bool canShowMIMEType(WebCore::String mimeType);
    virtual bool canShowMIMETypeAsHTML(WebCore::String mimeType);
    virtual void setMIMETypesShownAsHTML(WebCore::String mimeTypes, int cMimeTypes);
    virtual void initWithFrame(WebCore::IntRect frame, WebCore::String frameName, WebCore::String groupName);
    virtual void setDownloadDelegate(DefaultDownloadDelegate *d);
    virtual DefaultDownloadDelegate *downloadDelegate();
    virtual void setPolicyDelegate(DefaultPolicyDelegate *d);
    virtual DefaultPolicyDelegate *policyDelegate();
    virtual WebFrame *mainFrame();
    virtual WebFrame *focusedFrame();
    virtual WebBackForwardList *backForwardList();
    virtual void setMaintainsBackForwardList(bool flag);
    virtual bool goBack();
    virtual bool goForward();
    virtual bool goToBackForwardItem(WebHistoryItem *item);
    virtual void setTextSizeMultiplier(float multiplier);
    virtual float textSizeMultiplier();
    virtual void setApplicationNameForUserAgent(WebCore::String applicationName);
    virtual WebCore::String applicationNameForUserAgent();
    virtual void setCustomUserAgent(WebCore::String userAgentString);
    virtual WebCore::String customUserAgent();
    virtual WebCore::String userAgentForURL(WebCore::String url);
    virtual bool supportsTextEncoding();
    virtual void setCustomTextEncodingName(WebCore::String encodingName);
    virtual WebCore::String customTextEncodingName();
    virtual void setMediaStyle(WebCore::String media);
    virtual WebCore::String mediaStyle();
    virtual WebCore::String stringByEvaluatingJavaScriptFromString(WebCore::String script);
    virtual WebScriptObject *windowScriptObject();
    virtual void setPreferences(WebPreferences *prefs);
    virtual WebPreferences *preferences();
    virtual void setPreferencesIdentifier(WebCore::String anIdentifier);
    virtual WebCore::String preferencesIdentifier();
    //virtual void setHostWindow(BalWidget *window);
    //virtual BalWidget *hostWindow();
    virtual bool searchFor(WebCore::String str, bool forward, bool caseFlag, bool wrapFlag);
    virtual void setGroupName(WebCore::String);
    virtual WebCore::String groupName();
    virtual double estimatedProgress();
    virtual bool isLoading();
    virtual WebElementPropertyBag* elementAtPoint(WebCore::IntPoint point);
	virtual WebCore::Element* elementAtPoint(int x, int y);
    virtual WebCore::String selectedText();
    virtual void centerSelectionInVisibleArea();
    virtual void moveDragCaretToPoint(WebCore::IntPoint point);
    virtual void removeDragCaret();
    virtual void setDrawsBackground(bool drawsBackground);
    virtual bool drawsBackground();
    virtual void setMainFrameURL(WebCore::String urlString);
    virtual WebCore::String mainFrameURL();
    virtual WebCore::Document* mainFrameDocument();
    virtual WebCore::String mainFrameTitle();
    virtual void registerURLSchemeAsLocal(WebCore::String scheme);
    virtual void setSmartInsertDeleteEnabled(bool flag);
    virtual bool smartInsertDeleteEnabled();
    virtual void setContinuousSpellCheckingEnabled(bool flag);
    virtual bool isContinuousSpellCheckingEnabled();
    virtual int spellCheckerDocumentTag();
    //virtual WebUndoManager *undoManager();
    //virtual void setEditingDelegate(WebEditingDelegate *d);
    //virtual WebEditingDelegate * editingDelegate();
    virtual bool hasSelectedRange();
    virtual bool cutEnabled();
    virtual bool copyEnabled();
    virtual bool pasteEnabled();
    virtual bool deleteEnabled();
    virtual bool editingEnabled();
    virtual bool isGrammarCheckingEnabled();
    virtual void setGrammarCheckingEnabled(bool enabled);
    virtual void setPageSizeMultiplier(float multiplier);
    virtual float pageSizeMultiplier();

    virtual bool canZoomPageIn();
    virtual void zoomPageIn();
    virtual bool canZoomPageOut();
    virtual void zoomPageOut();
    virtual bool canResetPageZoom();
    virtual void resetPageZoom();

    virtual bool canMakeTextLarger();
    virtual void makeTextLarger();
    virtual bool canMakeTextSmaller();
    virtual void makeTextSmaller();
    virtual bool canMakeTextStandardSize();
    virtual void makeTextStandardSize();

    virtual void replaceSelectionWithText(WebCore::String text);
    virtual void replaceSelectionWithMarkupString(WebCore::String markupString);
    virtual void replaceSelectionWithArchive(WebArchive *archive);
    virtual void deleteSelection();
    virtual void clearSelection();

    virtual void copy();
    virtual void cut();
    virtual void paste();
    virtual void copyURL(WebCore::String url);
    virtual void copyFont();
    virtual void pasteFont();
    virtual void delete_();
    virtual void pasteAsPlainText();
    virtual void pasteAsRichText();

    virtual void changeFont();
    virtual void changeAttributes();
    virtual void changeDocumentBackgroundColor();
    virtual void changeColor();

    virtual void alignCenter();
    virtual void alignJustified();
    virtual void alignLeft();
    virtual void alignRight();

    virtual void checkSpelling();
    virtual void showGuessPanel();
    virtual void performFindPanelAction();
    virtual void startSpeaking();
    virtual void stopSpeaking();

    virtual void observe(const WebCore::String &topic, const WebCore::String &data, void *userData);
    //virtual void setCustomDropTarget(DropTarget* dt);
    //virtual void removeCustomDropTarget();

    virtual void setInViewSourceMode(bool flag);
    virtual bool inViewSourceMode();

    virtual BalWidget* viewWindow();
    virtual void setViewWindow(BalWidget*);

    virtual WebCore::IntPoint scrollOffset();
    virtual void scrollBy(WebCore::IntPoint offset);

    virtual WebCore::IntRect visibleContentRect();
    virtual void updateFocusedAndActiveState();

    virtual void executeCoreCommandByName(WebCore::String name, WebCore::String value);
    virtual unsigned int markAllMatchesForText(WebCore::String search, bool caseSensitive, bool highlight, unsigned int limit);
    virtual void unmarkAllTextMatches();
    virtual Vector<WebCore::IntRect> rectsForTextMatches();
    virtual WebCore::Image* generateSelectionImage(bool forceWhiteText);

    virtual WebCore::IntRect selectionRect();

    // virtual  DragEnter(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
    // virtual  DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
    // virtual  DragLeave();
    // virtual  Drop(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);

    virtual bool canHandleRequest(WebMutableURLRequest *request);
    virtual void clearFocusNode();
    virtual void setInitialFocus(bool forward);
    virtual void setTabKeyCyclesThroughElements(bool cycles);
    virtual bool tabKeyCyclesThroughElements();
    virtual void setAllowSiteSpecificHacks(bool allows);
    virtual void addAdditionalPluginDirectory(WebCore::String directory);
    virtual void loadBackForwardListFromOtherView(WebView *otherView);
    virtual WebInspector* inspector();
    virtual void clearUndoRedoOperations();
    virtual bool shouldClose();
    virtual void setProhibitsMainFrameScrolling(bool);
    virtual void setTranparentBackground(const bool);
    virtual void setShouldApplyMacFontAscentHack(bool);
    virtual void windowAncestryDidChange();
    virtual void paintDocumentRectToContext(WebCore::IntRect rect, PlatformGraphicsContext *pgc);

    WebCore::Page* page();

    void onExpose(BalEventExpose);

    void onKeyDown(BalEventKey);
    void onKeyUp(BalEventKey);
    void onMouseMotion(BalEventMotion);
    void onMouseButtonDown(BalEventButton);
    void onMouseButtonUp(BalEventButton);
    void onScroll(BalEventScroll);
    void onResize(BalResizeEvent);
    void onQuit(BalQuitEvent);
    void onUserEvent(BalUserEvent);

    void paint();
    bool ensureBackingStore();
    void addToDirtyRegion(const WebCore::IntRect&);
    WebCore::IntRect dirtyRegion();
    void clearDirtyRegion();
    void scrollBackingStore(WebCore::FrameView*, int dx, int dy, const WebCore::IntRect& scrollViewRect, const WebCore::IntRect& clipRect);
    void updateBackingStore(WebCore::FrameView*);
    void deleteBackingStore();
    WebCore::IntRect frameRect();

    void closeWindow();
    void closeWindowSoon();
    void close();
    bool didClose() const { return m_didClose; }

    void selectionChanged();

    //HRESULT registerDragDrop();
    //HRESULT revokeDragDrop();

    // Convenient to be able to violate the rules of COM here for easy movement to the frame.
    WebFrame* topLevelFrame() const { return m_mainFrame; }

    const WebCore::String& userAgentForKURL(const WebCore::KURL& url);

    static bool canHandleRequest(const WebCore::ResourceRequest&);

    void setIsBeingDestroyed() { m_isBeingDestroyed = true; }
    bool isBeingDestroyed() const { return m_isBeingDestroyed; }

    const char* interpretKeyEvent(const WebCore::KeyboardEvent*);
    bool handleEditingKeyboardEvent(WebCore::KeyboardEvent*);

    bool isPainting() const { return m_paintCount > 0; }

    void setToolTip(const WebCore::String, WebCore::IntPoint, const int handleID);    // 6/10/10 Chris Sidhall - Added tooltip location for client callback.
    void ResetToolTip();

#if ENABLE(ICON_DATABASE)
    void registerForIconNotification(bool listen);
    void dispatchDidReceiveIconFromWebFrame(WebFrame*);
    void notifyDidAddIcon();
#endif

    void notifyPreferencesChanged(WebPreferences*);

    static void setCacheModel(WebCacheModel);
    static WebCacheModel cacheModel();
    static bool didSetCacheModel();
    static WebCacheModel maxCacheModelInAnyInstance();

    void updateActiveStateSoon() const;
    void deleteBackingStoreSoon();
    void cancelDeleteBackingStoreSoon();

    BalWidget* topLevelParent() const { return m_topLevelParent; }

    void updateActiveState();

    void parseConfigFile(WebCore::String url = "");

    //FIXME EventLoop
    //bool onGetObject(WPARAM, LPARAM, LRESULT&) const;

    //static STDMETHODIMP AccessibleObjectFromWindow(HWND, DWORD objectID, REFIID, void** ppObject);
    
    virtual bool canGoBack(unsigned count);        // + 4/1/10 CSidhall- Added so can know before even trying.
    virtual bool canGoForward(unsigned count);     // + 

private:
    void setZoomMultiplier(float multiplier, bool isTextOnly);
    float zoomMultiplier(bool isTextOnly);
    bool canZoomIn(bool isTextOnly);
    void zoomIn(bool isTextOnly);
    bool canZoomOut(bool isTextOnly);
    void zoomOut(bool isTextOnly);
    bool canResetZoom(bool isTextOnly);
    void resetZoom(bool isTextOnly);

    bool active();

protected:
    //HIMC getIMMContext();
    //void releaseIMMContext(HIMC);

    static bool allowSiteSpecificHacks() { return s_allowSiteSpecificHacks; } 

    bool continuousCheckingAllowed();
    void initializeToolTipWindow();
    void closeWindowTimerFired(WebCore::Timer<WebView>*);

    //void prepareCandidateWindow(WebCore::Frame*, HIMC);
    //void updateSelectionForIME();
    //bool onIMERequestCharPosition(WebCore::Frame*, IMECHARPOSITION*, LRESULT*);
    //bool onIMERequestReconvertString(WebCore::Frame*, RECONVERTSTRING*, LRESULT*);

    bool developerExtrasEnabled() const;

    // AllWebViewSet functions
    void addToAllWebViewsSet();
    void removeFromAllWebViewsSet();

    //virtual void windowReceivedMessage(HWND, UINT message, WPARAM, LPARAM);

protected:
    BalWidget *m_viewWindow;
    WebFrame* m_mainFrame;
    WebCore::Page* m_page;
    
    OwnPtr<WebCore::Image> m_backingStoreBitmap;
    WebCore::IntPoint m_backingStoreSize;
    WebCore::IntRect m_backingStoreDirtyRegion;

    DefaultPolicyDelegate* m_policyDelegate;
    DefaultDownloadDelegate* m_downloadDelegate;
    WebPreferences* m_preferences;
    WebInspector* m_webInspector;

    bool m_userAgentOverridden;
    bool m_useBackForwardList;
    WebCore::String m_userAgentCustom;
    WebCore::String m_userAgentStandard;
    float m_zoomMultiplier;
    bool m_zoomMultiplierIsTextOnly;
    WebCore::String m_overrideEncoding;
    WebCore::String m_applicationName;
    bool m_mouseActivated;
    // WebCore dragging logic needs to be able to inspect the drag data
    // this is updated in DragEnter/Leave/Drop
    //COMPtr<IDataObject> m_dragData;
    //COMPtr<IDropTargetHelper> m_dropTargetHelper;
    //UChar m_currentCharacterCode;
    bool m_isBeingDestroyed;
    unsigned m_paintCount;
    bool m_hasSpellCheckerDocumentTag;
    bool m_smartInsertDeleteEnabled;
    bool m_didClose;
    bool m_hasCustomDropTarget;
    unsigned m_inIMEComposition;
    BalWidget* m_toolTipHwnd;
    WebCore::String m_toolTip;
    bool m_deleteBackingStoreTimerActive;

    static bool s_allowSiteSpecificHacks;

    WebCore::Timer<WebView> m_closeWindowTimer;
    //OwnPtr<TRACKMOUSEEVENT> m_mouseOutTracker;

    BalWidget* m_topLevelParent;
    WebViewPrivate *d;

    WKAL::TextPopup m_toolTipPopup;

    // 4/9/09 CSidhall - Added for leak fix
    static HashMap<int, const char*>* s_keyDownCommandsMap;
    static HashMap<int, const char*>* s_keyPressCommandsMap;
};

#endif
