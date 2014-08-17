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

#ifndef WebMutableURLRequest_H
#define WebMutableURLRequest_H

#include <wtf/FastAllocBase.h>

/**
 *  @file  WebMutableURLRequest.h
 *  WebMutableURLRequest description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include <ResourceRequest.h>
#include <ResourceRequest.h>
#include <ResourceRequestBase.h>

namespace WebCore
{
    class FormData;
    class String;
}

class HTTPHeaderPropertyBag;

class WebMutableURLRequest/*: public WTF::FastAllocBase*/
{
public:

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebMutableURLRequest* createInstance();

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebMutableURLRequest* createInstance(WebMutableURLRequest* req);

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebMutableURLRequest* createInstance(const WebCore::ResourceRequest&);


    /**
     *  createImmutableInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebMutableURLRequest* createImmutableInstance();

    /**
     *  createImmutableInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebMutableURLRequest* createImmutableInstance(const WebCore::ResourceRequest&);
protected:

    /**
     *  WebMutableURLRequest description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    WebMutableURLRequest(bool isMutable);

public:

    /**
     *  ~WebMutableURLRequest description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual ~WebMutableURLRequest();

    /**
     *  requestWithURL description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void requestWithURL( WebCore::String theURL, WebCore::ResourceRequestCachePolicy cachePolicy, double timeoutInterval);

    /**
     *  allHTTPHeaderFields description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual HTTPHeaderPropertyBag* allHTTPHeaderFields();

    /**
     *  cachePolicy description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::ResourceRequestCachePolicy cachePolicy();

    /**
     *  HTTPBody description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String HTTPBody();

    /**
     *  HTTPBodyStream description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String HTTPBodyStream();

    /**
     *  HTTPMethod description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String HTTPMethod();

    /**
     *  HTTPShouldHandleCookies description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual bool HTTPShouldHandleCookies();

    /**
     *  initWithURL description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void initWithURL(WebCore::String url, WebCore::ResourceRequestCachePolicy cachePolicy, double timeoutInterval);

    /**
     *  mainDocumentURL description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String mainDocumentURL();

    /**
     *  timeoutInterval description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual double timeoutInterval();

    /**
     *  URL description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String URL();

    /**
     *  valueForHTTPHeaderField description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String valueForHTTPHeaderField(WebCore::String field);

    /**
     *  isEmpty description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual bool isEmpty();


    /**
     *  addValue description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void addValue(WebCore::String value, WebCore::String field);

    /**
     *  setAllHTTPHeaderFields description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void setAllHTTPHeaderFields(HTTPHeaderPropertyBag *headerFields);

    /**
     *  setCachePolicy description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void setCachePolicy(WebCore::ResourceRequestCachePolicy policy);

    /**
     *  setHTTPBody description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void setHTTPBody(WebCore::String data);

    /**
     *  setHTTPBodyStream description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void setHTTPBodyStream(WebCore::String data);

    /**
     *  setHTTPMethod description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void setHTTPMethod(WebCore::String method);

    /**
     *  setHTTPShouldHandleCookies description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void setHTTPShouldHandleCookies(bool handleCookies);

    /**
     *  setMainDocumentURL description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void setMainDocumentURL(WebCore::String theURL);

    /**
     *  setTimeoutInterval description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void setTimeoutInterval(double timeoutInterval);

    /**
     *  setURL description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void setURL(WebCore::String theURL);

    /**
     *  setValue description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void setValue(WebCore::String value, WebCore::String field);

    /**
     *  setAllowsAnyHTTPSCertificate description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void setAllowsAnyHTTPSCertificate();


    /**
     *  setClientCertificate description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    //virtual void setClientCertificate(OLE_HANDLE cert);


    /**
     *  setFormData description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    void setFormData(const PassRefPtr<WebCore::FormData> data);
    const PassRefPtr<WebCore::FormData> formData() const;
    

    /**
     *  addHTTPHeaderFields description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    void addHTTPHeaderFields(const WebCore::HTTPHeaderMap& headerFields);
    const WebCore::HTTPHeaderMap& httpHeaderFields() const;

    const WebCore::ResourceRequest& resourceRequest() const;
protected:
    bool m_isMutable;
    WebCore::ResourceRequest m_request;
};

#endif
