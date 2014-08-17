/*
Copyright (C) 2008-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/////////////////////////////////////////////////////////////////////////////
// BCResourceHandleManagerEA.cpp
// Created by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Resource-related classes:
//     ResourceHandleManager
//     ResourceHandle
//     ResourceHandleInternal
//     ResourceHandleClient
//     ResourceRequest : public ResourceRequestBase
//     ResourceResponse : public ResourceResponseBase
//     
//
// In order to implement the loading of these resources in a way that can
// be plugged in by the user, we add the folloowing classes:
//     TransportHandler
//     TransportInfo
//     TransportServer
//     ResourceHandleManager::JobInfo
//
// Useful test pages:
//     http://www.snee.com/xml/crud/posttest.html
//     http://www.sacregioncommuterclub.org/cookie.asp
//
/////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "ResourceHandleManager.h"
#include "CString.h"
#include "MIMETypeRegistry.h"
#include "NotImplemented.h"
#include "ResourceError.h"
#include "ResourceHandle.h"
#include "ResourceHandleInternal.h"
#include "HTTPParsers.h"
#include "Base64.h"
#include "Timer.h"
#include <ctype.h>
#include <errno.h>
#include <wtf/Vector.h>
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebKitFPUPrecision.h>
#include <EAWebKit/Internal/EAWebKitString.h>   // For Stricmp and friends.
#include <EAWebKit/internal/EAWebKitDomainFilter.h>
#include <EAIO/FnEncode.h>          // For Strlcpy and friends.
#include <EASTL/fixed_substring.h>
#include "BCFormDataStreamEA.h"
#include <wtf/StringExtras.h>
#include <EAWebKit/internal/EAWebKitViewHelper.h> // For multiview support
#include "WebError.h"

#ifdef GetJob // If windows.h has #defined GetJob to GetJobA...
    #undef GetJob
#endif


namespace Local
{
    void ConvertWebCoreStr16ToEAString8(const WebCore::String& s16, EA::WebKit::FixedString8_128& s8)
    {
        uint32_t s16Len = s16.length()+1;       //webcore strings do not include the NULL terminator
        s8.resize(s16Len);
        int nRequiredDestLength = EA::Internal::Strlcpy(&s8[0], s16.characters(), s8.length(), s16Len);

        if((uint32_t)nRequiredDestLength > s8.length())
        {
            s8.resize(nRequiredDestLength);
            EA::Internal::Strlcpy(&s8[0], s16.characters(), s8.length(), s16Len);
        } 
    }


    // Converts a WebCore ResourceHandle to its associated EAWebKit View.
    EA::WebKit::View* GetView(WebCore::ResourceHandle* pRH)
    {
        EAW_ASSERT(pRH != NULL);
        if(pRH)
        {
            WebCore::ResourceHandleInternal* pRHI = pRH->getInternal();

            EAW_ASSERT(pRHI != NULL);
            if(pRHI)
            {
                WebCore::ResourceHandleClient* pRHC = pRHI->client();

                EAW_ASSERT(pRHC != NULL);
                if(pRHC)
                {
                    WebCore::Frame* pFrame =  pRHC->getFrame();

                    EAW_ASSERT(pFrame != NULL);
                    if(pFrame)
                    {
                        EA::WebKit::View* pView = EA::WebKit::GetView(pFrame);

                        EAW_ASSERT(pView != NULL);
                        return pView;
                    }
                }
            }
        }

        return NULL;
    }

} // namespace


namespace WKAL {


/////////////////////////////////////////////////////////////////////////////
// ResourceHandleManager
/////////////////////////////////////////////////////////////////////////////

ResourceHandleManager::JobInfo::JobInfo()
  : mId(0),
    mpRH(NULL),
    mJobState(kJSInit),
    mpTH(NULL),
    mTInfo(),
    mbTHInitialized(false),
    mbTHShutdown(false),
    mbSuccess(true),
    mbSynchronous(false),
    mbAuthorizationRequired(false),
	mbAsyncJobPaused(false),
    mProcessInfo(EA::WebKit::kVProcessTypeTransportJob, EA::WebKit::kVProcessStatusStarted) 
    #if EAWEBKIT_DUMP_TRANSPORT_FILES
   ,mFileImage()
    #endif
{
	NOTIFY_PROCESS_STATUS(mProcessInfo,EA::WebKit::kVProcessStatusStarted);
}


ResourceHandleManager* ResourceHandleManager::m_pInstance = NULL;

const double kPollTimeSeconds = 0.016;
ResourceHandleManager::ResourceHandleManager()
    : m_downloadTimer(this, &ResourceHandleManager::downloadTimerCallback)
    , m_cookieFilePath()
    , m_resourceHandleList()
    , m_runningJobs(0)
    , m_pollTimeSeconds(kPollTimeSeconds)
    , m_timeoutSeconds(120)
    , m_maxConcurrentJobs(32)
    , m_cookieManager()
    , m_AuthenticationManager(this)
    , m_THInfoList()
    , m_JobInfoList()
    , m_JobIdNext(0)
    , m_CondemnedJobsExist(false)
    #if EAWEBKIT_DUMP_TRANSPORT_FILES
	, m_DebugWriteFileImages(false) 
    , m_DebugFileImageDirectory()
    #endif
    , m_readVolume(0)
    , m_writeVolume(0)
{
    const EA::WebKit::Parameters& parameters = EA::WebKit::GetParameters();

    m_maxConcurrentJobs = parameters.mMaxTransportJobs;
    m_pollTimeSeconds   = parameters.mTransportPollTimeSeconds;
	if(m_pollTimeSeconds >= kPollTimeSeconds)//We want to make sure that we poll at least 60 Hz
		m_pollTimeSeconds = kPollTimeSeconds;
    
#ifdef EA_DEBUG
        m_timeoutSeconds = 1000000;
    #else
        m_timeoutSeconds = parameters.mPageTimeoutSeconds;
        if(m_timeoutSeconds < 120)
           m_timeoutSeconds = 120;
    #endif

    // To consider: Move this init to somewhere else, like with the cookie manager.
    m_AuthenticationManager.Init();

#if EAWEBKIT_DUMP_TRANSPORT_FILES
	char8_t baseDir[EA::IO::kMaxDirectoryLength];
	memset(baseDir, 0, EA::IO::kMaxDirectoryLength); 
	if(EA::WebKit::GetFileSystem()->GetBaseDirectory(baseDir, EA::IO::kMaxDirectoryLength - 1)
		&& baseDir[0])
	{
		EA::IO::Path::PathString8 fullPath(baseDir);
		EA::IO::Path::Append(fullPath, "TransportFilesDump");
		EA::IO::Path::EnsureTrailingSeparator(fullPath);

		EA::WebKit::FixedString8_256 fullPathStr(fullPath.c_str());
		EA::WebKit::ConvertToString16(fullPathStr, m_DebugFileImageDirectory);
	}
	else //Old code path.
	{
		#if defined(EA_PLATFORM_WINDOWS)
			m_DebugFileImageDirectory = L"c:\\temp\\TransportFilesDump\\";
		#elif defined(EA_PLATFORM_WINCE)
			m_DebugFileImageDirectory = L"\\temp\\TransportFilesDump\\";
		#elif defined(EA_PLATFORM_XENON)
			m_DebugFileImageDirectory = L"d:\\temp\\TransportFilesDump\\";
		#elif defined(EA_PLATFORM_PS3)
			m_DebugFileImageDirectory = L"/app_home/TransportFilesDump/";
		#endif
	}

	
	EA::WebKit::FixedString8_256 debugFileImageDirectory8;
	EA::WebKit::ConvertToString8(m_DebugFileImageDirectory, debugFileImageDirectory8);

	EA::WebKit::GetFileSystem()->MakeDirectory(debugFileImageDirectory8.c_str());
	#endif
}


ResourceHandleManager::~ResourceHandleManager()
{
    CondemnAllTHJobs();
    ProcessTHJobs();
    RemoveTransportHandlers();

    //Nicki Vankoughnett:  TransportHandlerFileCache is not added as a transport handler directly due to the special nature of the cache.
    m_THFileCache.Shutdown(NULL);
    m_AuthenticationManager.Shutdown();
    m_cookieManager.Shutdown();
}


void ResourceHandleManager::setCookieFilePath(const char* cookieFilePath)
{
    m_cookieFilePath = cookieFilePath;
}


ResourceHandleManager* ResourceHandleManager::sharedInstance()
{
    if(!m_pInstance)
        m_pInstance = new ResourceHandleManager;

    return m_pInstance;
}


void ResourceHandleManager::staticFinalize()
{
    delete m_pInstance;
    m_pInstance = NULL;
}


void ResourceHandleManager::downloadTimerCallback(Timer<ResourceHandleManager>* /*timer*/)
{
    int runningHandles = 0;

    startScheduledJobs();

    runningHandles += ProcessTHJobs();

    m_AuthenticationManager.Tick();

    const bool started = startScheduledJobs(); // new jobs might have been added in the meantime

    if(!m_downloadTimer.isActive() && (started || (runningHandles > 0) || (m_AuthenticationManager.HasJobs())))
        m_downloadTimer.startOneShot(m_pollTimeSeconds);
}


// This is the first function called to initiate loading a resource (e.g. over HTTP).
void ResourceHandleManager::add(ResourceHandle* pRH)
{
    // The pRH object has been ref()'d by the caller. We keep that ref until later when we dispose of the job.

    // We don't process the list right away because we don't want any 
    // callbacks from the processing to occur until in a timer callback
    // due to possible reentrancy issues.
    #if defined(EA_DEBUG)
        KURL   kurl    = pRH->request().url();
        String url     = kurl.string();
        String sScheme = kurl.protocol();
        const char16_t* pScheme = sScheme.charactersWithNullTermination();
        EA::WebKit::TransportHandler* pTH = GetTransportHandler(pScheme);
        if( (!pTH)&& (!kurl.protocolIs("data")) )  // 6/17/09 CSidhall. Data scheme is supported in code.
		{
			EAW_ASSERT_FORMATTED(pTH != NULL, "There is no transport handler for this scheme: %ls\n", pScheme); // There is no transport handler for this scheme.						
		}

    #endif

    m_resourceHandleList.append(pRH);

    if(!m_downloadTimer.isActive())
        m_downloadTimer.startOneShot(m_pollTimeSeconds);
}


void ResourceHandleManager::cancel(ResourceHandle* pRH)
{
    if(!removeScheduledJob(pRH)) // If the pRH has started already and thus isn't in our to-start queue.
    {
        ResourceHandleInternal* pRHI = pRH->getInternal();

        pRHI->m_cancelled = true; // We'll pick this up later and shut down the job.

        if(!m_downloadTimer.isActive())
            m_downloadTimer.startOneShot(m_pollTimeSeconds);
    }
}


// Removes a job from our list of jobs that are queued for processing but haven't started yet.
bool ResourceHandleManager::removeScheduledJob(ResourceHandle* pRH)
{
    for(int i = 0, iEnd = m_resourceHandleList.size(); i < iEnd; i++)
    {
        if(pRH == m_resourceHandleList[i])
        {
            //+ 5/07/09 CSidhall - Added deref count for exit during load leak fix
            pRH->deref();      
            //-
            m_resourceHandleList.remove(i);
            return true;
        }
    }

    return false;
}


// Goes through our list of jobs that are queued and starts them. 
// Then removes them from the queue (m_resourceHandleList).
bool ResourceHandleManager::startScheduledJobs()
{
    // To consider: Create a separate stack of jobs for each URI domain.
    bool started = false;
	
    while (!m_resourceHandleList.isEmpty() && (m_runningJobs < m_maxConcurrentJobs))
    {
        ResourceHandle* pRH = m_resourceHandleList[0];
        // 6/17/09 CSidhall - Agree with the initial comment bellow.
        // This should be done before startjob for if a job is terminated inside start job, it gets removed from this list and then we would 
        // delete the next job which was not even started yet. Data scheme jobs for example are now removed inside the startJob() for fix lost pointers.
        
        m_resourceHandleList.remove(0);  // This line is after the startJob line, but ResourceHandle isn't currently ref-counted by m_resourceHandleList, so perhaps it would be better if it was up a line.

        startJob(pRH);
        started = true;
    }

    return started;
}


static void parseDataURI(ResourceHandle* handle)
{
    // Example data URI:
    //     <img src="data:image/png;base64,QWxhZGRpbjpvcGVuIHNlc2FtZQ==" alt="Red dot"/>

    String data = handle->request().url().string();

    EAW_ASSERT(data.startsWith("data:", false));

    String header;
    bool base64 = false;

    int index = data.find(',');
    if (index != -1) {
        header = data.substring(5, index - 5).lower();
        data = data.substring(index + 1);

        if (header.endsWith(";base64")) {
            base64 = true;
            header = header.left(header.length() - 7);
        }
    } else
        data = String();

    data = decodeURLEscapeSequences(data);

    size_t outLength = 0;
    char* outData = 0;
    if (base64 && !data.isEmpty()) {
        Vector<char> out;

        // EA's base64Decode implementation is standards-conforming.
        if (base64Decode(data.latin1().data(), data.length(), out)) {
            outData = (char*)WTF::fastMalloc(out.size());
            for (size_t i = 0; i < out.size(); i++)
                outData[i] = out[i];
            outLength = out.size();
        }
        else
            data = String();
    }

    if (header.isEmpty())
        header = "text/plain;charset=US-ASCII";

    ResourceHandleClient* client = handle->getInternal()->client();

    ResourceResponse response;

    response.setMimeType(extractMIMETypeFromMediaType(header));
    response.setTextEncodingName(extractCharsetFromMediaType(header));
    if (outData)
        response.setExpectedContentLength(outLength);
    else
        response.setExpectedContentLength(data.length());
    response.setHTTPStatusCode(200);

    client->didReceiveResponse(handle, response);

    if (outData)
        client->didReceiveData(handle, outData, outLength, 0);
    else
        client->didReceiveData(handle, data.latin1().data(), data.length(), 0);

    if(outData)
        WTF::fastFree(outData);

    client->didFinishLoading(handle);

    // 6/17/09 CSidhall - This is somewhat of a special case where we need to terminate the job but the job
    // never got added to a job list so we need to destroy it here manually or the pointer ends up leaking.
    // Also for this to work, the resource needs to already have been removed from the resource list (modified the startScheduledJobs() too)
    handle->deref();

}


// This function needs to synchronously (i.e. blocking in place) do a resource load.
void ResourceHandleManager::dispatchSynchronousJob(ResourceHandle* pRH)
{
    const KURL& kurl = pRH->request().url();

    if(kurl.protocolIs("data"))
        parseDataURI(pRH);
    else
    {
        const int jobId = initializeHandle(pRH, true);

        if(jobId)
        {
            PauseAsyncJobs();
            
			while(GetJob(jobId))
			{
			               
                ProcessTHJobs();

                // 1/22/10 CSidhall - Added transport tick update or this could end up locked in the while loop.
				TickTransportHandlers();
            }
			
			ResumeAsyncJobs();
        }
    }
}

void ResourceHandleManager::PauseAsyncJobs()
{
	for(JobInfoList::iterator it = m_JobInfoList.begin(); it != m_JobInfoList.end(); ++it)
    {
        JobInfo& jobInfo = *it;
		
		if(!jobInfo.mbSynchronous)
			jobInfo.mbAsyncJobPaused = true;
	}
}

void ResourceHandleManager::ResumeAsyncJobs()
{
	for(JobInfoList::iterator it = m_JobInfoList.begin(); it != m_JobInfoList.end(); ++it)
    {
        JobInfo& jobInfo = *it;
		
		if(!jobInfo.mbSynchronous)
			jobInfo.mbAsyncJobPaused = false;
	}
}
void ResourceHandleManager::startJob(ResourceHandle* pRH)
{
    const KURL& kurl = pRH->request().url();

    if(kurl.protocolIs("data"))
        parseDataURI(pRH);
    else
        initializeHandle(pRH, false);
}


// Returns a job id. 
int ResourceHandleManager::initializeHandle(ResourceHandle* pRH, bool bSynchronous)
{
    KURL            kurl;
    String          url;
    String          sScheme;
    const char16_t* pScheme;

    TryAgain:
    kurl    = pRH->request().url();
    kurl.setRef(""); // Remove any fragment part.
    url     = kurl.string();
    sScheme = kurl.protocol();
    pScheme = sScheme.charactersWithNullTermination();

    EA::WebKit::TransportHandler* pTH = GetTransportHandler(pScheme);

    if(!pTH)
    {
        WebCore::ResourceRequest& rRequest = const_cast<WebCore::ResourceRequest&>(pRH->request());  // ResourceHandle allows only const access to the ResourceRequest, but we need to add a header.

        kurl = rRequest.url();
        url  = kurl.string();
        url.replace(0, sScheme.length(), String("http"));
        rRequest.setURL(KURL(url));

        goto TryAgain;
    }

    if(kurl.isLocalFile() || (pTH && pTH->IsLocalFile())) // If using "file" scheme, as in "file://" URIs...
    {
        String query = kurl.query();

        // Remove any query part sent to a local file.
        if(!query.isEmpty())
            url = url.left(url.find(query));

        // Determine the MIME type based on the path.
        ResourceHandleInternal* pRHI = pRH->getInternal();
        pRHI->m_response.setMimeType(MIMETypeRegistry::getMIMETypeForPath(url));
    }

    int jobId = 0;

	if(pTH)
	{	
		jobId = AddTHJob(pRH, pTH, kurl, url, pScheme, bSynchronous);
		// Note by Arpit Baldeva: We want all the links to be filtered as per our domain filter. There does not seem a central/well known place in the 
		//code base whereevery link makes through. But this place does. So putting it here.
		//Ideally, I'd like to integrate it with an in-built security module for navigation but could not find that either.
		
		//Domain filter call is efficient in the sense that if the user has not set it up, it is just a "if" check (+ func call).
		if(!EA::WebKit::EAWebKitDomainFilter::GetInstance().CanNavigateToURL(kurl))
		{
            JobInfo* pJob = GetJob(jobId); 
            // Get the job view for the notification
            EA::WebKit::View* pView = NULL;
            if(pJob)
                pView = pJob->mTInfo.mpView; 
            CondemnTHJob(pJob); //slow but safe
			//CondemnTHJob(&(m_JobInfoList.back())); //fast but error prone
			
			//Notify the user that we filtered this link. They may want to do something with it.
			EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
			if(pViewNotification)
			{
				EA::WebKit::LinkNotificationInfo lni;
				lni.mpView = pView;
                lni.mbURIInterceptedByDomainFiltering = true;
				const WebCore::String& webCoreURI = kurl.string();
				GetFixedString(lni.mOriginalURI)->assign(webCoreURI.characters(), webCoreURI.length());
				pViewNotification->LinkSelected(lni);
			}
		}
	}

    if(jobId)
        m_runningJobs++;

    return jobId;
}


static void GetPathFromFileURL(const KURL& kurl, EA::WebKit::FixedString8_128& sPath)
{
    WebCore::String           sURLPath = kurl.path();
    EA::WebKit::FixedString16_128 sPath16(sURLPath.characters(), sURLPath.length());

    // Look for Microsoft-style paths (e.g. "/C|/windows/test.html", to convert to "C:\windows\test.html")
    #if defined(_MSC_VER) || defined(EA_PLATFORM_WINDOWS)
        if((sPath16[0] == '/') &&
            sPath16[1] &&
           (sPath16[2] == ':' || sPath16[2] == '|'))
        {
            sPath16.erase(0, 1);  // Remove leading '/' char
            sPath16[1] = ':';
        }

        for(eastl_size_t i = 0, iEnd = sPath16.length(); i < iEnd; ++i)
        {
            if(sPath16[i] == '/')
                sPath16[i] = '\\';
        }
    #endif

    const size_t requiredStrlen = EA::IO::StrlcpyUTF16ToUTF8(NULL, 0, sPath16.data(), sPath16.length());
    sPath.resize(requiredStrlen);
    EA::IO::StrlcpyUTF16ToUTF8(&sPath[0], sPath.capacity(), sPath16.data(), sPath16.length());
}


int ResourceHandleManager::AddTHJob(ResourceHandle* pRH, EA::WebKit::TransportHandler* pTH, const KURL& kurl, const String& url, const char16_t* pScheme, bool bSynchronous)
{
    using namespace EA::WebKit;

    m_JobInfoList.push_back();
    JobInfo& jobInfo = m_JobInfoList.back();

    {  // Job setup
        jobInfo.mId					= ++m_JobIdNext;
        jobInfo.mProcessInfo.mJobId = jobInfo.mId;
		jobInfo.mpRH				= pRH;
        jobInfo.mJobState			= kJSInit;
        jobInfo.mpTH				= pTH;
        jobInfo.mbSynchronous		= bSynchronous;
		jobInfo.mTInfo.mbAsync		= !bSynchronous;

        GetFixedString(jobInfo.mTInfo.mURI)->assign(url.characters(), url.length());
        GetFixedString(jobInfo.mTInfo.mEffectiveURI)->assign(GetFixedString(jobInfo.mTInfo.mURI)->c_str());
        EA::IO::EAIOStrlcpy16(jobInfo.mTInfo.mScheme, pScheme, sizeof(jobInfo.mTInfo.mScheme) / sizeof(jobInfo.mTInfo.mScheme[0]));
        jobInfo.mTInfo.mPort = kurl.port();

        if(EA::Internal::Stricmp(jobInfo.mTInfo.mScheme, L"file") == 0)
            GetPathFromFileURL(kurl, *GetFixedString(jobInfo.mTInfo.mPath));
        
        const WebCore::HTTPHeaderMap& customHeaders = pRH->request().httpHeaderFields(); // typedef HashMap<String, String, CaseFoldingHash> HTTPHeaderMap;
        WebCore::HTTPHeaderMap::const_iterator end = customHeaders.end();

        // We translate WebCore::HTTPHeaderMap to EA::WebKit::HeaderMap.
        for(WebCore::HTTPHeaderMap::const_iterator it = customHeaders.begin(); it != end; ++it)
        {
            const WebCore::HTTPHeaderMap::ValueType& wcValue = *it;
			EA::WebKit::HeaderMap::value_type eaValue(HeaderMap::key_type(wcValue.first.characters(), wcValue.first.length()), HeaderMap::mapped_type(wcValue.second.characters(), wcValue.second.length()));

			GetHeaderMap(jobInfo.mTInfo.mHeaderMapOut)->insert(eaValue);
        }

        // If we already have existing Basic or Digest username/password for this domain, we can try to provide it here.
        m_AuthenticationManager.ProvideCredentialsHeader(jobInfo.mTInfo);
        
        // Nothing to do. The transport handler fills this in.
        // jobInfo.mTInfo.mHeaderMapIn;

        jobInfo.mTInfo.mResultCode     = 0;  // We expect an HTTP-style code such as 200 or 404.

        const String& method = pRH->request().httpMethod();
        
		if(equalIgnoringCase(method,"GET"))
			jobInfo.mTInfo.mHttpRequestType = EA::WebKit::kHttpRequestTypeGET;
		else if(equalIgnoringCase(method,"HEAD"))
			jobInfo.mTInfo.mHttpRequestType = EA::WebKit::kHttpRequestTypeHEAD;
		else if(equalIgnoringCase(method,"POST"))
			jobInfo.mTInfo.mHttpRequestType = EA::WebKit::kHttpRequestTypePOST;
		else if(equalIgnoringCase(method,"PUT"))
			jobInfo.mTInfo.mHttpRequestType = EA::WebKit::kHttpRequestTypePUT;
		else if(equalIgnoringCase(method,"DELETE"))
			jobInfo.mTInfo.mHttpRequestType = EA::WebKit::kHttpRequestTypeDELETE;
		else if(equalIgnoringCase(method,"OPTIONS"))
			jobInfo.mTInfo.mHttpRequestType = EA::WebKit::kHttpRequestTypeOPTIONS;
		else
			EAW_ASSERT_MSG(false, "Unknown http method\n");


        // Absolute timeout time.
        double timeNow = EA::WebKit::GetTime();

        jobInfo.mTInfo.mTimeoutInterval = pRH->request().timeoutInterval();
        EAW_ASSERT(jobInfo.mTInfo.mTimeoutInterval > 0);
        if(jobInfo.mTInfo.mTimeoutInterval < m_timeoutSeconds)
           jobInfo.mTInfo.mTimeoutInterval = m_timeoutSeconds;

        jobInfo.mTInfo.mTimeout = timeNow + jobInfo.mTInfo.mTimeoutInterval;

        WebCore::ResourceRequest& rRequest = const_cast<WebCore::ResourceRequest&>(pRH->request());  // ResourceHandle allows only const access to the ResourceRequest, but we need to add a header.
        rRequest.setTimeoutInterval(jobInfo.mTInfo.mTimeoutInterval);

        // Set other values
        const EA::WebKit::Parameters& parameters = EA::WebKit::GetParameters();

        jobInfo.mTInfo.mbVerifyPeers            = parameters.mbVerifyPeers;
        jobInfo.mTInfo.mbEffectiveURISet        = false;
        jobInfo.mTInfo.mpTransportServerJobInfo = &jobInfo;
        jobInfo.mTInfo.mpRH                     = pRH;
        jobInfo.mTInfo.mpView                   = Local::GetView(pRH);
        jobInfo.mTInfo.mpTransportServer        = this;
        jobInfo.mTInfo.mpTransportHandler       = pTH;
        jobInfo.mTInfo.mTransportHandlerData    = 0;
        jobInfo.mTInfo.mpCookieManager          = &m_cookieManager;

		if(jobInfo.mTInfo.mHttpRequestType == EA::WebKit::kHttpRequestTypePOST)
            SetupTHPost(&jobInfo);
        else if(jobInfo.mTInfo.mHttpRequestType == EA::WebKit::kHttpRequestTypePUT)
            SetupTHPut(&jobInfo);

        //Nicki Vankoughnett:  This is where we decide if we want to load via the cache.
        //first, we decide if we need to invalidate the cache for this object, assuming said cache exists
        m_THFileCache.InvalidateCachedData(&jobInfo.mTInfo);

        //Now we Determine if we can redirect this job to the local file cache.  If the previous
        //function invalidated the cache, we have no problems.
		if(m_THFileCache.GetCachedDataValidity(GetFixedString(jobInfo.mTInfo.mURI)->c_str()))
        {    
            jobInfo.mpTH = &m_THFileCache;
            jobInfo.mProcessInfo.mProcessType = EA::WebKit::kVProcessTypeFileCacheJob;
        }    
        //Cookie Manager code
        m_cookieManager.OnHeadersSend(&jobInfo.mTInfo);

        #if EAWEBKIT_DUMP_TRANSPORT_FILES 
            
            // This will dump the loaded files to a dir for debug.
            // 3/4//10 CSidhall - Changed the file name building to include a sub dir and when possible to
            // use the base file name. 
            if(m_DebugWriteFileImages)
            {
                FixedString16_256 sFilePath(m_DebugFileImageDirectory);
                FixedString16_256 sDirName(jobInfo.mTInfo.mpView->GetURI());    // Could look into using base page insead. 
                FixedString16_256 sURIFileName(GetFixedString(jobInfo.mTInfo.mURI)->c_str());

                // --- Isolate the file name ---
                int position =0;
                for(eastl_size_t i = 0, iEnd = sURIFileName.size(); i < iEnd; ++i)
                {
                    if((sURIFileName[i] == '/') || (sURIFileName[i] == '\\'))
                        position = i + 1;
                }
                if(position)
                    sURIFileName.erase(0, position);


              if(!sURIFileName.size())
                {
                    // No file name found so might be a index.html                    
                    sURIFileName = GetFixedString(jobInfo.mTInfo.mURI)->c_str();
                    if((sURIFileName.find(L"http://") == 0) || (sURIFileName.find(L"file://") == 0))
                        sURIFileName.erase(0, 7);
                    
                    static const wchar_t* kDefaultExt =L".html";
                    sURIFileName.append_sprintf(L"%s", kDefaultExt);
                    sDirName.clear();   // The URI dir might no longer be valid so don't use it 
                }

                // Cleanup file name
                for(eastl_size_t i = 0, iEnd = sURIFileName.size(); i < iEnd; ++i)
                {
                    if((sURIFileName[i] == '/') || (sURIFileName[i] == '\\') ||(sURIFileName[i] == ':') ||(sURIFileName[i] == '.'))
                        sURIFileName[i] = '_';

                    if((sURIFileName[i] == ';') || (sURIFileName[i] == '#') || (sURIFileName[i] == '?'))
                    {
                        sURIFileName.resize(i);
                        break;
                    }
                }

                
                 //--- Build the directory to where to put the files ---
                 if((sDirName.find(L"http://") == 0) || (sDirName.find(L"file://") == 0))
                    sDirName.erase(0, 7);
                
                 if((sDirName.find(L"www.") == 0) && (sDirName[3] == '.'))
                    sDirName.erase(0, 4);      

              //  Cleanup
                for(eastl_size_t i = 0, iEnd = sDirName.size(); i < iEnd; ++i)
                {
                    if((sDirName[i] == ':') || (sDirName[i] == '.') )
                        sDirName[i] = '_';

                    if((sDirName[i] == ';') || (sDirName[i] == '#') || (sDirName[i] == '?'))
                    {
                        sDirName.resize(i);
                        break;
                    }
                }

                position = 0;
                for(eastl_size_t i = 0, iEnd = sDirName.size(); i < iEnd; ++i)
                {
                    if((sDirName[i] == '/') || (sDirName[i] == '\\'))
                    {
                        if((i+1) >= iEnd)
                        {
                            sDirName.resize(i); // End of string so remove now
                            break;    
                        }
                        else
                            position = i + 1;
                    }
                } 
                if(position) 
                    sDirName.erase(0, position);


                // Add and create dir to path 
                if(sDirName.size())
                {
                    sFilePath += sDirName;                
                    EA::IO::Directory::Create(sFilePath.c_str());
#if defined(EA_PLATFORM_WINDOWS) || defined(EA_PLATFORM_XENON)
					sFilePath += '\\';
#elif defined(EA_PLATFORM_WINCE)
					sFilePath += '\\';
#elif defined(EA_PLATFORM_PS3)
					sFilePath += '/';
#endif
                }

  
                if(sURIFileName.size())
                {
                    sFilePath += sURIFileName;

                    jobInfo.mFileImage.SetPath(sFilePath.c_str());
                    bool bResult = jobInfo.mFileImage.Open(EA::IO::kAccessFlagRead | EA::IO::kAccessFlagWrite, EA::IO::kCDCreateAlways);
                    (void)bResult;
                    // We can get asserts here if we have for example 2 identical files open so disabled this assert for now.
                    //  EAW_ASSERT(bResult);
                }
            }
        #endif
    }

    
    jobInfo.mProcessInfo.mURI = &jobInfo.mTInfo.mURI;
	

	NOTIFY_PROCESS_STATUS(jobInfo.mProcessInfo, EA::WebKit::kVProcessStatusQueuedToInit);
	return jobInfo.mId;
}


ResourceHandleManager::JobInfo* ResourceHandleManager::GetJob(int jobId)
{
    for(JobInfoList::iterator it = m_JobInfoList.begin(); it != m_JobInfoList.end(); ++it)
    {
        JobInfo& jobInfo = *it;

        if(jobInfo.mId == jobId)
            return &jobInfo;
    }

    return NULL;
}


void ResourceHandleManager::CondemnTHJob(JobInfo* pJobInfo)
{
    // We don't actually remove it here, we put it into a condemned state, to be removed later.
    pJobInfo->mJobState = kJSRemove;
    m_CondemnedJobsExist = true;

	NOTIFY_PROCESS_STATUS(pJobInfo->mProcessInfo, EA::WebKit::kVProcessStatusEnded);
    // We shouldn't need the following, as there should already be one active.
    // if(!m_downloadTimer.isActive())
    //     m_downloadTimer.startOneShot(m_pollTimeSeconds);
}


void ResourceHandleManager::CondemnAllTHJobs()
{
    // We don't actually remove it here, we put it into a condemned state, to be removed later.
    for(JobInfoList::iterator it = m_JobInfoList.begin(); it != m_JobInfoList.end(); ++it)
    {
        JobInfo& jobInfo = *it;
        CondemnTHJob(&jobInfo);
    }

    // We shouldn't need the following, as there should already be one active.
    // if(!m_downloadTimer.isActive())
    //     m_downloadTimer.startOneShot(m_pollTimeSeconds);
}


// Used for the HTTP "PUT" method, typically used to copy a file from client to server.
void ResourceHandleManager::SetupTHPut(JobInfo* /*pJobInfo*/)
{
    // For the "file" scheme, we just let the transport handler write the file to disk.
    // For the "http" scheme, we may want to set up some headers (such as content-disposition) that tell the server more about what to do.

    //if(Stricmp(pJobInfo->mScheme, "http") == 0)
    //{
    //    // To do: Do something to support HTTP put.
    //    Write TransportInfo::mPostSize with the file size.
    //}
}


// Used for the HTTP "POST" method, typically used to submit HTML form data to the server.
// We don't read the data here; it is instead read in the ReadData callback function.
void ResourceHandleManager::SetupTHPost(JobInfo* pJobInfo)
{
    FormData* const pFormData = pJobInfo->mpRH->request().httpBody();

    if(pFormData && !pFormData->elements().isEmpty())
    {
        const Vector<FormDataElement>& elements     = pFormData->elements();
        const size_t                   elementCount = elements.size();

        pJobInfo->mTInfo.mPostSize = 0;

        for(size_t i = 0; i < elementCount; i++)
        {
            const FormDataElement& element = elements[i];

            if(element.m_type == FormDataElement::encodedFile) // Types are either 'data' or 'encodedFile'
            {
                int64_t fileSizeResult;

                if(getFileSize(element.m_filename, fileSizeResult))
                {
                    EAW_ASSERT(fileSizeResult >= 0);
                    pJobInfo->mTInfo.mPostSize += fileSizeResult;
                }
                // We don't have an option for the case of being unable to read the size (i.e. unable to read the disk file).
            } 
            else // FormDataElement::data
                pJobInfo->mTInfo.mPostSize += (int64_t)elements[i].m_data.size();
        }
    }
    else
        pJobInfo->mTInfo.mPostSize = -1;
}


// This is a loop which runs each existing job as a simple state machine.
int ResourceHandleManager::ProcessTHJobs()
{
	SET_AUTOFPUPRECISION(EA::WebKit::kFPUPrecisionExtended);   
	// 11/09/09 CSidhall - Added notify start of process to user
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeTHJobs, EA::WebKit::kVProcessStatusStarted);
    
    for(JobInfoList::iterator it = m_JobInfoList.begin(); it != m_JobInfoList.end(); ++it)
    {
        JobInfo& jobInfo = *it;
        SET_AUTO_ACTIVE_VIEW(jobInfo.mTInfo.mpView);   // For mutliple view support

        //If a job is synchronous, make sure that its AsyncJobPaused flag is set to false. 
		//If the job is asynchronous, dont bother. Just pass the true to the assert macro.
		EAW_ASSERT((jobInfo.mbSynchronous ? !jobInfo.mbAsyncJobPaused : true));
		
		if(!jobInfo.mbAsyncJobPaused) //This is always going to be true for synchronous jobs. For async jobs, it is a valid app-controlled flag.
		{
			if(jobInfo.mJobState != kJSRemove)
			{
				bool bStateComplete = false;
				bool bRemoveJob     = false;

            switch (jobInfo.mJobState)
            {
                case kJSInit:
                    if(jobInfo.mpTH->InitJob(&jobInfo.mTInfo, bStateComplete))
                    {
                        jobInfo.mbTHInitialized = true;

                        if(bStateComplete)
						{
							jobInfo.mJobState = kJSConnect;
							NOTIFY_PROCESS_STATUS(jobInfo.mProcessInfo, EA::WebKit::kVProcessStatusQueuedToConnection);
						}
						
					}
                    else
                    {
                        bRemoveJob = true;
                        jobInfo.mbSuccess = false;
						NOTIFY_PROCESS_STATUS(jobInfo.mProcessInfo, EA::WebKit::kVProcessStatusQueuedToRemove);
                    }
                    break;

                case kJSConnect:
                    if(jobInfo.mpTH->Connect(&jobInfo.mTInfo, bStateComplete))
                    {
                        if(bStateComplete)
                        {
                            jobInfo.mJobState = kJSTransfer;
							NOTIFY_PROCESS_STATUS(jobInfo.mProcessInfo, EA::WebKit::kVProcessStatusQueuedToTransfer);
						}    
                    }
                    else
                    {
                        bRemoveJob = true;
                        jobInfo.mbSuccess = false;
						NOTIFY_PROCESS_STATUS(jobInfo.mProcessInfo, EA::WebKit::kVProcessStatusQueuedToRemove);
                    }
                    break;

                case kJSTransfer:
                    if(jobInfo.mpTH->Transfer(&jobInfo.mTInfo, bStateComplete))
                    {
                        if(bStateComplete)
                        {
                            jobInfo.mJobState = kJSDisconnect;
							NOTIFY_PROCESS_STATUS(jobInfo.mProcessInfo, EA::WebKit::kVProcessStatusQueuedToDisconnect);
                        }
                    }
                    else
                    {
                        bRemoveJob = true;
                        jobInfo.mbSuccess = false;
						NOTIFY_PROCESS_STATUS(jobInfo.mProcessInfo, EA::WebKit::kVProcessStatusQueuedToRemove);
                    }
                    break;

                case kJSDisconnect:
                    if(jobInfo.mpTH->Disconnect(&jobInfo.mTInfo, bStateComplete))
                    {
                        if(bStateComplete)
                        {
                            jobInfo.mJobState = kJSShutdown;
							NOTIFY_PROCESS_STATUS(jobInfo.mProcessInfo, EA::WebKit::kVProcessStatusQueuedToShutdown);
                        }
                    }
                    else
					{
						bRemoveJob = true;
						NOTIFY_PROCESS_STATUS(jobInfo.mProcessInfo, EA::WebKit::kVProcessStatusQueuedToRemove);
					}

                    break;

                case kJSShutdown:
                    if(jobInfo.mpTH->ShutdownJob(&jobInfo.mTInfo, bStateComplete))
                    {
                        jobInfo.mbTHShutdown = true;

                        if(bStateComplete)
						{
							bRemoveJob = true;
							NOTIFY_PROCESS_STATUS(jobInfo.mProcessInfo, EA::WebKit::kVProcessStatusQueuedToRemove);
						}

                    }
                    else
					{
						bRemoveJob = true;
						NOTIFY_PROCESS_STATUS(jobInfo.mProcessInfo, EA::WebKit::kVProcessStatusQueuedToRemove);
					}
                    break;

                case kJSRemove:
                    bRemoveJob = true;
                    break;
            }

            if(!bRemoveJob)
            {
                // Check to see if somehow the job was cancelled at the ResourceHandle level.
                ResourceHandleInternal* pRHI = jobInfo.mpRH->getInternal();

                if(pRHI->m_cancelled)
                    bRemoveJob = true;
            }

				if(bRemoveJob)
				{
					CondemnTHJob(&jobInfo);
				}
			}
		}
    }

    if(m_CondemnedJobsExist)
    {
        for(JobInfoList::iterator it = m_JobInfoList.begin(); it != m_JobInfoList.end(); )
        {
            JobInfo& jobInfo = *it;

            //If a job is synchronous, make sure that its AsyncJobPaused flag is set to false. 
			//If the job is asynchronous, dont bother. Just pass the true to the assert macro.
			EAW_ASSERT((jobInfo.mbSynchronous ? !jobInfo.mbAsyncJobPaused : true));
		
			if(!jobInfo.mbAsyncJobPaused) //This is always going to be true for synchronous jobs. For async jobs, it is a valid app-controlled flag.
			{	
				if(jobInfo.mJobState == kJSRemove)
				{
					if(!jobInfo.mbSuccess)
					{
						ResourceHandleInternal* pRHI = jobInfo.mpRH->getInternal();
						ResourceHandleClient*   pRHC = pRHI->client();

                    if(pRHC)
                    {
                        // To consider: Make a means for the transport handler to specify its own error code type and localized string.
						const WebCore::String sURI(GetFixedString(jobInfo.mTInfo.mURI)->data(), GetFixedString(jobInfo.mTInfo.mURI)->length());
                        const WebCore::String sError; // To consider: Assign a meaningful string to this, such as "Transport error."
						
                        ResourceError error(sURI, jobInfo.mTInfo.mResultCode, sURI, sError);
                        pRHC->didFail(jobInfo.mpRH, error);
                    }
                }
                // To consider: Call pRHC->didFinishLoading here instead of the DataDone function.

                // We promise the TransportHandler that if we Init it then eventually we will Shutdown it. This allows it to clean up.
                if(jobInfo.mbTHInitialized && !jobInfo.mbTHShutdown)
                {
                    bool bStateComplete;
                    jobInfo.mpTH->ShutdownJob(&jobInfo.mTInfo, bStateComplete);
                }

                // If this job came back requiring authorization, we leave the resource handle
                // alive, put the job in limbo, and wait for the user to supply name/password.
                if(jobInfo.mbAuthorizationRequired)
                {
                    // The authorization manager will temporarily take over the job and 
                    // initiate user interaction to get a username/password.
                    // Note that we expect m_AuthenticationManager to use jobInfo.mpRH->ref().
                    m_AuthenticationManager.BeginAuthorization(jobInfo.mTInfo);
                }

                jobInfo.mpRH->deref();  // This matches the ref() done by the entity that handed us the ResourceHandle originally.
                jobInfo.mpRH = NULL;
                m_runningJobs--;

					it = m_JobInfoList.erase(it);
					//Note by Arpit Baldeva: I moved this flag to be here instead of at the top. This is so that if a condemned job exists but is paused, I 
					//want to make sure we are not clearning this flag without even "condeming" the job for real.
					//Also, I am not sure at this point if condemning a job while it is paused is a good idea.
					m_CondemnedJobsExist = false; 
				}
				else
					++it; //The job is not in removed state. Just increment the iterator.
			}
			else
				++it; //The job is paused. Just increment the iterator.
		}
    }

	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeTHJobs, EA::WebKit::kVProcessStatusEnded);
    return (int)m_JobInfoList.size();
}


bool ResourceHandleManager::SetEffectiveURI(EA::WebKit::TransportInfo* pTInfo, const char* pURI)
{
    ResourceHandle*         pRH  = static_cast<ResourceHandle*>(pTInfo->mpRH);
    ResourceHandleInternal* pRHI = pRH->getInternal();

    pTInfo->mbEffectiveURISet = true;

    if(!pRHI->m_cancelled)
    {
        const KURL    url(pURI);
        const String& s(url.string());

        pRHI->m_response.setUrl(url);
		GetFixedString(pTInfo->mEffectiveURI)->assign(s.characters(), s.length());

        return true;
    }

    return false;
}


bool ResourceHandleManager::SetRedirect(EA::WebKit::TransportInfo* pTInfo, const char* pURI)
{
    WebCore::ResourceHandle* pRH  = (WebCore::ResourceHandle*)pTInfo->mpRH;
    ResourceHandleInternal*  pRHI = pRH->getInternal();
    ResourceHandleClient*    pRHC = pRHI->client();
    WebCore::String          sURI(pURI);
    KURL                     newURL(pRH->request().url(), sURI);

    // Note by Paul Pedriana: I don't know why WebKit has this willSendRequest function and how necessary it is to use it.
    if(pRHC)
    {
        ResourceRequest redirectedRequest = pRH->request();
        redirectedRequest.setURL(newURL);
        pRHC->willSendRequest(pRH, redirectedRequest, pRHI->m_response);
    }

	EA::WebKit::FixedString8_128 url8; 
	Local::ConvertWebCoreStr16ToEAString8(newURL.string(),url8);
	SetEffectiveURI(pTInfo,url8.c_str());
    pRHI->m_request.setURL(newURL); // Do we need or want to do this?

    return true;
}


bool ResourceHandleManager::SetExpectedLength(EA::WebKit::TransportInfo* pTInfo, int64_t size)
{
    ResourceHandle*         pRH  = static_cast<ResourceHandle*>(pTInfo->mpRH);
    ResourceHandleInternal* pRHI = pRH->getInternal();

    if(!pRHI->m_cancelled)
    {
        pRHI->m_response.setExpectedContentLength(size);

        return true;
    }

    return false;
}


bool ResourceHandleManager::SetEncoding(EA::WebKit::TransportInfo* pTInfo, const char* pEncoding)
{
    // This typically doesn't need to be called by the HTTP scheme, as the
    // HTTP scheme transport handler will instead call HeadersReceived and 
    // thus supply equivalent information.

    ResourceHandle*         pRH  = static_cast<ResourceHandle*>(pTInfo->mpRH);
    ResourceHandleInternal* pRHI = pRH->getInternal();

    if(!pRHI->m_cancelled)
    {
        const String sCharSet = pEncoding;
        pRHI->m_response.setTextEncodingName(sCharSet);

        return true;
    }

    return false;
}


bool ResourceHandleManager::SetMimeType(EA::WebKit::TransportInfo* pTInfo, const char* pMimeType)
{
    // This typically doesn't need to be called by the HTTP scheme, as the
    // HTTP scheme transport handler will instead call HeadersReceived and 
    // thus supply equivalent information.

    ResourceHandle*         pRH  = static_cast<ResourceHandle*>(pTInfo->mpRH);
    ResourceHandleInternal* pRHI = pRH->getInternal();

    if(!pRHI->m_cancelled)
    {
        const String sMimeType = pMimeType;
        pRHI->m_response.setMimeType(sMimeType);

        return true;
    }

    return false;
}


bool ResourceHandleManager::HeadersReceived(EA::WebKit::TransportInfo* pTInfo)
{
	SET_AUTOFPUPRECISION(EA::WebKit::kFPUPrecisionExtended);   
	// This typically needs to be called only by the HTTP scheme.
    // The headers are expected to be in TransportInfo::mHeaderMapIn.
    ResourceHandle*         pRH  = static_cast<ResourceHandle*>(pTInfo->mpRH);
    ResourceHandleInternal* pRHI = pRH->getInternal();
    ResourceHandleClient*   pRHC = pRHI->client();

    if(!pRHI->m_cancelled)
    {
        // Copy mHeaderMapIn to m_response.
		for(EA::WebKit::HeaderMap::const_iterator it = GetHeaderMap(pTInfo->mHeaderMapIn)->begin(); it != GetHeaderMap(pTInfo->mHeaderMapIn)->end(); ++it)
        {
			const EA::WebKit::HeaderMap::key_type& sKey   = it->first;
			const EA::WebKit::HeaderMap::mapped_type& sValue = it->second;

            // Translate EA::WebKit::FixedString16_256 to WebCore::String (both are 16 bit string types).
            const WebCore::String sWKey  (WebCore::String(sKey.data(),   sKey.length()));
            const WebCore::String sWValue(WebCore::String(sValue.data(), sValue.length()));

            pRHI->m_response.setHTTPHeaderField(sWKey, sWValue);            
        }

        // content-type examples:
        //     Content-Type: text/plain
        //     content-type: text/html; charset=utf-8

        // We use the eastl find_as function to find a string in a hash table case-insensitively.
		EA::WebKit::HeaderMap::const_iterator it = GetHeaderMap(pTInfo->mHeaderMapIn)->find_as(L"content-type", EA::WebKit::str_iless());

		if(it != GetHeaderMap(pTInfo->mHeaderMapIn)->end())
        {
            const EA::WebKit::HeaderMap::mapped_type& sValue = it->second;

            const String sMimeType = extractMIMETypeFromMediaType(sValue.c_str());
            pRHI->m_response.setMimeType(sMimeType);

            const String sCharSet = extractCharsetFromMediaType(sValue.c_str());
            pRHI->m_response.setTextEncodingName(sCharSet);
        }

        // content-disposition examples:
        //     Content-Disposition: filename=checkimage.jpg
        //     content-disposition: attachment; filename=checkimage.jpg
        //     Content-Disposition: attachment; filename=genome.jpeg; modification-date="Wed, 12 Feb 1997 16:29:51 -0500;

		it = GetHeaderMap(pTInfo->mHeaderMapIn)->find_as(L"content-disposition", EA::WebKit::str_iless());

		if(it != GetHeaderMap(pTInfo->mHeaderMapIn)->end())
        {
            const EA::WebKit::HeaderMap::mapped_type& sValue = it->second;

            const String sFileName = filenameFromHTTPContentDisposition(sValue.c_str());
            pRHI->m_response.setSuggestedFilename(sFileName);
        }

        if((pTInfo->mResultCode == 401) || (pTInfo->mResultCode == 407)) // Authorization required or proxy authorization required.
        {
            // We should have a header like this:
            //     WWW-Authenticate: Basic realm="WallyWorld"

			it = GetHeaderMap(pTInfo->mHeaderMapIn)->find_as(L"WWW-Authenticate", EA::WebKit::str_iless());

			if(it != GetHeaderMap(pTInfo->mHeaderMapIn)->end()) // 
            {
                eastl_size_t pos, posEnd;

                EA::WebKit::HeaderMap::mapped_type sValue = it->second;
                JobInfo* pJobInfo = (JobInfo*)pTInfo->mpTransportServerJobInfo;

                pos = sValue.find_first_not_of(L" \t\r\n");

                if(pos < sValue.size())
                {
                    posEnd = sValue.find(' ', pos + 1);

                    if(posEnd < sValue.size())
                        sValue[posEnd] = 0;


                    EA::Internal::Strcpy(pTInfo->mAuthorizationType, &sValue[pos]);

                    // We currently unilaterally fail authorization unless it is of
                    // Basic type. Later when we implement support for Digest and NTLM
                    // authorization we will either want to handle those here or let
                    // the authorization manager handle them. It would be better here
                    // if we used the EAWebKit ErrorInfo system to let the application
                    // display a dialog box instead of failing like this. But the end 
                    // result is the same either way: a 401 or 407 page error.
                    const bool bAuthorizationSupported = (EA::Internal::Stricmp(pTInfo->mAuthorizationType, L"Basic") == 0);
                    EAW_ASSERT(bAuthorizationSupported);

                    if(bAuthorizationSupported)
                    {
                        pJobInfo->mbAuthorizationRequired = true;

                        pos = sValue.find('\"', pos + 1);
                        if(pos < sValue.size())
                        {
                            eastl_size_t posEnd = sValue.find('\"', pos + 1);
                            if(posEnd < sValue.size())
								GetFixedString(pTInfo->mAuthorizationRealm)->assign(&sValue[pos + 1], &sValue[posEnd]);
                        }
                    }
					else
					{
						// To consider: Make a means for the transport handler to specify its own error code type and localized string.
						const WebCore::String sURI(GetFixedString(pJobInfo->mTInfo.mURI)->data(), GetFixedString(pJobInfo->mTInfo.mURI)->length());                        

                        ResourceError error(sURI, WebURLErrorUserAuthenticationUnsupported, sURI, GetWebkitErrorString(WebURLErrorUserAuthenticationUnsupported));
                        pRHC->didFail(pJobInfo->mpRH, error);
					}
                }
            }
        }
        else
            pRHI->m_response.setHTTPStatusCode(pTInfo->mResultCode);

		//Note by Arpit Baldeva:
		//We set the response URL as soon as we get the headers. This is so that we can inform the application deep down through the didReceiveResponse.
		//In following, if status code indicates a redirect, we set the response URL to be the original to indicate the URL for which we received response. 
		//Note that this is not the redirected URL.
		//If status code is regular, we use effective URI which should be correct in case of both normal and redirect flow.
		//It may sound opposite initially but it is not.
		if(pTInfo->mResultCode >=300 && pTInfo->mResultCode <= 399)
		{
			const KURL url(GetFixedString(pTInfo->mURI)->c_str());
			pRHI->m_response.setUrl(url);
		}
		else
		{
			const KURL url(GetFixedString(pTInfo->mEffectiveURI)->c_str());
			pRHI->m_response.setUrl(url);
		}
		
		if(pRHC)
            pRHC->didReceiveResponse(pRH, pRHI->m_response);
		
		pRHI->m_response.setResponseFired(true);

        return true;
    }

    return false;
}


bool ResourceHandleManager::DataReceived(EA::WebKit::TransportInfo* pTInfo, const void* pData, int64_t size)
{
	SET_AUTOFPUPRECISION(EA::WebKit::kFPUPrecisionExtended);   
	ResourceHandle*         pRH  = static_cast<ResourceHandle*>(pTInfo->mpRH);
    ResourceHandleInternal* pRHI = pRH->getInternal();
    ResourceHandleClient*   pRHC = pRHI->client();      // In theory this could be NULL, so we check for that below.

    m_readVolume += size;

    JobInfo* pJobInfo = (JobInfo*)pTInfo->mpTransportServerJobInfo;       
    if(pJobInfo)
        pJobInfo->mProcessInfo.mSize +=size;

    #if defined(AUTHOR_PPEDRIANA) && defined(EA_DEBUG)
        // Search for some incoming text. Problem: this won't catch text that straddles two receive  buffers.
        char* pString = (char*)(const char*)pData;
        eastl::fixed_substring<char> s(pString, pString + (eastl_size_t)size);
        if(s.find("some search phrase") < s.size())
            pString++; // Can put a breakpoint here. 
    #endif

    if(!pRHI->m_cancelled)
    {
        // Since HeadersReceived will not have called for "file" URIs,
        // the code to set the URI and fire didReceiveResponse probably hasn't been run,
        // which means the ResourceLoader's response does not contain the URI.
        // We run the code here for local files to resolve the issue.
        // Note by Paul Pedriana: This code here is similar to the Curl version of this code.
        JobInfo* pJobInfo = (JobInfo*)pTInfo->mpTransportServerJobInfo;
        if(!pRHI->m_response.responseFired())
        {
            // Note by Arpit Baldeva - Modified following to use mEffectiveURI instead of mURI. In fact, this is what is used in the DataDone() below.
			const KURL url(GetFixedString(pTInfo->mEffectiveURI)->c_str());
            pRHI->m_response.setUrl(url);
       
            if(pRHC)
               pRHC->didReceiveResponse(pRH, pRHI->m_response);

            pRHI->m_response.setResponseFired(true);
        }

        // We don't want to tell the client that we received any data if we are requiring 
        // authorization (user name/password). We want to wait until we have the final 
        // authorized page before we do that.
        if(!pJobInfo->mbAuthorizationRequired)
        {
            if(pRHC)
                pRHC->didReceiveData(pRH, (char*)pData, (size_t)size, 0);
        }
		else //Save data for later use so if the user cancels authorization, the message can be displayed.
		{
			m_AuthenticationManager.SaveCancelMessageData(pRH,(char*)pData, (size_t)size );
		}

#if EAWEBKIT_DUMP_TRANSPORT_FILES
		if(pJobInfo->mFileImage.GetAccessFlags()) // If it's open... echo the data to it.
			pJobInfo->mFileImage.Write(pData, (EA::IO::size_type)size);
#endif


        return true;
    }

    return false;
}


int64_t ResourceHandleManager::ReadData(EA::WebKit::TransportInfo* pTInfo, void* pData, int64_t size)
{
	SET_AUTOFPUPRECISION(EA::WebKit::kFPUPrecisionExtended);   
	// Currently we always assume that the 
    ResourceHandle*         pRH      = static_cast<ResourceHandle*>(pTInfo->mpRH);
    ResourceHandleInternal* pRHI     = pRH->getInternal();

    size_t readSize = 0;

    if(pTInfo->mHttpRequestType == EA::WebKit::kHttpRequestTypePOST)
    {
        if(size && !pRHI->m_cancelled)
        {
            readSize = pRHI->m_formDataStream.read(pData, (size_t)size, 1);

            if(readSize <= (size_t)size)  // If it wasn't a negative value and thus some kind of error...
                m_writeVolume += readSize;
            else
                cancel(pRH); // Something went wrong so cancel the pRH.
        }
    }
    else if(pTInfo->mHttpRequestType == EA::WebKit::kHttpRequestTypePUT)
    {
        // We don't currently handle this.
    }

    return readSize;
}


bool ResourceHandleManager::DataDone(EA::WebKit::TransportInfo* pTInfo, bool result)
{
	SET_AUTOFPUPRECISION(EA::WebKit::kFPUPrecisionExtended);   
	ResourceHandle*         pRH  = static_cast<ResourceHandle*>(pTInfo->mpRH);
    ResourceHandleInternal* pRHI = pRH->getInternal();
    ResourceHandleClient*   pRHC = pRHI->client();      // In theory this could be NULL, so we check for that below.


    if(!pRHI->m_cancelled)
    {
        JobInfo* pJobInfo = (JobInfo*)pTInfo->mpTransportServerJobInfo;

        // We don't want to tell the client that we received any data if we are requiring 
        // authorization (user name/password). We want to wait until we have the final 
        // authorized page before we do that.
        if(!pJobInfo->mbAuthorizationRequired)
        {
            pRHI->m_response.setHTTPStatusCode(pTInfo->mResultCode);

            // The following is the same as in our SetEffectiveURI function, so we could possibly
            // instead call that function instead of use this code. The main point is to call the
            // m_response setURL function.
			EAW_ASSERT(!GetFixedString(pTInfo->mEffectiveURI)->empty());
			WebCore::String sEffectiveURI(GetFixedString(pTInfo->mEffectiveURI)->data(), GetFixedString(pTInfo->mEffectiveURI)->length());
            KURL url(sEffectiveURI);
            pRHI->m_response.setUrl(url);

            if(pRHC)
            {
                if(result) // If succeeded... 
                {
                    if( pTInfo->mpTransportHandler->CanCacheToDisk() &&  m_THFileCache.CacheEnabled() )  //No sense in caching either stuff loaded via cache, or file:///
                    {
                        //Check with the response header directives if we can cache this file
                        EA::WebKit::CacheResponseHeaderInfo cacheHeaderInfo;
                        if(cacheHeaderInfo.SetDirectivesFromHeader(pTInfo))
                        {
                            EA::WebKit::FixedString8_128 mimeStr;
                            Local::ConvertWebCoreStr16ToEAString8(pRHI->m_response.mimeType(), mimeStr);

                            const SharedBuffer* requestData = pRHC->getRequestData();
                            //Note by Arpit Baldeva: Check against null pointer. Crashed on http://kotaku.com
                            if(requestData)
								m_THFileCache.CacheToDisk(*GetFixedString(pTInfo->mURI), mimeStr, *requestData, cacheHeaderInfo);
                        }
                    }
                    pRHC->didFinishLoading(pRH);

                    //Note by Arpit Baldeva: Send the information to the cookie manager. Do not rely on the DirtySDK handler header receive callback
					//because an application may override it.
					m_cookieManager.OnHeadersRead(pTInfo);
                }
                else
                {
                    pJobInfo->mbSuccess = false;
                    CondemnTHJob(pJobInfo);

                    // To consider: Call pRHC->didFail(pRH, ResourceError()) here instead of 
                    // ProcessTHJobs. Else move all of this DataDone functionality to ProcessTHJobs.
                }
            }
        }

        return true;
    }

    return false;
}


void ResourceHandleManager::AssertionFailure(EA::WebKit::TransportInfo* /*pTInfo*/, const char* pString)
{
    EAW_ASSERT_MSG(pString != NULL, pString);
}


void ResourceHandleManager::Trace(EA::WebKit::TransportInfo* /*pTInfo*/, int /*channel*/, const char* pString)
{
    // channel not currently supported, but maybe we want to in the future.
    EAW_TRACE_MSG(pString);
}


void ResourceHandleManager::AddTransportHandler(EA::WebKit::TransportHandler* pTH, const char16_t* pScheme)
{
    THInfo thInfo;
	thInfo.mpTH = pTH;
	EAW_ASSERT_MSG(EA::Internal::Strlen(pScheme)<WKAL::MAX_SCHEME_LENGTH,"The scheme name is too big"); 
	EA::Internal::Strncpy(thInfo.mpScheme,pScheme, WKAL::MAX_SCHEME_LENGTH);

    pTH->Init(thInfo.mpScheme);
    m_THInfoList.push_front(thInfo); // We want the list to be LIFO.
}


void ResourceHandleManager::RemoveTransportHandler(EA::WebKit::TransportHandler* pTH, const char16_t* pScheme)
{
	// Note by Arpit Baldeva: Now we have the capability of an external TransportHandler installed for EAWebKit. So we make sure
	// that we finish up all the jobs that depend on this transport handler. Otherwise, this could randomly crash.  
	RemoveDependentJobs(pTH, pScheme);
	
	for(THInfoList::iterator it = m_THInfoList.begin(); it != m_THInfoList.end(); ++it)
    {
        const THInfo& thInfo = *it;

        if((thInfo.mpTH == pTH) && (EA::Internal::Stricmp(thInfo.mpScheme, pScheme) == 0))
        {
            thInfo.mpTH->Shutdown(pScheme);
            m_THInfoList.erase(it);
            break;
        }
    }
}

void ResourceHandleManager::TickTransportHandlers()
{
	for(THInfoList::iterator it = m_THInfoList.begin(); it != m_THInfoList.end(); ++it)
	{
		const THInfo& thInfo = *it;
		thInfo.mpTH->Tick();
	}
}

void ResourceHandleManager::TickDownload()
{
	downloadTimerCallback(0);
}
//Note by Arpit Baldeva: When you are removing a transport handler, you need to remove all dependent jobs regardless of the scheme
//Otherwise, other schemes may end up using an already removed transport handler (say if you have both http and https schemes).
//TO be more accurate, the RemoveTransportHandler call does not even require the scheme but will keep it like that for backward compatibility.
//and because transport handler requires it.
void ResourceHandleManager::RemoveDependentJobs(EA::WebKit::TransportHandler* pTH, const char16_t* /*pScheme*/) 
{
	for(JobInfoList::iterator it = m_JobInfoList.begin(); it != m_JobInfoList.end(); ++it)
	{
		JobInfo& jobInfo = *it;
		if((jobInfo.mpTH == pTH) /*&& (EA::Internal::Stricmp(jobInfo.mTInfo.mScheme, pScheme) == 0)*/)
			CondemnTHJob(&jobInfo);
	}
	ProcessTHJobs();
}
void ResourceHandleManager::RemoveTransportHandlers()
{
    for(THInfoList::iterator it = m_THInfoList.begin(); it != m_THInfoList.end(); ++it)
    {
        const THInfo& thInfo = *it;
        thInfo.mpTH->Shutdown(thInfo.mpScheme);
    }

    m_THInfoList.clear();
}


EA::WebKit::TransportHandler* ResourceHandleManager::GetTransportHandler(const char16_t* pScheme)
{
	EAW_ASSERT(pScheme);

    EA::WebKit::TransportHandler* pTransportHandler = GetTransportHandlerInternal(pScheme);
	
	if(pTransportHandler)
		return pTransportHandler;

	//If the transport handler does not exist, install the default transport handler for the scheme and check again.
	AddDefaultTransportHandler(pScheme);
	pTransportHandler = GetTransportHandlerInternal(pScheme);

    return pTransportHandler;
}

EA::WebKit::TransportHandler* ResourceHandleManager::GetTransportHandlerInternal(const char16_t* pScheme)
{
	for(THInfoList::iterator it = m_THInfoList.begin(); it != m_THInfoList.end(); ++it)
	{
		const THInfo& thInfo = *it;

		if(EA::Internal::Stricmp(thInfo.mpScheme, pScheme) == 0)
			return thInfo.mpTH;
	}

	return NULL;
}

void ResourceHandleManager::AddDefaultTransportHandler(const char16_t* pScheme)
{
	const EA::WebKit::Parameters& parameters = EA::WebKit::GetParameters();

	if((EA::Internal::Stricmp(pScheme, L"file") == 0))
	{
		if(parameters.mbEnableFileTransport)
			AddTransportHandler(&m_THFile, L"file");
	}

#if USE(CURL)
	if(parameters.mbEnableCurlTransport)
	{
		if((EA::Internal::Stricmp(pScheme, L"http") == 0) || (EA::Internal::Stricmp(pScheme, L"ftp") == 0))
		{
			EAW_ASSERT_MSG(false, "No application supplied transport handler is found. Using the transport handler in the EAWebKit pacakge.");
			AddTransportHandler(&m_THCurl, pScheme);
		}
	}
#endif

#if USE(DIRTYSDK)
	if(parameters.mbEnableDirtySDKTransport)
	{
		if((EA::Internal::Stricmp(pScheme, L"http") == 0) || (EA::Internal::Stricmp(pScheme, L"https") == 0))
		{
			EAW_ASSERT_MSG(false, "No application supplied transport handler is found. Using the transport handler in the EAWebKit pacakge.");
			AddTransportHandler(&m_THDirtySDK, pScheme);
		}
	}
#endif
}

EA::WebKit::CookieManager* ResourceHandleManager::GetCookieManager()
{
    return &m_cookieManager;
}


EA::WebKit::AuthenticationManager* ResourceHandleManager::GetAuthenticationManager()
{
    return &m_AuthenticationManager;
}


// Nicki Vankoughnett:  These are accessors for the disk cache.  It is a bit 
// strange, but the disk cache object belongs to this class as a member.
// Directly exposing the transport handler seems to be an inelegant solution.
// Using a series of wrappers may not be ideal, but its better.
void ResourceHandleManager::ClearDiskCache()
{
    m_THFileCache.ClearCache();
}


bool ResourceHandleManager::UseFileCache(bool enabled) 
{
    return m_THFileCache.UseFileCache(enabled);
}


bool ResourceHandleManager::SetCacheDirectory(const char16_t* pCacheDirectory)
{
    return m_THFileCache.SetCacheDirectory(pCacheDirectory);
}


bool ResourceHandleManager::SetCacheDirectory(const char8_t* pCacheDirectory)
{
    return m_THFileCache.SetCacheDirectory(pCacheDirectory);
}


void ResourceHandleManager::GetCacheDirectory(EA::WebKit::FixedString8_128& cacheDirectory)
{
    return m_THFileCache.GetCacheDirectory(cacheDirectory);
}


void ResourceHandleManager::GetCacheDirectory(EA::WebKit::FixedString16_128& cacheDirectory)
{
    return m_THFileCache.GetCacheDirectory(cacheDirectory);
}


void ResourceHandleManager::SetMaxCacheSize(uint32_t nCacheSize)
{
    m_THFileCache.SetMaxCacheSize(nCacheSize);
}


uint32_t ResourceHandleManager::GetMaxCacheSize()
{
    return m_THFileCache.GetMaxCacheSize();
}


void ResourceHandleManager::SetMaxNumberOfCachedFiles(const uint32_t count)
{
    m_THFileCache.SetMaxFileCount(count);
}


uint32_t ResourceHandleManager::GetMaxNumberOfCachedFiles() const
{
    return m_THFileCache.GetMaxFileCount();
}


void ResourceHandleManager::SetMaxNumberOfOpenFiles(const uint32_t count)
{
    m_THFileCache.SetMaxJobCount(count);
}

void ResourceHandleManager::SetMinFileSizeToCache(const uint32_t size)
{
    m_THFileCache.SetMinFileSize(size);
}


} // namespace WebCore
