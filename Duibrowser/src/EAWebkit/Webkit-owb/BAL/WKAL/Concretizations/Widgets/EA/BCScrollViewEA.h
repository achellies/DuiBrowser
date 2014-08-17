/*
Copyright (C) 2008-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
// BCScrollViewEA.h
// By Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#ifndef ScrollView_h
#define ScrollView_h

#include "IntRect.h"
#include "ScrollTypes.h"
#include "Widget.h"
#include <wtf/HashSet.h>
#include "BALBase.h"


namespace WKAL {

    class FloatRect;
    class PlatformScrollbar;
    class PlatformWheelEvent;

    // Note by Paul Pedriana:
    // A scroll view is a Widget (Window) that is scrollable and thus has a surface larger than the Window viewport.
    // A parent class of scroll view is FrameView, which is a scrollable view of an HTML frame.
    class ScrollView : public Widget {
    public:
        ScrollView();
        ~ScrollView();

        int visibleWidth() const;
        int visibleHeight() const;
        FloatRect visibleContentRect() const;
        FloatRect visibleContentRectConsideringExternalScrollers() const;

        int contentsWidth() const;
        int contentsHeight() const;
        int contentsX() const;
        int contentsY() const;
        IntSize scrollOffset() const;
        void scrollBy(int dx, int dy);
        virtual void scrollRectIntoViewRecursively(const IntRect&);

        virtual void setContentsPos(int x, int y);

        virtual void setVScrollbarMode(ScrollbarMode);
        virtual void setHScrollbarMode(ScrollbarMode);

        // Set the mode for both scrollbars at once.
        virtual void setScrollbarsMode(ScrollbarMode);

        // This gives us a means of blocking painting on our scrollbars until the first layout has occurred.
        void suppressScrollbars(bool suppressed, bool repaintOnUnsuppress = false);

        ScrollbarMode vScrollbarMode() const;
        ScrollbarMode hScrollbarMode() const;

        void addChild(Widget*);
        void removeChild(Widget*);

        virtual void resizeContents(int w, int h);
        void updateContents(const IntRect&, bool now = false);
        void update();

        // Event coordinates are assumed to be in the coordinate space of a window that contains
        // the entire widget hierarchy. It is up to the platform to decide what the precise definition
        // of containing window is. (For example on Mac it is the containing NSWindow.)
        IntPoint windowToContents(const IntPoint&) const;
        IntPoint contentsToWindow(const IntPoint&) const;
        IntRect windowToContents(const IntRect&) const;
        IntRect contentsToWindow(const IntRect&) const;

        void setStaticBackground(bool);

        bool inWindow() const;

        // For platforms that need to hit test scrollbars from within the engine's event handlers (like Win32).
        PlatformScrollbar* scrollbarUnderMouse(const PlatformMouseEvent& mouseEvent);

        // This method exists for scrollviews that need to handle wheel events manually.
        // On Mac the underlying NSScrollView just does the scrolling, but on other platforms
        // (like Windows), we need this method in order to do the scroll ourselves.
        void wheelEvent(PlatformWheelEvent&);

        bool scroll(ScrollDirection, ScrollGranularity);

        #if HAVE(ACCESSIBILITY)
            IntRect contentsToScreen(const IntRect&) const;
            IntPoint screenToContents(const IntPoint&) const;
        #endif

    public:
        HashSet<Widget*>* children();

    private:
        IntSize maximumScroll() const;
        class ScrollViewPrivate;
        ScrollViewPrivate* m_data;
        int m_scrollViewPaintRecursiveCount;        // 2/13/09 CSidhall - Added to track which calls are recursive in scroll view paint  

    public:
        virtual void paint(GraphicsContext*, const IntRect& rect);

        // This is a function which a user may want to override.
        // Returns true if there was any area to update after clipping to the surface.
        // If true then rectResult contains the surface rect that was updated.
        virtual bool updateView(EA::Raster::ISurface* pSurface, const IntRect& rect);

        virtual IntPoint convertChildToSelf(const Widget*, const IntPoint&) const;
        virtual IntPoint convertSelfToChild(const Widget*, const IntPoint&) const;

        virtual void geometryChanged() const;
        virtual void setFrameGeometry(const IntRect&);

        void addToDirtyRegion(const IntRect&);
        void scrollBackingStore(int dx, int dy, const IntRect& scrollViewRect, const IntRect& clipRect);
        void updateBackingStore();

    private:
        void updateScrollbars(const IntSize& desiredOffset);

    public:
        void setBalAdjustments(BalAdjustment* hadj, BalAdjustment* vadj);

    }; // class ScrollView




    inline IntRect ScrollView::contentsToWindow(const IntRect& rect) const
    {
        return IntRect(contentsToWindow(rect.location()), rect.size());
    }

    inline IntRect ScrollView::windowToContents(const IntRect& rect) const
    {
        return IntRect(windowToContents(rect.location()), rect.size());
    }


} // namespace WebCore

#endif // ScrollView_h
