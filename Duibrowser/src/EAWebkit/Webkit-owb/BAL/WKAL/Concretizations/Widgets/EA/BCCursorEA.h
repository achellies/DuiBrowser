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
// BCCursorEA.h
// By Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#ifndef Cursor_h
#define Cursor_h

#include <wtf/Platform.h>
#include "BALBase.h"

namespace WKAL {

    class Image;
    class IntPoint;

    // A cursor is simply an enumerated value of the CursorId enum (below).
    typedef int PlatformCursor;


    enum CursorId
    {
        kCursorIdPointer,
        kCursorIdCross,
        kCursorIdHand,
        kCursorIdMove,
        kCursorIdIBeam,
        kCursorIdWait,
        kCursorIdHelp,
        kCursorIdResizeE,
        kCursorIdResizeN,
        kCursorIdResizeNE,
        kCursorIdResizeNW,
        kCursorIdResizeS,
        kCursorIdResizeSE,
        kCursorIdResizeSW,
        kCursorIdResizeW,
        kCursorIdResizeNS,
        kCursorIdResizeEW,
        kCursorIdResizeNESW,
        kCursorIdResizeNWSE,
        kCursorIdResizeColumn,
        kCursorIdResizeRow,
        kCursorIdVerticalText,
        kCursorIdCell,
        kCursorIdContextMenu,
        kCursorIdNoDrop,
        kCursorIdNotAllowed,
        kCursorIdProgress,
        kCursorIdZAlias,
        kCursorIdZoomIn,
        kCursorIdZoomOut,
        kCursorIdCopy,
        kCursorIdNone,

        kCursorIdCount
    };


    class Cursor : public WKALBase {
    public:
        Cursor();
        Cursor(Image*, const IntPoint& hotspot);
        Cursor(const Cursor&);
       ~Cursor();
        Cursor& operator=(const Cursor&);

        Cursor(PlatformCursor);
        PlatformCursor impl() const;

        // Causes the cursor to be activated.
        void Set() const;

     protected:
        friend const Cursor& enumeratedCursor(CursorId id);

        PlatformCursor m_impl;
    };


    const Cursor& enumeratedCursor(CursorId id);




    ///////////////////////////////////////////////////////////////////////////
    // inlines
    ///////////////////////////////////////////////////////////////////////////

    inline Cursor::Cursor() : m_impl(0)
    {
    }


    inline Cursor::~Cursor()
    {
    }


    inline Cursor::Cursor(Image*, const IntPoint&)
    {
        NotImplemented();
        // It's possible for us to implement this, but the Images would have to be something that
        // is drawn directly on top of the graphics display and not into our render buffer.
    }


    inline Cursor& Cursor::operator=(const Cursor& other)
    {
        m_impl = other.m_impl;
        return *this;
    }


    inline PlatformCursor Cursor::impl() const
    {
        return m_impl;
    }


    // It would be nice if these were a single function taking an id instead of 
    // a bunch of independent functions that serve to bloat the codebase.
    // However, WebKit is using this means and we would need to push a patch
    // through WebKit to change this. In the meantime we redirect in place.
    // We could make these #defines, but that's a little risky with respect
    // to possible name collisions.
    inline const Cursor& pointerCursor()                   { return enumeratedCursor(kCursorIdPointer); }
    inline const Cursor& crossCursor()                     { return enumeratedCursor(kCursorIdCross); }
    inline const Cursor& handCursor()                      { return enumeratedCursor(kCursorIdHand); }
    inline const Cursor& moveCursor()                      { return enumeratedCursor(kCursorIdMove); }
    inline const Cursor& iBeamCursor()                     { return enumeratedCursor(kCursorIdIBeam); }
    inline const Cursor& waitCursor()                      { return enumeratedCursor(kCursorIdWait); }
    inline const Cursor& helpCursor()                      { return enumeratedCursor(kCursorIdHelp); }
    inline const Cursor& eastResizeCursor()                { return enumeratedCursor(kCursorIdResizeE); }
    inline const Cursor& northResizeCursor()               { return enumeratedCursor(kCursorIdResizeN); }
    inline const Cursor& northEastResizeCursor()           { return enumeratedCursor(kCursorIdResizeNE); }
    inline const Cursor& northWestResizeCursor()           { return enumeratedCursor(kCursorIdResizeNW); }
    inline const Cursor& southResizeCursor()               { return enumeratedCursor(kCursorIdResizeS); }
    inline const Cursor& southEastResizeCursor()           { return enumeratedCursor(kCursorIdResizeSE); }
    inline const Cursor& southWestResizeCursor()           { return enumeratedCursor(kCursorIdResizeSW); }
    inline const Cursor& westResizeCursor()                { return enumeratedCursor(kCursorIdResizeW); }
    inline const Cursor& northSouthResizeCursor()          { return enumeratedCursor(kCursorIdResizeNS); }
    inline const Cursor& eastWestResizeCursor()            { return enumeratedCursor(kCursorIdResizeEW); }
    inline const Cursor& northEastSouthWestResizeCursor()  { return enumeratedCursor(kCursorIdResizeNESW); }
    inline const Cursor& northWestSouthEastResizeCursor()  { return enumeratedCursor(kCursorIdResizeNWSE); }
    inline const Cursor& columnResizeCursor()              { return enumeratedCursor(kCursorIdResizeColumn); }
    inline const Cursor& rowResizeCursor()                 { return enumeratedCursor(kCursorIdResizeRow); }
    inline const Cursor& verticalTextCursor()              { return enumeratedCursor(kCursorIdVerticalText); }
    inline const Cursor& cellCursor()                      { return enumeratedCursor(kCursorIdCell); }
    inline const Cursor& contextMenuCursor()               { return enumeratedCursor(kCursorIdContextMenu); }
    inline const Cursor& noDropCursor()                    { return enumeratedCursor(kCursorIdNoDrop); }
    inline const Cursor& notAllowedCursor()                { return enumeratedCursor(kCursorIdNotAllowed); }
    inline const Cursor& progressCursor()                  { return enumeratedCursor(kCursorIdProgress); }
    inline const Cursor& aliasCursor()                     { return enumeratedCursor(kCursorIdZAlias); }
    inline const Cursor& zoomInCursor()                    { return enumeratedCursor(kCursorIdZoomIn); }
    inline const Cursor& zoomOutCursor()                   { return enumeratedCursor(kCursorIdZoomOut); }
    inline const Cursor& copyCursor()                      { return enumeratedCursor(kCursorIdCopy); }
    inline const Cursor& noneCursor()                      { return enumeratedCursor(kCursorIdNone); }


} // namespace WebCore


#endif // Cursor_h
