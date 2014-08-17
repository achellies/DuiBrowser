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
// BCPlatformKeyboardEventEA.cpp
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "PlatformKeyboardEvent.h"
#include "KeyboardCodes.h"
#include "BALBase.h"
#include "TextEncoding.h"
#include "Unicode.h"
#include "CString.h"
#include <EAWebKit/EAWebKitInput.h>


namespace WKAL {


static WebCore::String keyIdentifierForEAKeyCode(uint32_t keyCode)
{
    using namespace EA::WebKit;

    switch (keyCode)
    {
        case kAlt:
        case kLAlt:
        case kRAlt:
            return "Alt";
        case kEnd:
            return "End";
		case kNewLine:
		case kCarriageReturn:
            return "Enter";
        case kNumpadEnter:
            return "Enter";
        case kF1:
            return "F1";
        case kF2:
            return "F2";
        case kF3:
            return "F3";
        case kF4:
            return "F4";
        case kF5:
            return "F5";
        case kF6:
            return "F6";
        case kF7:
            return "F7";
        case kF8:
            return "F8";
        case kF9:
            return "F9";
        case kF10:
            return "F10";
        case kF11:
            return "F11";
        case kF12:
            return "F12";
        case kF13:
            return "F13";
        case kF14:
            return "F14";
        case kF15:
            return "F15";
        case kHelp:
            return "Help";
        case kHome:
            return "Home";
        case kInsert:
            return "Insert";
        case kPageDown:
            return "PageDown";
        case kPageUp:
            return "PageUp";
        case kPause:
            return "Pause";
        case kPrintScreen:
            return "PrintScreen";
        case kArrowLeft:
            return "Left";
        case kArrowDown:
            return "Down";
        case kArrowRight:
            return "Right";
        case kArrowUp:
            return "Up";
        case kDelete:
            return "U+00007F";
        default:
            return WebCore::String::format("U+%04X", WTF::Unicode::toUpper(keyCode));
    }
}


// Here we convert out event data format to WebCore event data format.
// WebCore has an internal PlatformKeyboardEvent class which some components use 
// to handle mouse wheel usage. In theory we could define our BAL (Browser 
// Abstraction Layer) BalEventKey to be the same thing as the WebKit 
// PlatformKeyboardEvent, though doing so would result in a more complicated
// struct and WebKit/OWB is somewhat designed to have this layering and 
// we'll be more future compatible if we just go with it.

PlatformKeyboardEvent::PlatformKeyboardEvent(BalEventKey* pEventKey)
{
    using namespace EA::WebKit;

    if(pEventKey->mbChar)
        m_type = Char;
    else
        m_type = (pEventKey->mbDepressed) ? RawKeyDown : KeyUp;

    m_autoRepeat = false;

    if(pEventKey->mbChar)
        m_windowsVirtualKeyCode = 0;
    else
        m_windowsVirtualKeyCode = pEventKey->mId;  // It turns out that these are mapped identically.
    
    m_isKeypad  = !pEventKey->mbChar && (((pEventKey->mId >= kNumpad0) && (pEventKey->mId <= kNumpadDivide)) || (pEventKey->mId == kNumpadEnter) || (pEventKey->mId == kNumpadCenter));
    m_shiftKey  = pEventKey->mbShift;
    m_ctrlKey   = pEventKey->mbControl;
    m_altKey    = pEventKey->mbAlt;
    m_metaKey   = pEventKey->mbOS;

    m_balEventKey = pEventKey;

    // 4/13/09 CSidhall - Added check for (pEventKey->mbDepressed) as bug fix to select a link with the enter key.
    // The defaultEventHandler in HTMLAnchorElement.cpp (around line 179) uses  the "Enter" string to check for the trigger but 
    // it was only set when (pEnteKey->mbChar == true) however the EventHandler::keyEvent() in EventHandler.cpp exits when the m_type == char 
    // so the key "enter" was never detected.
    // So we need to set the m_keyidentifier here even when mbChar is a physical key (this "enter" detection might be the only case it needs it).   
    // 7/20/09 Seems the string ref counts are not init correctly if we leave them as such without copy str
    if((pEventKey->mbChar) || (pEventKey->mbDepressed)) 
    {
		uint32_t id = pEventKey->mId;
   
		UChar str[2] = { id, 0 };
		m_text           = str;
		m_unmodifiedText = str;
		m_keyIdentifier  = keyIdentifierForEAKeyCode(id);
    }
}


void PlatformKeyboardEvent::disambiguateKeyDownEvent(Type type, bool backwardCompatibilityMode)
{
    // Can only change type from KeyDown to RawKeyDown or Char, as we lack information for other conversions.
    ASSERT(m_type == KeyDown);
    m_type = type;

    if (!backwardCompatibilityMode)
    {
        if (type == RawKeyDown)
        {
            m_text.truncate(0);
            m_unmodifiedText.truncate(0);
        }
        else
        {
            m_keyIdentifier.truncate(0);
            m_windowsVirtualKeyCode = 0;
        }
    }
}


bool PlatformKeyboardEvent::currentCapsLockState()
{
    // To do: Implement this. The easiest way may be to simply write platform-specific code here.
    return false;
}


BalEventKey* PlatformKeyboardEvent::balEventKey() const
{
    return m_balEventKey;
}

} // namespace WKAL
