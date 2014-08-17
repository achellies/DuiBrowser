#ifndef _WebView_H
#define _WebView_H

#include "PlatformString.h"
#include "IntRect.h"
#include "KURL.h"
#include "wtf/Vector.h"
#include "wtf/OwnPtr.h"

#include <cairo.h>
#include <cairo-win32.h>

#ifdef _WIN32_WCE
#define MAX_VIEM_NUM		4
#else
#define MAX_VIEM_NUM		16
#endif

namespace WebCore {
class Frame;
class Page;
class FrameView;
class ResourceRequest;
class HTMLFrameOwnerElement;
class KeyboardEvent;
class ResourceError;
}

class WebView
{
public:
	WebView();
	~WebView();

	static HWND getViewWindow() { return m_viewWindow; }
	static HWND initViewWindow();

public:
	LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

	void activeView(bool = true);

	WebCore::Frame* frame() { return m_frame; }
	WebCore::Page* page() { return m_page; }
	WebCore::FrameView *view();

	LPCWSTR getTitle(int &nLen);
	LPCWSTR getTitle() { int nLen; return getTitle(nLen); }
	LPCWSTR getUrl(int &nLen);

	bool initWithFrame(LPCWSTR, LPCWSTR, bool = true);
	bool initWithFrame(const WebCore::String &, const WebCore::String &, bool = true);
	
	void load(LPCWSTR url);
	void load(const WebCore::String &url);
	void load(const WebCore::ResourceRequest &request);
	void reload();
	void stop();

	void goForward();
	void goBack();

	bool historyBegin();
	bool historyEnd();

	void addToDirtyRegion(const WebCore::IntRect&);
	void scrollBackingStore(WebCore::FrameView*, int dx, int dy, const WebCore::IntRect& scrollViewRect, const WebCore::IntRect& clipRect);
	void updateBackingStore(WebCore::FrameView*, HDC, bool backingStoreCompletelyDirty);
	void frameRect(RECT* rect);

	PassRefPtr<WebCore::Frame> createFrame(const WebCore::KURL&, const WebCore::String&, WebCore::HTMLFrameOwnerElement*, const WebCore::String&, bool, int, int);

	void didReceiveTitle(const WebCore::String title, bool setTab = true);
	void dispatchDidCommitLoad();

	// For EditorClientISee
	bool handleEditingKeyboardEvent(WebCore::KeyboardEvent*);

	void close();
    bool didClose() const { return m_didClose; }

	bool onIMEStartComposition();
	bool onIMEComposition(LPARAM);
	bool onIMEEndComposition();
	bool onIMEChar(WPARAM, LPARAM);
	bool onIMENotify(WPARAM, LPARAM, LRESULT*);
	bool onIMERequest(WPARAM, LPARAM, LRESULT*);
	bool onIMESelect(WPARAM, LPARAM);
	bool onIMESetContext(WPARAM, LPARAM);
	void selectionChanged();
	void resetIME(WebCore::Frame*);
	bool inIMEKeyDown() const { return m_inIMEKeyDown; }
	void setInputMethodState(bool);

	// JavaScript Dialog
	void runJavaScriptAlert(const WebCore::String& message);
	bool runJavaScriptConfirm(const WebCore::String& message);
	bool runJavaScriptPrompt(const WebCore::String& message, const WebCore::String& defaultValue, WebCore::String& result);

	HWND topLevelParent() const { return m_topLevelParent; }

	void updateActiveState();

	// 缩放处理
	bool setTextSizeMultiplier(float multiplier);
	bool setPageSizeMultiplier(float multiplier);
	void setZoomMultiplier(float multiplier, bool isTextOnly);
	void textSizeMultiplier(float* multiplier);
	void pageSizeMultiplier(float* multiplier);
	float zoomMultiplier(bool isTextOnly);

	bool canMakeTextLarger();
	bool canZoomPageIn();
	bool canZoomIn(bool isTextOnly);
	bool makeTextLarger();
	bool zoomPageIn();
	bool zoomIn(bool isTextOnly);
	bool canMakeTextSmaller();
	bool canZoomPageOut();
	bool canZoomOut(bool isTextOnly);
	bool makeTextSmaller();
	bool zoomPageOut();
	bool zoomOut(bool isTextOnly);
	bool canMakeTextStandardSize();
	bool canResetPageZoom();
	bool canResetZoom(bool isTextOnly);
	bool makeTextStandardSize();
	bool resetPageZoom();
	bool resetZoom(bool isTextOnly);
	
private:
#if 0 // 目前采用单Window句柄实现多TAB，暂时不需要采用这个策略
	void setIsBeingDestroyed() { m_isBeingDestroyed = true; }
	bool isBeingDestroyed() const { return m_isBeingDestroyed; }
#endif

	bool inResizer(LPARAM lParam);
	void setMouseActivated(bool flag) { m_mouseActivated = flag; }
	bool handleMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
	bool mouseWheel(WPARAM wParam, LPARAM lParam, bool isHorizontal);
	bool execCommand(WPARAM wParam, LPARAM lParam);
	bool keyDown(WPARAM virtualKeyCode, LPARAM keyData, bool systemKeyDown = false);
	bool keyUp(WPARAM virtualKeyCode, LPARAM keyData, bool systemKeyDown = false);
	bool keyPress(WPARAM charCode, LPARAM keyData, bool systemKeyDown = false);
	const char* interpretKeyEvent(const WebCore::KeyboardEvent*);

	// 窗口是否激活
	bool active();

	// 设置配置
	void setPageSettings();

	// 能否画页面？
	bool isPainting() const { return m_paintCount > 0; }

	// 绘图函数
	void paint(HDC, LPARAM);
	void paintIntoBackingStore(WebCore::FrameView*, HDC, LPRECT);
	void paintIntoWindow(HDC, HDC, LPRECT);
	bool ensureBackingStore();
	void deleteBackingStore();

	// SubFrame
	void loadURLIntoChild(const WebCore::KURL&, const WebCore::String&, WebCore::Frame*);

	// IME
	HIMC getIMMContext();
	void releaseIMMContext(HIMC);
	void prepareCandidateWindow(WebCore::Frame*, HIMC);
	void updateSelectionForIME();
	bool onIMERequestCharPosition(WebCore::Frame*, IMECHARPOSITION*, LRESULT*);
	bool onIMERequestReconvertString(WebCore::Frame*, RECONVERTSTRING*, LRESULT*);

	// Window & paint
	void windowAncestryDidChange();
	void updateFocusedAndActiveState();
	void updateActiveStateSoon() const;
    void deleteBackingStoreSoon();
    void cancelDeleteBackingStoreSoon();

	// 打开开发者选项
	bool developerExtrasEnabled() const;

	// 视图焦点管理
	void focusView();
	void unfocusView();

	// 设置地址栏URL
	void setAddressBarUrl(const WebCore::String& url);

private:
	static HWND m_viewWindow;

	WebCore::Frame* m_frame;
	WebCore::Page* m_page;
	WebCore::String m_frameName;
	WebCore::String m_groupName;

#if 0 // 目前采用单Window句柄实现多TAB，暂时不需要采用这个策略
	bool	m_isBeingDestroyed;
#endif
	POINT	m_lastMousePos;
	bool	m_mouseActivated;
	UChar	m_currentCharacterCode;

	cairo_surface_t* m_backingStoreSurface;
	SIZE m_backingStoreSize;
	RECT m_windowRect;
	unsigned int m_paintCount;

	WebCore::String m_title;

	float m_zoomMultiplier;
    bool m_zoomMultiplierIsTextOnly;

	bool m_didClose;
	unsigned m_inIMEComposition;
	bool m_inIMEKeyDown;

	HWND m_topLevelParent;

	bool m_deleteBackingStoreTimerActive;

#ifndef _WIN32_WCE
	OwnPtr<TRACKMOUSEEVENT> m_mouseOutTracker;
#endif

private:
	// 为部分刷新准备
	struct BackingStoreDirty
	{
		BackingStoreDirty(const WebCore::IntRect &dirtyRect) : rcDirty(dirtyRect) {}
		BackingStoreDirty *next;
		WebCore::IntRect rcDirty;
	};
	BackingStoreDirty *m_freeDirtyStructs;
	BackingStoreDirty *m_backingStoreDirty;

	BackingStoreDirty *newDirtyStruct(const WebCore::IntRect &dirtyRect);
	void deleteDirtyStruct(BackingStoreDirty *dirtyRect);
	void uniteDirtyRect(const WebCore::IntRect &dirtyRect);
	void clearDirtyStructs();
	void deleteAllDirtyStructs();
	int getDirtyRect(LPRECT rcDirty);

// 页面列表处理
public:
	static WebView* createWebViewWithRequest(bool, const WebCore::ResourceRequest&);
	static WebView *newWebView(LPCWSTR, LPCWSTR, LPCWSTR);
	static WebView *newWebView(const WebCore::ResourceRequest&, const WebCore::String&, const WebCore::String&);
	static bool deleteWebView(int i);
	static bool deleteWebView(WebView *wv);
	static WebView *currentView();
	static void deleteAll();
	static int countViews();
	static void setViewFocus();

// 页面错误处理
public:
	static void setErrorStrings(LPCWSTR szTitle, LPCWSTR szDisp);
	void didFailLoadingWithError(unsigned long identifier, const WebCore::ResourceError& error);

private:
	static WebCore::String m_errorTitle;
	static WebCore::String m_errorDisplay;

// Enable javascript window.open ??
public:
	static void enableWindowOpen(bool b);

private:
	static bool m_enableWindowOpen;
};

#endif // _WebView_H
