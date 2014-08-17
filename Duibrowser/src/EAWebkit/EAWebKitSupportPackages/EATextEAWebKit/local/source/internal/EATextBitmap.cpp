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
// EATextBitmap.cpp
// By Paul Pedriana - 2007
/////////////////////////////////////////////////////////////////////////////


#include <EAText/EATextConfig.h>
#include <EAText/EAText.h>
#include <EAText/internal/EATextBitmap.h>
#include <coreallocator/icoreallocator_interface.h>
#include <EASTL/algorithm.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include EA_ASSERT_HEADER



namespace EA
{

namespace Text
{

namespace Internal
{
    template <typename T>
    inline void swap(T& a, T& b)
    {
        T temp(a);
        a = b;
        b = temp;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Bitmap32
///////////////////////////////////////////////////////////////////////////////

Bitmap32::Bitmap32(Allocator::ICoreAllocator* pCoreAllocator, int w, int h)
  : mpData(NULL),
    mnWidth(w),
    mnHeight(h),
    mnStride(w),
    mnCapacity(0),
    mpCoreAllocator(pCoreAllocator ? pCoreAllocator : EA::Text::GetAllocator())
{
    Init(w, h);
}

bool Bitmap32::Init(int w, int h)
{
    Shutdown();
    return SetSize(w, h);
}

void Bitmap32::Shutdown()
{
    if(mpCoreAllocator)
        mpCoreAllocator->Free(mpData);

    mpData     = NULL;
    mnWidth    = 0;
    mnHeight   = 0;
    mnStride   = 0;
    mnCapacity = 0;
}

bool Bitmap32::SetSize(int w, int h)
{
    EA_ASSERT((w >= 0) && (h >= 0));

    if((w * h) > mnCapacity) // If we need to reallocate...
    {
        if(mpCoreAllocator)
        {
            if(mpData)
                mpCoreAllocator->Free(mpData);

            mnCapacity  = w * h;
            mpData      = (Pixel*)mpCoreAllocator->Alloc(mnCapacity * sizeof(Pixel), EATEXT_ALLOC_PREFIX "Bitmap8", 0);
            EA_ASSERT(mpData != NULL);
        }
    }

    mnWidth  = w;
    mnHeight = h;
    mnStride = w * sizeof(Pixel);

    return (mpData != NULL);
}

Bitmap32::Pixel Bitmap32::GetPixelClamped(int x, int y) const
{
    if(x < 0)
        x = 0;
    else if(x >= mnWidth)
        x = mnWidth - 1;

    if(y < 0)
        y = 0;
    else if (y >= mnHeight)
        y = mnHeight - 1;

    return mpData[(y * mnWidth) + x];
}

void Bitmap32::Clear()
{
    Pixel* pDstRow = mpData;

    for(int y = 0; y < mnHeight; y++)
    {
        memset(pDstRow, 0, mnWidth * sizeof(Pixel));
        pDstRow += mnStride / sizeof(Pixel);
    }
}

#if EATEXT_DEBUG
    bool Bitmap32::WritePPMFile(const char* pPath, Pixel d)
    {
        // See http://netpbm.sourceforge.net/doc/ppm.html
        // PPM supports alpha somehow, but it's not well documented.

	    FILE* const fp = fopen(pPath, "w");

        if(fp)
        {
	        fprintf(fp, "P3\n");
		    fprintf(fp, "# %s\n", pPath);
	        fprintf(fp, "%d %d\n", mnWidth, mnHeight);
	        fprintf(fp, "%d\n", 255);

	        for(int y = 0; y < mnHeight; y++)
	        {
		        for(int x = 0; x < mnWidth; x++)
		        {
			        const Pixel s = GetPixel(x, y);

                    if((s >> 24) == 0x00) // If source is transparent (which will often be so)...
                    {
                        const int32_t r = ((d >> 16) & 0xff);
                        const int32_t g = ((d >>  8) & 0xff);
                        const int32_t b = ((d >>  0) & 0xff);

    			        fprintf(fp, "%03d %03d %03d \t", (int)r, (int)g, (int)b);
                    }
                    else
                    {
                        const float sa = ((s >> 24) & 0xff) / 255.f;
                        const float sr = ((s >> 16) & 0xff) / 255.f;
                        const float sg = ((s >>  8) & 0xff) / 255.f;
                        const float sb = ((s >>  0) & 0xff) / 255.f;

                        const float dr = ((d >> 16) & 0xff) / 255.f;
                        const float dg = ((d >>  8) & 0xff) / 255.f;
                        const float db = ((d >>  0) & 0xff) / 255.f;

                        const int32_t r = (int32_t)(255 * ((dr * (1.f - sa)) + (sr * sa)));
                        const int32_t g = (int32_t)(255 * ((dg * (1.f - sa)) + (sg * sa)));
                        const int32_t b = (int32_t)(255 * ((db * (1.f - sa)) + (sb * sa)));

    			        fprintf(fp, "%03d %03d %03d \t", (int)r, (int)g, (int)b);
                    }
		        }

		        fprintf(fp, "\n");
	        }

	        fprintf(fp, "\n");
	        fclose(fp);

            return true;
        }

        return false;
    }
#endif

///////////////////////////////////////////////////////////////////////////////
// Bitmap8
///////////////////////////////////////////////////////////////////////////////

Bitmap8::Bitmap8(Allocator::ICoreAllocator* pCoreAllocator, int w, int h)
  : mpData(NULL),
    mnWidth(w),
    mnHeight(h),
    mnStride(w),
    mnCapacity(0),
    mpCoreAllocator(pCoreAllocator ? pCoreAllocator : EA::Text::GetAllocator())
{
    Init(w, h);
}

bool Bitmap8::Init(int w, int h)
{
    Shutdown();
    return SetSize(w, h);
}

void Bitmap8::Shutdown()
{
    if(mpCoreAllocator)
        mpCoreAllocator->Free(mpData);

    mpData     = NULL;
    mnWidth    = 0;
    mnHeight   = 0;
    mnStride   = 0;
    mnCapacity = 0;
}


bool Bitmap8::SetSize(int w, int h)
{
    EA_ASSERT((w >= 0) && (h >= 0));

    if((w * h) > mnCapacity) // If we need to reallocate...
    {
        if(mpCoreAllocator)
        {
            if(mpData)
                mpCoreAllocator->Free(mpData);

            mnCapacity  = w * h;
            mpData      = (Pixel*)mpCoreAllocator->Alloc(mnCapacity * sizeof(Pixel), EATEXT_ALLOC_PREFIX "Bitmap8", 0);
            EA_ASSERT(mpData != NULL);
        }
    }

    mnWidth  = w;
    mnHeight = h;
    mnStride = w * sizeof(Pixel);

    return (mpData != NULL);
}

void Bitmap8::Clear()
{
    Pixel* pDstRow = mpData;

    for(int y = 0; y < mnHeight; y++)
    {
        memset(pDstRow, 0, mnWidth * sizeof(Pixel));
        pDstRow += mnStride / sizeof(Pixel);
    }
}

#if EATEXT_DEBUG
    bool Bitmap8::WritePPMFile(const char* pPath, Pixel /*d*/)
    {
        // See http://netpbm.sourceforge.net/doc/ppm.html
        // PPM supports alpha somehow, but it's not well documented.

	    FILE* const fp = fopen(pPath, "w");

        if(fp)
        {
	        fprintf(fp, "P3\n");
		    fprintf(fp, "# %s\n", pPath);
	        fprintf(fp, "%d %d\n", mnWidth, mnHeight);
	        fprintf(fp, "%d\n", 255);

	        for(int y = 0; y < mnHeight; y++)
	        {
		        for(int x = 0; x < mnWidth; x++)
		        {
			        const Pixel s = GetPixel(x, y);
			        fprintf(fp, "%03d %03d %03d \t", (int)s, (int)s, (int)s);
		        }

		        fprintf(fp, "\n");
	        }

	        fprintf(fp, "\n");
	        fclose(fp);

            return true;
        }

        return false;
    }
#endif



} // namespace Text

} // namespace EA

