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
// Name: XmlTokenBuffer.h
// 
// Purpose: Buffer to efficiently construct XML tokens in encoded form.
//
// Copyright (c) 2007 Electronic Arts Inc.
// Created by: Talin
///////////////////////////////////////////////////////////////////////////////

#ifndef UTFXML_XMLTOKENBUFFER_H
#define UTFXML_XMLTOKENBUFFER_H

#ifndef INCLUDED_eabase_H
#include <EABase/eabase.h>
#endif

#ifndef EA_ALLOCATOR_ICOREALLOCATOR_INTERFACE_H
#include <coreallocator/icoreallocator_interface.h>
#endif

#include <UTFXml/internal/Config.h>
#include <string.h>

namespace EA {

    namespace XML {

        //////////////////////////////////////////////////////////////////////////
        /// A buffer that stores a series of incrementally-built tokens with
        /// a minimum of memory allocations (i.e. less than one allocation
        /// per token, since many tokens can share the same memory block.)
        //////////////////////////////////////////////////////////////////////////
        class UTFXML_API XmlTokenBuffer {
        private:
            struct Block;

        public:
            /// Typedef representing a character string whose encoding type is unknown
            typedef intptr_t ssize_t; // To do: Switch this from intptr_t to EASTL::ssize_t after EASTL 1.04.00 becomes widely available.

            static const size_t kBlockSizeDefault = 0x1000;  // 4K

            //////////////////////////////////////////////////////////////////////////
            /// Bookmark
            /*class Bookmark {
            private:
                friend class XmlTokenBuffer;
                Block             * mpCurrentBlock;
                char              * mpCurrentBlockEnd;
                char              * mpCurrentTokenBegin;
                char              * mpCurrentTokenPos;
            };*/

            //////////////////////////////////////////////////////////////////////////
            XmlTokenBuffer( EA::Allocator::ICoreAllocator * pAllocator, size_t blockSize = 0 );
            ~XmlTokenBuffer();

            //////////////////////////////////////////////////////////////////////////
            /// Clear the buffer (keeps 1 block)
            void Clear();

            //////////////////////////////////////////////////////////////////////////
            /// Specify the block size for future buffer allocations.
            void SetBlockSize(size_t blockSize) {
                mBlockSize = blockSize;
            }

            size_t GetBlockSize() const {
                return mBlockSize;
            }

            //////////////////////////////////////////////////////////////////////////
            /// Append a single character to the buffer, encoded as UTF-8
            void AppendEncodedChar( uint32_t c );

            //////////////////////////////////////////////////////////////////////////
            /// Append a byte (unencoded) to the buffer
            void AppendByte( uint8_t b ) {
                if (EA_UNLIKELY( mpCurrentTokenPos + 1 > mpCurrentBlockEnd )) {
                    IncreaseCapacity( 1 );
                }

                *mpCurrentTokenPos++ = (char)b;
            }

            //////////////////////////////////////////////////////////////////////////
            /// Append two bytes (unencoded) to the buffer
            void AppendBytes( uint8_t b0, uint8_t b1 ) {
                if (EA_UNLIKELY( mpCurrentTokenPos + 2 > mpCurrentBlockEnd )) {
                    IncreaseCapacity( 2 );
                }

                *mpCurrentTokenPos++ = (char)b0;
                *mpCurrentTokenPos++ = (char)b1;
            }

            //////////////////////////////////////////////////////////////////////////
            /// Append three bytes (unencoded) to the buffer
            void AppendBytes( uint8_t b0, uint8_t b1, uint8_t b2 ) {
                if (EA_UNLIKELY( mpCurrentTokenPos + 3 > mpCurrentBlockEnd )) {
                    IncreaseCapacity( 3 );
                }

                *mpCurrentTokenPos++ = (char)b0;
                *mpCurrentTokenPos++ = (char)b1;
                *mpCurrentTokenPos++ = (char)b2;
            }

            //////////////////////////////////////////////////////////////////////////
            /// Append four bytes (unencoded) to the buffer
            void AppendBytes( uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3 ) {
                if (EA_UNLIKELY( mpCurrentTokenPos + 4 > mpCurrentBlockEnd )) {
                    IncreaseCapacity( 4 );
                }

                *mpCurrentTokenPos++ = (char)b0;
                *mpCurrentTokenPos++ = (char)b1;
                *mpCurrentTokenPos++ = (char)b2;
                *mpCurrentTokenPos++ = (char)b3;
            }

            //////////////////////////////////////////////////////////////////////////
            /// Append bytes (unencoded) to the buffer
            void AppendBytes( const uint8_t * bytes, ssize_t nLen ) {
                if (EA_UNLIKELY( mpCurrentTokenPos + nLen > mpCurrentBlockEnd )) {
                    IncreaseCapacity( nLen );
                }

                memcpy( mpCurrentTokenPos, bytes, (size_t)nLen );
                mpCurrentTokenPos += nLen;
            }

            //////////////////////////////////////////////////////////////////////////
            /// Add a terminating NUL to the token and return its address.
            const char * FinishToken() {
                AppendByte( 0 );

                const char * pResult = mpCurrentTokenBegin;
                mpCurrentTokenBegin = mpCurrentTokenPos;
                return pResult;
            }

            //////////////////////////////////////////////////////////////////////////
            /// Return the current token length. Not valid after FinishToken() is called.
            ssize_t TokenLength() const { return mpCurrentTokenPos - mpCurrentTokenBegin; }

            //////////////////////////////////////////////////////////////////////////
            /// Save the current token in progress, and start a new token.
            /*void SaveBookmark( Bookmark & bm ) {
                bm.mpCurrentBlock = mpCurrentBlock;
                bm.mpCurrentBlockEnd = mpCurrentBlockEnd;
                bm.mpCurrentTokenBegin = mpCurrentTokenBegin;
                bm.mpCurrentTokenPos = mpCurrentTokenPos;

                // Start a new token.
                mpCurrentTokenBegin = mpCurrentTokenPos;
            }*/

            //////////////////////////////////////////////////////////////////////////
            /// Restore a bookmark
            /*void RestoreBookmark( const Bookmark & bm );*/

            //////////////////////////////////////////////////////////////////////////
            /// Save the current token in progress, and start a new token.
            void PushBookmark();

            //////////////////////////////////////////////////////////////////////////
            /// Restore the memory to the state just after the bookmark was pushed,
            /// but don't pop the bookmark.
            void RestoreBookmark();

            //////////////////////////////////////////////////////////////////////////
            /// Restore a bookmark
            void PopBookmark();

        private:
            void IncreaseCapacity( ssize_t size );

            typedef EA::Allocator::ICoreAllocator ICoreAllocator;

            struct Block {
                Block      * mpPrev;
                char       * mpBegin;
                char       * mpEnd;
            };

            struct Bookmark {
                Bookmark   * mpNext;
                char       * mpCurrentTokenBegin;
                char       * mpCurrentTokenPos;
            };

            ICoreAllocator * mpAllocator;
            Block          * mpCurrentBlock;
            char           * mpCurrentBlockEnd;
            char           * mpCurrentTokenBegin;
            char           * mpCurrentTokenPos;
            Bookmark       * mpBookmark;
            size_t           mBlockSize;
        };
    }
}

#endif // Header include guard

