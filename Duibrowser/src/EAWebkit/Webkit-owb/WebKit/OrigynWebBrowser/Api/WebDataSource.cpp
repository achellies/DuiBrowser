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
#include "WebDataSource.h"

#include "WebDocumentLoader.h"
#include "WebError.h"
#include "WebFrame.h"
#include "WebMutableURLRequest.h"
#include "WebResource.h"
#include "WebURLResponse.h"

#include <PlatformString.h>
#include <DocLoader.h>
#include <Document.h>
#include <FrameLoader.h>
#include <KURL.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

WebDataSource::WebDataSource(WebDocumentLoader* loader)
    : m_loader(loader)
{
    DS_CONSTRUCT();
}

WebDataSource::~WebDataSource()
{
    if (m_loader)
        m_loader->detachDataSource();
    m_loader = 0;
    DS_DESTRUCT();
}

WebDataSource* WebDataSource::createInstance(WebDocumentLoader* loader)
{
    WebDataSource* instance = new WebDataSource(loader);
    return instance;
}

WebDocumentLoader* WebDataSource::documentLoader() const
{
    return m_loader.get();
}

String WebDataSource::overrideEncoding()
{
    return String();
}

void WebDataSource::setOverrideEncoding(String /*encoding*/)
{
}

WebError* WebDataSource::mainDocumentError()
{
    if (!m_loader)
        return 0;

    if (m_loader->mainDocumentError().isNull())
        return 0;

    return WebError::createInstance(m_loader->mainDocumentError());
}

void WebDataSource::initWithRequest(WebMutableURLRequest* /*request*/)
{
}

PassRefPtr<SharedBuffer> WebDataSource::data()
{
    if (!m_loader)
        return 0;

    return m_loader->mainResourceData();
}

// WebDocumentRepresentation* WebDataSource::representation()
// {
//     HRESULT hr = S_OK;
//     if (!m_representation) {
//         WebHTMLRepresentation* htmlRep = WebHTMLRepresentation::createInstance(static_cast<WebFrame*>(m_loader->frameLoader()->client()));
//         hr = htmlRep->QueryInterface(IID_IWebDocumentRepresentation, (void**) &m_representation);
//         htmlRep->Release();
//     }
// 
//     return m_representation.copyRefTo(rep);
// }

WebFrame* WebDataSource::webFrame()
{
    return static_cast<WebFrame*>(m_loader->frameLoader()->client());
}

WebMutableURLRequest* WebDataSource::initialRequest()
{
    return WebMutableURLRequest::createInstance(m_loader->originalRequest());
}

WebMutableURLRequest* WebDataSource::request()
{
    return WebMutableURLRequest::createInstance(m_loader->request());
}

WebURLResponse* WebDataSource::response()
{
    return WebURLResponse::createInstance(m_loader->response());
}

String WebDataSource::textEncodingName()
{
    String encoding = m_loader->overrideEncoding();
    if (encoding.isNull())
        encoding = m_loader->response().textEncodingName();

    return encoding;
}

bool WebDataSource::isLoading()
{
    return m_loader->isLoadingInAPISense();
}

String WebDataSource::pageTitle()
{
    return m_loader->title();
}

String WebDataSource::unreachableURL()
{
    KURL unreachableURL = m_loader->unreachableURL();
    return unreachableURL.string();
}

WebArchive* WebDataSource::webArchive()
{
    return 0;
}

WebResource* WebDataSource::mainResource()
{
    return 0;
}

WebResource* WebDataSource::subresourceForURL(String url)
{
    Document *doc = m_loader->frameLoader()->frame()->document();

    if (!doc)
        return 0;

    CachedResource *cachedResource = doc->docLoader()->cachedResource(String(url));

    if (!cachedResource)
        return 0;

    return WebResource::createInstance(cachedResource->data(), cachedResource->response());
}

void WebDataSource::addSubresource(WebResource* /*subresource*/)
{
}
