/*
Copyright (C) 2010 Electronic Arts, Inc.  All rights reserved.

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

///////////////////////////////////////////////////////////////////////////////
// EAWebKitUtilStreamDecompressor.h
//
// Copyright (c) 2010 Electronic Arts. All Rights Reserved.
// By Arpit Baldeva 
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// This file (and cpp implementation) define an abstraction for the stream decompression. It also implements a decompressor that is capable of 
// decompressing a stream compressed using "DEFLATE" algorithm. 

// All 3 common decompression specifications are supported.
// ZLib			-	http://tools.ietf.org/html/rfc1950 (Content-Encoding: deflate)
// Raw Deflate	-	http://tools.ietf.org/html/rfc1951 (Content-Encoding: deflate)
// gzip			-	http://tools.ietf.org/html/rfc1952 (Content-Encoding: gzip)
///////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKITUTIL_STREAM_DECOMPRESSOR_H
#define EAWEBKITUTIL_STREAM_DECOMPRESSOR_H

#ifndef BUILDING_EAWEBKIT_DLL
#include <EAWebKitUtil/EAWebKitUtilConfig.h>
#endif //

#if (ENABLE_PAYLOAD_DECOMPRESSION)
#include <EABase/eabase.h>
#include "PackageRedirector.h"

namespace EA
{
	namespace EAWEBKIT_PACKAGE_NAMESPACE
	{
		//This is the callback for the application when data is decompressed. 
		typedef void (*DecompressedDataCallback)(void* userData, uint8_t* decompressedData, uint32_t decompressedDataLength);
		
		class IStreamDecompressor
		{
		public:
			IStreamDecompressor(){}
			virtual ~IStreamDecompressor();

			//Return value indicates the status. Need to have proper error codes.
			//For now, passing the zlib error code would be sufficient as it is the only library that we use.
			virtual int32_t Init();
			
			//A positive return value indicates the amount of bytes decompressed in the buffer.
			//A negative return value indicates an error.
			//If no decompression buffer is passed, the implementation should allocate one as required.
			virtual int32_t Decompress(uint8_t* sourceStream, uint32_t sourceLength, uint8_t* decompressionBuffer = 0, uint32_t decompressionBufferCapacity = 0) = 0;
			//Set a callback to receive the data when decompressed.
			virtual void	SetDecompressedDataCallback(DecompressedDataCallback callback, void* userData){};
		private:
			IStreamDecompressor(const IStreamDecompressor& streamDecompressor);
			IStreamDecompressor& operator=(const IStreamDecompressor& streamDecompressor);
		};
	}
}

// Our implementation here cheats a bit as per our requirement. For example, we don't copy the data in the decompression buffer in the DeCompress(). We
// rely on the callback so that the application can consume the decompressed data when available. If the decompression buffer is supplied by the application,
// we'll use it.Relying on callback is better so that the application can consume the data when it is available without having to worry about "decompression
// buffer large enough" for the input data and no dependency is introduced to the rest of the application.


typedef struct z_stream_s z_stream;

namespace EA
{
	namespace EAWEBKIT_PACKAGE_NAMESPACE
	{
		enum eStreamType
		{
			eStreamTypeNone,
			eStreamTypeZLib,
			eStreamTypeRaw,
			eStreamTypeGZip
		};
		
		//This stream decompressor is able to decompress a stream compressed using "DEFLATE" algorithm. It can be any of the three common streams
		//used in Http 1.1. 
		class DeflateStreamDecompressor : public IStreamDecompressor
		{
		public:
			DeflateStreamDecompressor(EA::EAWEBKIT_PACKAGE_NAMESPACE::eStreamType streamType);
			virtual			~DeflateStreamDecompressor();
			virtual int32_t	Init();
			
			//If no decompression buffer is passed, the implementation would allocate one. This would be kept alive as long as it is suitable.
			//For now, not passing the decompression buffer would provide better memory usage.
			virtual int32_t Decompress(uint8_t* sourceStream, uint32_t sourceLength, uint8_t* decompressionBuffer = 0, uint32_t decompressionBufferCapacity = 0);

			virtual void	SetDecompressedDataCallback(DecompressedDataCallback callback, void* userData);

		private:
			void UnInit();
			void AllocateDecompressionBuffer( uint32_t sourceLength );
			int32_t ProcessGZipHeader(uint8_t*& sourceStream, uint32_t& sourceLength);
			
			z_stream*										mZStream;
			DecompressedDataCallback						mDecompressedDataCallback;
			uint8_t*										mDecompressionBuffer;
			size_t											mDecompressionBufferCapacity;
			EA::EAWEBKIT_PACKAGE_NAMESPACE::eStreamType		mStreamType;
			void*											mUserData;
			bool											mInitialized;
			bool											mRawDeflateStream;
			bool											mCheckForErrorCondition;
		};

	}
}
#endif //ENABLE_PAYLOAD_DECOMPRESSION
#endif //EAWEBKITUTIL_STREAM_DECOMPRESSOR_H