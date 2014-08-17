/*
Copyright (C) 2008-2009-2010 Electronic Arts, Inc.  All rights reserved.

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
// BCResourceHandleManagerEA.h
// Created by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////


#ifndef ResourceHandleManager_h
#define ResourceHandleManager_h

#include <wtf/FastAllocBase.h>

#include "Frame.h"
#include "Timer.h"
#include "ResourceHandleClient.h"
#include <wtf/Vector.h>
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebKitTransport.h>
#include <EAWebKit/internal/EAWebKitEASTLHelpers.h>
#include <EASTL/fixed_list.h>
#include "BCCookieManagerEA.h"
#include "BCTransportHandlerFileCache.h"
#include "BCAuthenticationManagerEA.h"

//Note by Arpit Baldeva:If following is defined as 1, it will dump the files received by the transport handler to a location at the disk. 
// You'll also need to call SetDebugFileDumpStatus(const bool enabled) at runtime.
#ifndef EAWEBKIT_DUMP_TRANSPORT_FILES
	#ifdef _DEBUG
		// Make sure not to have this active in a final release build.
		#define EAWEBKIT_DUMP_TRANSPORT_FILES 1
	#else
		#define EAWEBKIT_DUMP_TRANSPORT_FILES 0
	#endif
#endif

#if EAWEBKIT_DUMP_TRANSPORT_FILES
    #include <EAIO/EAFileStream.h>
	#include <EAIO/EAFileUtil.h>
#endif

#if USE(CURL)
    #include "BCTransportHandlerCurl.h"
#endif

#if USE(DIRTYSDK)
    #include "BCTransportHandlerDirtySDK.h"
#endif



#ifdef GetJob // If windows.h has #defined GetJob to GetJobA...
    #undef GetJob
#endif


namespace WKAL {

const unsigned int MAX_SCHEME_LENGTH = 32;
class ResourceHandleManager : public EA::WebKit::TransportServer, public WTF::FastAllocBase
{
public:
    static ResourceHandleManager* m_pInstance;
    static ResourceHandleManager* sharedInstance();
    static void                   staticFinalize();

    void add(ResourceHandle* pRH);
    void cancel(ResourceHandle* pRH);
    void setCookieFilePath(const char* cookieFilePath);
    void dispatchSynchronousJob(ResourceHandle* pRH);
	void PauseAsyncJobs();
	void ResumeAsyncJobs();

    // TransportHandler info. This is a mapping of TransportHandler to supported scheme.
    struct THInfo 
    {
        EA::WebKit::TransportHandler* mpTH;             // Plug-innable TransportHandler
        char16_t               mpScheme[MAX_SCHEME_LENGTH+1];         // e.g. "http" "file" "ftp"
    };

    enum JobState   // See the TransportHandler class.
    {
        kJSInit,        // Call TransportHandler::Init
        kJSConnect,     // Call TransportHandler::Connect
        kJSTransfer,    // Call TransportHandler::Transfer
        kJSDisconnect,  // Call TransportHandler::Disconnect
        kJSShutdown,    // Call TransportHandler::Shutdown
        kJSRemove       // Remove the Job
    };

    // To consider: We might be able to fold this information into TransportInfo and 
    // just use TransportInfo instead encapsulating TransportInfo into JobInfo here.
    struct JobInfo/*: public WTF::FastAllocBase*/
    {
        int                           mId;                      // This is simply an ever-increasing number.
        WebCore::ResourceHandle*      mpRH;                     // WebKit ResourceHandle. Ref-counted while stored in this struct.
        JobState                      mJobState;                // 
        EA::WebKit::TransportHandler* mpTH;                     // The TransportHandler for this job.
        EA::WebKit::TransportInfo     mTInfo;                   // TransportInfo
        bool                          mbTHInitialized;          // True if the TransportHandler was initialized.
        bool                          mbTHShutdown;             // True if the TransportHandler was shut down.
        bool                          mbSuccess;                // True if the job is OK, false if the job failed.
        bool                          mbSynchronous;            // True if the job is asynchronous, else synchronous.
        bool                          mbAuthorizationRequired;  // True if the job was completed with a 401 or 407 result (authorization required).
		bool						  mbAsyncJobPaused;			// True if the job is paused. You may need it in some situations.	
        EA::WebKit::ViewProcessInfo   mProcessInfo;             // Used for process user callback notifications 
        #if EAWEBKIT_DUMP_TRANSPORT_FILES
        EA::IO::FileStream            mFileImage;               // Used to write received data to disk in debug builds.
        #endif

        JobInfo();
    };

    bool    SetEffectiveURI         (EA::WebKit::TransportInfo* pTInfo, const char* pURI);
    bool    SetRedirect             (EA::WebKit::TransportInfo* pTInfo, const char* pURI);
    bool    SetExpectedLength       (EA::WebKit::TransportInfo* pTInfo, int64_t size);
    bool    SetEncoding             (EA::WebKit::TransportInfo* pTInfo, const char* pEncoding);
    bool    SetMimeType             (EA::WebKit::TransportInfo* pTInfo, const char* pMimeType);
    bool    HeadersReceived         (EA::WebKit::TransportInfo* pTInfo);
    bool    DataReceived            (EA::WebKit::TransportInfo* pTInfo, const void* pData, int64_t size);
    int64_t ReadData                (EA::WebKit::TransportInfo* pTInfo, void* pData, int64_t size);
    bool    DataDone                (EA::WebKit::TransportInfo* pTInfo, bool result);
    void    AssertionFailure        (EA::WebKit::TransportInfo* pTInfo, const char* pExpression);
    void    Trace                   (EA::WebKit::TransportInfo* pTInfo, int channel, const char* pString);

    void							AddTransportHandler    (EA::WebKit::TransportHandler* pTH, const char16_t* pScheme);
    void							RemoveTransportHandler (EA::WebKit::TransportHandler* pTH, const char16_t* pScheme);
    void							RemoveTransportHandlers();
	void							TickTransportHandlers();
	void							TickDownload();
	EA::WebKit::TransportHandler*	GetTransportHandler    (const char16_t* pScheme);

    EA::WebKit::CookieManager*         GetCookieManager();
    EA::WebKit::AuthenticationManager* GetAuthenticationManager();

    //Disk cache accessors
    void                            ClearDiskCache();
    bool                            UseFileCache(bool enabled);
    bool                            SetCacheDirectory(const char16_t* pCacheDirectory);
    bool                            SetCacheDirectory(const char8_t* pCacheDirectory);
    void                            GetCacheDirectory(EA::WebKit::FixedString8_128& cacheDirectory);
    void                            GetCacheDirectory(EA::WebKit::FixedString16_128& cacheDirectory);

    void                            SetMaxCacheSize(uint32_t nCacheSize);
    uint32_t                        GetMaxCacheSize();
    void                            SetMaxNumberOfCachedFiles(const uint32_t count);
    uint32_t                        GetMaxNumberOfCachedFiles() const; 
    void                            SetMaxNumberOfOpenFiles(const uint32_t count);
    void                            SetMinFileSizeToCache(const uint32_t size);

    int      AddTHJob(ResourceHandle* pRH, EA::WebKit::TransportHandler* pTH, const KURL& kurl, const String& url, const char16_t* pScheme, bool bSynchronous);
    JobInfo* GetJob(int jobId);
    void     CondemnTHJob(JobInfo* pJobInfo);
    void     CondemnAllTHJobs();
    void     SetupTHPut(JobInfo* pJobInfo);
    void     SetupTHPost(JobInfo* pJobInfo);
    int      ProcessTHJobs(); // To be called repeatedly while there are active jobs.


   #if EAWEBKIT_DUMP_TRANSPORT_FILES
    void setDebugWriteFileStatus(const bool enabled) {m_DebugWriteFileImages = enabled; }    
    #endif

protected:
    ResourceHandleManager();
    virtual ~ResourceHandleManager();

    void downloadTimerCallback(Timer<ResourceHandleManager>*);
    bool removeScheduledJob(ResourceHandle* pRH);
    void startJob(ResourceHandle* pRH);
    bool startScheduledJobs();
    int  initializeHandle(ResourceHandle* pRH, bool bSynchronous);
	EA::WebKit::TransportHandler*	GetTransportHandlerInternal    (const char16_t* pScheme);
	void							RemoveDependentJobs(EA::WebKit::TransportHandler* pTH, const char16_t* pScheme);
	//We first try to get the transport handler that the app may have provided to us. If none exists, we install the default ones and use them.
	void							AddDefaultTransportHandler(const char16_t* pScheme);	


protected:
    Timer<ResourceHandleManager>        m_downloadTimer;            // 
    EA::WebKit::FixedString8_256            m_cookieFilePath;           // File path to use to store cookies in a persistent way.
    Vector<ResourceHandle*>             m_resourceHandleList;       // This is a list of jobs that haven't been started yet.
    int                                 m_runningJobs;              // This is a count of jobs that have been started and are running.
    double                              m_pollTimeSeconds;          // Defaults to something small like 0.05 seconds.
    int                                 m_timeoutSeconds;           // Timeout for all loads.
    int                                 m_maxConcurrentJobs;        // Max number of jobs occurring at at time.
    EA::WebKit::CookieManager           m_cookieManager;            // 
    EA::WebKit::AuthenticationManager   m_AuthenticationManager;    // 

    typedef eastl::fixed_list<THInfo,  4, true, EA::WebKit::EASTLAllocator> THInfoList;    // List of available TransportHandlers.
    typedef eastl::fixed_list<JobInfo, 8, true, EA::WebKit::EASTLAllocator> JobInfoList;   // Currently running jobs.

    THInfoList                              m_THInfoList;
    JobInfoList                             m_JobInfoList;
    int                                     m_JobIdNext;
    bool                                    m_CondemnedJobsExist;
    EA::WebKit::TransportHandlerFile        m_THFile;               // Used for handling "file" scheme (i.e. local disk access).
    EA::WebKit::TransportHandlerFileCache   m_THFileCache;
    #if USE(CURL)
    EA::WebKit::TransportHandlerCurl		m_THCurl;
    #endif
    #if USE(DIRTYSDK)
    EA::EAWEBKIT_PACKAGE_NAMESPACE::TransportHandlerDirtySDK	m_THDirtySDK;
    #endif

    #if EAWEBKIT_DUMP_TRANSPORT_FILES
        bool                                m_DebugWriteFileImages;     // Used to enable writing received file contents to disk.
        EA::WebKit::FixedString16_256       m_DebugFileImageDirectory;  
    #endif

public:
    // Metrics, added by EA.
    uint64_t m_readVolume;      // Number of bytes ever read.
    uint64_t m_writeVolume;     // Number of bytes ever written.
};



} // namespace



#endif
