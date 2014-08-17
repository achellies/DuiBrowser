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
#include "WebResource.h"

#include <PlatformString.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

WebResource::WebResource(PassRefPtr<WebCore::SharedBuffer> data, const WebCore::KURL& url, const WebCore::String& mimeType, const WebCore::String& textEncodingName, const WebCore::String& frameName)
    : m_data(data)
    , m_url(url)
    , m_mimeType(mimeType)
    , m_textEncodingName(textEncodingName)
    , m_frameName(frameName)
{
    DS_CONSTRUCT();
}

WebResource::~WebResource()
{
    DS_DESTRUCT();
}

WebResource* WebResource::createInstance(PassRefPtr<WebCore::SharedBuffer> data, const WebCore::ResourceResponse& response)
{
    WebResource* instance = new WebResource(data, response.url(), response.mimeType(), response.textEncodingName(), String());
    return instance;
}

void WebResource::initWithData( PassRefPtr<WebCore::SharedBuffer> data, String url, String mimeType, String textEncodingName, String frameName)
{
    m_data = data;
    m_url = KURL(String(url));
    m_mimeType = String(mimeType);
    m_textEncodingName = String(textEncodingName);
    m_frameName = String(frameName);
}

    
PassRefPtr<WebCore::SharedBuffer> WebResource::data()
{
    return m_data;
}
   
String WebResource::URL()
{
    return m_url.string();
}
    
String WebResource::MIMEType()
{
    return m_mimeType;
}
   
String WebResource::textEncodingName()
{
    return m_textEncodingName;
}
    
String WebResource::frameName()
{
    return m_frameName;
}
