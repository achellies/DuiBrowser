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
#include "WebElementPropertyBag.h"

#include "WebFrame.h"
#include <Document.h>
#include <Frame.h>
#include <HitTestResult.h>
#include <FrameLoader.h>
#include <Image.h>
#include <KURL.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

WebElementPropertyBag::WebElementPropertyBag(const HitTestResult& result)
    : m_result(new HitTestResult(result))
{
    DS_CONSTRUCT();
}

WebElementPropertyBag::~WebElementPropertyBag()
{
    DS_DESTRUCT();
}

WebElementPropertyBag* WebElementPropertyBag::createInstance(const HitTestResult& result)
{
    WebElementPropertyBag* instance = new WebElementPropertyBag(result); 

    return instance;
}



String WebElementPropertyBag::Read(String pszPropName)
{
    /*if (!m_result)
        return String();

    String key = pszPropName;
    if (WebElementDOMNodeKey == key) {
        IDOMNode* node = DOMNode::createInstance(m_result->innerNonSharedNode());
        V_VT(pVar) = VT_UNKNOWN;
        V_UNKNOWN(pVar) = node;
        return S_OK;
    } else if (isEqual(WebElementFrameKey, key)) {
        if (!(m_result->innerNonSharedNode() && m_result->innerNonSharedNode()->document()
           && m_result->innerNonSharedNode()->document()->frame()))
            return E_FAIL;
        Frame* coreFrame = m_result->innerNonSharedNode()->document()->frame();
        WebFrame* webFrame = static_cast<WebFrame*>(coreFrame->loader()->client());
        IWebFrame* iWebFrame;
        if (FAILED(webFrame->QueryInterface(IID_IWebFrame, (void**)&iWebFrame)))
            return E_FAIL;
        V_VT(pVar) = VT_UNKNOWN;
        V_UNKNOWN(pVar) = iWebFrame;
        return S_OK;
    } else if (isEqual(WebElementImageAltStringKey, key))
        return convertStringToVariant(pVar, m_result->altDisplayString());
    else if (isEqual(WebElementImageKey, key)) {
        V_VT(pVar) = VT_BYREF;
        V_BYREF(pVar) = m_result->image();
        return S_OK;
    } else if (isEqual(WebElementImageRectKey, key)) {
        V_VT(pVar) = VT_ARRAY;
        V_ARRAY(pVar) = MarshallingHelpers::intRectToSafeArray(m_result->boundingBox());
        return S_OK;
    } else if (isEqual(WebElementImageURLKey, key))
        return convertStringToVariant(pVar, m_result->absoluteImageURL().string());
    else if (isEqual(WebElementIsSelectedKey, key)) {
        V_VT(pVar) = VT_BOOL;
        if (m_result->isSelected())
            V_BOOL(pVar) = VARIANT_TRUE;
        else
            V_BOOL(pVar) = VARIANT_FALSE;
        return S_OK;
    } else if (isEqual(WebElementSpellingToolTipKey, key))
        return convertStringToVariant(pVar, m_result->spellingToolTip());
    else if (isEqual(WebElementTitleKey, key))
        return convertStringToVariant(pVar, m_result->title());
    else if (isEqual(WebElementLinkURLKey, key))
        return convertStringToVariant(pVar, m_result->absoluteLinkURL().string());
    else if (isEqual(WebElementLinkTargetFrameKey, key)) {
        if (!m_result->targetFrame())
            return E_FAIL;
        WebFrame* webFrame = kit(m_result->targetFrame());
        IWebFrame* iWebFrame;
        if (FAILED(webFrame->QueryInterface(IID_IWebFrame, (void**)&iWebFrame)))
            return E_FAIL;
        V_VT(pVar) = VT_UNKNOWN;
        V_UNKNOWN(pVar) = iWebFrame;
        return S_OK;
    } else if (isEqual(WebElementLinkTitleKey, key))
        return convertStringToVariant(pVar, m_result->titleDisplayString());
    else if (isEqual(WebElementLinkLabelKey, key))
        return convertStringToVariant(pVar, m_result->textContent());
    else if (isEqual(WebElementIsContentEditableKey, key)) {
        V_VT(pVar) = VT_BOOL;
        if (m_result->isContentEditable())
            V_BOOL(pVar) = VARIANT_TRUE;
        else
            V_BOOL(pVar) = VARIANT_FALSE;
        return S_OK;
    }

    return E_INVALIDARG;*/
    return String();
}

