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

#ifndef WebURLAuthenticationChallenge_h
#define WebURLAuthenticationChallenge_h

#include <wtf/FastAllocBase.h>

/**
 *  @file  WebURLAuthenticationChallenge.h
 *  WebURLAuthenticationChallenge description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include <AuthenticationChallenge.h>
#include <AuthenticationChallenge.h>

class WebURLProtectionSpace;
class WebURLCredential;
class WebURLResponse;
class WebError;
class WebURLAuthenticationChallengeSender;

class WebURLAuthenticationChallenge/*: public WTF::FastAllocBase*/ {
public:

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebURLAuthenticationChallenge* createInstance(const WebCore::AuthenticationChallenge&);

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebURLAuthenticationChallenge* createInstance(const WebCore::AuthenticationChallenge&, WebURLAuthenticationChallengeSender*);
private:

    /**
     *  WebURLAuthenticationChallenge description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    WebURLAuthenticationChallenge(const WebCore::AuthenticationChallenge&, WebURLAuthenticationChallengeSender*);
public:

    /**
     *  ~WebURLAuthenticationChallenge description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual ~WebURLAuthenticationChallenge();

    /**
     *  initWithProtectionSpace description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void initWithProtectionSpace(WebURLProtectionSpace* space, WebURLCredential* proposedCredential, int previousFailureCount, WebURLResponse* failureResponse, WebError* error, WebURLAuthenticationChallengeSender* sender);

    /**
     *  initWithAuthenticationChallenge description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void initWithAuthenticationChallenge(WebURLAuthenticationChallenge* challenge, WebURLAuthenticationChallengeSender* sender);

    /**
     *  error description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebError* error();

    /**
     *  failureResponse description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebURLResponse* failureResponse();

    /**
     *  previousFailureCount description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual unsigned previousFailureCount();

    /**
     *  proposedCredential description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebURLCredential* proposedCredential();

    /**
     *  protectionSpace description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebURLProtectionSpace* protectionSpace();

    /**
     *  sender description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebURLAuthenticationChallengeSender* sender();
   
    /**
     * authenticationChallenge description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */ 
    const WebCore::AuthenticationChallenge& authenticationChallenge() const;

protected:
    WebCore::AuthenticationChallenge m_authenticationChallenge;
    WebURLAuthenticationChallengeSender* m_sender;
};


#endif
