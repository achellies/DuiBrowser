/*
Copyright (C) 2008-2011 Electronic Arts, Inc.  All rights reserved.

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
// BCTransportHandlerDirtySDK.cpp
// Created by Paul Pedriana 
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// How to POST with 'Content-Type: multipart/form-data' and 'Content-Type: application/x-www-form-urlencoded':
//     http://www.w3.org/TR/html401/interact/forms.html#submit-format
//
// Example POST using 'Content-Type: application/x-www-form-urlencoded':
//
//     POST /action HTTP/1.1
//     Host: www.blah.com
//     User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.4b) Gecko/20030516 Mozilla Firebird/0.6
//     Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,image/jpeg,image/gif;q=0.2,*/*;q=0.1
//     Connection: Close
//     Content-Type: application/x-www-form-urlencoded
//     Content-Length: 12 
//
//     var=12345678 
/////////////////////////////////////////////////////////////////////////////

#include "TransportHandler.h"
#include <EAWebKit/EAWebKitConfig.h>
#include <EAWebKit/EAWebKitView.h>
#include <EAAssert/eaassert.h>
#if defined(WTF_USE_DIRTYSDK) && WTF_USE_DIRTYSDK
//Note by Arpit Baldeva: Forward declare some functions from the DirtySDK here instead of including the
//header file. This is because DirtySDK has a header named "platform.h" which conflicts with soo many
//"platform.h" found in the WebKit and confuses the compiler.

#ifdef __cplusplus
extern "C" {
#endif
// create new instance of socket interface module
int32_t SocketCreate(int32_t iThreadPrio);
// release resources and destroy module.
int32_t SocketDestroy(uint32_t uFlags);

#ifdef __cplusplus
}
#endif

#include <EAWebKit/EAWebkitSTLWrapper.h>
#include <EAIO/FnEncode.h>          // For Strlcpy and friends.
#include <stdio.h>
#include "protossl.h"

#if defined(EA_PLATFORM_WINDOWS)
    #include <windows.h>
#elif defined(EA_PLATFORM_WINCE)
	#include <windows.h>
#endif

#define MULTICHAR_CONST(a,b,c,d) (a << 24 | b << 16 | c << 8 | d) 
#define DIRTY_OPEN MULTICHAR_CONST('o','p','e','n')
#define DIRTY_ESSL MULTICHAR_CONST('e','s','s','l')
#define DIRTY_PIPE MULTICHAR_CONST('p','i','p','e')
#define DIRTY_PIPE_WITHOUT_KEEP_ALIVE MULTICHAR_CONST('p','w','k','a')


#ifndef BUILDING_EAWEBKIT_DLL
#include "netconn.h"
#endif
#include "StreamDecompressor.h"

namespace EA
{
	namespace TransportHelper
	{
		char8_t* Stristr(const char8_t* s1, const char8_t* s2)
		{
			const char8_t* cp = s1;

			if(!*s2)
				return (char8_t*)s1;

			while(*cp)
			{
				const char8_t* s = cp;
				const char8_t* t = s2;

				while(*s && *t && (tolower(*s) == tolower(*t)))
					++s, ++t;

				if(*t == 0)
					return (char8_t*)cp;
				++cp;
			}

			return 0;
		}

		char16_t* Stristr(const char16_t* s1, const char16_t* s2)
		{
			const char16_t* cp = s1;

			if(!*s2)
				return (char16_t*)s1;

			while(*cp)
			{
				const char16_t* s = cp;
				const char16_t* t = s2;

				while(*s && *t && (tolower(*s) == tolower(*t)))
					++s, ++t;

				if(*t == 0)
					return (char16_t*)cp;
				++cp;
			}

			return 0;
		}


		int Stricmp(const char8_t* pString1, const char8_t* pString2)
		{
			char8_t c1, c2;

			// PowerPC has lhzu and sthu instructions which make pre-increment 
			// updates faster for the PowerPC's load and store architecture.
#ifdef EA_PROCESSOR_POWERPC
			--pString1;
			--pString2;
			while((c1 = tolower(*++pString1)) == (c2 = tolower(*++pString2)))
			{
				if(c1 == 0)
					return 0;
			}
#else
			while((c1 = tolower(*pString1++)) == (c2 = tolower(*pString2++)))
			{
				if(c1 == 0)
					return 0;
			}
#endif

			return (c1 - c2);
		}

		int Stricmp(const char16_t* pString1, const char16_t* pString2)
		{
			char16_t c1, c2;

			// PowerPC has lhzu and sthu instructions which make pre-increment 
			// updates faster for the PowerPC's load and store architecture.
#ifdef EA_PROCESSOR_POWERPC
			--pString1;
			--pString2;
			while((c1 = tolower(*++pString1)) == (c2 = tolower(*++pString2)))
			{
				if(c1 == 0)
					return 0;
			}
#else
			while((c1 = tolower(*pString1++)) == (c2 = tolower(*pString2++)))
			{
				if(c1 == 0)
					return 0;
			}
#endif

			return (c1 - c2);
		}

		size_t Strlen(const char8_t* pString)
		{
			return strlen(pString);
		}

		size_t Strlen(const char16_t* pString)
		{
			return wcslen(pString);
		}

		static void ConvertToString8(const EA::TransportHelper::TransportString16& s16, EA::TransportHelper::TransportString8& s8)
		{
			// A 16 bit string of strlen n16 will convert to an 8 bit UTF8 string with a strlen >= n16.
			eastl_size_t size16 = s16.size();
			eastl_size_t size8  = size16 + 8;   // Give it some extra space to detect that Strlcpy needed more space..

			// Most of the time we are copying ascii text and the dest length == source length.
			s8.resize(size8);
			size_t destLen = EA::IO::StrlcpyUTF16ToUTF8(&s8[0], size8 + 1, s16.c_str(), size16); // +1 because there is a 0 char at the end that is included in the string capacity.

			if(destLen > size16) // If there were multibyte expansions, and thus that we need to use a dest size > source size...
			{
				destLen = EA::IO::StrlcpyUTF16ToUTF8(NULL, 0, s16.c_str(), size16); // Call with NULL in order to get the required strlen.
				s8.resize((eastl_size_t)destLen);
				EA::IO::StrlcpyUTF16ToUTF8(&s8[0], destLen + 1, s16.c_str(), size16);
			}
			else
				s8.resize((eastl_size_t)destLen);
		}
		
		static void WriteHeaderMapEntry(EA::TransportHelper::TransportHeaderMap::key_type& sKey, EA::TransportHelper::TransportHeaderMap::mapped_type& sValue, EA::TransportHelper::TransportHeaderMap& headerMap)
		{
			while(!sKey.empty() && ((sKey.back() == ' ') || (sKey.back() == '\t')))
				sKey.pop_back(); // Remove trailing whitespace.

			while(!sValue.empty() && ((sValue.back() == ' ') || (sValue.back() == '\t')))
				sValue.pop_back(); // Remove trailing whitespace.

			if(!sKey.empty() && !sValue.empty())
			{
				const EA::TransportHelper::TransportHeaderMap::value_type entry(sKey, sValue);
				headerMap.insert(entry);
			}
		}

		static void CopyHeaderLine(const char16_t* pKey, const EA::TransportHelper::TransportHeaderMap& headerMap, char*& pHeader, uint32_t& uHeaderCapacity)
		{
			// Write the Host field.
			EA::TransportHelper::TransportHeaderMap::const_iterator it = headerMap.find_as(pKey, EA::TransportHelper::str_iless());

			if(it != headerMap.end())
			{
				const EA::TransportHelper::TransportHeaderMap::mapped_type& sKey   = it->first;
				const EA::TransportHelper::TransportHeaderMap::mapped_type& sValue = it->second;

				const uint32_t lineSize = (uint32_t)(sKey.length() + 2 + sValue.length() + 2 + 1); //1 for the terminating 0 char

				if(uHeaderCapacity > lineSize) // Use > instead of >= because we want to write a terminating 0 char.
				{
					for(eastl_size_t i = 0, iEnd = sKey.length(); i != iEnd; ++i)
						*pHeader++ = (char)sKey[i];

					*pHeader++ = ':';
					*pHeader++ = ' ';

					for(eastl_size_t i = 0, iEnd = sValue.length(); i != iEnd; ++i)
						*pHeader++ = (char)sValue[i];

					*pHeader++ = '\r';
					*pHeader++ = '\n';
					*pHeader   = 0;

					uHeaderCapacity -= lineSize; 
				}
			}
		}
		// We read multi-line text like the following:
		//    aaaa : bbbb\n
		//    aaaa:    bbbb\n
		//    aaaa:bb\n
		//     bb\n
		//    aaaa : bbbb\n

		static bool SetHeaderMapWrapperFromText(const char* pHeaderMapText, uint32_t textSize, EA::TransportHelper::TransportHeaderMap& headerMap, bool bExpectFirstCommandLine, bool bClearMap)
		{
			bool processedWithError = false;

			enum Mode
			{
				kModeKey,               // We are in the process of reading the key
				kModeKeyValueSeparator, // We are in the process of reading the characters (e.g. " : ") between the key and the value.
				kModeValue,             // We are in the process of reading the value (which may span multiple lines).
				kModeNewLine            // We are in the process of reading the newline after a value.
			};

			Mode               mode = kModeKey;
			const char*        p    = pHeaderMapText;
			const char*        pEnd = pHeaderMapText + textSize;
			EA::TransportHelper::TransportHeaderMap::key_type		sKey;
			EA::TransportHelper::TransportHeaderMap::mapped_type	sValue;
			const eastl_size_t kMaxKeySize = 128;
			const eastl_size_t kMaxValueSize = 2048;
			bool               inErrorCondition = false;
			bool               colonFound = false;

			if(bClearMap)
				headerMap.clear();

			if(bExpectFirstCommandLine)
			{
				// The first line should always be the status line, without a key:value pair. So skip it.
				while(*p && (*p != '\r') && (*p != '\n') && (p < pEnd))
					++p;

				if(*p == '\r')
					++p;

				if(*p == '\n')
					++p;
			}

			if((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
			{
				processedWithError = true;
				return processedWithError;
			}

			while(*p && (p < pEnd))
			{
				char c = *p++;

				switch(mode)
				{
				case kModeKey:
					if((c == '\r') || (c == '\n'))  // If we have reached a newline before reading whitespace or ':' char, the header is malformed.
					{
						processedWithError = inErrorCondition = true;
						mode = kModeNewLine;
					}
					else if((c == ' ') || (c == '\t') || (c == ':'))
					{
						mode = kModeKeyValueSeparator;
						if(c == ':')
							colonFound = true;
					}
					else
					{
						if(sKey.length() < kMaxKeySize)
							sKey += (wchar_t)(c);
					}
					break;

				case kModeKeyValueSeparator:
					if((c == '\r') ||(c == '\n')) // If we have reached a newline before any chars of the value, the value is empty. 
					{
						inErrorCondition = !colonFound; // We are OK only if a colon was found between the key and the value.
						mode = kModeNewLine;
					}
					else if((c != ' ') && (c != '\t') && (c != ':')) // If it is part of the value...
					{
						sValue = (uint8_t)c;
						mode   = kModeValue;
					}
					else if(c == ':')
						colonFound = true;

					break;

				case kModeValue:
					if((c == '\r') || (c == '\n')) // If we reached the end of the line...
						mode = kModeNewLine;
					else
					{
						if(sValue.length() < kMaxValueSize)
							sValue += (uint8_t)c;
					}
					break;

				case kModeNewLine:
					{
						if((c != '\r') && (c != '\n')) // If we have reached the next line...
						{
							if((c == ' ') || (c == '\t'))  // If we have a value that is continued from the previous line...
							{
								mode = kModeValue; // Go back to appending to the value.

								// Eat leading whitespace.
								while((p < pEnd) && ((c = *p) == ' ') && (c == '\t'))
									p++;
							}
							else
							{
								// We beginning the new key/value pair. Finalize the previous one.
								if(!inErrorCondition) //Make sure that you are not in an error condition when writing this Key-Value pair
								{
									if((sKey.length() < kMaxKeySize) && (sValue.length() < kMaxValueSize)) // If no overflow occurred...
										EA::TransportHelper::WriteHeaderMapEntry(sKey, sValue, headerMap);
								}
								else
									processedWithError = true; //Also, indicate to the caller that you did encounter some error in processing.

								inErrorCondition = false;  // Reset the error condition for next line.
								mode = kModeKey;           // This is part of a new key.
								colonFound = false;
								sKey = (uint8_t)c;
								sValue.clear();
							}
						}

						break;
					}
				}
			}

			// Finalize the last entry.
			if(!inErrorCondition && (sKey.length() < kMaxKeySize) && (sValue.length() < kMaxValueSize)) // If no overflow occurred...
				EA::TransportHelper::WriteHeaderMapEntry(sKey, sValue, headerMap);

			return processedWithError;
		}


	}
}

namespace EA
{
namespace EAWEBKIT_PACKAGE_NAMESPACE
{
#if ENABLE_PAYLOAD_DECOMPRESSION	
	static void DecompressedDataCallbackFunc(void* userData, uint8_t* decompressedData, uint32_t decompressedDataLength)
	{
		EA::WebKit::TransportInfo* pTInfo = reinterpret_cast<EA::WebKit::TransportInfo*>(userData);
		pTInfo->mpTransportServer->DataReceived(pTInfo, decompressedData, decompressedDataLength);
	}
#endif

	TransportHandlerDirtySDK::DirtySDKInfo::DirtySDKInfo():
		mHttpHandle(0), 
		mURI(), 
		mbHeadersReceived(false), 
		mSendIndex(0), 
		mPostData(), 
		mbPostActive(false), 
		mPostBufferSize(0), 
		mPostBufferPosition(0), 
		mStreamDecompressor(0) 
		{ 

		}

	TransportHandlerDirtySDK::DirtySDKInfo::~DirtySDKInfo()
	{
#if ENABLE_PAYLOAD_DECOMPRESSION
		if(mStreamDecompressor)
		{
			mStreamDecompressor->~IStreamDecompressor();
			EA::WebKit::Allocator* pAllocator = ACCESS_EAWEBKIT_API(GetAllocator());
			pAllocator->Free(mStreamDecompressor,0);
			mStreamDecompressor = 0;
		}
#endif
	}
TransportHandlerDirtySDK::TransportHandlerDirtySDK()
:
mpHttpManager(NULL),
#ifdef EA_DEBUG
mJobCount(0),
#endif
mIsDirtySockStartedHere(false)
{

}


TransportHandlerDirtySDK::~TransportHandlerDirtySDK()
{
    Shutdown(NULL); // Just in case it somehow was missed.

    #ifdef EA_DEBUG
        EAW_ASSERT(mJobCount == 0); // If this fails then the TransportServer is leaking jobs.
    #endif
}


bool TransportHandlerDirtySDK::Init(const char16_t* /*pScheme*/)
{

#ifdef BUILDING_EAWEBKIT_DLL
	SocketCreate(0);
#else
	bool isStarted = NetConnStatus(DIRTY_OPEN, 0, NULL, 0);	
	if ( !isStarted )
	{
		mIsDirtySockStartedHere = true;
		NetConnStartup(""); 
	}
#endif
	
    //$$note -- called twice with the same transport handler
    if (!mpHttpManager)
    {
        mpHttpManager = HttpManagerCreate(4096, 16);
        // set callback function pointers
#if DIRTYVERS > 0x07050300
		HttpManagerCallback(mpHttpManager, &TransportHandlerDirtySDK::DirtySDKSendHeaderCallbackStatic, &TransportHandlerDirtySDK::DirtySDKRecvHeaderCallbackStatic);
#else
		HttpManagerCallback(mpHttpManager, &TransportHandlerDirtySDK::DirtySDKSendHeaderCallbackStatic, &TransportHandlerDirtySDK::DirtySDKRecvHeaderCallbackStatic, NULL);
#endif
		// set the redirection limit to a higher value
        HttpManagerControl(mpHttpManager, -1, 0x726d6178 /*'rmax'*/, 10, 0, NULL); 
		
		//Set timeout for DirtySDK based on the page timeout of EAWebKit params
		EA::WebKit::Parameters& params = ACCESS_EAWEBKIT_API(GetParameters());
		HttpManagerControl(mpHttpManager, -1, 0x74696d65 /*'time'*/, (int32_t)(params.mPageTimeoutSeconds * 1000.0), 0, NULL);

        // set debug level
        #if defined(EA_DEBUG) || defined(_DEBUG)
        HttpManagerControl(mpHttpManager, -1, 0x7370616d /*'spam'*/, 1, 0, NULL); 
        #endif
    }


    return true;
}


bool TransportHandlerDirtySDK::Shutdown(const char16_t* /*pScheme*/)
{
    //$$note -- called twice with the same transport handler
    if (mpHttpManager)
    {
        HttpManagerDestroy(mpHttpManager);
        mpHttpManager = NULL;
    }

#ifdef BUILDING_EAWEBKIT_DLL
	SocketDestroy(0);
#else
	if ( mIsDirtySockStartedHere )
	{
		NetConnShutdown(0); 
		mIsDirtySockStartedHere = false;
	}
#endif
	
	return true;
}


bool TransportHandlerDirtySDK::InitJob(EA::WebKit::TransportInfo* pTInfo, bool& bStateComplete)
{
	EA::WebKit::Allocator* pAllocator = ACCESS_EAWEBKIT_API(GetAllocator());
    DirtySDKInfo* pDirtySDKInfo = new(pAllocator->Malloc(sizeof(DirtySDKInfo), 0, "EAWebKit/TransportHandlerDirtySDK")) DirtySDKInfo; 
	pTInfo->mTransportHandlerData = (uintptr_t)pDirtySDKInfo;

    // allocate an HTTP transfer handle
    pDirtySDKInfo->mHttpHandle = HttpManagerAlloc(mpHttpManager);

    if (pDirtySDKInfo->mHttpHandle == 0)
    {
        EAW_ASSERT_MSG(false,"TransportHandlerDirtySDK: ProtoHttpCreate failed.\n");
        return false;
    }

    // disable certificate validation if set to do so. An application normally doesn't want to do this, but it's useful for debugging.
    if(!pTInfo->mbVerifyPeers)
        HttpManagerControl(mpHttpManager, pDirtySDKInfo->mHttpHandle, 0x6e637274 /*'ncrt'*/, 1, 0, NULL); 
   
	// set client timeout
    //HttpManagerControl(mpHttpManager, pDirtySDKInfo->mHttpHandle, 0x74696d65 /*'time'*/, (int32_t)(pTInfo->mTimeoutInterval * 1000.0), 0, NULL);

    // Copy the 16 bit URI to our 8 bit version. No encoding translations are done.
	
	const char16_t* tInfoURI = ACCESS_EAWEBKIT_API(GetCharacters(pTInfo->mURI));
	size_t iEnd = EA::TransportHelper::Strlen(tInfoURI);
	
	pDirtySDKInfo->mURI.resize(iEnd+1);

	for(size_t i = 0; i < iEnd; ++i)
        pDirtySDKInfo->mURI[i] = (char8_t)(tInfoURI[i]);
	
	pDirtySDKInfo->mURI[iEnd] ='\0';
    
	// Make sure our header map has an Accept-Encoding entry.
    // By default WebKit doesn't create this header, nor does DirtySDK, 
    // yet some web servers are non-conforming and require it.
	if(!ACCESS_EAWEBKIT_API(GetHeaderMapValue(pTInfo->mHeaderMapOut,L"Accept-Encoding")))
	{
#if ENABLE_PAYLOAD_DECOMPRESSION
		//Note by Arpit Baldeva: put deflate first as it has a lower overhead compared to gzip. Since we support both raw DEFLATE stream and zlib wrapped DEFLATE stream transparently,
		//we end up with lower overhead.
		ACCESS_EAWEBKIT_API(SetHeaderMapValue(pTInfo->mHeaderMapOut,L"Accept-Encoding", L"deflate,gzip,identity"));
#else
		ACCESS_EAWEBKIT_API(SetHeaderMapValue(pTInfo->mHeaderMapOut,L"Accept-Encoding", L"identity"));
#endif
	}
	
	pDirtySDKInfo->mbHeadersReceived = false;

    #ifdef EA_DEBUG
        mJobCount++;
    #endif

    bStateComplete = true;
    return true;
}


bool TransportHandlerDirtySDK::ShutdownJob(EA::WebKit::TransportInfo* pTInfo, bool& bStateComplete)
{
    if(pTInfo->mTransportHandlerData)
    {
        DirtySDKInfo*          pDirtySDKInfo  = (DirtySDKInfo*)pTInfo->mTransportHandlerData;
        EA::WebKit::Allocator* pAllocator     = ACCESS_EAWEBKIT_API(GetAllocator());

        if(pDirtySDKInfo->mHttpHandle != 0)
            HttpManagerFree(mpHttpManager, pDirtySDKInfo->mHttpHandle);

        pDirtySDKInfo->~DirtySDKInfo();
        pAllocator->Free(pDirtySDKInfo, sizeof(DirtySDKInfo));
        pTInfo->mTransportHandlerData = 0;

        #ifdef EA_DEBUG
            mJobCount--;
        #endif
    }

    bStateComplete = true;
    return true;
}


bool TransportHandlerDirtySDK::Connect(EA::WebKit::TransportInfo* pTInfo, bool& bStateComplete)
{
    bool    bReturnValue = true;
    int32_t iResult = 0;

    EAW_ASSERT(pTInfo->mTransportHandlerData);

    DirtySDKInfo* pDirtySDKInfo = (DirtySDKInfo*)pTInfo->mTransportHandlerData;

    // set callback user info
    HttpManagerControl(mpHttpManager, pDirtySDKInfo->mHttpHandle, MULTICHAR_CONST('c','b','u','p'), 0, 0, (void *)pTInfo);
   
	switch(pTInfo->mHttpRequestType)
	{
	case EA::WebKit::kHttpRequestTypeGET:
	case EA::WebKit::kHttpRequestTypeHEAD:
		//iResult = HttpManagerGet(mpHttpManager, pDirtySDKInfo->mHttpHandle, pDirtySDKInfo->mURI.c_str(), PROTOHTTP_HEADBODY);
		iResult = HttpManagerRequest(mpHttpManager, pDirtySDKInfo->mHttpHandle, pDirtySDKInfo->mURI.c_str(), NULL, 0, ((pTInfo->mHttpRequestType == EA::WebKit::kHttpRequestTypeGET) ? PROTOHTTP_REQUESTTYPE_GET : PROTOHTTP_REQUESTTYPE_HEAD));
		break;
	case EA::WebKit::kHttpRequestTypePOST:
	case EA::WebKit::kHttpRequestTypePUT:
		{

			// Original code that did a chunked send but which doesn't work with HTTP servers.
			// pDirtySDKInfo->mbPostActive        = true;
			// pDirtySDKInfo->mPostBufferSize     = 0;
			// pDirtySDKInfo->mPostBufferPosition = 0;
			// iResult = ProtoHttpPost(pDirtySDKInfo->mpProtoHttp, pDirtySDKInfo->mURI.c_str(), NULL, PROTOHTTP_STREAM_BEGIN, PROTOHTTP_POST);
			// if(iResult == PROTOHTTP_STREAM_BEGIN) // PROTOHTTP_STREAM_BEGIN == -1, so we can have an iResult that is negative yet not really an error.
			//    iResult = 0;

			// New code whereby we don't use chunked data.
			pDirtySDKInfo->mPostData.clear();      // Shouldn't be necessary.
			pDirtySDKInfo->mbPostActive = false;    // Skip right to reading the data from the server (see the Transfer function).

			char    buffer[256];
			int64_t size;

			do {
				size = pTInfo->mpTransportServer->ReadData(pTInfo, buffer, sizeof(buffer));
				if(size > 0)
					pDirtySDKInfo->mPostData.append(buffer, size);
			} while(size > 0);

			EAW_ASSERT(size == 0);
			if(size == 0)
			{
				char16_t bufferLen[32];
				_snwprintf(bufferLen,32, L"%u",pDirtySDKInfo->mPostData.length());

				ACCESS_EAWEBKIT_API(SetHeaderMapValue(pTInfo->mHeaderMapOut, L"Content-Length", bufferLen));
				//iResult = HttpManagerPost(mpHttpManager, pDirtySDKInfo->mHttpHandle, pDirtySDKInfo->mURI.c_str(), pDirtySDKInfo->mPostData.c_str(), (int32_t) pDirtySDKInfo->mPostData.length(), PROTOHTTP_POST);
				iResult = HttpManagerRequest(mpHttpManager, pDirtySDKInfo->mHttpHandle, pDirtySDKInfo->mURI.c_str(), pDirtySDKInfo->mPostData.c_str(), (int32_t) pDirtySDKInfo->mPostData.length(), ((pTInfo->mHttpRequestType == EA::WebKit::kHttpRequestTypePOST) ? PROTOHTTP_REQUESTTYPE_POST : PROTOHTTP_REQUESTTYPE_PUT));

			}
			break;
		}

	case EA::WebKit::kHttpRequestTypeOPTIONS:
		//iResult = HttpManagerRequest(mpHttpManager, pDirtySDKInfo->mHttpHandle, pDirtySDKInfo->mURI.c_str(), NULL, 0, PROTOHTTP_REQUESTTYPE_OPTIONS);
		break;
	case EA::WebKit::kHttpRequestTypeDELETE:
		iResult = HttpManagerRequest(mpHttpManager, pDirtySDKInfo->mHttpHandle, pDirtySDKInfo->mURI.c_str(), NULL, 0, PROTOHTTP_REQUESTTYPE_DELETE);
		break;
	default:
		EAW_ASSERT_MSG(false,"Unsupported Http request method\n");
		break;
	}

    if(iResult < 0)
    {
        EAW_ASSERT_MSG(false, "TransportHandlerDirtySDK: ProtoHttpGet or ProtoHttpPost failed.\n");
        bReturnValue = false;
    }

    bStateComplete = true;
    return bReturnValue;
}




#if DIRTYVERS > 0x07050300 
int32_t 
#else
void
#endif
TransportHandlerDirtySDK::DirtySDKSendHeaderCallbackStatic(ProtoHttpRefT* pState, char* pHeader, uint32_t uHeaderCapacity, const char* pBody, uint32_t uBodyLen, void* pUserRef)
{
    EA::WebKit::TransportInfo* pTInfo = static_cast<EA::WebKit::TransportInfo*>(pUserRef);
#if DIRTYVERS > 0x07050300 
    int32_t iResult = ((TransportHandlerDirtySDK*)pTInfo->mpTransportHandler)->DirtySDKSendHeaderCallback(pHeader, uHeaderCapacity, pBody, uBodyLen, pTInfo);
	return iResult;
#else
    ((TransportHandlerDirtySDK*)pTInfo->mpTransportHandler)->DirtySDKSendHeaderCallback(pHeader, uHeaderCapacity, pBody, uBodyLen, pTInfo);
#endif
}

// This is called by DirtySDK before it sends headers to the HTTP server. We can possibly revise the headers here.
//
// pHeader is 0-terminated the raw header text received by ProtoHttp.
// uHeaderCapacity is the capacity of the buffer pointed to by pHeader, which will usually be > than the strlen of pHeader.
// pData is the immutable body data that will be sent. It is data and so not necessarily 0-terminated.
// uDataLen is the length of the body data.
//
// We are expected to possibly rewrite pHeader, though we can't use more than uHeaderCapacity space.
//
int32_t TransportHandlerDirtySDK::DirtySDKSendHeaderCallback(char* pHeader, uint32_t uHeaderCapacity, const char* /*pBody*/, uint32_t /*uBodyLen*/, EA::WebKit::TransportInfo* pTInfo)
{
	
	EA::TransportHelper::TransportHeaderMap     headerMapDSDK;
    EA::TransportHelper::TransportString16		sCommandLine;
    char*                     p = pHeader;
    DirtySDKInfo*             pDirtySDKInfo = (DirtySDKInfo*)pTInfo->mTransportHandlerData;

    pDirtySDKInfo->mSendIndex++;  // The first time through, this changes from 0 to 1.

    if(pDirtySDKInfo->mSendIndex > 1) // If this is the 2nd or later time through... then that means we have been redirected by the server to a new URL...
    {
		// If this is a redirect (mSendIndex > 1) then remove cookies from the 
		// previous time through (when mSendIndex == 0) and re-add them now, as they may
		// have changed, especially if the redirect happened to give us a new cookie. 
		// Some servers (including EA servers) rely on this.
		ACCESS_EAWEBKIT_API(ReattachCookies(pTInfo));
    }

    // if(pDirtySDKInfo->mSendIndex == 1) // If this is the first GET and not a possible subsequent GET after a (e.g. 302) redirect...
    {
        // Save the first line from pHeader, as it is the command line (e.g. the "GET" line).
        while(*p && ((*p != '\r') && (*p != '\n')))
            sCommandLine += (uint8_t)*p++;

        // Move past the new line.
        while(*p && ((*p == '\r') || (*p == '\n')))
            p++;

        // We read the text into a temp header map, do any modifications, then write the result out.
        bool errorEncountered = EA::TransportHelper::SetHeaderMapWrapperFromText(p, strlen(p), headerMapDSDK, false, true);
        (void)(errorEncountered);
		//TODO: Do something useful here in the release build? Indicate the error?
		EAW_ASSERT_MSG(!errorEncountered, "The incoming text header map has some error. The resource response object created using partial header info may not be valid.");

        // Write sCommandLine
        const uint32_t commandSize = (uint32_t)(sCommandLine.length() + 2); // +2 for \r\n. 

        if(uHeaderCapacity >= commandSize) // Use > instead of >= because we want to write a terminating 0 char.
        {
            for(size_t i = 0, iEnd = sCommandLine.length(); i != iEnd; ++i)
                *pHeader++ = (uint8_t)sCommandLine[i];

            *pHeader++ = '\r';
            *pHeader++ = '\n';
            *pHeader   = 0;

            uHeaderCapacity -= commandSize;
        }

        // Fix the Host header to remove :80 if it is present. Some web servers (e.g. Google's www.gstatic.com or www.twitter.com) 
		// are broken and expect that :80 is never present, even though :80 is valid.
		EA::TransportHelper::TransportHeaderMap::iterator itHost = headerMapDSDK.find_as(L"host", EA::TransportHelper::str_iless());

		if(itHost != headerMapDSDK.end())  // This should always be true.
		{
			//Note by Arpit Baldeva: As it happens, we can send a request as "http". Server redirects that request to a "https" url. Now, DirtySDK
			//send the headers back but we failed to strip the port information in following logic previously because our scheme stays as "http".
			//Following is a workaround for this.
			
			//no other side effects are known at this time of EAWebKit and DirtySDK being a little our of sync
			//in this case. Probably a better fix would be to changes the transportInfo scheme but I am not sure of the ramifications currently.
			if(EA::TransportHelper::Stricmp(pTInfo->mScheme, L"http") == 0 || EA::TransportHelper::Stricmp(pTInfo->mScheme, L"https") == 0)
			{
				EA::TransportHelper::TransportHeaderMap::mapped_type& sValueHost = itHost->second;
				size_t sizeStr = sValueHost.size();
				if(sValueHost.substr(sizeStr-3,sizeStr) == L":80") //strlen ":80" = 3
				{
					sValueHost.erase(sizeStr-3);  // Remove the ":80", as some servers mistakenly fail when it's present for HTTP.
				}
				else if(sValueHost.substr(sizeStr-4,sizeStr) == L":443")//strlen ":443" = 4
				{
					sValueHost.erase(sizeStr-4);  // Remove the ":443", as some servers mistakenly fail when it's present for HTTPS.
				}
			}
			/*else if(EA::TransportHelper::Stricmp(pTInfo->mScheme, L"https") == 0)
			{
				EA::TransportHelper::TransportHeaderMap::mapped_type& sValueHost = itHost->second;
				size_t sizeStr = sValueHost.size();
				
			}*/
		}

		EA::TransportHelper::CopyHeaderLine(L"host",              headerMapDSDK, pHeader, uHeaderCapacity);
        EA::TransportHelper::CopyHeaderLine(L"transfer-encoding", headerMapDSDK, pHeader, uHeaderCapacity);
        EA::TransportHelper::CopyHeaderLine(L"connection",        headerMapDSDK, pHeader, uHeaderCapacity);
  
		//TODO: DirtySDK user-agent detection. Make this a simple static operation. Do it once at the transport handler init.

		// We append the DirtySDK user-agent string to our user-agent string.
		EA::TransportHelper::TransportHeaderMap::const_iterator itDSDK = headerMapDSDK.find_as(L"user-agent", EA::TransportHelper::str_iless());
		if(itDSDK != headerMapDSDK.end())  // This happens to always be true.
		{
			const EA::TransportHelper::TransportHeaderMap::mapped_type& sValueDSDK = itDSDK->second;
			EA::TransportHelper::TransportString16 userAgentWebKit(ACCESS_EAWEBKIT_API(GetHeaderMapValue(pTInfo->mHeaderMapOut, L"user-agent")));
			if(!userAgentWebKit.empty())
			{
				if(pDirtySDKInfo->mSendIndex == 1) // Only do it the first time (i.e. not on redirects), lest we repeat appending each time through.
				{
					userAgentWebKit += L' ';
					userAgentWebKit += sValueDSDK;
					ACCESS_EAWEBKIT_API(SetHeaderMapValue(pTInfo->mHeaderMapOut,L"user-agent",userAgentWebKit.c_str()));
				}
			}
			else
			{
				ACCESS_EAWEBKIT_API(SetHeaderMapValue(pTInfo->mHeaderMapOut,L"user-agent",sValueDSDK.c_str()));
			}
		}
		
        *pHeader = 0; // In case there are no header lines in mHeaderMapOut which we write next.

        if(pTInfo->mHttpRequestType == EA::WebKit::kHttpRequestTypePOST)
            ACCESS_EAWEBKIT_API(SetHeaderMapValue(pTInfo->mHeaderMapOut, L"Content-Type", L"application/x-www-form-urlencoded"));

        // Nicki Vankoughnett: A redirection may create a situation whereby we need to send different
        // headers than we sent for the original request. An example would be if the original request
        // was a POST (with body content) but the redirect was to a location that we need to GET from,
        // whereby we no longer have to provide the body content and thus its associated headers.
		if(sCommandLine.find(L"GET") == 0)
		{
			ACCESS_EAWEBKIT_API(EraseHeaderMapValue(pTInfo->mHeaderMapOut,L"Content-Length"));
			ACCESS_EAWEBKIT_API(EraseHeaderMapValue(pTInfo->mHeaderMapOut,L"Content-Type"));
		}

        return(ACCESS_EAWEBKIT_API(SetTextFromHeaderMapWrapper(pTInfo->mHeaderMapOut, pHeader, uHeaderCapacity)));
    }
}


#if DIRTYVERS > 0x07050300 
int32_t 
#else
void
#endif
TransportHandlerDirtySDK::DirtySDKRecvHeaderCallbackStatic(ProtoHttpRefT* pState, const char* pHeader, uint32_t uHeaderSize, void* pUserRef)
{
    EA::WebKit::TransportInfo* pTInfo = static_cast<EA::WebKit::TransportInfo*>(pUserRef);

    ((TransportHandlerDirtySDK*)pTInfo->mpTransportHandler)->DirtySDKRecvHeaderCallback(pHeader, uHeaderSize, pTInfo);
#if DIRTYVERS > 0x07050300 
	return 0;
#endif
}

void TransportHandlerDirtySDK::DirtySDKRecvHeaderCallback(const char* pHeader, uint32_t uHeaderSize, EA::WebKit::TransportInfo* pTInfo)
{
    // pHeader includes all the received header text.
    DirtySDKInfo* pDirtySDKInfo = (DirtySDKInfo*)pTInfo->mTransportHandlerData;

	//Note by Arpit Baldeva: Don't return early. Breaks facebook login. I have not done a deep investigation but may be it is a redirect issue.
	//if(pDirtySDKInfo->mbHeadersReceived)
	//	return; 
	///EAW_ASSERT_MSG(!pDirtySDKInfo->mbHeadersReceived, "Headers were already received and processed\n");
	pDirtySDKInfo->mbHeadersReceived = true;

	// Check for a 200, 404, etc. code. ProtoHttpStatus returns -1 if headers have not been successfully received.
	pTInfo->mResultCode = HttpManagerStatus(mpHttpManager, pDirtySDKInfo->mHttpHandle, 0x636f6465 /*'code'*/, NULL, 0);

// 	Expected document length
// 	Note by Arpit Baldeva: Following does not work anymore. I think because it is querying "body". We simply parse it from the headers.
// 	int32_t contentLength = HttpManagerStatus(mpHttpManager, pDirtySDKInfo->mHttpHandle, 0x626f6479 /*'body'*/, NULL, 0);
//  	if(contentLength >= 0) 
//  		pTInfo->mpTransportServer->SetExpectedLength(pTInfo, static_cast<int64_t>(contentLength));

	// Check for a Content-Length header
	const char* pContentLength = EA::TransportHelper::Stristr(pHeader, "\nContent-Length");
	if(pContentLength)
	{
		pContentLength = EA::TransportHelper::Stristr(pContentLength, ":") + 1;
		while((*pContentLength == ' ') || (*pContentLength == '\t'))
			++pContentLength;

		const char* pContentLengthEnd = pContentLength;

		while(*pContentLengthEnd != '\r' && *pContentLengthEnd != '\n' && *pContentLengthEnd != '\0' && (pContentLengthEnd < (pHeader + uHeaderSize)))
			++pContentLengthEnd;

		if(pContentLengthEnd > pContentLength)  // If not empty...
		{
			char contentLengthBuffer[32];
			strncpy(contentLengthBuffer,pContentLength,pContentLengthEnd-pContentLength);
			contentLengthBuffer[pContentLengthEnd-pContentLength]='\0';
			int32_t contentLength;
			sscanf(contentLengthBuffer,"%d",&contentLength);
			if(contentLength >= 0) 
				pTInfo->mpTransportServer->SetExpectedLength(pTInfo, static_cast<int64_t>(contentLength));
		}
	}

	const char* pLocation = EA::TransportHelper::Stristr(pHeader, "\nLocation");

    if(pLocation)
    {
        // We have something like "Location: http://eucr-webdev03.eu.ad.ea.com/abc/def"
        // Some servers violate the HTTP Standard and send domain-relative URLs instead of 
        // full URLs. For example http://www.pogo.com/ redirects to /home/home.do, which is
        // invalid but is supported by some browsers.

        pLocation = EA::TransportHelper::Stristr(pLocation, ":") + 1;
        while((*pLocation == ' ') || (*pLocation == '\t'))
            ++pLocation;

        const char* pLocationEnd = pLocation;

        while(*pLocationEnd != '\r' && *pLocationEnd != '\n' && *pLocationEnd != '\0' && (pLocationEnd < (pHeader + uHeaderSize)))
            ++pLocationEnd;

        if(pLocationEnd > pLocation)  // If not empty...
        {
            EA::TransportHelper::TransportString8 sLocationValue(pLocation, pLocationEnd);

            if(sLocationValue.find("://") > 6) // If it doesn't appear to be a fully qualified URL...
            {
				EA::TransportHelper::TransportString16 uriOriginal16(ACCESS_EAWEBKIT_API(GetCharacters(pTInfo->mURI)));
				EA::TransportHelper::TransportString8 uriOriginal;
                EA::TransportHelper::ConvertToString8(uriOriginal16, uriOriginal);

                eastl_size_t domainPos = uriOriginal.find("://"); // We have a problem if this is not found, but not much to do about it.
                if(domainPos != EA::TransportHelper::TransportString8::npos)
                {
                    eastl_size_t pathPos = uriOriginal.find('/', domainPos + 3);

                    if(pathPos == EA::TransportHelper::TransportString8::npos)
                        pathPos = uriOriginal.size();

                    uriOriginal.resize(pathPos); // Convert urlOriginal from something like http://www.pogo.com/abc/def to something like http://www.pogo.com
                    sLocationValue.insert(0, uriOriginal); // Convert sLocationValue from something like /home/home.do to http://www.pogo.com/home/home.do
                }
            }

            pTInfo->mpTransportServer->SetEffectiveURI(pTInfo, sLocationValue.c_str());

            if((pTInfo->mResultCode >= 300) && (pTInfo->mResultCode < 400)) //The error code is now valid because it is set at the top of the function.             
               pTInfo->mpTransportServer->SetRedirect(pTInfo, sLocationValue.c_str());
        }
    }

#if ENABLE_PAYLOAD_DECOMPRESSION
	const char* contentEncoding = EA::TransportHelper::Stristr(pHeader, "\nContent-Encoding");

	if(contentEncoding)
	{
		contentEncoding = EA::TransportHelper::Stristr(contentEncoding, ":") + 1;
		while((*contentEncoding == ' ') || (*contentEncoding == '\t'))
			++contentEncoding;

		const char* contentEncodingEnd = contentEncoding;

		while(*contentEncodingEnd != '\r' && *contentEncodingEnd != '\n' && *contentEncodingEnd != '\0' && (contentEncodingEnd < (pHeader + uHeaderSize)))
			++contentEncodingEnd;

		if(contentEncodingEnd > contentEncoding)  // If not empty...
		{
			EA::WebKit::Allocator* pAllocator = ACCESS_EAWEBKIT_API(GetAllocator());
			//This can happen, for example, on a redirect. Free the existing decompressor in case the content encoding type changes on the redirect.
			if(pDirtySDKInfo->mStreamDecompressor)
			{
				pDirtySDKInfo->mStreamDecompressor->~IStreamDecompressor();
				ACCESS_EAWEBKIT_API(GetAllocator())->Free(pDirtySDKInfo->mStreamDecompressor,0);
				pDirtySDKInfo->mStreamDecompressor = 0;
			}
			
			EA::TransportHelper::TransportString8 sContentEncodingValue(contentEncoding, contentEncodingEnd);
			if(sContentEncodingValue.comparei("deflate") == 0)
			{
				
				pDirtySDKInfo->mStreamDecompressor = new(pAllocator->Malloc(sizeof(DeflateStreamDecompressor),0,0)) DeflateStreamDecompressor(EA::EAWEBKIT_PACKAGE_NAMESPACE::eStreamTypeZLib);
				pDirtySDKInfo->mStreamDecompressor->SetDecompressedDataCallback(DecompressedDataCallbackFunc, pTInfo);
			}
			else if(sContentEncodingValue.comparei("gzip") == 0 )
			{
				pDirtySDKInfo->mStreamDecompressor = new(pAllocator->Malloc(sizeof(DeflateStreamDecompressor),0,0)) DeflateStreamDecompressor(EA::EAWEBKIT_PACKAGE_NAMESPACE::eStreamTypeGZip);
				pDirtySDKInfo->mStreamDecompressor->SetDecompressedDataCallback(DecompressedDataCallbackFunc, pTInfo);
			}
		}
	}
	else
	{
		//This can happen, for example, on a redirect where the original response was compressed but redirected response is not compressed. Free the existing decompressor.
		if(pDirtySDKInfo->mStreamDecompressor)
		{
			pDirtySDKInfo->mStreamDecompressor->~IStreamDecompressor();
			ACCESS_EAWEBKIT_API(GetAllocator())->Free(pDirtySDKInfo->mStreamDecompressor,0);
			pDirtySDKInfo->mStreamDecompressor = 0;
		}
	}
#endif
	
	
	//Store the headers in the incoming header map.
	bool errorEncountered = ACCESS_EAWEBKIT_API(SetHeaderMapWrapperFromText(pHeader, uHeaderSize, pTInfo->mHeaderMapIn, true, true));
	(void) errorEncountered;
	EAW_ASSERT_MSG(!errorEncountered, "The incoming text header map has some error. The resource response object created using partial header info may not be valid.");

	pTInfo->mpTransportServer->HeadersReceived(pTInfo);

	//Update the cookie manager with cookies
	ACCESS_EAWEBKIT_API(CookiesReceived(pTInfo));
}


bool TransportHandlerDirtySDK::Disconnect(EA::WebKit::TransportInfo* /*pTInfo*/, bool& bStateComplete)
{
    // We don't do any kind of disconnect here. We do all disconnect/shutdown 
    // functionality in out ShutdownJob function.
    bStateComplete = true;
    return true;
}

bool TransportHandlerDirtySDK::Tick()
{
	if(mpHttpManager)    
		HttpManagerUpdate(mpHttpManager);
	
	return true;
}

bool TransportHandlerDirtySDK::Transfer(EA::WebKit::TransportInfo* pTInfo, bool& bStateComplete)
{
	bool          bReturnValue = true;
	char          buffer[1024]; 
	int32_t       iResult = 0;
	DirtySDKInfo* pDirtySDKInfo = (DirtySDKInfo*)pTInfo->mTransportHandlerData;

	const double dCurrentTime = ACCESS_EAWEBKIT_API(GetTime());

	if(dCurrentTime > pTInfo->mTimeout)
		bReturnValue = false;

	switch(pTInfo->mHttpRequestType)
	{
		//Note by Arpit Baldeva: GET and POST method response usually would have some data to go with.
	case EA::WebKit::kHttpRequestTypeGET:
	case EA::WebKit::kHttpRequestTypePOST:

	case EA::WebKit::kHttpRequestTypePUT:
	case EA::WebKit::kHttpRequestTypeHEAD:
	case EA::WebKit::kHttpRequestTypeDELETE:
	case EA::WebKit::kHttpRequestTypeOPTIONS:
		{
			while((iResult = HttpManagerRecv(mpHttpManager, pDirtySDKInfo->mHttpHandle, buffer, 1, sizeof(buffer))) > 0)  // While there is received data...
			{
				EAW_ASSERT_MSG(pDirtySDKInfo->mbHeadersReceived,"The headers should have been received and processed by this time through the DirtySDK callback\n");
#if ENABLE_PAYLOAD_DECOMPRESSION
				if(pDirtySDKInfo->mStreamDecompressor)
				{
					if(pDirtySDKInfo->mStreamDecompressor->Decompress((uint8_t*)buffer,iResult)<0)//if there is any error in the processing of stream, error out.
					{
						pTInfo->mpTransportServer->DataDone(pTInfo, false);
						bReturnValue   = false;
						bStateComplete = true;
						return bReturnValue;
					}
				}
				else
#endif
				{
					pTInfo->mpTransportServer->DataReceived(pTInfo, buffer,iResult);
				}
			}
			break;
		}
	case EA::WebKit::kHttpRequestTypeCount:
		EAW_ASSERT_MSG(false, "No supported type");
		break;
	}

	// iResult is one of:
	//   #define  PROTOHTTP_RECVDONE   (-1) // receive operation complete, and all data has been read 
	//   #define  PROTOHTTP_RECVFAIL   (-2) // receive operation failed 
	//   #define  PROTOHTTP_RECVWAIT   (-3) // waiting for body data 
	//   #define  PROTOHTTP_RECVHEAD   (-4) // in headonly mode and header has been received 
	//   #define  PROTOHTTP_RECVBUFF   (-5) // recvall did not have enough space in the provided buffer 


	switch (iResult)
	{
	case PROTOHTTP_RECVDONE:
		//Note by Arpit Baldeva: Old code processed the header here as well. But we should really be processing headers from the DirtySDK callback. 
		//Old comment - "It's possible this will be called if the body size is zero."
		EAW_ASSERT_MSG(pDirtySDKInfo->mbHeadersReceived,"The headers should have been received and processed by this time through the DirtySDK callback\n");
		pTInfo->mpTransportServer->DataDone(pTInfo, true);
		bReturnValue   = true;
		bStateComplete = true;
		break;

	case PROTOHTTP_RECVFAIL:
		{
			pTInfo->mpTransportServer->DataDone(pTInfo, false);
			bReturnValue   = false;
			bStateComplete = true;

			//We would rather not have code structured as follows (multiple if conditions) but in DirtySDK, there is no uniform set of errors. The error interpretation
			//depends on what you are querying. 
			//We don't see this getting bigger so its okay for now.
			EA::WebKit::NetworkErrorInfo info;
			int32_t timeout = HttpManagerStatus(mpHttpManager, pDirtySDKInfo->mHttpHandle, 0x74696d65 /*'time'*/, NULL, 0);
			if(timeout)
			{
				info.mNetworkErrorType = EA::WebKit::kNetworkErrorTimeOut;
				info.mNetworkErrorCode = timeout;
				ACCESS_EAWEBKIT_API(GetViewNotification())->NetworkError(info);
				break;
			}

#if DIRTYVERS >= 0x07060A00
			int32_t essl = HttpManagerStatus(mpHttpManager, pDirtySDKInfo->mHttpHandle, DIRTY_ESSL, NULL, 0);
			if(essl < 0 && pDirtySDKInfo->mURI.find("https") != -1)//DirtySDK triggers the error for even non SSL links. Don't report those errors.
			{
				info.mNetworkErrorType = EA::WebKit::kNetworkErrorSSLCert;
				info.mNetworkErrorCode = essl;
				ACCESS_EAWEBKIT_API(GetViewNotification())->NetworkError(info);
				break;
			}
#endif //DIRTYVERS >= 0x07060A00

			//Default case.
			ACCESS_EAWEBKIT_API(GetViewNotification())->NetworkError(info);
			break;
		}

	case PROTOHTTP_RECVWAIT:
		break;
	case PROTOHTTP_RECVHEAD:
		//Note by Arpit Baldeva: Sticking a DataDone here because DirtySDK explicitly notifies about this return value.
		EAW_ASSERT_MSG(pDirtySDKInfo->mbHeadersReceived,"The headers should have been received and processed by this time through the DirtySDK callback\n");
		pTInfo->mpTransportServer->DataDone(pTInfo, true);
		bReturnValue   = true;
		bStateComplete = true;
	case PROTOHTTP_RECVBUFF:
		break;
	}
#if 0 //Note by Arpit Baldeva: Disable the unused code for http post streaming.	
	else // POST
	{
		if(pDirtySDKInfo->mPostBufferSize == 0)
		{
			EAW_ASSERT(pDirtySDKInfo->mPostBufferPosition == 0);

			pDirtySDKInfo->mPostBufferSize = pTInfo->mpTransportServer->ReadData(pTInfo, pDirtySDKInfo->mPostBuffer, sizeof(pDirtySDKInfo->mPostBuffer));

			if(pDirtySDKInfo->mPostBufferSize == 0) // If there was nothing else to read, and the read was thus completed...
			{
				HttpManagerSend(mpHttpManager, pDirtySDKInfo->mHttpHandle, NULL, PROTOHTTP_STREAM_END);
				pDirtySDKInfo->mbPostActive = false;

			}
			else if(pDirtySDKInfo->mPostBufferSize < 0)
			{
				bReturnValue   = false;
				bStateComplete = true;
			}
			// Else we have POST data, so fall through and send it
		}

		int64_t postSize = (pDirtySDKInfo->mPostBufferSize - pDirtySDKInfo->mPostBufferPosition);

		if(postSize > 0)
		{
			iResult = HttpManagerSend(mpHttpManager, pDirtySDKInfo->mHttpHandle, pDirtySDKInfo->mPostBuffer + pDirtySDKInfo->mPostBufferPosition, (int32_t)postSize);

			if(iResult > 0)
			{
				pDirtySDKInfo->mPostBufferPosition += iResult;;

				if(pDirtySDKInfo->mPostBufferPosition == pDirtySDKInfo->mPostBufferSize)
				{
					pDirtySDKInfo->mPostBufferPosition = 0;
					pDirtySDKInfo->mPostBufferSize     = 0;
				}
			}
			else if(iResult < 0)
			{
				bReturnValue   = false;
				bStateComplete = true;
			}
			// else do nothing because nothing was sent.
		}
	}
#endif //if 0

	return bReturnValue;
}
bool TransportHandlerDirtySDK::CanCacheToDisk()
{
    return true;
}

} // namespace WebKit
} // namespace EA

#endif // #if USE(DIRTYSDK)
