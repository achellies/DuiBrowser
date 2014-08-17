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
#include "WebURLCredential.h"

#include <PlatformString.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

WebURLCredential::WebURLCredential(const Credential& credential)
    : m_credential(credential)
{
    DS_CONSTRUCT();
}

WebURLCredential::~WebURLCredential()
{
    DS_DESTRUCT();
}

WebURLCredential* WebURLCredential::createInstance()
{
    WebURLCredential* instance = new WebURLCredential(Credential());
    return instance;
}

WebURLCredential* WebURLCredential::createInstance(const Credential& credential)
{
    WebURLCredential* instance = new WebURLCredential(credential);
    return instance;
}

bool WebURLCredential::hasPassword()
{
    return m_credential.hasPassword();
}

void WebURLCredential::initWithUser(String user, String password, WebURLCredentialPersistence persistence)
{
    CredentialPersistence corePersistence = CredentialPersistenceNone;
    switch (persistence) {
    case WebURLCredentialPersistenceNone:
        break;
    case WebURLCredentialPersistenceForSession:
        corePersistence = CredentialPersistenceForSession;
        break;
    case WebURLCredentialPersistencePermanent:
        corePersistence = CredentialPersistencePermanent;
        break;
    default:
        ASSERT_NOT_REACHED();
    }

    m_credential = Credential(user, password, corePersistence);
}

String WebURLCredential::password()
{
    return  m_credential.password();
}

WebURLCredentialPersistence WebURLCredential::persistence()
{
    switch (m_credential.persistence()) {
    case CredentialPersistenceNone:
        return WebURLCredentialPersistenceNone;
    case CredentialPersistenceForSession:
        return WebURLCredentialPersistenceForSession;
    case CredentialPersistencePermanent:
        return WebURLCredentialPersistencePermanent;
    default:
        ASSERT_NOT_REACHED();
    }
    return WebURLCredentialPersistenceNone;
}

String WebURLCredential::user()
{
    return m_credential.user();
}

const WebCore::Credential& WebURLCredential::credential() const
{
    return m_credential;
}

