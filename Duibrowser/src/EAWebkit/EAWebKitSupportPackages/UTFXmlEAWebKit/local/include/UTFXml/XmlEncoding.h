/*
Copyright (C) 2009-2010 Electronic Arts, Inc.  All rights reserved.

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
// XMLEncoding.h
//
// Copyright (c) 2003 Electronic Arts Inc.
// Created by: Paul Pedriana
///////////////////////////////////////////////////////////////////////////////

#ifndef UTFXML_XMLENCODING_H
#define UTFXML_XMLENCODING_H

#include <EABase/eabase.h>
#include <UTFXml/internal/Config.h>

namespace EA {

    namespace IO {
        class IStream;
    }

    namespace XML {
        //////////////////////////////////////////////////////////////////////////
        /// Character encodings for XmlReader
        enum ReadEncoding {
            kReadEncodingUnknown,   /// The XmlReader will attempt to auto-detect encodings of this type. It's best if such files have leading encoding bytes.
            kReadEncodingUTF8,
            kReadEncodingUTF16LE,
            kReadEncodingUTF16BE,
          #ifdef EA_SYSTEM_LITTLE_ENDIAN
            kReadEncodingUTF16 = kReadEncodingUTF16LE,
          #else
            kReadEncodingUTF16 = kReadEncodingUTF16BE,
          #endif
        };

        //////////////////////////////////////////////////////////////////////////
        /// Character encodings for XmlWriter
        enum WriteEncoding 
        {
            kWriteEncodingUnknown,
            kWriteEncodingUTF8,
            kWriteEncodingUTF16,
            kWriteEncodingLatin1,
            kWriteEncodingASCII
        };

        //////////////////////////////////////////////////////////////////////////
        /// Defines 
        enum LineEnd
        {
            kLineEndUnix,       // \n
            kLineEndWindows,    // \r\n
            kLineEndDefault = kLineEndWindows
        };
      

        UTFXML_API 
        bool WriteEncoded(EA::IO::IStream* pDestination, const char8_t* pSourceText, 
                          size_t nSourceWriteCount, WriteEncoding srcEncoding, WriteEncoding destEncoding);

        UTFXML_API 
        bool WriteEncoded(EA::IO::IStream* pDestination, const char16_t* pSourceText, 
                          size_t nSourceWriteCount, WriteEncoding destEncoding);

        UTFXML_API 
        size_t ConvertEncoding(const void* pTextInput,  size_t  nLengthInput,  WriteEncoding nEncodingInput,
                                     void* pTextOutput, size_t& nLengthOutput, WriteEncoding nEncodingOutput);

        UTFXML_API
        int Strlcpy(char8_t* pDest, const char16_t* pSource, size_t nDestCapacity, size_t nSourceLength);


        UTFXML_API
        int Strlcpy(char16_t* pDest, const char8_t* pSource, size_t nDestCapacity, size_t nSourceLength);

    } // namespace XML

} // namespace EA


#endif // Header include guard





