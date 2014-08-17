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

#include "HTTPHeaderPropertyBag.h"

#include <wtf/Platform.h>
#include <PlatformString.h>
#include <KURL.h>
#include <ResourceHandle.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

static String HTTPMessageCopyLocalizedShortDescriptionForStatusCode(int statusCode)
{
    if (statusCode < 100 || statusCode >= 600)
        return "server error";
    else if (statusCode >= 100 && statusCode <= 199) {
        switch (statusCode) {
            case 100:
                return "continue";
            case 101:
                return "switching protocols";
            default:
                return "informational";
        }
    } else if (statusCode >= 200 && statusCode <= 299) {
        switch (statusCode) {
            case 200:
                return "no error";
            case 201:
                return "created";
            case 202:
                return "accepted";
            case 203:
                return "non-authoritative information";
            case 204:
                return "no content";
            case 205:
                return "reset content";
            case 206:
                return "partial content";
            default:
                return "success";
        } 
    } else if (statusCode >= 300 && statusCode <= 399) {
        switch (statusCode) {
            case 300:
                return "multiple choices";
            case 301:
                return "moved permanently";
            case 302:
                return "found";
            case 303:
                return "see other";
            case 304:
                return "not modified";
            case 305:
                return "needs proxy";
            case 307:
                return "temporarily redirected";
            case 306:   // 306 status code unused in HTTP
            default:
                return "redirected";
        }
    } else if (statusCode >= 400 && statusCode <= 499) {
        switch (statusCode) {
            case 400:
                return "bad request";
            case 401:
                return "unauthorized";
            case 402:
                return "payment required";
            case 403:
                return "forbidden";
            case 404:
                return "not found";
            case 405:
                return "method not allowed";
            case 406:
                return "unacceptable";
            case 407:
                return "proxy authentication required";
            case 408:
                return "request timed out";
            case 409:
                return "conflict";
            case 410:
                return "no longer exists";
            case 411:
                return "length required";
            case 412:
                return "precondition failed";
            case 413:
                return "request too large";
            case 414:
                return "requested URL too long";
            case 415:
                return "unsupported media type";
            case 416:
                return "requested range not satisfiable";
            case 417:
                return "expectation failed";
            default:
                return "client error";
        }
    } else if (statusCode >= 500 && statusCode <= 599) {
        switch (statusCode) {
            case 500:
                return "internal server error";
            case 501:
                return "unimplemented";
            case 502:
                return "bad gateway";
            case 503:
                return "service unavailable";
            case 504:
                return "gateway timed out";
            case 505:
                return "unsupported version";
            default:
                return "server error";
        }
    }
    return "";
}

WebURLResponse::WebURLResponse()
{
    DS_CONSTRUCT();
}

WebURLResponse::~WebURLResponse()
{
    DS_DESTRUCT();
}

WebURLResponse* WebURLResponse::createInstance()
{
    WebURLResponse* instance = new WebURLResponse();
    // fake an http response - so it has the IWebHTTPURLResponse interface
    instance->m_response = ResourceResponse(KURL("http://"), String(), 0, String(), String());
    return instance;
}

WebURLResponse* WebURLResponse::createInstance(const ResourceResponse& response)
{
    if (response.isNull())
        return 0;

    WebURLResponse* instance = new WebURLResponse();
    instance->m_response = response;

    return instance;
}

long long WebURLResponse::expectedContentLength()
{
    return m_response.expectedContentLength();
}

void WebURLResponse::initWithURL(String url, String mimeType, int expectedContentLength, String textEncodingName)
{
    m_response = ResourceResponse(KURL(url), String(mimeType), expectedContentLength, String(textEncodingName), String());
}

String WebURLResponse::MIMEType()
{
    return m_response.mimeType();
}

String WebURLResponse::suggestedFilename()
{
    return m_response.suggestedFilename();
}

String WebURLResponse::textEncodingName()
{
    return m_response.textEncodingName();
}

String WebURLResponse::URL()
{
    return m_response.url().string();
}

HTTPHeaderPropertyBag* WebURLResponse::allHeaderFields()
{
    ASSERT(m_response.isHTTP());
    return HTTPHeaderPropertyBag::createInstance(this);
}

String WebURLResponse::localizedStringForStatusCode(int statusCode)
{
    ASSERT(m_response.isHTTP());
    
    return HTTPMessageCopyLocalizedShortDescriptionForStatusCode(statusCode);
}

int WebURLResponse::statusCode()
{
    ASSERT(m_response.isHTTP());
    return m_response.httpStatusCode();
}

bool WebURLResponse::isAttachment()
{
    return m_response.isAttachment();
}


/*OLE_HANDLE WebURLResponse::sslPeerCertificate()
{
#if USE(CFNETWORK)
    CFDictionaryRef dict = certificateDictionary();
    if (!dict)
        return E_FAIL;
    void* data = wkGetSSLPeerCertificateData(dict);
    if (!data)
        return E_FAIL;
    *result = (OLE_HANDLE)(ULONG64)data;
#endif

    return *result ? S_OK : E_FAIL;
}*/


String WebURLResponse::suggestedFileExtension()
{
    //FIXME : trouver l'extension

//     if (m_response.mimeType().isEmpty())
//         return E_FAIL;
// 
//     BString mimeType(m_response.mimeType());
//     HKEY key;
//     LONG err = RegOpenKeyEx(HKEY_CLASSES_ROOT, TEXT("MIME\\Database\\Content Type"), 0, KEY_QUERY_VALUE, &key);
//     if (!err) {
//         HKEY subKey;
//         err = RegOpenKeyEx(key, mimeType, 0, KEY_QUERY_VALUE, &subKey);
//         if (!err) {
//             DWORD keyType = REG_SZ;
//             TCHAR extension[MAX_PATH];
//             DWORD keySize = sizeof(extension)/sizeof(extension[0]);
//             err = RegQueryValueEx(subKey, TEXT("Extension"), 0, &keyType, (LPBYTE)extension, &keySize);
//             if (!err && keyType != REG_SZ)
//                 err = ERROR_INVALID_DATA;
//             if (err) {
//                 // fallback handlers
//                 if (!_tcscmp(mimeType, TEXT("text/html"))) {
//                     _tcscpy(extension, TEXT(".html"));
//                     err = 0;
//                 } else if (!_tcscmp(mimeType, TEXT("application/xhtml+xml"))) {
//                     _tcscpy(extension, TEXT(".xhtml"));
//                     err = 0;
//                 } else if (!_tcscmp(mimeType, TEXT("image/svg+xml"))) {
//                     _tcscpy(extension, TEXT(".svg"));
//                     err = 0;
//                 }
//             }
//             if (!err) {
//                 *result = SysAllocString(extension);
//                 if (!*result)
//                     err = ERROR_OUTOFMEMORY;
//             }
//             RegCloseKey(subKey);
//         }
//         RegCloseKey(key);
//     }
// 
//     return HRESULT_FROM_WIN32(err);
    return String();
}

const ResourceResponse& WebURLResponse::resourceResponse() const
{
    return m_response;
}
