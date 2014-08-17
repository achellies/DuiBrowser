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
// EARasterBlit.cpp
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Portions of this file are based on SDL.
//
// SDL - Simple DirectMedia Layer
// Copyright (C) 1997-2006 Sam Lantinga
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
///////////////////////////////////////////////////////////////////////////////


#include <EARaster/EARaster.h> 
#include "EARaster/internal/EARasterUtils.h"
#include <EAWebKit/internal/EAWebKitAssert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <EAWebKit/EAWebKitView.h>
#ifndef MMX_ASMBLIT
    #if defined(_MSC_VER) && defined(_M_IX86) && !defined(_WIN64)
        #define MMX_ASMBLIT 1
    #else
        #define MMX_ASMBLIT 0
    #endif
#endif

#ifndef MSVC_ASMBLIT
    #if defined(_MSC_VER) && defined(_M_IX86) && !defined(_WIN64)
        #define MSVC_ASMBLIT 1
    #else
        #define MSVC_ASMBLIT 0
    #endif
#endif

#ifndef GCC_ASMBLIT
    #define GCC_ASMBLIT 0
#endif

#ifndef ALTIVEC_BLIT
    #if (defined(__GNUC__) && defined(__PPU__))
        #define ALTIVEC_BLIT 1
    #else
        #define ALTIVEC_BLIT 0
    #endif
#endif


#if GCC_ASMBLIT
    #include "mmx.h"
#elif MSVC_ASMBLIT
    #include <mmintrin.h>
    #include <mm3dnow.h>
#elif ALTIVEC_BLIT
    #include <altivec.h>
#endif




namespace EA {

namespace Raster {


static void Blit4to4MaskAlpha       (const BlitInfo& info);
static void BlitNtoN                (const BlitInfo& info);
static void BlitNtoNCopyAlpha       (const BlitInfo& info);
static void BlitCopy                (const BlitInfo& info);
static void BlitCopyOverlap         (const BlitInfo& info);
static void BlitRGBtoRGBSurfaceAlpha(const BlitInfo& info);
static void BlitRGBtoRGBPixelAlpha  (const BlitInfo& info);
static void BlitNtoNSurfaceAlpha    (const BlitInfo& info);
static void BlitNtoNPixelAlpha      (const BlitInfo& info);

#if MSVC_ASMBLIT
    static void BlitRGBtoRGBSurfaceAlphaMMX   (const BlitInfo& info);
    static void BlitRGBtoRGBPixelAlphaMMX     (const BlitInfo& info);
    static void BlitRGBtoRGBPixelAlphaMMX3DNOW(const BlitInfo& info);
#endif

#if ALTIVEC_BLIT 
    static void Blit32to32PixelAlphaAltivec    (const BlitInfo& info);
    static void BlitRGBtoRGBPixelAlphaAltivec  (const BlitInfo& info);
    static void Blit32to32SurfaceAlphaAltivec  (const BlitInfo& info);
    static void BlitRGBtoRGBSurfaceAlphaAltivec(const BlitInfo& info);
#endif



void* revcpy(void* pDst, const void* pSrc, size_t len)
{
    const char* pSrc8 = (const char*)pSrc + (len - 1);
    char*       pDst8 = (char*)      pDst + (len - 1);

    while (len--)
        *pDst8-- = *pSrc8--;

    return pDst;
}


#if MMX_ASMBLIT

    static int CPUIDFeatures()
    {
        int features = 0;

        #if (defined(_MSC_VER) && defined(_M_IX86))
            __asm {
                xor     eax, eax            ; Set up for CPUID instruction
                cpuid                       ; Get and save vendor ID
                cmp     eax, 1              ; Make sure 1 is valid input for CPUID
                jl      done                ; We dont have the CPUID instruction
                xor     eax, eax
                inc     eax
                cpuid                       ; Get family/model/stepping/features
                mov     features, edx
                done:
            }
        #endif

        return features;
    }

    static int CPUIDFeaturesEx()
    {
        int features = 0;

        #if (defined(_MSC_VER) && defined(_M_IX86))
            __asm {
                mov     eax,80000000h       ; Query for extended functions
                cpuid                       ; Get extended function limit
                cmp     eax,80000001h
                jl      done                ; Nope, we dont have function 800000001h
                mov     eax,80000001h       ; Setup extended function 800000001h
                cpuid                       ; and get the information
                mov     features,edx
                done:
            }
         #endif

        return features;
    }

    static bool HaveMMX()
    {
        return (CPUIDFeatures() & 0x00800000);
    }

    // static bool HaveMMXExt()
    // {
    //     return (CPUIDFeaturesEx() & 0x00400000);
    // }

    static bool Have3DNow()
    {
        return (CPUIDFeaturesEx() & 0x80000000);
    }

    // static bool Have3DNowExt()
    // {
    //     return (CPUIDFeaturesEx() & 0x40000000);
    // }

    // static bool HaveSSE()
    // {
    //     return (CPUIDFeatures() & 0x02000000);
    // }

    // static bool HaveSSE2()
    // {
    //     return (CPUIDFeatures() & 0x04000000);
    // }

#endif


#if ALTIVEC_BLIT

    static bool HaveAltiVec()
    {
        #if defined(EA_PROCESSOR_POWERPC)
            return true;
        #else
            return false;
        #endif
    }

#endif


// 8-times unrolled loop
#define DUFFS_LOOP8(pixel_copy_increment, width)        \
{                                                       \
    int n = (width + 7) / 8;                            \
    switch (width & 7) {                                \
    case 0: do {   pixel_copy_increment;                \
    case 7:        pixel_copy_increment;                \
    case 6:        pixel_copy_increment;                \
    case 5:        pixel_copy_increment;                \
    case 4:        pixel_copy_increment;                \
    case 3:        pixel_copy_increment;                \
    case 2:        pixel_copy_increment;                \
    case 1:        pixel_copy_increment;                \
        } while (--n > 0);                              \
    }                                                   \
}


// 4-times unrolled loop
#define DUFFS_LOOP4(pixel_copy_increment, width)        \
{                                                       \
    int n = (width + 3) / 4;                            \
    switch (width & 3) {                                \
    case 0: do {   pixel_copy_increment;                \
    case 3:        pixel_copy_increment;                \
    case 2:        pixel_copy_increment;                \
    case 1:        pixel_copy_increment;                \
        } while (--n > 0);                              \
    }                                                   \
}


#define DUFFS_LOOP(pixel_copy_increment, width)         \
    DUFFS_LOOP8(pixel_copy_increment, width)


// 2 - times unrolled loop
#define DUFFS_LOOP_DOUBLE2(pixel_copy_increment, double_pixel_copy_increment, width) \
{                                                       \
    int n, w = width;                                   \
    if (w & 1) {                                        \
        pixel_copy_increment;                           \
        w--;                                            \
    }                                                   \
    if (w > 0) {                                        \
        n = (w + 2) / 4;                                \
        switch(w & 2) {                                 \
        case 0: do {   double_pixel_copy_increment;     \
        case 2:        double_pixel_copy_increment;     \
            } while (--n > 0);                          \
        }                                               \
    }                                                   \
}


#define RGB_FROM_PIXEL(pixel, fmt, r, g, b)         \
{                                                   \
    r = ((pixel & fmt.mRMask) >> fmt.mRShift);      \
    g = ((pixel & fmt.mGMask) >> fmt.mGShift);      \
    b = ((pixel & fmt.mBMask) >> fmt.mBShift);      \
}


#define DISEMBLE_RGB(buf, bpp, fmt, pixel, r, g, b)     \
do {                                                    \
    switch (bpp)                                        \
    {                                                   \
        case 3:                                         \
        {                                               \
            uint8_t* p = (uint8_t*)buf;                 \
            pixel = (p[0] << 16) + (p[1] << 8) + p[2];  \
        }                                               \
        break;                                          \
                                                        \
        case 4:                                         \
        default:                                        \
            pixel = *((uint32_t*)(buf));                \
            break;                                      \
                                                        \
    }                                                   \
    RGB_FROM_PIXEL(pixel, fmt, r, g, b);                \
} while(0)


// Blend the RGB values of two pixels based on a source alpha value.
#define ALPHA_BLEND(sR, sG, sB, A, dR, dG, dB)      \
do {                                                \
    dR = (((sR - dR) * (A)) >> 8) + dR;             \
    dG = (((sG - dG) * (A)) >> 8) + dG;             \
    dB = (((sB - dB) * (A)) >> 8) + dB;             \
} while(0)


// Additive blend the RGB values of two pixels based on a source alpha value.
#define ADDITIVE_ALPHA_BLEND_RGB(sR, sG, sB, A, dR, dG, dB)     \
do {                                                        \
    unsigned tA;                                            \
    tA = 255 - A;                                           \
    dR = sR + DivideBy255Rounded(dR * tA);                  \
    dG = sG + DivideBy255Rounded(dG * tA);                  \
    dB = sB + DivideBy255Rounded(dB * tA);                  \
} while(0)

#define ADDITIVE_ALPHA_BLEND_ARGB(sR, sG, sB, sA, dR, dG, dB, dA)\
    do {                                                    \
    unsigned tA;                                            \
    tA = 255 - sA;                                          \
    dA = sA + DivideBy255Rounded(dA * tA);                  \
    dR = sR + DivideBy255Rounded(dR * tA);                  \
    dG = sG + DivideBy255Rounded(dG * tA);                  \
    dB = sB + DivideBy255Rounded(dB * tA);                  \
    } while(0)


// Blend the RGB values of two Pixels based on a source alpha value.
#define ACCURATE_ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB)    \
do {                                                        \
    unsigned tR, tG, tB, tA;                                \
    tA = 255 - sA;                                          \
    tR = 1 + (sR * sA) + (dR * tA);                         \
    dR = (tR + (tR >> 8)) >> 8;                             \
    tG = 1 + (sG * sA) + (dG * tA);                         \
    dG = (tG + (tG >> 8)) >> 8;                             \
    tB = 1 + (sB * sA) + (dB * tA);                         \
    dB = (tB + (tB >> 8)) >> 8;                             \
} while(0)


#define PIXEL_FROM_RGBA(pixel, fmt, r, g, b, a)     \
{                                                   \
    pixel = (r << fmt.mRShift) |                    \
            (g << fmt.mGShift) |                    \
            (b << fmt.mBShift) |                    \
            (a << fmt.mAShift);                     \
}


#define RGBA_FROM_PIXEL(pixel, fmt, r, g, b, a)     \
{                                                   \
    r = ((pixel & fmt.mRMask) >> fmt.mRShift);      \
    g = ((pixel & fmt.mGMask) >> fmt.mGShift);      \
    b = ((pixel & fmt.mBMask) >> fmt.mBShift);      \
    a = ((pixel & fmt.mAMask) >> fmt.mAShift);      \
}


#define ASSEMBLE_RGBA(buf, bpp, fmt, r, g, b, a)        \
{                                                       \
    switch (bpp)                                        \
    {                                                   \
        case 3:                                         \
            *((buf) + fmt.mRShift / 8) = r;             \
            *((buf) + fmt.mGShift / 8) = g;             \
            *((buf) + fmt.mBShift / 8) = b;             \
            break;                                      \
                                                        \
        case 4:                                         \
        {                                               \
            uint32_t pixel;                             \
            PIXEL_FROM_RGBA(pixel, fmt, r, g, b, a);    \
            *(uint32_t*)(buf) = pixel;                  \
            break;                                      \
        }                                               \
    }                                                   \
}


#define DISEMBLE_RGBA(buf, bpp, fmt, pixel, r, g, b, a) \
do {                                                    \
    switch (bpp)                                        \
    {                                                   \
        case 3:                                         \
        {                                               \
            uint8_t* p = (uint8_t*)(buf);               \
            pixel = p[0] + (p[1] << 8) + (p[2] << 16);  \
        }                                               \
        break;                                          \
                                                        \
        default:                                        \
        case 4:                                         \
            pixel = *(uint32_t*)(buf);                  \
            break;                                      \
    }                                                   \
    RGBA_FROM_PIXEL(pixel, fmt, r, g, b, a);            \
    pixel &= ~fmt.mAMask;                               \
} while(0)


EARASTER_API bool ClipForBlit(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, 
                    Rect& rectSourceResult, Rect& rectDestResult)
{
    int srcX, srcY, w, h;

    EAW_ASSERT(pSource && pDest);

    // If the destination rectangle is NULL, use the entire dest surface.
    if(pRectDest)
        rectDestResult = *pRectDest;
    else
    {
        rectDestResult.x = 0;
        rectDestResult.y = 0;
        // It turns out that due to the logic below we don't need to set w/h now.
    }

    // Clip the source rectangle to the source surface.
    if(pRectSource)
    {    
        srcX = pRectSource->x;
        w    = pRectSource->w;

        if(srcX < 0)
        {
            w += srcX;
            rectDestResult.x -= srcX;
            srcX = 0;
        }

        int maxW = pSource->GetWidth() - srcX;

        if(maxW < w)
            w = maxW;

        srcY = pRectSource->y;
        h    = pRectSource->h;

        if(srcY < 0)
        {
            h += srcY;
            rectDestResult.y -= srcY;
            srcY = 0;
        }

        int maxH = pSource->GetHeight() - srcY;

        if(maxH < h)
            h = maxH;
    }
    else
    {
        srcX = 0;
        srcY = 0;
        w    = pSource->GetWidth();
        h    = pSource->GetHeight();
    }

    // Clip the destination rectangle against the clip rectangle
    {
        const Rect& rectClip = pDest->GetClipRect();

        int dx = rectClip.x - rectDestResult.x;

        if(dx > 0)
        {
            w -= dx;
            rectDestResult.x += dx;
            srcX += dx;
        }

        dx = (rectDestResult.x + w) - (rectClip.x + rectClip.w);

        if(dx > 0)
            w -= dx;

        int dy = rectClip.y - rectDestResult.y;

        if(dy > 0)
        {
            h -= dy;
            rectDestResult.y += dy;
            srcY += dy;
        }

        dy = (rectDestResult.y + h) - (rectClip.y + rectClip.h);

        if(dy > 0)
            h -= dy;
    }

    if(w > 0 && h > 0)
    {
        rectSourceResult.x = srcX;
        rectSourceResult.y = srcY;
        rectSourceResult.w = w;
        rectSourceResult.h = h;

        rectDestResult.w = w;
        rectDestResult.h = h;

        return true;
    }

    // To consider: enable this.
    // rectDestResult.w = 0;
    // rectDestResult.h = 0;

    return false;
}


EARASTER_API int BlitTiled(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, int offsetX, int offsetY)
{
    Rect rectSource;  // Used if pRectSource is NULL.
    Rect rectDest;

    if(!pRectSource)
    {
        rectSource.x = 0;
        rectSource.y = 0;
        rectSource.w = pSource->GetWidth();
        rectSource.h = pSource->GetHeight();

        pRectSource = &rectSource;
    }

    if(!pRectDest)
    {
        rectDest.x = 0;
        rectDest.y = 0;
        rectDest.w = pDest->GetWidth();
        rectDest.h = pDest->GetHeight();

        pRectDest = &rectDest;
    }

    const int imageW = pRectSource->w;
    const int imageH = pRectSource->h;

    EAW_ASSERT((offsetX <= imageW) && (offsetY <= imageH));  // The user must pass in offsets that are in the range of [0, image size]

    // Convert offsetX to the starting destination position of our blit.
    if(offsetX > 0)
        offsetX -= imageW; // offsetX will be negative.

    if(offsetY > 0)
        offsetY -= imageH; // offsetY will be negative.

    // We stamp pSource into pDest a bunch of times to cover the destination area. 
    for(int y = (pRectDest->y + offsetY), yEnd = (y + pRectDest->h); y < yEnd; y += imageH)
    {
        for(int x = (pRectDest->x + offsetX), xEnd = (x + pRectDest->w); x < xEnd; x += imageW)
        {
            const Rect rectDestBlit(x, y, imageW, imageH);
            Blit(pSource, pRectSource, pDest, &rectDestBlit, pRectDest);
        }
    }

    return 0; // To do: Need to make a sensible return value.
}


EARASTER_API int BlitEdgeTiled(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, const Rect* pRectSourceCenter)
{
    Rect rectSource;  // Used if pRectSource is NULL.
    Rect rectDest;
    Rect rectSourceCenter;

    if(!pRectSource)
    {
        rectSource.x = 0;
        rectSource.y = 0;
        rectSource.w = pSource->GetWidth();
        rectSource.h = pSource->GetHeight();

        pRectSource = &rectSource;
    }

    if(!pRectDest)
    {
        rectDest.x = 0;
        rectDest.y = 0;
        rectDest.w = pDest->GetWidth();
        rectDest.h = pDest->GetHeight();

        pRectDest = &rectDest;
    }

    if(!pRectSourceCenter)
    {
        // We assume the pRectSource is divided into 9 (3x3) equal squares.
        rectSourceCenter.x = pRectSource->x + (pRectSource->w / 3);
        rectSourceCenter.y = pRectSource->y + (pRectSource->h / 3);
        rectSourceCenter.w = pRectSource->w / 3;
        rectSourceCenter.h = pRectSource->h / 3;

        pRectSourceCenter = &rectSourceCenter;
    }

    // Before doing the blits, we precalculate values we'll need.
    const int source1X = pRectSource->x;                                                    // X of first part of source
    const int source1Y = pRectSource->y;
    const int source1W = pRectSourceCenter->x - pRectSource->x;                             // Width of first part of source
    const int source1H = pRectSourceCenter->y - pRectSource->y;
    const int source2X = source1X + source1W;                                               // X of second part of source
    const int source2Y = source1Y + source1H;
    const int source2W = pRectSourceCenter->w;                                              // Width of second part of source
    const int source2H = pRectSourceCenter->h;
    const int source3X = source2X + source2W;                                               // X of third part of source
    const int source3Y = source2Y + source2H;
    const int source3W = pRectSource->w - (pRectSourceCenter->x + pRectSourceCenter->w);    // Width of third part of source
    const int source3H = pRectSource->h - (pRectSourceCenter->y + pRectSourceCenter->h);

    const int dest1X = pRectDest->x;
    const int dest1Y = pRectDest->y;
    const int dest1W = source1W;
    const int dest1H = source1H;
    const int dest2X = dest1X + dest1W;
    const int dest2Y = dest1Y + dest1H;
    const int dest2W = pRectDest->x + pRectDest->w - source3W;
    const int dest2H = pRectDest->y + pRectDest->h - source3W;
    const int dest3X = dest2X + dest2W;
    const int dest3Y = dest2Y + dest2H;
    const int dest3W = source3W;
    const int dest3H = source3H;

    // Top left corner
    const Rect rectSourceTL(source1X, source1Y, source1W, source1H);
    const Rect rectDestTL(dest1X, dest1Y, dest1W, dest1H);
    Blit(pSource, &rectSourceTL, pDest, &rectDestTL);

    // Top right corner
    const Rect rectSourceTR(source3X, source1Y, source3W, source1H);
    const Rect rectDestTR(dest3X, dest1Y, dest3W, dest1H);
    Blit(pSource, &rectSourceTR, pDest, &rectDestTR);

    // Bottom left corner
    const Rect rectSourceBL(source1X, source3Y, source1W, source3H);
    const Rect rectDestBL(dest1X, dest3Y, dest1W, dest3H);
    Blit(pSource, &rectSourceBL, pDest, &rectDestBL);

    // Bottom right corner
    const Rect rectSourceBR(source3X, source3Y, source3W, source3H);
    const Rect rectDestBR(dest3X, dest3Y, dest3W, dest3H);
    Blit(pSource, &rectSourceBR, pDest, &rectDestBR);

    // Top and bottom edges
    if(dest2W > 0)
    {
        // Top edge
        const Rect rectSourceT(source2X, source1Y, source2W, source1H);
        const Rect rectDestT(dest2X, dest1Y, dest2W, dest1H);
        BlitTiled(pSource, &rectSourceT, pDest, &rectDestT, 0, 0);

        // Bottom edge
        const Rect rectSourceB(source2X, source3Y, source2W, source3H);
        const Rect rectDestB(dest2X, dest3Y, dest2W, dest3H);
        BlitTiled(pSource, &rectSourceB, pDest, &rectDestB, 0, 0);
    }

    // Left and right edges
    if(dest2H > 0)
    {
        // Left edge
        const Rect rectSourceL(source1X, source2Y, source1W, source2H);
        const Rect rectDestL(dest1X, dest2Y, dest1W, dest2H);
        BlitTiled(pSource, &rectSourceL, pDest, &rectDestL, 0, 0);

        // Right edge
        const Rect rectSourceR(source3X, source2Y, source3W, source2H);
        const Rect rectDestR(dest3X, dest2Y, dest3W, dest2H);
        BlitTiled(pSource, &rectSourceR, pDest, &rectDestR, 0, 0);
    }

    // Center area
    if((dest2W > 0) && (dest2H > 0))
    {
        const Rect rectSourceC(source2X, source2Y, source2W, source2H);
        const Rect rectDestC(dest2X, dest2Y, dest2W, dest2H);
        BlitTiled(pSource, &rectSourceC, pDest, &rectDestC, 0, 0);
    }

    return 0;
}


EARASTER_API int Blit(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest, const Rect* pDestClipRect)
{
    EAW_ASSERT(pSource && pDest);

    // If the source rectangle is NULL, use the entire source surface.
    // If the destination rectangle is NULL, use the entire dest surface.

    Rect rectSourceResult;
    Rect rectDestResult;

    if(ClipForBlit(pSource, pRectSource, pDest, pRectDest, rectSourceResult, rectDestResult))  // If there is anything to draw...
    {
        if(pDestClipRect)
        {
            // Test left side of clip rect to left side of dest rect.
            int d = (pDestClipRect->x - rectDestResult.x);

            if(d > 0)
            {
                rectDestResult.x   += d;
                rectDestResult.w   -= d;
                rectSourceResult.x += d;
                rectSourceResult.w -= d;
            }

            // Test right side of clip rect to right side of dest rect.
            d = (rectDestResult.x + rectDestResult.w) - (pDestClipRect->x + pDestClipRect->w);

            if(d > 0)
            {
                rectDestResult.w   -= d;
                rectSourceResult.w -= d;
            }

            // Test top side of clip rect to top side of dest rect.
            d = (pDestClipRect->y - rectDestResult.y);

            if(d > 0)
            {
                rectDestResult.y   += d;
                rectDestResult.h   -= d;
                rectSourceResult.y += d;
                rectSourceResult.h -= d;
            }

            // Test bottom side of clip rect to bottom side of dest rect.
            d = (rectDestResult.y + rectDestResult.h) - (pDestClipRect->y + pDestClipRect->h);

            if(d > 0)
            {
                rectDestResult.h   -= d;
                rectSourceResult.h -= d;
            }

            if((rectSourceResult.w <= 0) || (rectSourceResult.h <= 0))
                return 0;
        }

        //Nicki Vankoughnett Stomp Fix:  
        //it appears that it is possible at this piont for the pRectDest to cause us to attempt to draw out
        //of bounds.  So lets constrain our pRectDest to the pDest surface mWidth and mHeight values.
        EA::Raster::Rect destSurfaceConstraint(0,0,pDest->GetWidth(), pDest->GetHeight());
        destSurfaceConstraint.constrainRect(rectDestResult);
        if(rectDestResult.w <= 0 || rectDestResult.h <= 0)
            return 0;

        return BlitNoClip(pSource, &rectSourceResult, pDest, &rectDestResult);
    }

    return 0;
}


EARASTER_API int BlitNoClip(ISurface* pSource, const Rect* pRectSource, ISurface* pDest, const Rect* pRectDest)
{
    Rect rectSource; // Used if pRectSource is NULL.
    Rect rectDest;   // Used if pRectDest is NULL.

    // See if we need to set up the blit function.
    // If we add the ability for the source or dest pixel format to change, 
    // then we'll need to add a change detection mechanism.
    if((pSource->GetBlitDest() != pDest) || !pSource->GetBlitFunction())
    {
        if(!SetupBlitFunction(pSource, pDest))
            return -1;
    }

    // 11/09/09 CSidhall Added notify start of process to user
	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawRaster, EA::WebKit::kVProcessStatusStarted);
	
	if(!pRectSource)
    {
        rectSource.x = 0;
        rectSource.y = 0;
        rectSource.w = pSource->GetWidth();
        rectSource.h = pSource->GetHeight();

        pRectSource = &rectSource;
    }

    if(!pRectDest)
    {
        rectDest.x = 0;
        rectDest.y = 0;
        rectDest.w = pDest->GetWidth();     // To do: Define the the behaviour for the case of (pRectSource->w != pRectDest->w).
        rectDest.h = pDest->GetHeight();

        pRectDest = &rectDest;
    }

    //Nicki Vankoughnett:
    //Avoid any chance of having a dest rect that cannot fit on the dest surface.
    EAW_ASSERT(pRectDest->x >=  0 && pRectDest->x <= pDest->GetWidth());
    EAW_ASSERT((pRectDest->x + pRectDest->w) >=  0 && (pRectDest->x + pRectDest->w) <= pDest->GetWidth());
    EAW_ASSERT(pRectDest->y >=  0 && pRectDest->y <= pDest->GetHeight());    // 3/10/09 CSidhall - Typo fix to mHeight instead of mWidth
    EAW_ASSERT((pRectDest->y + pRectDest->h) >=  0 && (pRectDest->y + pRectDest->h) <= pDest->GetHeight());

    // Blits are done through a BlitFunction pointer. 
    // The blit function takes a BlitInfo struct to encapsulate all parameters.
    BlitInfo blitInfo;

    blitInfo.mpSource  = pSource;
    blitInfo.mpSPixels = (uint8_t*)pSource->GetData() + (pRectSource->y * pSource->GetStride()) + (pRectSource->x * pSource->GetPixelFormat().mBytesPerPixel);
    blitInfo.mnSWidth  = pRectSource->w;
    blitInfo.mnSHeight = pRectSource->h;
    blitInfo.mnSSkip   = pSource->GetStride() - (blitInfo.mnSWidth * pSource->GetPixelFormat().mBytesPerPixel);

    blitInfo.mpDest    = pDest;
    blitInfo.mpDPixels = (uint8_t*)pDest->GetData() + (pRectDest->y * pDest->GetStride()) + ( pRectDest->x * pDest->GetPixelFormat().mBytesPerPixel);
    blitInfo.mnDWidth  = pRectDest->w;
    blitInfo.mnDHeight = pRectDest->h;
    blitInfo.mnDSkip   = pDest->GetStride() - (blitInfo.mnDWidth * pDest->GetPixelFormat().mBytesPerPixel);
   
    pDest->Lock();
    pSource->GetBlitFunction()(blitInfo);
    pDest->Unlock();

	NOTIFY_PROCESS_STATUS(EA::WebKit::kVProcessTypeDrawRaster, EA::WebKit::kVProcessStatusEnded);

	return 0;
}


EARASTER_API bool SetupBlitFunction(ISurface* pSource, ISurface* pDest)
{
    pSource->SetBlitDest(pDest);
    pSource->SetDrawFlags(0);
    pSource->SetBlitFunction(BlitNtoN);  // Initial default.

    const bool bIdenticalFormats = (pSource->GetPixelFormat().mPixelFormatType == pDest->GetPixelFormat().mPixelFormatType);
    const bool bSourceHasAlpha   = (pSource->GetPixelFormat().mAMask != 0);
    const bool bDestHasAlpha     = (pDest->GetPixelFormat().mAMask != 0);
    const bool bEnableAlphaBlend = (pSource->GetSurfaceFlags() & kFlagDisableAlpha) == 0;
    const bool bSurfaceAlpha     = (pSource->GetPixelFormat().mSurfaceAlpha != 0xff);
    const bool bBlitAlpha        = (bEnableAlphaBlend && (bSourceHasAlpha || bSurfaceAlpha));

    if(bIdenticalFormats)
        pSource->SetDrawFlags(pSource->GetDrawFlags() | kDrawFlagIdenticalFormats);

    // If we are using a non-alpha copy between identical pixel formats, use a copy blit.
    if(!bBlitAlpha && bIdenticalFormats)
    {
        pSource->SetBlitFunction(BlitCopy);

        if (pSource == pDest)
            pSource->SetBlitFunction(BlitCopyOverlap);
    }
    else
    {
        if(bBlitAlpha)
        {
            const PixelFormat& sf = pSource->GetPixelFormat();
            const PixelFormat& df = pDest->GetPixelFormat();

            if(sf.mAMask == 0)  // If source doesn't have alpha...
            {
                // Per-surface alpha blits
                switch(df.mBytesPerPixel)
                {
                    case 4:
                    {
                        if((sf.mRMask == df.mRMask) &&
                           (sf.mGMask == df.mGMask) &&
                           (sf.mBMask == df.mBMask) &&
                           (sf.mBytesPerPixel == 4))
                        {
                            #if MMX_ASMBLIT
                                if(HaveMMX())
                                {
                                    pSource->SetBlitFunction(BlitRGBtoRGBSurfaceAlphaMMX);
                                    break;
                                }
                            #endif

                            if((sf.mRMask | sf.mGMask | sf.mBMask) == 0x00ffffff)
                            {
                                #if ALTIVEC_BLIT
                                    if(HaveAltiVec())
                                        pSource->SetBlitFunction(BlitRGBtoRGBSurfaceAlphaAltivec);
                                    else
                                #endif
                                        pSource->SetBlitFunction(BlitRGBtoRGBSurfaceAlpha);
                                break;
                            }
                        }

                        #if ALTIVEC_BLIT
                            if(HaveAltiVec())
                                pSource->SetBlitFunction(Blit32to32SurfaceAlphaAltivec);
                            else
                        #endif
                                pSource->SetBlitFunction(BlitNtoNSurfaceAlpha);
                        break;
                    }

                    case 3:
                    default:
                        pSource->SetBlitFunction(BlitNtoNSurfaceAlpha);
                        break;
                }
            }
            else // Else source has alpha.
            {
                // Per-pixel alpha blits
                switch(df.mBytesPerPixel)
                {
                    case 4:
                        if((sf.mRMask == df.mRMask) && 
                           (sf.mGMask == df.mGMask) && 
                           (sf.mBMask == df.mBMask) && 
                           (sf.mBytesPerPixel == 4))
                        {
                            #if MMX_ASMBLIT
                                if(Have3DNow())
                                {
                                    pSource->SetBlitFunction(BlitRGBtoRGBPixelAlphaMMX3DNOW);
                                    break;
                                }

                                if(HaveMMX())
                                {
                                    pSource->SetBlitFunction(BlitRGBtoRGBPixelAlphaMMX);
                                    break;
                                }
                            #endif

                            if(sf.mAMask == 0xff000000)
                            {
                                #if ALTIVEC_BLIT
                                    if(HaveAltiVec())
                                        pSource->SetBlitFunction(BlitRGBtoRGBPixelAlphaAltivec);
                                    else
                                #endif
                                        pSource->SetBlitFunction(BlitRGBtoRGBPixelAlpha);

                                break;
                            }
                        }

                        #if ALTIVEC_BLIT
                            if(HaveAltiVec())
                                pSource->SetBlitFunction(Blit32to32PixelAlphaAltivec);
                            else
                        #endif
                                pSource->SetBlitFunction(BlitNtoNPixelAlpha);
                        break;

                    case 3:
                    default:
                        pSource->SetBlitFunction(BlitNtoNPixelAlpha);
                        break;
                }
            }
        }
        else
        {
            // To do: Come up with a better default value.
            pSource->SetBlitFunction(BlitNtoN);

            if(pSource->GetBlitFunction() == BlitNtoN)
            {
                // Fastpath C fallback: 32bit RGB<->RGBA blit with matching RGB.
                if ((pSource->GetPixelFormat().mBytesPerPixel == 4) && 
                    (pDest->GetPixelFormat().mBytesPerPixel == 4) &&
                    (pSource->GetPixelFormat().mRMask == pDest->GetPixelFormat().mRMask) &&
                    (pSource->GetPixelFormat().mGMask == pDest->GetPixelFormat().mGMask) &&
                    (pSource->GetPixelFormat().mBMask == pDest->GetPixelFormat().mBMask) )
                {
                    pSource->SetBlitFunction(Blit4to4MaskAlpha);
                }
                else if (bDestHasAlpha)
                    pSource->SetBlitFunction(BlitNtoNCopyAlpha);
            }
        }
    }

    return true;
}


// Blits 32 bit RGB <-> RGBA with both surfaces having the same R,G,B fields
static void Blit4to4MaskAlpha(const BlitInfo& info)
{
    const int           width   = info.mnDWidth;
    int                 height  = info.mnDHeight;
    const uint32_t*     pSource = (uint32_t*)info.mpSPixels;
    const int           srcskip = info.mnSSkip;
    uint32_t*           pDest   = (uint32_t*)info.mpDPixels;
    const int           dstskip = info.mnDSkip;
    const PixelFormat&  srcfmt  = info.mpSource->GetPixelFormat();
    const PixelFormat&  dstfmt  = info.mpDest->GetPixelFormat();

    if (dstfmt.mAMask)
    {
        // RGB->RGBA, SET_ALPHA
        const uint32_t aMask = (dstfmt.mSurfaceAlpha << dstfmt.mAShift);

        while (height--)
        {
            DUFFS_LOOP(
            {
                *pDest = *pSource | aMask;

                ++pSource;
                ++pDest;
            }, width);

            pSource = (uint32_t*)((uint8_t*)pSource + srcskip);
            pDest   = (uint32_t*)((uint8_t*)pDest + dstskip);
        }
    }
    else
    {
        // RGBA->RGB, NO_ALPHA
        const uint32_t rgbMask = (srcfmt.mRMask | srcfmt.mGMask | srcfmt.mBMask);

        while (height--)
        {
            DUFFS_LOOP(
            {
                *pDest = *pSource & rgbMask;

                ++pSource;
                ++pDest;
            }, width);

            pSource = (uint32_t*)((uint8_t*)pSource + srcskip);
            pDest   = (uint32_t*)((uint8_t*)pDest   + dstskip);
        }
    }
}


// Non-alpha-blending blit of source and dest of different formats. Generic but slow.
static void BlitNtoN(const BlitInfo& info)
{
    const int           width   = info.mnDWidth;
    int                 height  = info.mnDHeight;
    const uint8_t*      pSource = info.mpSPixels;
    const int           srcskip = info.mnSSkip;
    uint8_t*            pDest   = info.mpDPixels;
    const int           dstskip = info.mnDSkip;
    const PixelFormat&  srcfmt  = info.mpSource->GetPixelFormat();
    const int           srcbpp  = srcfmt.mBytesPerPixel;
    const PixelFormat&  dstfmt  = info.mpDest->GetPixelFormat();
    const int           dstbpp  = dstfmt.mBytesPerPixel;
    const unsigned      alpha   = (dstfmt.mPixelFormatType == kPixelFormatTypeRGB) ? 0 : srcfmt.mSurfaceAlpha;

    while (height--)
    {
        DUFFS_LOOP(
        {
            uint32_t pixel;
            unsigned sR;
            unsigned sG;
            unsigned sB;

            DISEMBLE_RGB(pSource, srcbpp, srcfmt, pixel, sR, sG, sB);
            ASSEMBLE_RGBA(pDest, dstbpp, dstfmt, sR, sG, sB, alpha);

            pSource += srcbpp;
            pDest   += dstbpp;
        }, width);

        pSource += srcskip;
        pDest   += dstskip;
    }
}


static void BlitNtoNCopyAlpha(const BlitInfo& info)
{
    const int           width   = info.mnDWidth;
    int                 height  = info.mnDHeight;
    const uint8_t*      pSource = info.mpSPixels;
    const int           srcskip = info.mnSSkip;
    uint8_t*            pDest   = info.mpDPixels;
    const int           dstskip = info.mnDSkip;
    const PixelFormat&  srcfmt  = info.mpSource->GetPixelFormat();
    const int           srcbpp  = srcfmt.mBytesPerPixel;
    const PixelFormat&  dstfmt  = info.mpDest->GetPixelFormat();
    const int           dstbpp  = dstfmt.mBytesPerPixel;

    // To do: Should map alpha to [0..255] correctly.
    while (height--)
    {
        for (int c = width; c; --c)
        {
            uint32_t pixel;
            unsigned sR, sG, sB, sA;

            DISEMBLE_RGBA(pSource, srcbpp, srcfmt, pixel, sR, sG, sB, sA);
            ASSEMBLE_RGBA(pDest,   dstbpp, dstfmt,        sR, sG, sB, sA);

            pSource += srcbpp;
            pDest   += dstbpp;
        }

        pSource += srcskip;
        pDest   += dstskip;
    }
}


// Non-alpha-blending blit of equivalent source/dest format.
static void BlitCopy(const BlitInfo& info)
{
    int            w       = info.mnDWidth * info.mpDest->GetPixelFormat().mBytesPerPixel;
    int            h       = info.mnDHeight;
    const uint8_t* pSource = info.mpSPixels;
    uint8_t*       pDest   = info.mpDPixels;
    const int      srcskip = w + info.mnSSkip;
    const int      dstskip = w + info.mnDSkip;

    while(h--)
    {
        memcpy(pDest, pSource, w);

        pSource += srcskip;
        pDest   += dstskip;
    }
}


// Non-alpha-blending blit of equivalent source/dest format of a surface to itself.
static void BlitCopyOverlap(const BlitInfo& info)
{
    int            w       = info.mnDWidth * info.mpDest->GetPixelFormat().mBytesPerPixel;
    int            h       = info.mnDHeight;
    const uint8_t* pSource = info.mpSPixels;
    uint8_t*       pDest   = info.mpDPixels;
    const int      srcskip = w + info.mnSSkip;
    const int      dstskip = w + info.mnDSkip;

    if (pDest < pSource)
    {
        while (h--)
        {
            memmove(pDest, pSource, w);

            pSource += srcskip;
            pDest   += dstskip;
        }
    }
    else
    {
        // To do: Find a way to make this use memmove instead of revcpy. This might be possible
        //        by simply always using the above pathway instead of this one.
        pSource += ((h - 1) * srcskip);
        pDest   += ((h - 1) * dstskip);

        while (h--)
        {
            revcpy(pDest, pSource, w);

            pSource -= srcskip;
            pDest   -= dstskip;
        }
    }
}


// fast RGB888->(A)RGB888 blending with surface alpha
static void BlitRGBtoRGBSurfaceAlpha(const BlitInfo& info)
{
    unsigned        alpha   = info.mpSource->GetPixelFormat().mSurfaceAlpha;
    int             width   = info.mnDWidth;
    int             height  = info.mnDHeight;
    const uint32_t* pSrc    = (uint32_t*)info.mpSPixels;
    int             srcskip = info.mnSSkip >> 2;
    uint32_t*       pDst    = (uint32_t*)info.mpDPixels;
    int             dstskip = info.mnDSkip >> 2;
    uint32_t        s;
    uint32_t        d;

    while(height--)
    {
        // This can certainly be more optimized if needed...        
        DUFFS_LOOP4({
            s = *pSrc;
            d = *pDst;

            s = s | (alpha << 24);
            s = PremultiplyColor(s);
            *pDst = BlendARGB32Premultiplied(s, d);

            ++pSrc;
            ++pDst;
        }, width);

        pSrc += srcskip;
        pDst += dstskip;
    }
}


// Fast ARGB -> ARGB blending with source pixel alpha.
static void BlitRGBtoRGBPixelAlpha(const BlitInfo& info)
{
    int             width   = info.mnDWidth;
    int             height  = info.mnDHeight;
    const uint32_t* pSrc    = (uint32_t*)info.mpSPixels;
    int             srcskip = info.mnSSkip >> 2;
    uint32_t*       pDst    = (uint32_t*)info.mpDPixels;
    int             dstskip = info.mnDSkip >> 2;

    while(height--)
    {
        // This is a bit slower then the            
        DUFFS_LOOP4({
            uint32_t d = *pDst;
            uint32_t s = *pSrc;
            uint32_t dalpha =  (d >> 24);
            uint32_t alpha = (s >> 24);

            if (alpha)
            {
                if (!dalpha || alpha == 255)
                {
                    *pDst = s;
                }
                else
                {
                    *pDst = BlendARGB32Premultiplied(s, d);
                }
            }
            ++pSrc;
            ++pDst;
        }, width);

        pSrc += srcskip;
        pDst += dstskip;
    }
}


// General (slow) N->N blending with per-surface alpha.
static void BlitNtoNSurfaceAlpha(const BlitInfo& info)
{
    const int            width   = info.mnDWidth;
    int                  height  = info.mnDHeight;
    const uint8_t*       pSource = info.mpSPixels;
    const int            srcskip = info.mnSSkip;
    uint8_t*             pDest   = info.mpDPixels;
    const int            dstskip = info.mnDSkip;
    const PixelFormat&   srcfmt  = info.mpSource->GetPixelFormat();
    const PixelFormat&   dstfmt  = info.mpDest->GetPixelFormat();
    const int            srcbpp  = srcfmt.mBytesPerPixel;
    const int            dstbpp  = dstfmt.mBytesPerPixel;
    const unsigned       sA      = srcfmt.mSurfaceAlpha;
    const unsigned       dA      = dstfmt.mAMask ? 255 : 0;

    if (sA)
    {
        // Pre-multiplied alpha blend        
        while (height--)
        {
            DUFFS_LOOP4(
            {
                uint32_t pixel;
                unsigned sR;
                unsigned sG;
                unsigned sB;
                unsigned dR;
                unsigned dG;
                unsigned dB;

                DISEMBLE_RGB(pSource, srcbpp, srcfmt, pixel, sR, sG, sB);
                DISEMBLE_RGB(pDest,   dstbpp, dstfmt, pixel, dR, dG, dB);
                ADDITIVE_ALPHA_BLEND_RGB(sR, sG, sB, sA, dR, dG, dB);
                ASSEMBLE_RGBA(pDest, dstbpp, dstfmt, dR, dG, dB, dA);

                pSource += srcbpp;
                pDest   += dstbpp;
            }, width);

            pSource += srcskip;
            pDest   += dstskip;
        }    
    }
}


// General (slow) N->N blending with pixel alpha.
static void BlitNtoNPixelAlpha(const BlitInfo& info)
{
    const int            width   = info.mnDWidth;
    int                  height  = info.mnDHeight;
    const uint8_t*       pSource = info.mpSPixels;
    const int            srcskip = info.mnSSkip;
    uint8_t*             pDest   = info.mpDPixels;
    const int            dstskip = info.mnDSkip;
    const PixelFormat&   srcfmt  = info.mpSource->GetPixelFormat();
    const PixelFormat&   dstfmt  = info.mpDest->GetPixelFormat();
    const int            srcbpp  = srcfmt.mBytesPerPixel;
    const int            dstbpp  = dstfmt.mBytesPerPixel;
    
    // Note: For 8bpp source alpha, this doesn't get opaque values
    // quite right. For <8bpp source alpha, it gets them very wrong.
    // It is unclear whether there is a good general solution that doesn't
    // need a branch (or a divide).

    // Pre-multiplied alpha blend
    while (height--)
    {
        DUFFS_LOOP4(
        {
            uint32_t pixel;
            unsigned sR;
            unsigned sG;
            unsigned sB;
            unsigned dR;
            unsigned dG;
            unsigned dB;
            unsigned sA;
            unsigned dA;

            DISEMBLE_RGBA(pSource, srcbpp, srcfmt, pixel, sR, sG, sB, sA);

            if(sA)
            {
                DISEMBLE_RGBA(pDest, dstbpp, dstfmt, pixel, dR, dG, dB, dA);
                if(!dA)
                { 
                    // 0 alpha background support               
                    ASSEMBLE_RGBA(pDest, dstbpp, dstfmt, sR, sG, sB, sA);
                }
                else
                {
                    ADDITIVE_ALPHA_BLEND_ARGB(sR, sG, sB, sA, dR, dG, dB, dA);
                    ASSEMBLE_RGBA(pDest, dstbpp, dstfmt, dR, dG, dB, dA);
                }
            }

            pSource += srcbpp;
            pDest   += dstbpp;
        }, width);

        pSource += srcskip;
        pDest   += dstskip;
    }
}


#if MSVC_ASMBLIT


// Fast XRGB -> XRGB/ARGB with source surface alpha.
static void BlitRGBtoRGBSurfaceAlphaMMX(const BlitInfo& info)
{
    const PixelFormat&  df       = info.mpDest->GetPixelFormat();
    const uint32_t      chanmask = df.mRMask | df.mGMask | df.mBMask;
    const uint32_t      amask    = df.mAMask;
    const unsigned      alpha    = info.mpSource->GetPixelFormat().mSurfaceAlpha;
    const int           width    = info.mnDWidth;
    int                 height   = info.mnDHeight;
    const uint32_t*     pSrc     = (const uint32_t*)info.mpSPixels;
    const int           srcskip  = info.mnSSkip >> 2;
    uint32_t*           pDst     = (uint32_t*)info.mpDPixels;
    const int           dstskip  = info.mnDSkip >> 2;
    uint32_t            amult, invAmult;
   
    __m64  src1, dst1, mm_alpha, mm_alphaMask255, mm_chanMask, mm_invAlpha, mm_zero, mm_shiftedBy8;
    
    mm_zero  = _mm_setzero_si64();          // 0 -> mm_zero

    __m64 mm_255, mm_128;
    mm_255 = _mm_set1_pi16(0xff);   // 0x00FF 00FF 00FF 00FF
    mm_128 = _mm_set1_pi16(0x80);   // 0x0080 0080 0080 0080
      
    // So the alpha stay fixed in this version...

    // alpha
    amult       = alpha | (alpha << 8);
    amult       = amult | (amult << 16);
    mm_alpha    = _mm_set_pi32(0, amult);                           // 0000AAAA -> mm_alpha
    mm_alpha    = _mm_unpacklo_pi8(mm_alpha, mm_zero);              // 0A0A0A0A -> mm_alpha

    // alpha mask for the alpha component (multiplied by 255)
    mm_alphaMask255 = _mm_set_pi32(0, amult & amask);               // 0000A000 -> mm_alphaMask, with A in the correct channel
    mm_alphaMask255 = _mm_unpacklo_pi8(mm_alphaMask255, mm_zero);   // 0A000000 -> mm_alphaMask, with A in the correct channel
    mm_alphaMask255 = _mm_mullo_pi16(mm_alphaMask255, mm_255);      // AA000000 -> mm_alphaMask, with A multiplied by 255

    // channel mask for the non-alpha component
    mm_chanMask = _mm_set_pi32(0, chanmask);                        // 00000111 -> mm_chanMask, minus alpha chan
    mm_chanMask = _mm_unpacklo_pi8(mm_chanMask, mm_chanMask);       // 00111111 -> mm_chanMask, minus alpha chan
    
    // invAlpha
    unsigned invAlpha = 255 - alpha;    // Inverse alpha
    invAmult    = invAlpha | (invAlpha << 8);
    invAmult    = invAmult | (invAmult << 16);
    mm_invAlpha = _mm_set_pi32(0, invAmult);                        // 0000AAAA -> mm_invAlpha
    mm_invAlpha = _mm_unpacklo_pi8(mm_invAlpha, mm_zero);           // 0A0A0A0A -> mm_invAlpha
    
    while (height--)
    {
        int n = width;
        while(n--)
        {
            // Extract the colors
            dst1 = _mm_cvtsi32_si64(*pDst);                     // pDest(ARGB) -> dst1 (0000ARGB)
            src1 = _mm_cvtsi32_si64(*pSrc);                     // pSource(ARGB) -> src1 (0000ARGB)
            src1 = _mm_unpacklo_pi8(src1, mm_zero);             // 000R0G0B -> src1  
            dst1 = _mm_unpacklo_pi8(dst1, mm_zero);             // 0A0R0G0B -> dst1

            // Premultiply the color with the surface alpha
            src1 = _mm_mullo_pi16(src1, mm_alpha);              // src1 * alpha -> src1

            // add the alpha to the src1 color
            src1 = _mm_and_si64(src1, mm_chanMask);             // 00RRGGBB -> src1 (clear the alpha channel in src1 first)
            src1 = _mm_or_si64(src1, mm_alphaMask255);          // AARRGGBB -> src1

            // Blend the colors (premultiplied): dst1 = ( src1 + (dst1 * invAlpha) ) / 255
            dst1 = _mm_mullo_pi16(dst1, mm_invAlpha);           // dst1 * invAlpha 

            dst1 = _mm_add_pi16(src1, dst1);                    // src1 + dst1

            // dst1 / 255 (see DivideBy255Rounded)
            dst1 = _mm_add_pi16(dst1, mm_128);                  // dst1 + 128
            mm_shiftedBy8 = _mm_srli_pi16(dst1, 8);             // dst1 >> 8
            dst1 = _mm_add_pi16(dst1, mm_shiftedBy8);           // dst1 + (dst1 >> 8)
            dst1 = _mm_srli_pi16(dst1, 8);                      // dst1 >> 8
                      
            // Pack down to byte format
            dst1 = _mm_packs_pu16(dst1, mm_zero);               // 0000ARGB -> dst1
            *pDst = _mm_cvtsi64_si32(dst1);                     // dst1 -> pixel
        
            ++pSrc;
            ++pDst;            
        }

        pSrc += srcskip;
        pDst += dstskip;
    }
    _mm_empty();
}


// Fast ARGB -> XRGB/ARGB with pixel alpha.
static void BlitRGBtoRGBPixelAlphaMMX(const BlitInfo& info)
{
    const int           width    = info.mnDWidth;
    int                 height   = info.mnDHeight;
    const uint32_t*     pSrc     = (uint32_t*)info.mpSPixels;
    const int           srcskip  = info.mnSSkip >> 2;
    uint32_t*           pDst     = (uint32_t*)info.mpDPixels;
    const int           dstskip  = info.mnDSkip >> 2;
    const PixelFormat&  sf       = info.mpSource->GetPixelFormat();
    const uint32_t      chanmask = sf.mRMask | sf.mGMask | sf.mBMask;
    const uint32_t      amask    = sf.mAMask;
    const uint32_t      ashift   = sf.mAShift;
    __m64               src1, dst1, mm_alpha, mm_zero, mm_shiftedBy8;

    mm_zero  = _mm_setzero_si64();          // 0 -> mm_zero

    __m64 mm_255, mm_128;
    mm_255 = _mm_set1_pi16(0xff);   // 0x00FF 00FF 00FF 00FF
    mm_128 = _mm_set1_pi16(0x80);   // 0x0080 0080 0080 0080

    while(height--)
    {
        DUFFS_LOOP4({
            uint32_t alpha = *pSrc & amask;
            uint32_t destAlpha = *pDst & amask;
            
            if (alpha)
            {
                if (!destAlpha || alpha == amask)
                {
                    // 7/23/09 CSidhall - Added 0 alpha case for destination for the MMX blend bellow will fail with a 0 dest alpha
                    *pDst = *pSrc;
                }
                else
                {
                    // Extract the colors
                    src1 = _mm_cvtsi32_si64(*pSrc);                     // pSource(ARGB) -> src1 (0000ARGB)
                    dst1 = _mm_cvtsi32_si64(*pDst);                     // pDest(ARGB) -> dst1 (0000ARGB)
                    mm_alpha = _mm_srli_si64(src1, ashift);             // Grab the alpha: mm_alpha >> ashift -> mm_alpha(0000000A) 
                    src1 = _mm_unpacklo_pi8(src1, mm_zero);             // 0A0R0G0B -> src1  
                    dst1 = _mm_unpacklo_pi8(dst1, mm_zero);             // 0A0R0G0B -> dst1

                    // Build the inverse alpha and broadcast it to all the color channels 
                    // We don't care if it goes into the alpha channel as this will be cleaned later
                    mm_alpha =_mm_sub_pi8(mm_255, mm_alpha);            // inverse alpha: 255 - alpha - we ignore the upper words
                    mm_alpha = _mm_unpacklo_pi16(mm_alpha, mm_alpha);   // 00000A0A -> mm_alpha
                    mm_alpha = _mm_unpacklo_pi32(mm_alpha, mm_alpha);   // 0A0A0A0A -> mm_alpha

                    // Blend the colors (premultiplied): dst = src + (dst * invAlpha) / 255
                    dst1 = _mm_mullo_pi16(dst1, mm_alpha);              // dst1 * invAlpha

                    // dst / 255 (see DivideBy255Rounded)
                    dst1 = _mm_add_pi16(dst1, mm_128);                  // dst + 128
                    mm_shiftedBy8 = _mm_srli_pi16(dst1, 8);             // dst >> 8
                    dst1 = _mm_add_pi16(dst1, mm_shiftedBy8);           // dst + (dst >> 8)
                    dst1 = _mm_srli_pi16(dst1, 8);                      // dst >> 8

                    dst1 = _mm_add_pi16(src1, dst1);                    // src1 + dst1

                    // Pack down to byte format
                    dst1 = _mm_packs_pu16(dst1, mm_zero);               // 0000ARGB -> dst1
                    *pDst = _mm_cvtsi64_si32(dst1);                     // dst1 -> pixel
                }
            }

            ++pSrc;
            ++pDst;
        }, width);

        pSrc += srcskip;
        pDst += dstskip;
    }
   _mm_empty();
}


// Fast (as in MMX with prefetch) ARGB888->(A)RGB888 blending with pixel alpha.
static void BlitRGBtoRGBPixelAlphaMMX3DNOW(const BlitInfo& info)
{
    const int           width    = info.mnDWidth;
    int                 height   = info.mnDHeight;
    const uint32_t*     pSrc     = (uint32_t*)info.mpSPixels;
    const int           srcskip  = info.mnSSkip >> 2;
    uint32_t*           pDst     = (uint32_t*)info.mpDPixels;
    const int           dstskip  = info.mnDSkip >> 2;
    const PixelFormat&  sf       = info.mpSource->GetPixelFormat();
    const uint32_t      chanmask = sf.mRMask | sf.mGMask | sf.mBMask;
    const uint32_t      amask    = sf.mAMask;
    const uint32_t      ashift   = sf.mAShift;
    __m64               src1, dst1, mm_alpha, mm_zero, mm_shiftedBy8;

    mm_zero  = _mm_setzero_si64();          // 0 -> mm_zero

    __m64 mm_255, mm_128;
    mm_255 = _mm_set1_pi16(0xff);   // 0x00FF 00FF 00FF 00FF
    mm_128 = _mm_set1_pi16(0x80);   // 0x0080 0080 0080 0080

    while(height--)
    {
        DUFFS_LOOP4({
            uint32_t alpha;
            uint32_t destAlpha;

            // 3D Now 
            _m_prefetch((uint32_t*)pSrc + 16);
            _m_prefetch((uint32_t*)pDst + 16);

            alpha = *pSrc & amask;
            destAlpha = *pDst & amask;

            if (alpha)
            {
                if (!destAlpha || alpha == amask)
                {
                    // 7/23/09 CSidhall - Added 0 alpha case for destination for the MMX blend bellow will fail with a 0 dest alpha
                    *pDst = *pSrc;
                }
                else
                {
                    // Extract the colors
                    src1 = _mm_cvtsi32_si64(*pSrc);                     // pSource(ARGB) -> src1 (0000ARGB)
                    dst1 = _mm_cvtsi32_si64(*pDst);                     // pDest(ARGB) -> dst1 (0000ARGB)
                    mm_alpha = _mm_srli_si64(src1, ashift);             // Grab the alpha: mm_alpha >> ashift -> mm_alpha(0000000A) 
                    src1 = _mm_unpacklo_pi8(src1, mm_zero);             // 0A0R0G0B -> src1  
                    dst1 = _mm_unpacklo_pi8(dst1, mm_zero);             // 0A0R0G0B -> dst1

                    // Build the inverse alpha and broadcast it to all the color channels 
                    // We don't care if it goes into the alpha channel as this will be cleaned later
                    mm_alpha =_mm_sub_pi8(mm_255, mm_alpha);            // inverse alpha: 255 - alpha - we ignore the upper words
                    mm_alpha = _mm_unpacklo_pi16(mm_alpha, mm_alpha);   // 00000A0A -> mm_alpha
                    mm_alpha = _mm_unpacklo_pi32(mm_alpha, mm_alpha);   // 0A0A0A0A -> mm_alpha

                    // Blend the colors (premultiplied): dst = src + (dst * invAlpha) / 255
                    dst1 = _mm_mullo_pi16(dst1, mm_alpha);              // dst1 * invAlpha

                    // dst / 255 (see DivideBy255Rounded)
                    dst1 = _mm_add_pi16(dst1, mm_128);                  // dst + 128
                    mm_shiftedBy8 = _mm_srli_pi16(dst1, 8);             // dst >> 8
                    dst1 = _mm_add_pi16(dst1, mm_shiftedBy8);           // dst + (dst >> 8)
                    dst1 = _mm_srli_pi16(dst1, 8);                      // dst >> 8

                    dst1 = _mm_add_pi16(src1, dst1);                    // src1 + dst1

                    // Pack down to byte format
                    dst1 = _mm_packs_pu16(dst1, mm_zero);               // 0000ARGB -> dst1
                    *pDst = _mm_cvtsi64_si32(dst1);                     // dst1 -> pixel
                }
            }

            ++pSrc;
            ++pDst;
        }, width);

        pSrc += srcskip;
        pDst += dstskip;
    }
    _mm_empty();
}
#endif // MSVC_ASMBLIT


#if ALTIVEC_BLIT  // The matching #endif is way below.

#if (defined(__MACOSX__) && (__GNUC__ < 4))
    #define VECUINT8_LITERAL(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) (vector unsigned char) ( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p )
    #define VECUINT16_LITERAL(a,b,c,d,e,f,g,h)                (vector unsigned short) ( a,b,c,d,e,f,g,h )
#else
    #define VECUINT8_LITERAL(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) (vector unsigned char) { a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p }
    #define VECUINT16_LITERAL(a,b,c,d,e,f,g,h)                (vector unsigned short) { a,b,c,d,e,f,g,h }
#endif

#define UNALIGNED_PTR(x) (((size_t) x) & 0x0000000F)

#define VECPRINT(msg, v) \
do { \
    vector unsigned int tmpvec = (vector unsigned int)(v); \
    unsigned int *vp = (unsigned int *)&tmpvec; \
    printf("%s = %08X %08X %08X %08X\n", msg, vp[0], vp[1], vp[2], vp[3]); \
} while (0)


// The permuation vector that takes the high bytes out of all the appropriate shorts 
//  (vector unsigned char)(
//      0x00, 0x10, 0x02, 0x12,
//      0x04, 0x14, 0x06, 0x16,
//      0x08, 0x18, 0x0A, 0x1A,
//      0x0C, 0x1C, 0x0E, 0x1E );



#define VEC_128()            ((vector unsigned short) vec_sl((vector unsigned short)vec_splat_u16(1), (vector unsigned short) vec_splat_u16(7)))
#define VEC_MERGE_PERMUTE()  (vec_add(vec_lvsl(0, (int*)NULL), (vector unsigned char)vec_splat_u16(0x0F)))
#define VEC_U32_24()         (vec_add(vec_splat_u32(12), vec_splat_u32(12)))
#define VEC_ALPHA_MASK()     ((vector unsigned char)vec_sl((vector unsigned int)vec_splat_s8(-1), VEC_U32_24()))
#define VEC_ALIGNER(pSource) ((UNALIGNED_PTR(pSource)) ? vec_lvsl(0, pSource) : vec_add(vec_lvsl(8, pSource), vec_splat_u8(8)))

#define VEC_MULTIPLY_ALPHA(vs, vd, valpha, mergePermute, v1_16, v8_16) \
do { \
    /* vtemp1 contains source AAGGAAGGAAGGAAGG */ \
    vector unsigned short vtemp1 = vec_mule(vs, valpha); \
    /* vtemp2 contains source RRBBRRBBRRBBRRBB */ \
    vector unsigned short vtemp2 = vec_mulo(vs, valpha); \
    /* valpha2 is 255-alpha */ \
    vector unsigned char valpha2 = vec_nor(valpha, valpha); \
    /* vtemp3 contains dest AAGGAAGGAAGGAAGG */ \
    vector unsigned short vtemp3 = vec_mule(vd, valpha2); \
    /* vtemp4 contains dest RRBBRRBBRRBBRRBB */ \
    vector unsigned short vtemp4 = vec_mulo(vd, valpha2); \
    /* add source and dest */ \
    vtemp1 = vec_add(vtemp1, vtemp3); \
    vtemp2 = vec_add(vtemp2, vtemp4); \
    /* vtemp1 = (vtemp1 + 1) + ((vtemp1 + 1) >> 8) */ \
    vtemp1 = vec_add(vtemp1, v1_16); \
    vtemp3 = vec_sr(vtemp1, v8_16); \
    vtemp1 = vec_add(vtemp1, vtemp3); \
    /* vtemp2 = (vtemp2 + 1) + ((vtemp2 + 1) >> 8) */ \
    vtemp2 = vec_add(vtemp2, v1_16); \
    vtemp4 = vec_sr(vtemp2, v8_16); \
    vtemp2 = vec_add(vtemp2, vtemp4); \
    /* (>>8) and get ARGBARGBARGBARGB */ \
    vd = (vector unsigned char)vec_perm(vtemp1, vtemp2, mergePermute); \
} while (0)


// 12/17/09 Chris Sidhall - Added Altivec support for additive blending
// 3/2/10 - Updated based on work by YChin to the PC/360 blend side (higher divide precision by 255 instead of 256, src and dest alpha blend from translucent 
// backgrounds and removing overflow checks since now everything is premultiplied).  

// So the permute vector here is expecting 0x0110310 for
// we the additive alpha, we don't use the upper 8 bits of the shorts,
// these are mostly used for checking for overflow so that we can clamp the values back to 255 thanks to the 
// vec_min instruction. 
#define VEC_MULTIPLY_ADDITIVE_ALPHA(vs, vd, valpha, mergePermute, v1_8, v8_16, v128_16)  \
do {                                                                            \
    /* valpha2 is 255-alpha */                                                  \
    vector unsigned char invAlpha = vec_nor(valpha, valpha);                    \
    /* Additive destination blend */                                            \
    /* vtemp1 contains dest AAGGAAGGAAGGAAGG */                                 \
    vector unsigned short vtemp1 = vec_mule(vd, invAlpha);                      \
    /* vtemp2 contains dest RRBBRRBBRRBBRRBB */                                 \
    vector unsigned short vtemp2 = vec_mulo(vd, invAlpha);                      \
    /* Use rounded system instead of >> 8 shift: dst / 255 */                   \
    /* Step 1: dst + 128 */                                                     \
    vtemp1 = vec_add(vtemp1, v128_16);                                          \
    vtemp2 = vec_add(vtemp2, v128_16);                                          \
    /* Step2: dst >> 8 */                                                       \
    vector unsigned short vtemp3 = vec_sr(vtemp1, v8_16);                       \
    vector unsigned short vtemp4 = vec_sr(vtemp2, v8_16);                       \
    /* Step 3: dst + (dst >> 8) */                                              \
    vtemp1 = vec_add(vtemp1, vtemp3);                                           \
    vtemp2 = vec_add(vtemp2, vtemp4);                                           \
    /* Step 4: dst >> 8 */                                                      \
    vtemp1 = vec_sr(vtemp1, v8_16);                                             \
    vtemp2 = vec_sr(vtemp2, v8_16);                                             \
    /* Merge back lower bytes to destination */                                 \
    vd = (vector unsigned char)vec_perm(vtemp1, vtemp2, mergePermute);          \
    /* Add source + detination */                                               \
    vd = vec_add(vd, vs);                                                       \
} while (0)                                                                     


// Function version of VEC_MULTIPLY_ADDITIVE_ALPHA for debugging
/*
static vector unsigned char VEC_MULTIPLY_ADDITIVE_ALPHA(vector unsigned char vs, vector unsigned char vd, 
                                        vector unsigned char valpha, vector unsigned char mergePermute, 
                                        vector unsigned char  v1_8, vector unsigned short v8_16, vector unsigned short v128_16) 
{
//    VECPRINT("vs", vs); 
//    VECPRINT("vd", vd); 
//    VECPRINT("Alpha", valpha);  
//    VECPRINT("128", v128_16);  

    // valpha2 is 255-alpha 
    vector unsigned char invAlpha = vec_nor(valpha, valpha); 
//    VECPRINT("InvAlpha", invAlpha);  

    // Destination blend
    // vtemp3 contains dest AAGGAAGGAAGGAAGG 
    vector unsigned short vtemp1 = vec_mule(vd, invAlpha);
    // vtemp4 contains dest RRBBRRBBRRBBRRBB 
    vector unsigned short vtemp2 = vec_mulo(vd, invAlpha); 

    // Use rounded system instead of >> 8 shift: dst / 255 (see DivideBy255Rounded)
    // Step 1: dst + 128
    vtemp1 = vec_add(vtemp1, v128_16); 
    vtemp2 = vec_add(vtemp2, v128_16); 
    
//    VECPRINT("Temp1", vtemp1);  
//    VECPRINT("Temp2", vtemp2);  

    // Step2: dst >> 8
    vector unsigned short vtemp3 = vec_sr(vtemp1, v8_16); 
    vector unsigned short vtemp4 = vec_sr(vtemp2, v8_16); 

//    VECPRINT("Temp3", vtemp3);  
//    VECPRINT("Temp4", vtemp4);  

    // Step 3: dst + (dst >> 8)
     vtemp1 = vec_add(vtemp1, vtemp3); 
     vtemp2 = vec_add(vtemp2, vtemp4); 

//    VECPRINT("Temp1", vtemp1);  
//    VECPRINT("Temp2", vtemp2);  

    // Step 4: dst >> 8
    vtemp1 = vec_sr(vtemp1, v8_16); 
    vtemp2 = vec_sr(vtemp2, v8_16); 

//    VECPRINT("Temp1", vtemp1);  
//    VECPRINT("Temp2", vtemp2);  

    vd = (vector unsigned char)vec_perm(vtemp1, vtemp2, mergePermute); 
//    VECPRINT("vd blend", vd); 

    // Add source + detination
    vd = vec_add(vd, vs); 
//    VECPRINT("vd+vs", vd); 
    return vd;                  
}
*/

// Calculate the permute vector used for 32->32 swizzling

static vector unsigned char calc_swizzle32(const PixelFormat* srcfmt, const PixelFormat* dstfmt)
{
    // We have to assume that the bits that aren't used by other
    // colors is alpha, and it's one complete byte, since some formats
    // leave alpha with a zero mask, but we should still swizzle the bits.

    // ARGB
    const static PixelFormat default_pixel_format = 
    {
        (PixelFormatType)0, 32, 0xff,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000,
        16, 8, 0, 24,
    };

    if (!srcfmt)
        srcfmt = &default_pixel_format;

    if (!dstfmt)
        dstfmt = &default_pixel_format;

    const vector unsigned char plus = VECUINT8_LITERAL
                                            ( 0x00, 0x00, 0x00, 0x00,
                                              0x04, 0x04, 0x04, 0x04,
                                              0x08, 0x08, 0x08, 0x08,
                                              0x0C, 0x0C, 0x0C, 0x0C );
    vector unsigned char vswiz;
    vector unsigned int srcvec;

    #define RESHIFT(X) (3 - ((X) >> 3))

    uint32_t rmask = RESHIFT(srcfmt->mRShift) << (dstfmt->mRShift);
    uint32_t gmask = RESHIFT(srcfmt->mGShift) << (dstfmt->mGShift);
    uint32_t bmask = RESHIFT(srcfmt->mBShift) << (dstfmt->mBShift);
    uint32_t amask;

    // Use zero for alpha if either surface doesn't have alpha.
    if (dstfmt->mAMask)
        amask = ((srcfmt->mAMask) ? RESHIFT(srcfmt->mAShift) : 0x10) << (dstfmt->mAShift);
    else
        amask = 0x10101010 & ((dstfmt->mRMask | dstfmt->mGMask | dstfmt->mBMask) ^ 0xFFFFFFFF);

    #undef RESHIFT

    ((unsigned int *)(char*)&srcvec)[0] = (rmask | gmask | bmask | amask);
    vswiz = vec_add(plus, (vector unsigned char)vec_splat(srcvec, 0));

    return vswiz;
}


static void Blit32to32PixelAlphaAltivec(const BlitInfo& info)
{
    int          width   = info.mnDWidth;
    int          height  = info.mnDHeight;
    uint32_t*    pSrc    = (uint32_t*)info.mpSPixels;
    int          srcskip = info.mnSSkip >> 2;
    uint32_t*    pDst    = (uint32_t*)info.mpDPixels;
    int          dstskip = info.mnDSkip >> 2;
    PixelFormat& srcfmt  = info.mpSource->GetPixelFormat();
    PixelFormat& dstfmt  = info.mpDest->GetPixelFormat();

    vector unsigned char  mergePermute;
    vector unsigned char  valphaPermute;
    vector unsigned char  vsrcPermute;
    vector unsigned char  vdstPermute;
    vector unsigned char  vsdstPermute;
    vector unsigned char  valphamask;
    vector unsigned char  vpixelmask;
    vector unsigned char  v0;
    vector unsigned short v8;
    vector unsigned short v128;

    v128 = VEC_128();
    v0 = vec_splat_u8(0);
    v8 = vec_splat_u16(8);

    mergePermute  = VEC_MERGE_PERMUTE();
    valphamask    = VEC_ALPHA_MASK();
    valphaPermute = vec_and(vec_lvsl(0, (int *)NULL), vec_splat_u8(0xC));
    vpixelmask    = vec_nor(valphamask, v0);
    vsrcPermute   = calc_swizzle32(&srcfmt, NULL);
    vdstPermute   = calc_swizzle32(NULL,    &dstfmt);
    vsdstPermute  = calc_swizzle32(&dstfmt, NULL);


    // This is doing an additive alpha blend: source + dest * inv alpha 
    vector unsigned char v1 = vec_splat_u8(1);

    // For the additive blend we use a different permute index (+1 basically)
    // so that we can index lower 8 bits of each short
    mergePermute = vec_add(mergePermute, v1);
  
    while ( height-- )
    {
        width = info.mnDWidth;

        #define ONE_PIXEL_BLEND(condition, widthvar) \
        while ((condition)) { \
            uint32_t pixel; \
            unsigned sR, sG, sB, dR, dG, dB, sA, dA; \
            DISEMBLE_RGBA((uint8_t*)pSrc, 4, srcfmt, pixel, sR, sG, sB, sA); \
            if(sA) { \
                DISEMBLE_RGBA((uint8_t*)pDst, 4, dstfmt, pixel, dR, dG, dB, dA); \
                ADDITIVE_ALPHA_BLEND_ARGB(sR, sG, sB, sA, dR, dG, dB, dA); \
                ASSEMBLE_RGBA((uint8_t*)pDst, 4, dstfmt, dR, dG, dB, dA); \
            } \
            ++pSrc; \
            ++pDst; \
            widthvar--; \
        }

        ONE_PIXEL_BLEND((UNALIGNED_PTR(pDst)) && (width), width);

        if (width > 0)
        {
            // vsrcPermute
            // vdstPermute
            int extrawidth = (width % 4);
            vector unsigned char valigner = VEC_ALIGNER(pSrc);
            vector unsigned char vs = (vector unsigned char)vec_ld(0, pSrc);
            width -= extrawidth;

            while (width)
            {
                vector unsigned char voverflow;
                vector unsigned char vd;
                vector unsigned char valpha;
             
                // s = *pSrc
                voverflow = (vector unsigned char)vec_ld(15, pSrc);
                vs = vec_perm(vs, voverflow, valigner);
                vs = vec_perm(vs, v0, vsrcPermute);

                valpha = vec_perm(vs, v0, valphaPermute);
                
                // d = *pDst
                vd = (vector unsigned char)vec_ld(0, pDst);
                vd = vec_perm(vd, v0, vsdstPermute);
     
                VEC_MULTIPLY_ADDITIVE_ALPHA(vs, vd, valpha, mergePermute, v1, v8, v128);

                 // *pDst = res
                vec_st((vector unsigned int)vd, 0, pDst);
                
                pSrc += 4;
                pDst += 4;
                width -= 4;
                vs = voverflow; 

            }
            ONE_PIXEL_BLEND((extrawidth), extrawidth);
        }

        pSrc += srcskip;
        pDst += dstskip;

        #undef ONE_PIXEL_BLEND
    }
}

// Fast ARGB888->(A)RGB888 blending with pixel alpha
static void BlitRGBtoRGBPixelAlphaAltivec(const BlitInfo& info)
{
    int       width   = info.mnDWidth;
    int       height  = info.mnDHeight;
    uint32_t* pSrc    = (uint32_t*)info.mpSPixels;
    int       srcskip = info.mnSSkip >> 2;
    uint32_t* pDst    = (uint32_t*)info.mpDPixels;
    int       dstskip = info.mnDSkip >> 2;

    vector unsigned char mergePermute;
    vector unsigned char valphaPermute;
    vector unsigned char valphamask;
    vector unsigned char vpixelmask;
    vector unsigned char v0;
    vector unsigned short v8;
    vector unsigned short v128;

    v128 = VEC_128();
    v8 = vec_splat_u16(8);
    v0 = vec_splat_u8(0);

    mergePermute  = VEC_MERGE_PERMUTE();
    valphamask    = VEC_ALPHA_MASK();
    valphaPermute = vec_and(vec_lvsl(0, (int *)NULL), vec_splat_u8(0xC));
    
    vpixelmask = vec_nor(valphamask, v0);


    // This is doing an additive alpha blend: source + dest * inv alpha 
    vector unsigned char v1 = vec_splat_u8(1);

    // For the additive blend we use a different permute index (+1 basically)
    // so that we can index lower 8 bits of each short
    mergePermute = vec_add(mergePermute, v1);
    
    PixelFormat& srcfmt  = info.mpSource->GetPixelFormat();
    PixelFormat& dstfmt  = info.mpDest->GetPixelFormat();

    while(height--)
    {
        width = info.mnDWidth;


       #define ONE_PIXEL_BLEND(condition, widthvar) \
        while ((condition)) { \
            uint32_t pixel; \
            unsigned sR, sG, sB, dR, dG, dB, sA, dA; \
            DISEMBLE_RGBA((uint8_t*)pSrc, 4, srcfmt, pixel, sR, sG, sB, sA); \
            if(sA) { \
                DISEMBLE_RGBA((uint8_t*)pDst, 4, dstfmt, pixel, dR, dG, dB, dA); \
                /* zero alpha support */ \
                /* if(dA == 0) */ \
                /*    *pDst = *pSrc;  */ \
                /* else */ \
                ADDITIVE_ALPHA_BLEND_ARGB(sR, sG, sB, sA, dR, dG, dB, dA); \
                ASSEMBLE_RGBA((uint8_t*)pDst, 4, dstfmt, dR, dG, dB, dA); \
            } \
            ++pSrc; \
            ++pDst; \
            widthvar--; \
        }
 
        ONE_PIXEL_BLEND((UNALIGNED_PTR(pDst)) && (width), width);

        if (width > 0)
        {
            int extrawidth = (width % 4);
            vector unsigned char valigner = VEC_ALIGNER(pSrc);
            vector unsigned char vs = (vector unsigned char)vec_ld(0, pSrc);
            width -= extrawidth;

            while (width)
            {
                vector unsigned char voverflow;
                vector unsigned char vd;
                vector unsigned char valpha;

                // s = *pSrc
                voverflow = (vector unsigned char)vec_ld(15, pSrc);
                vs = vec_perm(vs, voverflow, valigner);

                valpha = vec_perm(vs, v0, valphaPermute);

                // d = *pDst
                vd = (vector unsigned char)vec_ld(0, pDst);

                VEC_MULTIPLY_ADDITIVE_ALPHA(vs, vd, valpha, mergePermute,v1, v8, v128);

                // *pDst = res
                vec_st((vector unsigned int)vd, 0, pDst);
                
                pSrc += 4;
                pDst += 4;
                width -= 4;
                vs = voverflow;
            }

            ONE_PIXEL_BLEND((extrawidth), extrawidth);
        }

        pSrc += srcskip;
        pDst += dstskip;
 
  }

    #undef ONE_PIXEL_BLEND
}


static void Blit32to32SurfaceAlphaAltivec(const BlitInfo& info)
{
  
    // XXX : 6
    unsigned     alpha   = info.mpSource->GetPixelFormat().mSurfaceAlpha;
    int          height  = info.mnDHeight;
    uint32_t*    pSrc    = (uint32_t*)info.mpSPixels;
    int          srcskip = info.mnSSkip >> 2;
    uint32_t*    pDst    = (uint32_t*)info.mpDPixels;
    int          dstskip = info.mnDSkip >> 2;
    PixelFormat& srcfmt  = info.mpSource->GetPixelFormat();
    PixelFormat& dstfmt  = info.mpDest->GetPixelFormat();

    vector unsigned char mergePermute;
    vector unsigned char vsrcPermute;
    vector unsigned char vdstPermute;
    vector unsigned char vsdstPermute;
    vector unsigned char valpha;
    vector unsigned char valphamask;
    vector unsigned char vbits;
    vector unsigned short v8;
    vector unsigned short v128;


    v8 = vec_splat_u16(8);
    v128 = VEC_128();
    mergePermute = VEC_MERGE_PERMUTE();

    // set the alpha to 255 on the destination surf
    valphamask = VEC_ALPHA_MASK();

    vsrcPermute = calc_swizzle32(&srcfmt, NULL);
    vdstPermute = calc_swizzle32(NULL,    &dstfmt);
    vsdstPermute = calc_swizzle32(&dstfmt, NULL);

    // set a vector full of alpha and 255-alpha
    ((unsigned char *)&valpha)[0] = alpha;
    valpha = vec_splat(valpha, 0);
    vbits = (vector unsigned char)vec_splat_s8(-1);


    // This is doing an additive alpha blend: source + dest * inv alpha 

    vector unsigned char v1 = vec_splat_u8(1);

    // For the additive blend we use a different permute index (+1 basically)
    // so that we can index lower 8 bits of each short
    mergePermute = vec_add(mergePermute, v1);


    while(height--)
    {
        int width = info.mnDWidth;

     #define ONE_PIXEL_BLEND(condition, widthvar) \
        while ((condition)) { \
            uint32_t pixel; \
            unsigned sR, sG, sB, dR, dG, dB, sA, dA; \
            DISEMBLE_RGBA((uint8_t*)pSrc, 4, srcfmt, pixel, sR, sG, sB, sA); \
            if(sA) { \
                DISEMBLE_RGBA((uint8_t*)pDst, 4, dstfmt, pixel, dR, dG, dB, dA); \
                ADDITIVE_ALPHA_BLEND_ARGB(sR, sG, sB, sA, dR, dG, dB, dA); \
                ASSEMBLE_RGBA((uint8_t*)pDst, 4, dstfmt, dR, dG, dB, dA); \
            } \
            ++pSrc; \
            ++pDst; \
            widthvar--; \
        }

        ONE_PIXEL_BLEND((UNALIGNED_PTR(pDst)) && (width), width);

        if (width > 0)
        {
            int extrawidth = (width % 4);
            vector unsigned char valigner = VEC_ALIGNER(pSrc);
            vector unsigned char vs = (vector unsigned char)vec_ld(0, pSrc);
            width -= extrawidth;

            while (width)
            {
                vector unsigned char voverflow;
                vector unsigned char vd;

                // s = *pSrc
                voverflow = (vector unsigned char)vec_ld(15, pSrc);
                vs = vec_perm(vs, voverflow, valigner);
                vs = vec_perm(vs, valpha, vsrcPermute);
                
                // d = *pDst
                vd = (vector unsigned char)vec_ld(0, pDst);
                vd = vec_perm(vd, vd, vsdstPermute);

                VEC_MULTIPLY_ADDITIVE_ALPHA(vs, vd, valpha, mergePermute, v1, v8, v128);

                 // *pDst = res
                vec_st((vector unsigned int)vd, 0, pDst);
                
                pSrc += 4;
                pDst += 4;
                width -= 4;
                vs = voverflow;
            }

            ONE_PIXEL_BLEND((extrawidth), extrawidth);
        }

        #undef ONE_PIXEL_BLEND
 
        pSrc += srcskip;
        pDst += dstskip;
    }
}

// Fast RGB888->(A)RGB888 blending
static void BlitRGBtoRGBSurfaceAlphaAltivec(const BlitInfo& info)
{
    unsigned  alpha   = info.mpSource->GetPixelFormat().mSurfaceAlpha;
    int       height  = info.mnDHeight;
    uint32_t* pSrc    = (uint32_t*)info.mpSPixels;
    int       srcskip = info.mnSSkip >> 2;
    uint32_t* pDst    = (uint32_t*)info.mpDPixels;
    int       dstskip = info.mnDSkip >> 2;

    vector unsigned char mergePermute;
    vector unsigned char valpha;
    vector unsigned char valphamask;
    vector unsigned short v8;
    vector unsigned short v128;

    v128         = VEC_128();
    mergePermute = VEC_MERGE_PERMUTE();
    v8           = vec_splat_u16(8);

    // set the alpha to 255 on the destination surf
    valphamask = VEC_ALPHA_MASK();

    // set a vector full of alpha and 255-alpha
    ((unsigned char *)&valpha)[0] = alpha;
    valpha = vec_splat(valpha, 0);


    // This is doing an additive alpha blend: source + dest * inv alpha 

    vector unsigned char v1 = vec_splat_u8(1);
    
    PixelFormat& srcfmt  = info.mpSource->GetPixelFormat();
    PixelFormat& dstfmt  = info.mpDest->GetPixelFormat();

    // For the additive blend we use a different permute index (+1 basically)
    // so that we can index lower 8 bits of each short
    mergePermute = vec_add(mergePermute, v1);

    while(height--)
    {
        int width = info.mnDWidth;


     #define ONE_PIXEL_BLEND(condition, widthvar) \
        while ((condition)) { \
            uint32_t pixel; \
            unsigned sR, sG, sB, dR, dG, dB, sA, dA; \
            DISEMBLE_RGBA((uint8_t*)pSrc, 4, srcfmt, pixel, sR, sG, sB, sA); \
            if(sA) { \
                DISEMBLE_RGBA((uint8_t*)pDst, 4, dstfmt, pixel, dR, dG, dB, dA); \
                ADDITIVE_ALPHA_BLEND_ARGB(sR, sG, sB, sA, dR, dG, dB,dA); \
                ASSEMBLE_RGBA((uint8_t*)pDst, 4, dstfmt, dR, dG, dB, dA); \
            } \
            ++pSrc; \
            ++pDst; \
            widthvar--; \
        }

        ONE_PIXEL_BLEND((UNALIGNED_PTR(pDst)) && (width), width);

        if (width > 0)
        {
            int extrawidth = (width % 4);
            vector unsigned char valigner = VEC_ALIGNER(pSrc);
            vector unsigned char vs = (vector unsigned char)vec_ld(0, pSrc);
            width -= extrawidth;

            while (width)
            {
                vector unsigned char voverflow;
                vector unsigned char vd;

                // s = *pSrc
                voverflow = (vector unsigned char)vec_ld(15, pSrc);
                vs = vec_perm(vs, voverflow, valigner);
                
                // d = *pDst
                vd = (vector unsigned char)vec_ld(0, pDst);

                VEC_MULTIPLY_ADDITIVE_ALPHA(vs, vd, valpha, mergePermute, v1, v8, v128);

                // *pDst = res
                vec_st((vector unsigned int)vd, 0, pDst);
                
                pSrc += 4;
                pDst += 4;
                width -= 4;
                vs = voverflow;
            }

            ONE_PIXEL_BLEND((extrawidth), extrawidth);
        }

        #undef ONE_PIXEL_BLEND
 
        pSrc += srcskip;
        pDst += dstskip;
    }
}

#ifdef __MWERKS__
    #pragma altivec_model off
#endif

#endif // ALTIVEC_BLIT



} // namespace Raster

} // namespace EA


