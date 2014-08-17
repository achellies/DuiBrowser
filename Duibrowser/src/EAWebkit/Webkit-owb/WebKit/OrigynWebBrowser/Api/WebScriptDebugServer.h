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

#ifndef WebScriptDebugServer_H
#define WebScriptDebugServer_H

#include <wtf/FastAllocBase.h>


/**
 *  @file  WebScriptDebugServer.h
 *  WebScriptDebugServer description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include <JavaScriptDebugListener.h>
#include <JavaScriptDebugListener.h>
#include "PlatformString.h"

namespace WebCore {
    class Page;
}

class WebView;
class WebDataSource;
class WebFrame;
class WebScriptCallFrame;

class WebScriptDebugListener/*: public WTF::FastAllocBase*/ {
public:
    /**
     * didLoadMainResourceForDataSource description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    void didLoadMainResourceForDataSource(WebView* view, WebDataSource* dataSource) {};

    /**
     * didParseSource description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    void didParseSource(WebView* view, WebCore::String sourceCode, unsigned baseLineNumber, WebCore::String url, int sourceID, WebFrame* forWebFrame) {};

    /**
     * failedToParseSource description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    void failedToParseSource(WebView* view, WebCore::String sourceCode, unsigned baseLineNumber, WebCore::String url, WebCore::String error, WebFrame* forWebFrame) {};

    /**
     * didEnterCallFrame description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    void didEnterCallFrame(WebView* view, WebScriptCallFrame* frame, int sourceID, int lineNumber, WebFrame* forWebFrame) {};

    /**
     * willExecuteStatement description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    void willExecuteStatement(WebView* view, WebScriptCallFrame* frame, int sourceID, int lineNumber, WebFrame* forWebFrame) {};

    /**
     * willLeaveCallFrame description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    void willLeaveCallFrame(WebView* view, WebScriptCallFrame* frame, int sourceID, int lineNumber, WebFrame* forWebFrame) {};

    /**
     * exceptionWasRaised description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    void exceptionWasRaised(WebView* view, WebScriptCallFrame* frame, int sourceID, int lineNumber, WebFrame* forWebFrame) {};

    /**
     * serverDidDie description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    void serverDidDie() {};
};

class WebScriptDebugServer { //: public WebCore::JavaScriptDebugListener {
public:

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebScriptDebugServer* createInstance();

    /**
     *  sharedWebScriptDebugServer description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebScriptDebugServer* sharedWebScriptDebugServer();


    /**
     *  ~WebScriptDebugServer description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual ~WebScriptDebugServer();


    /**
     *  addListener description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void addListener(WebScriptDebugListener*);

    /**
     *  removeListener description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void removeListener(WebScriptDebugListener*);

    /**
     *  step description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void step();

    /**
     *  pause description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void pause();

    /**
     *  resume description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void resume();

    /**
     *  isPaused description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual bool isPaused();

private:

    /**
     *  WebScriptDebugServer description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    WebScriptDebugServer();


    bool m_paused;
    bool m_step;
    bool m_callingListeners;
};

#endif
