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
// BCPlatformMouseEventEA.cpp
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "PlatformMouseEvent.h"
#include "SystemTime.h"
#include "Assertions.h"
#include <EAWebKit/EAWebKitInput.h>


namespace WKAL {


// Here we convert out event data format to WebCore event data format.
// WebCore has an internal PlatformMouseEvent class which some components use 
// to handle mouse wheel usage. In theory we could define our BAL (Browser 
// Abstraction Layer) BalEventButton to be the same thing as the WebKit 
// PlatformMouseEvent, though doing so would result in a more complicated
// struct and WebKit/OWB is somewhat designed to have this layering and 
// we'll be more future compatible if we just go with it.

PlatformMouseEvent::PlatformMouseEvent(const BalEventButton* pEventButton)
{
    using namespace EA::WebKit;

    m_timestamp      = WebCore::currentTime();
    m_position       = IntPoint((int)pEventButton->mX, (int)pEventButton->mY);
    m_globalPosition = IntPoint((int)pEventButton->mX, (int)pEventButton->mY); // Does this need to be the screen-relative position as opposed to window-relative position?
    m_shiftKey       = pEventButton->mbShift;
    m_ctrlKey        = pEventButton->mbControl;
    m_altKey         = pEventButton->mbAlt;
    m_metaKey        = pEventButton->mbOS;

    if( pEventButton->mId == kMouseLeft )
        m_button = LeftButton;
    else if( pEventButton->mId == kMouseMiddle )
        m_button = MiddleButton;
    else if( pEventButton->mId == kMouseRight )
        m_button = RightButton;
    else
        m_button = NoButton;

    // To consider: Do we need to store static state so we know if a double-click occurred?
    //              Or does WebCore handle double-clicking? Looks like we need to do it.
    //              We should get out the old UTFWin double-click-state machine code. 
    //              Correctly handling double-clicking is not as easy as it might seem 
    //              on the surface.

    if(pEventButton->mbDepressed)
    {
        m_eventType  = MouseEventPressed;
        m_clickCount = 1;   
    }
    else
    {
        m_eventType  = MouseEventReleased;
        m_clickCount = 0;
    }
}



// Here we convert out event data format to WebCore event data format.
// We define an unused 'int' parameter so that this function doesn't conflict 
// with the other version when BalEventMotion == BalEventButton.

PlatformMouseEvent::PlatformMouseEvent(const BalEventMotion* pEventMotion, int)
{
    using namespace EA::WebKit;

    m_timestamp      = WebCore::currentTime();
    m_position       = IntPoint((int)pEventMotion->mX, (int)pEventMotion->mY);
    m_globalPosition = IntPoint((int)pEventMotion->mX, (int)pEventMotion->mY); // Does this need to be the screen-relative position as opposed to window-relative position?
    m_shiftKey       = pEventMotion->mbShift;
    m_ctrlKey        = pEventMotion->mbControl;
    m_altKey         = pEventMotion->mbAlt;
    m_metaKey        = pEventMotion->mbOS;
    m_eventType      = MouseEventMoved;
    m_button         = NoButton;
    m_clickCount     = 0;
}

} // namespace WKAL
