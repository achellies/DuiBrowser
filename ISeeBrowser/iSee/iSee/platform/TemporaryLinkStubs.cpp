#include "config.h"
#include <stdio.h>
#include <windows.h>
#include <winsock2.h>

#include "CString.h"
#include "CachedPage.h"
#include "CachedResource.h"
#include "Clipboard.h"
#include "ContextMenu.h"
#include "ContextMenuItem.h"
#include "CookieJar.h"
#include "Cursor.h"
#include "DocumentFragment.h"
#include "DocumentLoader.h"
#include "EditCommand.h"
#include "Editor.h"
#include "Font.h"
#include "FrameWin.h"
#include "FrameLoadRequest.h"
#include "FrameLoader.h"
#include "FrameLoaderClientISee.h"
#include "FrameView.h"
#include "GraphicsContext.h"
#include "HTMLFrameOwnerElement.h"
#include "Icon.h"
#include "IconDatabase.h"
#include "IconLoader.h"
#include "IntPoint.h"
#include "KURL.h"
#include "Language.h"
#include "MainResourceLoader.h"
#include "Node.h"
#include "NotImplemented.h"
#include "Pasteboard.h"
#include "PlatformMouseEvent.h"
#include "PlatformScrollBar.h"
#include "PlugInInfoStore.h"
#include "ResourceError.h"
#include "ResourceHandle.h"
#include "ResourceLoader.h"
#include "Screen.h"
#include "ScrollBar.h"
#include "SearchPopupMenu.h"
#include "TextBoundaries.h"
#include "TextBreakIteratorInternalICU.h"
#include "loader.h"

using namespace WebCore;

namespace WebCore {
    class Page;
}

/********************************************************/
/* Completely empty stubs (mostly to allow DRT to run): */
/********************************************************/

void SearchPopupMenu::saveRecentSearches(const AtomicString& name, const Vector<String>& searchItems) { notImplemented(); }
void SearchPopupMenu::loadRecentSearches(const AtomicString& name, Vector<String>& searchItems) { notImplemented(); }
SearchPopupMenu::SearchPopupMenu(PopupMenuClient* client) : PopupMenu(client) { notImplemented(); }
bool SearchPopupMenu::enabled() { notImplemented(); return true; }

Color WebCore::focusRingColor() { return 0xFF0000FF; }
void WebCore::setFocusRingColorChangeFunction(void (*)()) { }

Pasteboard* Pasteboard::generalPasteboard() { notImplemented(); return 0; }
void Pasteboard::writeSelection(Range*, bool, Frame*) { notImplemented(); }
void Pasteboard::writeURL(const KURL&, const String&, Frame*) { notImplemented(); }
void Pasteboard::writeImage(Node*, const KURL&, const String&) { notImplemented(); }
void Pasteboard::clear() { notImplemented(); }
bool Pasteboard::canSmartReplace() { notImplemented(); return false; }
PassRefPtr<DocumentFragment> Pasteboard::documentFragment(Frame*, PassRefPtr<Range>, bool allowPlainText, bool& chosePlainText) { notImplemented(); return 0; }
String Pasteboard::plainText(Frame* frame) { notImplemented(); return String(); }
Pasteboard::Pasteboard() { notImplemented(); }
Pasteboard::~Pasteboard() { notImplemented(); }

ContextMenu::ContextMenu(const HitTestResult& result) : m_hitTestResult(result) { notImplemented(); }
ContextMenu::~ContextMenu() { notImplemented(); }
void ContextMenu::appendItem(ContextMenuItem&) { notImplemented(); }
void ContextMenu::setPlatformDescription(PlatformMenuDescription menu) { m_platformDescription = menu; }
PlatformMenuDescription ContextMenu::platformDescription() const  { return m_platformDescription; }
PlatformMenuDescription ContextMenu::releasePlatformDescription() {
	PlatformMenuDescription description = m_platformDescription;
	m_platformDescription = 0;
	return description;
}

ContextMenuItem::ContextMenuItem(PlatformMenuItemDescription) { notImplemented(); }
ContextMenuItem::ContextMenuItem(ContextMenu*) { notImplemented(); }
ContextMenuItem::ContextMenuItem(ContextMenuItemType type, ContextMenuAction action, const String& title, ContextMenu* subMenu) { notImplemented(); }
ContextMenuItem::~ContextMenuItem() { notImplemented(); }
PlatformMenuItemDescription ContextMenuItem::releasePlatformDescription() { notImplemented(); return m_platformDescription; }
ContextMenuItemType ContextMenuItem::type() const { notImplemented(); return ActionType; }
void ContextMenuItem::setType(ContextMenuItemType) { notImplemented(); }
ContextMenuAction ContextMenuItem::action() const { notImplemented(); return ContextMenuItemTagNoAction; }
void ContextMenuItem::setAction(ContextMenuAction) { notImplemented(); }
String ContextMenuItem::title() const { notImplemented(); return String(); }
void ContextMenuItem::setTitle(const String&) { notImplemented(); }
PlatformMenuDescription ContextMenuItem::platformSubMenu() const { notImplemented(); return 0; }
void ContextMenuItem::setSubMenu(ContextMenu*) { notImplemented(); }
void ContextMenuItem::setChecked(bool) { notImplemented(); }
void ContextMenuItem::setEnabled(bool) { notImplemented(); }

namespace WebCore {
Vector<String> supportedKeySizes() { notImplemented(); return Vector<String>(); }
String signedPublicKeyAndChallengeString(unsigned keySizeIndex, const String &challengeString, const KURL &url) { return String(); }

String KURL::fileSystemPath() const { notImplemented(); return String(); }
}

// for ThreadingPthreads.cpp
int gettimeofday(struct timeval *p, struct timezone *tz)
{
	SYSTEMTIME sysTime;
	union {
		__int64 ns100; /*time since 1 Jan 1601 in 100ns units */
		FILETIME ft;
	} _now;

	GetSystemTime(&sysTime);
	SystemTimeToFileTime(&sysTime, &_now.ft);

	p->tv_usec = (long) ((_now.ns100 / 10) % 1000000);
	p->tv_sec  = (long) ((_now.ns100 - 116444736000000000) / 10000000);
	return 0;
}
