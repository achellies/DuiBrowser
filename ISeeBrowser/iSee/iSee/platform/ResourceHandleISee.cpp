/*
 * Copyright (C) 2004, 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"

#if USE(WININET)
#include "ResourceHandle.h"
#include "ResourceHandleClient.h"
#include "ResourceHandleInternal.h"
#include "ResourceHandleISee.h"

#include "CString.h"
#include "DocLoader.h"
#include "Document.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "Page.h"
#include "ResourceError.h"
#include "Timer.h"
#include "HTTPParsers.h"
#include <windows.h>
#include <wininet.h>

namespace WebCore {

static unsigned transferJobId = 0;
static HashMap<int, ResourceHandle*>* jobIdMap = 0;

static HINTERNET internetHandle = 0;
static HWND transferJobWindowHandle = 0;
const LPCWSTR kResourceHandleWindowClassName = L"ResourceHandleWindowClass";

// Message types for internal use (keep in sync with kMessageHandlers)
enum {
  handleCreatedMessage = WM_USER,
  requestRedirectedMessage,
  requestCompleteMessage,
  deleteResourceHandle
};

typedef void (ResourceHandle:: *ResourceHandleEventHandler)(LPARAM);
static const ResourceHandleEventHandler messageHandlers[] = {
    &ResourceHandle::onHandleCreated,
    &ResourceHandle::onRequestRedirected,
    &ResourceHandle::onRequestComplete
};

static int addToOutstandingJobs(ResourceHandle* job)
{
    if (!jobIdMap)
        jobIdMap = new HashMap<int, ResourceHandle*>;
    transferJobId++;
    jobIdMap->set(transferJobId, job);
    return transferJobId;
}

static void removeFromOutstandingJobs(int jobId)
{
    if (!jobIdMap)
        return;
    jobIdMap->remove(jobId);
}

static ResourceHandle* lookupResourceHandle(int jobId)
{
    if (!jobIdMap)
        return 0;
    return jobIdMap->get(jobId);
}

static LRESULT CALLBACK ResourceHandleWndProc(HWND hWnd, UINT message,
                                              WPARAM wParam, LPARAM lParam)
{
    if (message >= handleCreatedMessage) {
        UINT index = message - handleCreatedMessage;
        if (index < _countof(messageHandlers)) {
            unsigned jobId = (unsigned) wParam;
            ResourceHandle* job = lookupResourceHandle(jobId);
            if (job) {
                ASSERT(job->d->m_jobId == jobId);
                ASSERT(job->d->m_threadId == GetCurrentThreadId());
                (job->*(messageHandlers[index]))(lParam);
            }
            return 0;
        }
		else if (deleteResourceHandle == message)
		{	// delete ResourceHandle object
			unsigned jobId = (unsigned) wParam;
            ResourceHandle* job = lookupResourceHandle(jobId);
            if (job) {
                ASSERT(job->d->m_jobId == jobId);
                delete job;
			}
			return 0;
		}
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

static void initializeOffScreenResourceHandleWindow()
{
    if (transferJobWindowHandle)
        return;

    WNDCLASS wc;
    memset(&wc, 0, sizeof(WNDCLASS));
    wc.lpfnWndProc    = ResourceHandleWndProc;
    wc.hInstance      = Page::instanceHandle();
    wc.lpszClassName  = kResourceHandleWindowClassName;
    RegisterClass(&wc);

    transferJobWindowHandle = CreateWindow(kResourceHandleWindowClassName, 0, 0, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
        NULL, 0, Page::instanceHandle(), 0);
}

ResourceHandleInternal::~ResourceHandleInternal()
{
    if (m_fileHandle != INVALID_HANDLE_VALUE)
        CloseHandle(m_fileHandle);
}

ResourceHandle::~ResourceHandle()
{
    if (d->m_jobId)
        removeFromOutstandingJobs(d->m_jobId);
}

void ResourceHandle::onHandleCreated(LPARAM lParam)
{
    if (!d->m_resourceHandle) {
        d->m_resourceHandle = HINTERNET(lParam);
        if (d->status != 0) {
            // We were canceled before Windows actually created a handle for us, close and delete now.
            InternetCloseHandle(d->m_resourceHandle);
			// delete this;
			PostMessage(transferJobWindowHandle, deleteResourceHandle, (WPARAM)d->m_jobId, 0);
            return;
        }

        if (request().httpMethod() == "POST") {
            // FIXME: Too late to set referrer properly.
            String urlStr = request().url().path();
            int fragmentIndex = urlStr.find('#');
            if (fragmentIndex != -1)
                urlStr = urlStr.left(fragmentIndex);
            static LPCSTR accept[2]={"*/*", NULL};
            HINTERNET urlHandle = HttpOpenRequestA(d->m_resourceHandle, 
                                                   "POST", urlStr.latin1().data(), 0, 0, accept,
                                                   INTERNET_FLAG_KEEP_CONNECTION | 
                                                   INTERNET_FLAG_FORMS_SUBMIT |
                                                   INTERNET_FLAG_RELOAD |
                                                   INTERNET_FLAG_NO_CACHE_WRITE |
                                                   INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
                                                   INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP,
                                                   (DWORD_PTR)d->m_jobId);
            if (urlHandle == INVALID_HANDLE_VALUE) {
                InternetCloseHandle(d->m_resourceHandle);
				// delete this;
                PostMessage(transferJobWindowHandle, deleteResourceHandle, (WPARAM)d->m_jobId, 0);
            }
        }
    } else if (!d->m_secondaryHandle) {
        d->m_secondaryHandle = HINTERNET(lParam);
        
        // Need to actually send the request now.
        String headers = "Content-Type: application/x-www-form-urlencoded\n";
        headers += "Referer: ";
        headers += d->m_postReferrer;
        headers += "\n";
        const CString& headersLatin1 = headers.latin1();
		CString formData = request().httpBody()->flattenToString().latin1();
        INTERNET_BUFFERSA buffers;
        memset(&buffers, 0, sizeof(buffers));
        buffers.dwStructSize = sizeof(INTERNET_BUFFERSA);
		buffers.lpcszHeader = headersLatin1.data();
        buffers.dwHeadersLength = headers.length();
        buffers.dwBufferTotal = formData.length();
        
        d->m_bytesRemainingToWrite = formData.length();
        d->m_formDataString = (char*)malloc(formData.length());
        d->m_formDataLength = formData.length();
		strncpy(d->m_formDataString, formData.data(), formData.length());
        d->m_writing = true;
        HttpSendRequestExA(d->m_secondaryHandle, &buffers, 0, 0, (DWORD_PTR)d->m_jobId);
        // FIXME: add proper error handling
    }
}

void ResourceHandle::onRequestRedirected(LPARAM lParam)
{
    // If already canceled, then ignore this event.
    if (d->status != 0)
        return;

    ResourceRequest request((StringImpl*) lParam);
    ResourceResponse redirectResponse; // FIXME: use d->m_response ???
    client()->willSendRequest(this, request, redirectResponse);
}

static BOOL GetHeader(HINTERNET hHttp, DWORD dwHeaderType, LPVOID *lpOutBuffer)
{
	DWORD dwSize = 0;
	*lpOutBuffer = NULL;

retry:
	//call HttpQueryInfo.
	//first time with a zero buffer size to get the size of the buffer
	//needed and to check if the header exists
	if(!HttpQueryInfo(hHttp, dwHeaderType, *lpOutBuffer, &dwSize, NULL))
	{
		// check if the header was not found
		if (GetLastError() == ERROR_HTTP_HEADER_NOT_FOUND)
		{
			return FALSE;
		}
		else
		{
			//check if the buffer was insufficient
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				//allocate the buffer
				*lpOutBuffer = new char[dwSize + 2];
				if (*lpOutBuffer)
					goto retry;
			}
			return FALSE;
		}
	}

	return TRUE;
}

void ResourceHandle::onRequestComplete(LPARAM lParam)
{
    if (d->m_writing) {
        DWORD bytesWritten;
        InternetWriteFile(d->m_secondaryHandle,
                          d->m_formDataString + (d->m_formDataLength - d->m_bytesRemainingToWrite),
                          d->m_bytesRemainingToWrite,
                          &bytesWritten); // Function is synchronous under WinCE.
        d->m_bytesRemainingToWrite -= bytesWritten;
        if (!d->m_bytesRemainingToWrite) {
            // End the request.
            d->m_writing = false;
            HttpEndRequest(d->m_secondaryHandle, 0, 0, (DWORD_PTR)d->m_jobId);
            free(d->m_formDataString);
            d->m_formDataString = 0;
			// delete this;
			PostMessage(transferJobWindowHandle, deleteResourceHandle, (WPARAM)d->m_jobId, 0);
        }
        return;
    }

	HINTERNET handle = (request().httpMethod() == "POST") ? d->m_secondaryHandle : d->m_resourceHandle;
	LPSTR lpszData;		// buffer for the data
	DWORD dwSize;		// size of the data available
	DWORD dwDownloaded;	// size of the downloaded data
	BOOL ok = FALSE;

	// This loop handles reading the data.
	do
	{
		// The call to InternetQueryDataAvailable determines the amount of
		// data available to download.
		if (!InternetQueryDataAvailable(handle, &dwSize, 0, 0))
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				iSeeLogA("WM_REQUEST_COMPLETE is ERROR_IO_PENDING 1\n");
				return; // 等待下一次数据来临
			}
			else // 出现错误
				break;
		}
		else
		{	
			// Allocates a buffer of the size returned by InternetQueryDataAvailable
			lpszData = new char[dwSize + 1];

			// Reads the data from the HINTERNET handle.
			dwDownloaded = 0;
			if(!InternetReadFile(handle, (LPVOID)lpszData, dwSize, &dwDownloaded))
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					if (dwDownloaded == 0) // 无数据
					{
						iSeeLogA("WM_REQUEST_COMPLETE is ERROR_IO_PENDING 2\n");
						delete[] lpszData;
						return; // 等待下一次数据来临
					} // else 收到数据，处理收到数据
				}
				else
				{	// 出现错误
					delete[] lpszData;
					break;
				}
			}
			
			iSeeLogA("WM_REQUEST_COMPLETE %d\n", dwDownloaded);
			// Adds a null terminator to the end of the data buffer
			lpszData[dwDownloaded]='\0';
			
			if (dwDownloaded > 0)
			{
				if (!hasReceivedResponse()) {
					setHasReceivedResponse();

					ResourceResponse response;
					LPVOID lpOutBuffer;

					response.setUrl(request().url());

					if (GetHeader(handle, HTTP_QUERY_CONTENT_LENGTH, &lpOutBuffer))
					{
						DWORD contentLength = 0;
						swscanf((WCHAR *)lpOutBuffer, L"%u", &contentLength);
						response.setExpectedContentLength(static_cast<long long int>(contentLength)); 
						delete [](char *)lpOutBuffer;
					}

					if (GetHeader(handle, HTTP_QUERY_STATUS_CODE, &lpOutBuffer))
					{
						int httpCode = 0;
						swscanf((WCHAR *)lpOutBuffer, L"%d", &httpCode);
						response.setHTTPStatusCode(httpCode);
						delete [](char *)lpOutBuffer;
					}

					if (GetHeader(handle, HTTP_QUERY_CONTENT_TYPE, &lpOutBuffer))
					{
						response.setMimeType(extractMIMETypeFromMediaType((UChar *)lpOutBuffer));
						response.setTextEncodingName(extractCharsetFromMediaType((UChar *)lpOutBuffer));
						delete [](char *)lpOutBuffer;
					}

					if (GetHeader(handle, HTTP_QUERY_CONTENT_DISPOSITION, &lpOutBuffer))
					{
						response.setSuggestedFilename(filenameFromHTTPContentDisposition((UChar *)lpOutBuffer));
						delete [](char *)lpOutBuffer;
					}

					client()->willSendRequest(this, d->m_request, response); // FIXME:need ???
					client()->didReceiveResponse(this, response);
				}
				client()->didReceiveData(this, lpszData, dwDownloaded, 0);
			}

			// Clear data buffer
			delete[] lpszData;

			// Check the size of the remaining data.  If it is zero, break.
			if (dwDownloaded == 0)
			{	// 接收数据完成
				iSeeLogA("WM_REQUEST_COMPLETE ended\n");
				ok = TRUE;
				break;
			}
		}
	} while(TRUE); // 循环接收数据

    PlatformDataStruct platformData;
    platformData.errorString = 0;
    platformData.error = 0;
    platformData.loaded = ok;

    if (!ok) {
        DWORD errorStringChars = 0;
        if (!InternetGetLastResponseInfo(&platformData.error, 0, &errorStringChars)) {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                platformData.errorString = new TCHAR[errorStringChars];
                InternetGetLastResponseInfo(&platformData.error, platformData.errorString, &errorStringChars);
				delete []platformData.errorString;
            }
        }
    }
    
    if (d->m_secondaryHandle)
        InternetCloseHandle(d->m_secondaryHandle);
    InternetCloseHandle(d->m_resourceHandle);

    client()->didFinishLoading(this);
    // delete this;
	PostMessage(transferJobWindowHandle, deleteResourceHandle, (WPARAM)d->m_jobId, 0);
}

static void __stdcall transferJobStatusCallback(HINTERNET handle,
                                                DWORD_PTR jobId,
                                                DWORD internetStatus,
                                                LPVOID statusInformation,
                                                DWORD statusInformationLength)
{
#ifdef RESOURCE_LOADER_DEBUG
    char buf[64];
    snprintf(buf, sizeof(buf), "status-callback: status=%u, job=%p\n",
              internetStatus, jobId);
    OutputDebugStringA(buf);
#endif

    UINT msg;
    LPARAM lParam;

    switch (internetStatus) {
    case INTERNET_STATUS_HANDLE_CREATED:
        // tell the main thread about the newly created handle
        msg = handleCreatedMessage;
        lParam = (LPARAM) LPINTERNET_ASYNC_RESULT(statusInformation)->dwResult;
        break;
    case INTERNET_STATUS_REQUEST_COMPLETE:
#ifdef RESOURCE_LOADER_DEBUG
        snprintf(buf, sizeof(buf), "request-complete: result=%p, error=%u\n",
            LPINTERNET_ASYNC_RESULT(statusInformation)->dwResult,
            LPINTERNET_ASYNC_RESULT(statusInformation)->dwError);
        OutputDebugStringA(buf);
#endif
        // tell the main thread that the request is done
        msg = requestCompleteMessage;
        lParam = 0;
        break;
    case INTERNET_STATUS_REDIRECT:
        // tell the main thread to observe this redirect (FIXME: we probably
        // need to block the redirect at this point so the application can
        // decide whether or not to follow the redirect)
        msg = requestRedirectedMessage;
        lParam = (LPARAM) StringImpl::create((const UChar*) statusInformation,
                                         statusInformationLength).releaseRef();
        break;
    case INTERNET_STATUS_USER_INPUT_REQUIRED:
        // FIXME: prompt the user if necessary
        ResumeSuspendedDownload(handle, 0);
    case INTERNET_STATUS_STATE_CHANGE:
        // may need to call ResumeSuspendedDownload here as well
    default:
        return;
    }

    PostMessage(transferJobWindowHandle, msg, (WPARAM) jobId, lParam);
}

bool ResourceHandle::start(Frame* frame)
{
    ref();
	
	initializeOffScreenResourceHandleWindow();
	d->m_jobId = addToOutstandingJobs(this);

    if (request().url().isLocalFile()) {
        String path = request().url().path();
        // windows does not enjoy a leading slash on paths
        if (path[0] == '/')
            path = path.substring(1);
        // FIXME: This is wrong. Need to use wide version of this call.
		d->m_fileHandle = CreateFileW(path.charactersWithNullTermination(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        // FIXME: perhaps this error should be reported asynchronously for
        // consistency.
        if (d->m_fileHandle == INVALID_HANDLE_VALUE) {
			// delete this;
			PostMessage(transferJobWindowHandle, deleteResourceHandle, (WPARAM)d->m_jobId, 0);
            return false;
        }

        d->m_fileLoadTimer.startOneShot(0.0);
        return true;
    } else {
        if (!internetHandle) {
			KURL tmpUrl;
            String userAgentStr = frame->loader()->userAgent(tmpUrl) + String("", 1);
            LPCWSTR userAgent = reinterpret_cast<const WCHAR*>(userAgentStr.characters());
            // leak the Internet for now
            internetHandle = InternetOpen(userAgent, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, INTERNET_FLAG_ASYNC);
        }
        if (!internetHandle) {
			// delete this;
			PostMessage(transferJobWindowHandle, deleteResourceHandle, (WPARAM)d->m_jobId, 0);
            return false;
        }
        static INTERNET_STATUS_CALLBACK callbackHandle = 
            InternetSetStatusCallback(internetHandle, transferJobStatusCallback);

        DWORD flags =
            INTERNET_FLAG_KEEP_CONNECTION |
            INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
            INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP;

        // For form posting, we can't use InternetOpenURL.  We have to use
        // InternetConnect followed by HttpSendRequest.
        HINTERNET urlHandle;
        String referrer = frame->loader()->referrer();
		if (request().httpMethod() == "POST") {
            d->m_postReferrer = referrer;
            String host = request().url().host();
            urlHandle = InternetConnectA(internetHandle, host.latin1().data(),
                                         request().url().port(),
                                         NULL, // no username
                                         NULL, // no password
                                         INTERNET_SERVICE_HTTP,
                                         flags, (DWORD_PTR)d->m_jobId);
        } else {
            String urlStr = request().url().string();
            int fragmentIndex = urlStr.find('#');
            if (fragmentIndex != -1)
                urlStr = urlStr.left(fragmentIndex);
            String headers;
            if (!referrer.isEmpty())
                headers += String("Referer: ") + referrer + "\r\n";

            urlHandle = InternetOpenUrlA(internetHandle, urlStr.latin1().data(),
                                         headers.latin1().data(), headers.length(),
                                         flags, (DWORD_PTR)d->m_jobId);
        }

        if (urlHandle == INVALID_HANDLE_VALUE) {
			// delete this;
			PostMessage(transferJobWindowHandle, deleteResourceHandle, (WPARAM)d->m_jobId, 0);
            return false;
        }
        d->m_threadId = GetCurrentThreadId();

        return true;
    }
}

void ResourceHandle::fileLoadTimer(Timer<ResourceHandle>* timer)
{
	// since the code in headerCallback will not have run for local files
    // the code to set the URL and fire didReceiveResponse is never run,
    // which means the ResourceLoader's response does not contain the URL.
    // Run the code here for local files to resolve the issue.
    // TODO: See if there is a better approach for handling this.
	ResourceResponse response;
	response.setUrl(request().url());
    if (client())
        client()->didReceiveResponse(this, response);
    response.setResponseFired(true);

    bool result = false;
    DWORD bytesRead = 0;

    do {
        const int bufferSize = 8192;
        char buffer[bufferSize];
        result = ReadFile(d->m_fileHandle, &buffer, bufferSize, &bytesRead, NULL); 
        if (result && bytesRead)
            client()->didReceiveData(this, buffer, bytesRead, 0);
        // Check for end of file. 
    } while (result && bytesRead);

    // FIXME: handle errors better

    CloseHandle(d->m_fileHandle);
    d->m_fileHandle = INVALID_HANDLE_VALUE;

    client()->didFinishLoading(this);
	
	// delete this;
	PostMessage(transferJobWindowHandle, deleteResourceHandle, (WPARAM)d->m_jobId, 0);
}

void ResourceHandle::cancel()
{
    if (d->m_resourceHandle)
        InternetCloseHandle(d->m_resourceHandle);
    else
        d->m_fileLoadTimer.stop();

    client()->didFinishLoading(this); 

    if (!d->m_resourceHandle)
        // Async load canceled before we have a handle -- mark ourselves as in error, to be deleted later.
        // FIXME: need real cancel error
        client()->didFail(this, ResourceError());
	
	// delete this;
	PostMessage(transferJobWindowHandle, deleteResourceHandle, (WPARAM)d->m_jobId, 0);
}

void ResourceHandle::setHasReceivedResponse(bool b)
{
    d->m_hasReceivedResponse = b;
}

bool ResourceHandle::hasReceivedResponse() const
{
    return d->m_hasReceivedResponse;
}

PassRefPtr<SharedBuffer> ResourceHandle::bufferedData()
{
    return 0;
}

bool ResourceHandle::supportsBufferedData()
{
    return false;
}

void ResourceHandle::setDefersLoading(bool defers)
{
    d->m_defersLoading = defers;
}

bool ResourceHandle::willLoadFromCache(ResourceRequest&)
{
    return false;
}

bool ResourceHandle::loadsBlocked()
{
    return false;
}

void ResourceHandle::loadResourceSynchronously(const ResourceRequest& request, ResourceError& error, ResourceResponse& response, Vector<char>& data, Frame*)
{
#if 0
    WebCoreSynchronousLoader syncLoader;
    ResourceHandle handle(request, &syncLoader, true, false, true);

    ResourceHandleManager* manager = ResourceHandleManager::sharedInstance();

    manager->dispatchSynchronousJob(&handle);

    error = syncLoader.resourceError();
    data = syncLoader.data();
    response = syncLoader.resourceResponse();
#endif
}

} // namespace WebCore
#endif // #if USE(WININET)
