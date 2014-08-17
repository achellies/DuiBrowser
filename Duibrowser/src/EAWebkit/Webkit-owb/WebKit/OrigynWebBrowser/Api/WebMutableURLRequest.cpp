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
#include "WebMutableURLRequest.h"

#include <PlatformString.h>
#include <CString.h>
#include <FormData.h>
#include <ResourceHandle.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

WebMutableURLRequest::WebMutableURLRequest(bool isMutable)
    : m_isMutable(isMutable)
{
    DS_CONSTRUCT();
}

WebMutableURLRequest* WebMutableURLRequest::createInstance()
{
    WebMutableURLRequest* instance = new WebMutableURLRequest(true);
    return instance;
}

WebMutableURLRequest* WebMutableURLRequest::createInstance(WebMutableURLRequest* req)
{
    WebMutableURLRequest* instance = new WebMutableURLRequest(true);
    instance->m_request = static_cast<WebMutableURLRequest*>(req)->m_request;
    return instance;
}

WebMutableURLRequest* WebMutableURLRequest::createInstance(const ResourceRequest& request)
{
    WebMutableURLRequest* instance = new WebMutableURLRequest(true);
    instance->m_request = request;
    return instance;
}

WebMutableURLRequest* WebMutableURLRequest::createImmutableInstance()
{
    WebMutableURLRequest* instance = new WebMutableURLRequest(false);
    return instance;
}

WebMutableURLRequest* WebMutableURLRequest::createImmutableInstance(const ResourceRequest& request)
{
    WebMutableURLRequest* instance = new WebMutableURLRequest(false);
    instance->m_request = request;
    return instance;
}

WebMutableURLRequest::~WebMutableURLRequest()
{
    DS_DESTRUCT();
}

void WebMutableURLRequest::requestWithURL(String /*theURL*/, WebCore::ResourceRequestCachePolicy /*cachePolicy*/, double /*timeoutInterval*/)
{
}

HTTPHeaderPropertyBag* WebMutableURLRequest::allHTTPHeaderFields()
{
    return 0;
}

WebCore::ResourceRequestCachePolicy WebMutableURLRequest::cachePolicy()
{
    return m_request.cachePolicy();
}

String WebMutableURLRequest::HTTPBody()
{
    return String();
}

String WebMutableURLRequest::HTTPBodyStream()
{
    return String();
}

String WebMutableURLRequest::HTTPMethod()
{
    return m_request.httpMethod();
}

bool WebMutableURLRequest::HTTPShouldHandleCookies()
{
    return false;
}

void WebMutableURLRequest::initWithURL(String url, ResourceRequestCachePolicy cachePolicy, double timeoutInterval)
{
    m_request.setURL(KURL(url));
    m_request.setCachePolicy(cachePolicy);
    m_request.setTimeoutInterval(timeoutInterval);
}

String WebMutableURLRequest::mainDocumentURL()
{
    return m_request.url();
}

double WebMutableURLRequest::timeoutInterval()
{
    return m_request.timeoutInterval();
}

String WebMutableURLRequest::URL()
{
    return m_request.url();
}

String WebMutableURLRequest::valueForHTTPHeaderField(String field)
{
    return m_request.httpHeaderField(field);
}

bool WebMutableURLRequest::isEmpty()
{
    return m_request.isEmpty();
}

void WebMutableURLRequest::addValue(String /*value*/, String /*field*/)
{
}

void WebMutableURLRequest::setAllHTTPHeaderFields(HTTPHeaderPropertyBag *)
{
}

void WebMutableURLRequest::setCachePolicy(WebCore::ResourceRequestCachePolicy policy)
{
    m_request.setCachePolicy(policy);
}

void WebMutableURLRequest::setHTTPBody(String)
{
}

void WebMutableURLRequest::setHTTPBodyStream(String)
{
}

void WebMutableURLRequest::setHTTPMethod(String method)
{
    m_request.setHTTPMethod(method);
}

void WebMutableURLRequest::setHTTPShouldHandleCookies(bool /*handleCookies*/)
{
}

void WebMutableURLRequest::setMainDocumentURL(String /*theURL*/)
{
}

void WebMutableURLRequest::setTimeoutInterval(double /*timeoutInterval*/)
{
}

void WebMutableURLRequest::setURL(String url)
{
    m_request.setURL(KURL(url));
}

void WebMutableURLRequest::setValue(String /*value*/, String /*field*/)
{
}

void WebMutableURLRequest::setAllowsAnyHTTPSCertificate()
{
    //ResourceHandle::setHostAllowsAnyHTTPSCertificate(m_request.url().host());
}

// void WebMutableURLRequest::setClientCertificate(OLE_HANDLE cert)
// {
//     if (!cert)
//         return E_POINTER;
// 
//     PCCERT_CONTEXT certContext = reinterpret_cast<PCCERT_CONTEXT>((ULONG64)cert);
//     RetainPtr<CFDataRef> certData(AdoptCF, copyCert(certContext));
//     ResourceHandle::setClientCertificate(m_request.url().host(), certData.get());
//     return S_OK;
// }

void WebMutableURLRequest::setFormData(const PassRefPtr<FormData> data)
{
    m_request.setHTTPBody(data);
}

const PassRefPtr<FormData> WebMutableURLRequest::formData() const
{
    return m_request.httpBody();
}

void WebMutableURLRequest::addHTTPHeaderFields(const HTTPHeaderMap& headerFields)
{
    m_request.addHTTPHeaderFields(headerFields);
}

const HTTPHeaderMap& WebMutableURLRequest::httpHeaderFields() const
{
    return m_request.httpHeaderFields();
}

const ResourceRequest& WebMutableURLRequest::resourceRequest() const
{
    return m_request;
}
