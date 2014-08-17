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
#include "WebURLProtectionSpace.h"

#include <PlatformString.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

WebURLProtectionSpace::WebURLProtectionSpace(const ProtectionSpace& protectionSpace)
    : m_protectionSpace(protectionSpace)
{
    DS_CONSTRUCT();
}

WebURLProtectionSpace::~WebURLProtectionSpace()
{
    DS_DESTRUCT();
}

WebURLProtectionSpace* WebURLProtectionSpace::createInstance()
{
    WebURLProtectionSpace* instance = new WebURLProtectionSpace(ProtectionSpace());
    return instance;
}

WebURLProtectionSpace* WebURLProtectionSpace::createInstance(const ProtectionSpace& protectionSpace)
{
    WebURLProtectionSpace* instance = new WebURLProtectionSpace(protectionSpace);
    return instance;
}

WebCore::String WebURLProtectionSpace::authenticationMethod()
{
    switch (m_protectionSpace.authenticationScheme()) {
    case ProtectionSpaceAuthenticationSchemeDefault:
        return WebURLAuthenticationMethodDefault;
        break;
    case ProtectionSpaceAuthenticationSchemeHTTPBasic:
        return WebURLAuthenticationMethodHTTPBasic;
        break;
    case ProtectionSpaceAuthenticationSchemeHTTPDigest:
        return WebURLAuthenticationMethodHTTPDigest;
        break;
    case ProtectionSpaceAuthenticationSchemeHTMLForm:
        return WebURLAuthenticationMethodHTMLForm;
        break;
    default:
        ASSERT_NOT_REACHED();
        return String();
    }
    return String();
}

String WebURLProtectionSpace::host()
{
    return m_protectionSpace.host();
}

static ProtectionSpaceAuthenticationScheme coreScheme(String authenticationMethod)
{
    ProtectionSpaceAuthenticationScheme scheme = ProtectionSpaceAuthenticationSchemeDefault;
    if (authenticationMethod == WebURLAuthenticationMethodDefault)
        scheme = ProtectionSpaceAuthenticationSchemeDefault;
    else if (authenticationMethod == WebURLAuthenticationMethodHTTPBasic)
        scheme = ProtectionSpaceAuthenticationSchemeHTTPBasic;
    else if (authenticationMethod == WebURLAuthenticationMethodHTTPDigest)
        scheme = ProtectionSpaceAuthenticationSchemeHTTPDigest;
    else if (authenticationMethod == WebURLAuthenticationMethodHTMLForm)
        scheme = ProtectionSpaceAuthenticationSchemeHTMLForm;
    else
        ASSERT_NOT_REACHED();
    return scheme;
}

void WebURLProtectionSpace::initWithHost(String host, int port, String protocol, String realm, String authenticationMethod)
{
    ProtectionSpaceServerType serverType = ProtectionSpaceServerHTTP;
    if (protocol == WebURLProtectionSpaceHTTP)
        serverType = ProtectionSpaceServerHTTP;
    else if (protocol == WebURLProtectionSpaceHTTPS)
        serverType = ProtectionSpaceServerHTTPS;
    else if (protocol == WebURLProtectionSpaceFTP)
        serverType = ProtectionSpaceServerFTP;
    else if (protocol == WebURLProtectionSpaceFTPS)
        serverType = ProtectionSpaceServerFTPS;
    else
        ASSERT_NOT_REACHED();

    m_protectionSpace = ProtectionSpace(host, port, serverType, realm, coreScheme(authenticationMethod));
}

void WebURLProtectionSpace::initWithProxyHost(String host, int port, String proxyType, String realm, String authenticationMethod)
{
    ProtectionSpaceServerType serverType = ProtectionSpaceProxyHTTP;
    if (proxyType == WebURLProtectionSpaceHTTPProxy)
        serverType = ProtectionSpaceProxyHTTP;
    else if (proxyType == WebURLProtectionSpaceHTTPSProxy)
        serverType = ProtectionSpaceProxyHTTPS;
    else if (proxyType == WebURLProtectionSpaceFTPProxy)
        serverType = ProtectionSpaceProxyFTP;
    else if (proxyType == WebURLProtectionSpaceSOCKSProxy)
        serverType = ProtectionSpaceProxySOCKS;
    else
        ASSERT_NOT_REACHED();

    m_protectionSpace = ProtectionSpace(host, port, serverType, realm, coreScheme(authenticationMethod));

}

bool WebURLProtectionSpace::isProxy()
{
    return m_protectionSpace.isProxy();
}

int WebURLProtectionSpace::port()
{
    return m_protectionSpace.port();
}

String WebURLProtectionSpace::protocol()
{
    switch (m_protectionSpace.serverType()) {
    case ProtectionSpaceServerHTTP:
        return WebURLProtectionSpaceHTTP;
        break;
    case ProtectionSpaceServerHTTPS:
        return WebURLProtectionSpaceHTTPS;
        break;
    case ProtectionSpaceServerFTP:
        return WebURLProtectionSpaceFTP;
        break;
    case ProtectionSpaceServerFTPS:
        return WebURLProtectionSpaceFTPS;
        break;
    default:
        ASSERT_NOT_REACHED();
        return String();
    }
    return String();
}

String WebURLProtectionSpace::proxyType()
{
    switch (m_protectionSpace.serverType()) {
    case ProtectionSpaceProxyHTTP:
        return WebURLProtectionSpaceHTTPProxy;
        break;
    case ProtectionSpaceProxyHTTPS:
        return WebURLProtectionSpaceHTTPSProxy;
        break;
    case ProtectionSpaceProxyFTP:
        return WebURLProtectionSpaceFTPProxy;
        break;
    case ProtectionSpaceProxySOCKS:
        return WebURLProtectionSpaceSOCKSProxy;
        break;
    default:
        ASSERT_NOT_REACHED();
        return String();
    }
    return String();
}

String WebURLProtectionSpace::realm()
{
    return m_protectionSpace.realm();
}

bool WebURLProtectionSpace::receivesCredentialSecurely()
{
    return m_protectionSpace.receivesCredentialSecurely();
}

const ProtectionSpace& WebURLProtectionSpace::protectionSpace() const
{
    return m_protectionSpace;
}
