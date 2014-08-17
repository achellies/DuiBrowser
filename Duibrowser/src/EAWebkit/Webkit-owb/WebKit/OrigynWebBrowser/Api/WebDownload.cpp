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
* This file was modified by Electronic Arts Inc Copyright ?2009-2010
*/

#include "config.h"
#include "WebDownload.h"

#include "CString.h"
#include "DefaultDownloadDelegate.h"
#include "WebError.h"
#include "WebMutableURLRequest.h"
#include "WebURLAuthenticationChallenge.h"
#include "WebURLCredential.h"
#include "WebURLResponse.h"

#if defined(EA_PLATFORM_WINCE)
#else
	#include <sys/stat.h>
	#include <sys/types.h>
#endif

#include <PlatformString.h>
#include <NotImplemented.h>
#include <ResourceError.h>
#include <ResourceHandle.h>
#include <ResourceRequest.h>
#include <ResourceResponse.h>
#include <SystemTime.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

WebDownload::WebDownload()
{
    DS_CONSTRUCT();
}

void WebDownload::init(ResourceHandle* handle, const ResourceRequest& request, const ResourceResponse& response, DefaultDownloadDelegate* delegate)
{
    m_delegate = delegate ? delegate : DefaultDownloadDelegate::sharedInstance();
    /*CFURLConnectionRef connection = handle->connection();
    if (!connection) {
        LOG_ERROR("WebDownload::WebDownload(ResourceHandle*,...) called with an inactive ResourceHandle");    
        return;
    }

    CFURLDownloadClient client = {0, this, 0, 0, 0, didStartCallback, willSendRequestCallback, didReceiveAuthenticationChallengeCallback, 
        didReceiveResponseCallback, willResumeWithResponseCallback, didReceiveDataCallback, shouldDecodeDataOfMIMETypeCallback,
        decideDestinationWithSuggestedObjectNameCallback, didCreateDestinationCallback, didFinishCallback, didFailCallback};

    m_request.adoptRef(WebMutableURLRequest::createInstance(request));
    m_download.adoptCF(CFURLDownloadCreateAndStartWithLoadingConnection(0, connection, request.cfURLRequest(), response.cfURLResponse(), &client));

    // It is possible for CFURLDownloadCreateAndStartWithLoadingConnection() to fail if the passed in CFURLConnection is not in a "downloadable state"
    // However, we should never hit that case
    if (!m_download) {
        ASSERT_NOT_REACHED();
        LOG_ERROR("WebDownload - Failed to create WebDownload from existing connection (%s)", request.url().string().utf8().data());
    } else
        LOG(Download, "WebDownload - Created WebDownload %p from existing connection (%s)", this, request.url().string().utf8().data());

    // The CFURLDownload either starts successfully and retains the CFURLConnection, 
    // or it fails to creating and we have a now-useless connection with a dangling ref. 
    // Either way, we need to release the connection to balance out ref counts
    handle->releaseConnectionForDownload();
    CFRelease(connection);*/
}

void WebDownload::init(const KURL& url, DefaultDownloadDelegate* delegate)
{
    m_delegate = delegate ? delegate : DefaultDownloadDelegate::sharedInstance();
    //LOG_ERROR("Delegate is %p", m_delegate.get());

    ResourceRequest request(url);
    /*CFURLRequestRef cfRequest = request.cfURLRequest();

    CFURLDownloadClient client = {0, this, 0, 0, 0, didStartCallback, willSendRequestCallback, didReceiveAuthenticationChallengeCallback, 
                                  didReceiveResponseCallback, willResumeWithResponseCallback, didReceiveDataCallback, shouldDecodeDataOfMIMETypeCallback, 
                                  decideDestinationWithSuggestedObjectNameCallback, didCreateDestinationCallback, didFinishCallback, didFailCallback};
    m_request.adoptRef(WebMutableURLRequest::createInstance(request));
    m_download.adoptCF(CFURLDownloadCreate(0, cfRequest, &client));

    CFURLDownloadScheduleWithCurrentMessageQueue(m_download.get());
    CFURLDownloadScheduleDownloadWithRunLoop(m_download.get(), ResourceHandle::loaderRunLoop(), kCFRunLoopDefaultMode);

    LOG(Download, "WebDownload - Initialized download of url %s in WebDownload %p", url.string().utf8().data(), this);*/
}

WebDownload::~WebDownload()
{
    //LOG(Download, "WebDownload - Destroying download (%p)", this);
    cancel();
    if(m_delegate)
        m_delegate = 0;
    if(m_request)
        delete m_request;
    DS_DESTRUCT();
}

WebDownload* WebDownload::createInstance()
{
    WebDownload* instance = new WebDownload();
    return instance;
}

WebDownload* WebDownload::createInstance(ResourceHandle* handle, const ResourceRequest& request, const ResourceResponse& response, DefaultDownloadDelegate* delegate)
{
    WebDownload* instance = new WebDownload();
    instance->init(handle, request, response, delegate);
    return instance;
}

WebDownload* WebDownload::createInstance(const KURL& url, DefaultDownloadDelegate* delegate)
{
    WebDownload* instance = new WebDownload();
    instance->init(url, delegate);
    return instance;
}

void WebDownload::initWithRequest(
        /* [in] */ WebMutableURLRequest* request, 
        /* [in] */ DefaultDownloadDelegate* delegate)
{
    m_delegate = delegate;

/*    LOG(Download, "Delegate is %p", m_delegate.get());

    WebMutableURLRequest *webRequest;

    RetainPtr<CFURLRequestRef> cfRequest = webRequest->resourceRequest().cfURLRequest();

    CFURLDownloadClient client = {0, this, 0, 0, 0, didStartCallback, willSendRequestCallback, didReceiveAuthenticationChallengeCallback, 
                                  didReceiveResponseCallback, willResumeWithResponseCallback, didReceiveDataCallback, shouldDecodeDataOfMIMETypeCallback, 
                                  decideDestinationWithSuggestedObjectNameCallback, didCreateDestinationCallback, didFinishCallback, didFailCallback};
    m_request.adoptRef(WebMutableURLRequest::createInstance(webRequest.get()));
    m_download.adoptCF(CFURLDownloadCreate(0, cfRequest.get(), &client));

    // If for some reason the download failed to create, 
    // we have particular cleanup to do
    if (!m_download) {
        m_request = 0;    
        return E_FAIL;
    }

    CFURLDownloadScheduleWithCurrentMessageQueue(m_download.get());
    CFURLDownloadScheduleDownloadWithRunLoop(m_download.get(), ResourceHandle::loaderRunLoop(), kCFRunLoopDefaultMode);

    LOG(Download, "WebDownload - initWithRequest complete, started download of url %s", webRequest->resourceRequest().url().string().utf8().data());
    return S_OK;*/
}

void WebDownload::initToResumeWithBundle(
        /* [in] */ String bundlePath, 
        /* [in] */ DefaultDownloadDelegate* delegate)
{
    /*LOG(Download, "Attempting resume of download bundle %s", String(bundlePath, SysStringLen(bundlePath)).ascii().data());

    RetainPtr<CFDataRef> resumeData(AdoptCF, extractResumeDataFromBundle(String(bundlePath, SysStringLen(bundlePath))));
    
    if (!resumeData)
        return E_FAIL;

    if (!delegate)
        return E_FAIL;
    m_delegate = delegate;
    LOG(Download, "Delegate is %p", m_delegate.get());

    CFURLDownloadClient client = {0, this, 0, 0, 0, didStartCallback, willSendRequestCallback, didReceiveAuthenticationChallengeCallback, 
                                  didReceiveResponseCallback, willResumeWithResponseCallback, didReceiveDataCallback, shouldDecodeDataOfMIMETypeCallback, 
                                  decideDestinationWithSuggestedObjectNameCallback, didCreateDestinationCallback, didFinishCallback, didFailCallback};
    
    RetainPtr<CFURLRef> pathURL(AdoptCF, MarshallingHelpers::PathStringToFileCFURLRef(String(bundlePath, SysStringLen(bundlePath))));
    ASSERT(pathURL);

    m_download.adoptCF(CFURLDownloadCreateWithResumeData(0, resumeData.get(), pathURL.get(), &client));

    if (!m_download) {
        LOG(Download, "Failed to create CFURLDownloadRef for resume");    
        return E_FAIL;
    }
    
    m_bundlePath = String(bundlePath, SysStringLen(bundlePath));
    // Attempt to remove the ".download" extension from the bundle for the final file destination
    // Failing that, we clear m_destination and will ask the delegate later once the download starts
    if (m_bundlePath.endsWith(BundleExtension, false)) {
        m_destination = m_bundlePath.copy();
        m_destination.truncate(m_destination.length() - BundleExtension.length());
    } else
        m_destination = String();
    
    CFURLDownloadScheduleWithCurrentMessageQueue(m_download.get());
    CFURLDownloadScheduleDownloadWithRunLoop(m_download.get(), ResourceHandle::loaderRunLoop(), kCFRunLoopDefaultMode);

    LOG(Download, "WebDownload - initWithRequest complete, resumed download of bundle %s", String(bundlePath, SysStringLen(bundlePath)).ascii().data());
    return S_OK;*/
}

bool WebDownload::canResumeDownloadDecodedWithEncodingMIMEType(
        /* [in] */ String)
{
    notImplemented();
    return false;
}

void WebDownload::start()
{
    /*LOG(Download, "WebDownload - Starting download (%p)", this);
    if (!m_download)
        return E_FAIL;

    CFURLDownloadStart(m_download.get());
    // FIXME: 4950477 - CFURLDownload neglects to make the didStart() client call upon starting the download.
    // This is a somewhat critical call, so we'll fake it for now!
    didStart();

    return S_OK;*/
}

void WebDownload::cancel()
{
    /*LOG(Download, "WebDownload - Cancelling download (%p)", this);
    if (!m_download)
        return E_FAIL;

    CFURLDownloadCancel(m_download.get());
    m_download = 0;
    return S_OK;*/
}

void WebDownload::cancelForResume()
{
    /*LOG(Download, "WebDownload - Cancelling download (%p), writing resume information to file if possible", this);
    ASSERT(m_download);
    if (!m_download)
        return E_FAIL;

    HRESULT hr = S_OK;
    RetainPtr<CFDataRef> resumeData;
    if (m_destination.isEmpty()) {
        CFURLDownloadCancel(m_download.get());
        goto exit;
    }

    CFURLDownloadSetDeletesUponFailure(m_download.get(), false);
    CFURLDownloadCancel(m_download.get());

    resumeData = CFURLDownloadCopyResumeData(m_download.get());
    if (!resumeData) {
        LOG(Download, "WebDownload - Unable to create resume data for download (%p)", this);
        goto exit;
    }

    appendResumeDataToBundle(resumeData.get(), m_bundlePath);
   
exit:
    m_download = 0;
    return hr;*/
}

bool WebDownload::deletesFileUponFailure()
{
    /*if (!m_download)
        return E_FAIL;
    *result = CFURLDownloadDeletesUponFailure(m_download.get());
    return S_OK;*/
    return false;
}

String WebDownload::bundlePathForTargetPath(String targetPath)
{
    /*if (!targetPath)
        return E_INVALIDARG;

    String bundle(targetPath, SysStringLen(targetPath));
    if (bundle.isEmpty())
        return E_INVALIDARG;

    if (bundle[bundle.length()-1] == '/')
        bundle.truncate(1);

    bundle += BundleExtension;
    *bundlePath = SysAllocStringLen(bundle.characters(), bundle.length());
    if (!*bundlePath)
        return E_FAIL;
    return S_OK;*/
    return String();
}

WebMutableURLRequest* WebDownload::request()
{
    return m_request;
}

void WebDownload::setDeletesFileUponFailure(bool deletesFileUponFailure)
{
    /*if (!m_download)
        return E_FAIL;
    CFURLDownloadSetDeletesUponFailure(m_download.get(), !!deletesFileUponFailure);
    return S_OK;*/
}

void WebDownload::setDestination(String path, bool allowOverwrite)
{
    /*if (!m_download)
        return E_FAIL;

    m_destination = String(path, SysStringLen(path));
    m_bundlePath = m_destination + BundleExtension;

    CFURLRef pathURL = MarshallingHelpers::PathStringToFileCFURLRef(m_bundlePath);
    CFURLDownloadSetDestination(m_download.get(), pathURL, !!allowOverwrite);
    CFRelease(pathURL);

    LOG(Download, "WebDownload - Set destination to %s", m_bundlePath.ascii().data());

    return S_OK;*/
}

void WebDownload::cancelAuthenticationChallenge(WebURLAuthenticationChallenge*)
{
    /*if (m_download) {
        CFURLDownloadCancel(m_download.get());
        m_download = 0;
    }

    // FIXME: Do we need a URL or description for this error code?
    ResourceError error(String(WebURLErrorDomain), WebURLErrorUserCancelledAuthentication, "", "");
    COMPtr<WebError> webError(AdoptCOM, WebError::createInstance(error));
    m_delegate->didFailWithError(this, webError.get());

    return S_OK;*/
}

void WebDownload::continueWithoutCredentialForAuthenticationChallenge(WebURLAuthenticationChallenge* challenge)
{
    /*COMPtr<WebURLAuthenticationChallenge> webChallenge(Query, challenge);
    if (!webChallenge)
        return E_NOINTERFACE;

    if (m_download)
        CFURLDownloadUseCredential(m_download.get(), 0, webChallenge->authenticationChallenge().cfURLAuthChallengeRef());
    return S_OK;*/
}

void WebDownload::useCredential(WebURLCredential* credential, WebURLAuthenticationChallenge* challenge)
{
    /*COMPtr<WebURLAuthenticationChallenge> webChallenge(Query, challenge);
    if (!webChallenge)
        return E_NOINTERFACE;

    COMPtr<WebURLCredential> webCredential(Query, credential);
    if (!webCredential)
        return E_NOINTERFACE;

    RetainPtr<CFURLCredentialRef> cfCredential(AdoptCF, createCF(webCredential->credential()));

    if (m_download)
        CFURLDownloadUseCredential(m_download.get(), cfCredential.get(), webChallenge->authenticationChallenge().cfURLAuthChallengeRef());
    return S_OK;*/
}

