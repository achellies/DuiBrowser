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

#ifndef WebDataSource_H
#define WebDataSource_H

#include <wtf/FastAllocBase.h>

/**
 *  @file  WebDataSource.h
 *  WebDataSource description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include <wtf/RefPtr.h>
#include <wtf/RefPtr.h>

class WebDocumentLoader;
class WebMutableURLRequest;
class WebURLResponse;
class WebArchive;
class WebResource;
class WebError;
class WebFrame;

namespace WebCore {
    class SharedBuffer;
    class String;
}

class WebDataSource/*: public WTF::FastAllocBase*/
{
public:

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebDataSource* createInstance(WebDocumentLoader*);
protected:

    /**
     *  WebDataSource description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    WebDataSource(WebDocumentLoader*);

public:

    /**
     *  ~WebDataSource description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual ~WebDataSource();

    /**
     *  initWithRequest description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void initWithRequest(WebMutableURLRequest *request);


    /**
     *  data description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WTF::PassRefPtr<WebCore::SharedBuffer> data();

    /**
     *  representation description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    //virtual WebHTMLRepresentation* representation();

    /**
     *  webFrame description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebFrame* webFrame();

    /**
     *  initialRequest description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebMutableURLRequest* initialRequest();

    /**
     *  request description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebMutableURLRequest* request();

    /**
     *  response description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebURLResponse* response();

    /**
     *  textEncodingName description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String textEncodingName();

    /**
     *  isLoading description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual bool isLoading();

    /**
     *  pageTitle description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String pageTitle();

    /**
     *  unreachableURL description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String unreachableURL();

    /**
     *  webArchive description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebArchive* webArchive();

    /**
     *  mainResource description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebResource* mainResource();

    /**
     *  subresourceForURL description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebResource* subresourceForURL(WebCore::String url);

    /**
     *  addSubresource description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void addSubresource(WebResource *subresource);
    

    /**
     *  overrideEncoding description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String overrideEncoding();

    /**
     *  setOverrideEncoding description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void setOverrideEncoding(WebCore::String encoding);

    /**
     *  mainDocumentError description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebError* mainDocumentError();

    // WebDataSource
    WebDocumentLoader* documentLoader() const;
protected:
    RefPtr<WebDocumentLoader> m_loader;
    //COMPtr<IWebDocumentRepresentation> m_representation;
};

#endif
