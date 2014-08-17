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
// BCPlatformWheelEventEA.cpp
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "PlatformWheelEvent.h"


namespace WKAL {

// Here we convert out event data format to WebCore event data format.
// WebCore has an internal PlatformWheelEvent class which some components use 
// to handle mouse wheel usage. In theory we could define our BAL (Browser 
// Abstraction Layer) BalEventScroll to be the same thing as the WebKit 
// PlatformWheelEvent, though doing so would result in a more complicated
// struct and WebKit/OWB is somewhat designed to have this layering and 
// we'll be more future compatible if we just go with it.

PlatformWheelEvent::PlatformWheelEvent(BalEventScroll* pEventWheel)
{
	// Note by Gautam Narain: On Safari 3.1.2 we see that shift key causes page to scroll left and right. 
	// So we will follow suit and make it work exactly like Safari 3.1.2 does. 
	m_deltaX         = pEventWheel->mbShift == true ? pEventWheel->mLineDelta : 0.0f;  
    m_deltaY         = pEventWheel->mbShift == false ? pEventWheel->mLineDelta : 0.0f; 
    m_shiftKey       = pEventWheel->mbShift;
    m_ctrlKey        = pEventWheel->mbControl;
    m_altKey         = pEventWheel->mbAlt;
    m_metaKey        = pEventWheel->mbOS;
    m_position       = IntPoint((int)pEventWheel->mX, (int)pEventWheel->mY);
    m_globalPosition = IntPoint((int)pEventWheel->mX, (int)pEventWheel->mY);
    m_isAccepted     = false;
    m_isContinuous   = false;
}

}

