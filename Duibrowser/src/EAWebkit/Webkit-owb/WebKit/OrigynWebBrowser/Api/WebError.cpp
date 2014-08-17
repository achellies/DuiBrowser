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
#include "WebError.h"

#include <PlatformString.h>
#include "HTTPHeaderPropertyBag.h"
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

static WebErrorInfo gErrorInfo[] = 
{
	{0, L"Unknown Webkit Error"},
	{WebKitErrorCannotShowMIMEType, L"Cannot show MIME type"},
	{WebKitErrorCannotShowURL, L"Cannot show URL"},
	{WebKitErrorFrameLoadInterruptedByPolicyChange, L"Frame load interrupted due to policy change"},
	{WebKitErrorCannotUseRestrictedPort, L"Port is restricted"},

	{WebKitErrorCannotFindPlugIn, L"Cannot find Plugin"},
	{WebKitErrorCannotLoadPlugIn, L"Cannot load plugin"},
	{WebKitErrorJavaUnavailable, L"Java runtime is not installed on this machine"},
	{WebURLErrorUnknown, L"URL Error"},
	{WebURLErrorCancelled, L"URL error cancelled"},
	{WebURLErrorBadURL, L"Invalid URL"},
	{WebURLErrorTimedOut, L"Website timed out"},
	{WebURLErrorUnsupportedURL, L"URL is unsupported"},
	{WebURLErrorCannotFindHost, L"Cannot find host"},
	{WebURLErrorCannotConnectToHost, L"Cannot connect to host"},
	{WebURLErrorNetworkConnectionLost, L"Network connection was lost"},
	{WebURLErrorDNSLookupFailed, L"DNS lookup failed"},
	{WebURLErrorHTTPTooManyRedirects, L"Too many redirects"},
	{WebURLErrorResourceUnavailable, L"Resource is unavailable"},
	{WebURLErrorNotConnectedToInternet, L"No connection to internet"},
	{WebURLErrorRedirectToNonExistentLocation, L"Redirection to non existing location"},
	{WebURLErrorBadServerResponse, L"Bad server response"},
	{WebURLErrorUserCancelledAuthentication, L"User cancelled authentication"},
	{WebURLErrorUserAuthenticationRequired, L"User authentication requried"},
	{WebURLErrorUserAuthenticationUnsupported, L"User authentication no supported"},    // The requested authorization type (e.g. Basic, Digest, NTLM) is not currently supported.
	{WebURLErrorZeroByteResource, L"Zero byte resource"},
	{WebURLErrorFileDoesNotExist, L"File does not exist"},
	{WebURLErrorFileIsDirectory, L"File is a directory"},
	{WebURLErrorNoPermissionsToReadFile, L"No permissions to read file"},
	{WebURLErrorSecureConnectionFailed, L"Secure connection failed"},
	{WebURLErrorServerCertificateHasBadDate, L"Server certificate has bad date"},
	{WebURLErrorServerCertificateUntrusted, L"Server certificate is untrusted"},
	{WebURLErrorServerCertificateHasUnknownRoot, L"Server certificate has unknown root"},
	{WebURLErrorServerCertificateNotYetValid, L"Invalid certificate"},
	{WebURLErrorClientCertificateRejected, L"Client certificate rejected"},
	{WebURLErrorClientCertificateRequired, L"Client certificate required"},
	{WebURLErrorCannotLoadFromNetwork, L"Cannot load from network"},
	{WebURLErrorCannotCreateFile, L"Cannot create file"},
	{WebURLErrorCannotOpenFile, L"Cannot open file"},
	{WebURLErrorCannotCloseFile, L"Cannot close file"},
	{WebURLErrorCannotWriteToFile, L"Cannot write to file"},
	{WebURLErrorCannotRemoveFile, L"Cannot remove file"},
	{WebURLErrorCannotMoveFile, L"Cannot move file"},
	{WebURLErrorDownloadDecodingFailedMidStream, L"Download decoding failed mid stream"},
	{WebURLErrorDownloadDecodingFailedToComplete, L"Download decoding failed to complete"}
};


const char16_t *GetWebkitErrorString(int webkitErrorCode)
{
	// by default we keep the first error string if no string is found.
	char16_t *errorString = gErrorInfo[0].mErrorString;

	int size = sizeof(gErrorInfo)/ sizeof(WebErrorInfo);

	for(int i = 0; i < size; ++i)
	{
		if(gErrorInfo[i].mWebkitErrorCode == webkitErrorCode)
		{
			errorString = gErrorInfo[i].mErrorString;
			break;
		}
	}

	return errorString;
}

WebError::WebError(const ResourceError& error, HTTPHeaderPropertyBag* userInfo)
    : m_userInfo(userInfo)
    , m_error(error)
{
    DS_CONSTRUCT();
}

WebError::~WebError()
{
    if(m_userInfo)
        delete m_userInfo;
    DS_DESTRUCT();
}

WebError* WebError::createInstance(const ResourceError& error, HTTPHeaderPropertyBag* userInfo)
{
    WebError* instance = new WebError(error, userInfo);
    return instance;
}

WebError* WebError::createInstance()
{
    return createInstance(ResourceError());
}

void WebError::init(String domain, int code, String url)
{
    m_error = ResourceError(domain, code, url, String());
}
  
int WebError::code()
{
    return m_error.errorCode();
}
        
String WebError::domain()
{
    return m_error.domain();
}
               
String WebError::localizedDescription()
{
    return m_error.localizedDescription();
}

        
String WebError::localizedFailureReason()
{
    return String();
}
        
String WebError::localizedRecoverySuggestion()
{
    return String();
}
       
HTTPHeaderPropertyBag* WebError::userInfo()
{
    return m_userInfo;
}

String WebError::failingURL()
{
    return m_error.failingURL();
}

bool WebError::isPolicyChangeError()
{
    return m_error.domain() == String("WebKitErrorDomain")
        && m_error.errorCode() == WebKitErrorFrameLoadInterruptedByPolicyChange;
}

// OLE_HANDLE WebError::sslPeerCertificate()
// {
//     if (!m_cfErrorUserInfoDict) {
//         // copy userinfo from CFErrorRef
//         CFErrorRef cfError = m_error;
//         m_cfErrorUserInfoDict.adoptCF(CFErrorCopyUserInfo(cfError));
//     }
// 
//     if (!m_cfErrorUserInfoDict)
//         return E_FAIL;
// 
// #if USE(CFNETWORK)
//     void* data = wkGetSSLPeerCertificateData(m_cfErrorUserInfoDict.get());
//     if (!data)
//         return E_FAIL;
//     *result = (OLE_HANDLE)(ULONG64)data;
// #endif
//     return *result ? S_OK : E_FAIL;
// }

const ResourceError& WebError::resourceError() const
{
    return m_error;
}
