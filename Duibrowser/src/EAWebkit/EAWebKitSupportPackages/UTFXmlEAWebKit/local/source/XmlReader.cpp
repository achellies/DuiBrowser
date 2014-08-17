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

/////////////////////////////////////////////////////////////////////////////
// Name: XmlReader.cpp
//
// Purpose: Classes for reading in hierarchical data structures from XML files
//
// Copyright (c) 2006 Electronic Arts Inc.
// Created by: Talin and Paul Pedriana
/////////////////////////////////////////////////////////////////////////////

#include <UTFXml/internal/Config.h>
#include <UTFXml/XmlReader.h>
#include <EAIO/EAStream.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <stdio.h>
#include EA_ASSERT_HEADER


namespace EA {
namespace XML {

namespace {
    //////////////////////////////////////////////////////////////////////////
    // Create a copy of a string given an allocator
    const char *StrDup( EA::Allocator::ICoreAllocator * alloc, const char * str ) {
        size_t const nLength = (strlen( str ) + 1);
        char * const pResult = (char *)alloc->Alloc( nLength, UTFXML_ALLOC_PREFIX "XmlReader/StrDup", 0 );
        if (pResult)
            memcpy( pResult, str, nLength );
        return pResult;
    }

    //////////////////////////////////////////////////////////////////////////
    /// Include the character class table
    #include <UTFXml/XmlCharClasses.inl>

    static const int32_t kMaxInputDepth = 16;

    static const int32_t kLeadingSurrogateStart  = 0x0000d800;
  //static const int32_t kLeadingSurrogateEnd    = 0x0000dbff;
    static const int32_t kTrailingSurrogateStart = 0x0000dc00;
    static const int32_t kTrailingSurrogateEnd   = 0x0000dfff;
    static const int32_t kSurrogateOffset        = 0x00010000 - (kLeadingSurrogateStart << 10) - kTrailingSurrogateStart;

    //////////////////////////////////////////////////////////////////////////
    static uint8_t utf8lengthTable[ 256 ] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    //////////////////////////////////////////////////////////////////////////
    /// Used to subtract out the control bits in multi-byte sequence.
    static const int utf8DecodingOffsetTable[ 5 ] = {
        0,                                                  // 0x00000000
        0,                                                  // 0x00000000
        (0xC0 << 6) + 0x80,                                 // 0x00003080
        (0xE0 << 12) + (0x80 << 6) + 0x80,                  // 0x000e2080
        (0xF0 << 18) + (0x80 << 12) + (0x80 << 6) + 0x80    // 0x03c82080
    };

    //////////////////////////////////////////////////////////////////////////
    // The minimum value that can be encoded in a particular number
    // of bytes. Used to disallow non-canonical encoded sequences.
    // It turns out that this is not a fully proper way to check for 
    // valid sequences. See the Unicode Standard http://unicode.org/versions/corrigendum1.html
    static const int utf8MinimumValueTable[ 5 ] = {
        0x00000000,     // This slot is unused
        0x00000000,     // 1 byte per char
        0x00000080,     // 2 bytes per char
        0x00000800,     // 3 bytes per char
        0x00010000      // 4 bytes per char
    };

    //////////////////////////////////////////////////////////////////////////
    // One past the maximum value that can be encoded in a particular number
    // of bytes. Used to disallow non-canonical encoded sequences.
    static const int utf8MaximumValueTable[ 5 ] = {
        0x00000000,     // This slot is unused
        0x00000080,     // 1 byte per char
        0x00000800,     // 2 bytes per char
        0x00010000,     // 3 bytes per char
        0x00110000      // 4 bytes per char
    };


    //////////////////////////////////////////////////////////////////////////
    static const char utf8ByteMark[]    = { 0xEFu, 0xBBu, 0xBFu };
    static const char utf16BEByteMark[] = { 0xFEu, 0xFFu };
    static const char utf16LEByteMark[] = { 0xFFu, 0xFEu };

    static const char utf8Prolog[]    = { 0x3C, 0x3F, 0x78, 0x6D };
    static const char utf16BEProlog[] = { 0x00, 0x3C, 0x00, 0x3F };
    static const char utf16LEProlog[] = { 0x3c, 0x00, 0x3F, 0x00 };

    static const char utf16BEFragment[] = { 0x00, 0x3C };
    static const char utf16LEFragment[] = { 0x3c, 0x00 };

} // namespace 


UTFXML_API int Strlcpy(char8_t* pDest, const char16_t* pSource, size_t nDestCapacity, size_t nSourceLength)
{
    size_t destCount = 0;

    while(nSourceLength-- > 0)
    {
        uint32_t c = *pSource++;   // Deal with surrogate characters

        // Encode as UTF-8
        if (c < 0x00000080u)
        {
            if(c == 0) // Break on NULL char, even if explicit length was set
                break;

            if(pDest && ((destCount + 1) < nDestCapacity))
                *pDest++ = static_cast<char8_t>(c);

            destCount += 1;
        }
        else if(c < 0x00000800u)
        {
            if(pDest && ((destCount + 2) < nDestCapacity))
            {
                *pDest++ = static_cast<char8_t>((c >> 6) | 0xc0);
                *pDest++ = static_cast<char8_t>((c | 0x80) & 0xbf);
            }

            destCount += 2;
        }
        else if(c < 0x00010000u)
        {
            if(pDest && ((destCount + 3) < nDestCapacity))
            {
                *pDest++ = static_cast<char8_t>((c >> 12) | 0xe0);
                *pDest++ = static_cast<char8_t>(((c >>  6) | 0x80) & 0xbf);
                *pDest++ = static_cast<char8_t>((c | 0x80) & 0xbf);
            }

            destCount += 3;
        }
        else if(c < 0x00200000u)
        {
            if(pDest && ((destCount + 4) < nDestCapacity))
            {
                *pDest++ = static_cast<char8_t>((c >> 18) | 0xf0);
                *pDest++ = static_cast<char8_t>(((c >> 12) | 0x80) & 0xbf);
                *pDest++ = static_cast<char8_t>(((c >>  6) | 0x80) & 0xbf);
                *pDest++ = static_cast<char8_t>((c | 0x80) & 0xbf);
            }

            destCount += 4;
        }
        else
        {
            const uint32_t kIllegalUnicodeChar = 0x0000fffd;

            if(pDest && ((destCount + 3) < nDestCapacity))
            {
                *pDest++ = static_cast<char8_t>( (kIllegalUnicodeChar >> 12) | 0xe0);
                *pDest++ = static_cast<char8_t>(((kIllegalUnicodeChar >>  6) | 0x80) & 0xbf);
                *pDest++ = static_cast<char8_t>(((kIllegalUnicodeChar >>  0) | 0x80) & 0xbf);
            }

            destCount += 3;
        }
    }

    if(pDest && nDestCapacity != 0)
        *pDest = 0;

    return (int)(unsigned)destCount;
}


UTFXML_API int Strlcpy(char16_t* pDest, const char8_t* pSource, size_t nDestCapacity, size_t nSourceLength)
{
    size_t destCount = 0;

    while(nSourceLength-- > 0)
    {
        uint32_t c = (uint8_t)*pSource++;

        if(c < 128)
        {
            if(c == 0) // Break on NULL char, even if explicit length was set
                break;

            if(pDest && ((destCount + 1) < nDestCapacity))
                *pDest++ = static_cast<char16_t>(c);

            destCount++;
        }
        else
        {
            uint32_t nLength = utf8lengthTable[c]; // nLength may be zero, in which case we'll return 'IncorrectEncoding'.

            // Do we have an incomplete string?
            if((nLength > (nSourceLength + 1)) || (nLength == 0))
            {
                EA_FAIL_MSG("Incomplete Unicode character in buffer");
                return -1;
            }

            // Now decode the remaining ("following") bytes.
            for(uint32_t i = 0; i < nLength - 1; ++i) 
            {
                uint8_t nByte = (uint8_t)*pSource++;

                if((nByte < 0x80u) || (nByte > 0xbfu))   // Syntax check
                {
                    EA_FAIL_MSG("Invalid following byte");
                    return -1;
                }

                c = (c << 6) + nByte;   // Preserve control bits (don't OR)
            }

            nSourceLength -= nLength;                 // We've just processed all remaining bytes for this multi-byte character
            c -= utf8DecodingOffsetTable[nLength];    // Subtract accumulated control bits just once

            // Check for canonical encoding.
            if((c >= (uint32_t)utf8MinimumValueTable[nLength]) && (c < (uint32_t)utf8MaximumValueTable[nLength]))
            {
                if(pDest && ((destCount + 1) < nDestCapacity))
                    *pDest++ = static_cast<char16_t>(c);

                destCount++;
            }
            else
                break;
        }
    }

    if(pDest && (nDestCapacity != 0))
        *pDest = 0;

    return (int)(unsigned)destCount;
}



//////////////////////////////////////////////////////////////////////////
// Fill the buffer for this input stream.
XmlReader::ssize_t XmlReader::InputStream::FillBuffer() {
    mpInputBufferEnd = mpInputBufferPos = mpInputBufferStart;
    const ssize_t actual = ssize_t( mpStreamPtr->Read( mpInputBufferStart, EA::IO::size_type(mpInputBufferCapacity - mpInputBufferStart) ));

    // If no error occurred, then set the length of the buffer based on the number of bytes read.
    if (actual >= 0)
        mpInputBufferEnd = mpInputBufferStart + actual;

    return actual;
}


//////////////////////////////////////////////////////////////////////////
int32_t XmlReader::InputStream::ReadCharUTF8() {

    // Test the most common case, which will be true more than 99% of the time.
    if ((mpInputBufferPos < mpInputBufferEnd) && (*mpInputBufferPos < 128))
        return *mpInputBufferPos++;

    // Check if we need to fill the buffer
    if (EA_UNLIKELY( mpInputBufferPos >= mpInputBufferEnd )) {
        // Refill the input buffer if we can
        if (mpStreamPtr == NULL || FillBuffer() <= 0)
            return EoF;
    }

    uint8_t nByte = *mpInputBufferPos++;
    if (nByte < 128) // Early out for single byte
        return nByte;

    const int nLength = utf8lengthTable[ nByte ]; // nLength may be zero, in which case we'll return 'IncorrectEncoding'.
    int32_t nChar = nByte;

    // Now decode the remaining bytes.
    for (int i = nLength - 1; i > 0; --i) {
        if (EA_UNLIKELY( mpInputBufferPos >= mpInputBufferEnd )) {
            // Refill the input buffer if we can
            if ((mpStreamPtr == NULL) || (FillBuffer() <= 0))
                return PartialChar;  // Partial character error
        }

        nByte = *mpInputBufferPos++;

        if ((nByte < 0x80u) || (nByte > 0xbfu))   // Syntax check
            return IncorrectEncoding;

        nChar = (nChar << 6) + nByte;
    }

    nChar -= utf8DecodingOffsetTable[ nLength ];

    // Check for canonical encoding.
    if ((nChar >= utf8MinimumValueTable[ nLength ]) && (nChar < utf8MaximumValueTable[ nLength ]))
        return nChar;

    return IncorrectEncoding;
}


//////////////////////////////////////////////////////////////////////////
int32_t XmlReader::InputStream::ReadCharUTF16BE() {
    // Check if we need to fill the buffer
    if (EA_UNLIKELY( mpInputBufferPos >= mpInputBufferEnd )) {
        // Refill the input buffer if we can
        if (mpStreamPtr == NULL || FillBuffer() <= 0) {
            return EoF;
        }
    }

    int32_t nChar = *mpInputBufferPos++;

    // Check if we need to fill the buffer for the next byte
    if (EA_UNLIKELY( mpInputBufferPos >= mpInputBufferEnd )) {
        // Refill the input buffer if we can
        if (mpStreamPtr == NULL || FillBuffer() <= 0) {
            return PartialChar;
        }
    }

    nChar = (nChar << 8) | *mpInputBufferPos++;

    // Early out for non-surrogates
    if (nChar < kLeadingSurrogateStart || nChar > kTrailingSurrogateEnd)
        return nChar;

    // Trailing surrogates now allowed by themselves
    if (nChar >= kTrailingSurrogateStart)
        return IncorrectEncoding;

    // Check if we need to fill the buffer for the next byte
    if (EA_UNLIKELY( mpInputBufferPos >= mpInputBufferEnd )) {
        // Refill the input buffer if we can
        if (mpStreamPtr == NULL || FillBuffer() <= 0) {
            return PartialChar;
        }
    }

    int32_t nTrail = *mpInputBufferPos++;

    // Check if we need to fill the buffer for the next byte
    if (EA_UNLIKELY( mpInputBufferPos >= mpInputBufferEnd )) {
        // Refill the input buffer if we can
        if (mpStreamPtr == NULL || FillBuffer() <= 0) {
            return PartialChar;
        }
    }

    nTrail = (nTrail << 8) | *mpInputBufferPos++;
    nChar = (nChar << 10) + nTrail + kSurrogateOffset;
    if (nChar > 0x10ffff)
        return IncorrectEncoding; // Illegal byte sequence

    return nChar;
}

//////////////////////////////////////////////////////////////////////////
int32_t XmlReader::InputStream::ReadCharUTF16LE() {
    // Check if we need to fill the buffer
    if (EA_UNLIKELY( mpInputBufferPos >= mpInputBufferEnd )) {
        // Refill the input buffer if we can
        if (mpStreamPtr == NULL || FillBuffer() <= 0) {
            return EoF;
        }
    }

    int32_t nChar = *mpInputBufferPos++;

    // Check if we need to fill the buffer for the next byte
    if (EA_UNLIKELY( mpInputBufferPos >= mpInputBufferEnd )) {
        // Refill the input buffer if we can
        if (mpStreamPtr == NULL || FillBuffer() <= 0) {
            return PartialChar;
        }
    }

    nChar = nChar | (*mpInputBufferPos++ << 8);

    // Early out for non-surrogates
    if (nChar < kLeadingSurrogateStart || nChar > kTrailingSurrogateEnd)
        return nChar;

    // Trailing surrogates now allowed by themselves
    if (nChar >= kTrailingSurrogateStart)
        return IncorrectEncoding;

    // Check if we need to fill the buffer for the next byte
    if (EA_UNLIKELY( mpInputBufferPos >= mpInputBufferEnd )) {
        // Refill the input buffer if we can
        if (mpStreamPtr == NULL || FillBuffer() <= 0) {
            return PartialChar;
        }
    }

    int32_t nTrail = *mpInputBufferPos++;

    // Check if we need to fill the buffer for the next byte
    if (EA_UNLIKELY( mpInputBufferPos >= mpInputBufferEnd )) {
        // Refill the input buffer if we can
        if (mpStreamPtr == NULL || FillBuffer() <= 0) {
            return PartialChar;
        }
    }

    nTrail = nTrail | (*mpInputBufferPos++ << 8);
    nChar = (nChar << 10) + nTrail + kSurrogateOffset;
    if (nChar > 0x0010ffff)
        return IncorrectEncoding; // Illegal byte sequence

    return nChar;
}

//////////////////////////////////////////////////////////////////////////
int32_t XmlReader::InputStream::ReadChar() {
    for (;;) {
        // Track how many bytes were read by mpDecoderFunc so that we can advance mByteIndex by that amount.
        const uint8_t* const pSavedPos = mpInputBufferPos;
        const int32_t nChar = (this->*mpDecoderFunc)();
        mByteIndex += (ssize_t)(mpInputBufferPos - pSavedPos);
        
        if (nChar != '\n') {
            if (nChar != '\r') {
                // We have a normal character; just update column index and return the character.
                mLineIndex = mNextLineIndex;
                mColumnIndex = mNextColumnIndex++;
                mPrevChar = nChar;
                return nChar;
            }

            // We have \r; do a line break and return \n instead of \r.
            mColumnIndex = mNextColumnIndex;
            mLineIndex = mNextLineIndex++;
            mNextColumnIndex = 0;
            mPrevChar = nChar;
            return '\n';
        }

        // If this is a \r\n pair, then the \r has already
        // done a line break (above), so just ignore this \n char.
        if (mPrevChar != '\r') { // If we have a lone \n char...
            // Otherwise, it's a \n by itself, in which case
            // we return a \n character and do line break.
            mColumnIndex = mNextColumnIndex;
            mLineIndex = mNextLineIndex++;
            mNextColumnIndex = 0;
            mPrevChar = '\n';
            return '\n';
        }

        // mPrevChar is \r; change it to \n.
        mPrevChar = '\n';
    }
}

//////////////////////////////////////////////////////////////////////////
XmlReader::XmlReader( Allocator::ICoreAllocator *pAllocator, size_t bufferBlockSize )
    : mpCoreAllocator( pAllocator ? pAllocator : Allocator::ICoreAllocator::GetDefaultAllocator() )
    , mFlags( kFlagsDefault )
    , mResultCode( kSuccess )
    , mElementDepth( 0 )
    , mInputDepth( 0 )
    , mpInputStack( NULL )
    , mpTopNamedStream( NULL )
    , mpNamespaceAliases( NULL )
    , mpNamespaceDecls( NULL )
    , mDefaultNS( NULL )
    , mElementNS( NULL )
    //mBuiltInEntities
    , mEntityDecls( CoreAllocatorAdapter( UTFXML_ALLOC_PREFIX "XmlReader/EntityDecls", mpCoreAllocator ) )
    , mpEntityResolver( NULL )
    , mpEntityResolverContext( NULL )
    , mChar( 0 )
    , mState( State_Undecided )
    , mNodeType( None )
    , mIsEmptyElement( false )
    , mIsCData( false )
    , mIsStandalone( true )
    , mpTokenName( NULL )
    , mpTokenValue( NULL )
    , mValueLength( 0 )
    , mpPendingEntityRef( NULL )
    , mTokenBuffer( mpCoreAllocator, bufferBlockSize )
    , mAttributeArray( CoreAllocatorAdapter( UTFXML_ALLOC_PREFIX "XmlReader/Attributes", mpCoreAllocator ) )
    , mpDocTypeName( NULL )
    , mpSystemId( NULL )
    , mpPublicId( NULL )
    , mLineIndex( 0 )
    , mColumnIndex( 0 )
    , mByteIndex( 0 )
{
    Init();
}

//////////////////////////////////////////////////////////////////////////
void XmlReader::Init()
{
    mAttributeArray.reserve( 16 );

    // Built-in entity declarations
    // We intentionally don't do the following:
    //    mEntityDecls["amp"] = "&";
    // The reason why is that user-defined entities also use mEntityDecls (via DefineEntity) and the 
    // strings for it are allocated by Strdup. So the strings added to mEntityDecls by the user need
    // to be freed in our Reset function. But the string literals defined below need to not be freed.
    // So in Reset we free any strings that aren't our constant strings. We are stuck being unable
    // to use Strdup here for our built-in types because Strdup requires that we have an allocator 
    // which might not be set by the user until after Init is called. 
    
    mBuiltInEntities[0] = "amp";
    mBuiltInEntities[1] = "&";

    mBuiltInEntities[2] = "apos";
    mBuiltInEntities[3] = "'";

    mBuiltInEntities[4] = "quot";
    mBuiltInEntities[5] = "\"";

    mBuiltInEntities[6] = "lt";
    mBuiltInEntities[7] = "<";

    mBuiltInEntities[8] = "gt";
    mBuiltInEntities[9] = ">";
 
    for(int i = 0; i < 10; i += 2)
        mEntityDecls[mBuiltInEntities[i]]  = mBuiltInEntities[i + 1];
}


//////////////////////////////////////////////////////////////////////////
XmlReader::~XmlReader() {
    Reset();
}


//////////////////////////////////////////////////////////////////////////
void XmlReader::Reset() {
    // mpCoreAllocator;     // Retain its current value.
    // mFlags;              // Retain its current value.

    mResultCode   = kSuccess;
    mElementDepth = 0;
    mInputDepth   = 0;

    while (mpInputStack)
        PopInputStream();

    mpTopNamedStream = NULL;

    while (mpNamespaceDecls)
        PopNamespace();

    mDefaultNS = NULL;
    mElementNS = NULL;

    while (mpNamespaceAliases) {
        NSAlias * const pAlias = mpNamespaceAliases;
        mpNamespaceAliases = pAlias->mpNext;
        if (pAlias->mOwnStrings) {
            mpCoreAllocator->Free( (void *)pAlias->mpNamespace );
            mpCoreAllocator->Free( (void *)pAlias->mpAlias );
        }
        mpCoreAllocator->Free( pAlias );
    }

    // We need to free any user-defined strings in mEntityDecls.
    for(tEntityMap::iterator it = mEntityDecls.begin(); it != mEntityDecls.end(); ++it)
    {
        const char*  pKey        = it->first;
        const char*  pValue      = it->second;
        const size_t kEntryCount = sizeof(mBuiltInEntities) / sizeof(mBuiltInEntities[0]);

        if(eastl::find(mBuiltInEntities, mBuiltInEntities + kEntryCount, pKey) == mBuiltInEntities + kEntryCount) // If not in our mBuiltInEntityList...
            mpCoreAllocator->Free((void*)pKey);

        if(eastl::find(mBuiltInEntities, mBuiltInEntities + kEntryCount, pValue) == mBuiltInEntities + kEntryCount) // If not in our mBuiltInEntityList...
            mpCoreAllocator->Free((void*)pValue);
    }

    mEntityDecls.clear();

    // mpEntityResolver;            // Retain its current value.
    // mpEntityResolverContext;     // Retain its current value.

    mChar              = 0;
    mState             = State_Undecided;
    mNodeType          = None;
    mIsEmptyElement    = false;
  //mIsCData           = false;  // We don't need to do this, because the value is always set anew when the node type is set to CharacterData.
    mIsStandalone      = true;
    mpTokenName        = NULL;
    mpTokenValue       = NULL;
    mValueLength       = 0;
    mpPendingEntityRef = NULL;
    mTokenBuffer.Clear();
    mAttributeArray.clear();
    mpDocTypeName = NULL;
    mpSystemId    = NULL;
    mpPublicId    = NULL;
    mLineIndex    = 1;
    mColumnIndex  = 0;
    mByteIndex    = 0;

    Init();
}

//////////////////////////////////////////////////////////////////////////
ReadEncoding XmlReader::DetectEncoding( uint8_t *& pBuffer, const uint8_t * pBufferEnd ) {
    const size_t bufferSize = (size_t)(pBufferEnd - pBuffer);

    // Detect UTF-8 byte mark
    if ((bufferSize >= sizeof( utf8ByteMark )) && memcmp( pBuffer, utf8ByteMark, sizeof( utf8ByteMark ) ) == 0) {
        pBuffer += sizeof( utf8ByteMark );
        return kReadEncodingUTF8;
    }

    // Detect UTF-8 XML prologue
    if ((bufferSize >= sizeof( utf8Prolog )) && memcmp( pBuffer, utf8Prolog, sizeof( utf8Prolog ) ) == 0) {
        return kReadEncodingUTF8;
    }

    // Detect UTF-16 byte mark (Big-endian)
    if ((bufferSize >= sizeof( utf16BEByteMark )) && memcmp( pBuffer, utf16BEByteMark, sizeof( utf16BEByteMark ) ) == 0) {
        pBuffer += sizeof( utf16BEByteMark );
        return kReadEncodingUTF16BE;
    }

    // Detect UTF-16 XML prologue (big-endian)
    if ((bufferSize >= sizeof( utf16BEProlog )) && memcmp( pBuffer, utf16BEProlog, sizeof( utf16BEProlog ) ) == 0) {
        return kReadEncodingUTF16BE;
    }

    // Detect UTF-16 byte mark (Little-endian)
    if ((bufferSize >= sizeof( utf16LEByteMark )) && memcmp( pBuffer, utf16LEByteMark, sizeof( utf16LEByteMark ) ) == 0) {
        pBuffer += sizeof( utf16LEByteMark );
        return kReadEncodingUTF16LE;
    }

    // Detect UTF-16 XML prologue (Little-endian)
    if ((bufferSize >= sizeof( utf16LEProlog )) && memcmp( pBuffer, utf16LEProlog, sizeof( utf16LEProlog ) ) == 0) {
        return kReadEncodingUTF16LE;
    }

    // More guessing

    // Detect UTF-16 XML prologue (big-endian)
    if ((bufferSize >= sizeof( utf16BEFragment )) && memcmp( pBuffer, utf16BEFragment, sizeof( utf16BEFragment ) ) == 0) {
        return kReadEncodingUTF16BE;
    }

    // Detect UTF-16 XML prologue (Little-endian)
    if ((bufferSize >= sizeof( utf16LEFragment )) && memcmp( pBuffer, utf16LEFragment, sizeof( utf16LEFragment ) ) == 0) {
        return kReadEncodingUTF16LE;
    }

    // If the first few characters don't have the high byte set, it's probably an 8-bit stream anyway.
    if ((bufferSize >= 3) && (pBuffer[ 0 ] < 128) && (pBuffer[ 1 ] < 128) && (pBuffer[ 2 ] < 128))
        return kReadEncodingUTF8;

    return kReadEncodingUnknown;
}

//////////////////////////////////////////////////////////////////////////
void XmlReader::PushInputStream(
    EA::IO::IStream * pStream,
    ReadEncoding encoding,
    const char * pStreamURI,
    ssize_t nBufferSize )
{
    if (mInputDepth >= kMaxInputDepth) {
        Fatal( kErrorRecursionDepthExceeded );
        return;
    }

    if (nBufferSize <= 0)
        nBufferSize = (ssize_t)mTokenBuffer.GetBlockSize();

    InputStream * const istream = (InputStream *)mpCoreAllocator->Alloc( sizeof( InputStream ), UTFXML_ALLOC_PREFIX "XmlReader/InputStream", 0 );
    if (istream == NULL) {
        Fatal( kErrorNoMemory );
        return;
    }
    istream->mpStreamPtr = pStream;
    istream->mpStreamPtr->AddRef();
    istream->mEncoding = encoding;
    istream->mpInputBufferStart = (uint8_t *)mpCoreAllocator->Alloc( size_t(nBufferSize), UTFXML_ALLOC_PREFIX "XmlReader/InputStream/Buffer", 0 );
    if (istream->mpInputBufferStart == NULL) {
        Fatal( kErrorNoMemory );
        return;
    }
    istream->mOwnBuffer = true;
    istream->mpInputBufferCapacity = istream->mpInputBufferStart + nBufferSize;
    istream->mpInputBufferEnd = istream->mpInputBufferStart;
    istream->mpInputBufferPos = istream->mpInputBufferStart;

    istream->mpStreamURI = pStreamURI;
    istream->mPrevChar = 0;
    istream->mLineIndex = 0;
    istream->mColumnIndex = 0;
    istream->mNextLineIndex = 0;
    istream->mNextColumnIndex = 0;
    istream->mByteIndex = -1;

    istream->FillBuffer();

    // Auto-detect encoding logic
    if (istream->mEncoding == kReadEncodingUnknown) {
        istream->mEncoding = DetectEncoding( istream->mpInputBufferPos, istream->mpInputBufferEnd );
        if (istream->mEncoding == kReadEncodingUnknown) {
            Fatal( kErrorUnknownEncoding );
            return;
        }
    }

    switch (istream->mEncoding) {
        case kReadEncodingUTF8:    istream->mpDecoderFunc = &InputStream::ReadCharUTF8; break;
        case kReadEncodingUTF16BE: istream->mpDecoderFunc = &InputStream::ReadCharUTF16BE; break;
        case kReadEncodingUTF16LE: istream->mpDecoderFunc = &InputStream::ReadCharUTF16LE; break;
        default:
            Fatal( kErrorUnknownEncoding );
            return;
    }

    istream->mpNext = mpInputStack;
    mpInputStack = istream;
    if (istream->mpStreamURI || mpTopNamedStream == NULL)
        mpTopNamedStream = istream;
    mInputDepth++;

    ReadChar();
}


//////////////////////////////////////////////////////////////////////////
XmlReader::InputStream* XmlReader::CreateStreamFromBuffer(
    const void * pBuffer,
    size_t nBufferLen,
    ReadEncoding encoding,
    const char * pStreamURI,
    bool bCopy,
    ReadEncoding defaultEncoding
    )
{
    InputStream * const istream = (InputStream *)mpCoreAllocator->Alloc( sizeof( InputStream ), UTFXML_ALLOC_PREFIX "XmlReader/InputStream", 0 );

    if(istream)
    {
        istream->mpStreamPtr = NULL;
        istream->mEncoding = encoding;

        if (bCopy) {
            istream->mpInputBufferStart = (uint8_t *)mpCoreAllocator->Alloc( nBufferLen, UTFXML_ALLOC_PREFIX "XmlReader/InputStream/Buffer", 0 );
            memcpy( istream->mpInputBufferStart, pBuffer, nBufferLen );
        }
        else
            istream->mpInputBufferStart = (uint8_t *)pBuffer;

        istream->mpInputBufferEnd = istream->mpInputBufferStart + nBufferLen;
        istream->mpInputBufferCapacity = istream->mpInputBufferEnd;
        istream->mpInputBufferPos = istream->mpInputBufferStart;
        istream->mOwnBuffer = bCopy;
        istream->mpNext = NULL;
        istream->mpStreamURI = pStreamURI;
        istream->mPrevChar = 0;
        istream->mLineIndex = 0;
        istream->mColumnIndex = 0;
        istream->mNextLineIndex = 0;
        istream->mNextColumnIndex = 0;
        istream->mByteIndex = -1;

        // Auto-detect encoding logic
        if (istream->mEncoding == kReadEncodingUnknown) {
            istream->mEncoding = DetectEncoding( istream->mpInputBufferPos, istream->mpInputBufferEnd );

            if (istream->mEncoding == kReadEncodingUnknown)
                istream->mEncoding = defaultEncoding;
        }

        switch (istream->mEncoding) {
            case kReadEncodingUTF8:
                istream->mpDecoderFunc = &InputStream::ReadCharUTF8;
                break;
            case kReadEncodingUTF16BE:
                istream->mpDecoderFunc = &InputStream::ReadCharUTF16BE;
                break;
            case kReadEncodingUTF16LE:
                istream->mpDecoderFunc = &InputStream::ReadCharUTF16LE;
                break;
            default:
                Fatal( kErrorUnknownEncoding );
                break;
        }
    }

    return istream;
}


//////////////////////////////////////////////////////////////////////////
void XmlReader::PushInputBuffer(
    const void * pBuffer,
    size_t nBufferLen,
    ReadEncoding encoding,
    const char * pStreamURI,
    bool bCopy )
{
    if (mInputDepth >= kMaxInputDepth) {
        Fatal( kErrorRecursionDepthExceeded );
        return;
    }

    const ReadEncoding defaultEncoding = mpInputStack ? mpInputStack->mEncoding : kReadEncodingUTF8;

    InputStream * const istream = CreateStreamFromBuffer(pBuffer, nBufferLen, encoding, pStreamURI, bCopy, defaultEncoding);

    if (istream == NULL) {
        Fatal( kErrorNoMemory );
        return;
    }

    istream->mpNext = mpInputStack;
    mpInputStack = istream;
    if (istream->mpStreamURI || mpTopNamedStream == NULL)
        mpTopNamedStream = istream;
    mInputDepth++;

    ReadChar();
}

//////////////////////////////////////////////////////////////////////////
void XmlReader::AppendInputBuffer(
    const void * pBuffer,
    size_t nBufferLen,
    ReadEncoding encoding,
    const char * pStreamURI,     // URI of stream, for debugging purposes
    bool bCopy)
{
    if (mInputDepth >= kMaxInputDepth) {
        Fatal( kErrorRecursionDepthExceeded );
        return;
    }

    // Note: This is wrong: it should be using the last item in the stack, not the first. 99.9% of the time these will be the same.
    const ReadEncoding defaultEncoding = mpInputStack ? mpInputStack->mEncoding : kReadEncodingUTF8;

    InputStream * const istream = CreateStreamFromBuffer(pBuffer, nBufferLen, encoding, pStreamURI, bCopy, defaultEncoding);

    if (istream == NULL) {
        Fatal( kErrorNoMemory );
        return;
    }

    // Put istream at the back (bottom) of the input stack. If the input stack is empty then 
    // the back will be the same as the front (top).

    if(mpInputStack)
    {
        InputStream * pStreamLast = mpInputStack;

        while(pStreamLast->mpNext)
            pStreamLast = pStreamLast->mpNext;

        pStreamLast->mpNext = istream;
    }
    else
    {
        mpInputStack = istream;

        if (mpTopNamedStream == NULL)
            mpTopNamedStream = istream;

        ReadChar();
    }
}

//////////////////////////////////////////////////////////////////////////
void XmlReader::PopInputStream() {
    InputStream * const pStream = mpInputStack;
    EA_ASSERT( pStream );
    if (pStream) {
        mpInputStack = mpInputStack->mpNext;
        if (pStream->mpStreamURI)
            mpTopNamedStream = pStream;
        if(pStream->mOwnBuffer)
            mpCoreAllocator->Free( pStream->mpInputBufferStart );
        if (pStream->mpStreamPtr)
            pStream->mpStreamPtr->Release();
        mpCoreAllocator->Free( pStream );
        mInputDepth--;
    }
}

//////////////////////////////////////////////////////////////////////////
const char * XmlReader::GetSourcePath() const {
    for (InputStream * pStream = mpInputStack; pStream; pStream = pStream->mpNext) {
        if (pStream->mpStreamURI)
            return pStream->mpStreamURI;
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
void XmlReader::Fatal( ResultCode rc ) {
    if (mResultCode == kSuccess) {
        mResultCode = rc;
        // To consider: Call the callback function here.
    }
}

//////////////////////////////////////////////////////////////////////////
void XmlReader::AddNamespaceAlias( const char * pAliasName, const char * pNamespace, bool bCopy ) {
    if (bCopy) {
        pAliasName = StrDup( mpCoreAllocator, pAliasName );
        pNamespace = StrDup( mpCoreAllocator, pNamespace );
    }

    NSAlias * const pAlias = (NSAlias *)mpCoreAllocator->Alloc( sizeof( NSAlias ), UTFXML_ALLOC_PREFIX "XmlReader/NSAlias", 0 );
    if (pAlias) {
        pAlias->mOwnStrings = bCopy;
        pAlias->mpNamespace = pNamespace;
        pAlias->mpAlias = pAliasName;
        pAlias->mpNext = mpNamespaceAliases;
        mpNamespaceAliases = pAlias;
    } else {
        Fatal( kErrorNoMemory );
    }
}

//////////////////////////////////////////////////////////////////////////
const char * XmlReader::GetNamespaceFromAlias( const char * pAliasName ) {
    for (NSAlias * pAlias = mpNamespaceAliases; pAlias != NULL; pAlias = pAlias->mpNext) {
        if (strcmp( pAlias->mpAlias, pAliasName ) == 0)
            return pAlias->mpNamespace;
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
int32_t XmlReader::GetElementNamespaceCount() const {
    // We walk from the end of the namespaceDecls stack backwards and 
    // count any entries that have the same depth as the the current
    // element depth. The last entries will have bigger depth, and 
    // since namespaces are pushed and popped, all entries are contiguous.
    int32_t count = 0;

    if(mpNamespaceDecls)
    {
        const int32_t currentElementDepth = GetDepth();
        
        for(NSDeclaration* pNS = mpNamespaceDecls; pNS && (pNS->mDepth == currentElementDepth); pNS = pNS->mpPrev)
            ++count;
    }

    return count;
}

//////////////////////////////////////////////////////////////////////////
const char* XmlReader::GetElementNamespace(int32_t index, const char** pNamespace) const {
    if(mpNamespaceDecls)
    {
        const int32_t  currentElementDepth = GetDepth();
        NSDeclaration* pNS = mpNamespaceDecls;
        int32_t        i = 0;

        // Currently this results in the elements being iterated backwards relative to their xml text order.
        while(pNS && (pNS->mDepth == currentElementDepth) && (i < index))
            pNS = pNS->mpPrev;

        if(pNS && (pNS->mDepth == currentElementDepth) && (i == index))  // If we got to the desired item...
        {
            if(pNamespace)
                *pNamespace = pNS->mpNamespace;
            return pNS->mpLocalName;
        }
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////////
bool XmlReader::IsNameStartChar( int32_t c ) {
    // Note we don't use the CRT character test functions
    // because we want to use exactly the definition in the XML standard.
    return (gNameStartChars[ (c >> 8) & 0x00ff ][ (c >> 3) & 0x1f ] & (1 << (c & 7))) != 0;
}

//////////////////////////////////////////////////////////////////////////
bool XmlReader::IsNameChar( int32_t c ) {
    // Note we don't use the CRT character test functions
    // because we want to use exactly the definition in the XML standard.
    return (gNameChars[ (c >> 8) & 0x00ff ][ (c >> 3) & 0x1f ] & (1 << (c & 7))) != 0;
}

//////////////////////////////////////////////////////////////////////////
void XmlReader::DefineEntity(
    const char * pEntityName,
    const char * pEntityValue )
{
    if (mElementDepth > 0) {
        Fatal( kErrorCantDefineEntitiesOnceParsing );
    } else {
        pEntityName  = StrDup( mpCoreAllocator, pEntityName );
        pEntityValue = StrDup( mpCoreAllocator, pEntityValue );
        mEntityDecls[ pEntityName ] = pEntityValue;
    }
}

//////////////////////////////////////////////////////////////////////////
void XmlReader::ReadChar() {
    while (mpInputStack) {
        mChar = mpInputStack->ReadChar();
        if (mChar >= 0)
            return;

        switch (mChar) {
            case InputStream::EoF:
                // We finished this stream, pop it and see if there's any more
                PopInputStream();

                // Find the top named stream again
                mpTopNamedStream = NULL;
                for (InputStream * istream = mpInputStack; istream != NULL; istream = istream->mpNext) {
                    if (istream->mpStreamURI != NULL || istream->mpNext == NULL) {
                        mpTopNamedStream = istream;
                        break;
                    }
                }
                break;

            case InputStream::IOError:
                Fatal( kErrorRead );
                return;

            case InputStream::PartialChar:
                Fatal( kErrorPartialChar );
                return;

            case InputStream::IncorrectEncoding:
                Fatal( kErrorIncorrectEncoding );
                return;

            default:
                Fatal( kErrorInternal );
                return;
        }
    }

    // Eof logic
    mChar = -1;
}

//////////////////////////////////////////////////////////////////////////
// This function returns true and moves to the next char if the current 
// char is c. If the current char is not c this function does nothing.
//
bool XmlReader::SkipChar( int32_t c ) {
    if (mChar != c)
        return false; // This is the most common pathway.

    ReadChar();
    return true;
}

//////////////////////////////////////////////////////////////////////////
bool XmlReader::SkipWS() {
    // We use the XML definition of whitespace.
    bool result = false;
    while (mChar == 0x20 || mChar == 0x0D || mChar == 0x0A || mChar == 0x09) {
        ReadChar();
        result = true;
    }
    return result;
}

//////////////////////////////////////////////////////////////////////////
// The expanded name is required by the XML Standard to be in the form
// of a URI, though it has no location meaning.
void XmlReader::PushNamespace( const char * nsLocalName, const char * nsExpandedName ) {
    // Create a new namespace declaration
    NSDeclaration * const pNS = (NSDeclaration *)mpCoreAllocator->Alloc( sizeof (NSDeclaration), UTFXML_ALLOC_PREFIX "XmlReader/NSDeclaration", 0 );

    pNS->mpLocalName = StrDup( mpCoreAllocator, nsLocalName );
    pNS->mpNamespace = StrDup( mpCoreAllocator, nsExpandedName );
    pNS->mpAlias     = NULL;
    pNS->mDepth      = mElementDepth;
    pNS->mpPrev      = mpNamespaceDecls;

    mpNamespaceDecls = pNS;

    if (nsLocalName[ 0 ] == '\0')
        mDefaultNS = pNS;

    // See if there's any application aliases for this
    for (NSAlias * alias = mpNamespaceAliases; alias != NULL; alias = alias->mpNext) {
        if (strcmp( pNS->mpNamespace, alias->mpNamespace ) == 0) {
            pNS->mpAlias = alias->mpAlias;
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
void XmlReader::PopNamespace() {
    NSDeclaration * const pNS = mpNamespaceDecls; 

    mpNamespaceDecls = pNS->mpPrev;
    mpCoreAllocator->Free( (void *)pNS->mpNamespace );
    mpCoreAllocator->Free( (void *)pNS->mpLocalName );
    mpCoreAllocator->Free( pNS );
}

//////////////////////////////////////////////////////////////////////////
const char * XmlReader::ParseName() {
    if (IsNameStartChar( mChar )) {
        AppendChar( mChar );
        ReadChar();

        while (IsNameChar( mChar )) {
            AppendChar( mChar );
            ReadChar();
        }

        if (mResultCode == kSuccess) 
            return TokenText();
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
// We read and parse a string of either of the following forms:
//    xHHHH;
//    DDDD;
// This function is called when we encounter strings like this:
//    &#x2AC4;
//    &#5543;
//
bool XmlReader::ParseCharacterRef()
{
    const ssize_t kMaxCharEntityLength = 64;   // Nobody will ever use a number this high, but W3C tests use it and it doesn't really cost us any more to have this limit.
    char entityRef[ kMaxCharEntityLength + 1 ];
    ssize_t nEntityRefLength = 0;

    if (SkipChar( 'x' ) || SkipChar( 'X' )) {
        // A hex character constant
        while ( ((((unsigned)mChar - '0') <= '9' - '0') ||    // This is likely faster than using iswxdigit( mChar ), given Microsoft's non-trivial implementation of it.
                 (((unsigned)mChar - 'a') <= 'f' - 'a') || 
                 (((unsigned)mChar - 'A') <= 'F' - 'A')) ) 
        {
            if (nEntityRefLength >= kMaxCharEntityLength) {
                Fatal( kErrorBadCharRef );
                return false;
            }
            entityRef[ nEntityRefLength++ ] = (char)mChar;
            ReadChar();
        }
        
        if (nEntityRefLength == 0) {
            Fatal( kErrorBadCharRef );
            return false;
        }

        if (!SkipChar( ';' )) {
            Fatal( kErrorMissingSemicolon );
            return false;
        }
        
        entityRef[ nEntityRefLength++ ] = '\0';
        AppendChar( strtol( entityRef, NULL, 16 ) );
    } else {
        // Decimal char constant
        uint32_t nResultChar = 0;

        for (;;) {
            const uint32_t nDigit = (unsigned)mChar - '0';

            if (nDigit <= 9) {
                nResultChar = (nResultChar * 10u) + nDigit;
                nEntityRefLength++;
            } else {
                break;
            }
            ReadChar();
        }

        if (!SkipChar( ';' )) {
            Fatal( kErrorMissingSemicolon );
            return false;
        }

        // Check to see if it's too long
        if ((nEntityRefLength == 0) || (nEntityRefLength > kMaxCharEntityLength) || (nResultChar > 0x00010000)) {
            Fatal( kErrorBadCharRef );
            return false;
        }

        AppendChar( (int32_t)nResultChar );
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////
bool XmlReader::ParseEntityRef() {

    // See if it starts with a name start char.
    if (!IsNameStartChar( mChar )) {
        Fatal( kErrorSyntax );
        return false;
    }

    // Append to entity.
    AppendChar( mChar );
    ReadChar();
    
    // Collect name characters
    while (IsNameChar( mChar )) {
        AppendChar( mChar );
        ReadChar();
    }
    
    if (!SkipChar( ';' )) {
        Fatal( kErrorMissingSemicolon );
        return false;
    }

    //AppendByte( '\0' );
    return true;
}

//////////////////////////////////////////////////////////////////////////
bool XmlReader::ParseEntityOrCharacterRef() {
    // Parse character or entity reference in text

    if (SkipChar( '#' )) {      // A character entity
        if (ParseCharacterRef()) {
            return mResultCode == kSuccess;
        }
    } else {                    // An entity reference
        mTokenBuffer.PushBookmark();           // Save the current token as-is
        if (ParseEntityRef()) {
            const char * const pEntityName = TokenText(); // Get the entity name token

            tEntityMap::const_iterator it = mEntityDecls.find( pEntityName );
            if (it != mEntityDecls.end()) {
                const char * const pEntityValue = it->second;

                mTokenBuffer.PopBookmark();// Restore the previous token
                AppendBytes( (const uint8_t *)pEntityValue, ssize_t(strlen( pEntityValue )) );
                return true;
            }
            
            if (ResolveEntity( pEntityName )) {
                mTokenBuffer.PopBookmark();// Restore the previous token
                return true;
            }

            if ((mState == State_Text || mState == State_Undecided) && (mFlags & kIncludeUnresolvedEntityRef)) {
                if (mState == State_Undecided) {
                    // If we're in an undecided state, then just return the entity reference as the next token
                    mpTokenName = pEntityName;
                    mNodeType = EntityRef;
                    return true;
                } else {
                    // We need to return the characters preceding the entity ref first,
                    // so make a temp copy of the entity ref and restore the previous
                    // token.
                    mpPendingEntityRef = StrDup( mpCoreAllocator, pEntityName );
                    mTokenBuffer.PopBookmark();
                    mState = State_EntityRef;
                    return true;
                }
            } else {
                // Can't return an unresolved reference in mid-token
                mTokenBuffer.PopBookmark();// Restore the previous token
                Fatal( kErrorUndefinedEntity );
            }
        } else {
            mTokenBuffer.PopBookmark();// Restore the previous token
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////
bool XmlReader::ParseComment() {
    // Declaration or comment
    if (SkipChar( '-' )) {
        // Comment
        if (SkipChar( '-' )) {
            // Parse comment text
            for (;;) {
                if (mChar < 0) {
                    Fatal( kErrorUnclosedToken );
                    return false;
                }
                if (SkipChar( '-' )) {
                    if (SkipChar( '-' )) {
                        if (SkipChar( '>' )) {
                            // Finish the comment
                            mpTokenValue = TokenText( &mValueLength );
                            mState = State_Undecided;
                            mNodeType = Comment;
                            return true;
                        } else {
                            // Two dashes followed by anything other than '>'
                            // is an error
                            Fatal( kErrorSyntax );
                            return false;
                        }
                    } else {
                        // Single dash is OK tho
                        AppendByte( '-' );
                        AppendChar( mChar );
                        ReadChar();
                    }
                } else {
                    AppendChar( mChar );
                    ReadChar();
                }
            }
        } else {
            // error - invalid comment
            return false;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////
bool XmlReader::ParseDeclaration() {
    // Parse a DOCTYPE, ATTLIST or other declaration
    mpTokenName = ParseName();
    if (!mpTokenName) {
        Fatal( kErrorSyntax );
        return false;
    }

    // Determin the declaration type
    if (strcmp( mpTokenName, "DOCTYPE" ) == 0) {
        mNodeType = DocTypeDecl;
    } else if (strcmp( mpTokenName, "ENTITY" ) == 0) {
        mNodeType = EntityDecl;
    } else if (strcmp( mpTokenName, "ELEMENT" ) == 0) {
        mNodeType = ElementDecl;
    } else if (strcmp( mpTokenName, "ATTLIST" ) == 0) {
        mNodeType = AttListDecl;
    } else if (strcmp( mpTokenName, "NOTATION" ) == 0) {
        mNodeType = NotationDecl;
    } else {
        Fatal( kErrorSyntax );
        return false;
    }

    // Parse the parameters to the declaration
    mAttributeArray.clear();
    for (;;) {
        SkipWS();
        if (mResultCode != kSuccess)
            return false;
        if (mChar < 0) {
            Fatal( kErrorUnclosedToken );
            return false;
        }

        if (SkipChar( '>' )) {
            // Got to the end
            break;
        } else if (SkipChar( '\'' )) {
            while (!SkipChar( '\'' ) && mResultCode == kSuccess) {
                if (mChar <= 0) {
                    Fatal( kErrorUnclosedToken );
                    return false;
                }
                AppendChar( mChar );
                ReadChar();
            }
            mAttributeArray.push_back( TokenText() );
        } else if (SkipChar( '\"' )) {
            while (!SkipChar( '\"' ) && mResultCode == kSuccess) {
                if (mChar <= 0) {
                    Fatal( kErrorUnclosedToken );
                    return false;
                }
                AppendChar( mChar );
                ReadChar();
            }
            mAttributeArray.push_back( TokenText() );
        } else if (mNodeType == DocTypeDecl && SkipChar( '[' )) {
            for (;;) {
                // Parse internal subset.
                // For now, we just skip over it, instead of returning it to the caller.
                SkipWS();
                if (mChar <= 0) {
                    Fatal( kErrorUnclosedToken );
                    return false;
                } else if (SkipChar( '<' ) && SkipChar( '!' )) {
                    if (!ParseDeclaration())
                        return false;
                } else if (SkipChar( ']' )) {
                    // Reset the node type as it may have been changed.
                    mNodeType = DocTypeDecl;
                    break;
                }
            }
        } else {
            while (mChar != '>' && !SkipWS() && mResultCode == kSuccess) {
                if (mChar <= 0) {
                    Fatal( kErrorUnclosedToken );
                    return false;
                }
                AppendChar( mChar );
                ReadChar();
            }
            mAttributeArray.push_back( TokenText() );
        }
    }

    if (mNodeType == DocTypeDecl) {
        if (mAttributeArray.empty()) {
            Fatal( kErrorSyntax );
            return false;
        }

        // Get the DOCTYPE name, and the system and public IDs.
        mpDocTypeName = mAttributeArray[ 0 ];
        mpSystemId = NULL;
        mpPublicId = NULL;

        if (mAttributeArray.size() > 1) {
            if (strcmp( mAttributeArray[ 1 ], "SYSTEM" ) == 0) {
                if (mAttributeArray.size() < 3) {
                    Fatal( kErrorSyntax );
                    return false;
                }
                mpSystemId = mAttributeArray[ 2 ];
                mpPublicId = NULL;
            } else if (strcmp( mAttributeArray[ 1 ], "PUBLIC" ) == 0) {
                if (mAttributeArray.size() < 4) {
                    Fatal( kErrorSyntax );
                    return false;
                }
                mpPublicId = mAttributeArray[ 2 ];
                mpSystemId = mAttributeArray[ 3 ];
            }
        }
    }

    mpTokenValue = mpTokenName;
    mState = State_Undecided;
    return true;
}

//////////////////////////////////////////////////////////////////////////
bool XmlReader::ParseProcessingInstruction() {
    bool bProlog = false;

    // The name of the processing instruction
    const char * const pPIName = ParseName();
    if (!pPIName) {
        if (mChar < 0)
            Fatal( kErrorIncompletePI );
        else 
            Fatal( kErrorSyntax );
        return false;
    }

    // If the name is 'xml' then it's a prolog
    if (mNodeType == None && strcmp( pPIName, "xml" ) == 0) {
        mIsStandalone = true;  // it will be set to false if there is a   standalone="no"   entry.
        bProlog = true;
    }

    // Processing instruction with no arguments
    if (SkipChar( '?' )) {
        if (SkipChar( '>' )) {
            mpTokenName = TokenText();
            mpTokenValue = "";
            mValueLength = 0;
            if (bProlog)
                mNodeType = Prologue;
            else 
                mNodeType = ProcessingInstruction;
            mState = State_Undecided;
            return true;
        } else {
            Fatal( kErrorSyntax );
            return false;
        }
    }

    // If PI name isn't followed by '?' then it must be followed by a space.
    if (!SkipWS()) {
        Fatal( kErrorSyntax );
        return false;
    }

    if (bProlog) {
        mAttributeArray.clear();
        for (;;) {
            if (SkipChar( '?' )) {
                if (SkipChar( '>' )) {
                    // Do prolog processing
                    mNodeType = Prologue;
                    mState = State_Undecided;
                    return true;
                } else {
                    Fatal( kErrorSyntax );
                    return false;
                }
            }

            const char * pAttrName = NULL;

            if (IsNameStartChar( mChar )) {
                pAttrName = ParseName();

                if (!pAttrName) {
                    Fatal( kErrorInvalidToken );
                    return false;
                }
                mAttributeArray.push_back( pAttrName );
            }

            SkipWS();

            if (SkipChar( '=' )) {
                SkipWS();
                if (mChar == '"' || mChar == '\'') {
                    int nQuote = mChar;
                    ReadChar();
                    while (mChar >= 0 && mChar != nQuote && mResultCode == kSuccess) {
                        AppendChar( mChar );
                        ReadChar();
                    }
                    if (mResultCode != kSuccess)
                        return false;
                    if (mChar < 0) {
                        Fatal( kErrorIncompletePI );
                        return false;
                    }
                    ReadChar();

                    const char * pTokenText = TokenText();
                    mAttributeArray.push_back( pTokenText );

                    if((mNodeType == Prologue) && pAttrName && (strcmp(pAttrName, "standalone") == 0))
                        mIsStandalone = (strcmp(pTokenText, "no") == 0);
                }
                SkipWS();
            }
            SkipWS();
        }
    }

    mpTokenName = pPIName;
    while (mChar > 0 && mResultCode == kSuccess) {
        if (SkipChar( '?' )) {
            // Check if we're at the end.
            if (SkipChar( '>' )) {
                mpTokenValue = TokenText( &mValueLength );
                mNodeType = ProcessingInstruction;
                mState = State_Undecided;
                return true;
            } else {
                // No, it was just a question mark.
                AppendByte( '?' );
            }
        }
        AppendChar( mChar );
        ReadChar();
    }

    mAttributeArray.push_back( TokenText() );

    // If it was a premature end of file, return the fact that we saw no end
    // to the processing instruction
    if (mChar < 0) {
        Fatal( kErrorIncompletePI );
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////
XmlReader::NSDeclaration * XmlReader::LookupNamespaceFromPrefix( const char * pQName ) {
    const char * const pNSSep = strchr( pQName, ':' );
    if (pNSSep) {
        ssize_t nPrefixLen = pNSSep - pQName;
        for (NSDeclaration * pNS = mpNamespaceDecls; pNS != NULL; pNS = pNS->mpPrev) {
            // Can't use strcmp here because the prefix isn't null-terminated.
            // instead, we memcmp, and then test for an explicit NUL at the end of
            // the namespace's local name.
            if (pNS->mpLocalName
                && memcmp( pNS->mpLocalName, pQName, size_t(nPrefixLen) ) == 0
                && pNS->mpLocalName[ nPrefixLen ] == '\0')
            {
                return pNS;
            }
        }
    } else if (mDefaultNS) {
        return mDefaultNS;
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
const char * XmlReader::NormalizeNamespace( NSDeclaration * pNS, const char * pQName ) {
    // Transform the element name, replacing the namespace with its alias.
    if (pNS && pNS->mpAlias && pNS->mpAlias[ 0 ]) {
        const char * pNSSep = strchr( pQName, ':' );
        if (pNSSep == NULL)
            pNSSep = pQName;
        else 
            pNSSep += 1;
        size_t nAliasLen = strlen( pNS->mpAlias );
        size_t nLocalLen = strlen( pNSSep );
        mTokenBuffer.AppendBytes( (const uint8_t *)pNS->mpAlias, (ssize_t)nAliasLen );
        mTokenBuffer.AppendByte( ':' );
        mTokenBuffer.AppendBytes( (const uint8_t *)pNSSep, (ssize_t)nLocalLen + 1 );
        return (char *)mTokenBuffer.FinishToken();
    }
    return pQName;
}

//////////////////////////////////////////////////////////////////////////
void XmlReader::NormalizeNamespaces() {
    mElementNS = LookupNamespaceFromPrefix( mpTokenName );
    if (mpNamespaceAliases) {
        if (mElementNS)
            mpTokenName = NormalizeNamespace( mElementNS, mpTokenName );

        for (eastl_size_t i = 0, iEnd = mAttributeArray.size(); i < iEnd; i += 2) {
            NSDeclaration * const pNS = LookupNamespaceFromPrefix( mAttributeArray[ i ] );
            if (pNS)
                mAttributeArray[ i ] = NormalizeNamespace( pNS, mAttributeArray[ i ] );
        }
    }
}

//////////////////////////////////////////////////////////////////////////
bool XmlReader::ParseElement() {
    // Check if it's an end element
    bool bEndElement = false;
    if (SkipChar( '/' )) {
        bEndElement = true;
        if (mElementDepth == 0) {
            Fatal( kErrorInvalidToken );
            return false;
        }
    }

    // Clear the token buffer
    mAttributeArray.clear();
    mIsEmptyElement = false;

    // Look for a valid XML name
    mpTokenValue = mpTokenName = ParseName();
    if (!mpTokenValue) {
        Fatal( kErrorInvalidToken );
        return false;
    }

    if (mChar < 0) {
        Fatal( kErrorUnclosedToken );
        return false;
    }

    SkipWS();

    if (bEndElement) {
        if (SkipChar( '>' )) {
            // Return an end element
            NormalizeNamespaces();
            mState = State_Undecided;
            mNodeType = EndElement;
            mElementDepth -= 1;
            return true;
        } else {
            // error - '>' expected
            Fatal( mChar < 0 ? kErrorUnclosedToken : kErrorSyntax );
            return false;
        }
    }

    // Read the list of attributes
    for (;;) {

        // Empty element close
        if (mChar < 0) {
            Fatal( kErrorUnclosedToken );
            return false;
        } else if (SkipChar( '/' )) {
            if (SkipChar( '>' )) {
                // Return start element and set state for empty element
                NormalizeNamespaces();
                mState = State_Undecided;
                mNodeType = Element;
                mIsEmptyElement = true;
                return true;
            } else {
                // error - '>' expected
                Fatal( mChar < 0 ? kErrorUnclosedToken : kErrorSyntax );
                return false;
            }
        } else if (SkipChar( '>' )) {
            // End of the element
            // Return the token
            NormalizeNamespaces();
            mState = State_Undecided;
            mIsEmptyElement = false;
            mNodeType = Element;
            return true;
        } else if (IsNameStartChar( mChar )) {            // Start of an attribute

            // Parse the attribute name
            const char * pAttrName = ParseName();
            if (!pAttrName) {
                Fatal( kErrorInvalidToken );
                return false;
            }

            SkipWS();

            if (!SkipChar( '=' )) {
                // error - '=' expected
                Fatal( mChar < 0 ? kErrorUnclosedToken : kErrorSyntax );
                return false;
            }

            SkipWS();

            if (mChar != '\"' && mChar != '\'') {
                // error - '"' expected
                Fatal( mChar < 0 ? kErrorUnclosedToken : kErrorSyntax );
                return false;
            }
            int32_t nQuoteChar = mChar;
            ReadChar();

            // Save the input stream where we saw the opening quote in.
            InputStream * const pOpeningQuoteStream = mpInputStack;
            for (;;) {
                // If we see a closing quote in the same stream as the opening
                // quote, then we're finished with this attribute value.

                if (mChar == nQuoteChar && mpInputStack == pOpeningQuoteStream)
                    break;
                
                if (SkipChar( '&' )) {
                    // Resolve the entity
                    if (!ParseEntityOrCharacterRef()) {
                        return false;
                    }
                } else if (mChar < 0) {
                    Fatal( kErrorUnclosedToken );
                    return false;
                } else {
                    AppendChar( mChar );
                    ReadChar();
                }
            }
            // Skip over the closing quote.
            ReadChar();

            // Finish the attribute value.
            const char * const pAttrValue = TokenText();

            // Check see if this attr is a namespace definition.
            // We copy namespace attributes to the namespace stack and not
            // to the attribute array. The user can use GetElementNamespace 
            // to read the namespaces declared for the current element.
            if (memcmp( pAttrName, "xmlns", 5 ) == 0) {
                if (pAttrName[ 5 ] == '\0') {
                    // Declaring a default namespace. 
                    PushNamespace( &pAttrName[ 5 ], pAttrValue );
                } else if (pAttrName[ 5 ] == ':') {
                    // Declaring a default namespace
                    PushNamespace( &pAttrName[ 6 ], pAttrValue );
                }
            } else {
                // Note: The name we push here includes any namespace prefixes that may
                // be present. Attributes don't currently store prefixed attributes 
                // with the prefix and local name separate. For example, if the attribute
                // declaration is "ns.table="12", the name is stored as "ns.table".
                mAttributeArray.push_back( pAttrName );
                mAttributeArray.push_back( pAttrValue );
            }

            // Leading space
            SkipWS();
        } else {
            // Error - invalid element character
            Fatal( kErrorInvalidToken );
            return false;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
bool XmlReader::Read() {

    if (mNodeType == Element && !mIsEmptyElement)
        mElementDepth += 1;

    // Pop any namespace decls
    if (mNodeType == EndElement || (mNodeType == Element && mIsEmptyElement)) {
        bool bChanged = false;
        while (mpNamespaceDecls && mpNamespaceDecls->mDepth > mElementDepth) {
            PopNamespace();
            bChanged = true;
        }

        // Calculate the new default namespace, if any.
        if (bChanged) {
            mDefaultNS = NULL;
            for (NSDeclaration * pNS = mpNamespaceDecls; pNS != NULL; pNS = pNS->mpPrev) {
                if (pNS->mpLocalName[ 0 ] == '\0') {
                    mDefaultNS = pNS;
                    break;
                }
            }
        }

        //if (mFlags & kRetainParentState)
        //    mTokenBuffer.PopBookmark();
    } else if (mpPendingEntityRef) {
        // If there's a pending entity reference
        // if we haven't returned it yet, then return it.
        if (mState == State_EntityRef) {
            mpTokenName = mpPendingEntityRef;
            mNodeType = EntityRef;
            mState = State_Undecided;
            return true;
        }
        // Otherwise, free it.
        mpCoreAllocator->Free( (void *)mpPendingEntityRef );
        mpPendingEntityRef = NULL;
    }

next_token:
    // Set the location of the current token.
    if (mpTopNamedStream) {
        mLineIndex   = mpTopNamedStream->mLineIndex;
        mColumnIndex = mpTopNamedStream->mColumnIndex;
        mByteIndex   = mpTopNamedStream->mByteIndex;

        // Due to some assymmetry in the state algorithm below, upon exiting
        // State_Text, we will have 
        // already read one-past the next node. So mColumnIndex will be referring
        // to the char -after- the '<' char of an element instead of the '<' char itself.
        if((mNodeType == CharacterData) && !mIsCData)
        {
            --mColumnIndex;
            --mByteIndex;
        }
        else if(mState == State_CDATA)
        {
            // mColumnIndex and mByteIndex currently point to the first character
            // of the CDATA string, which is 9 characters into the CDATA node. 
            // In order to be consistent, we subtract 9 so that these values refer
            // to the beginning of the CDATA node <![CDATA[blah]]>.
            mColumnIndex -= 9;
            mByteIndex   -= 9;
        }
    }

    /// Free everything in the current token buffer up to the last bookmark
    /// (which may be null, in which case everything is cleared.)
    mTokenBuffer.RestoreBookmark();

    for (;;) {
        // End of file.
        switch (mState) {
            case State_EntityRef:
            default:
                break;

            case State_Undecided: {
                // Start a new token (Fall through)
                if (SkipChar( '<' )) {
                    // Start of a new element. Continue parsing.
                    mState = State_Element;
                } else if (SkipChar( '&' )) {
                    // Parse character or entity reference in text
                    if (!ParseEntityOrCharacterRef())
                        return false;
                    if (mNodeType == EntityRef)
                        return true;
                    mState = State_Text;
                } else if (mChar < 0) { // End of stream
                    mNodeType = None;
                    // mResultCode == kSuccess;     Do we want to do this? 
                    return false;
                } else {
                    // Start of character data. Continue parsing.
                    mState = State_Text;
                    AppendChar( mChar );
                    ReadChar();
                }
                break;
            }

            case State_Text: {
                if (SkipChar( '<' )) {
                    // Start of a new element. Return the current token.
                    mpTokenValue = TokenText( &mValueLength );
                    mState = State_Element;
                    mNodeType = CharacterData;
                    mIsCData = false;
                    return true;
                } else if (SkipChar( '&' )) {
                    // Parse character or entity reference in text
                    if (!ParseEntityOrCharacterRef())
                        return false;
                    if (mpPendingEntityRef) {
                        mpTokenValue = TokenText( &mValueLength );
                        mState = State_Undecided;
                        mNodeType = CharacterData;
                        mIsCData = false;
                        return (mResultCode == kSuccess);
                    }
                } else if (mChar < 0) { // End of file signal
                    mpTokenValue = TokenText( &mValueLength );
                    mState = State_Undecided;
                    mNodeType = CharacterData;
                    mIsCData = false;
                    return (mResultCode == kSuccess);
                } else {
                    AppendChar( mChar );
                    ReadChar();
                }
                break;
            }

            case State_CDATA: {
                // Look for a sequence of ']]>'
                if (SkipChar( ']' )) {
                    if (SkipChar( ']' )) {
                        for (;;) {
                            if (SkipChar( '>' )) {
                                // If it's ']]>' we're done
                                if (mTokenBuffer.TokenLength() > 0) {
                                    mpTokenValue = TokenText( &mValueLength );
                                    mState = State_Text;
                                    mNodeType = CharacterData;
                                    mIsCData = true;
                                    return (mResultCode == kSuccess);
                                }
                                mState = State_Undecided;
                                break;
                            } else if (SkipChar( ']' )) {
                                // If it's ']]]', drop first one, and continue
                                AppendChar( ']' );
                                continue;
                            } else {
                                AppendChar( ']' );
                                AppendChar( ']' );
                                break;
                            }
                        }
                    } else {
                        AppendChar( ']' );
                        break;
                    }
                } if (mChar < 0) { // End of file signal
                    Fatal( kErrorUnclosedToken );
                    return false;
                }

                AppendChar( mChar );
                ReadChar();
                break;
            }

            case State_Element: {
                if (SkipChar( '!' )) {
                    if (ParseComment()) {
                        // If we're not skipping comments, return true
                        if (mFlags & kIncludeComments)
                            return true;
                        goto next_token;
                    } else if (SkipChar( '[' )) {
                        // CDATA section
                        const char * pName = ParseName();
                        if (!pName || strcmp( pName, "CDATA" ) != 0) {
                            Fatal( kErrorInvalidToken );
                            return false;
                        }
                        if (!SkipChar( '[' )) {
                            Fatal( kErrorInvalidToken );
                            return false;
                        }
                        mTokenBuffer.Clear();
                        mState = State_CDATA;
                        goto next_token;
                    } else {
                        // Parse a DOCTYPE, ATTLIST or other declaration
                        if (ParseDeclaration()) {
                            if (mNodeType == DocTypeDecl) {
                                if (mFlags & kIncludeDocType)
                                    return true;
                            } else if (mFlags & kIncludeDeclarations) {
                                return true;
                            }
                        }
                        goto next_token;
                    }
                } else if (SkipChar( '?' )) {
                    if (!ParseProcessingInstruction())
                        return false;
                    // If we're not skipping PI's, return true
                    if ((mNodeType == Prologue) && (mFlags & kIncludePrologue))
                        return true;
                    if ((mNodeType == ProcessingInstruction) && (mFlags & kIncludeProcessingInstructions))
                        return true;
                    goto next_token;
                } else {
                    return ParseElement();
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
bool XmlReader::Skip() {
    if (mNodeType == Element && !mIsEmptyElement) {
        int32_t     nElementDepth = mElementDepth;

        while (Read()) {
            if (mElementDepth == nElementDepth) {
                if (mNodeType == EndElement) {
                    return Read();
                }
                EA_FAIL_MESSAGE( "Tree depth inconsistant with node type" );
                Fatal( kErrorInternal );
            }
            EA_ASSERT( mElementDepth > nElementDepth );
        }
        return false;
    }

    return Read();
}

//////////////////////////////////////////////////////////////////////////
/// Returns the text name of this token.
/// The result will depend on the current node type:
///    Element - the current element name. This is the fully qualified name, of the format [<prefix>:]<localName>
///    DocumentType - The document type name.
///    Processing Instruction - The target of the processing instruction
///    Declaration (DOCTYPE, etc.) - the name of the declaration
///    Entity Ref - the name of the entity referenced.
const char * XmlReader::GetName() const {
    return mpTokenName;
}

//////////////////////////////////////////////////////////////////////////
/// Returns the name of this element, without the namespace prefix.
const char * XmlReader::GetLocalName() const {
    if (mNodeType == Element || mNodeType == EndElement) {
        const char * const pNSSep = strchr( mpTokenName, ':' );
        if (pNSSep)
            return pNSSep + 1;
        return mpTokenName;
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
/// Returns the complete, expanded namespace of this element.
const char * XmlReader::GetNamespaceURI() const {
    if (mElementNS && ((mNodeType == Element) || (mNodeType == EndElement)))
        return mElementNS->mpNamespace;
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
/// Return the text of the token (element name, text data, etc.).
/// Note that Text tokens may have embedded NUL characters (but will
/// be NUL terminated) -- use ValueLength() if you aren't sure.
/// For elements, this text will be the element name. For text and comments,
/// it will be the content of the element. For processing instruction, it will
/// be the target (such as "jsp" for <?jsp args?>. For namespaces, it will be
/// the namespace prefix. For doctype declarations it will be the doctype name.
const char * XmlReader::GetValue() const {
    switch (mNodeType) {
        case CharacterData:
        case Element:
        case EndElement:
        case Comment:
        case ProcessingInstruction:
        case DocTypeDecl:
        case EntityDecl:
        case ElementDecl:
        case AttListDecl:
        case NotationDecl:
            return mpTokenValue;

        case Prologue:
        case None:
        case EntityRef:
        default:
            return NULL;
    }
}

//////////////////////////////////////////////////////////////////////////
size_t XmlReader::GetValueLength() const {
    switch (mNodeType) {
        case CharacterData:
        case ProcessingInstruction:
        case Element:
        case EndElement:
        case Comment:
        case DocTypeDecl:
        case EntityDecl:
        case ElementDecl:
        case AttListDecl:
        case NotationDecl:
            return size_t(mValueLength);

        case Prologue:
        case None:
        case EntityRef:
        default:
            return 0;
    }
}

//////////////////////////////////////////////////////////////////////////
bool XmlReader::HasValue() const {
    switch (mNodeType) {
        case CharacterData:
        case ProcessingInstruction:
        case Element:
        case EndElement:
        case Comment:
        case DocTypeDecl:
        case EntityDecl:
        case ElementDecl:
        case AttListDecl:
        case NotationDecl:
            return true;

        case Prologue:
        case None:
        case EntityRef:
        default:
            return false;
    }
}


//////////////////////////////////////////////////////////////////////////
// For a name of "table" return "table"
// For a name of "xyz.table" return "xyz.table"
const char * XmlReader::GetAttributeName( int nIndex ) const
{
    nIndex *= 2;

    if ((eastl_size_t)nIndex < mAttributeArray.size())
        return mAttributeArray[ (eastl_size_t)nIndex ];

    return NULL;
}


//////////////////////////////////////////////////////////////////////////
// For a name of "table" return "table"
// For a name of "xyz.table" return "table"
const char * XmlReader::GetAttributeLocalName( int nIndex ) const
{
    const char* p = GetAttributeName( nIndex );

    if(p)
    {
        for(const char* pCurrent = p; *pCurrent; ++pCurrent)
        {
            if(*pCurrent == ':')
                return pCurrent + 1;
        }
    }

    return p;
}


//////////////////////////////////////////////////////////////////////////
const char * XmlReader::GetAttributeValue( int nIndex ) const
{
    nIndex *= 2;

    if ((eastl_size_t)(nIndex + 1) < mAttributeArray.size())
        return mAttributeArray[ (eastl_size_t)(nIndex + 1) ];

    return NULL;
}


//////////////////////////////////////////////////////////////////////////
const char * XmlReader::GetAttributeValue( const char * psAttrName ) const
{
    const eastl_size_t len = mAttributeArray.size();

    for (eastl_size_t i = 0; i < len; i += 2)
    {
        const char * pName = mAttributeArray[ i ];

        if (strcmp( pName, psAttrName ) == 0)
            return mAttributeArray[ i + 1 ];
    }

    return NULL;
}


} // namespace XML
} // namespace EA



