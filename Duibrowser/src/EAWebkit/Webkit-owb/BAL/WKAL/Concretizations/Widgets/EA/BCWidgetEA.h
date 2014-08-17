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
// BCWidgetEA.h
// By Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////

#ifndef Widget_h
#define Widget_h

#include <wtf/Platform.h>
#include "BALBase.h"

namespace WKAL {

    class Cursor;
    class Event;
    class Font;
    class GraphicsContext;
    class IntPoint;
    class IntRect;
    class IntSize;
    class PlatformMouseEvent;
    class ScrollView;
    class WidgetClient;
    class WidgetPrivate;

    // Note by Paul Pedriana:
    // A widget is a also known as a window. While some web browsers decorate windows 
    // with caption bars, URL bars, and menus, Widget doesn't know or care if that is 
    // present, though WebKit porters could implement it if they want.
    // The Widget class is a wrapper for the platform-specific window/HWND/viewport/etc.

    class Widget : public WKALBase {
    public:
        Widget();
        virtual ~Widget();

        virtual void setEnabled(bool);
        virtual bool isEnabled() const;

        int x() const;
        int y() const;
        int width() const;
        int height() const;
        IntSize size() const;
        void resize(int, int);
        void resize(const IntSize&);
        IntPoint pos() const;
        void move(int, int);
        void move(const IntPoint&);

        virtual void paint(GraphicsContext*, const IntRect&);
        virtual void invalidate();
        virtual void invalidateRect(const IntRect&);

        virtual void setFrameGeometry(const IntRect&);
        virtual IntRect frameGeometry() const;

        virtual void setFocus();

        void setCursor(const Cursor&);
        Cursor cursor();

        virtual void show();
        virtual void hide();

        void setIsSelected(bool);

        void setClient(WidgetClient*);
        WidgetClient* client() const;

        virtual bool isFrameView() const;
        virtual bool isPluginView() const;

        virtual void removeFromParent();

        // This method is used by plugins on all platforms to obtain a clip rect that includes clips set by WebCore,
        // e.g., in overflow:auto sections.  The clip rects coordinates are in the containing window's coordinate space.
        // This clip includes any clips that the widget itself sets up for its children.
        virtual IntRect windowClipRect() const;

        virtual void handleEvent(Event*);

        // The 'containing window' is the platform-specific window associated with this widget.
        void setContainingWindow(PlatformWidget);
        PlatformWidget containingWindow() const;

        virtual void setParent(ScrollView*);
        ScrollView* parent() const;

        virtual void geometryChanged() const;

        IntRect convertToContainingWindow(const IntRect&) const;
        IntPoint convertToContainingWindow(const IntPoint&) const;
        IntPoint convertFromContainingWindow(const IntPoint&) const;

        virtual IntPoint convertChildToSelf(const Widget*, const IntPoint&) const;
        virtual IntPoint convertSelfToChild(const Widget*, const IntPoint&) const;

        bool suppressInvalidation() const;
        void setSuppressInvalidation(bool);

        // Note by Paul Pedriana: I added these because I couldn't find any other
        // means of getting this information. But perhaps there is some way and 
        // these functions are redundant.
        void SetDirty(bool);    // The widget has a region that needs to be repainted.
        bool IsDirty() const;   

    protected:
        WidgetPrivate* data;    // This is our lower-level representation of the viewport. Defined in the .cpp for this file, as of this writing.
    };

} // namespace WebCore

#endif // Widget_h
