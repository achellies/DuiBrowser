/*
Copyright (C) 2007,2009-2010 Electronic Arts, Inc.  All rights reserved.

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
// EATextBitmap.h
// By Paul Pedriana - 2007
/////////////////////////////////////////////////////////////////////////////


#ifndef EATEXT_INTERNAL_EATEXTBITMAP_H
#define EATEXT_INTERNAL_EATEXTBITMAP_H


#include <EABase/eabase.h>
#include EA_ASSERT_HEADER


namespace EA
{
    namespace Allocator
    {
        class ICoreAllocator;
    }


    namespace Text
    {

        ///////////////////////////////////////////////////////////////////////
        // Bitmap32
        //
        // Implements a 32 bpp pixel map. Typically it is ARGB.
        // The coordinate system for the bitmap is assumed to be one with the 
        // origin (0,0) being at the top-left of the image at pixel address 0 and 
        // the end (w,h) being at the bottom-rigth of the image at pixel address
        // (capacity-1).
        //
        class Bitmap32
        {
        public:
            typedef uint32_t Pixel;

            Bitmap32(Allocator::ICoreAllocator* pCoreAllocator = NULL, int w = 0, int h = 0);
           ~Bitmap32();

            bool   Init(int w, int h);
            void   Shutdown();

            bool   SetSize(int w, int h);
            int    GetWidth() const;
            int    GetHeight() const;
            int    GetStride() const;
            Pixel* GetData() const;
            Pixel* GetPixelPtr(int x, int y) const;
            Pixel* GetPixelPtrChecked(int x, int y) const;
            Pixel  GetPixel(int x, int y) const;
           Pixel  GetPixelClamped(int x, int y) const;
            void   Clear();

            #if EATEXT_DEBUG
                bool WritePPMFile(const char* pPath, Pixel backgroundColor = 0xff000000);
            #endif

        public:
            Pixel* mpData;       /// ARGB (A = high byte, B = low byte).
            int    mnWidth;      /// Bitmap32 row width in pixels.
            int    mnHeight;     /// Bitmap32 column height in pixels.
            int    mnStride;     /// Bitmap32 row width in bytes.
            int    mnCapacity;   /// Number of pixels we can hold, equal to the number of Pixel elements allcoated to mpData.
            Allocator::ICoreAllocator* mpCoreAllocator;
        };


        ///////////////////////////////////////////////////////////////////////
        // Bitmap8
        //
        // Implements an 8 bpp gray map, transparency map, or luminescence map.
        // The coordinate system for the bitmap is assumed to be one with the 
        // origin (0,0) being at the top-left of the image at pixel address 0 and 
        // the end (w,h) being at the bottom-rigth of the image at pixel address
        // (capacity-1).
        //
        class Bitmap8
        {
        public:
            typedef uint8_t Pixel;

            Bitmap8(Allocator::ICoreAllocator* pCoreAllocator = NULL, int w = 0, int h = 0);
           ~Bitmap8();

            bool   Init(int w, int h);
            void   Shutdown();

            bool   SetSize(int w, int h);
            void   Acquire(Pixel* pData, int w, int h, int stride);

            int    GetWidth() const;
            int    GetHeight() const;
            int    GetStride() const;
            Pixel* GetData() const;

            Pixel* GetPixelPtr(int x, int y) const;
            Pixel* GetPixelPtrChecked(int x, int y) const;
            Pixel  GetPixel(int x, int y) const;
            void   Clear();

            #if EATEXT_DEBUG
                bool WritePPMFile(const char* pPath, Pixel backgroundColor = 0x00);
            #endif

        public:
            Pixel* mpData;       /// ARGB (A = high byte, B = low byte).
            int    mnWidth;      /// Bitmap32 row width in pixels.
            int    mnHeight;     /// Bitmap32 column height in pixels.
            int    mnStride;     /// Bitmap32 row width in bytes.
            int    mnCapacity;   /// Number of pixels we can hold, equal to the number of Pixel elements allcoated to mpData.
            Allocator::ICoreAllocator* mpCoreAllocator;
        };

    } // namespace Text

} // namespace EA






///////////////////////////////////////////////////////////////////////////////
// Inlines
///////////////////////////////////////////////////////////////////////////////

namespace EA
{
    namespace Text
    {
        ///////////////////////////////////////////////////////////////////////
        // Bitmap32
        ///////////////////////////////////////////////////////////////////////

        inline Bitmap32::~Bitmap32()
        {
            Shutdown();
        }


        inline int Bitmap32::GetWidth() const
        {
            return mnWidth;
        }


        inline int Bitmap32::GetHeight() const
        {
            return mnHeight;
        }


        inline int Bitmap32::GetStride() const
        {
            return mnStride;
        }


        inline Bitmap32::Pixel* Bitmap32::GetData() const
        {
            return mpData;
        }


        inline Bitmap32::Pixel* Bitmap32::GetPixelPtr(int x, int y) const
        {
            EA_ASSERT((x >= 0) && (y >= 0) && (x < mnWidth) && (y < mnHeight));
            return mpData + (y * mnWidth) + x;
        }


        inline Bitmap32::Pixel* Bitmap32::GetPixelPtrChecked(int x, int y) const
        {
            if((x >= 0) && (y >= 0) && (x < mnWidth) && (y < mnHeight))
                return mpData + (y * mnWidth) + x;
            return NULL;
        }


        inline Bitmap32::Pixel Bitmap32::GetPixel(int x, int y) const
        {
            const Pixel* const pPixel = GetPixelPtr(x, y);
            return *pPixel;
        }


        ///////////////////////////////////////////////////////////////////////
        // Bitmap8
        ///////////////////////////////////////////////////////////////////////

        inline Bitmap8::~Bitmap8()
        {
            Shutdown();
        }

        inline int Bitmap8::GetWidth() const
        {
            return mnWidth;
        }


        inline int Bitmap8::GetHeight() const
        {
            return mnHeight;
        }


        inline int Bitmap8::GetStride() const
        {
            return mnStride;
        }


        inline Bitmap8::Pixel* Bitmap8::GetData() const
        {
            return mpData;
        }


        inline Bitmap8::Pixel* Bitmap8::GetPixelPtr(int x, int y) const
        {
            EA_ASSERT((x >= 0) && (y >= 0) && (x < mnWidth) && (y < mnHeight));
            return mpData + (y * mnWidth) + x;
        }

        inline Bitmap8::Pixel* Bitmap8::GetPixelPtrChecked(int x, int y) const
        {
            if((x >= 0) && (y >= 0) && (x < mnWidth) && (y < mnHeight))
                return mpData + (y * mnWidth) + x;
            return NULL;
        }


        inline Bitmap8::Pixel Bitmap8::GetPixel(int x, int y) const
        {
            const Pixel* const pPixel = GetPixelPtr(x, y);
            return *pPixel;
        }

    } // namespace Text

} // namespace EA


#endif // EATEXT_INTERNAL_EATEXTBITMAP_H

















