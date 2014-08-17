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

#ifndef WebArchive_h
#define WebArchive_h

#include <wtf/FastAllocBase.h>

/**
 *  @file  WebArchive.h
 *  WebArchive description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include <wtf/PassRefPtr.h>
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>

namespace WebCore {
    class Archive;
    class SharedBuffer;
}

class WebResource;

class WebArchive/*: public WTF::FastAllocBase*/
{
public:

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebArchive* createInstance();

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebArchive* createInstance(PassRefPtr<WebCore::Archive>);
protected:

    /**
     *  WebArchive description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    WebArchive(PassRefPtr<WebCore::Archive>);

public:

    /**
     *  ~WebArchive description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual ~WebArchive();

    /**
     *  initWithMainResource description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void initWithMainResource(WebResource* mainResource, WebResource** subResources, int cSubResources,  WebArchive** subFrameArchives, int cSubFrameArchives);


    /**
     *  initWithData description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void initWithData(RefPtr<WebCore::SharedBuffer>);

    /**
     *  mainResource description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebResource*  mainResource();


    /**
     *  data description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual RefPtr<WebCore::SharedBuffer> data();

protected:
    RefPtr<WebCore::Archive> m_archive;
};

#endif // WebArchive_h
