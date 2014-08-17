/*
 * Copyright (C) 2008 Pleyo.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Pleyo nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY PLEYO AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PLEYO OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/
#include "config.h"

#include "WebActionPropertyBag.h"
#include "WebElementPropertyBag.h"

#include <PlatformString.h>
#include <EventHandler.h>
#include <MouseEvent.h>
#include <HitTestResult.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

WebActionPropertyBag::WebActionPropertyBag(const NavigationAction& action, Frame* frame)
    : m_action(action) 
    , m_frame(frame)
{
    DS_CONSTRUCT();
}

WebActionPropertyBag::~WebActionPropertyBag()
{
    m_frame = 0;
    DS_DESTRUCT();
}

WebActionPropertyBag* WebActionPropertyBag::createInstance(const NavigationAction& action, Frame* frame)
{
    WebActionPropertyBag* instance = new WebActionPropertyBag(action, frame); 

    return instance;
}


/*static const MouseEvent* findMouseEvent(const Event* event)
{
    for (const Event* e = event; e; e = e->underlyingEvent())
        if (e->isMouseEvent())
            return static_cast<const MouseEvent*>(e);
    return 0;
}*/

String WebActionPropertyBag::Read(String pszPropName)
{
    /*if (pszPropName == WebActionNavigationTypeKey) {
        V_VT(pVar) = VT_I4;
        V_I4(pVar) = m_action.type();
        return S_OK;
    } else if (isEqual(pszPropName, WebActionElementKey)) {
        if (const MouseEvent* mouseEvent = findMouseEvent(m_action.event())) {
            IntPoint point(mouseEvent->clientX(), mouseEvent->clientY());
            V_VT(pVar) = VT_UNKNOWN;
            V_UNKNOWN(pVar) = WebElementPropertyBag::createInstance(m_frame->eventHandler()->hitTestResultAtPoint(point, false));
            return S_OK;
        }
    } else if (isEqual(pszPropName, WebActionButtonKey)) {
        if (const MouseEvent* mouseEvent = findMouseEvent(m_action.event())) {
            V_VT(pVar) = VT_I4;
            V_I4(pVar) = mouseEvent->button();
            return S_OK;
        }
    } else if (isEqual(pszPropName, WebActionOriginalURLKey)) {
        V_VT(pVar) = VT_BSTR;
        V_BSTR(pVar) = BString(m_action.url().string()).release();
        return S_OK;
    } else if (isEqual(pszPropName, WebActionModifierFlagsKey)) {
        if (const UIEventWithKeyState* keyEvent = findEventWithKeyState(const_cast<Event*>(m_action.event()))) {
            int modifiers = 0;

            if (keyEvent->ctrlKey())
                modifiers |= MK_CONTROL;
            if (keyEvent->shiftKey())
                modifiers |= MK_SHIFT;
            if (keyEvent->altKey())
                modifiers |= MK_ALT;

            V_VT(pVar) = VT_I4;
            V_I4(pVar) = modifiers;
            return S_OK;
        }
    }
    return E_INVALIDARG;*/
    return String();
}

