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

#ifndef WebURLResponse_H
#define WebURLResponse_H

#include <wtf/FastAllocBase.h>

/**
 *  @file  WebURLResponse.h
 *  WebURLResponse description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include <ResourceResponse.h>
#include <ResourceResponse.h>

namespace WebCore {
    class String;
}

class HTTPHeaderPropertyBag;

class WebURLResponse/*: public WTF::FastAllocBase*/
{
public:

    /**
     *  createInstance creates an instance
     * @param[in]: 
     * @param[out]: WebURLResponse*
     */
    static WebURLResponse* createInstance();

    /**
     *  createInstance creates an instance from a WebCore::ResourceResponse&
     * @param[in]: WebCore::ResourceResponse&
     * @param[out]: WebURLResponse*
     */
    static WebURLResponse* createInstance(const WebCore::ResourceResponse& response);
protected:

    /**
     *  WebURLResponse constructor
     */
    WebURLResponse();

public:

    /**
     *  ~WebURLResponse destructor
     */
    virtual ~WebURLResponse();

    /**
     *  expectedContentLength returns the expected content length
     * @param[in]: 
     * @param[out]: length
     */
    virtual long long expectedContentLength();

    /**
     *  initWithURLA prepares URL response
     * @param[in]: see signature
     * @param[out]: 
     */
    virtual void initWithURL( WebCore::String url, WebCore::String mimeType, int expectedContentLength, WebCore::String textEncodingName);

    /**
     *  MIMEType returns MIME Type
     * @param[in]: 
     * @param[out]: 
     */
    virtual WebCore::String MIMEType();

    /**
     *  suggestedFilename description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String suggestedFilename();

    /**
     *  textEncodingName proposed file name
     * @param[in]: 
     * @param[out]: Suggested filename
     */
    virtual WebCore::String textEncodingName();

    /**
     *  URL returns URL
     * @param[out]:  encoding name
     */
    virtual WebCore::String URL();


    /**
     *  *allHeaderFields sets all HTTP headers
     * @param[in]: 
     * @param[out]: HTTPHeaderPropertyBag
     */
    virtual HTTPHeaderPropertyBag *allHeaderFields();

    /**
     *  localizedStringForStatusCode returns the correct string for status depending on localisation
     * @param[in]: status
     * @param[out]: string localized
     */
    virtual WebCore::String localizedStringForStatusCode(int statusCode);

    /**
     *  statusCode returns status code
     * @param[in]: 
     * @param[out]: status code
     */
    virtual int statusCode();

    /**
     *  isAttachment identifes if it's an attachment
     * @param[in]: 
     * @param[out]: boolean
     */
    virtual bool isAttachment();


    /**
     *  sslPeerCertificate description
     * @param[in]: description
     * @param[out]: description
     */
    //virtual OLE_HANDLE sslPeerCertificate();
   
    /**
     * resourceResponse returns WebCore ResourceResponse
     * @param[in]: 
     * @param[out]: WebCore::ResourceResponse&
     * @code
     * @endcode
     */ 
    const WebCore::ResourceResponse& resourceResponse() const;

protected:

    /**
     *  suggestedFileExtension tries to propose a file extension
     * @param[in]: 
     * @param[out]: file extension
     * @code
     * @endcode
     */
    WebCore::String suggestedFileExtension();

protected:
    WebCore::ResourceResponse m_response;
};

#endif
