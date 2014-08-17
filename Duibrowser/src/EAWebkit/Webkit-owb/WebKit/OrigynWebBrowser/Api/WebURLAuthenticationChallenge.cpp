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
#include "WebURLAuthenticationChallenge.h"

#include "WebError.h"
#include "WebURLAuthenticationChallengeSender.h"
#include "WebURLCredential.h"
#include "WebURLProtectionSpace.h"
#include "WebURLResponse.h"

#include <PlatformString.h>
#include <ResourceHandle.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

WebURLAuthenticationChallenge::WebURLAuthenticationChallenge(const AuthenticationChallenge& authenticationChallenge, WebURLAuthenticationChallengeSender* sender)
    : m_authenticationChallenge(authenticationChallenge)
    , m_sender(sender)
{
    DS_CONSTRUCT();
}

WebURLAuthenticationChallenge::~WebURLAuthenticationChallenge()
{
    delete m_sender;
    DS_DESTRUCT();
}

WebURLAuthenticationChallenge* WebURLAuthenticationChallenge::createInstance(const AuthenticationChallenge& authenticationChallenge)
{
    WebURLAuthenticationChallenge* instance = new WebURLAuthenticationChallenge(authenticationChallenge, 0);
    return instance;
}

WebURLAuthenticationChallenge* WebURLAuthenticationChallenge::createInstance(const AuthenticationChallenge& authenticationChallenge, WebURLAuthenticationChallengeSender* sender)
{
    WebURLAuthenticationChallenge* instance = new WebURLAuthenticationChallenge(authenticationChallenge, sender);
    return instance;
}

void WebURLAuthenticationChallenge::initWithProtectionSpace(WebURLProtectionSpace* space, WebURLCredential* proposedCredential, int previousFailureCount, WebURLResponse* failureResponse, WebError* error, WebURLAuthenticationChallengeSender* sender)
{
    //LOG_ERROR("Calling the ala carte init for WebURLAuthenticationChallenge - is this really what you want to do?");

    // FIXME: After we change AuthenticationChallenge to use "ResourceHandle" as the abstract "Sender" or "Source of this Auth Challenge", then we'll
    // construct the AuthenticationChallenge with that as obtained from the webSender

    m_authenticationChallenge = AuthenticationChallenge(space->protectionSpace(), proposedCredential->credential(), previousFailureCount, failureResponse->resourceResponse(), error->resourceError());
}

void WebURLAuthenticationChallenge::initWithAuthenticationChallenge(WebURLAuthenticationChallenge* challenge, WebURLAuthenticationChallengeSender* sender)
{
/*#if USE(CFNETWORK)
    m_authenticationChallenge = AuthenticationChallenge(webChallenge->authenticationChallenge().cfURLAuthChallengeRef(), webSender->resourceHandle());

    return S_OK;
#else*/

}

WebError* WebURLAuthenticationChallenge::error()
{
    return WebError::createInstance(m_authenticationChallenge.error());
}

WebURLResponse* WebURLAuthenticationChallenge::failureResponse()
{
    return WebURLResponse::createInstance(m_authenticationChallenge.failureResponse());
}

unsigned WebURLAuthenticationChallenge::previousFailureCount()
{
    return m_authenticationChallenge.previousFailureCount();
}

WebURLCredential* WebURLAuthenticationChallenge::proposedCredential()
{
    return WebURLCredential::createInstance(m_authenticationChallenge.proposedCredential());
}

WebURLProtectionSpace* WebURLAuthenticationChallenge::protectionSpace()
{
    return WebURLProtectionSpace::createInstance(m_authenticationChallenge.protectionSpace());
}

WebURLAuthenticationChallengeSender* WebURLAuthenticationChallenge::sender()
{
    if (!m_sender) {
        ResourceHandle* handle = m_authenticationChallenge.sourceHandle();
        m_sender = WebURLAuthenticationChallengeSender::createInstance(handle);
    }

    return m_sender;
}

const AuthenticationChallenge& WebURLAuthenticationChallenge::authenticationChallenge() const
{
    return m_authenticationChallenge;
}
