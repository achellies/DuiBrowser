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
// BCTransportHandlerDirtySDK.h
// Created by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////

#ifndef BCTransportHandlerDirtySDK_h
#define BCTransportHandlerDirtySDK_h

#include "PackageRedirector.h"


#include <EAWebKit/EAWebKitTransport.h>
#include <EAWebKit/EAWebkitAllocator.h>

#include <eastl/fixed_string.h>
#include <eastl/fixed_map.h>

namespace EA
{
	namespace EAWEBKIT_PACKAGE_NAMESPACE
	{
		class IStreamDecompressor;
	}
}
namespace EA
{
	namespace TransportHelper
	{
#ifdef BUILDING_EAWEBKIT_DLL
		typedef eastl::fixed_string<char16_t, 64, true,EA::WebKit::EASTLAllocator> TransportString16;
		typedef eastl::fixed_string<char8_t, 64, true,EA::WebKit::EASTLAllocator> TransportString8;

		// Used for HTTP header entries.
		struct fstr_iless : public eastl::binary_function<TransportString16, TransportString16, bool>
		{
			bool operator()(const TransportString16& a, const TransportString16& b) const { return (a.comparei(b) < 0); }
		};

		// Used for HeaderMap::find_as() calls.
		struct str_iless : public eastl::binary_function<TransportString16, const char16_t*, bool>
		{
			bool operator()(const TransportString16& a, const char16_t*      b) const { return (a.comparei(b) < 0); }
			bool operator()(const char16_t*      b, const TransportString16& a) const { return (a.comparei(b) > 0); }
		};
		typedef eastl::fixed_multimap<TransportString16, TransportString16, 8, true, fstr_iless, EA::WebKit::EASTLAllocator> TransportHeaderMap;
#else
		//Note by Arpit Baldeva: An application may want to provide their own allocator to following.
		typedef eastl::fixed_string<char16_t, 64, true/*,EA::WebKit::EASTLAllocator*/> TransportString16;
		typedef eastl::fixed_string<char8_t, 64, true/*,EA::WebKit::EASTLAllocator*/> TransportString8;

		// Used for HTTP header entries.
		struct fstr_iless : public eastl::binary_function<TransportString16, TransportString16, bool>
		{
			bool operator()(const TransportString16& a, const TransportString16& b) const { return (a.comparei(b) < 0); }
		};

		// Used for HeaderMap::find_as() calls.
		struct str_iless : public eastl::binary_function<TransportString16, const char16_t*, bool>
		{
			bool operator()(const TransportString16& a, const char16_t*      b) const { return (a.comparei(b) < 0); }
			bool operator()(const char16_t*      b, const TransportString16& a) const { return (a.comparei(b) > 0); }
		};
		//Note by Arpit Baldeva: An application may want to provide their own allocator to following.
		typedef eastl::fixed_multimap<TransportString16, TransportString16, 8, true, fstr_iless/*,EA::WebKit::EASTLAllocator*/> TransportHeaderMap;
#endif

	}
}

#include "protohttpmanager.h"
#include "dirtyvers.h" // Defines (e.g.) #define DIRTYVERS (0x07000000)


namespace EA
{
namespace EAWEBKIT_PACKAGE_NAMESPACE
{
	
class TransportHandlerDirtySDK : public EA::WebKit::TransportHandler
{
public:
    TransportHandlerDirtySDK();
   ~TransportHandlerDirtySDK();

    bool Init           (const char16_t* pScheme);
    bool Shutdown       (const char16_t* pScheme);
    bool InitJob        (EA::WebKit::TransportInfo* pTInfo, bool& bStateComplete);
    bool ShutdownJob    (EA::WebKit::TransportInfo* pTInfo, bool& bStateComplete);
    bool Connect        (EA::WebKit::TransportInfo* pTInfo, bool& bStateComplete);
    bool Disconnect     (EA::WebKit::TransportInfo* pTInfo, bool& bStateComplete);
    bool Transfer       (EA::WebKit::TransportInfo* pTInfo, bool& bStateComplete);
    bool CanCacheToDisk ();  
	bool Tick			();

#if DIRTYVERS > 0x07050300 
	static int32_t DirtySDKSendHeaderCallbackStatic(ProtoHttpRefT* pState, char* pHeader, uint32_t uHeaderSize, const char* pData, uint32_t uDataLen, void* pUserRef);
#else
	static void DirtySDKSendHeaderCallbackStatic(ProtoHttpRefT* pState, char* pHeader, uint32_t uHeaderSize, const char* pData, uint32_t uDataLen, void* pUserRef);
#endif
	int32_t        DirtySDKSendHeaderCallback(char* pHeader, uint32_t uHeaderSize, const char* pData, uint32_t uDataLen, EA::WebKit::TransportInfo* pTInfo);

#if DIRTYVERS > 0x07050300
	static int32_t DirtySDKRecvHeaderCallbackStatic(ProtoHttpRefT* pState, const char* pHeader, uint32_t uHeaderSize, void* pUserRef);
#else
	static void DirtySDKRecvHeaderCallbackStatic(ProtoHttpRefT* pState, const char* pHeader, uint32_t uHeaderSize, void* pUserRef);
#endif
	void        DirtySDKRecvHeaderCallback(const char* pHeader, uint32_t uHeaderSize, EA::WebKit::TransportInfo* pTInfo);

protected:
    
    HttpManagerRefT*             mpHttpManager;

    struct DirtySDKInfo  // Our info for a given job.
    {
        int32_t									mHttpHandle;           // Handle for current HTTP transaction
		EA::TransportHelper::TransportString8	mURI;
        bool									mbHeadersReceived;
        int										mSendIndex;            // If we receive 300-family redirects then DirtySDK will resend requests for the new URL. We keep track of this.

        EA::TransportHelper::TransportString8	mPostData;             // String which holds all POST data.
        bool									mbPostActive;          // Deprecated unless/until we go back to using chunked data. True if we are writing POST data. When we are done writing the POST data, we set this to false and go into receive mode as if we were doing GET.
        char									mPostBuffer[512];      // Deprecated unless/until we go back to using chunked data.
        int64_t									mPostBufferSize;       // Deprecated unless/until we go back to using chunked data. 
        int64_t									mPostBufferPosition;   // Deprecated unless/until we go back to using chunked data.
		IStreamDecompressor*					mStreamDecompressor;
        DirtySDKInfo();
		~DirtySDKInfo();
		
    };
#ifdef EA_DEBUG
	int mJobCount;  // Used to verify jobs are all shut down and not leaked.
#endif
	bool mIsDirtySockStartedHere;

};


} // namespace EAWEBKIT_PACKAGE_NAMESPACE
} // namespace EA


#endif // Header include guard
