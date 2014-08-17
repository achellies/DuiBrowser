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

#ifndef PlatformWheelEvent_h
#define PlatformWheelEvent_h

#include "IntPoint.h"
#include "BALBase.h"


namespace WKAL {

    class PlatformWheelEvent : public WKALBase {
    public:
        const IntPoint& pos() const { return m_position; }
        const IntPoint& globalPos() const { return m_globalPosition; }

        float deltaX() const { return m_deltaX; }
        float deltaY() const { return m_deltaY; }

        bool isAccepted() const { return m_isAccepted; }
        bool shiftKey() const { return m_shiftKey; }
        bool ctrlKey() const { return m_ctrlKey; }
        bool altKey() const { return m_altKey; }
        bool metaKey() const { return m_metaKey; }

        int x() const { return m_position.x(); }
        int y() const { return m_position.y(); }
        int globalX() const { return m_globalPosition.x(); }
        int globalY() const { return m_globalPosition.y(); }

        void accept() { m_isAccepted = true; }
        void ignore() { m_isAccepted = false; }
        
        bool isContinuous() const { return m_isContinuous; }
        float continuousDeltaX() const { return m_continuousDeltaX; }
        float continuousDeltaY() const { return m_continuousDeltaY; }

        PlatformWheelEvent(BalEventScroll*);

    private:
        IntPoint m_position;
        IntPoint m_globalPosition;
        float m_deltaX;
        float m_deltaY;
        bool m_isAccepted;
        bool m_shiftKey;
        bool m_ctrlKey;
        bool m_altKey;
        bool m_metaKey;
        bool m_isContinuous;
        float m_continuousDeltaX;
        float m_continuousDeltaY;
    };

} // namespace WKAL

#endif // PlatformWheelEvent_h
