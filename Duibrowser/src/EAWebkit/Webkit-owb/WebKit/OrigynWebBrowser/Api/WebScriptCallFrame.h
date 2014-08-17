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

#ifndef WebScriptCallFrame_h
#define WebScriptCallFrame_h

#include <wtf/FastAllocBase.h>

/**
 *  @file  WebScriptCallFrame.h
 *  WebScriptCallFrame description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include "PropertyNameArray.h"
#include "PropertyNameArray.h"

namespace KJS {
    class ExecState;
    class JSValue;
    class UString;
}

namespace WebCore {
    class String;
}

class WebScriptCallFrame/*: public WTF::FastAllocBase*/ {
public:

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebScriptCallFrame* createInstance(KJS::ExecState*);

private:

    /**
     *  WebScriptCallFrame description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    WebScriptCallFrame(KJS::ExecState*);

public:

    /**
     *  ~WebScriptCallFrame description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual ~WebScriptCallFrame();

    /**
     *  caller description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebScriptCallFrame* caller();

    /**
     *  functionName description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String functionName();

    /**
     *  stringByEvaluatingJavaScriptFromString description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String stringByEvaluatingJavaScriptFromString(WebCore::String script);

    /**
     *  variableNames description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual KJS::PropertyNameArray variableNames();

    /**
     *  valueForVariable description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String valueForVariable(WebCore::String key);


    /**
     *  valueByEvaluatingJavaScriptFromString description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual KJS::JSValue* valueByEvaluatingJavaScriptFromString(WebCore::String script);

    /**
     * state description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual KJS::ExecState* state() const { return m_state; }


    /**
     *  jsValueToString description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static KJS::UString jsValueToString(KJS::ExecState*, KJS::JSValue*);

private:

    KJS::ExecState* m_state;
};

#endif
