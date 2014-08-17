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

#ifndef PlatformMouseEvent_h
#define PlatformMouseEvent_h

#include <wtf/FastAllocBase.h>
#include "IntPoint.h"
#include <wtf/Platform.h>
#include "BALBase.h"

namespace WKAL {
    
    // These button numbers match the ones used in the DOM API, 0 through 2, except for NoButton which isn't specified.
    enum MouseButton { NoButton = -1, LeftButton, MiddleButton, RightButton };
    enum MouseEventType { MouseEventMoved, MouseEventPressed, MouseEventReleased, MouseEventScroll };
    
    class PlatformMouseEvent/*: public WTF::FastAllocBase*/ {
    public:
        PlatformMouseEvent()
            : m_button(NoButton)
            , m_eventType(MouseEventMoved)
            , m_clickCount(0)
            , m_shiftKey(false)
            , m_ctrlKey(false)
            , m_altKey(false)
            , m_metaKey(false)
            , m_timestamp(0)
            , m_modifierFlags(0)
        {
        }

        PlatformMouseEvent(const IntPoint& pos, const IntPoint& globalPos, MouseButton button, MouseEventType eventType,
                           int clickCount, bool shift, bool ctrl, bool alt, bool meta, double timestamp)
            : m_position(pos), m_globalPosition(globalPos), m_button(button)
            , m_eventType(eventType)
            , m_clickCount(clickCount)
            , m_shiftKey(shift)
            , m_ctrlKey(ctrl)
            , m_altKey(alt)
            , m_metaKey(meta)
            , m_timestamp(timestamp)
            , m_modifierFlags(0)
        {
        }

        const IntPoint& pos() const { return m_position; }
        int x() const { return m_position.x(); }
        int y() const { return m_position.y(); }
        int globalX() const { return m_globalPosition.x(); }
        int globalY() const { return m_globalPosition.y(); }
        MouseButton button() const { return m_button; }
        MouseEventType eventType() const { return m_eventType; }
        int clickCount() const { return m_clickCount; }
        bool shiftKey() const { return m_shiftKey; }
        bool ctrlKey() const { return m_ctrlKey; }
        bool altKey() const { return m_altKey; }
        bool metaKey() const { return m_metaKey; }
        unsigned modifierFlags() const { return m_modifierFlags; }
        
        //time in seconds
        double timestamp() const { return m_timestamp; }

        PlatformMouseEvent(const BalEventButton*);
        PlatformMouseEvent(const BalEventMotion*, int);

    private:
        IntPoint m_position;
        IntPoint m_globalPosition;
        MouseButton m_button;
        MouseEventType m_eventType;
        int m_clickCount;
        bool m_shiftKey;
        bool m_ctrlKey;
        bool m_altKey;
        bool m_metaKey;
        double m_timestamp; // unit: seconds
        unsigned m_modifierFlags;
    };

} // namespace WebCore

#endif // PlatformMouseEvent_h
