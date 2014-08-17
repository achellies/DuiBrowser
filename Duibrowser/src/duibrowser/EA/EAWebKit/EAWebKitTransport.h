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
// EAWebKitTransport.h
// Created by Paul Pedriana - 2008
/////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKIT_EAWEBKITTRANSPORT_H
#define EAWEBKIT_EAWEBKITTRANSPORT_H


#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebKitConfig.h>
#include <EAWebKit/EAWebKitFileSystem.h>
#include <EAWebKit/EAWebKitForwardDeclarations.h>



namespace EA
{
    namespace WebKit
    {
        // EAWebKit Transport
        //
        // A web browser needs to do HTTP connections to web servers to get HTML documents, and possibly
        // to "post" or "put" content. Additionally a browser can typically read disk file data, as with 
        // "file://" URIs. Often "ftp://" is supported as well. We may also want a web browser to support
        // custom schemes (e.g. "game://") whereby the application supplies mechanism for handling the 
        // data transport. This file defines some interfaces for implementing application-supplied transport.
        //
        // EAWebKit provides a TransportServer and the user provides one or more TransportHandlers.
        // The TransportServer calls the TransportHandler to do the transport. The TransportHandler may in
        // some cases need to call back the TransportServer, as in the case of newly received data.
        //
        // EAWebKit supplies automatically registered built-in support for http, https and file schemes, 
        // though you can override them.

        class TransportInfo;
        class TransportServer;
        class TransportHandler;
        class CookieManager;


        // Allows the application to register transport handlers.
        // A transport handler for the "file" scheme is automatically registered.
        // Schemes can include custom schemes defined by the application.
        EAWEBKIT_API void              AddTransportHandler(TransportHandler* pTH, const char16_t* pScheme);
        EAWEBKIT_API void              RemoveTransportHandler(TransportHandler* pTH, const char16_t* pScheme);
        EAWEBKIT_API TransportHandler* GetTransportHandler(const char16_t* pScheme);

		//HTTP request type enums
		enum HttpRequestType
		{
			kHttpRequestTypeHEAD = 0,
			kHttpRequestTypeGET,
			kHttpRequestTypePOST,
			kHttpRequestTypePUT,
			kHttpRequestTypeDELETE,
			kHttpRequestTypeOPTIONS,
			kHttpRequestTypeCount
		};

		// This is used to describe the high level resource request information.
        class TransportInfo
        {
        public:
            EASTLFixedString16Wrapper       mURI;                       // Full 16 bit URI as specified by the application or user. However, any present URI fragment (see URI spec. for definition of fragment) will be removed.
            EASTLFixedString16Wrapper       mEffectiveURI;              // The URI that was eventually used. This is usually the same as mURI, but a server redirect can change it to something else.
            char16_t						mScheme[16];                // "http", "file", "ftp", etc. 
            uint32_t						mPort;                      // Meaningful for HTTP. 
            EASTLFixedString8Wrapper        mPath;                      // File path to use for file URLs in UTF8 encoding. Equivalent to UTF8-decoded path from mURI.
            EASTLHeaderMapWrapper           mHeaderMapOut;              // Outgoing headers
            EASTLHeaderMapWrapper           mHeaderMapIn;               // Incoming headers
            //EASTLFixedString8Wrapper        mCookieFilePath;            // File path to use to store cookies in a persistent way.
            int32_t							mResultCode;                // Conventional HTTP result code.
            int64_t							mPostSize;                  // If mMethod is PUT or POST, this is the size of the body data (e.g. form data size in the case of POST). -1 if not used.
            HttpRequestType					mHttpRequestType;			// HEAD, GET etc. represented as an enum
            char16_t						mAuthorizationType[16];     // Typically one of "Basic" "Digest" or "NTLM".
            EASTLFixedString16Wrapper       mAuthorizationRealm;        // This is a string defined by the server.
            double							mTimeout;                   // Absolute (as opposed to relative) timeout time in seconds. See EA::WebKit::GetTime().
            double							mTimeoutInterval;           // Relative timeout for the initial request, in seconds.
            bool							mbVerifyPeers;              // True if the job should validate peers with secure connections.
            bool							mbEffectiveURISet;          // True if TransportServer::SetEffectiveURI has been called.
            bool							mbProcessed;                // Used by Authorization system only at this time.

            void*							mpTransportServerJobInfo;   // Private to TransportServer. Allows connecting a TransportInfo back to the server job that manages the TransportInfo.
            void*							mpRH;                       // WebCore::ResourceHandle, used internally.
            EA::WebKit::View*				mpView;                     // 
            TransportServer*				mpTransportServer;          // 
            TransportHandler*				mpTransportHandler;         // 
            uintptr_t						mTransportHandlerData;      // The TransportHandler can use this internally.
            CookieManager*					mpCookieManager;            // The CookieManager.
			bool							mbAsync;

        public:
            TransportInfo();
		private:
			// Note by Arpit Baldeva:TODO:Compiler is generating the copy and assignment operator for us because it seems we are making a copy of the TransportInfo in BCAuthenticationManager.cpp.
			// We should probably fix that code to simply contain pointers to the TransportInfo instances. Since I am in the middle of a big change here(hiding templates behind the interface),
			// I'll revisit it separately.

			//TransportInfo(const TransportInfo& rhs);
			//TransportInfo& operator = (const TransportInfo& rhs);

        };


        // Interface which the TransportHandler calls.
        class TransportServer
        {
        public:
            virtual ~TransportServer() { } 

            virtual bool    SetEffectiveURI       (TransportInfo* pTInfo, const char* pURI) = 0;                                // The actual URI associated with the page. May be different from requested URI if the request was redirected during HTTP processing.
            virtual bool    SetRedirect           (TransportInfo* pTInfo, const char* pURI) = 0;                                // The redirected URI in the case of an HTTP redirection.
            virtual bool    SetExpectedLength     (TransportInfo* pTInfo, int64_t size) = 0;                                    // The expected size of the resource data has been revealed. Negative values mean size is known but undermined ahead of time.
            virtual bool    SetEncoding           (TransportInfo* pTInfo, const char* pEncoding) = 0;                           // The encoding of the data is known.
            virtual bool    SetMimeType           (TransportInfo* pTInfo, const char* pMimeType) = 0;                           // The MIME type of the data has been explicitly specified (e.g. via an HTTP header).
            virtual bool    HeadersReceived       (TransportInfo* pTInfo) = 0;                                                  // The TransportHandler has received HTTP headers (which come before the data in an HTTP transfer). The headers are expected to be in TransportInfo::mHeaderMapIn.
            virtual bool    DataReceived          (TransportInfo* pTInfo, const void* pData, int64_t size) = 0;                 // The TransportHandler has received data (e.g. for GET operation).
            virtual int64_t ReadData              (TransportInfo* pTInfo, void* pData, int64_t size) = 0;                       // The TransportHandler needs more data to send (e.g. for PUT operation). Return value is the same as EAIO IStream::Read. Negative value means error, 0 means nothing more to read. Return value may be less than size.
            virtual bool    DataDone              (TransportInfo* pTInfo, bool result) = 0;                                     // The transport completed, with a success/failure result. TransportInfo::mResultCode contains more information.
            virtual void    AssertionFailure      (TransportInfo* pTInfo, const char* pString) = 0;                             // The TransportHandler has failed an assert.
            virtual void    Trace                 (TransportInfo* pTInfo, int channel, const char* pString) = 0;                // The TransportHandler would like to do a debug trace.
        };


        // Interface which the TransportServer calls.
        class TransportHandler
        {
        public:
            virtual ~TransportHandler()														{ } 

            virtual bool Init           (const char16_t* pScheme)							{ return true; }							// Called upon library init.
            virtual bool Shutdown       (const char16_t* pScheme)							{ return true; }							// Called upon library shutdown.
            virtual bool InitJob        (TransportInfo* pTInfo, bool& bStateComplete)		{ bStateComplete = true; return true; }     // Called upon job creation. If InitJob returns true, then you can be guaranteed that ShutdownJob will be called later. Use the TransportInfo data to decide what to do. A return value of false means to skip this TransportHandler and try another.
            virtual bool ShutdownJob    (TransportInfo* pTInfo, bool& bStateComplete)		{ bStateComplete = true; return true; }     // Called upon job shutdown. 
            virtual bool Connect        (TransportInfo* pTInfo, bool& bStateComplete)		{ bStateComplete = true; return true; }     // Called to begin transport. For file protocols, this is analagous to file open.
            virtual bool Disconnect     (TransportInfo* pTInfo, bool& bStateComplete)		{ bStateComplete = true; return true; }     // Called to end transport. For file protocols, this is analagous to file close.
            virtual bool Transfer       (TransportInfo* pTInfo, bool& bStateComplete)		{ bStateComplete = true; return true; }     // Called (usually repeatedly) to do transfer processing (read or write). For file protocols, this is analagous to file read or write.
            virtual bool CanCacheToDisk ()													{ return false; }							// Return true of loaded data should be cached to disk
            virtual bool IsLocalFile    ()													{ return false; }							// return true if you need locally determine the mime type
			virtual bool Tick			()													{ return true; }							// An application can put the network tick related code in this function. WebKit calls it internally at regular interval. Curently, from View::Tick().
        };



        // Implements a transport handler for the "file" scheme (e.g. "file:///c|/Windows/test.html").
        // This class handles only OS-native synchronous file IO, as with fopen/fclose, though it
        // uses the user-installed synchronous file IO facilities instead of directly calling the C FILE functions.
        // For code to handle asynchronous file IO though a custom file system such as rwfilesystem,
        // talk to Paul Pedriana or the current maintainer of this library. We will try to provide
        // a sample version of an easily installable rwfilesystem handler.
        class TransportHandlerFile : public TransportHandler
        {
        public:
            TransportHandlerFile();
           ~TransportHandlerFile();

            bool InitJob    (TransportInfo* pTInfo, bool& bStateComplete);
            bool ShutdownJob(TransportInfo* pTInfo, bool& bStateComplete);
            bool Connect    (TransportInfo* pTInfo, bool& bStateComplete);
            bool Disconnect (TransportInfo* pTInfo, bool& bStateComplete);
            bool Transfer   (TransportInfo* pTInfo, bool& bStateComplete);

            char* GetFileDownloadBuffer();                         /// Get and allocate a shared download buffer if needed.
            void RemoveFileDownloadBuffer();                             /// Remove the allocated buffer


            struct FileInfo
            {
                EA::WebKit::FileSystem::FileObject mFileObject;
                int64_t mFileSize;
                bool    mbRead;    // True if Reading, else writing
                FileInfo();
            };

            #ifdef EA_DEBUG
                int mJobCount;  // Used to verify jobs are all shut down and not leaked.
            #endif
            
            private:
              static char* spFileDownloadBuffer;                       /// Shared buffer for downloading blocking reads 
              static int32_t sFileOpenJobCount;                        /// Count for active jobs     

        };


        // Implements a transport handler for caching files for the http scheme.
        // If disk caching is enabled, files transferred over the network via HTTP 
        // can be cached to a local disk, subject to HTTP cache-expiry conditions.
        // This class acts like a transport handler for HTTP but in fact it will 
        // read the data from disk if possible and maintains a disk cache. 
        // Note that RAM caching of HTTP content is handled internally within WebKit 
        // and doesn't affect us here. We won't be called if the content was found 
        // in the RAM cache.
        class TransportHandlerHTTPCache : public TransportHandler
        {
        public:
            bool InitJob    (TransportInfo* pTInfo, bool& bStateComplete);
            bool ShutdownJob(TransportInfo* pTInfo, bool& bStateComplete);
            bool Connect    (TransportInfo* pTInfo, bool& bStateComplete);
            bool Disconnect (TransportInfo* pTInfo, bool& bStateComplete);
            bool Transfer   (TransportInfo* pTInfo, bool& bStateComplete);
        };


		//Helper API for the transport handler
		// Overwrites the entry if present, else creates it.
		EAWEBKIT_API void SetHeaderMapValue(EASTLHeaderMapWrapper& headerMapWrapper, const char16_t* pKey, const char16_t* pValue);
		// If key found, returns the char16_t* for of the value. Otherwise, returns NULL.
		EAWEBKIT_API const char16_t* GetHeaderMapValue(const EASTLHeaderMapWrapper& headerMapWrapper, const char16_t* pKey);
		// If key found, erase the element.
		EAWEBKIT_API void EraseHeaderMapValue(EASTLHeaderMapWrapper& headerMapWrapper, const char16_t* pKey);
		// Returns the required strlen, which may be >= textSize. 
		// You need to provide a larger textCapacity if the return value >= textCapacity.
		EAWEBKIT_API int32_t SetTextFromHeaderMapWrapper(const EASTLHeaderMapWrapper& headerMapWrapper, char* pHeaderMapText, uint32_t textCapacity);
		// We read multi-line text like the following:
		//    aaaa : bbbb\n
		//    aaaa:    bbbb\n
		//    aaaa:bb\n
		//     bb\n
		//    aaaa : bbbb\n
		EAWEBKIT_API bool SetHeaderMapWrapperFromText(const char* pHeaderMapText, uint32_t textSize, EASTLHeaderMapWrapper& headerMapWrapper, bool bExpectFirstCommandLine, bool bClearMap);

    }
}


#endif // Header include guard
