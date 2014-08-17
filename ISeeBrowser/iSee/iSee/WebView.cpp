#include "config.h"

#include <Windows.h>
#include <WinSock2.h>
#include <tchar.h>

#include "CString.h"
#include "Document.h"
#include "DocumentLoader.h"
#include "DragClient.h"
#include "EditorClient.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "FrameView.h"
#include "FrameTree.h"
#include "Page.h"
#include "KURL.h"
#include "Logging.h"
#include "EventHandler.h"
#include "EventNames.h"
#include "FocusController.h"
#include "GraphicsContext.h"
#include "HitTestRequest.h"
#include "HitTestResult.h"
#include "KeyboardEvent.h"
#include "PlatformKeyboardEvent.h"
#include "PlatformMouseEvent.h"
#include "PlatformWheelEvent.h"
#include "RenderObject.h"
#include "RenderTreeAsText.h"
#include "ResourceHandle.h"
#include "ResourceResponse.h"
#include "ResourceHandleManager.h"
#include "SelectionController.h"
#include "Settings.h"
#include "TypingCommand.h"
#include "Editor.h"
#include "ResourceHandleClient.h"
#include "HistoryItem.h"
#include "HTMLFormElement.h"
#include "HTMLInputElement.h"
#include "HTMLNames.h"
#include "HTMLFrameElement.h"
#include "BackForwardList.h"
#include "HistoryItem.h"
#include "IntSize.h"
#include "ScrollBar.h"
#include "ResourceError.h"
#include "ResourceHandleManager.h"

#include "ChromeClientISee.h"
#include "ContextMenuClientISee.h"
#include "DragClientISee.h"
#include "InspectorClientISee.h"
#include "EditorClientISee.h"
#include "FrameLoaderClientISee.h"

#if SVG_SUPPORT
#include "SVGNames.h"
#include "XLinkNames.h"
#include "SVGDocumentExtensions.h"
#endif

#include "WebView.h"
#include "iSeeDefs.h"

#include "browser.h"

#define USE_COMPATIBLE	1
#define TEST_DRAW		1

#if TEST_DRAW
static HBITMAP gTestBitmap = NULL;
static HDC gTestHdc = NULL;
static int gLastW = 0, gLastH = 0;
static void CreateTestBitmap()
{
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth         = 800; 
	bitmapInfo.bmiHeader.biHeight        = -640;
	bitmapInfo.bmiHeader.biPlanes        = 1;
	bitmapInfo.bmiHeader.biBitCount      = 32;
	bitmapInfo.bmiHeader.biCompression   = BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage     = 0;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapInfo.bmiHeader.biClrUsed       = 0;
	bitmapInfo.bmiHeader.biClrImportant  = 0;

	void* pixels = NULL;
	gTestBitmap = ::CreateDIBSection(NULL, &bitmapInfo, DIB_RGB_COLORS, &pixels, NULL, 0);

	HDC hdc = GetDC(0);
	gTestHdc = CreateCompatibleDC(hdc);
	ReleaseDC(0, hdc);
	SelectObject(gTestHdc, gTestBitmap);
}

void DrawToTestDc(HDC hdc, int x, int y, int w, int h)
{
	RECT rcWhite;
	SetRect(&rcWhite, 0, 0, w, h);
	FillRect(gTestHdc, &rcWhite, (HBRUSH)GetStockObject(GRAY_BRUSH));
	BitBlt(gTestHdc, 0, 0, w, h, hdc,
		x, y, SRCCOPY);
#if 0
	RECT widgetRect;
	SetRect(&widgetRect, 0, h, w, h + 20);
	DrawFrameControl(hdc, &widgetRect, DFC_BUTTON, 4112);
#endif
	gLastW = w;
	gLastH = h;
}
#endif

// This function loads resources from WebKit
// This does not belong here and I'm not sure where
// it should go
// I don't know what the plans or design is
// for none code resources
Vector<char> loadResourceIntoArray(const char* resourceName)
{
	Vector<char> resource;
	//if (strcmp(resourceName,"missingImage") == 0) {
	//}
	return resource;
}

// 打开WebCore/HTMLNames命名空间
using namespace WebCore;
using namespace WebCore::EventNames;
using namespace HTMLNames;
using namespace WebKit;

WebView* kit(Page* page)
{
	return page ? static_cast<WebKit::ChromeClient *>(page->chrome()->client())->webView() : 0;
}

FrameView *WebView::view()
{
	return m_frame ? m_frame->view() : 0;
}

// 浏览器显示窗口
const LPCWSTR kWebViewWindowClassName = L"WebViewWindowClass";
HWND WebView::m_viewWindow = NULL;

// 删除后台显示延时
static const int delayBeforeDeletingBackingStoreMsec = 5000;

// 本地函数声明
static ATOM registerWebViewWindowClass();
static LRESULT CALLBACK WebViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static String autocorrectURL(const String& url);

enum {
	UpdateActiveStateTimer = 1,
	DeleteBackingStoreTimer = 2,
};

enum WebViewCmd { Cut = 100, Copy, Paste, ForwardDelete, SelectAll, Undo, Redo };

// WebView类实现
WebView::WebView()
: m_frame(0)
, m_page(0)
#if 0 // 目前采用单Window句柄实现多TAB，暂时不需要采用这个策略
, m_isBeingDestroyed(false)
#endif
, m_mouseActivated(false)
, m_currentCharacterCode(0)
, m_backingStoreSurface(0)
, m_paintCount(0)
, m_didClose(false)
, m_inIMEComposition(0)
, m_freeDirtyStructs(0)
, m_backingStoreDirty(0)
, m_topLevelParent(0)
, m_deleteBackingStoreTimerActive(false)
{
	// KJS::initializeThreading(); // Moved to InitBrowser

	// 私有变量设置
	m_lastMousePos.x = m_lastMousePos.y = -1;
	m_backingStoreSize.cx = m_backingStoreSize.cy = 0;
#ifdef TEST_DRAW
	// 创建一个图片区域
	CreateTestBitmap();
#endif
}

WebView::~WebView()
{
	close();
}

void WebView::close()
{
	if (m_didClose)
        return;
    m_didClose = true;

	if (m_frame)
		m_frame->loader()->detachFromParent();
#ifndef _WIN32_WCE
	if (m_mouseOutTracker) {
		m_mouseOutTracker->dwFlags = TME_CANCEL;
		::TrackMouseEvent(m_mouseOutTracker.get());
		m_mouseOutTracker.set(0);
	}
#endif
	delete m_page, m_page = 0;
	// 清理更新内存
	deleteAllDirtyStructs();
	// 清理显示缓冲
	deleteBackingStore();

	// 取消与Windows的关联
	if (WebView *view = (WebView *)
#ifndef _WIN32_WCE
		GetWindowLongPtr(m_viewWindow, 0)
#else
		GetWindowLong(m_viewWindow, 0)
#endif
	)
	{
		if (view == this)
		{
#ifndef _WIN32_WCE
			SetWindowLongPtr(m_viewWindow, 0, (LONG_PTR)0);
#else
			SetWindowLong(m_viewWindow, 0, (LONG)0);
#endif
		}
		view->deleteBackingStore();
	}
}

void WebView::deleteBackingStore()
{
	if (m_deleteBackingStoreTimerActive) {
        KillTimer(m_viewWindow, DeleteBackingStoreTimer);
        m_deleteBackingStoreTimerActive = false;
    }

	if (m_backingStoreSurface)
	{
		cairo_surface_destroy(m_backingStoreSurface);
		m_backingStoreSurface = 0;
	}

	clearDirtyStructs();

	m_backingStoreSize.cx = m_backingStoreSize.cy = 0;
}

bool WebView::ensureBackingStore()
{
	LONG width = m_windowRect.right - m_windowRect.left;
	LONG height = m_windowRect.bottom - m_windowRect.top;
	if (width > 0 && height > 0 && (width != m_backingStoreSize.cx || height != m_backingStoreSize.cy))
	{
		deleteBackingStore();
		m_backingStoreSize.cx = width;
		m_backingStoreSize.cy = height;
		
		if (cairo_win32_bpp() == 16)
			m_backingStoreSurface = cairo_win32_surface_create_with_dib(CAIRO_FORMAT_RGB16_565, width, height);
		else
			m_backingStoreSurface = cairo_win32_surface_create_with_dib(CAIRO_FORMAT_RGB24, width, height);
		
		return true;
	}

	return false;
}

WebView::BackingStoreDirty *WebView::newDirtyStruct(const IntRect &dirtyRect)
{
	if (m_freeDirtyStructs)
	{
		BackingStoreDirty *bsd = m_freeDirtyStructs;
		m_freeDirtyStructs = bsd->next;
		bsd->rcDirty = dirtyRect;
		return bsd;
	}
	return new BackingStoreDirty(dirtyRect);
}

void WebView::deleteDirtyStruct(BackingStoreDirty *dirtyRect)
{
	dirtyRect->next = m_freeDirtyStructs;
	m_freeDirtyStructs = dirtyRect;
}

void WebView::uniteDirtyRect(const IntRect &dirtyRect)
{
	if (dirtyRect.isEmpty())
		return;

	BackingStoreDirty *prev = 0, *unite = 0;
	for (BackingStoreDirty *bsd = m_backingStoreDirty; bsd; /* 内部赋值 */)
	{
		if (bsd->rcDirty.intersects(dirtyRect))
		{	// 有交集
			if (unite == 0)
			{
				unite = bsd;
				unite->rcDirty.unite(dirtyRect);
				prev = bsd; // 设置前向句柄
				bsd = bsd->next; // 取下一个
			}
			else
			{	// 已经和其他的更新进行了联合，需要删除这个更新
				unite->rcDirty.unite(bsd->rcDirty);
				if (prev)
					prev->next = bsd->next;
				else
					m_backingStoreDirty = bsd->next;
				BackingStoreDirty *tmp = bsd;
				bsd = bsd->next; // 取下一个
				deleteDirtyStruct(tmp);
			}
		}
		else
		{	// 无交集
			prev = bsd; // 设置前向句柄
			bsd = bsd->next; // 取下一个
		}
	}
	if (unite == 0)
	{
		BackingStoreDirty *newBsd = newDirtyStruct(dirtyRect);
		if (newBsd)
		{
			newBsd->next = m_backingStoreDirty;
			m_backingStoreDirty = newBsd;
		}
	}
}

// 清除当前的更新
void WebView::clearDirtyStructs()
{
	BackingStoreDirty *bsd = m_backingStoreDirty;
	while (bsd)
	{
		BackingStoreDirty *tmp = bsd;
		bsd = bsd->next;
		deleteDirtyStruct(tmp);
	}
	m_backingStoreDirty = 0;
}

// 物理删除更新结构列表
void WebView::deleteAllDirtyStructs()
{
	BackingStoreDirty *bsd = m_backingStoreDirty;
	while (bsd)
	{
		BackingStoreDirty *tmp = bsd;
		bsd = bsd->next;
		delete tmp;
	}
	bsd = m_freeDirtyStructs;
	while (bsd)
	{
		BackingStoreDirty *tmp = bsd;
		bsd = bsd->next;
		delete tmp;
	}
	m_backingStoreDirty = 0;
	m_freeDirtyStructs = 0;
}

// 获取所有更新的最大区域
int WebView::getDirtyRect(LPRECT rcDirty)
{
	int nCount = 0;
	IntRect dirtyRect;
	for (BackingStoreDirty *bsd = m_backingStoreDirty; bsd; bsd = bsd->next, nCount++)
	{
		dirtyRect.unite(bsd->rcDirty);
	}
	SetRect(rcDirty, dirtyRect.x(), dirtyRect.y(), dirtyRect.right(), dirtyRect.bottom());
	return nCount;
}

void WebView::addToDirtyRegion(const IntRect& dirtyRect)
{
	uniteDirtyRect(dirtyRect);
}

void WebView::scrollBackingStore(FrameView* frameView, int dx, int dy, const IntRect& scrollViewRect, const IntRect& clipRect)
{
	// If there's no backing store we don't need to update it
	if (!m_backingStoreSurface)
	{
#if 0 // FIXME : 滚动本地界面
		if (m_uiDelegatePrivate)
			m_uiDelegatePrivate->webViewScrolled(this);
#endif
		return;
	}

	// Scroll the bitmap.
	RECT scrollRectWin(scrollViewRect);
	RECT clipRectWin(clipRect);
	RECT regionBox;
	cairo_win32_surface_scroll_dc(m_backingStoreSurface, dx, dy, &scrollRectWin, &clipRectWin, &regionBox);
	
	// Flush.
	GdiFlush();

	// Add the dirty region to the backing store's dirty region.
	addToDirtyRegion(regionBox);
	
#if 0 // FIXME : 滚动本地界面
	if (m_uiDelegatePrivate)
		m_uiDelegatePrivate->webViewScrolled(this);
#endif

	// Update the backing store.
	iSeeLogA("Before scrollBackingStore %u\n", frameView->contentsY());
	updateBackingStore(frameView, cairo_win32_surface_get_dc(m_backingStoreSurface), false);
	iSeeLogA("After scrollBackingStore\n");
}

void WebView::updateBackingStore(FrameView* frameView, HDC bitmapDC, bool backingStoreCompletelyDirty)
{
	if (m_backingStoreSurface && (m_backingStoreDirty || backingStoreCompletelyDirty))
	{
		// Do a layout first so that everything we render to the backing store is always current.
		frameView->layoutIfNeededRecursive();

		// This emulates the Mac smarts for painting rects intelligently.  This is
		// very important for us, since we double buffer based off dirty rects.
		bool useRegionBox = true;
		const int cRectThreshold = 10;
		const int cMinRectThreshold = 5;
		const float cWastedSpaceThreshold = 0.60f;
		RECT regionBox;
		if (!backingStoreCompletelyDirty)
		{
			int rgnCount = getDirtyRect(&regionBox);
			if (rgnCount <= cRectThreshold)
			{
				bool regionUpdate = false;
				if (rgnCount >= cMinRectThreshold)
				{
					double unionPixels = (regionBox.right - regionBox.left) * (regionBox.bottom - regionBox.top);
					double singlePixels = 0;

					for (BackingStoreDirty *bsd = m_backingStoreDirty; bsd; bsd = bsd->next)
						singlePixels += bsd->rcDirty.width() * bsd->rcDirty.height();
					double wastedSpace = 1.0 - (singlePixels / unionPixels);
					if (wastedSpace > cWastedSpaceThreshold)
						regionUpdate = true;
				}
				else
					regionUpdate = true;
				if (regionUpdate)
				{
					// Paint individual rects.
					useRegionBox = false;
					for (BackingStoreDirty *bsd = m_backingStoreDirty; bsd; bsd = bsd->next)
					{
						RECT dirtyRect = RECT(bsd->rcDirty);
						paintIntoBackingStore(frameView, bitmapDC, &dirtyRect);
					}
				}
			}
		}
		else
			regionBox = m_windowRect;

		if (useRegionBox)
			paintIntoBackingStore(frameView, bitmapDC, &regionBox);

		// 清除更新区域
		clearDirtyStructs();
	}

	GdiFlush();
}

void WebView::paint(HDC dc, LPARAM options)
{
	m_paintCount++;

	FrameView* frameView = view();
	::GetClientRect(m_viewWindow, &m_windowRect);

	RECT rcPaint;
	HDC hdc;
	HRGN region = 0;
	int regionType = NULLREGION;
	PAINTSTRUCT ps;
	if (!dc)
	{
		region = CreateRectRgn(0,0,0,0);
		regionType = GetUpdateRgn(m_viewWindow, region, false);
		hdc = BeginPaint(m_viewWindow, &ps);
		if (IsRectEmpty(&ps.rcPaint))
			rcPaint = m_windowRect;
		else
			rcPaint = ps.rcPaint;
	}
	else
	{
		hdc = dc;
		rcPaint = m_windowRect;
#ifndef _WIN32_WCE
		if (options & PRF_ERASEBKGND)
#endif
			::FillRect(hdc, &rcPaint, (HBRUSH)GetStockObject(WHITE_BRUSH));
	}

	// Update our backing store if needed.
	bool backingStoreCompletelyDirty = ensureBackingStore();
	HDC bitmapDC = cairo_win32_surface_get_dc(m_backingStoreSurface);
	updateBackingStore(frameView, bitmapDC, backingStoreCompletelyDirty);

	// Now we blit the updated backing store
	IntRect windowDirtyRect = rcPaint;

	// Apply the same heuristic for this update region too.
	bool useWindowDirtyRect = true;
	if (region && regionType == COMPLEXREGION)
	{
		const int cRectThreshold = 10;
		const int cMinRectThreshold = 5;
		const float cWastedSpaceThreshold = 0.60f;
		DWORD regionDataSize = GetRegionData(region, sizeof(RGNDATA), NULL);
		if (regionDataSize)
		{
			RGNDATA* regionData = (RGNDATA*)malloc(regionDataSize);
			if (regionData)
			{
				GetRegionData(region, regionDataSize, regionData);
				if (regionData->rdh.nCount <= cRectThreshold)
				{
					RECT* rect;
					unsigned i;
					bool regionUpdate = false;
					if (regionData->rdh.nCount >= cMinRectThreshold)
					{
						double unionPixels = windowDirtyRect.width() * windowDirtyRect.height();
						double singlePixels = 0;

						for (i = 0, rect = (RECT*)regionData->Buffer; i < regionData->rdh.nCount; i++, rect++)
							singlePixels += (rect->right - rect->left) * (rect->bottom - rect->top);
						double wastedSpace = 1.0 - (singlePixels / unionPixels);
						if (wastedSpace > cWastedSpaceThreshold)
							regionUpdate = true;
					}
					else
						regionUpdate = true;
					if (regionUpdate)
					{	// Paint individual rects.
						useWindowDirtyRect = false;
						for (i = 0, rect = (RECT*)regionData->Buffer; i < regionData->rdh.nCount; i++, rect++)
							paintIntoWindow(bitmapDC, hdc, rect);
					}
				}
				free(regionData);
			}
		}
	}

	if (region)
		::DeleteObject(region);

	if (useWindowDirtyRect)
		paintIntoWindow(bitmapDC, hdc, &rcPaint);

	// Paint the gripper.
#if 0 // FIXME : 参见win/WebView.cpp，考虑为什这么实现
#endif

#ifdef TEST_DRAW
	BitBlt(hdc, 300, 0, gLastW, gLastH, gTestHdc, 0, 0, SRCCOPY);
#endif

	if (!dc)
		EndPaint(m_viewWindow, &ps);

	m_paintCount--;

	if (active())
        cancelDeleteBackingStoreSoon();
    else
        deleteBackingStoreSoon();
}

static unsigned int gTickCount[3];
static unsigned int gStartTick[3];
extern "C"
{
	void resetTickCount()
	{
		gTickCount[0] = 0;
		gTickCount[1] = 0;
		gTickCount[2] = 0;
	}
	
	void addTickCount(int w, int s)
	{
		if (s)
		{
			gStartTick[w] = GetTickCount();
		}
		else
		{
			gTickCount[w] += GetTickCount() - gStartTick[w];
		}
	}
}

void WebView::paintIntoBackingStore(FrameView* frameView, HDC bitmapDC, LPRECT dirtyRect)
{
#if 0
	HDC dc = ::GetDC(m_viewWindow);
	HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
	FillRect(dc, dirtyRect, yellowBrush);
	DeleteObject(yellowBrush);
	GdiFlush();
	Sleep(50);
	paintIntoWindow(bitmapDC, dc, dirtyRect);
	::ReleaseDC(m_viewWindow, dc);
#endif

	// FIXME：多画一个像素，避免交界的地方有问题
	if (dirtyRect->left < frameView->visibleWidth())
		dirtyRect->left--;
	if (dirtyRect->top < frameView->visibleHeight())
		dirtyRect->top--;
	
	// 设置本次绘制的裁剪区域
	RECT rcClip;
	IntersectRect(&rcClip, &m_windowRect, dirtyRect);
	if (IsRectEmpty(&rcClip))
		return;
	HRGN hrgn = CreateRectRgn(rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);
	if (hrgn)
	{
		SelectClipRgn(bitmapDC, hrgn);
		::DeleteObject(hrgn);
	}

	// 绘制刷新区域
	if (frameView && frameView->frame() && frameView->frame()->contentRenderer())
	{
		cairo_t *cr = cairo_create(m_backingStoreSurface);
		GraphicsContext gc(cr);
		gc.clip(IntRect(rcClip));
		resetTickCount();
		iSeeLogA("Before paint %d, %d, %d, %d\n", rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);
		frameView->paint(&gc, IntRect(rcClip));
		iSeeLogA("After paint %d, %d, %d\n", gTickCount[0], gTickCount[1], gTickCount[2]);
		cairo_destroy(cr);
	}
	else
		FillRect(bitmapDC, &rcClip, (HBRUSH)GetStockObject(WHITE_BRUSH));
}

void WebView::paintIntoWindow(HDC bitmapDC, HDC windowDC, LPRECT dirtyRect)
{
#if 0
	HBRUSH greenBrush = CreateSolidBrush(RGB(0, 255, 0));
	FillRect(windowDC, dirtyRect, greenBrush);
	DeleteObject(greenBrush);
	GdiFlush();
	Sleep(50);
#endif

	iSeeLogA("paintIntoWindow %u, %u, %u, %u\n", dirtyRect->left, dirtyRect->top, dirtyRect->right, dirtyRect->bottom);

	// Blit the dirty rect from the backing store into the same position
	// in the destination DC.
	BitBlt(windowDC, dirtyRect->left, dirtyRect->top, dirtyRect->right - dirtyRect->left, dirtyRect->bottom - dirtyRect->top, 
		bitmapDC, dirtyRect->left, dirtyRect->top, SRCCOPY);
}

void WebView::frameRect(RECT* rect)
{
	::GetWindowRect(m_viewWindow, rect);
}

/* FIXME：取消了菜单相关的处理
*/

bool WebView::handleMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	static LONG globalClickCount;
	static IntPoint globalPrevPoint;
	static MouseButton globalPrevButton;
	static LONG globalPrevMouseDownTime;

	// Create our event.
	// On WM_MOUSELEAVE we need to create a mouseout event, so we force the position
	// of the event to be at (MINSHORT, MINSHORT).
#ifdef _WIN32_WCE
	LPARAM position = lParam;
#else
	LPARAM position = (message == WM_MOUSELEAVE) ? ((MINSHORT << 16) | MINSHORT) : lParam;
#endif
	PlatformMouseEvent mouseEvent(m_viewWindow, message, wParam, position, m_mouseActivated);

	bool insideThreshold = abs(globalPrevPoint.x() - mouseEvent.pos().x()) < ::GetSystemMetrics(SM_CXDOUBLECLK) &&
	                       abs(globalPrevPoint.y() - mouseEvent.pos().y()) < ::GetSystemMetrics(SM_CYDOUBLECLK);
	LONG messageTime = ::GetMessageTime();

	bool handled = false;
	if (message == WM_LBUTTONDOWN || message == WM_MBUTTONDOWN || message == WM_RBUTTONDOWN)
	{
		// FIXME: I'm not sure if this is the "right" way to do this
		// but without this call, we never become focused since we don't allow
		// the default handling of mouse events.
		SetFocus(m_viewWindow);

		// WINCE下需要先发一次mousemove事件
#ifdef _WIN32_WCE
		{
			PlatformMouseEvent moveEvent(m_viewWindow, WM_MOUSEMOVE, 0, position, false);
			moveEvent.setClickCount(0);
			m_page->mainFrame()->eventHandler()->handleMouseMoveEvent(moveEvent);
		}
#endif

		// Always start capturing events when the mouse goes down in our HWND.
		::SetCapture(m_viewWindow);

		if (((messageTime - globalPrevMouseDownTime) < (LONG)::GetDoubleClickTime()) && 
			insideThreshold &&
			mouseEvent.button() == globalPrevButton)
			globalClickCount++;
		else
			// Reset the click count.
			globalClickCount = 1;
		globalPrevMouseDownTime = messageTime;
		globalPrevButton = mouseEvent.button();
		globalPrevPoint = mouseEvent.pos();

		mouseEvent.setClickCount(globalClickCount);
		handled = m_page->mainFrame()->eventHandler()->handleMousePressEvent(mouseEvent);
	}
	else if (message == WM_LBUTTONDBLCLK || message == WM_MBUTTONDBLCLK || message == WM_RBUTTONDBLCLK)
	{
		globalClickCount++;
		mouseEvent.setClickCount(globalClickCount);
		handled = m_page->mainFrame()->eventHandler()->handleMousePressEvent(mouseEvent);
	}
	else if (message == WM_LBUTTONUP || message == WM_MBUTTONUP || message == WM_RBUTTONUP)
	{
		// Record the global position and the button of the up.
		globalPrevButton = mouseEvent.button();
		globalPrevPoint = mouseEvent.pos();
		mouseEvent.setClickCount(globalClickCount);
		m_page->mainFrame()->eventHandler()->handleMouseReleaseEvent(mouseEvent);
		::ReleaseCapture();
	}
#ifndef _WIN32_WCE
	else if (message == WM_MOUSELEAVE && m_mouseOutTracker)
	{
		// Once WM_MOUSELEAVE is fired windows clears this tracker
        // so there is no need to disable it ourselves.
        m_mouseOutTracker.set(0);
        m_page->mainFrame()->eventHandler()->mouseMoved(mouseEvent);
        handled = true;
	}
#endif
	else if (message == WM_MOUSEMOVE)
	{
		if (!insideThreshold)
            globalClickCount = 0;
        mouseEvent.setClickCount(globalClickCount);
        handled = m_page->mainFrame()->eventHandler()->mouseMoved(mouseEvent);
#ifndef _WIN32_WCE
		if (!m_mouseOutTracker) {
            m_mouseOutTracker.set(new TRACKMOUSEEVENT);
            m_mouseOutTracker->cbSize = sizeof(TRACKMOUSEEVENT);
            m_mouseOutTracker->dwFlags = TME_LEAVE;
            m_mouseOutTracker->hwndTrack = m_viewWindow;
            ::TrackMouseEvent(m_mouseOutTracker.get());
        }
#endif
	}

	setMouseActivated(false);
	return handled;
}

bool WebView::mouseWheel(WPARAM wParam, LPARAM lParam, bool isHorizontal)
{
	// Ctrl+Mouse wheel doesn't ever go into WebCore.  It is used to
	// zoom instead (Mac zooms the whole Desktop, but Windows browsers trigger their
	// own local zoom modes for Ctrl+wheel).
	if (wParam & MK_CONTROL)
	{
		short delta = short(HIWORD(wParam));
		if (delta < 0)
			makeTextLarger();
		else
			makeTextSmaller();
		return true;
	}

	PlatformWheelEvent wheelEvent(m_viewWindow, wParam, lParam, isHorizontal);
	return frame()->eventHandler()->handleWheelEvent(wheelEvent);
}

bool WebView::execCommand(WPARAM wParam, LPARAM /*lParam*/)
{
    Frame* frame = m_page->focusController()->focusedOrMainFrame();
    switch (LOWORD(wParam)) {
    case SelectAll:
        return frame->editor()->command("SelectAll").execute();
    case Undo:
        return frame->editor()->command("Undo").execute();
    case Redo:
        return frame->editor()->command("Redo").execute();
    }
    return false;
}

bool WebView::keyUp(WPARAM virtualKeyCode, LPARAM keyData, bool systemKeyDown)
{
	PlatformKeyboardEvent keyEvent(m_viewWindow, virtualKeyCode, keyData, PlatformKeyboardEvent::KeyUp, systemKeyDown);

	Frame* frame = m_page->focusController()->focusedOrMainFrame();
	m_currentCharacterCode = 0;

	return frame->eventHandler()->keyEvent(keyEvent);
}

static const unsigned CtrlKey = 1 << 0;
static const unsigned AltKey = 1 << 1;
static const unsigned ShiftKey = 1 << 2;


struct KeyDownEntry {
	unsigned virtualKey;
	unsigned modifiers;
	const char* name;
};

struct KeyPressEntry {
	unsigned charCode;
	unsigned modifiers;
	const char* name;
};

static const KeyDownEntry keyDownEntries[] = {
	{ VK_LEFT,   0,                  "MoveLeft"                                    },
	{ VK_LEFT,   ShiftKey,           "MoveLeftAndModifySelection"                  },
	{ VK_LEFT,   CtrlKey,            "MoveWordLeft"                                },
	{ VK_LEFT,   CtrlKey | ShiftKey, "MoveWordLeftAndModifySelection"              },
	{ VK_RIGHT,  0,                  "MoveRight"                                   },
	{ VK_RIGHT,  ShiftKey,           "MoveRightAndModifySelection"                 },
	{ VK_RIGHT,  CtrlKey,            "MoveWordRight"                               },
	{ VK_RIGHT,  CtrlKey | ShiftKey, "MoveWordRightAndModifySelection"             },
	{ VK_UP,     0,                  "MoveUp"                                      },
	{ VK_UP,     ShiftKey,           "MoveUpAndModifySelection"                    },
	{ VK_PRIOR,  ShiftKey,           "MovePageUpAndModifySelection"                },
	{ VK_DOWN,   0,                  "MoveDown"                                    },
	{ VK_DOWN,   ShiftKey,           "MoveDownAndModifySelection"                  },
	{ VK_NEXT,   ShiftKey,           "MovePageDownAndModifySelection"              },
	{ VK_PRIOR,  0,                  "MovePageUp"                                  },
	{ VK_NEXT,   0,                  "MovePageDown"                                },
	{ VK_HOME,   0,                  "MoveToBeginningOfLine"                       },
	{ VK_HOME,   ShiftKey,           "MoveToBeginningOfLineAndModifySelection"     },
	{ VK_HOME,   CtrlKey,            "MoveToBeginningOfDocument"                   },
	{ VK_HOME,   CtrlKey | ShiftKey, "MoveToBeginningOfDocumentAndModifySelection" },

	{ VK_END,    0,                  "MoveToEndOfLine"                             },
	{ VK_END,    ShiftKey,           "MoveToEndOfLineAndModifySelection"           },
	{ VK_END,    CtrlKey,            "MoveToEndOfDocument"                         },
	{ VK_END,    CtrlKey | ShiftKey, "MoveToEndOfDocumentAndModifySelection"       },

	{ VK_BACK,   0,                  "DeleteBackward"                              },
	{ VK_BACK,   ShiftKey,           "DeleteBackward"                              },
	{ VK_DELETE, 0,                  "DeleteForward"                               },
	{ VK_BACK,   CtrlKey,            "DeleteWordBackward"                          },
	{ VK_DELETE, CtrlKey,            "DeleteWordForward"                           },

	{ 'B',       CtrlKey,            "ToggleBold"                                  },
	{ 'I',       CtrlKey,            "ToggleItalic"                                },

	{ VK_ESCAPE, 0,                  "Cancel"                                      },
#ifndef _WIN32_WCE
	{ VK_OEM_PERIOD, CtrlKey,        "Cancel"                                      },
#endif
	{ VK_TAB,    0,                  "InsertTab"                                   },
	{ VK_TAB,    ShiftKey,           "InsertBacktab"                               },
	{ VK_RETURN, 0,                  "InsertNewline"                               },
	{ VK_RETURN, CtrlKey,            "InsertNewline"                               },
	{ VK_RETURN, AltKey,             "InsertNewline"                               },
	{ VK_RETURN, AltKey | ShiftKey,  "InsertNewline"                               },

	// It's not quite clear whether clipboard shortcuts and Undo/Redo should be handled
	// in the application or in WebKit. We chose WebKit.
	{ 'C',       CtrlKey,            "Copy"                                        },
	{ 'V',       CtrlKey,            "Paste"                                       },
	{ 'X',       CtrlKey,            "Cut"                                         },
	{ 'A',       CtrlKey,            "SelectAll"                                   },
	{ VK_INSERT, CtrlKey,            "Copy"                                        },
	{ VK_DELETE, ShiftKey,           "Cut"                                         },
	{ VK_INSERT, ShiftKey,           "Paste"                                       },
	{ 'Z',       CtrlKey,            "Undo"                                        },
	{ 'Z',       CtrlKey | ShiftKey, "Redo"                                        },
};

static const KeyPressEntry keyPressEntries[] = {
	{ '\t',   0,                  "InsertTab"                                   },
	{ '\t',   ShiftKey,           "InsertBacktab"                               },
	{ '\r',   0,                  "InsertNewline"                               },
	{ '\r',   CtrlKey,            "InsertNewline"                               },
	{ '\r',   AltKey,             "InsertNewline"                               },
	{ '\r',   AltKey | ShiftKey,  "InsertNewline"                               },
};

const char* WebView::interpretKeyEvent(const KeyboardEvent* evt)
{
	ASSERT(evt->type() == keydownEvent || evt->type() == keypressEvent);

	static HashMap<int, const char*>* keyDownCommandsMap = 0;
	static HashMap<int, const char*>* keyPressCommandsMap = 0;

	if (!keyDownCommandsMap) {
		keyDownCommandsMap = new HashMap<int, const char*>;
		keyPressCommandsMap = new HashMap<int, const char*>;

		for (unsigned i = 0; i < _countof(keyDownEntries); i++)
			keyDownCommandsMap->set(keyDownEntries[i].modifiers << 16 | keyDownEntries[i].virtualKey, keyDownEntries[i].name);

		for (unsigned i = 0; i < _countof(keyPressEntries); i++)
			keyPressCommandsMap->set(keyPressEntries[i].modifiers << 16 | keyPressEntries[i].charCode, keyPressEntries[i].name);
	}

	unsigned modifiers = 0;
	if (evt->shiftKey())
		modifiers |= ShiftKey;
	if (evt->altKey())
		modifiers |= AltKey;
	if (evt->ctrlKey())
		modifiers |= CtrlKey;

	if (evt->type() == keydownEvent) {
		int mapKey = modifiers << 16 | evt->keyCode();
		return mapKey ? keyDownCommandsMap->get(mapKey) : 0;
	}

	int mapKey = modifiers << 16 | evt->charCode();
	return mapKey ? keyPressCommandsMap->get(mapKey) : 0;
}

bool WebView::handleEditingKeyboardEvent(KeyboardEvent* evt)
{
	Node* node = evt->target()->toNode();
	ASSERT(node);
	Frame* frame = node->document()->frame();
	ASSERT(frame);

	const PlatformKeyboardEvent* keyEvent = evt->keyEvent();
	if (!keyEvent || keyEvent->isSystemKey())  // do not treat this as text input if it's a system key event
		return false;

	Editor::Command command = frame->editor()->command(interpretKeyEvent(evt));

	if (keyEvent->type() == PlatformKeyboardEvent::RawKeyDown) {
		// WebKit doesn't have enough information about mode to decide how commands that just insert text if executed via Editor should be treated,
		// so we leave it upon WebCore to either handle them immediately (e.g. Tab that changes focus) or let a keypress event be generated
		// (e.g. Tab that inserts a Tab character, or Enter).
		return !command.isTextInsertion() && command.execute(evt);
	}

	if (command.execute(evt))
		return true;

	// Don't insert null or control characters as they can result in unexpected behaviour
	if (evt->charCode() < ' ')
		return false;

	return frame->editor()->insertText(evt->keyEvent()->text(), evt);
}

bool WebView::keyDown(WPARAM virtualKeyCode, LPARAM keyData, bool systemKeyDown)
{
	Frame* frame = m_page->focusController()->focusedOrMainFrame();
	if (virtualKeyCode == VK_CAPITAL)
		frame->eventHandler()->capsLockStateMayHaveChanged();

	PlatformKeyboardEvent keyEvent(m_viewWindow, virtualKeyCode, keyData, PlatformKeyboardEvent::RawKeyDown, systemKeyDown);
	bool handled = frame->eventHandler()->keyEvent(keyEvent);

	// These events cannot be canceled, and we have no default handling for them.
	// FIXME: match IE list more closely, see <http://msdn2.microsoft.com/en-us/library/ms536938.aspx>.
	if (systemKeyDown && virtualKeyCode != VK_RETURN)
		return false;

	if (handled) {
		// FIXME: remove WM_UNICHAR, too
		MSG msg;
		// WM_SYSCHAR events should not be removed, because access keys are implemented in WebCore in WM_SYSCHAR handler.
		if (!systemKeyDown)
			::PeekMessage(&msg, m_viewWindow, WM_CHAR, WM_CHAR, PM_REMOVE);
		return true;
	}

	// We need to handle back/forward using either Backspace(+Shift) or Ctrl+Left/Right Arrow keys.
	if ((virtualKeyCode == VK_BACK && keyEvent.shiftKey()) || (virtualKeyCode == VK_RIGHT && keyEvent.ctrlKey()))
		goForward();
	else if (virtualKeyCode == VK_BACK || (virtualKeyCode == VK_LEFT && keyEvent.ctrlKey()))
		goBack();

	if (IsFullScreen() && VK_RETURN == virtualKeyCode)
	{	// 恢复为标准窗口
		SendMessage(ghMainWnd, WM_FULL_SCREEN, FALSE, 0);
	}

	// Need to scroll the page if the arrow keys, space(shift), pgup/dn, or home/end are hit.
	ScrollDirection direction;
	ScrollGranularity granularity;
	switch (virtualKeyCode)
	{
	case VK_LEFT:
		granularity = ScrollByPage /* ScrollByLine */;
		direction = ScrollLeft;
		break;
	case VK_RIGHT:
		granularity = ScrollByPage /* ScrollByLine */;
		direction = ScrollRight;
		break;
	case VK_UP:
#ifdef _WIN32_WCE
		granularity = ScrollByPage;
#else
		granularity = ScrollByLine;
#endif
		direction = ScrollUp;
		break;
	case VK_DOWN:
#ifdef _WIN32_WCE
		granularity = ScrollByPage;
#else
		granularity = ScrollByLine;
#endif
		direction = ScrollDown;
		break;
	case VK_HOME:
		granularity = ScrollByDocument;
		direction = ScrollUp;
		break;
	case VK_END:
		granularity = ScrollByDocument;
		direction = ScrollDown;
		break;
	case VK_PRIOR:
		granularity = ScrollByPage;
		direction = ScrollUp;
		break;
	case VK_NEXT:
		granularity = ScrollByPage;
		direction = ScrollDown;
		break;
	default:
		return false;
	}

	iSeeLogA("Start Scroll\n");
	if (!frame->eventHandler()->scrollOverflow(direction, granularity))
	{
		handled = frame->view()->scroll(direction, granularity);
		Frame* parent = frame->tree()->parent();
		while(!handled && parent)
		{
			handled = parent->view()->scroll(direction, granularity);
			parent = parent->tree()->parent();
		}
		if (!handled && virtualKeyCode == VK_RIGHT && frame->view()->visibleWidth() < frame->view()->contentsWidth())
		{
			IntSize offset = frame->view()->scrollOffset();
			frame->view()->scrollBy(-offset.width(), 0);
		}
	}
	return handled;
}

// FIXME：目前仅能支持手写输入发，对于整句数据目前没有测试，可能需要修改
bool WebView::keyPress(WPARAM charCode, LPARAM keyData, bool systemKeyDown)
{
	Frame* frame = m_page->focusController()->focusedOrMainFrame();

	PlatformKeyboardEvent keyEvent(m_viewWindow, charCode, keyData, PlatformKeyboardEvent::Char, systemKeyDown);
	// IE does not dispatch keypress event for WM_SYSCHAR.
	if (systemKeyDown)
		return frame->eventHandler()->handleAccessKey(keyEvent);
	if (frame->eventHandler()->keyEvent(keyEvent))
		return true;

	// Need to scroll the page if space is hit.
	if (charCode == ' ') {
		ScrollDirection direction = keyEvent.shiftKey() ? ScrollUp : ScrollDown;
		if (!frame->eventHandler()->scrollOverflow(direction, ScrollByPage))
			frame->view()->scroll(direction, ScrollByPage);
		return true;
	}
	return false;
}

bool WebView::inResizer(LPARAM lParam)
{
	// FIXME，此处暂时还没有处理
	return false;
}

// 浏览器显示窗口
static ATOM registerWebViewWindowClass()
{
	static bool haveRegisteredWindowClass = false;
	if (haveRegisteredWindowClass)
		return true;

	WNDCLASS wc;

	wc.style          = CS_DBLCLKS;
	wc.lpfnWndProc    = WebViewWndProc;
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = sizeof(void *); // 4 bytes for the Frame pointer
	wc.hInstance      = ghInstance;
	wc.hIcon          = 0;
	wc.hCursor        = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground  = 0;
	wc.lpszMenuName   = 0;
	wc.lpszClassName  = kWebViewWindowClassName;

	return RegisterClass(&wc);
}

namespace WebCore {
	extern HCURSOR lastSetCursor;
}

// Get the top level parent
static HWND findTopLevelParent(HWND window)
{
	if (!window)
		return 0;

	HWND current = window;
	for (HWND parent = GetParent(current); current; current = parent, parent = GetParent(parent))
#ifdef _WIN32_WCE
		if (!parent || !(GetWindowLong(current, GWL_STYLE) & (WS_POPUP | WS_CHILD)))
#else
		if (!parent || !(GetWindowLongPtr(current, GWL_STYLE) & (WS_POPUP | WS_CHILD)))
#endif
			return current;
	ASSERT_NOT_REACHED();
	return 0;
}

LRESULT WebView::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!view())
		return DefWindowProc(hWnd, message, wParam, lParam);

#if 0 // 目前采用单Window句柄实现多TAB，暂时不需要采用这个策略
	if(isBeingDestroyed())
		return DefWindowProc(hWnd, message, wParam, lParam);
#endif

#if 0
	// Windows Media Player has a modal message loop that will deliver messages
    // to us at inappropriate times and we will crash if we handle them when
    // they are delivered. We repost paint messages so that we eventually get
    // a chance to paint once the modal loop has exited, but other messages
    // aren't safe to repost, so we just drop them.
    if (PluginView::isCallingPlugin()) {
        if (message == WM_PAINT)
            PostMessage(hWnd, message, wParam, lParam);
        return 0;
    }
#endif

	LRESULT lResult = 0;
	bool handled = true;
	switch (message)
	{
	case WM_PAINT:
		paint(0, 0);
		break;
#ifndef _WIN32_WCE
	case WM_PRINTCLIENT:
		paint((HDC)wParam, lParam);
		break;
#endif

#if 0 // 目前采用单Window句柄实现多TAB，暂时不需要采用这个策略
	case WM_DESTROY:
		setIsBeingDestroyed();
		break;
#endif

	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
#ifndef _WIN32_WCE
	case WM_MOUSELEAVE:
#endif
		if (frame()->eventHandler() && view()->didFirstLayout())
			handled = handleMouseEvent(message, wParam, lParam);
		break;

	case WM_MOUSEWHEEL:
#if WINVER >= 0x0600
	case WM_VISTA_MOUSEHWHEEL:
#endif
		if (frame()->eventHandler() && view()->didFirstLayout())
			handled = mouseWheel(wParam, lParam, (wParam & MK_SHIFT)
#if WINVER >= 0x0600
			|| message == WM_VISTA_MOUSEHWHEEL
#endif
			);
		break;

	case WM_SYSKEYDOWN:
		handled = keyDown(wParam, lParam, true);
		break;
	case WM_KEYDOWN:
		handled = keyDown(wParam, lParam);
		break;
	case WM_SYSKEYUP:
		handled = keyUp(wParam, lParam, true);
		break;
	case WM_KEYUP:
		handled = keyUp(wParam, lParam);
		break;
	case WM_SYSCHAR:
		handled = keyPress(wParam, lParam, true);
		break;
	case WM_CHAR:
		handled = keyPress(wParam, lParam);
		break;

	case WM_SIZE:
#if 0 // 目前采用单Window句柄实现多TAB，暂时不需要采用这个策略
		if (isBeingDestroyed())
			// If someone has sent us this message while we're being destroyed, we should bail out so we don't crash.
			break;
#endif
		if (lParam != 0 && view())
		{
			int w = LOWORD(lParam), h = HIWORD(lParam);
			if (w != m_backingStoreSize.cx || h != m_backingStoreSize.cy)
			{
				deleteBackingStore();
				view()->resize(w, h);
			}
		}
		break;

	case WM_SHOWWINDOW:
		DefWindowProc(hWnd, message, wParam, lParam);
		if (wParam == 0)
			// The window is being hidden (e.g., because we switched tabs.
			// Null out our backing store.
			deleteBackingStore();
		break;

	case WM_SETFOCUS: {
			// FIXME 可能还需要通知本地主程序控制层

			// 通知WEBKIT控制层
			FocusController* focusController = m_page->focusController();
			if (Frame* frame = focusController->focusedFrame()) {
				// If the previously focused window is a child of ours (for example a plugin), don't send any
				// focus events.
				if (!IsChild(hWnd, reinterpret_cast<HWND>(wParam)))
					frame->selection()->setFocused(true);
			} else
				focusController->setFocusedFrame(m_page->mainFrame());
		}
		break;
	case WM_KILLFOCUS: {
			HWND newFocusWnd = reinterpret_cast<HWND>(wParam);
			// FIXME 可能还需要通知本地主程序控制层

			// 通知WEBKIT控制层
			FocusController* focusController = m_page->focusController();
			resetIME(focusController->focusedOrMainFrame());
			if (topLevelParent() != findTopLevelParent(newFocusWnd)) {
				if (Frame* frame = focusController->focusedOrMainFrame()) {
					// If we're losing focus to a child of ours, don't send blur events.
					if (!IsChild(hWnd, newFocusWnd))
						frame->selection()->setFocused(false);
				}
			} else
				focusController->setFocusedFrame(0);
		}
		break;

	case WM_WINDOWPOSCHANGED:
		if (reinterpret_cast<WINDOWPOS*>(lParam)->flags & SWP_SHOWWINDOW)
			updateActiveStateSoon();
		handled = false;
		break;

	case WM_CUT:
		m_page->focusController()->focusedOrMainFrame()->editor()->command("Cut").execute();
		break;
	case WM_COPY:
		m_page->focusController()->focusedOrMainFrame()->editor()->command("Copy").execute();
		break;
	case WM_PASTE:
		m_page->focusController()->focusedOrMainFrame()->editor()->command("Paste").execute();
		break;
	case WM_CLEAR:
		m_page->focusController()->focusedOrMainFrame()->editor()->command("Delete").execute();
		break;
	case WM_COMMAND:
        if (HIWORD(wParam))
            handled = execCommand(wParam, lParam);
#if 0 // FIXME：菜单处理
        else // If the high word of wParam is 0, the message is from a menu
            performContextMenuAction(wParam, lParam, false);
#endif
        break;

#if 0 // 菜单处理
	case WM_MENUCOMMAND:
		performContextMenuAction(wParam, lParam, true);
		break;
	case WM_CONTEXTMENU:
		handled = handleContextMenuEvent(wParam, lParam);
		break;
	case WM_INITMENUPOPUP:
		handled = onInitMenuPopup(wParam, lParam);
		break;
	case WM_MEASUREITEM:
		handled = onMeasureItem(wParam, lParam);
		break;
	case WM_DRAWITEM:
		handled = onDrawItem(wParam, lParam);
		break;
	case WM_UNINITMENUPOPUP:
		handled = onUninitMenuPopup(wParam, lParam);
		break;
	case WM_XP_THEMECHANGED:
		deleteBackingStore();
		view()->themeChanged();
		break;
	case WM_MOUSEACTIVATE:
		setMouseActivated(true);
		break;
	case WM_GETDLGCODE:
		{
			COMPtr<IWebUIDelegate> uiDelegate;
			COMPtr<IWebUIDelegatePrivate> uiDelegatePrivate;
			LONG_PTR dlgCode = 0;
			UINT keyCode = 0;
			if (lParam) {
				LPMSG lpMsg = (LPMSG)lParam;
				if (lpMsg->message == WM_KEYDOWN)
					keyCode = (UINT) lpMsg->wParam;
			}
			if (SUCCEEDED(webView->uiDelegate(&uiDelegate)) && uiDelegate &&
				SUCCEEDED(uiDelegate->QueryInterface(IID_IWebUIDelegatePrivate, (void**) &uiDelegatePrivate)) && uiDelegatePrivate &&
				SUCCEEDED(uiDelegatePrivate->webViewGetDlgCode(webView, keyCode, &dlgCode)))
				return dlgCode;
			handled = false;
			break;
		}
#endif

#if 0 // IME处理
	case WM_IME_STARTCOMPOSITION:
		handled = onIMEStartComposition();
		break;
	case WM_IME_REQUEST:
		onIMERequest(wParam, lParam, &lResult);
		break;
	case WM_IME_COMPOSITION:
		handled = onIMEComposition(lParam);
		break;
	case WM_IME_ENDCOMPOSITION:
		handled = onIMEEndComposition();
		break;
	case WM_IME_CHAR:
		handled = onIMEChar(wParam, lParam);
		break;
	case WM_IME_NOTIFY:
		handled = onIMENotify(wParam, lParam, &lResult);
		break;
	case WM_IME_SELECT:
		handled = onIMESelect(wParam, lParam);
		break;
	case WM_IME_SETCONTEXT:
		handled = onIMESetContext(wParam, lParam);
		break;
#endif

	case WM_TIMER:
		switch (wParam) {
		case UpdateActiveStateTimer:
			KillTimer(hWnd, UpdateActiveStateTimer);
			updateActiveState();
			break;
		case DeleteBackingStoreTimer:
            deleteBackingStore();
            break;
		}
		break;

	case WM_ACTIVATE:
		break;

	case WM_SETCURSOR:
		if (lastSetCursor)
		{
			SetCursor(lastSetCursor);
			break;
		}
		__fallthrough;
	default:
		handled = false;
		break;
	}

	if (!handled)
        lResult = DefWindowProc(hWnd, message, wParam, lParam);

	// Let the client know whether we consider this message handled.
    return (message == WM_KEYDOWN || message == WM_SYSKEYDOWN || message == WM_KEYUP || message == WM_SYSKEYUP) ? !handled : lResult;
}

bool WebView::developerExtrasEnabled() const
{
    return true;
}

void WebView::focusView()
{
	// 通知WEBKIT控制层
	FocusController* focusController = m_page->focusController();
	if (Frame* frame = focusController->focusedOrMainFrame()) {
		frame->selection()->setFocused(true);
	}
	// else ASSERT_NOT_REACHED();
}

void WebView::unfocusView()
{
	// 通知WEBKIT控制层
	FocusController* focusController = m_page->focusController();
	resetIME(focusController->focusedOrMainFrame());
	if (Frame* frame = focusController->focusedOrMainFrame()) {
		frame->selection()->setFocused(false);
	}
	// else ASSERT_NOT_REACHED();
}

bool WebView::initWithFrame(LPCWSTR frameName, LPCWSTR groupName, bool active)
{
	String fn(frameName), gn(groupName);
	return initWithFrame(fn, gn, active);
}

// 建立View窗口
HWND WebView::initViewWindow()
{
	if (m_viewWindow == NULL)
	{
		// 创建WebView窗口
		registerWebViewWindowClass();
		m_viewWindow = CreateWindowEx(0, kWebViewWindowClassName, 0, WS_CHILD /*| WS_HSCROLL | WS_VSCROLL*/,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, ghMainWnd, 0, ghInstance, 0);
		ShowWindow(m_viewWindow, SW_SHOW);
	}
	return m_viewWindow;
}

// 初始化Frame
bool WebView::initWithFrame(const String &frameName, const String &groupName, bool active)
{
	initViewWindow();

	m_frameName = frameName;
	m_groupName = groupName;

	// 建立HTML视图显示
	m_page = new Page(new WebKit::ChromeClient(this), new WebKit::ContextMenuClient(this), new WebKit::EditorClient(this), new WebKit::DragClient(), new WebKit::InspectorClient(this));
	// 设置显示相关参数
	setPageSettings();

	// 建立coreFrame
	WebKit::FrameLoaderClient *loaderClient = new WebKit::FrameLoaderClient(this);
	RefPtr<Frame> frame = Frame::create(m_page, 0, loaderClient);
	m_frame = frame.get();
	loaderClient->setFrame(m_frame);

	// 初始化空白页，m_page->mainFrame() = m_frame
	m_page->mainFrame()->tree()->setName(m_frameName);
	m_page->mainFrame()->init();
	m_page->setGroupName(m_groupName);

	windowAncestryDidChange();

	// 设置窗口大小
	GetClientRect(m_viewWindow, &m_windowRect);
	if (view())
		view()->resize(m_windowRect.right - m_windowRect.left, m_windowRect.bottom - m_windowRect.top);

	return true;
}

// 缩放处理
bool WebView::setTextSizeMultiplier(float multiplier)
{
    if (!m_frame)
        return false;
    setZoomMultiplier(multiplier, true);
    return true;
}

bool WebView::setPageSizeMultiplier(float multiplier)
{
    if (!m_frame)
        return false;
    setZoomMultiplier(multiplier, false);
    return true;
}

void WebView::setZoomMultiplier(float multiplier, bool isTextOnly)
{
    m_zoomMultiplier = multiplier;
    m_zoomMultiplierIsTextOnly = isTextOnly;
    if (m_frame)
        m_frame->setZoomFactor(multiplier, isTextOnly);
}

void WebView::textSizeMultiplier(float* multiplier)
{
    *multiplier = zoomMultiplier(true);
}

void WebView::pageSizeMultiplier(float* multiplier)
{
    *multiplier = zoomMultiplier(false);
}

float WebView::zoomMultiplier(bool isTextOnly)
{
    if (isTextOnly != m_zoomMultiplierIsTextOnly)
        return 1.0f;
    return m_zoomMultiplier;
}

// void WebView::setCustomTextEncodingName(LPWSTR encodingName)

void WebView::updateActiveStateSoon() const
{
	SetTimer(m_viewWindow, UpdateActiveStateTimer, 0, 0);
}

void WebView::deleteBackingStoreSoon()
{
    m_deleteBackingStoreTimerActive = true;
    SetTimer(m_viewWindow, DeleteBackingStoreTimer, delayBeforeDeletingBackingStoreMsec, 0);
}

void WebView::cancelDeleteBackingStoreSoon()
{
    if (!m_deleteBackingStoreTimerActive)
        return;
    m_deleteBackingStoreTimerActive = false;
    KillTimer(m_viewWindow, DeleteBackingStoreTimer);
}

bool WebView::active()
{
    HWND activeWindow = GetActiveWindow();
    return (activeWindow && m_topLevelParent == findTopLevelParent(activeWindow));
}

void WebView::updateActiveState()
{
	HWND activeWindow = GetActiveWindow();
	m_page->focusController()->setActive(activeWindow && m_topLevelParent == findTopLevelParent(activeWindow));
}

void WebView::updateFocusedAndActiveState()
{
	updateActiveState();

	bool active = m_page->focusController()->isActive();
	Frame* mainFrame = m_page->mainFrame();
	Frame* focusedFrame = m_page->focusController()->focusedOrMainFrame();
	mainFrame->selection()->setFocused(active && mainFrame == focusedFrame);
}

void WebView::windowAncestryDidChange()
{
	HWND newParent = findTopLevelParent(m_viewWindow);
	if (newParent == m_topLevelParent)
		return;
	m_topLevelParent = newParent;

	updateActiveState();
}

// FIXME: This code should move into WebCore so it can be shared by all the WebKits.
#define MinimumZoomMultiplier   0.5f
#define MaximumZoomMultiplier   3.0f
#define ZoomMultiplierRatio     1.2f

bool WebView::canMakeTextLarger()
{
    return canZoomIn(true);
}

bool WebView::canZoomPageIn()
{
	return canZoomIn(false);
}

bool WebView::canZoomIn(bool isTextOnly)
{
    return zoomMultiplier(isTextOnly) * ZoomMultiplierRatio < MaximumZoomMultiplier;
}

bool WebView::makeTextLarger()
{
	return zoomIn(true);
}

bool WebView::zoomPageIn()
{
    return zoomIn(false);
}

bool WebView::zoomIn(bool isTextOnly)
{
    if (!canZoomIn(isTextOnly))
        return false;
    setZoomMultiplier(zoomMultiplier(isTextOnly) * ZoomMultiplierRatio, isTextOnly);
    return true;
}

bool WebView::canMakeTextSmaller()
{
    return canZoomOut(true);
}

bool WebView::canZoomPageOut()
{
    return canZoomOut(false);
}

bool WebView::canZoomOut(bool isTextOnly)
{
    return zoomMultiplier(isTextOnly) / ZoomMultiplierRatio > MinimumZoomMultiplier;
}

bool WebView::makeTextSmaller()
{
	return zoomOut(false);
}

bool WebView::zoomPageOut()
{
    return zoomOut(false);
}

bool WebView::zoomOut(bool isTextOnly)
{
    if (!canZoomOut(isTextOnly))
        return false;
    setZoomMultiplier(zoomMultiplier(isTextOnly) / ZoomMultiplierRatio, isTextOnly);
    return true;
}

bool WebView::canMakeTextStandardSize()
{
    return canResetZoom(true);
}

bool WebView::canResetPageZoom()
{
    return canResetZoom(false);
}

bool WebView::canResetZoom(bool isTextOnly)
{
    return zoomMultiplier(isTextOnly) != 1.0f;
}

bool WebView::makeTextStandardSize()
{
    return resetZoom(true);
}

bool WebView::resetPageZoom()
{
    return resetZoom(false);
}

bool WebView::resetZoom(bool isTextOnly)
{
    if (!canResetZoom(isTextOnly))
        return false;
    setZoomMultiplier(1.0f, isTextOnly);
    return true;
}

// 页面错误处理
String WebView::m_errorTitle;
String WebView::m_errorDisplay;
void WebView::setErrorStrings(LPCWSTR szTitle, LPCWSTR szDisp)
{
	m_errorTitle = String(szTitle);
	m_errorDisplay = String(szDisp);
}

void WebView::didFailLoadingWithError(unsigned long identifier, const ResourceError& error)
{
	FrameLoader *frameLoader = frame()->loader();
	if (frameLoader)
	{
		LPCWSTR format = 
			L"<HTML><HEAD>"
			L"<STYLE>.error-container {position: relative;padding: 10px;min-height: 50px;width: 80%%;margin-left: auto;margin-right: auto;margin-top: 49px;border: 1px solid #999999;-webkit-border-radius: 10px;}</STYLE>"
			L"<TITLE>%s</TITLE>"
			L"</HEAD><BODY>"
			L"<div class='error-container'>"
			L"<p>'%s'&nbsp;%s</p>"
			L"</div>"
			L"</BODY></HTML>";
		String failUrl = error.failingURL();
		LPWSTR errorPage = new WCHAR[wcslen(format) + m_errorTitle.length() + m_errorDisplay.length() + failUrl.length() + 1];
		if (errorPage)
		{
			unsigned nLen = swprintf(errorPage, format, 
				m_errorTitle.charactersWithNullTermination(), 
				failUrl.charactersWithNullTermination(), 
				m_errorDisplay.charactersWithNullTermination());

			PassRefPtr<SharedBuffer> buf(SharedBuffer::create((const char*)errorPage, nLen * sizeof(WCHAR)));
			SubstituteData data(buf, String("text/html"), String("Unicode"), KURL(failUrl));
			frameLoader->load(WebCore::ResourceRequest(failUrl), data);

			delete errorPage;
		}
	}
}

// 设置页面属性
void WebView::setPageSettings()
{
	Settings* settings = m_page->settings();

	LPCWSTR str = GetConfigAttr(L"FACE", L"FONT", L"DEFAULT");
	settings->setCursiveFontFamily(AtomicString(str, wcslen(str)));
	settings->setFantasyFontFamily(AtomicString(str, wcslen(str)));
	settings->setFixedFontFamily(AtomicString(str, wcslen(str)));
	settings->setSerifFontFamily(AtomicString(str, wcslen(str)));
	settings->setSansSerifFontFamily(AtomicString(str, wcslen(str)));
	settings->setStandardFontFamily(AtomicString(str, wcslen(str)));

	int nSize = 14;
	LPCWSTR szSize = GetConfigAttr(L"SIZE", L"FONT", L"DEFAULT");
	if (szSize && *szSize)
		swscanf(szSize, L"%d", &nSize);
	settings->setDefaultFixedFontSize(nSize);
	settings->setDefaultFontSize(nSize);

	str = GetConfigAttr(L"VALUE", L"BASE", L"ENCODING");
	settings->setDefaultTextEncodingName(AtomicString(str, wcslen(str)));

	settings->setJavaEnabled(false);
	settings->setJavaScriptEnabled(true);
	settings->setJavaScriptCanOpenWindowsAutomatically(true);

	settings->setMinimumFontSize(8);
	settings->setMinimumLogicalFontSize(8);

	settings->setPluginsEnabled(false);
	settings->setPrivateBrowsingEnabled(false);

	settings->setLoadsImagesAutomatically(true);
	settings->setUserStyleSheetLocation(KURL());
	
	settings->setShouldPrintBackgrounds(true);
	settings->setTextAreasAreResizable(false);
	settings->setDeveloperExtrasEnabled(developerExtrasEnabled());
	settings->setShouldPaintCustomScrollbars(true);

#if 0 // FIXME：以下采用缺省值
	settings->setEditableLinkBehavior(false);
	settings->setUsesPageCache(true);
	settings->setDOMPasteAllowed(true);
	settings->setShowsURLsInToolTips(false);

    settings->setForceFTPDirectoryListings(true);
    // settings->setDeveloperExtrasEnabled(developerExtrasEnabled());
    settings->setNeedsSiteSpecificQuirks(false);
	settings->setFontRenderingMode(NormalRenderingMode /* AlternateRenderingMode */ );
	settings->setAuthorAndUserStylesEnabled(false);
	settings->setApplicationChromeMode(false);
	settings->setOfflineWebApplicationCacheEnabled(false);
#endif
}

// iSee添加
void WebView::load(LPCWSTR url)
{
	iSeeLogW(L"WebView::load %s\n", url);
	load(String(url));
}

void WebView::load(const String &url)
{
	load(WebCore::ResourceRequest(autocorrectURL(url)));
}

void WebView::load(const WebCore::ResourceRequest &request)
{
	frame()->loader()->load(request);
}

void WebView::reload()
{
	frame()->loader()->reload();
}

void WebView::stop()
{
	frame()->loader()->stopAllLoaders();
}

void WebView::setAddressBarUrl(const WebCore::String& url)
{
	LPCWSTR wszUrl = (LPCWSTR)url.characters();
	SendMessage(ghMainWnd, WM_SET_URL, (WPARAM)url.length(), (LPARAM)wszUrl);
}

void WebView::goForward()
{
	if (historyEnd())
		return;
	if (m_page->goForward())
	{
		BackForwardList* bfl = m_page->backForwardList();
		if (bfl && bfl->currentItem())
		{
			setAddressBarUrl(bfl->currentItem()->urlString());
		}
	}
}

void WebView::goBack()
{
	if (historyBegin())
		return;
	if (m_page->goBack())
	{
		BackForwardList* bfl = m_page->backForwardList();
		if (bfl && bfl->currentItem())
		{
			setAddressBarUrl(bfl->currentItem()->urlString());
		}
	}
}

bool WebView::historyBegin()
{
	BackForwardList* bfl = m_page->backForwardList();
	if (bfl)
		return bfl->backItem() ? false : true;
	return true;
}

bool WebView::historyEnd()
{
	BackForwardList* bfl = m_page->backForwardList();
	if (bfl)
		return bfl->forwardItem() ? false : true;
	return true;
}

LPCWSTR WebView::getTitle(int &nLen)
{
	LPCWSTR wszRes = m_title.charactersWithNullTermination();
	nLen = m_title.length();
	return wszRes;
}

LPCWSTR WebView::getUrl(int &nLen)
{
	String url = frame()->loader()->url().string();
	LPCWSTR wszUrl = (LPCWSTR)url.charactersWithNullTermination();
	nLen = (int)url.length();
	return wszUrl;
}

PassRefPtr<Frame> WebView::createFrame(const KURL& url, const String& name, HTMLFrameOwnerElement* ownerElement, const String& referrer, 
							bool /*allowsScrolling*/, int /*marginWidth*/, int /*marginHeight*/)
{
	Frame* coreFrame = frame(); // = m_frame
	// 建立childFrame
	WebKit::FrameLoaderClient *loaderClient = new WebKit::FrameLoaderClient(this);
	RefPtr<Frame> childFrame = Frame::create(m_page, ownerElement, loaderClient); // 注意FrameLoaderClient = WebFrame
	loaderClient->setFrame(childFrame.get());

	coreFrame->tree()->appendChild(childFrame);
	childFrame->tree()->setName(name);
	childFrame->init();

	// 载入url
	loadURLIntoChild(url, referrer, childFrame.get());

	// The frame's onload handler may have removed it from the document.
	if (!childFrame->tree()->parent())
		return 0;

	// Propagate the marginwidth/height and scrolling modes to the view.
	if (ownerElement->hasTagName(frameTag) || ownerElement->hasTagName(iframeTag)) {
		HTMLFrameElement* frameElt = static_cast<HTMLFrameElement*>(ownerElement);
		if (frameElt->scrollingMode() == ScrollbarAlwaysOff)
			childFrame->view()->setScrollbarsMode(ScrollbarAlwaysOff);
		int marginWidth = frameElt->getMarginWidth();
		int marginHeight = frameElt->getMarginHeight();
		if (marginWidth != -1)
			childFrame->view()->setMarginWidth(marginWidth);
		if (marginHeight != -1)
			childFrame->view()->setMarginHeight(marginHeight);
	}

	return childFrame.release();
}

void WebView::loadURLIntoChild(const KURL& originalURL, const String& referrer, Frame *childFrame)
{
	ASSERT(childFrame);

	Frame* coreFrame = frame();
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
		if (HistoryItem* childItem = parentItem->childItemWithName(childFrame->tree()->name())) {
			// Use the original URL to ensure we get all the side-effects, such as
			// onLoad handlers, of any redirects that happened. An example of where
			// this is needed is Radar 3213556.
			url = childItem->originalURL();
			// These behaviors implied by these loadTypes should apply to the child frames
			childLoadType = loadType;

			if (isBackForwardLoadType(loadType))
				// For back/forward, remember this item so we can traverse any child items as child frames load
				childFrame->loader()->setProvisionalHistoryItem(childItem);
			else
				// For reload, just reinstall the current item, since a new child frame was created but we won't be creating a new BF item
				childFrame->loader()->setCurrentHistoryItem(childItem);
		}
	}

	// FIXME: Handle loading WebArchives here
	String frameName = childFrame->tree()->name();
	childFrame->loader()->loadURL(url, referrer, frameName, childLoadType, 0, 0);
}

// 创建一个新窗口
WebView* WebView::createWebViewWithRequest(bool evented, const WebCore::ResourceRequest& request)
{
	if (m_enableWindowOpen || evented)
		return newWebView(request, String(L""), String(L"Default"));
	else
		return NULL;
}

// 设置菜单窗口
void WebView::didReceiveTitle(const WebCore::String title, bool setTab)
{
	m_title = title;
	if (setTab)
		SendMessage(ghMainWnd, WM_SET_TITLE, 0, (LPARAM)this);
}

// 通知下载完成
void WebView::dispatchDidCommitLoad()
{
	SendMessage(ghMainWnd, WM_UPDATE_STATE, 0, (LPARAM)this);
	// 更新frame状态
	updateFocusedAndActiveState();
}

// JavaScript对话框
void WebView::runJavaScriptAlert(const String& message)
{
	String newStr(message);
	RunJavaScriptAlert(newStr.charactersWithNullTermination());
}

bool WebView::runJavaScriptConfirm(const String& message)
{
	String newStr(message);
	return RunJavaScriptConfirm(newStr.charactersWithNullTermination());
}

bool WebView::runJavaScriptPrompt(const String& message, const String& defaultValue, String& result)
{
	String newStr1(message), newStr2(defaultValue);
	UINT nLen = 0;
	LPWSTR wszRes = RunJavaScriptPrompt(newStr1.charactersWithNullTermination(), newStr2.charactersWithNullTermination(), nLen);
	if (wszRes && nLen > 0)
	{
		result = String(wszRes, nLen);
		free(wszRes);
		return true;
	}
	return false;
}

// IME操作
class IMMDict
{
	typedef HIMC (CALLBACK *getContextPtr)(HWND);
	typedef BOOL (CALLBACK *releaseContextPtr)(HWND, HIMC);
	typedef LONG (CALLBACK *getCompositionStringPtr)(HIMC, DWORD, LPVOID, DWORD);
	typedef BOOL (CALLBACK *setCandidateWindowPtr)(HIMC, LPCANDIDATEFORM);
	typedef BOOL (CALLBACK *setOpenStatusPtr)(HIMC, BOOL);
	typedef BOOL (CALLBACK *notifyIMEPtr)(HIMC, DWORD, DWORD, DWORD);
	typedef BOOL (CALLBACK *associateContextExPtr)(HWND, HIMC, DWORD);

public:
	getContextPtr getContext;
	releaseContextPtr releaseContext;
	getCompositionStringPtr getCompositionString;
	setCandidateWindowPtr setCandidateWindow;
	setOpenStatusPtr setOpenStatus;
	notifyIMEPtr notifyIME;
	associateContextExPtr associateContextEx;

	static const IMMDict& dict();
private:
	IMMDict();
	HMODULE m_instance;
};

const IMMDict& IMMDict::dict()
{
	static IMMDict instance;
	return instance;
}

IMMDict::IMMDict()
{
#ifndef _WIN32_WCE
	m_instance = ::LoadLibrary(TEXT("IMM32.DLL"));
#else
	m_instance = ::LoadLibrary(TEXT("coredll.DLL"));
#undef GetProcAddress
#define GetProcAddress GetProcAddressA
#endif
	getContext = reinterpret_cast<getContextPtr>(::GetProcAddress(m_instance, "ImmGetContext"));
	ASSERT(getContext);
	releaseContext = reinterpret_cast<releaseContextPtr>(::GetProcAddress(m_instance, "ImmReleaseContext"));
	ASSERT(releaseContext);
	getCompositionString = reinterpret_cast<getCompositionStringPtr>(::GetProcAddress(m_instance, "ImmGetCompositionStringW"));
	ASSERT(getCompositionString);
	setCandidateWindow = reinterpret_cast<setCandidateWindowPtr>(::GetProcAddress(m_instance, "ImmSetCandidateWindow"));
	ASSERT(setCandidateWindow);
	setOpenStatus = reinterpret_cast<setOpenStatusPtr>(::GetProcAddress(m_instance, "ImmSetOpenStatus"));
	ASSERT(setOpenStatus);
	notifyIME = reinterpret_cast<notifyIMEPtr>(::GetProcAddress(m_instance, "ImmNotifyIME"));
	ASSERT(notifyIME);
	associateContextEx = reinterpret_cast<associateContextExPtr>(::GetProcAddress(m_instance, "ImmAssociateContextEx"));
	ASSERT(associateContextEx);
}

HIMC WebView::getIMMContext() 
{
	HIMC context = IMMDict::dict().getContext(m_viewWindow);
	return context;
}

void WebView::releaseIMMContext(HIMC hIMC)
{
	if (!hIMC)
		return;
	IMMDict::dict().releaseContext(m_viewWindow, hIMC);
}

void WebView::prepareCandidateWindow(Frame* targetFrame, HIMC hInputContext) 
{
	IntRect caret;
	if (RefPtr<Range> range = targetFrame->selection()->selection().toRange())
	{
		ExceptionCode ec = 0;
		RefPtr<Range> tempRange = range->cloneRange(ec);
		caret = targetFrame->firstRectForRange(tempRange.get());
	}
	caret = targetFrame->view()->contentsToWindow(caret);
	CANDIDATEFORM form;
	form.dwIndex = 0;
	form.dwStyle = CFS_EXCLUDE;
	form.ptCurrentPos.x = caret.x();
	form.ptCurrentPos.y = caret.y() + caret.height();
	form.rcArea.top = caret.y();
	form.rcArea.bottom = caret.bottom();
	form.rcArea.left = caret.x();
	form.rcArea.right = caret.right();
	IMMDict::dict().setCandidateWindow(hInputContext, &form);
}

void WebView::resetIME(Frame* targetFrame)
{
	if (targetFrame)
		targetFrame->editor()->confirmCompositionWithoutDisturbingSelection();

	if (HIMC hInputContext = getIMMContext())
	{
		IMMDict::dict().notifyIME(hInputContext, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		releaseIMMContext(hInputContext);
	}
}

void WebView::updateSelectionForIME()
{
	Frame* targetFrame = m_page->focusController()->focusedOrMainFrame();
	if (!targetFrame || !targetFrame->editor()->hasComposition())
		return;

	if (targetFrame->editor()->ignoreCompositionSelectionChange())
		return;

	unsigned start;
	unsigned end;
	if (!targetFrame->editor()->getCompositionSelection(start, end))
		resetIME(targetFrame);
}

void WebView::setInputMethodState(bool enabled)
{
	IMMDict::dict().associateContextEx(m_viewWindow, 0, enabled ? IACE_DEFAULT : 0);
}

void WebView::selectionChanged()
{
	updateSelectionForIME();
}

bool WebView::onIMEStartComposition()
{
	m_inIMEComposition++;
	Frame* targetFrame = m_page->focusController()->focusedOrMainFrame();
	if (!targetFrame)
		return true;

	HIMC hInputContext = getIMMContext();
	prepareCandidateWindow(targetFrame, hInputContext);
	releaseIMMContext(hInputContext);
	return true;
}

static bool getCompositionString(HIMC hInputContext, DWORD type, String& result)
{
	int compositionLength = IMMDict::dict().getCompositionString(hInputContext, type, 0, 0);
	if (compositionLength <= 0)
		return false;
	Vector<UChar> compositionBuffer(compositionLength / 2);
	compositionLength = IMMDict::dict().getCompositionString(hInputContext, type, (LPVOID)compositionBuffer.data(), compositionLength);
	result = String(compositionBuffer.data(), compositionLength / 2);
	ASSERT(!compositionLength || compositionBuffer[0]);
	ASSERT(!compositionLength || compositionBuffer[compositionLength / 2 - 1]);
	return true;
}

static void compositionToUnderlines(const Vector<DWORD>& clauses, const Vector<BYTE>& attributes, Vector<CompositionUnderline>& underlines)
{
	if (clauses.isEmpty())
	{
		underlines.clear();
		return;
	}

	const size_t numBoundaries = clauses.size() - 1;
	underlines.resize(numBoundaries);
	for (unsigned i = 0; i < numBoundaries; i++)
	{
		underlines[i].startOffset = clauses[i];
		underlines[i].endOffset = clauses[i + 1];
		BYTE attribute = attributes[clauses[i]];
		underlines[i].thick = attribute == ATTR_TARGET_CONVERTED || attribute == ATTR_TARGET_NOTCONVERTED;
		underlines[i].color = Color(0,0,0);
	}
}

bool WebView::onIMEComposition(LPARAM lparam)
{
	HIMC hInputContext = getIMMContext();
	if (!hInputContext)
		return true;

	Frame* targetFrame = m_page->focusController()->focusedOrMainFrame();
	if (!targetFrame || !targetFrame->editor()->canEdit())
		return true;

	prepareCandidateWindow(targetFrame, hInputContext);

	if (lparam & GCS_RESULTSTR || !lparam)
	{
		String compositionString;
		if (!getCompositionString(hInputContext, GCS_RESULTSTR, compositionString) && lparam)
			return true;

		targetFrame->editor()->confirmComposition(compositionString);
	}
	else
	{
		String compositionString;
		if (!getCompositionString(hInputContext, GCS_COMPSTR, compositionString))
			return true;

		// Composition string attributes
		int numAttributes = IMMDict::dict().getCompositionString(hInputContext, GCS_COMPATTR, 0, 0);
		Vector<BYTE> attributes(numAttributes);
		IMMDict::dict().getCompositionString(hInputContext, GCS_COMPATTR, attributes.data(), numAttributes);

		// Get clauses
		int numClauses = IMMDict::dict().getCompositionString(hInputContext, GCS_COMPCLAUSE, 0, 0);
		Vector<DWORD> clauses(numClauses / sizeof(DWORD));
		IMMDict::dict().getCompositionString(hInputContext, GCS_COMPCLAUSE, clauses.data(), numClauses);

		Vector<CompositionUnderline> underlines;
		compositionToUnderlines(clauses, attributes, underlines);

		int cursorPosition = LOWORD(IMMDict::dict().getCompositionString(hInputContext, GCS_CURSORPOS, 0, 0));

		targetFrame->editor()->setComposition(compositionString, underlines, cursorPosition, 0);
	}

	return true;
}

bool WebView::onIMEEndComposition()
{
	if (m_inIMEComposition) 
		m_inIMEComposition--;
	return true;
}

bool WebView::onIMEChar(WPARAM, LPARAM)
{
	return true;
}

bool WebView::onIMENotify(WPARAM, LPARAM, LRESULT*)
{
	return false;
}

bool WebView::onIMERequestCharPosition(Frame* targetFrame, IMECHARPOSITION* charPos, LRESULT* result)
{
	IntRect caret;
	ASSERT(charPos->dwCharPos == 0 || targetFrame->editor()->hasComposition());
	if (RefPtr<Range> range = targetFrame->editor()->hasComposition() ? targetFrame->editor()->compositionRange() : targetFrame->selection()->selection().toRange())
	{
		ExceptionCode ec = 0;
		RefPtr<Range> tempRange = range->cloneRange(ec);
		tempRange->setStart(tempRange->startContainer(ec), tempRange->startOffset(ec) + charPos->dwCharPos, ec);
		caret = targetFrame->firstRectForRange(tempRange.get());
	}
	caret = targetFrame->view()->contentsToWindow(caret);
	charPos->pt.x = caret.x();
	charPos->pt.y = caret.y();
	::ClientToScreen(m_viewWindow, &charPos->pt);
	charPos->cLineHeight = caret.height();
	::GetWindowRect(m_viewWindow, &charPos->rcDocument);
	*result = TRUE;
	return true;
}

bool WebView::onIMERequestReconvertString(Frame* targetFrame, RECONVERTSTRING* reconvertString, LRESULT* result)
{
	RefPtr<Range> selectedRange = targetFrame->selection()->toRange();
	String text = selectedRange->text();
	if (!reconvertString)
	{
		*result = sizeof(RECONVERTSTRING) + text.length() * sizeof(UChar);
		return true;
	}

	unsigned totalSize = sizeof(RECONVERTSTRING) + text.length() * sizeof(UChar);
	*result = totalSize;
	if (totalSize > reconvertString->dwSize)
	{
		*result = 0;
		return false;
	}
	reconvertString->dwCompStrLen = text.length();
	reconvertString->dwStrLen = text.length();
	reconvertString->dwTargetStrLen = text.length();
	reconvertString->dwStrOffset = sizeof(RECONVERTSTRING);
	memcpy(reconvertString + 1, text.characters(), text.length() * sizeof(UChar));
	return true;
}

bool WebView::onIMERequest(WPARAM request, LPARAM data, LRESULT* result)
{
	Frame* targetFrame = m_page->focusController()->focusedOrMainFrame();
	if (!targetFrame || !targetFrame->editor()->canEdit())
		return true;

	switch (request)
	{
	case IMR_RECONVERTSTRING:
		return onIMERequestReconvertString(targetFrame, (RECONVERTSTRING*)data, result);

	case IMR_QUERYCHARPOSITION:
		return onIMERequestCharPosition(targetFrame, (IMECHARPOSITION*)data, result);
	}
	return false;
}

bool WebView::onIMESelect(WPARAM, LPARAM)
{
	return false;
}

bool WebView::onIMESetContext(WPARAM, LPARAM)
{
	return false;
}

// 绘制应用程序启动的Loading...
static void DrawAppLoading(HWND hWnd)
{
	RECT rcPaint;
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd, &ps);
#ifndef _WIN32_WCE
	rcPaint = ps.rcPaint;
#else
	GetClientRect(hWnd, &rcPaint);
#endif
	// The size here looks unusual.  The negative number is intentional.  The logical size constant is 32.
	LOGFONT winfont;
	winfont.lfHeight = -28;
	winfont.lfWidth = 0;
	winfont.lfEscapement = 0;
	winfont.lfOrientation = 0;
	winfont.lfUnderline = false;
	winfont.lfStrikeOut = false;
	winfont.lfCharSet = DEFAULT_CHARSET;
	winfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	winfont.lfQuality = 5; // Force cleartype.
	winfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	winfont.lfItalic = false;
	winfont.lfWeight = 700; // FIXME: Support weights for real.
	wcscpy(winfont.lfFaceName, L"Tahoma");

	HFONT hfont = CreateFontIndirect(&winfont);
	SelectObject(hdc, hfont);

	// 写等待字样
	const WCHAR szText[] = L"Loading...";
	DrawText(hdc, szText, wcslen(szText), &rcPaint, DT_VCENTER | DT_CENTER);

	DeleteObject(hfont);

	// 结束绘制
	EndPaint(hWnd, &ps);
}

static LRESULT CALLBACK WebViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 删除全部视图
	if (message == WM_DESTROY)
	{
#ifndef _WIN32_WCE
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)0);
#else
		SetWindowLong(hWnd, 0, (LONG)0);
#endif
		WebView::deleteAll();
		return TRUE;
	}
	
	// 窗口消息处理
#ifndef _WIN32_WCE
	LONG_PTR longPtr = GetWindowLongPtr(hWnd, 0);
#else
	LONG longPtr = GetWindowLong(hWnd, 0);
#endif
	WebView* webView = reinterpret_cast<WebView*>(longPtr);
	if (webView)
	{	// 缺省消息已经在函数WndProc内部处理了。
		return webView->WndProc(hWnd, message, wParam, lParam);
	}
	else if (message == WM_PAINT)
	{
		DrawAppLoading(hWnd);
		// 缺省消息处理
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	else
	{	// 缺省消息处理
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

// 本地函数
static String autocorrectURL(const String& url)
{
	if (url.isEmpty())
		return blankURL();
	
	String parsedURL = url;
	if (!url.startsWith("http://") && !url.startsWith("https://")
		&& !url.startsWith("file://") && !url.startsWith("ftp://"))
		parsedURL = String("http://") + url;
	return parsedURL;
}

// 页面列表处理
static Vector<WebView*, 8> m_viewList;
static int m_nCurrItem = 0;

void WebView::activeView(bool setUrl)
{
	bool f = false;
	for (int i = 0; i < m_viewList.size(); i++)
	{
		if (m_viewList[i] == this)
		{
			m_nCurrItem = i;
			f = true;
			break;
		}
	}
	if (!f)
		return;

	// 清理前一个页面占用的定时器、内存等等。
	if (WebView *view = (WebView *)
#ifndef _WIN32_WCE
	GetWindowLongPtr(m_viewWindow, 0)
#else
	GetWindowLong(m_viewWindow, 0)
#endif
	)
	{
		view->unfocusView();
		view->deleteBackingStore();
	}

	// 通知页面URL改变
	if (setUrl)
	{
		setAddressBarUrl(frame()->loader()->url().string());
	}

	// 激活窗口
	focusView();
	
	// 打开显示
#pragma warning(suppress: 4244)
#ifndef _WIN32_WCE
	SetWindowLongPtr(m_viewWindow, 0, (LONG_PTR)this);
#else
	SetWindowLong(m_viewWindow, 0, (LONG)this);
#endif
	RECT rect;
	::GetClientRect(m_viewWindow, &rect);
	if (_rw(rect) != _rw(m_windowRect) || _rh(rect) != _rh(m_windowRect))
	{
		m_windowRect = rect;
		view()->resize(_rw(rect), _rh(rect));
	}
	InvalidateRect(m_viewWindow, NULL, FALSE);
	UpdateWindow(m_viewWindow);
}

WebView *WebView::newWebView(LPCWSTR url, LPCWSTR frameName, LPCWSTR groupName)
{
	return newWebView(WebCore::ResourceRequest(autocorrectURL(String(url))), String(frameName), String(groupName));
}

WebView *WebView::newWebView(const WebCore::ResourceRequest &request, const String &frameName, const String &groupName)
{
	if (m_viewList.size() >= MAX_VIEM_NUM)
		return NULL;
	WebView *view = new WebView();
	if (0 == view)
		return NULL;
	if (view)
	{	// 加入到列表
		if (m_viewList.size() > 0)
			m_viewList.insert(m_nCurrItem, &view, 1);
		else
			m_viewList.append(&view, 1);

		// 开始处理
		iSeeLogA("WebView::newWebView %08X\n", view);
		if (view->initWithFrame(frameName, groupName, true))
		{
			SendMessage(ghMainWnd, WM_ADD_TABWIN, 0, (LPARAM)view);
			if (!request.isEmpty())
				view->load(request);
			view->activeView(true); // 激活这个窗口
			iSeeLogA("WebView::newWebView active\n");
			return view;
		}
		else
			deleteWebView(view);
	}
	return NULL;
}

bool WebView::deleteWebView(int i)
{
	if (i >= 0 && i < m_viewList.size())
	{
		WebView *wv = m_viewList[i];
		m_viewList.remove(i);
		SendMessage(ghMainWnd, WM_DEL_TABWIN, 0, (LPARAM)wv);
		delete wv;
		if (m_viewList.size() == 0)
		{	// 如果是最后一个页面，则需要新建一个空白页
			newWebView(L"", L"", L"Default");
		}
		return true;
	}
	return false;
}

bool WebView::deleteWebView(WebView *wv)
{
	if (wv)
	{
		for (int i = 0; i < m_viewList.size(); i++)
		{
			if (m_viewList[i] == wv)
			{
				m_viewList.remove(i);
				SendMessage(ghMainWnd, WM_DEL_TABWIN, 0, (LPARAM)wv);
				delete wv;
				if (m_viewList.size() == 0)
				{	// 如果是最后一个页面，则需要新建一个空白页
					newWebView(L"", L"", L"Default");
				}
				return true;
			}
		}
	}
	return false;
}

WebView *WebView::currentView()
{
	return m_viewList.size() > 0 ? m_viewList[m_nCurrItem] : 0;
}

void WebView::deleteAll()
{
	for (int i = 0; i < m_viewList.size(); i++)
	{
		WebView *wv = m_viewList[i];
		if (wv)
			delete wv;
	}
	m_viewList.clear();
}

int WebView::countViews()
{
	return m_viewList.size();
}

void WebView::setViewFocus()
{
	SetFocus(m_viewWindow);
}

// Enable javascript window.open ??
bool WebView::m_enableWindowOpen = true;
void WebView::enableWindowOpen(bool b)
{
	m_enableWindowOpen = b;
}
