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
// Name: XmlTokenBuffer.cpp
//
// Purpose: Buffer to efficiently construct XML tokens in encoded form.
//
// Copyright (c) 2007 Electronic Arts Inc.
// Created by: Talin
/////////////////////////////////////////////////////////////////////////////

#include <UTFXml/internal/Config.h>
#include <UTFXml/XmlTokenBuffer.h>
#include EA_ASSERT_HEADER

namespace EA {
namespace XML {

//////////////////////////////////////////////////////////////////////////
XmlTokenBuffer::XmlTokenBuffer( ICoreAllocator * pAllocator, size_t blockSize )
    : mpAllocator( pAllocator )
    , mpCurrentBlock( NULL )
    , mpCurrentBlockEnd( NULL )
    , mpCurrentTokenBegin( NULL )
    , mpCurrentTokenPos( NULL )
    , mpBookmark( NULL )
    , mBlockSize( (blockSize > 0) ? blockSize : kBlockSizeDefault )
{
    EA_ASSERT( pAllocator );
}

//////////////////////////////////////////////////////////////////////////
XmlTokenBuffer::~XmlTokenBuffer() {
    while (mpCurrentBlock) {
        Block * const pBlock = mpCurrentBlock;
        mpCurrentBlock = mpCurrentBlock->mpPrev;
        mpAllocator->Free( pBlock );
    }
}

//////////////////////////////////////////////////////////////////////////
void XmlTokenBuffer::Clear() {
    while (mpCurrentBlock && mpCurrentBlock->mpPrev) {  // Free all but the first block.
        Block * const pBlock = mpCurrentBlock;
        mpCurrentBlock = mpCurrentBlock->mpPrev;
        mpCurrentBlockEnd = mpCurrentBlock->mpEnd;
        mpAllocator->Free( pBlock );
    }

    if (mpCurrentBlock)
        mpCurrentTokenPos = mpCurrentTokenBegin = mpCurrentBlock->mpBegin;
}

//////////////////////////////////////////////////////////////////////////
void XmlTokenBuffer::AppendEncodedChar( uint32_t c ) {
    // Encode as UTF-8
    if (c < 0x00000080u) {
        AppendByte( (uint8_t)c );
    } else if (c < 0x00000800u) {
        AppendBytes( uint8_t(  (c >>  6) | 0xc0 ),
                     uint8_t( (c | 0x80) & 0xbf ) );
    } else if (c < 0x00010000u) {
        AppendBytes( uint8_t(  (c >> 12) | 0xe0 ),
                     uint8_t( ((c >>  6) | 0x80) & 0xbf ),
                     uint8_t( (c | 0x80) & 0xbf ) );
    } else if (c < 0x00200000u) {
        AppendBytes( uint8_t(  (c >> 18) | 0xf0 ),
                     uint8_t( ((c >> 12) | 0x80) & 0xbf ),
                     uint8_t( ((c >>  6) | 0x80) & 0xbf ),
                     uint8_t( (c | 0x80) & 0xbf ) );
    } else {
        const uint32_t kIllegalUnicodeChar = 0x0000fffd;
        AppendBytes( uint8_t(  (kIllegalUnicodeChar >> 12) | 0xe0 ),
                     uint8_t( ((kIllegalUnicodeChar >>  6) | 0x80) & 0xbf ),
                     uint8_t( (kIllegalUnicodeChar | 0x80) & 0xbf ) );
    }
}

//////////////////////////////////////////////////////////////////////////
void XmlTokenBuffer::IncreaseCapacity( ssize_t nLen ) {
    size_t      currentSize = size_t(mpCurrentTokenPos - mpCurrentTokenBegin);
    size_t      requiredSize = (mpCurrentTokenPos - mpCurrentTokenBegin) + nLen + sizeof( Block );
    size_t      targetSize = mBlockSize;

    while (targetSize < requiredSize) {
        targetSize *= 2;
    }

    // Allocate the block
    uintptr_t pBlockMem = (uintptr_t)mpAllocator->Alloc( targetSize, UTFXML_ALLOC_PREFIX "XmlTokenBuffer", 0 );
    Block * pBlock = (Block *)pBlockMem;

    // Initialize the block
    pBlock->mpPrev = mpCurrentBlock;
    pBlock->mpBegin = (char *)(pBlockMem + sizeof(Block));
    pBlock->mpEnd = (char *)(pBlockMem + targetSize);

    // Relocate the partially-constructed token
    memcpy( pBlock->mpBegin, mpCurrentTokenBegin, currentSize );
    mpCurrentTokenBegin = pBlock->mpBegin;
    mpCurrentTokenPos = mpCurrentTokenBegin + currentSize;

    // Set this as the current block
    mpCurrentBlock = pBlock;
    mpCurrentBlockEnd = pBlock->mpEnd;
}

//////////////////////////////////////////////////////////////////////////
void XmlTokenBuffer::PushBookmark() {
    if (mpCurrentTokenBegin == NULL) {
        // If we're pushing a bookmark, we need to have at least one block allocated
        EA_ASSERT( mpCurrentTokenPos == NULL );
        IncreaseCapacity( 0 );
        EA_ASSERT( mpCurrentTokenBegin != NULL );
    }

    char * const pCurrentTokenBegin = mpCurrentTokenBegin;
    char * const pCurrentTokenPos = mpCurrentTokenPos;

    // Align mpCurrentTokenBegin
    mpCurrentTokenBegin = mpCurrentTokenPos = (char *)( ((uintptr_t)mpCurrentTokenPos + 7) & ~7 );

    // Allocate space for bookmark
    if (mpCurrentTokenPos + sizeof( Bookmark ) >= mpCurrentBlockEnd ) {
        IncreaseCapacity( sizeof( Bookmark ) );
    }

    // Fill in the bookmark
    Bookmark * pBookmark = (Bookmark *)mpCurrentTokenBegin;
    pBookmark->mpNext = mpBookmark;
    pBookmark->mpCurrentTokenBegin = pCurrentTokenBegin;
    pBookmark->mpCurrentTokenPos = pCurrentTokenPos;
    mpBookmark = pBookmark;
    mpCurrentTokenBegin = mpCurrentTokenPos = (char *)pBookmark + sizeof( Bookmark );
}

//////////////////////////////////////////////////////////////////////////
void XmlTokenBuffer::PopBookmark() {
    EA_ASSERT( mpBookmark );

    Bookmark * pBookmark = mpBookmark;
    mpBookmark = pBookmark->mpNext;

    // Search for the block containing this bookmark's mark
    Block * pBookmarkBlock = mpCurrentBlock;
    while (pBookmarkBlock != NULL) {
        if (pBookmark->mpCurrentTokenPos >= pBookmarkBlock->mpBegin
            && pBookmark->mpCurrentTokenPos <= pBookmarkBlock->mpEnd)
        {
            break;
        }
        pBookmarkBlock = pBookmarkBlock->mpPrev;
    }

    // Free up any blocks after the bookmarked one, and then restore the token.
    EA_ASSERT( pBookmarkBlock != NULL );
    if (pBookmarkBlock != NULL) {
        mpCurrentTokenBegin = pBookmark->mpCurrentTokenBegin;
        mpCurrentTokenPos = pBookmark->mpCurrentTokenPos;
        while (mpCurrentBlock != pBookmarkBlock) {
            Block * pBlock = mpCurrentBlock;
            mpCurrentBlock = mpCurrentBlock->mpPrev;
            mpCurrentBlockEnd = mpCurrentBlock->mpEnd;
            mpAllocator->Free( pBlock );
        }
    }
}

//////////////////////////////////////////////////////////////////////////
void XmlTokenBuffer::RestoreBookmark() {

    if (mpBookmark) {
        char * pPos = (char *)mpBookmark + sizeof( Bookmark );

        // Search for the block containing this bookmark's mark
        Block * pBookmarkBlock = mpCurrentBlock;
        while (pBookmarkBlock != NULL) {
            if (pPos >= pBookmarkBlock->mpBegin && pPos <= pBookmarkBlock->mpEnd)
                break;
            pBookmarkBlock = pBookmarkBlock->mpPrev;
        }

        // Free up any blocks after the bookmarked one, and then restore the token.
        //EA_ASSERT( pBookmarkBlock != NULL ); This assertion is triggering. Something needs to be fixed.
        if (pBookmarkBlock != NULL) {
            mpCurrentTokenBegin = mpCurrentTokenPos = pPos;
            while (mpCurrentBlock != pBookmarkBlock) {
                Block * pBlock = mpCurrentBlock;
                mpCurrentBlock = mpCurrentBlock->mpPrev;
                mpCurrentBlockEnd = mpCurrentBlock->mpEnd;
                mpAllocator->Free( pBlock );
            }
        }
    } else {
        Clear();
    }
}

} // namespace XML
} // namespace EA







