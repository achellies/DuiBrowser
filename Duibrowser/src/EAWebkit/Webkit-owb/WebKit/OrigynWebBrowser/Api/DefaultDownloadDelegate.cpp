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
#include "DefaultDownloadDelegate.h"
#include "WebDownload.h"
#include "WebMutableURLRequest.h"
#include "WebURLResponse.h"

#include <PlatformString.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;


DefaultDownloadDelegate::DefaultDownloadDelegate()
{
    DS_CONSTRUCT();
}

DefaultDownloadDelegate::~DefaultDownloadDelegate()
{
    HashSet<WebDownload*>::iterator i = m_downloads.begin();
    for (;i != m_downloads.end(); ++i)
        delete (*i);
    DS_DESTRUCT();
}

DefaultDownloadDelegate* DefaultDownloadDelegate::sharedInstance()
{
//+daw ca 29/07/2008 static and global management
    if (!m_st_pSharedinstance)
    {
        m_st_pSharedinstance = DefaultDownloadDelegate::createInstance();
    }
    return m_st_pSharedinstance;
//-daw ca
}

DefaultDownloadDelegate* DefaultDownloadDelegate::createInstance()
{
    DefaultDownloadDelegate* instance = new DefaultDownloadDelegate();
    return instance;
}


void DefaultDownloadDelegate::decideDestinationWithSuggestedFilename(WebDownload *download, String filename)
{
    /*LOG(Download, "DefaultDownloadDelegate %p - decideDestinationWithSuggestedFilename %s", download, String(filename, SysStringLen(filename)).ascii().data());

    TCHAR pathChars[MAX_PATH];
    if (FAILED(SHGetFolderPath(0, CSIDL_DESKTOPDIRECTORY  | CSIDL_FLAG_CREATE, 0, 0, pathChars))) {
        if (FAILED(download->setDestination(filename, true))) {
            LOG_ERROR("Failed to set destination on file");
            return E_FAIL;
        }  
        return S_OK;
    }

    size_t fullLength = _tcslen(pathChars) + SysStringLen(filename) + 2;
    BSTR full = SysAllocStringLen(0, (UINT)fullLength);
    if (!full)
        return E_OUTOFMEMORY;

    _tcscpy_s(full, fullLength, pathChars);
    _tcscat_s(full, fullLength, _T("\\"));
    _tcscat_s(full, fullLength, filename);
    BString fullPath;
    fullPath.adoptBSTR(full);

#ifndef NDEBUG
    String debug((BSTR)fullPath, SysStringLen(BSTR(fullPath)));
    LOG(Download, "Setting path to %s", debug.ascii().data());
#endif

    if (FAILED(download->setDestination(fullPath, true))) {
        LOG_ERROR("Failed to set destination on file");
        return E_FAIL;
    }
    return S_OK;*/
}
void DefaultDownloadDelegate::didCancelAuthenticationChallenge(WebDownload* download, WebURLAuthenticationChallenge* challenge)
{
    //LOG(Download, "DefaultDownloadDelegate %p - didCancelAuthenticationChallenge %p", download, challenge);
    download = 0;
    challenge = 0;
}
void DefaultDownloadDelegate::didCreateDestination(WebDownload* download, String destination)
{
    //LOG(Download, "DefaultDownloadDelegate %p - didCreateDestination %s", download, String(destination, SysStringLen(destination)).ascii().data());
    //download = 0;
    //destination = 0;
}

void DefaultDownloadDelegate::didReceiveAuthenticationChallenge(WebDownload* download, WebURLAuthenticationChallenge* challenge)
{
    //LOG(Download, "DefaultDownloadDelegate %p - didReceiveAuthenticationChallenge %p", download, challenge);
    download = 0;
    challenge = 0;
}

void DefaultDownloadDelegate::didReceiveDataOfLength(WebDownload* download, unsigned length)
{
    //LOG(Download, "DefaultDownloadDelegate %p - didReceiveDataOfLength %i", download, length);
    download = 0;
    length = 0;
}

void DefaultDownloadDelegate::didReceiveResponse(WebDownload* download, WebMutableURLResponse* response)
{
    //LOG(Download, "DefaultDownloadDelegate %p - didReceiveResponse %p", download, response);
    download = 0;
    response = 0;
}

bool DefaultDownloadDelegate::shouldDecodeSourceDataOfMIMEType(WebDownload* download, String encodingType)
{
    //LOG(Download, "DefaultDownloadDelegate %p - shouldDecodeSourceDataOfMIMEType %s", download, String(encodingType, SysStringLen(encodingType)).ascii().data());
    download = 0;
//    encodingType = 0;
    return false;
}

void DefaultDownloadDelegate::willResumeWithResponse(WebDownload* download, WebMutableURLResponse* response, long long fromByte)
{
    //LOG(Download, "DefaultDownloadDelegate %p - willResumeWithResponse %p, %q", download, response, fromByte);
    download = 0;
    response = 0;
    fromByte = 0;
}

WebMutableURLRequest* DefaultDownloadDelegate::willSendRequest(WebDownload* download, WebMutableURLRequest* request, WebMutableURLResponse* redirectResponse)
{
    //LOG(Download, "DefaultDownloadDelegate %p - willSendRequest %p %p", download, request, redirectResponse);
    download = 0;
    redirectResponse = 0;
    return request;
}

void DefaultDownloadDelegate::didBegin(WebDownload* download)
{
    //LOG(Download, "DefaultDownloadDelegate %p - didBegin", download);
    registerDownload(download);
}

void DefaultDownloadDelegate::didFinish(WebDownload* download)
{
    //LOG(Download, "DefaultDownloadDelegate %p - didFinish", download);
    unregisterDownload(download);
}

void DefaultDownloadDelegate::didFailWithError(WebDownload* download, WebError* error)
{
    //LOG(Download, "DefaultDownloadDelegate %p - didFailWithError %p", download, error);
    unregisterDownload(download);
    error = 0;
}

void DefaultDownloadDelegate::registerDownload(WebDownload* download)
{
    if (m_downloads.contains(download))
        return;
    m_downloads.add(download);
}

void DefaultDownloadDelegate::unregisterDownload(WebDownload* download)
{
    if (m_downloads.contains(download)) {
        m_downloads.remove(download);
    }
}

//+daw ca 29/07/2008 static and global management
DefaultDownloadDelegate* DefaultDownloadDelegate::m_st_pSharedinstance = NULL;

void DefaultDownloadDelegate::staticFinalize()
{
	delete m_st_pSharedinstance;
	m_st_pSharedinstance = NULL;
}

//-daw ca
