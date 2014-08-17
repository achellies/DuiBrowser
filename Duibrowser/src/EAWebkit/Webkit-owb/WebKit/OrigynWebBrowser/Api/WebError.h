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

#ifndef WebError_h
#define WebError_h

#include <wtf/FastAllocBase.h>

/**
 *  @file  WebError.h
 *  WebError description
 *  Repository informations :
 * - $URL$
 * - $Rev$
 * - $Date$
 */
#include "BALBase.h"
#include <ResourceError.h>
#include <ResourceError.h>

class HTTPHeaderPropertyBag;

namespace WebCore {
    class String;
}

enum {
    WebKitErrorCannotShowMIMEType =                             100,
    WebKitErrorCannotShowURL =                                  101,
    WebKitErrorFrameLoadInterruptedByPolicyChange =             102,
    WebKitErrorCannotUseRestrictedPort =                        103,
};

enum {
    WebKitErrorCannotFindPlugIn =                               200,
    WebKitErrorCannotLoadPlugIn =                               201,
    WebKitErrorJavaUnavailable =                                202,
};

enum
{
    WebURLErrorUnknown =                         -1,
    WebURLErrorCancelled =                       -999,
    WebURLErrorBadURL =                          -1000,
    WebURLErrorTimedOut =                        -1001,
    WebURLErrorUnsupportedURL =                  -1002,
    WebURLErrorCannotFindHost =                  -1003,
    WebURLErrorCannotConnectToHost =             -1004,
    WebURLErrorNetworkConnectionLost =           -1005,
    WebURLErrorDNSLookupFailed =                 -1006,
    WebURLErrorHTTPTooManyRedirects =            -1007,
    WebURLErrorResourceUnavailable =             -1008,
    WebURLErrorNotConnectedToInternet =          -1009,
    WebURLErrorRedirectToNonExistentLocation =   -1010,
    WebURLErrorBadServerResponse =               -1011,
    WebURLErrorUserCancelledAuthentication =     -1012,
    WebURLErrorUserAuthenticationRequired =      -1013,
    WebURLErrorUserAuthenticationUnsupported =   -10131,    // The requested authorization type (e.g. Basic, Digest, NTLM) is not currently supported.
    WebURLErrorZeroByteResource =                -1014,
    WebURLErrorFileDoesNotExist =                -1100,
    WebURLErrorFileIsDirectory =                 -1101,
    WebURLErrorNoPermissionsToReadFile =         -1102,
    WebURLErrorSecureConnectionFailed =          -1200,
    WebURLErrorServerCertificateHasBadDate =     -1201,
    WebURLErrorServerCertificateUntrusted =      -1202,
    WebURLErrorServerCertificateHasUnknownRoot = -1203,
    WebURLErrorServerCertificateNotYetValid =    -1204,
    WebURLErrorClientCertificateRejected =       -1205,
    WebURLErrorClientCertificateRequired =       -1206,
    WebURLErrorCannotLoadFromNetwork =           -2000,

    // Download and file I/O errors
    WebURLErrorCannotCreateFile =                -3000,
    WebURLErrorCannotOpenFile =                  -3001,
    WebURLErrorCannotCloseFile =                 -3002,
    WebURLErrorCannotWriteToFile =               -3003,
    WebURLErrorCannotRemoveFile =                -3004,
    WebURLErrorCannotMoveFile =                  -3005,
    WebURLErrorDownloadDecodingFailedMidStream = -3006,
    WebURLErrorDownloadDecodingFailedToComplete =-3007,
};

struct WebErrorInfo
{
	int mWebkitErrorCode;
	char16_t *mErrorString;
};

const char16_t *GetWebkitErrorString(int webkitErrorCode);

#define WebKitErrorDomain "WebKitErrorDomain"
#define WebURLErrorDomain "URLErrorDomain"
#define WebKitErrorMIMETypeKey "WebKitErrorMIMETypeKey"
#define WebKitErrorPlugInNameKey "WebKitErrorPlugInNameKey"
#define WebKitErrorPlugInPageURLStringKey "WebKitErrorPlugInPageURLStringKey"
#define WebPOSIXErrorDomain "NSPOSIXErrorDomain"


class WebError/*: public WTF::FastAllocBase*/ {
public:

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebError* createInstance(const WebCore::ResourceError&, HTTPHeaderPropertyBag* userInfo = 0);

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static WebError* createInstance();
protected:

    /**
     *  WebError description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    WebError(const WebCore::ResourceError&, HTTPHeaderPropertyBag* userInfo);

public:

    /**
     *  ~WebError description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual ~WebError();

    /**
     *  init description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void init(WebCore::String domain, int code, WebCore::String url);

    /**
     *  code description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual int code();

    /**
     *  domain description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String domain();

    /**
     *  localizedDescription description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String localizedDescription();

    /**
     *  localizedFailureReason description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String localizedFailureReason();

    /**
     *  localizedRecoverySuggestion description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String localizedRecoverySuggestion();

    /**
     *  userInfo description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual HTTPHeaderPropertyBag* userInfo();

    /**
     *  failingURL description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual WebCore::String failingURL();

    /**
     *  isPolicyChangeError description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual bool isPolicyChangeError();


    /**
     *  sslPeerCertificate description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    //virtual OLE_HANDLE sslPeerCertificate();

    const WebCore::ResourceError& resourceError() const;

private:
    HTTPHeaderPropertyBag* m_userInfo;
    WebCore::ResourceError m_error;
};

#endif
