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

#ifndef WebURLProtectionSpace_h
#define WebURLProtectionSpace_h

#include <wtf/FastAllocBase.h>

/**
 *  @file  WebURLProtectionSpace.h
 *  WebURLProtectionSpace description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include <ProtectionSpace.h>
#include <ProtectionSpace.h>

namespace WebCore {
    class String;
}

#define WebURLProtectionSpaceHTTP "http"
#define WebURLProtectionSpaceHTTPS "https"
#define WebURLProtectionSpaceFTP "ftp"
#define WebURLProtectionSpaceFTPS "ftps"
#define WebURLProtectionSpaceHTTPProxy "http"
#define WebURLProtectionSpaceHTTPSProxy "https"
#define WebURLProtectionSpaceFTPProxy "ftp"
#define WebURLProtectionSpaceSOCKSProxy "SOCKS"
#define WebURLAuthenticationMethodDefault "WebURLAuthenticationMethodDefault"
#define WebURLAuthenticationMethodHTTPBasic "WebURLAuthenticationMethodHTTPBasic"
#define WebURLAuthenticationMethodHTTPDigest "WebURLAuthenticationMethodHTTPDigest"
#define WebURLAuthenticationMethodHTMLForm "WebURLAuthenticationMethodHTMLForm"


class WebURLProtectionSpace/*: public WTF::FastAllocBase*/
{
public:

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebURLProtectionSpace* createInstance();

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebURLProtectionSpace* createInstance(const WebCore::ProtectionSpace&);
private:

    /**
     *  WebURLProtectionSpace description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    WebURLProtectionSpace(const WebCore::ProtectionSpace&);
public:

    /**
     *  ~WebURLProtectionSpace description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual ~WebURLProtectionSpace();

    /**
     *  authenticationMethod description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String authenticationMethod();

    /**
     *  host description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String host();

    /**
     *  initWithHost description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void initWithHost(WebCore::String host, int port, WebCore::String protocol, WebCore::String realm,  WebCore::String authenticationMethod);

    /**
     *  initWithProxyHost description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void initWithProxyHost(WebCore::String host, int port, WebCore::String proxyType, WebCore::String realm, WebCore::String authenticationMethod);

    /**
     *  isProxy description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual bool isProxy();

    /**
     *  port description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual int port();

    /**
     *  protocol description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String protocol();

    /**
     *  proxyType description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String proxyType();

    /**
     *  realm description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String realm();

    /**
     *  receivesCredentialSecurely description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual bool receivesCredentialSecurely();

    /**
     * protectionSpace description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    const WebCore::ProtectionSpace& protectionSpace() const;

protected:
    WebCore::ProtectionSpace m_protectionSpace;
};


#endif
