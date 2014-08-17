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

#ifndef WebResource_h
#define WebResource_h

#include <wtf/FastAllocBase.h>

/**
 *  @file  WebResource.h
 *  WebResource description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include <KURL.h>
#include <KURL.h>
#include <PlatformString.h>
#include <ResourceResponse.h>
#include <SharedBuffer.h>
#include <wtf/PassRefPtr.h>

class WebResource/*: public WTF::FastAllocBase*/ {
public:

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebResource* createInstance(PassRefPtr<WebCore::SharedBuffer> data, const WebCore::ResourceResponse& response);
protected:

    /**
     *  WebResource description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    WebResource(PassRefPtr<WebCore::SharedBuffer> data, const WebCore::KURL& url, const WebCore::String& mimeType, const WebCore::String& textEncodingName, const WebCore::String& frameName);

public:

    /**
     *  ~WebResource description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual ~WebResource();

    /**
     *  initWithData description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void initWithData(PassRefPtr<WebCore::SharedBuffer> data, WebCore::String url, WebCore::String mimeType, WebCore::String textEncodingName, WebCore::String frameName);

    /**
     *  data description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual PassRefPtr<WebCore::SharedBuffer> data();

    /**
     *  URL description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String URL();

    /**
     *  MIMEType description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String MIMEType();

    /**
     *  textEncodingName description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String textEncodingName();

    /**
     *  frameName description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String frameName();

private:
    PassRefPtr<WebCore::SharedBuffer> m_data;
    WebCore::KURL m_url;
    WebCore::String m_mimeType;
    WebCore::String m_textEncodingName;
    WebCore::String m_frameName;
};

#endif
