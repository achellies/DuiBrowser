/*
 * Copyright (C) 2007 Holger Hans Peter Freyther
 * Copyright (C) 2007, 2008 Christian Dywan <christian@imendio.com>
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
#include "ChromeClientISee.h"

#include "FloatRect.h"
#include "IntRect.h"
#include "FrameLoadRequest.h"
#include "WindowFeatures.h"
#include "HitTestResult.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "DocumentLoader.h"
#include "ScriptController.h"

#include "WebView.h"

using namespace WebCore;

namespace WebKit {

ChromeClient::ChromeClient(WebView *webView)
: m_webView(webView)
{
}

void ChromeClient::chromeDestroyed()
{
	delete this;
}

FloatRect ChromeClient::windowRect()
{
	if (!m_webView)
		return FloatRect();
	RECT rect;
	m_webView->frameRect(&rect);
	return rect;
}

void ChromeClient::setWindowRect(const FloatRect& r)
{
	iSeeLogA("ChromeClient::setWindowRect()\n");
#ifdef _WIN32_WCE
#else
// #error WIN32 Platform need fix this!
#endif
}

FloatRect ChromeClient::pageRect()
{
	if (!m_webView)
		return FloatRect();
	RECT rect;
	m_webView->frameRect(&rect);
	return rect;
}

float ChromeClient::scaleFactor()
{
	// Windows doesn't support UI scaling.
	iSeeLogA("ChromeClient::scaleFactor()\n");
	return 1.0;
}

void ChromeClient::focus()
{ 
	iSeeLogA("ChromeClient::focus()\n");
}

void ChromeClient::unfocus() 
{ 
	iSeeLogA("ChromeClient::unfocus()\n");
}

Page* ChromeClient::createWindow(Frame* frame, const FrameLoadRequest& frameLoadRequest, const WindowFeatures& features) 
{
	if (features.dialog)
	{	// FIXME：模式对话框，暂不实现
		return 0;
	}

	bool evented = false;
	if (frame)
	{
		evented = 
		(frame->loader() && frame->loader()->documentLoader() && frame->loader()->documentLoader()->triggeringAction().event()) ||
		(frame->script() && frame->script()->globalObject() && frame->script()->globalObject()->currentEvent());
	}
	WebView *view = WebView::createWebViewWithRequest(evented, frameLoadRequest.resourceRequest());
	if (view)
	{
		return view->page();
	}

	return 0; 
}

void ChromeClient::show()
{
	iSeeLogA("ChromeClient::show()\n");
	m_webView->activeView();
}

bool ChromeClient::canRunModal()
{
	iSeeLogA("ChromeClient::canRunModal()\n");
	return false;
}

void ChromeClient::runModal()
{
	iSeeLogA("ChromeClient::runModal()\n");
}

void ChromeClient::setToolbarsVisible(bool)
{
	iSeeLogA("ChromeClient::setToolbarsVisible()\n");
}

bool ChromeClient::toolbarsVisible()
{
	iSeeLogA("ChromeClient::toolbarsVisible()\n");
	return false;
}

void ChromeClient::setStatusbarVisible(bool)
{
	iSeeLogA("ChromeClient::setStatusbarVisible()\n");
}

bool ChromeClient::statusbarVisible()
{
	iSeeLogA("ChromeClient::statusbarVisible()\n");
	return false;
}

void ChromeClient::setScrollbarsVisible(bool)
{
	iSeeLogA("ChromeClient::setScrollbarsVisible()\n");
}

bool ChromeClient::scrollbarsVisible()
{
	iSeeLogA("ChromeClient::scrollbarsVisible()\n");
	return false;
}

void ChromeClient::setMenubarVisible(bool)
{
	iSeeLogA("ChromeClient::setMenubarVisible()\n");
}

bool ChromeClient::menubarVisible()
{
	iSeeLogA("ChromeClient::menubarVisible()\n");
	return false;
}

void ChromeClient::setResizable(bool)
{
	iSeeLogA("ChromeClient::setResizable()\n");
}

void ChromeClient::closeWindowSoon()
{	// Call WebView->closeWindowSoon()
	iSeeLogA("ChromeClient::closeWindowSoon()\n");
}

bool ChromeClient::canTakeFocus(FocusDirection)
{
	iSeeLogA("ChromeClient::canTakeFocus()\n");
	return true;
}

void ChromeClient::takeFocus(FocusDirection)
{
	iSeeLogA("ChromeClient::takeFocus()\n");
}

bool ChromeClient::canRunBeforeUnloadConfirmPanel()
{
	iSeeLogA("ChromeClient::canRunBeforeUnloadConfirmPanel()\n");
	return true;
}

bool ChromeClient::runBeforeUnloadConfirmPanel(const WebCore::String& message, WebCore::Frame* frame)
{
	return runJavaScriptConfirm(frame, message);
}

void ChromeClient::addMessageToConsole(const WebCore::String&, unsigned int, const WebCore::String&)
{
	iSeeLogA("ChromeClient::addMessageToConsole()\n");
}

void ChromeClient::runJavaScriptAlert(Frame*, const String& message)
{
	m_webView->runJavaScriptAlert(message);
}

bool ChromeClient::runJavaScriptConfirm(Frame*, const String& message)
{
	return m_webView->runJavaScriptConfirm(message);
}

bool ChromeClient::runJavaScriptPrompt(Frame*, const String& message, const String& defaultValue, String& result)
{
    return m_webView->runJavaScriptPrompt(message, defaultValue, result);
}

void ChromeClient::setStatusbarText(const String&)
{
    iSeeLogA("ChromeClient::setStatusbarText()\n");
}

bool ChromeClient::shouldInterruptJavaScript()
{
    iSeeLogA("ChromeClient::shouldInterruptJavaScript()\n");
    return false;
}

bool ChromeClient::tabsToLinks() const
{
    iSeeLogA("ChromeClient::tabsToLinks()\n");
    return true; // FIXME：将TAB转换为LINK？？（原来为false）
}

IntRect ChromeClient::windowResizerRect() const
{
    iSeeLogA("ChromeClient::windowResizerRect()\n");
    return IntRect();
}

void ChromeClient::addToDirtyRegion(const IntRect& dirtyRect)
{
    m_webView->addToDirtyRegion(dirtyRect);
}

void ChromeClient::scrollBackingStore(int dx, int dy, const IntRect& scrollViewRect, const IntRect& clipRect)
{
    m_webView->scrollBackingStore(m_webView->view(), dx, dy, scrollViewRect, clipRect);
}

void ChromeClient::updateBackingStore()
{
    m_webView->updateBackingStore(m_webView->view(), 0, false);
}

void ChromeClient::mouseDidMoveOverElement(const HitTestResult& hit, unsigned modifierFlags)
{
	iSeeLogA("ChromeClient::mouseDidMoveOverElement()\n");
	// check if the element is a link...
	bool isLink = hit.isLiveLink();
	if (isLink) {
#if 0
		KURL url = hit.absoluteLinkURL();
		if (!url.isEmpty() && url != m_hoveredLinkURL) {
			CString titleString = hit.title().utf8();
			DeprecatedCString urlString = url.prettyURL().utf8();
			g_signal_emit_by_name(m_webView, "hovering-over-link", titleString.data(), urlString.data());
			m_hoveredLinkURL = url;
		}
#endif
	} else if (!isLink && !m_hoveredLinkURL.isEmpty()) {
#if 0
		g_signal_emit_by_name(m_webView, "hovering-over-link", 0, 0);
		m_hoveredLinkURL = KURL();
#endif
	}
}

void ChromeClient::setToolTip(const String&)
{
    iSeeLogA("ChromeClient::setToolTip()\n");
}

void ChromeClient::print(Frame*)
{
	iSeeLogA("ChromeClient::print()\n");
}

void ChromeClient::exceededDatabaseQuota(Frame*, const String&)
{
	iSeeLogA("ChromeClient::exceededDatabaseQuota()\n");
}

} // namespace WebKit
