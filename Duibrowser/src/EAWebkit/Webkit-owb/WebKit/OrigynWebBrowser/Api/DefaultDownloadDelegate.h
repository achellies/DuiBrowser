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

#ifndef DefaultDownloadDelegate_h
#define DefaultDownloadDelegate_h

#include <wtf/FastAllocBase.h>

/**
 *  @file  DefaultDownloadDelegate.h
 *  DefaultDownloadDelegate description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include <wtf/HashSet.h>
#include <wtf/HashSet.h>
#include "PlatformString.h"

class WebError;
class WebMutableURLResponse;
class WebMutableURLRequest;
class WebDownload;
class WebURLAuthenticationChallenge;

class DefaultDownloadDelegate/*: public WTF::FastAllocBase*/
{
public:

    /**
     *  sharedInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static DefaultDownloadDelegate* sharedInstance();

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static DefaultDownloadDelegate* createInstance();
private:

    /**
     *  DefaultDownloadDelegate description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    DefaultDownloadDelegate();
public:

    /**
     *  ~DefaultDownloadDelegate description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual ~DefaultDownloadDelegate();

    /**
     *  decideDestinationWithSuggestedFilename description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void decideDestinationWithSuggestedFilename(WebDownload *download, WebCore::String filename);

    /**
     *  didCancelAuthenticationChallenge description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void didCancelAuthenticationChallenge(WebDownload* download, WebURLAuthenticationChallenge* challenge);

    /**
     *  didCreateDestination description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void didCreateDestination(WebDownload* download,  WebCore::String destination);

    /**
     *  didFailWithError description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void didFailWithError(WebDownload* download, WebError* error);

    /**
     *  didReceiveAuthenticationChallenge description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void didReceiveAuthenticationChallenge(WebDownload* download, WebURLAuthenticationChallenge* challenge);

    /**
     *  didReceiveDataOfLength description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void didReceiveDataOfLength(WebDownload* download, unsigned length);

    /**
     *  didReceiveResponse description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void didReceiveResponse(WebDownload* download, WebMutableURLResponse* response);

    /**
     *  shouldDecodeSourceDataOfMIMEType description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual bool shouldDecodeSourceDataOfMIMEType(WebDownload* download, WebCore::String encodingType);

    /**
     *  willResumeWithResponse description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void willResumeWithResponse(WebDownload* download, WebMutableURLResponse* response, long long fromByte);

    /**
     *  willSendRequest description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebMutableURLRequest* willSendRequest(WebDownload* download, WebMutableURLRequest* request,  WebMutableURLResponse* redirectResponse);

    /**
     *  didBegin description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void didBegin(WebDownload* download);

    /**
     *  didFinish description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void didFinish(WebDownload* download);

    // DefaultDownloadDelegate

    /**
     *  registerDownload description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    void registerDownload(WebDownload*);

    /**
     *  unregisterDownload description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    void unregisterDownload(WebDownload*);
protected:
    HashSet<WebDownload*> m_downloads;

//+daw ca 29/07/2008 static and global management
private:
    static DefaultDownloadDelegate* m_st_pSharedinstance;
public:
    static void staticFinalize();
//-daw ca
};

#endif
