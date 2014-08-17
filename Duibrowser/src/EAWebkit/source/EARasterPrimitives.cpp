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

///////////////////////////////////////////////////////////////////////////////
// EARasterPrimitives.cpp
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



#include "EARaster/EARaster.h"
#include "EARaster/EARasterColor.h"
#include "EARaster/internal/EARasterUtils.h"
#include "Color.h"
#include "AffineTransform.h"
#include "IntRect.h"
#include <EAWebKit/internal/EAWebKitAssert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wtf/FastAllocBase.h>

#if (defined(__GNUC__) && (__GNUC__ >= 3)) || defined(__MWERKS__) || defined(__xlC__)
    #include <alloca.h>
#elif defined(_MSC_VER)
    #include <malloc.h>
#elif (defined(__GNUC__) && (__GNUC__ < 3)) && !defined(alloca)
    #define alloca __builtin_alloca
#endif



namespace EA {

namespace Raster {


#define kPi 3.14159265


union PixelBytes
{
    uint32_t color32;       // Same as NativeColor
    uint8_t  color8[4];
};


// Defines for pPixel clipping tests.
#define clip_xmin(pSurface) (pSurface->GetClipRect().x())
#define clip_xmax(pSurface) (pSurface->GetClipRect().x() + pSurface->GetClipRect().width())
#define clip_ymin(pSurface) (pSurface->GetClipRect().y())
#define clip_ymax(pSurface) (pSurface->GetClipRect().y() + pSurface->GetClipRect().height())


// Sets count uint32s at pDest to value.
static void memset4(void* pDest, uint32_t value, size_t count)
{
    unsigned  n = (count + 3) / 4;
    uint32_t* p = (uint32_t*)pDest;

    switch (count % 4)
    {
        case 0: do {  *p++ = value;
        case 3:       *p++ = value;
        case 2:       *p++ = value;
        case 1:       *p++ = value;
                } while(--n);
    }
}

// VC++ does not have lrint, so provide a local inline version
// We may be able to get away with a simpler implementation of 
// lrint for our uses here.
#if defined(_MSC_VER) && defined(_M_IX86)

    __inline long lrint(double value)
    {        
        long intgr;

        _asm
        {
            fld   value
            fistp intgr
        };

        return intgr;
    }

#elif !defined(__GNUC__)

    // For our purposes, perhaps we can get away with simply:
    //     long lrint(double x) { return (long)(x + 0.5); }

    #if defined(EA_SYSTEM_BIG_ENDIAN)
        #define LR_HIWORD 0
        #define LR_LOWORD 1
    #else
        #define LR_HIWORD 1
        #define LR_LOWORD 0
    #endif

    long lrint(double x)
    {
        union {
            unsigned i[2];
            double   d;
        } xx, yy;

        unsigned hx;

        xx.d = x;
        hx   = xx.i[LR_HIWORD] & ~0x80000000;

        if(hx < 0x43300000) // |x| < 2^52
        {
            // add and subtract a power of two to round x to an integer
            yy.i[LR_HIWORD] = (xx.i[LR_HIWORD] & 0x80000000) | 0x43e00000;
            yy.i[LR_LOWORD] = 0;
            x = (x + yy.d) - yy.d;
        }

        return (long) x;
    }

    #undef LR_LOWORD
    #undef LR_HIWORD

#endif


EARASTER_API void ConvertColor(NativeColor c, PixelFormatType cFormat, Color& result)
{
    const PixelBytes pb = { c };

    switch (cFormat)
    {
        default:
        case kPixelFormatTypeARGB:
            result = Color(pb.color32);  // 1:1 mapping.
            break;

        case kPixelFormatTypeRGBA:
            #ifdef EA_SYSTEM_BIG_ENDIAN
                result = Color(pb.color8[0], pb.color8[1], pb.color8[2], pb.color8[3]);
            #else
                //CS -This seems wrong? Would expect 3-2-1 offsets with 0 as alpha
                result = Color(pb.color8[1], pb.color8[2], pb.color8[3], pb.color8[0]);
            #endif
            break;

        case kPixelFornatTypeXRGB:
            result = Color(0xff000000 | pb.color32);
            break;

        case kPixelFormatTypeRGBX:
            #ifdef EA_SYSTEM_BIG_ENDIAN
                result = Color(pb.color8[0], pb.color8[1], pb.color8[2], 0xff);
            #else
                //CS -This seems wrong? Would expect 3-2-1 offsets with 0 as alpha
                result = Color(pb.color8[1], pb.color8[2], pb.color8[3], 0xff);
            #endif
            break;

        case kPixelFormatTypeRGB:
            result = Color(pb.color8[0], pb.color8[1], pb.color8[2]);
            break;
    }
}


EARASTER_API void ConvertColor(uint32_t pixel, const PixelFormat& pf, int& r, int& g, int& b, int& a)
{
    // unsigned v;

    r = (pixel & pf.mRMask) >> pf.mRShift;
    // v = (pixel & pf.mRMask) >> pf.mRShift;
    // r = v + (v >> 8);

    g = (pixel & pf.mGMask) >> pf.mGShift;
    // v = (pixel & pf.mGMask) >> pf.mGShift;
    // g = v + (v >> 8);

    b = (pixel & pf.mBMask) >> pf.mBShift;
    // v = (pixel & pf.mBMask) >> pf.mBShift;
    // b = v + (v >> 8);

    if(pf.mAMask)
    {
        a = (pixel & pf.mAMask) >> pf.mAShift;
        // v = (pixel & pf.mAMask) >> pf.mAShift;
        // a = v + (v >> 8);
    } 
    else
        a = 255;
}


EARASTER_API void ConvertColor(uint32_t pixel, const PixelFormat& pf, int& r, int& g, int& b)
{
    //unsigned v;

    r = (pixel & pf.mRMask) >> pf.mRShift;
    //v = (pixel & pf.mRMask) >> pf.mRShift;
    //r = v + (v >> 8);

    g = (pixel & pf.mGMask) >> pf.mGShift;
    //v = (pixel & pf.mGMask) >> pf.mGShift;
    //g = v + (v >> 8);

    b = (pixel & pf.mBMask) >> pf.mBShift;
    //v = (pixel & pf.mBMask) >> pf.mBShift;
    //b = v + (v >> 8);
}


EARASTER_API void ConvertColor(int r, int g, int b, int a, Color& result)
{
    result = Color(r, g, b, a);
}


EARASTER_API NativeColor ConvertColor(const Color& c, PixelFormatType resultFormat)
{
    const NativeColor c32 = c.rgb();

    switch (resultFormat)
    {
        default:
        case kPixelFormatTypeARGB:
            return c32;  // 1:1 mapping.

        case kPixelFormatTypeRGBA:
            return (c32 << 8) | (c32 >> 24);

        case kPixelFornatTypeXRGB:
            return c32 | 0xff000000;

        case kPixelFormatTypeRGBX:
            return (c32 << 8) | 0x000000ff;

        case kPixelFormatTypeRGB:
        {
            PixelBytes pb;

            pb.color8[0] = c.red();
            pb.color8[1] = c.green();
            pb.color8[2] = c.blue(); 
           
            return pb.color32;
        }
    }
}


EARASTER_API void ConvertColor(const Color& c, int& r, int& g, int& b, int& a)
{
    r = c.red();
    g = c.green();
    b = c.blue();
    a = c.alpha();
}

EARASTER_API void GetPixel(ISurface* pSurface, int x, int y, Color& color) 
{
    EAW_ASSERT(((unsigned)x < (unsigned)pSurface->GetWidth()) && ((unsigned)y < (unsigned)pSurface->GetHeight()));

    const void* const pPixel = (char*)pSurface->GetData() + (pSurface->GetStride() * y) + (pSurface->GetPixelFormat().mBytesPerPixel * x);

    uint32_t c;
    memcpy(&c, pPixel, pSurface->GetPixelFormat().mBytesPerPixel);

    ConvertColor(c, pSurface->GetPixelFormat().mPixelFormatType, color);
}


// The color is directly copied to the dest, including any alpha in the color. 
// No blending is done, regardless of the color alpha value.
EARASTER_API int SetPixelSolidColor(ISurface* pSurface, int x, int y, const Color& color)
{
    if(((unsigned)x < (unsigned)pSurface->GetWidth()) && ((unsigned)y < (unsigned)pSurface->GetHeight()))
    {
        pSurface->Lock();
        switch (pSurface->GetPixelFormat().mPixelFormatType)
        {
            case kPixelFormatTypeARGB: // 1:1 mapping
            {
                uint32_t* pPixel = (uint32_t*)((uint8_t*)pSurface->GetData() + (pSurface->GetStride() * y) + (4 * x));
                *pPixel = color.rgb();
                break;
            }

            case kPixelFormatTypeRGB:
            {
                uint8_t* pPixel = (uint8_t*)pSurface->GetData() + (pSurface->GetStride() * y) + (3 * x);

                pPixel[0] = color.red();
                pPixel[1] = color.green();
                pPixel[2] = color.blue();
                break;
            }

            default:
            {
                // We assume the pPixel is 32 bit aligned.
                uint32_t* pPixel = (uint32_t*)((uint8_t*)pSurface->GetData() + (pSurface->GetStride() * y) + (4 * x));
                *pPixel = ConvertColor(color, pSurface->GetPixelFormat().mPixelFormatType);
                break;
            }
        }
        pSurface->Unlock();            
    }

    return 0;
}


// 2/12/09 CSidhall This can do a rect clip before the pixel draw in case partially out of area  
static int SetPixelSolidColorAndUseClippingRect(ISurface* pSurface, const int x, const int y, const Color& color, const bool clipFlag)
{
    if(clipFlag == true)
    {
        if( (x < pSurface->GetClipRect().x) ||
            (x > (pSurface->GetClipRect().x + pSurface->GetClipRect().width())) ||
            (y < pSurface->GetClipRect().y) ||    
            (y > (pSurface->GetClipRect().y + pSurface->GetClipRect().width())) )
        {
            return 0;
        }
    }
    return SetPixelSolidColor(pSurface, x, y, color);
}




// The color is directly copied to the dest, including any alpha in the color. 
// No blending is done, regardless of the color alpha value.
EARASTER_API int SetPixelSolidColorNoClip(ISurface* pSurface, int x, int y, const Color& color)
{
    EAW_ASSERT(((unsigned)x < (unsigned)pSurface->GetWidth()) && ((unsigned)y < (unsigned)pSurface->GetHeight()));
    
    pSurface->Lock();

    switch (pSurface->GetPixelFormat().mPixelFormatType)
    {
    
        case kPixelFormatTypeARGB: // 1:1 mapping
        {
            uint32_t* pPixel = (uint32_t*)((uint8_t*)pSurface->GetData() + (pSurface->GetStride() * y) + (4 * x));
            *pPixel = color.rgb();
            break;
        }

        case kPixelFormatTypeRGB:
        {
            uint8_t* pPixel = (uint8_t*)pSurface->GetData() + (pSurface->GetStride() * y) + (3 * x);

            pPixel[0] = color.red();
            pPixel[1] = color.green();
            pPixel[2] = color.blue();
            break;
        }

        default:
        {
            // We assume the pPixel is 32 bit aligned.
            uint32_t* pPixel = (uint32_t*)((uint8_t*)pSurface->GetData() + (pSurface->GetStride() * y) + (4 * x));
            *pPixel = ConvertColor(color, pSurface->GetPixelFormat().mPixelFormatType);
            break;
        }
    }
    pSurface->Unlock();

    return 0;
}


// The color is alpha-blended with the destination surface.
EARASTER_API int SetPixelColor(ISurface* pSurface, int x, int y, const Color& color)
{
    if(((unsigned)x < (unsigned)pSurface->GetWidth()) && ((unsigned)y < (unsigned)pSurface->GetHeight()))
        SetPixelColorNoClip(pSurface, x, y, color);

    return 0;
}

// 2/12/09 CSidhall This can do a rect clip before the pixel draw in case partially out of area
static int SetPixelColorAndUseClippingRect(ISurface* pSurface, const int x, const int y, const Color& color, const bool clipFlag)
{
    if(clipFlag == true)
    {
        if( (x < pSurface->GetClipRect().x) ||
            (x > (pSurface->GetClipRect().x + pSurface->GetClipRect().width())) ||
            (y < pSurface->GetClipRect().y) ||    
            (y > (pSurface->GetClipRect().y + pSurface->GetClipRect().width())) )
        {
            return 0;
        }
    }
    return SetPixelColor(pSurface, x, y, color);
}


// The color is alpha-blended with the destination surface.
EARASTER_API int SetPixelColorNoClip(ISurface* pSurface, int x, int y, const Color& color)
{
    const int sA = color.alpha();

    pSurface->Lock();

    switch (pSurface->GetPixelFormat().mPixelFormatType)
    {
        case kPixelFormatTypeRGB:
        {
            uint8_t*  pPixel = (uint8_t*)pSurface->GetData() + (y * pSurface->GetStride()) + (x * 3);
            const int sR     = color.red();
            const int sG     = color.green();
            const int sB     = color.blue();

            if(sA == 255)
            {
                pPixel[0] = sR;
                pPixel[1] = sG;
                pPixel[2] = sB;
            }
            else
            {
                const int dR = pPixel[0];
                const int dG = pPixel[1];
                const int dB = pPixel[2];

                pPixel[0] = (uint8_t)(dR + (((sR - dR) * sA) >> 8));
                pPixel[1] = (uint8_t)(dG + (((sG - dG) * sA) >> 8));
                pPixel[2] = (uint8_t)(dB + (((sB - dB) * sA) >> 8));
            }

            break;
        }

        case kPixelFormatTypeARGB: // 1:1 mapping
        {
            uint32_t* const pPixel = (uint32_t*)pSurface->GetData() + (y * pSurface->GetStride() / 4) + x;

            if(sA == 255)   // If the source is opaque...
                *pPixel = color.rgb();
            else
            {
                uint32_t s = color.rgb();
                uint32_t d = *pPixel;
                s = PremultiplyColor(s);
                *pPixel = BlendARGB32Premultiplied(s, d);
            }

            break;
        }

        default:
        {
            // To do: Complete this. Possibly use generic but slower operations.
            //        A simple approach would be to convert the color to ARGB and  
            //        use the logic above. Then convert back.
            EAW_FAIL_MSG("EA::Raster::SetPixelColorNoClip: Unimplemented pathway.");
            SetPixelSolidColor(pSurface, x, y, color);
            break;
        }
    }
    pSurface->Unlock();

    return 0;
}


EARASTER_API int SetPixelRGBA(ISurface* pSurface, int x, int y, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return SetPixelColorNoClip(pSurface, x, y, color);
}


EARASTER_API int SetPixelRGBANoClip(ISurface* pSurface, int x, int y, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return SetPixelColor(pSurface, x, y, color);
}


///////////////////////////////////////////////////////////////////////
// Rectanble functions
///////////////////////////////////////////////////////////////////////

// Applies no blending. Writes the color to the destination. Writes the color
// alpha to the destination if the destination has an alpha channel.
EARASTER_API int FillRectSolidColor(ISurface* pSurface, const Rect* pRect, const Color& color)
{
    Rect rectTemp;

    if(pRect)
    {
        if(IntersectRect(*pRect, pSurface->GetClipRect(), rectTemp))  // If the intersection is empty... nothing to do.
            pRect = &rectTemp;
        else
            return 0;
    } 
    else
        pRect = &pSurface->GetClipRect();

    pSurface->Lock();
    uint8_t* pRow = (uint8_t*)pSurface->GetData() + (pRect->y * pSurface->GetStride()) + (pSurface->GetPixelFormat().mBytesPerPixel * pRect->x);

    switch (pSurface->GetPixelFormat().mBytesPerPixel)
    {
        case 3: // kPixelFormatTypeRGB
        {
            const uint8_t r = (uint8_t)color.red();
            const uint8_t g = (uint8_t)color.green();
            const uint8_t b = (uint8_t)color.blue();

            for(int y = pRect->h; y; --y)
            {
                uint8_t* pixels = pRow;

                for(int x = pRect->w; x; --x, pixels += 3)
                {
                    pixels[0] = r;
                    pixels[1] = g;
                    pixels[2] = b;
                }

                pRow += pSurface->GetStride();
            }
            break;
        }

        case 4:
        {
            const uint32_t c32 = ConvertColor(color, pSurface->GetPixelFormat().mPixelFormatType);

            for(int y = pRect->h, w = pRect->w; y; --y)
            {
                memset4(pRow, c32, w);
                pRow += pSurface->GetStride();
            }
            break;
        }
    }
    pSurface->Unlock();

    return 0;
}


EARASTER_API int FillRectColor(ISurface* pSurface, const Rect* pRect, const Color& color)
{
    Rect rectTemp;

    if(pRect)
    {
        if(IntersectRect(*pRect, pSurface->GetClipRect(), rectTemp))  // If the intersection is empty... nothing to do.
            pRect = &rectTemp;
        else
            return 0;
    } 
    else
        pRect = &pSurface->GetClipRect();

    const int sA = color.alpha();

    pSurface->Lock();
    uint8_t* pRow = (uint8_t*)pSurface->GetData() + (pRect->y * pSurface->GetStride()) + (pSurface->GetPixelFormat().mBytesPerPixel * pRect->x);
    

    switch (pSurface->GetPixelFormat().mBytesPerPixel)
    {
        case 3: // kPixelFormatTypeRGB
        {
            // 7/22/09 CSidhall - This is luckily not used for would cause endian problems with the byte storing of pPixel[0] = sR...
            const int sR = color.red();
            const int sG = color.green();
            const int sB = color.blue();

            for(int y = pRect->h; y; --y)
            {
                uint8_t* pPixel = pRow;

                for(int x = pRect->w; x; --x, pPixel += 3)
                {
                    if(sA == 255)
                    {
                        pPixel[0] = sR;
                        pPixel[1] = sG;
                        pPixel[2] = sB;
                    }
                    else
                    {
                        const int dR = pPixel[0];
                        const int dG = pPixel[1];
                        const int dB = pPixel[2];

                        pPixel[0] = (uint8_t)(dR + (((sR - dR) * sA) >> 8));
                        pPixel[1] = (uint8_t)(dG + (((sG - dG) * sA) >> 8));
                        pPixel[2] = (uint8_t)(dB + (((sB - dB) * sA) >> 8));
                    }
                }

                pRow += pSurface->GetStride();
            }
            break;
        }

        case 4:
        {
            // const uint32_t c32 = ConvertColor(color, pSurface->GetPixelFormat().mPixelFormatType); Uh, I don't think this is what we want to do, is it?
            uint32_t s = color.rgb();  // We are assuming that the color is 32 bit ARGB here.

            // 7/22/09 CS -Added faster loop for the most frequent case. Grabbed from FillRectSolidColor()     
            if(sA == 255)     
            {
                for(int y = pRect->h, w = pRect->w; y; --y)
                {
                    memset4(pRow, s, w);
                    pRow += pSurface->GetStride();
                }
            }
            else
            {
                // We only support colors with premultiplied alpha
                s = PremultiplyColor(s);

                for(int y = pRect->h, w = pRect->w; y; --y)
                {
                    uint32_t* pPixel = (uint32_t*)pRow;

                    for(int x = w; x; --x, pPixel++)
                    {
                        // Get the destination color
                        const uint32_t d = *pPixel;

                        uint32_t a = d & 0xff000000;   

                        // 7/22/09 CSidhall -Added case for not blending when no background is there
                        if(a)
                        {
                            *pPixel = BlendARGB32Premultiplied(s, d);
                        }
                        else
                        {
                            // There is nothing in the background so we can copy directly without blending                       
                            *pPixel = s;        
                        }
                    }
                    pRow += pSurface->GetStride();
                }
            }    
            break;
        }
    }
    
    pSurface->Unlock();

    return 0;
}


// Draws a single-pPixel-thick rectangle outline.
EARASTER_API int RectangleColor(ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color)
{
    // Check visibility of clipping rectangle
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
        return 0;

    // Swap x1, x2 if required 
    if(x1 > x2)
    {
        int xtmp = x1;
        x1       = x2;
        x2       = xtmp;
    }

    // Swap y1, y2 if required 
    if(y1 > y2)
    {
        int ytmp = y1;
        y1       = y2;
        y2       = ytmp;
    }

    // Calculate width/height 
    const int w = x2 - x1;
    const int h = y2 - y1;

    // Sanity check 
    if((w < 0) || (h < 0))
        return 0;

    // Test for special cases of straight lines or single point 
    if(x1 == x2)
    {
        if(y1 == y2)
            return SetPixelColor(pSurface, x1, y1, color);
        else
            return VLineColor(pSurface, x1, y1, y2, color);
    }
    else if(y1 == y2)
        return HLineColor(pSurface, x1, x2, y1, color);

    // Draw rectangle 
    int result = 0;

    result |= HLineColor(pSurface, x1, x2, y1, color);
    result |= HLineColor(pSurface, x1, x2, y2, color);

    y1 += 1;
    y2 -= 1;

    if(y1 <= y2)
    {
        result |= VLineColor(pSurface, x1, y1, y2, color);
        result |= VLineColor(pSurface, x2, y1, y2, color);
    }

    return result;
}

EARASTER_API int RectangleColor(ISurface* pSurface, const EA::Raster::Rect& rect, const EA::Raster::Color& c)
{
    return RectangleColor(  pSurface, 
                            rect.x, 
                            rect.y,
                            rect.x + rect.w, 
                            rect.y + rect.h,
                            c);
}

EARASTER_API int RectangleRGBA(ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return RectangleColor(pSurface, x1, y1, x2, y2, color);
}

///////////////////////////////////////////////////////////////////////
// Line functions
///////////////////////////////////////////////////////////////////////

#define CLIP_LEFT_EDGE   0x1
#define CLIP_RIGHT_EDGE  0x2
#define CLIP_BOTTOM_EDGE 0x4
#define CLIP_TOP_EDGE    0x8
#define CLIP_INSIDE(a)   (!a)
#define CLIP_REJECT(a,b) (a & b)
#define CLIP_ACCEPT(a,b) (!(a | b))


static int GetClipCode(int x, int y, int left, int top, int right, int bottom)
{
    int code = 0;

    if(x < left)
        code |= CLIP_LEFT_EDGE;
    else if(x > right)
        code |= CLIP_RIGHT_EDGE;

    if(y < top)
        code |= CLIP_TOP_EDGE;
    else if(y > bottom)
        code |= CLIP_BOTTOM_EDGE;

    return code;
}


static int ClipLine(ISurface* pSurface, int* x1, int* y1, int* x2, int* y2)
{
    int    left, right, top, bottom;
    int    code1, code2;
    int    draw = 0;
    int    swaptmp;
    float  m;

    // Get clipping boundary 
    left   = pSurface->GetClipRect().x;
    right  = pSurface->GetClipRect().x + pSurface->GetClipRect().width() - 1;
    top    = pSurface->GetClipRect().y;
    bottom = pSurface->GetClipRect().y + pSurface->GetClipRect().height() - 1;

    while(1)
    {
        code1 = GetClipCode(*x1, *y1, left, top, right, bottom);
        code2 = GetClipCode(*x2, *y2, left, top, right, bottom);

        if(CLIP_ACCEPT(code1, code2))
        {
            draw = 1;
            break;
        }
        else if(CLIP_REJECT(code1, code2))
            break;
        else
        {
            if(CLIP_INSIDE(code1))
            {
                swaptmp = *x2;
                *x2     = *x1;
                *x1     = swaptmp;

                swaptmp = *y2;
                *y2     = *y1;
                *y1     = swaptmp;

                swaptmp = code2;
                code2   = code1;
                code1   = swaptmp;
            }

            if(*x2 != *x1)
                m = (*y2 - *y1) / (float) (*x2 - *x1);
            else
                m = 1.0f;

            if(code1 & CLIP_LEFT_EDGE)
            {
                *y1 += (int) ((left - *x1) * m);
                *x1 = left;
            }
            else if(code1 & CLIP_RIGHT_EDGE)
            {
                *y1 += (int) ((right - *x1) * m);
                *x1 = right;
            }
            else if(code1 & CLIP_BOTTOM_EDGE)
            {
                if(*x2 != *x1)
                    *x1 += (int) ((bottom - *y1) / m);

                *y1 = bottom;
            }
            else if(code1 & CLIP_TOP_EDGE)
            {
                if(*x2 != *x1)
                    *x1 += (int) ((top - *y1) / m);

                *y1 = top;
            }
        }
    }

    return draw;
}


// Draw a solid line in a given color. The alpha of the color is taken into
// consideration while drawing pixels, but the line itself is not anti-aliased. 
EARASTER_API int LineColor(ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color)
{
    int      pixx, pixy;
    int      x, y;
    int      dx, dy;
    int      ax, ay;
    int      sx, sy;
    int      swaptmp;
    uint8_t* pPixel;

    // Clip line and test if we have to draw.
    // Note by Paul Pedriana: It seems to me that clipping the line before rasterizing it isn't exactly right.
    if(!ClipLine(pSurface, &x1, &y1, &x2, &y2))
        return 0;

    // Test for special cases of straight lines or single point.
    if(x1 == x2)
    {
        if(y1 < y2)
            return VLineColor(pSurface, x1, y1, y2, color);
        else if(y1 > y2)
            return VLineColor(pSurface, x1, y2, y1, color);
        else
            return SetPixelColor(pSurface, x1, y1, color);
    }

    if(y1 == y2)
    {
        if(x1 < x2)
            return HLineColor(pSurface, x1, x2, y1, color);
        else if(x1 > x2)
            return HLineColor(pSurface, x2, x1, y1, color);
    }

    // Variable setup 
    dx = x2 - x1;
    dy = y2 - y1;
    sx = (dx >= 0) ? 1 : -1;
    sy = (dy >= 0) ? 1 : -1;

    if(color.alpha() == 0xff) // If solid...
    {
        dx     = (sx * dx) + 1;
        dy     = (sy * dy) + 1;
        pixx   = pSurface->GetPixelFormat().mBytesPerPixel;
        pixy   = pSurface->GetStride();
        
        pSurface->Lock();
        pPixel = (uint8_t*)pSurface->GetData() + (pixy * y1) + (pixx * x1);
        pixx  *= sx;
        pixy  *= sy;

        if(dx < dy)
        {
            swaptmp = dx;
            dx      = dy;
            dy      = swaptmp;

            swaptmp = pixx;
            pixx    = pixy;
            pixy    = swaptmp;
        }

        // Draw 
        x = 0;
        y = 0;

        switch (pSurface->GetPixelFormat().mPixelFormatType)
        {
            case kPixelFormatTypeRGB:
            {
                const int sR = color.red();
                const int sG = color.green();
                const int sB = color.blue();

                for(; x < dx; x++, pPixel += pixx)
                {
                    pPixel[0] = sR;
                    pPixel[1] = sG;
                    pPixel[2] = sB;

                    y += dy;

                    if(y >= dx)
                    {
                        y     -= dx;
                        pPixel += pixy;
                    }
                }
                break;
            }

            case kPixelFormatTypeARGB: // 1:1 mapping
            {
                const uint32_t c32 = color.rgb();

                for(; x < dx; x++, pPixel += pixx)
                {
                    *(uint32_t*)pPixel = c32;
                    y += dy;

                    if(y >= dx)
                    {
                        y      -= dx;
                        pPixel += pixy;
                    }
                }
                break;
            }

            default:
            {
                EAW_FAIL_MSG("EA::Raster::LineSolidColor: Unimplemented pathway.");
                // Fall through. Draw something wrong so you can at least see it.
            }
        }
        pSurface->Unlock();
    }
    else
    {
        // Alpha blending required - use single-pPixel blits.
        ax = abs(dx) << 1;
        ay = abs(dy) << 1;
        x  = x1;
        y  = y1;

        if(ax > ay)
        {
            int d = ay - (ax >> 1);

            while(x != x2)
            {
                SetPixelColorNoClip(pSurface, x, y, color);

                if(d > 0 || (d == 0 && sx == 1))
                {
                    y += sy;
                    d -= ax;
                }

                x += sx;
                d += ay;
            }
        }
        else
        {
            int d = ax - (ay >> 1);

            while(y != y2)
            {
                SetPixelColorNoClip(pSurface, x, y, color);

                if(d > 0 || ((d == 0) && (sy == 1)))
                {
                    x += sx;
                    d -= ay;
                }

                y += sy;
                d += ax;
            }
        }

        SetPixelColorNoClip(pSurface, x, y, color);
    }

    return 0;
}


EARASTER_API int LineRGBA(ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return LineColor(pSurface, x1, y1, x2, y2, color);
}


// Overwrite the destination with the source.
EARASTER_API int HLineSolidColor(ISurface* pSurface, int x1, int x2, int y, const Color& color)
{
    int left, right, top, bottom;

    // Check visibility of clipping rectangle.
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
        return 0;
    
    // Swap x1, x2 if required to ensure (x1 <= x2).
    if(x1 > x2)
    {
        const int tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    // Get clipping boundary and check visibility of hline.
    left = pSurface->GetClipRect().x;
    if(x2 < left)
        return 0;

    right = pSurface->GetClipRect().x + pSurface->GetClipRect().width() - 1;
    if(x1 > right)
        return 0;

    top    = pSurface->GetClipRect().y;
    bottom = pSurface->GetClipRect().y + pSurface->GetClipRect().height() - 1;
    if((y < top) || (y > bottom))
        return 0;

    // Clip x 
    if(x1 < left)
       x1 = left;

    if(x2 > right)
       x2 = right;

    // Calculate width 
    const int w = x2 - x1;

    pSurface->Lock();

    // More variable setup.
    uint8_t* pPixel = (uint8_t*)pSurface->GetData() + (pSurface->GetStride() * y) + (pSurface->GetPixelFormat().mBytesPerPixel * x1);

    // Draw 
    switch (pSurface->GetPixelFormat().mBytesPerPixel)
    {
        case 3:
        {
            const int r = color.red();
            const int g = color.green();
            const int b = color.blue();

            for(uint8_t* pPixelLast = pPixel + (3 * w); pPixel <= pPixelLast; pPixel += 3)
            {
                pPixel[0] = r;
                pPixel[1] = g;
                pPixel[2] = b;
            }

            break;
        }

        case 4:
        {
            const uint32_t c32 = color.rgb();

            for(uint8_t* pPixelLast = pPixel + (4 * w); pPixel <= pPixelLast; pPixel += 4)
                *(uint32_t*)pPixel = c32;

            break;
        }
    }
    pSurface->Unlock();    

    return 0;
}


EARASTER_API int HLineSolidRGBA(ISurface* pSurface, int x1, int x2, int y, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return HLineSolidColor(pSurface, x1, x2, y, color);
}


EARASTER_API int HLineColor(ISurface* pSurface, int x1, int x2, int y, const Color& color)
{
    if(color.alpha() == 0xff) // If the color is solid...
        return HLineSolidColor(pSurface, x1, x2, y, color);

    const Rect rect(x1, y, (x2 - x1) + 1, 1);

    return FillRectColor(pSurface, &rect, color);
}


EARASTER_API int HLineRGBA(ISurface* pSurface, int x1, int x2, int y, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return HLineColor(pSurface, x1, x2, y, color);
}


// Draw vertical line without alpha blending. Copy the color to the destination.
EARASTER_API int VLineSolidColor(ISurface* pSurface, int x, int y1, int y2, const Color& color)
{
    int left, right, top, bottom;

    // Check visibility of clipping rectangle
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
        return 0;
    
    // Swap y1, y2 if required to ensure y1 <= y2
    if(y1 > y2)
    {
        const int ytmp = y1;
        y1 = y2;
        y2 = ytmp;
    }

    // Get clipping boundary and check visibility of vline 
    left  = pSurface->GetClipRect().x;
    right = pSurface->GetClipRect().x + pSurface->GetClipRect().width() - 1;
    if((x < left) || (x > right))
        return 0;

    top = pSurface->GetClipRect().y;
    if(y2 < top)
        return 0;

    bottom = pSurface->GetClipRect().y + pSurface->GetClipRect().height() - 1;
    if(y1 > bottom)
        return 0;

    // Clip x 
    if(y1 < top)
       y1 = top;

    if(y2 > bottom)
       y2 = bottom;

    // Calculate height
    const int h = y2 - y1;

    // More variable setup 
    pSurface->Lock();
    uint8_t* pPixel     = ((uint8_t*) pSurface->GetData()) + (pSurface->GetStride() * y1) + (pSurface->GetPixelFormat().mBytesPerPixel * x);
    uint8_t* pPixelLast = pPixel + (pSurface->GetStride() * h);

    // Draw 
    switch (pSurface->GetPixelFormat().mBytesPerPixel)
    {
        case 3:
        {
            const int r = color.red();
            const int g = color.green();
            const int b = color.blue();

            for(; pPixel <= pPixelLast; pPixel += pSurface->GetStride())
            {
                pPixel[0] = r;
                pPixel[1] = g;
                pPixel[2] = b;
            }

            break;
        }

        case 4:
        {
            const uint32_t c32 = color.rgb();

            for(; pPixel <= pPixelLast; pPixel += pSurface->GetStride())
                *(uint32_t*)pPixel = c32;

            break;
        }
    }
    pSurface->Unlock();
    return 0;
}


EARASTER_API int VLineSolidRGBA(ISurface* pSurface, int  x, int y1, int y2, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return VLineSolidColor(pSurface, x, y1, y2, color);
}


// Draw vertical line with alpha blending enabled from the given color.
// Line is inclusive from the [x,y1] to [x,y2].
EARASTER_API int VLineColor(ISurface* pSurface, int  x, int y1, int y2, const Color& color)
{
    if(color.alpha() == 0xff) // If the color is solid...
        return VLineSolidColor(pSurface, x, y1, y2, color);

    const Rect rect(x, y1, 1, (y2 - y1) + 1);

    return FillRectColor(pSurface, &rect, color);
}


EARASTER_API int VLineRGBA(ISurface* pSurface, int  x, int y1, int y2, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return VLineColor(pSurface, x, y1, y2, color);
}


static int SetPixelColorWeightedNoClip(ISurface* pSurface, int x, int y, const Color& color, int alpha)
{
    // The Color class doesn't have the ability to set just the alpha channel, so we manually handle it.
    const uint32_t c32 = color.rgb();
    const int      a   = DivideBy255Rounded(color.alpha() * alpha);
    const Color    colorTemp((c32 & 0x00ffffff) | (a << 24));

    return SetPixelColorNoClip(pSurface, x, y, colorTemp);
}


static int SetPixelColorWeighted(ISurface* pSurface, int x, int y, const Color& color, int alpha)
{
    // The Color class doesn't have the ability to set just the alpha channel, so we manually handle it.
    const uint32_t c32 = color.rgb();
    const int      a   = DivideBy255Rounded(color.alpha() * alpha);
    const Color    colorTemp((c32 & 0x00ffffff) | (a << 24));

    return SetPixelColor(pSurface, x, y, colorTemp);
}


// Draw anitialiased line, also taking into account color alpha.
// Standard Wu AA line implementation.
EARASTER_API int AALineColor(ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color, bool bDrawEndpoint)
{
    int32_t  xx0, yy0, xx1, yy1;
    uint32_t intshift, erracc, erradj;
    uint32_t erracctmp, wgt, wgtcompmask;
    int      dx, dy, tmp, xdir, y0p1, x0pxdir;

    const uint32_t kAALevelCount = 256;
    const uint32_t kAABitCount   = 8;

    // Check visibility of clipping rectangle.
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
        return 0;

    // Clip line and test if we have to draw.
    // Note by Paul Pedriana: It seems to me that clipping the line before rasterizing it isn't exactly right.
    if(!ClipLine(pSurface, &x1, &y1, &x2, &y2))
        return 0;

    // Keep on working with 32bit numbers.
    xx0 = x1;
    yy0 = y1;
    xx1 = x2;
    yy1 = y2;

    // Reorder points if required.
    if(yy0 > yy1)
    {
        tmp = yy0;
        yy0 = yy1;
        yy1 = tmp;

        tmp = xx0;
        xx0 = xx1;
        xx1 = tmp;
    }

    // Calculate distance.
    dx = xx1 - xx0;
    dy = yy1 - yy0;

    // Adjust for negative dx and set xdir.
    if(dx >= 0)
        xdir = 1;
    else
    {
        xdir =  -1;
        dx   = -dx;
    }

    // Check for special cases that don't need anti-aliasing.
    if(dx == 0)
        return VLineColor(pSurface, x1, y1, y2, color);
    else if(dy == 0)
        return HLineColor(pSurface, x1, x2, y1, color);
    else if(dx == dy)
        return LineColor(pSurface, x1, y1, x2, y2, color);

    // General line orientation case.
    int result = 0;

    erracc      = 0;                 // Zero accumulator 
    intshift    = 32 - kAABitCount;  // # of bits by which to shift erracc to get intensity level 
    wgtcompmask = kAALevelCount - 1; // Mask used to flip all bits in an intensity weighting

    // Draw the initial pPixel.
    result |= SetPixelColorNoClip(pSurface, x1, y1, color);

    if(dy > dx)  // If y-major...
    {
        // y-major. Calculate 16-bit fixed point fractional part of a pPixel that
        // X advances every time Y advances 1 pPixel, truncating the result so that
        // we won't overrun the endpoint along the X axis.

        // Not-so-portable version: erradj = ((Uint64)dx << 32) / (Uint64)dy; 
        erradj = ((dx << 16) / dy) << 16;

        // Draw all pixels other than the first and last.
        x0pxdir = xx0 + xdir;

        while(--dy)
        {
            erracctmp = erracc;
            erracc   += erradj;

            if(erracc <= erracctmp)
            {
                // Rollover in error accumulator, x coord advances.
                xx0      = x0pxdir;
                x0pxdir += xdir;
            }

            yy0++; // y-major so always advance Y

            // The kAABitCount most significant bits of erracc give us the intensity
            // weighting for this pPixel, and the complement of the weighting for
            // the paired pPixel. 
            wgt = (erracc >> intshift) & 255;

            result |= SetPixelColorWeightedNoClip(pSurface, xx0,     yy0, color, 255 - wgt);
            result |= SetPixelColorWeightedNoClip(pSurface, x0pxdir, yy0, color,       wgt);
        }
    }
    else
    {
        // x-major line. Calculate 16-bit fixed-point fractional part of a pPixel
        // that Y advances each time X advances 1 pPixel, truncating the result so
        // that we won't overrun the endpoint along the X axis. 

        // Alternatively: erradj = ((Uint64)dy << 32) / (Uint64)dx; 
        erradj = ((dy << 16) / dx) << 16;

        // Draw all pixels other than the first and last.
        y0p1 = yy0 + 1;

        while(--dx)
        {
            erracctmp = erracc;
            erracc   += erradj;

            if(erracc <= erracctmp)
            {
                // Accumulator turned over, advance y.
                yy0 = y0p1;
                y0p1++;
            }

            xx0 += xdir; // x-major so always advance x.

            // The kAABitCount most significant bits of erracc give us the intensity
            // weighting for this pPixel, and the complement of the weighting for
            // the paired pPixel. 

            wgt = (erracc >> intshift) & 255;
            result |= SetPixelColorWeightedNoClip(pSurface, xx0,  yy0, color, 255 - wgt);
            result |= SetPixelColorWeightedNoClip(pSurface, xx0, y0p1, color,       wgt);
        }
    }

    if(bDrawEndpoint)
    {
        // Draw final pPixel, always exactly intersected by the line and doesn't need to be weighted. 
        result |= SetPixelColorNoClip(pSurface, x2, y2, color);
    }

    return result;
}


EARASTER_API int AALineColor(ISurface* pSurface, int x1, int y1, int x2, int y2, const Color& color)
{
    return AALineColor(pSurface, x1, y1, x2, y2, color, true);
}


EARASTER_API int AALineRGBA(ISurface* pSurface, int x1, int y1, int x2, int y2, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return AALineColor(pSurface, x1, y1, x2, y2, color, true);
}


///////////////////////////////////////////////////////////////////////////////
// Circle / Ellipse
///////////////////////////////////////////////////////////////////////////////
//special case tiny circle
int MicroCircleColor(ISurface* pSurface, const Point& center, int radius, const Color& c)
{
    //circle pixels determined by drawing circles in MS Paint.  Center is assumed to 
    //be one to the bottom left of the true center.
    //Remember, top left is 0,0
    Point p(center);
    ASSERT(radius <= 4);
    switch(radius)
    {
    case 4:
        {
            //in msPaint, a hollow radius 4 circle appears as 4 diagonal lines of 4 pixels,
            //with 4 extra pixels at the outer diagonals
            Point topLeft(center.x-3, center.y);    //will draw top left edge
            Point btmLeft(center.x-3, center.y+1);  //will draw bottom left edge
            Point topRight(center.x+1, center.y-3); //will draw top right edge
            Point btmRight(center.x+1, center.y+4); //will draw bottom right edge
            int i;
            for(i=0;i<4;++i)
            {
                SetPixelColor(pSurface, topLeft.x++, topLeft.y--, c);
                SetPixelColor(pSurface, btmLeft.x++, btmLeft.y++, c);
                SetPixelColor(pSurface, topRight.x++, topRight.y++, c);
                SetPixelColor(pSurface, btmRight.x++, btmRight.y--, c);
            }

            //outer diagonals
            SetPixelColor(pSurface, center.x-2, center.y-2, c);
            SetPixelColor(pSurface, center.x+3, center.y-2, c);
            SetPixelColor(pSurface, center.x-2, center.y+3, c);
            SetPixelColor(pSurface, center.x+3, center.y+3, c);

        }
        break;
    case 3:
        {
            //in msPaint, a hollow radius 4 circle appears as 4 diagonal lines of 4 pixels,
            //with 4 extra pixels at the outer diagonals
            Point topLeft(center.x-2, center.y);    //will draw top left edge
            Point btmLeft(center.x-2, center.y+1);  //will draw bottom left edge
            Point topRight(center.x+1, center.y-2); //will draw top right edge
            Point btmRight(center.x+1, center.y+3); //will draw bottom right edge
            int i;
            for(i=0;i<3;++i)
            {
                SetPixelColor(pSurface, topLeft.x++, topLeft.y--, c);
                SetPixelColor(pSurface, btmLeft.x++, btmLeft.y++, c);
                SetPixelColor(pSurface, topRight.x++, topRight.y++, c);
                SetPixelColor(pSurface, btmRight.x++, btmRight.y--, c);
            }
        }
        break;
    case 2:
        {
            //2 pixel radius is a square with no corners
            Point top(center.x, center.y-1);    //will draw top edge
            Point btm(center.x, center.y+2);    //will draw bottom edge
            Point right(center.x-1, center.y);  //will draw right edge
            Point left(center.x+2, center.y);   //will draw left edge
            int i;
            for(i=0;i<3;++i)
            {
                SetPixelColor(pSurface, top.x++, top.y, c);
                SetPixelColor(pSurface, btm.x++, btm.y, c);
                SetPixelColor(pSurface, right.x, right.y++, c);
                SetPixelColor(pSurface, left.x, left.y++, c);
            }
        }
        break;
    case 1:
        SetPixelColor(pSurface, center.x+1, center.y+1, c);
        SetPixelColor(pSurface, center.x+1, center.y-1, c);
        SetPixelColor(pSurface, center.x-1, center.y+1, c);
        SetPixelColor(pSurface, center.x-1, center.y-1, c);
        break;
    case 0:
    default:
        SetPixelColor(pSurface, center.x, center.y, c);
        break;
    }

    return 0;
}

//special case tiny filled circle
int FilledMicroCircleRGBA(ISurface* pSurface, const Point& center, int radius, const Color& c)
{
    //circle pixesl determined by drawing circles in MS Paint.  Center is assumed to 
    //be one to the bottom left of the true center.
    //Remember, top left is 0,0
    Point p(center);
    ASSERT(radius <= 4);
    switch(radius)
    {
    case 4:
        {
            //in msPaint, a radius 4 circle appears as a square with some 2 pixel lines on the outer sides
            EA::Raster::Rect bulk(center.x-2, center.y-2, 6,6);
            FillRectColor(pSurface, &bulk, c);
            
            //top
            SetPixelColor(pSurface, center.x, center.y-3, c);
            SetPixelColor(pSurface, center.x+1, center.y-3, c);

            //btm
            SetPixelColor(pSurface, center.x, center.y+4, c);
            SetPixelColor(pSurface, center.x+1, center.y+4, c);

            //left
            SetPixelColor(pSurface, center.x-3, center.y, c);
            SetPixelColor(pSurface, center.x-3, center.y+1, c);

            //right
            SetPixelColor(pSurface, center.x+4, center.y, c);
            SetPixelColor(pSurface, center.x+4, center.y+1, c);
        }
        break;
    case 3:
        {
            int i;
            int xStart = center.x-2;
            int xEnd = center.x+3;
            for(i=0;i<3;++i)
            {
                HLineColor(pSurface, xStart, xEnd, center.y-i, c);
                HLineColor(pSurface, xStart, xEnd, center.y+i+1, c);
                ++xStart;
                --xEnd;
            }
        }
        break;
    case 2:
        for(p.x = center.x-1;  p.x <= center.x+2; ++p.x)
        {
            for(p.y = center.y-1;  p.y <= center.y+2; ++p.y)
            {
                //do not draw the corners of the 'square'
                if( p.x == center.x-1 && p.y == center.y-1 ||
                    p.x == center.x+2 && p.y == center.y-1 ||
                    p.x == center.x-1 && p.y == center.y+2 ||
                    p.x == center.x+2 && p.y == center.y+2 )
                    continue;
                else
                    SetPixelColor(pSurface, p.x, p.y, c);
            }
        }
        break;
    case 1:
        SetPixelColor(pSurface, center.x+1, center.y+1, c);
        SetPixelColor(pSurface, center.x+1, center.y-1, c);
        SetPixelColor(pSurface, center.x-1, center.y+1, c);
        SetPixelColor(pSurface, center.x-1, center.y-1, c);
        //intentional fall through
    case 0:
    default:
        SetPixelColor(pSurface, center.x, center.y, c);
        break;
    }

    return 0;
}

// Draws a single-pixel-thick solid circle outline.
EARASTER_API int CircleColor(ISurface* pSurface, int x, int y, int radius, const Color& color)
{
    int     left, right, top, bottom;
    int     x1, y1, x2, y2;
    int     cx = 0;
    int     cy = radius;
    int     df = 1 - radius;
    int     d_e = 3;
    int     d_se = -2 * radius + 5;
    int     xpcx, xmcx, xpcy, xmcy;
    int     ypcy, ymcy, ypcx, ymcx;

    // Check visibility of clipping rectangle
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
        return 0;

    // Sanity check radius 
    if(radius < 0)
        return -1;

    // Special case for very small circles 
    if(radius <= 4)
        return MicroCircleColor(pSurface, Point(x, y), radius, color);

    // Get circle and clipping boundary and test if bounding box of circle is visible 
    x2   = x + radius;
    left = pSurface->GetClipRect().x;
    if(x2 < left)
        return 0;

    x1    = x - radius;
    right = pSurface->GetClipRect().x + pSurface->GetClipRect().width() - 1;

    if(x1 > right)
        return 0;

    y2  = y + radius;
    top = pSurface->GetClipRect().y;
    if(y2 < top)
        return 0;

    y1     = y - radius;
    bottom = pSurface->GetClipRect().y + pSurface->GetClipRect().height() - 1;
    if(y1 > bottom)
        return 0;

    int result = 0;

    if((color.alpha()) == 0xff) // If the color is solid...
    {
        do
        {
            ypcy = y + cy;
            ymcy = y - cy;

            if(cx > 0)
            {
                xpcx = x + cx;
                xmcx = x - cx;

                result |= SetPixelSolidColor(pSurface, xmcx, ypcy, color);
                result |= SetPixelSolidColor(pSurface, xpcx, ypcy, color);
                result |= SetPixelSolidColor(pSurface, xmcx, ymcy, color);
                result |= SetPixelSolidColor(pSurface, xpcx, ymcy, color);
            }
            else
            {
                result |= SetPixelSolidColor(pSurface, x, ymcy, color);
                result |= SetPixelSolidColor(pSurface, x, ypcy, color);
            }

            xpcy = x + cy;
            xmcy = x - cy;

            if((cx > 0) && (cx != cy))
            {
                ypcx = y + cx;
                ymcx = y - cx;

                result |= SetPixelSolidColor(pSurface, xmcy, ypcx, color);
                result |= SetPixelSolidColor(pSurface, xpcy, ypcx, color);
                result |= SetPixelSolidColor(pSurface, xmcy, ymcx, color);
                result |= SetPixelSolidColor(pSurface, xpcy, ymcx, color);
            }
            else if(cx == 0)
            {
                result |= SetPixelSolidColor(pSurface, xmcy, y, color);
                result |= SetPixelSolidColor(pSurface, xpcy, y, color);
            }

            // Update 
            if(df < 0)
            {
                df   += d_e;
                d_e  += 2;
                d_se += 2;
            }
            else
            {
                df   += d_se;
                d_e  += 2;
                d_se += 4;
                cy--;
            }
            cx++;
        } while(cx <= cy);
    } 
    else
    {
        do
        {
            ypcy = y + cy;
            ymcy = y - cy;

            if(cx > 0)
            {
                xpcx = x + cx;
                xmcx = x - cx;

                result |= SetPixelColor(pSurface, xmcx, ypcy, color);
                result |= SetPixelColor(pSurface, xpcx, ypcy, color);
                result |= SetPixelColor(pSurface, xmcx, ymcy, color);
                result |= SetPixelColor(pSurface, xpcx, ymcy, color);
            }
            else
            {
                result |= SetPixelColor(pSurface, x, ymcy, color);
                result |= SetPixelColor(pSurface, x, ypcy, color);
            }

            xpcy = x + cy;
            xmcy = x - cy;

            if((cx > 0) && (cx != cy))
            {
                ypcx = y + cx;
                ymcx = y - cx;

                result |= SetPixelColor(pSurface, xmcy, ypcx, color);
                result |= SetPixelColor(pSurface, xpcy, ypcx, color);
                result |= SetPixelColor(pSurface, xmcy, ymcx, color);
                result |= SetPixelColor(pSurface, xpcy, ymcx, color);
            }
            else if(cx == 0)
            {
                result |= SetPixelColor(pSurface, xmcy, y, color);
                result |= SetPixelColor(pSurface, xpcy, y, color);
            }

            // Update 
            if(df < 0)
            {
                df   += d_e;
                d_e  += 2;
                d_se += 2;
            }
            else
            {
                df   += d_se;
                d_e  += 2;
                d_se += 4;
                cy--;
            }
            cx++;
        } while(cx <= cy);
    }

    return result;
}


EARASTER_API int CircleRGBA(ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return CircleColor(pSurface, x, y, radius, color);
}


// Draws a segment of a circle outline given by a start and end angle in degrees.
EARASTER_API int ArcColor(ISurface* pSurface, int x, int y, int radius, int start, int end, const Color& color)
{
    int      left, right, top, bottom;
    int      x1, y1, x2, y2;
    int      cx = 0;
    int      cy = radius;
    int      df = 1 - radius;
    int      d_e = 3;
    int      d_se = -2 * radius + 5;
    int      xpcx, xmcx, xpcy, xmcy;
    int      ypcy, ymcy, ypcx, ymcx;

    // Check visibility of clipping rectangle.
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
        return 0;

    // Sanity check radius 
    if(radius < 0)
        return -1;

    // Special case for radius == 0: draw a point.
    if(radius == 0)
        return SetPixelColor(pSurface, x, y, color);

    // Fixup angles
    start = start % 360;
    end   = end   % 360;

    // Get arc's circle and clipping boundary and test if bounding box of circle is visible.
    x2   = x + radius;
    left = pSurface->GetClipRect().x;
    if(x2 < left)
        return 0;

    x1    = x - radius;
    right = pSurface->GetClipRect().x + pSurface->GetClipRect().width() - 1;
    if(x1 > right)
        return 0;

    y2   = y + radius;
    top = pSurface->GetClipRect().y;
    if(y2 < top)
        return 0;

    y1     = y - radius;
    bottom = pSurface->GetClipRect().y + pSurface->GetClipRect().height() - 1;
    if(y1 > bottom)
        return 0;

    // Draw arc 
    int result = 0;

    // Octant labelling
    //      
    //  \ 5 | 6 /           //
    //   \  |  /            //
    //  4 \ | / 7           //
    //     \|/              //
    //------+------ +x      //
    //     /|\              //
    //  3 / | \ 0           //
    //   /  |  \            //
    //  / 2 | 1 \           //
    //      +y              //

   // drawoct defines whether or not to keep drawing a given octant.
   // For example: 0x00111100 means we're drawing in octants 2-5
    uint8_t drawoct = 0; // 0x00000000

    // 0 <= start & end < 360; note that sometimes start > end - if so, arc goes back through 0.
    while(start < 0)
        start += 360;
    while(end < 0)
        end += 360;
    start %= 360;
    end   %= 360;

    // Now we find which octants we're drawing in.
    int    startoct = start / 45;
    int    endoct   = end / 45;
    int    oct      = startoct - 1; // we increment as first step in loop
    int    stopval_start = 0;
    int    stopval_end = 0; // what values of cx to stop at.
    double temp = 0;

    do {
        oct = (oct + 1) % 8;

        if(oct == startoct)
        {
            // need to compute stopval_start for this octant.  Look at picture above if this is unclear
            switch (oct) 
            {
                case 0:
                case 3:
                    temp = sin(start * kPi / 180);
                    break;
                case 1:
                case 6:
                    temp = cos(start * kPi / 180);
                    break;
                case 2:
                case 5:
                    temp = -cos(start * kPi / 180);
                    break;
                case 4:
                case 7:
                    temp = -sin(start * kPi / 180);
                    break;
            }

            temp *= radius;
            stopval_start = (int)temp; // always round down.

            // This isn't arbitrary, but requires graph paper to explain well.
            // The basic idea is that we're always changing drawoct after we draw, so we
            // stop immediately after we render the last sensible pPixel at x = (int)temp.

            // and whether to draw in this octant initially
            if(oct % 2)
                drawoct |= (1 << oct); // this is basically like saying drawoct[oct] = true, if drawoct were a bool array
            else
                drawoct &= 255 - (1 << oct); // this is basically like saying drawoct[oct] = false
        }

        if(oct == endoct)
        {
            // need to compute stopval_end for this octant
            switch (oct)
            {
                case 0:
                case 3:
                    temp = sin(end * kPi / 180);
                    break;
                case 1:
                case 6:
                    temp = cos(end * kPi / 180);
                    break;
                case 2:
                case 5:
                    temp = -cos(end * kPi / 180);
                    break;
                case 4:
                case 7:
                    temp = -sin(end * kPi / 180);
                    break;
            }

            temp *= radius;
            stopval_end = (int)temp;

            // and whether to draw in this octant initially
            if(startoct == endoct)
            {
                // note:      we start drawing, stop, then start again in this case
                // otherwise: we only draw in this octant, so initialize it to false, it will get set back to true
                if(start > end)
                {
                    // unfortunately, if we're in the same octant and need to draw over the whole circle, 
                    // we need to set the rest to true, because the while loop will end at the bottom.
                    drawoct = 255;
                }
                else
                    drawoct &= 255 - (1 << oct);
            }

            else if(oct % 2)
                drawoct &= 255 - (1 << oct);
            else
                drawoct |= (1 << oct);
        }
        else if(oct != startoct)    // already verified that it's != endoct
            drawoct |= (1 << oct);  // draw this entire segment

    } while(oct != endoct);
    
    // Now we have what octants to draw and when to draw them.

    if(color.alpha() == 0xff) // If color is solid (opaque)...
    {
        do
        {
            ypcy = y + cy;
            ymcy = y - cy;

            if(cx > 0)
            {
                xpcx = x + cx;
                xmcx = x - cx;

                // always check if we're drawing a certain octant before adding a pPixel to that octant.
                if(drawoct & 4)  result |= SetPixelSolidColor(pSurface, xmcx, ypcy, color); // drawoct & 4 = 22; drawoct[2]
                if(drawoct & 2)  result |= SetPixelSolidColor(pSurface, xpcx, ypcy, color);
                if(drawoct & 32) result |= SetPixelSolidColor(pSurface, xmcx, ymcy, color);
                if(drawoct & 64) result |= SetPixelSolidColor(pSurface, xpcx, ymcy, color);
            }
            else
            {
                if(drawoct & 6)  result |= SetPixelSolidColor(pSurface, x, ypcy, color); // 4 + 2; drawoct[2] || drawoct[1]
                if(drawoct & 96) result |= SetPixelSolidColor(pSurface, x, ymcy, color); // 32 + 64
            }
            
            xpcy = x + cy;
            xmcy = x - cy;

            if(cx > 0 && cx != cy)
            {
                ypcx = y + cx;
                ymcx = y - cx;

                if(drawoct & 8)   result |= SetPixelSolidColor(pSurface, xmcy, ypcx, color);
                if(drawoct & 1)   result |= SetPixelSolidColor(pSurface, xpcy, ypcx, color);
                if(drawoct & 16)  result |= SetPixelSolidColor(pSurface, xmcy, ymcx, color);
                if(drawoct & 128) result |= SetPixelSolidColor(pSurface, xpcy, ymcx, color);
            }
            else if(cx == 0)
            {
                if(drawoct & 24)  result |= SetPixelSolidColor(pSurface, xmcy, y, color); // 8 + 16
                if(drawoct & 129) result |= SetPixelSolidColor(pSurface, xpcy, y, color); // 1 + 128
            }

            // Update whether we're drawing an octant
            if(stopval_start == cx)
            {
                // works like an on-off switch because start & end may be in the same octant.
                if(drawoct & (1 << startoct))
                    drawoct &= 255 - (1 << startoct);                
                else
                    drawoct |= (1 << startoct);
            }

            if(stopval_end == cx)
            {
                if(drawoct & (1 << endoct))
                    drawoct &= 255 - (1 << endoct);
                else
                    drawoct |= (1 << endoct);
            }

            // Update pixels
            if(df < 0)
            {
                df   += d_e;
                d_e  += 2;
                d_se += 2;
            }
            else
            {
                df   += d_se;
                d_e  += 2;
                d_se += 4;
                cy--;
            }
            cx++;
        } while(cx <= cy);
    }
    else
    {
        do
        {
            ypcy = y + cy;
            ymcy = y - cy;

            if(cx > 0)
            {
                xpcx = x + cx;
                xmcx = x - cx;

                // Check if we're drawing a certain octant before adding a pPixel to that octant.
                if(drawoct & 4)  result |= SetPixelColor(pSurface, xmcx, ypcy, color);
                if(drawoct & 2)  result |= SetPixelColor(pSurface, xpcx, ypcy, color);
                if(drawoct & 32) result |= SetPixelColor(pSurface, xmcx, ymcy, color);
                if(drawoct & 64) result |= SetPixelColor(pSurface, xpcx, ymcy, color);
            }
            else
            {
                if(drawoct & 96) result |= SetPixelColor(pSurface, x, ymcy, color);
                if(drawoct & 6)  result |= SetPixelColor(pSurface, x, ypcy, color);
            }
            
            xpcy = x + cy;
            xmcy = x - cy;

            if(cx > 0 && cx != cy)
            {
                ypcx = y + cx;
                ymcx = y - cx;

                if(drawoct & 8)   result |= SetPixelColor(pSurface, xmcy, ypcx, color);
                if(drawoct & 1)   result |= SetPixelColor(pSurface, xpcy, ypcx, color);
                if(drawoct & 16)  result |= SetPixelColor(pSurface, xmcy, ymcx, color);
                if(drawoct & 128) result |= SetPixelColor(pSurface, xpcy, ymcx, color);
            }
            else if(cx == 0)
            {
                if(drawoct & 24)  result |= SetPixelColor(pSurface, xmcy, y, color);
                if(drawoct & 129) result |= SetPixelColor(pSurface, xpcy, y, color);
            }

            // Update whether we're drawing an octant
            if(stopval_start == cx)
            {
                // works like an on-off switch.  
                // This is just in case start & end are in the same octant.
                if(drawoct & (1 << startoct))
                    drawoct &= 255 - (1 << startoct);                
                else
                    drawoct |= (1 << startoct);
            }

            if(stopval_end == cx)
            {
                if(drawoct & (1 << endoct))
                    drawoct &= 255 - (1 << endoct);
                else
                    drawoct |= (1 << endoct);
            }

            // Update pixels
            if(df < 0)
            {
                df   += d_e;
                d_e  += 2;
                d_se += 2;
            }
            else
            {
                df   += d_se;
                d_e  += 2;
                d_se += 4;
                cy--;
            }
            cx++;
        } while(cx <= cy);
    }

    return result;
}


EARASTER_API int ArcRGBA(ISurface* pSurface, int x, int y, int radius, int start, int end, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return ArcColor(pSurface, x, y, radius, start, end, color);
}


EARASTER_API int AACircleColor(ISurface* pSurface, int x, int y, int radius, const Color& color)
{
    return AAEllipseColor(pSurface, x, y, radius, radius, color);
}


EARASTER_API int AACircleRGBA(ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return AAEllipseColor(pSurface, x, y, radius, radius, color);
}


EARASTER_API int FilledCircleColor(ISurface* pSurface, int x, int y, int radius, const Color& color)
{
    int left, right, top, bottom;
    int x1, y1, x2, y2;
    int cx = 0;
    int cy = radius;
    int ocx = (int)-1;
    int ocy = (int)-1;
    int df = 1 - radius;
    int d_e = 3;
    int d_se = -2 * radius + 5;
    int xpcx, xmcx, xpcy, xmcy;
    int ypcy, ymcy, ypcx, ymcx;

    // Check visibility of clipping rectangle
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
        return 0;

    // Sanity check radius 
    if(radius < 0)
        return -1;

    // Special case for very small circles
    if(radius <= 4)
        return FilledMicroCircleRGBA(pSurface, Point(x, y), radius, color);

    // Get circle and clipping boundary and test if bounding box of circle is visible.
    x2   = x + radius;
    left = pSurface->GetClipRect().x;
    if(x2 < left)
        return 0;

    x1    = x - radius;
    right = pSurface->GetClipRect().x + pSurface->GetClipRect().width() - 1;
    if(x1 > right)
        return 0;

    y2  = y + radius;
    top = pSurface->GetClipRect().y;
    if(y2 < top)
        return 0;

    y1     = y - radius;
    bottom = pSurface->GetClipRect().y + pSurface->GetClipRect().height() - 1;
    if(y1 > bottom)
        return 0;

    // Draw 
    int result = 0;

    do
    {
        xpcx = x + cx;
        xmcx = x - cx;
        xpcy = x + cy;
        xmcy = x - cy;

        if(ocy != cy)
        {
            if(cy > 0)
            {
                ypcy = y + cy;
                ymcy = y - cy;
                result |= HLineColor(pSurface, xmcx, xpcx, ypcy, color);
                result |= HLineColor(pSurface, xmcx, xpcx, ymcy, color);
            }
            else
                result |= HLineColor(pSurface, xmcx, xpcx, y, color);

            ocy = cy;
        }

        if(ocx != cx)
        {
            if(cx != cy)
            {
                if(cx > 0)
                {
                    ypcx = y + cx;
                    ymcx = y - cx;
                    result |= HLineColor(pSurface, xmcy, xpcy, ymcx, color);
                    result |= HLineColor(pSurface, xmcy, xpcy, ypcx, color);
                }
                else
                    result |= HLineColor(pSurface, xmcy, xpcy, y, color);
            }

            ocx = cx;
        }

        // Update 
        if(df < 0)
        {
            df   += d_e;
            d_e  += 2;
            d_se += 2;
        }
        else
        {
            df   += d_se;
            d_e  += 2;
            d_se += 4;
            cy--;
        }
        cx++;

    } while(cx <= cy);

    return result;
}


EARASTER_API int FilledCircleRGBA(ISurface* pSurface, int x, int y, int radius, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return FilledCircleColor(pSurface, x, y, radius, color);
}

// Draws an ellipse outline of a single pixel thick.
// The outline is not anti-aliased, but the color alpha is taken into 
// account for the pixels drawn.
EARASTER_API int EllipseColor(ISurface* pSurface, int x, int y, int rx, int ry, const Color& color)
{
    // To do: Make specialized code for small circles, as this will be commonly used.
    int left, right, top, bottom;
    int x1, y1, x2, y2;
    int ix, iy;
    int h, i, j, k;
    int oh, oi, oj, ok;
    int xmh, xph, ypk, ymk;
    int xmi, xpi, ymj, ypj;
    int xmj, xpj, ymi, ypi;
    int xmk, xpk, ymh, yph;

    // Check visibility of clipping rectangle
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
        return 0;

    // Sanity check radii 
    if((rx < 0) || (ry < 0))
        return -1;

    // Special case for rx=0 - draw a vline 
    if(rx == 0)
        return VLineColor(pSurface, x, y - ry, y + ry, color);

    // Special case for ry=0 - draw a hline 
    if(ry == 0)
        return HLineColor(pSurface, x - rx, x + rx, y, color);

    // Get circle and clipping boundary and test if bounding box of circle is visible.
    x2   = x + rx;
    left = pSurface->GetClipRect().x;
    if(x2 < left)
        return 0;

    x1    = x - rx;
    right = pSurface->GetClipRect().x + pSurface->GetClipRect().width() - 1;
    if(x1 > right)
        return 0;

    y2  = y + ry;
    top = pSurface->GetClipRect().y;
    if(y2 < top)
        return 0;

    y1     = y - ry;
    bottom = pSurface->GetClipRect().y + pSurface->GetClipRect().height() - 1;
    if(y1 > bottom)
        return 0;

    // CS - Added check if there is a clip risk
    bool clipFlag = false;
    clipFlag |= x1 < left;
    clipFlag |= x2 > right;
    clipFlag |= y1 < top;
    clipFlag |= y2 > bottom;

    // Draw 
    int result = 0;

    oh = oi = oj = ok = 0xFFFF;

    // Check alpha 
    if(color.alpha() == 0xff) // If color is solid (opaque)...
    {
        if(rx > ry)
        {
            ix = 0;
            iy = rx * 64;

            do
            {
                h = (ix + 32) >> 6;
                i = (iy + 32) >> 6;
                j = (h * ry) / rx;
                k = (i * ry) / rx;

                if(((ok != k) && (oj != k)) || ((oj != j) && (ok != j)) || (k != j))
                {
                    xph = x + h;
                    xmh = x - h;

                    if(k > 0)
                    {
                        ypk = y + k;
                        ymk = y - k;
                        
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xmh, ypk, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xph, ypk, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xmh, ymk, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xph, ymk, color, clipFlag);
                    }
                    else
                    {
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xmh, y, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xph, y, color, clipFlag);
                    }

                    ok  = k;
                    xpi = x + i;
                    xmi = x - i;

                    if(j > 0)
                    {
                        ypj = y + j;
                        ymj = y - j;
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xmi, ypj, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xpi, ypj, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xmi, ymj, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xpi, ymj, color, clipFlag);
                    }
                    else
                    {
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xmi, y, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xpi, y, color, clipFlag);
                    }

                    oj = j;
                }

                ix = ix + iy / rx;
                iy = iy - ix / rx;

            } while(i > h);
        }
        else
        {
            ix = 0;
            iy = ry * 64;

            do
            {
                h = (ix + 32) >> 6;
                i = (iy + 32) >> 6;
                j = (h * rx) / ry;
                k = (i * rx) / ry;

                if(((oi != i) && (oh != i)) || ((oh != h) && (oi != h) && (i != h)))
                {
                    xmj = x - j;
                    xpj = x + j;

                    if(i > 0)
                    {
                        ypi = y + i;
                        ymi = y - i;
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xmj, ypi, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xpj, ypi, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xmj, ymi, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xpj, ymi, color, clipFlag);
                    }
                    else
                    {
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xmj, y, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xpj, y, color, clipFlag);
                    }

                    oi  = i;
                    xmk = x - k;
                    xpk = x + k;

                    if(h > 0)
                    {
                        yph = y + h;
                        ymh = y - h;
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xmk, yph, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xpk, yph, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xmk, ymh, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xpk, ymh, color, clipFlag);
                    }
                    else
                    {
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xmk, y, color, clipFlag);
                        result |= SetPixelSolidColorAndUseClippingRect(pSurface, xpk, y, color, clipFlag);
                    }

                    oh = h;
                }

                ix = ix + iy / ry;
                iy = iy - ix / ry;

            } while(i > h);
        }
    } 
    else
    {
        if(rx > ry)
        {
            ix = 0;
            iy = rx * 64;

            do
            {
                h = (ix + 32) >> 6;
                i = (iy + 32) >> 6;
                j = (h * ry) / rx;
                k = (i * ry) / rx;

                if(((ok != k) && (oj != k)) || ((oj != j) && (ok != j)) || (k != j))
                {
                    xph = x + h;
                    xmh = x - h;

                    if(k > 0)
                    {
                        ypk = y + k;
                        ymk = y - k;
                        result |= SetPixelColorAndUseClippingRect(pSurface, xmh, ypk, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xph, ypk, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xmh, ymk, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xph, ymk, color, clipFlag);
                    }
                    else
                    {
                        result |= SetPixelColorAndUseClippingRect(pSurface, xmh, y, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xph, y, color, clipFlag);
                    }

                    ok  = k;
                    xpi = x + i;
                    xmi = x - i;

                    if(j > 0)
                    {
                        ypj = y + j;
                        ymj = y - j;
                        result |= SetPixelColorAndUseClippingRect(pSurface, xmi, ypj, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xpi, ypj, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xmi, ymj, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xpi, ymj, color, clipFlag);
                    }
                    else
                    {
                        result |= SetPixelColorAndUseClippingRect(pSurface, xmi, y, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xpi, y, color, clipFlag);
                    }

                    oj = j;
                }

                ix = ix + iy / rx;
                iy = iy - ix / rx;

            } while(i > h);
        }
        else
        {
            ix = 0;
            iy = ry * 64;

            do
            {
                h = (ix + 32) >> 6;
                i = (iy + 32) >> 6;
                j = (h * rx) / ry;
                k = (i * rx) / ry;

                if(((oi != i) && (oh != i)) || ((oh != h) && (oi != h) && (i != h)))
                {
                    xmj = x - j;
                    xpj = x + j;

                    if(i > 0)
                    {
                        ypi = y + i;
                        ymi = y - i;
                        result |= SetPixelColorAndUseClippingRect(pSurface, xmj, ypi, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xpj, ypi, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xmj, ymi, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xpj, ymi, color, clipFlag);
                    }
                    else
                    {
                        result |= SetPixelColorAndUseClippingRect(pSurface, xmj, y, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xpj, y, color, clipFlag);
                    }

                    oi  = i;
                    xmk = x - k;
                    xpk = x + k;

                    if(h > 0)
                    {
                        yph = y + h;
                        ymh = y - h;
                        result |= SetPixelColorAndUseClippingRect(pSurface, xmk, yph, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xpk, yph, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xmk, ymh, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xpk, ymh, color, clipFlag);
                    }
                    else
                    {
                        result |= SetPixelColorAndUseClippingRect(pSurface, xmk, y, color, clipFlag);
                        result |= SetPixelColorAndUseClippingRect(pSurface, xpk, y, color, clipFlag);
                    }

                    oh = h;
                }

                ix = ix + iy / ry;
                iy = iy - ix / ry;

            } while(i > h);
        }
    }

    return result;
}


EARASTER_API int EllipseRGBA(ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return EllipseColor(pSurface, x, y, rx, ry, color);
}


// Draws an anti-aliased ellipse (or circle) at the center given by x/y and radius given by rx/ry.
// To do: Implement special cases for tiny circles, as that will be what we are doing very often.
EARASTER_API int AAEllipseColor(ISurface* pSurface, int xc, int yc, int rx, int ry, const Color& color)
{
    int      left, right, top, bottom;
    int      x1,y1,x2,y2;
    int      i;
    int      a2, b2, ds, dt, dxt, t, s, d;
    int      x, y, xs, ys, dyt, od, xx, yy, xc2, yc2;
    float    cp;
    double   sab;
    uint8_t  weight, iweight;

    // Check visibility of clipping rectangle
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
        return 0;

    // Sanity check radii 
    if((rx < 0) || (ry < 0))
        return -1;

    // Special case for rx=0 - draw a vline 
    if(rx == 0)
        return VLineColor(pSurface, xc, yc - ry, yc + ry, color);

    // Special case for ry=0 - draw a hline 
    if(ry == 0)
        return HLineColor(pSurface, xc - rx, xc + rx, yc, color);

    // Get circle and clipping boundary and test if bounding box of circle is visible 
    x2   = xc + rx;
    left = pSurface->GetClipRect().x;
    if(x2 < left)
        return 0;

    x1    = xc - rx;
    right = pSurface->GetClipRect().x + pSurface->GetClipRect().width() - 1;
    if(x1 > right)
        return 0;

    y2  = yc + ry;
    top = pSurface->GetClipRect().y;
    if(y2 < top)
        return 0;

    y1     = yc - ry;
    bottom = pSurface->GetClipRect().y + pSurface->GetClipRect().height() - 1;
    if(y1 > bottom)
        return 0;
    
    // Variable setup
    a2 = rx * rx;
    b2 = ry * ry;

    ds = 2 * a2;
    dt = 2 * b2;

    xc2 = 2 * xc;
    yc2 = 2 * yc;

    sab = sqrt((double)(a2 + b2));
    od  = lrint(sab * 0.01) + 1; // introduce some overdraw
    dxt = lrint((double)a2 / sab) + od;

    t = 0;
    s = -2 * a2 * ry;
    d = 0;

    x = xc;
    y = yc - ry;

    // Draw
    int result = 0;

    // End points
    result |= SetPixelColor(pSurface, x,       y,       color);
    result |= SetPixelColor(pSurface, xc2 - x, y,       color);
    result |= SetPixelColor(pSurface, x,       yc2 - y, color);
    result |= SetPixelColor(pSurface, xc2 - x, yc2 - y, color);

    for(i = 1; i <= dxt; i++)
    {
        x--;
        d += t - b2;

        if(d >= 0)
            ys = y - 1;
        else if((d - s - a2) > 0)
        {
            if((2 * d - s - a2) >= 0)
                ys = y + 1;
            else
            {
                ys = y;
                y++;
                d -= s + a2;
                s += ds;
            }
        }
        else
        {
            y++;
            ys = y + 1;
            d -= s + a2;
            s += ds;
        }

        t -= dt;

        // Calculate alpha
        if(s != 0.0)
        {
            cp = (float) abs(d) / (float) abs(s);
            if(cp > 1.0)
                cp = 1.0;
        }
        else
            cp = 1.0;

        // Calculate weights
        weight = (uint8_t) (cp * 255);
        iweight = 255 - weight;

        // Upper half
        xx = xc2 - x;
        result |= SetPixelColorWeighted(pSurface,  x, y, color, iweight);
        result |= SetPixelColorWeighted(pSurface, xx, y, color, iweight);

        result |= SetPixelColorWeighted(pSurface,  x, ys, color, weight);
        result |= SetPixelColorWeighted(pSurface, xx, ys, color, weight);

        // Lower half
        yy = yc2 - y;
        result |= SetPixelColorWeighted(pSurface,  x, yy, color, iweight);
        result |= SetPixelColorWeighted(pSurface, xx, yy, color, iweight);

        yy = yc2 - ys;
        result |= SetPixelColorWeighted(pSurface,  x, yy, color, weight);
        result |= SetPixelColorWeighted(pSurface, xx, yy, color, weight);
    }

    // Replaces original approximation code dyt = abs(y - yc);
    dyt = lrint((double)b2 / sab ) + od;    
    
    for(i = 1; i <= dyt; i++)
    {
        y++;
        d -= s + a2;

        if(d <= 0)
            xs = x + 1;
        else if((d + t - b2) < 0)
        {
            if((2 * d + t - b2) <= 0)
                xs = x - 1;
            else
            {
                xs = x;
                x--;
                d += t - b2;
                t -= dt;
            }
        }
        else
        {
            x--;
            xs = x - 1;
            d += t - b2;
            t -= dt;
        }

        s += ds;

        // Calculate alpha
        if(t != 0.0)
        {
            cp = (float) abs(d) / (float) abs(t);
            if(cp > 1.0)
                cp = 1.0;
        }
        else
            cp = 1.0;

        // Calculate weight
        weight = (uint8_t) (cp * 255);
        iweight = 255 - weight;

        // Left half
        xx = xc2 - x;
        yy = yc2 - y;
        result |= SetPixelColorWeighted(pSurface,  x, y, color, iweight);
        result |= SetPixelColorWeighted(pSurface, xx, y, color, iweight);

        result |= SetPixelColorWeighted(pSurface,  x, yy, color, iweight);
        result |= SetPixelColorWeighted(pSurface, xx, yy, color, iweight);

        // Right half
        xx = 2 * xc - xs;
        result |= SetPixelColorWeighted(pSurface, xs, y, color, weight);
        result |= SetPixelColorWeighted(pSurface, xx, y, color, weight);

        result |= SetPixelColorWeighted(pSurface, xs, yy, color, weight);
        result |= SetPixelColorWeighted(pSurface, xx, yy, color, weight);

    }

    return result;
}


EARASTER_API int AAEllipseRGBA(ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return AAEllipseColor(pSurface, x, y, rx, ry, color);
}


// Draws a solid color ellipse (or circle) at the center given by x/y and radius given by rx/ry.
// To do: Implement special cases for tiny circles, as that will be what we are doing very often.
EARASTER_API int FilledEllipseColor(ISurface* pSurface, int x, int y, int rx, int ry, const Color& color)
{
    int left, right, top, bottom;
    int x1, y1, x2, y2;
    int ix, iy;
    int h, i, j, k;
    int oh, oi, oj, ok;
    int xmh, xph;
    int xmi, xpi;
    int xmj, xpj;
    int xmk, xpk;

    // Check visibility of clipping rectangle
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
        return 0;

    // Sanity check radii 
    if((rx < 0) || (ry < 0))
        return -1;

    // Special case for rx=0: draw a vline 
    if(rx == 0)
        return VLineColor(pSurface, x, y - ry, y + ry, color);

    // Special case for ry=0: draw an hline 
    if(ry == 0)
        return HLineColor(pSurface, x - rx, x + rx, y, color);

    // Get circle and clipping boundary and test if bounding box of circle is visible.
    x2   = x + rx;
    left = pSurface->GetClipRect().x;
    if(x2 < left)
        return 0;

    x1    = x - rx;
    right = pSurface->GetClipRect().x + pSurface->GetClipRect().width() - 1;
    if(x1 > right)
        return 0;

    y2  = y + ry;
    top = pSurface->GetClipRect().y;
    if(y2 < top)
        return 0;

    y1     = y - ry;
    bottom = pSurface->GetClipRect().y + pSurface->GetClipRect().height() - 1;
    if(y1 > bottom)
        return 0;

    // Draw 
    int result = 0;

    oh = oi = oj = ok = 0xffff;

    if(rx > ry)
    {
        ix = 0;
        iy = rx * 64;

        do {
            h = (ix + 32) >> 6;
            i = (iy + 32) >> 6;
            j = (h * ry) / rx;
            k = (i * ry) / rx;

            if((ok != k) && (oj != k))
            {
                xph = x + h;
                xmh = x - h;

                if(k > 0)
                {
                    result |= HLineColor(pSurface, xmh, xph, y + k, color);
                    result |= HLineColor(pSurface, xmh, xph, y - k, color);
                } 
                else
                    result |= HLineColor(pSurface, xmh, xph, y, color);

                ok = k;
            }

            if((oj != j) && (ok != j) && (k != j))
            {
                xmi = x - i;
                xpi = x + i;

                if(j > 0)
                {
                    result |= HLineColor(pSurface, xmi, xpi, y + j, color);
                    result |= HLineColor(pSurface, xmi, xpi, y - j, color);
                }
                else
                    result |= HLineColor(pSurface, xmi, xpi, y, color);

                oj = j;
            }

            ix += (iy / rx);
            iy -= (ix / rx);

        } while(i > h);
    }
    else
    {
        ix = 0;
        iy = ry * 64;

        do
        {
            h = (ix + 32) >> 6;
            i = (iy + 32) >> 6;
            j = (h * rx) / ry;
            k = (i * rx) / ry;

            if((oi != i) && (oh != i))
            {
                xmj = x - j;
                xpj = x + j;

                if(i > 0)
                {
                    result |= HLineColor(pSurface, xmj, xpj, y + i, color);
                    result |= HLineColor(pSurface, xmj, xpj, y - i, color);
                }
                else
                    result |= HLineColor(pSurface, xmj, xpj, y, color);

                oi = i;
            }

            if((oh != h) && (oi != h) && (i != h))
            {
                xmk = x - k;
                xpk = x + k;

                if(h > 0)
                {
                    result |= HLineColor(pSurface, xmk, xpk, y + h, color);
                    result |= HLineColor(pSurface, xmk, xpk, y - h, color);
                }
                else
                    result |= HLineColor(pSurface, xmk, xpk, y, color);

                oh = h;
            }

            ix += (iy / ry);
            iy -= (ix / ry);

        } while(i > h);
    }

    return result;
}


EARASTER_API int FilledEllipseRGBA(ISurface* pSurface, int x, int y, int rx, int ry, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return FilledEllipseColor(pSurface, x, y, rx, ry, color);
}



// Low-speed float pie-calc implementation by drawing polygons/lines.
static int PieColorInternal(ISurface* pSurface, int x, int y, int radius, int start, int end, const Color& color, bool bFilled)
{
    int     left, right, top, bottom;
    int     x1, y1, x2, y2;
    int     result;
    double  angle, start_angle, end_angle;
    double  deltaAngle;
    double  dr;
    int     posX, posY;
    int     numpoints, i;
    int     *vx, *vy;

    // Check visibility of clipping rectangle
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
           return 0;

    // Sanity check radii 
    if(radius < 0)
        return -1;

    // Fixup angles
    start = start % 360;
    end   = end   % 360;

    // Special case for radius=0: draw a point 
    if(radius == 0)
        return SetPixelColor(pSurface, x, y, color);

    // Clip against circle, not pie (not 100% optimal).
    // Get pie's circle and clipping boundary and test if bounding box of circle is visible

    x2   = x + radius;
    left = pSurface->GetClipRect().x;
    if(x2 < left)
           return 0;

    x1    = x - radius;
    right = pSurface->GetClipRect().x + pSurface->GetClipRect().width() - 1;
    if(x1 > right)
           return 0;

    y2  = y + radius;
    top = pSurface->GetClipRect().y;
    if(y2 < top)
           return 0;

    y1     = y - radius;
    bottom = pSurface->GetClipRect().y + pSurface->GetClipRect().height() - 1;
    if(y1 > bottom)
           return 0;

    // Variable setup 
    dr          = (double) radius;
    deltaAngle  = 3.0 / dr;
    start_angle = (double) start * (2.0 * kPi / 360.0);
    end_angle   = (double) end   * (2.0 * kPi / 360.0);
    if(start > end)
        end_angle += (2.0 * kPi);

    // Count points (rather than calculate it)
    numpoints = 1;
    angle     = start_angle;
    while(angle <= end_angle)
    {
        angle += deltaAngle;
        numpoints++;
    }

    // Check size of array
    if(numpoints == 1)
        return SetPixelColor(pSurface, x, y, color);
    else if(numpoints == 2)
    {
        posX = x + (int) (dr * cos(start_angle));
        posY = y + (int) (dr * sin(start_angle));

        return LineColor(pSurface, x, y, posX, posY, color);
    }

    // Allocate vertex array
    // To do: Make this use alloca memory for all but large numbers of points.
    // We use stack space for the encoding where possible.
    // #if defined(EA_PLATFORM_WINDOWS) || defined(EA_PLATFORM_UNIX)
    //    const size_t kMaxStackSize = 4096;
    //#else
    //    const size_t kMaxStackSize = 1024;
    //#endif
    // Use alloca()

    vx = EAWEBKIT_NEW("PieColor Points") int[2 * numpoints]; //WTF::fastNewArray<int>(2 * numpoints);
    if(vx == NULL)
        return -1;

    vy = vx + numpoints;

    // Center
    vx[0] = x;
    vy[0] = y;

    // Calculate and store vertices
    i     = 1;
    angle = start_angle;

    while(angle <= end_angle)
    {
        vx[i] = x + (int) (dr * cos(angle));
        vy[i] = y + (int) (dr * sin(angle));
        angle += deltaAngle;
        i++;
    }

    // Draw
    if(bFilled)
        result = FilledPolygonColor(pSurface, vx, vy, numpoints, color);
    else
        result = PolygonColor(pSurface, vx, vy, numpoints, color);

    // Free vertex array
    EAWEBKIT_DELETE[] vx;//WTF::fastDeleteArray<int> (vx);

    return result;
}


// Draws a pie outline for a circle of the given x/y/radius and the start and end angle in degress.
EARASTER_API int PieColor(ISurface* pSurface, int x, int y, int radius, int start, int end, const Color& color) 
{
    return PieColorInternal(pSurface, x, y, radius, start, end, color, false);
}


// Draws a pie outline for a circle of the given x/y/radius and the start and end angle in degress.
EARASTER_API int PieRGBA(ISurface* pSurface, int x, int y, int radius, int start, int end, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return PieColorInternal(pSurface, x, y, radius, start, end, color, false);
}


// Draws a filled pie for a circle of the given x/y/radius and the start and end angle in degress.
EARASTER_API int FilledPieColor(ISurface* pSurface, int x, int y, int radius, int start, int end, const Color& color)
{
    return PieColorInternal(pSurface, x, y, radius, start, end, color, true);
}


// Draws a filled pie for a circle of the given x/y/radius and the start and end angle in degress.
EARASTER_API int FilledPieRGBA(ISurface* pSurface, int x, int y, int radius, int start, int end, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return PieColorInternal(pSurface, x, y, radius, start, end, color, true);
}



///////////////////////////////////////////////////////////////////////////////
// Polygon
///////////////////////////////////////////////////////////////////////////////

// SimpleTriangle
// Draws a triangle that is a tiny oriented arrow head, like that used in GUIs to 
// implement expandable lists, drop down button arrows, scroll bar button directions.
// The x/y position refers to where the tip of the triange is.
//
// Example of a SimpleTriangle of (size = 2) and (orientation = kORight):
//           *
//           * *    <--- tip
//           *
// Example of a SimpleTriangle of (size = 3) and (orientation = kOUp):
//
//           *      <--- tip
//         * * *
//       * * * * *
//
EARASTER_API int SimpleTriangle(ISurface* pSurface, int x, int y, int size, Orientation o, const Color& color)
{
    int result = 0;

    switch (o)
    {
        case kOLeft:
            for(int i = 0; i < size; ++i, ++x)
                result |= VLineColor(pSurface, x, y - i, y + i, color);
            break;

        case kOUp:
            for(int i = 0; i < size; ++i, ++y)
                result |= HLineColor(pSurface, x - i, x + i, y, color);
            break;

        case kORight:
            for(int i = 0; i < size; ++i, --x)
                result |= VLineColor(pSurface, x, y - i, y + i, color);
            break;

        case kODown:
            for(int i = 0; i < size; ++i, --y)
                result |= HLineColor(pSurface, x - i, x + i, y, color);
            break;
    }

    return result;
}


EARASTER_API int TrigonColor(ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color)
{
    const int vx[3] = { x1, x2, x3 }; 
    const int vy[3] = { y1, y2, y3 };

    return PolygonColor(pSurface, vx, vy, 3, color);
}


EARASTER_API int TrigonRGBA(ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a)
{
    const int vx[3] = { x1, x2, x3 }; 
    const int vy[3] = { y1, y2, y3 };

    return PolygonRGBA(pSurface, vx, vy, 3, r, g, b, a);
}                                 


EARASTER_API int AATrigonColor(ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color)
{
    const int vx[3] = { x1, x2, x3 }; 
    const int vy[3] = { y1, y2, y3 };
  
    return AAPolygonColor(pSurface, vx, vy, 3, color);
}


EARASTER_API int AATrigonRGBA(ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a)
{
    const int vx[3] = { x1, x2, x3 }; 
    const int vy[3] = { y1, y2, y3 };
 
    return AAPolygonRGBA(pSurface, vx, vy, 3, r, g, b, a);
}                                   


EARASTER_API int FilledTrigonColor(ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color)
{
    const int vx[3] = { x1, x2, x3 }; 
    const int vy[3] = { y1, y2, y3 };

    return FilledPolygonColor(pSurface, vx, vy, 3, color);
}


EARASTER_API int FilledTrigonRGBA(ISurface* pSurface, int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a)
{
    const int vx[3] = { x1, x2, x3 }; 
    const int vy[3] = { y1, y2, y3 };

    return FilledPolygonRGBA(pSurface, vx, vy, 3, r, g, b, a);
}



EARASTER_API int PolygonColor(ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color)
{
    const int *x1, *y1, *x2, *y2;

    // Check visibility of clipping rectangle
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
       return 0;

    // Sanity check 
    if(n < 3)
        return -1;

    // Pointer setup 
    x1 = x2 = vx;
    y1 = y2 = vy;
    x2++;
    y2++;

    // Draw 
    int result = 0;

    for(int i = 1; i < n; i++)
    {
        result |= LineColor(pSurface, *x1, *y1, *x2, *y2, color);

        x1 = x2++;
        y1 = y2++;
    }

    result |= LineColor(pSurface, *x1, *y1, *vx, *vy, color);

    return result;
}


EARASTER_API int PolygonRGBA(ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return PolygonColor(pSurface, vx, vy, n, color);
}


EARASTER_API int AAPolygonColor(ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color)
{
    const int *x1, *y1, *x2, *y2;

    // Check visibility of clipping rectangle
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
           return 0;

    // Sanity check 
    if(n < 3)
        return -1;

    // Pointer setup 
    x1 = x2 = vx;
    y1 = y2 = vy;
    x2++;
    y2++;

    // Draw 
    int result = 0;

    for(int i = 1; i < n; i++)
    {
        result |= AALineColor(pSurface, *x1, *y1, *x2, *y2, color, false);

        x1 = x2++;
        y1 = y2++;
    }

    result |= AALineColor(pSurface, *x1, *y1, *vx, *vy, color, false);

    return result;
}


EARASTER_API int AAPolygonRGBA(ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return AAPolygonColor(pSurface, vx, vy, n, color);
}


//+ 5/11/09 CSidhall - Added shell sort for FilledPolygonColorMT(). 
// The shell sort was invented by Donald Shell and first published in 1959.
// Might move this to EAWebkit internal but it only is used here currently and  
// this version is specialized to just the int type.
static void ShellSort(int* pBuffer, const int nCount)
{
    // Use a partial increment table here as we don't expect large nCounts.
    // Marcin Ciura, Best Increments for the Average Case of Shellsort, 13th International Symposium on Fundamentals of Computation 
    const int INC_TABLE_SIZE = 8;  
    const int incTable[INC_TABLE_SIZE] ={701, 301, 132, 57 ,23, 10, 4, 1}; 
  
    for(int k = 0; k < INC_TABLE_SIZE; k++)
    {
        int inc = incTable[k];           
        if(inc > nCount)
           continue;

        for(int i=0; i < nCount; i++)
        {
            int j = i;
            int value = pBuffer[i];
            bool swap = false;
            while((j >= inc) && (pBuffer[j-inc] > value))
            {
                pBuffer[j] = pBuffer[j - inc];
                j = j - inc;
                swap = true;
            }
            if(swap)
                pBuffer[j] = value;
        }
    }
}
//- CS

// Global vertex array to use if optional parameters are not given in polygon calls.
// To do: Get rid of this global vertex array stuff along with the int** polyInts, int* polyAllocated arguments.
//        The purpose of these variables is to deal with the case of the user not calling thus function simultaneously
//        from multiple threads. We won't be using multiple threads at all.
static int* gfxPrimitivesPolyIntsGlobal      = NULL;
static int  gfxPrimitivesPolyAllocatedGlobal = 0;

// Returns 0 if OK.
// This polygon is not necessarily convex.
// The last two parameters are optional; but required for multithreaded operation.
EARASTER_API int FilledPolygonColorMT(ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color, int** polyInts, int* polyAllocated)
{
    int  i;
    int  y, xa, xb;
    int  x1, y1;
    int  x2, y2;
    int* gfxPrimitivesPolyInts = NULL;
    int  gfxPrimitivesPolyAllocated = 0;

    // Check visibility of clipping rectangle
    if((pSurface->GetClipRect().width() == 0) || (pSurface->GetClipRect().height() == 0))
           return 0;

    // Sanity check number of edges
    if(n < 3)
        return -1;
     
    // Map polygon cache  
    if((polyInts == NULL) || (polyAllocated == NULL))
    {
       // Use global cache
       gfxPrimitivesPolyInts      = gfxPrimitivesPolyIntsGlobal;
       gfxPrimitivesPolyAllocated = gfxPrimitivesPolyAllocatedGlobal;
    }
    else
    {
       // Use local cache
       gfxPrimitivesPolyInts      = *polyInts;
       gfxPrimitivesPolyAllocated = *polyAllocated;
    }

    // Allocate temp array, only grow array.
    // To do: Fix the memory usage here to be non-abusive and to not use malloc and to use 
    //        local memory when possible and not to leak memory.
    //        Probably implement a generic allocator interface for this module.
    if(gfxPrimitivesPolyAllocated == 0)
        gfxPrimitivesPolyInts = (int*)malloc(sizeof(int) * n);
    else if(gfxPrimitivesPolyAllocated < n)
        gfxPrimitivesPolyInts = (int*)realloc(gfxPrimitivesPolyInts, sizeof(int) * n);

    // Check temp array
    if(gfxPrimitivesPolyInts)       
        gfxPrimitivesPolyAllocated = n;

    // Update cache variables
    if((polyInts == NULL) || (polyAllocated == NULL))
    { 
        gfxPrimitivesPolyIntsGlobal      = gfxPrimitivesPolyInts;
        gfxPrimitivesPolyAllocatedGlobal = gfxPrimitivesPolyAllocated;
    }
    else
    {
        *polyInts      = gfxPrimitivesPolyInts;
        *polyAllocated = gfxPrimitivesPolyAllocated;
    }

    // Check temp array again
    if(gfxPrimitivesPolyInts == NULL)       
        return -1;

    // Determine Y maxima 
    int miny = vy[0];
    int maxy = vy[0];

    for(i = 1; (i < n); i++)
    {
        if(vy[i] < miny)
            miny = vy[i];
        else if(vy[i] > maxy)
            maxy = vy[i];
    }

    // Draw, scanning y 
    int result = 0;

    for(y = miny; y <= maxy; y++) // For each horizontal scan line from min y to max y...
    {
        int intCount = 0;
        int ind1, ind2;

        for(i = 0; i < n; i++)
        {
            if(i)
            {
                ind1 = i - 1;
                ind2 = i;
            }
            else
            {
                ind1 = n - 1;
                ind2 = 0;
            }

            y1 = vy[ind1];
            y2 = vy[ind2];

            if(y1 < y2)
            {
                x1 = vx[ind1];
                x2 = vx[ind2];
            }
            else if(y1 > y2)
            {
                y2 = vy[ind1];
                y1 = vy[ind2];
                x2 = vx[ind1];
                x1 = vx[ind2];
            }
            else
                continue;

            // Add all points that are at the current y position. Store the points in 16:16 fixed point.
            if( ((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)) )
                gfxPrimitivesPolyInts[intCount++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
        }

//+5/10/09 Sidhall - Removed dependency on eastl sort    
// Original code:
//        if(intCount <= 6)
//            eastl::bubble_sort(gfxPrimitivesPolyInts, gfxPrimitivesPolyInts + intCount);
//        else
//            eastl::quick_sort(gfxPrimitivesPolyInts, gfxPrimitivesPolyInts + intCount);
// New code: 
// Note: As this sort is per scan line, intCount should be small so a simple ShellSort should be suffient. 
//       Could also add a merge sort here if it turns out that intCount can be large but currently it seems 
//       to be just in the = 2 range on many main sites

        ShellSort(gfxPrimitivesPolyInts, intCount);
//- CS

        for(i = 0; i < intCount; i += 2)
        {
            xa = gfxPrimitivesPolyInts[i + 0] + 1;
            xb = gfxPrimitivesPolyInts[i + 1] - 1;
            xa = (xa >> 16) + ((xa & 32768) >> 15); // Convert 16:16 fixed point to 16 bit int.
            xb = (xb >> 16) + ((xb & 32768) >> 15);

            result |= HLineColor(pSurface, xa, xb, y, color);
        }
    }

    return result;
}


EARASTER_API int FilledPolygonRGBAMT(ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a, int** polyInts, int* polyAllocated)
{
    const Color color(r, g, b, a);

    return FilledPolygonColorMT(pSurface, vx, vy, n, color, polyInts, polyAllocated);
}


// Standard versions are calling multithreaded versions with NULL cache parameters.
EARASTER_API int FilledPolygonColor(ISurface* pSurface, const int* vx, const int* vy, int n, const Color& color)
{
    return FilledPolygonColorMT(pSurface, vx, vy, n, color, NULL, NULL);
}


EARASTER_API int FilledPolygonRGBA(ISurface* pSurface, const int* vx, const int* vy, int n, int r, int g, int b, int a)
{
    const Color color(r, g, b, a);

    return FilledPolygonColorMT(pSurface, vx, vy, n, color, NULL, NULL);
}



///////////////////////////////////////////////////////////////////////
// Resampling
///////////////////////////////////////////////////////////////////////

struct ColorRGBA
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// This function assumes the destination surface is the right size.
static int ShrinkSurfaceRGBA(ISurface* pSource, ISurface* pDest, int factorX, int factorY)
{
    int       x, y, dx, dy, sgap, dgap, ra, ga, ba, aa;
    int       n_average;
    ColorRGBA *sp, *osp, *oosp;
    ColorRGBA *dp;

    // Precalculate division factor
    n_average = factorX * factorY;
   
    // Scan destination
    sp   = (ColorRGBA*)pSource->GetData();
    sgap = pSource->GetStride() - (pSource->GetWidth() * 4);

    pDest->Lock();
    dp   = (ColorRGBA*)pDest->GetData();
    dgap = pDest->GetStride() - (pDest->GetWidth() * 4);

    for (y = 0; y < pDest->GetHeight(); y++)
    {
        osp = sp;

        for (x = 0; x < pDest->GetWidth(); x++)
        {
            // Trace out source box and accumulate
            oosp = sp;
            ra   = ga = ba = aa = 0;

            for (dy = 0; dy < factorY; dy++)
            {
                for (dx = 0; dx < factorX; dx++)
                {
                    ra += sp->r;
                    ga += sp->g;
                    ba += sp->b;
                    aa += sp->a;
              
                    sp++;
                } // pSource dx loop

                sp = (ColorRGBA*)((uint8_t*)sp + (pSource->GetStride() - (4 * factorX))); // next y
            } // pSource dy loop

            // next box-x
            sp = (ColorRGBA*)((uint8_t*)oosp + (4 * factorX));
                    
            // Store result in destination
            dp->r = ra / n_average;
            dp->g = ga / n_average;
            dp->b = ba / n_average;
            dp->a = aa / n_average;
                     
            // Advance destination pointer 
            dp++;
        } // pDest x loop

        // next box-y
        sp = (ColorRGBA*)((uint8_t*)osp + (pSource->GetStride() * factorY));

        // Advance destination pointers 
        dp = (ColorRGBA*)((uint8_t*)dp + dgap);
    } // pDest y loop
    pDest->Unlock();

    return 0;
}


// This function assumes the destination surface is the right size.
// 32bit Zoomer with optional anti-aliasing by bilinear interpolation.
// Magnifies or shrinks 32bit RGBA/ABGR pSource surface to pDest surface.
static int ZoomSurfaceRGBA(ISurface* pSource, ISurface* pDest, bool bFlipX, bool bFlipY, bool bSmooth)
{
    int       x, y, sx, sy, *sax, *say, *csax, *csay, csx, csy, ex, ey, t1, t2, sstep;
    ColorRGBA *c00, *c01, *c10, *c11;
    ColorRGBA *sp, *csp, *dp;
    int       dgap;

    // Variable setup 
    if(bSmooth)
    {
        // For interpolation: assume source dimension is one pPixel 
        // smaller to avoid overflow on right and bottom edge.     
        sx = (int) (65536.0 * (float)(pSource->GetWidth()  - 1) / (float)pDest->GetWidth());
        sy = (int) (65536.0 * (float)(pSource->GetHeight() - 1) / (float)pDest->GetHeight());
    }
    else
    {
        sx = (int) (65536.0 * (float)pSource->GetWidth()  / (float)pDest->GetWidth());
        sy = (int) (65536.0 * (float)pSource->GetHeight() / (float)pDest->GetHeight());
    }


    // 1/4/10 CSidhall - Added for canvas draw crash
    if(pSource->GetData() == NULL)
    {
       return -1;
    }

   
    // Allocate memory for pRow increments.
    // To do: Remove usage of the heap here for all but pathological cases.
    sax = EAWEBKIT_NEW("ZoomSurface RGBA") int[pDest->GetWidth() + 1];// WTF::fastNewArray<int>(pDest->GetWidth() + 1);

    if(sax == NULL)
        return -1;

    say = EAWEBKIT_NEW("ZoomSurface RGBA") int[pDest->GetHeight() + 1];//WTF::fastNewArray<int> (pDest->GetHeight() + 1);

    if(say == NULL)
    {
        EAWEBKIT_DELETE[] sax;//WTF::fastDeleteArray<int> (sax);
        return -1;
    }

    // Precalculate pRow increments 
    sp = csp = (ColorRGBA*) pSource->GetData();
    
    pDest->Lock();
    dp = (ColorRGBA*) pDest->GetData();

    if(bFlipX)
        csp += (pSource->GetWidth() - 1);

    if(bFlipY)
        csp  = (ColorRGBA*)( (uint8_t*)csp + pSource->GetStride() * (pSource->GetHeight() - 1) );

    csx  = 0;
    csax = sax;

    for (x = 0; x <= pDest->GetWidth(); x++)
    {
        *csax = csx;
        csax++;
        csx &= 0xffff;
        csx += sx;
    }

    csy  = 0;
    csay = say;

    for (y = 0; y <= pDest->GetHeight(); y++)
    {
        *csay = csy;
        csay++;
        csy &= 0xffff;
        csy += sy;
    }

    dgap = pDest->GetStride() - (pDest->GetWidth() * 4);

    if(bSmooth) // If interpolating zoom ...
    {
        // Scan destination 
        csay = say;

        for (y = 0; y < pDest->GetHeight(); y++)
        {
            // Setup color source pointers 
            c00 = csp;
            c01 = csp;
            c01++;

            c10 = (ColorRGBA*) ((uint8_t*) csp + pSource->GetStride());
            c11 = c10;
            c11++;

            csax = sax;

            for (x = 0; x < pDest->GetWidth(); x++)
            {
                // Interpolate colors 
                ex = (*csax & 0xffff);
                ey = (*csay & 0xffff);

                t1 = ((((c01->r - c00->r) * ex) >> 16) + c00->r) & 0xff;
                t2 = ((((c11->r - c10->r) * ex) >> 16) + c10->r) & 0xff;
                dp->r = (((t2 - t1) * ey) >> 16) + t1;

                t1 = ((((c01->g - c00->g) * ex) >> 16) + c00->g) & 0xff;
                t2 = ((((c11->g - c10->g) * ex) >> 16) + c10->g) & 0xff;
                dp->g = (((t2 - t1) * ey) >> 16) + t1;

                t1 = ((((c01->b - c00->b) * ex) >> 16) + c00->b) & 0xff;
                t2 = ((((c11->b - c10->b) * ex) >> 16) + c10->b) & 0xff;
                dp->b = (((t2 - t1) * ey) >> 16) + t1;

                t1 = ((((c01->a - c00->a) * ex) >> 16) + c00->a) & 0xff;
                t2 = ((((c11->a - c10->a) * ex) >> 16) + c10->a) & 0xff;
                dp->a = (((t2 - t1) * ey) >> 16) + t1;

                // Advance source pointers 
                csax++;
                sstep = (*csax >> 16);
                c00 += sstep;
                c01 += sstep;
                c10 += sstep;
                c11 += sstep;

                // Advance destination pointer 
                dp++;
            }

            // Advance source pointer 
            csay++;
            csp = (ColorRGBA*) ((uint8_t*) csp + (*csay >> 16) * pSource->GetStride());

            // Advance destination pointers 
            dp = (ColorRGBA*) ((uint8_t*) dp + dgap);
        }
    }
    else
    {
        // Non-Interpolating Zoom 
        csay = say;

        for (y = 0; y < pDest->GetHeight(); y++)
        {
            sp   = csp;
            csax = sax;

            for (x = 0; x < pDest->GetWidth(); x++)
            {
                // Draw 
                *dp = *sp;

                // Advance source pointers 
                csax++;
                sstep = (*csax >> 16);

                if(bFlipX)
                    sstep = -sstep;

                sp += sstep;

                // Advance destination pointer 
                dp++;
            }

            // Advance source pointer 
            csay++;
            sstep = (*csay >> 16) * pSource->GetStride();

            if(bFlipY)
                sstep = -sstep;

            csp = (ColorRGBA*) ((uint8_t*) csp + sstep);

            // Advance destination pointers 
            dp = (ColorRGBA*) ((uint8_t*) dp + dgap);
        }
    }
    pDest->Unlock();

    // Remove temp arrays. 
    EAWEBKIT_DELETE[] sax;//WTF::fastDeleteArray<int> (sax);
    EAWEBKIT_DELETE[] say;//WTF::fastDeleteArray<int> (say);


    return 0;
}

 
// 32bit specialized 90degree rotator
// Rotates 'src' surface to 'pDest' surface in 90degree increments.
EARASTER_API ISurface* RotateSurface90Degrees(ISurface* pSurface, int nClockwiseTurns) 
{
    int      pRow, col;
    ISurface* pSurfaceResult = NULL;

    // Has to be a valid surface pointer and only 32-bit surfaces (for now).
    EAW_ASSERT(pSurface && (pSurface->GetPixelFormat().mBytesPerPixel == 4));

    if(pSurface && (pSurface->GetPixelFormat().mBytesPerPixel == 4))
    {
        // Normalize nClockwiseTurns.
        while(nClockwiseTurns < 0)
            nClockwiseTurns += 4;

        nClockwiseTurns = (nClockwiseTurns % 4);

        // If it's even, our new width will be the same as the source surface
        const int newWidth  = (nClockwiseTurns % 2) ? (pSurface->GetHeight()) : (pSurface->GetWidth() );
        const int newHeight = (nClockwiseTurns % 2) ? (pSurface->GetWidth())  : (pSurface->GetHeight() );

        pSurfaceResult = CreateSurface(newWidth, newHeight, pSurface->GetPixelFormat().mPixelFormatType, pSurface->GetCategory());

        if(pSurfaceResult)
        {
            pSurfaceResult->Lock();

            if(nClockwiseTurns != 0)
            {
                uint32_t* srcBuf = NULL;
                uint32_t* dstBuf = NULL;

                switch(nClockwiseTurns)
                {
                    // rotate clockwise
                    case 1: // rotated 90 degrees clockwise
                    {
                        for (pRow = 0; pRow < pSurface->GetHeight(); ++pRow)
                        {
                            srcBuf = (uint32_t*)(pSurface->GetData())       + (pRow * pSurface->GetStride() / 4);
                            dstBuf = (uint32_t*)(pSurfaceResult->GetData()) + (pSurfaceResult->GetWidth() - pRow - 1);

                            for (col = 0; col < pSurface->GetWidth(); ++col)
                            {
                                *dstBuf = *srcBuf++;
                                dstBuf += pSurfaceResult->GetStride() / 4;  // To do: This can be made faster by pre-calculating the delta.
                            }
                        }
                        break;
                    }

                    case 2: // rotated 180 degrees clockwise
                    {
                        for(pRow = 0; pRow < pSurface->GetHeight(); ++pRow)
                        {
                            srcBuf = (uint32_t*)(pSurface->GetData()) + (pRow * pSurface->GetStride() / 4);
                            dstBuf = (uint32_t*)(pSurfaceResult->GetData()) + ((pSurfaceResult->GetHeight() - pRow - 1) * pSurfaceResult->GetStride() / 4) + (pSurfaceResult->GetWidth() - 1);

                            for(col = 0; col < pSurface->GetWidth(); ++col)
                                *dstBuf-- = *srcBuf++;
                        }
                        break;
                    }

                    case 3:
                    {
                        for(pRow = 0; pRow < pSurface->GetHeight(); ++pRow)
                        {
                            srcBuf = (uint32_t*)(pSurface->GetData()) + (pRow * pSurface->GetStride() / 4);
                            dstBuf = (uint32_t*)(pSurfaceResult->GetData()) + pRow + ((pSurfaceResult->GetHeight() - 1) * pSurfaceResult->GetStride() / 4);

                            for(col = 0; col < pSurface->GetWidth(); ++col)
                            {
                                *dstBuf = *srcBuf++;
                                 dstBuf -= pSurfaceResult->GetStride() / 4;
                            }
                        }
                        break;
                    }
                } // switch
            }
            else
            {
                // To do: Implement blit here.
                EAW_FAIL_MSG("RotateSurface90Degrees: Not completed.");
                // if(Blit(pSurface, NULL, pSurfaceResult, NULL) != 0)
                {
                    DestroySurface(pSurfaceResult);
                    return NULL;
                }
            }
            pSurfaceResult->Unlock();
        }
    }

    return pSurfaceResult;
}

// This applies a transform to an existing texture and outputs a new transformed texture.
// The srcRect is needed in case we are just using part of the source texture instead of the full texture.
EARASTER_API ISurface* TransformSurface(ISurface* pSurface, EA::Raster::Rect& srcRect, Matrix2D& m) 
{
    using namespace WKAL;

     AffineTransform transform(m.m_m11,m.m_m12,m.m_m21,m.m_m22,0,0);                 
    IntRect rect = transform.mapRect(srcRect);
    int dstW = rect.width();
    int dstH = rect.height();

    // Create the new surface    
    ISurface* pSurfaceDst = CreateSurface(dstW, dstH, pSurface->GetPixelFormat().mPixelFormatType, pSurface->GetCategory());
    if(!pSurfaceDst)
        return pSurfaceDst;

    // Probably many different algorithms are possible here. This just a simple one to get things started here.
    // We can't just rotate all the texels from the source to the destination for we might get gaps because of floating point x,y coordinates or holes because of scale.
    // Instead we rotate back from the destination to sample the color.  If we get negative coords, we are outside of the texture.

    // Adjust the matrix to now convert the new surface to the old surface coordinate system
    AffineTransform invT = transform.inverse();    
    double x2,y2;  
    invT.map( (double) rect.x(), (double) rect.y(), &x2, &y2);    
    invT.setE(x2);
    invT.setF(y2);

  
    int strideSrc = pSurface->GetStride() >> 2;    
    int strideDst = pSurfaceDst->GetStride() >> 2;    
    
    pSurfaceDst->Lock();
    for(int y1=0; y1 < dstH; y1++)
    {
        uint32_t* pDst = (uint32_t*)(pSurfaceDst->GetData()) + (y1 * strideDst);

        // For now, just blast through all the pixels.
        // Better might be for example to fill the surface first and build a center diagonal Y and run from the center x till the 2 edges are found.
        // Would use a line equation to find the x center for each y row.
        for(int x1=0; x1 < dstW; x1++)
        {
            invT.map(x1, y1, &x2, &y2);    
            
            // We need to check with the delta x,y and not 0,0 in case this is a part inside a texture.
            if( ((int) x2 < srcRect.x) || ((int) x2 >= srcRect.w) || ((int) y2 < srcRect.y) || ((int) y2 >= srcRect.h) ) 
            {
                // Outside of the source texture so we blank it out      
                *pDst = 0;
            }
            else
            {
                // Inside the source texture so grab the color value.
                // To consider: Will Probably need to add filtering (bilinear) here instead of just sampling a single value.  
                uint32_t* pSrc = (uint32_t*)(pSurface->GetData()) + ((uint32_t) y2 * strideSrc) + (uint32_t) x2;   
                uint32_t value = *pSrc;
                *pDst = value;
            }
            pDst++;
        }
    }
    pSurfaceDst->Unlock();

    // The calling function will need to free the surface
    return pSurfaceDst;
}

EARASTER_API void ZoomSurfaceSize(int width, int height, double zoomX, double zoomY, int* destWidth, int* destHeight)
{
    if(zoomX < 0.001)
       zoomX = 0.001;

    if(zoomY < 0.001)
       zoomY = 0.001;

    *destWidth  = (int)((double)width  * zoomX);
    *destHeight = (int)((double)height * zoomY);

    if(*destWidth < 1)
       *destWidth = 1;

    if(*destHeight < 1)
       *destHeight = 1;
}


EARASTER_API ISurface* ZoomSurface(ISurface* pSurface, double zoomX, double zoomY, bool bSmooth)
{
    ISurface* pSurfaceResult = NULL;

    if(pSurface)
    {
        const bool bFlipX = (zoomX < 0);
        if(bFlipX)
            zoomX = -zoomX;

        const bool bFlipY = (zoomY < 0);
        if(bFlipY)
            zoomY = -zoomY;

        int destWidth, destHeight;
        ZoomSurfaceSize(pSurface->GetWidth(), pSurface->GetHeight(), zoomX, zoomY, &destWidth, &destHeight);

        pSurfaceResult = CreateSurface(destWidth, destHeight, pSurface->GetPixelFormat().mPixelFormatType, kSurfaceCategoryZoom);

        if(pSurfaceResult)
            ZoomSurfaceRGBA(pSurface, pSurfaceResult, bFlipX, bFlipY, bSmooth);
    }

    return pSurfaceResult;
}


EARASTER_API ISurface* ShrinkSurface(ISurface* pSurface, int factorX, int factorY)
{
    ISurface* pSurfaceResult = NULL;

    if(pSurface)
    {
        int destWidth  = pSurface->GetWidth()  / factorX;
        int destHeight = pSurface->GetHeight() / factorY;

        while((destWidth * factorX) > pSurface->GetWidth() )
            destWidth--;

        while((destHeight * factorY) > pSurface->GetHeight())
            destHeight--;

        pSurfaceResult = CreateSurface(destWidth, destHeight, pSurface->GetPixelFormat().mPixelFormatType, kSurfaceCategoryZoom);

        if(pSurfaceResult)
            ShrinkSurfaceRGBA(pSurface, pSurfaceResult, factorX, factorY);
    }

    return pSurfaceResult;
}


EARASTER_API ISurface* CreateTransparentSurface(ISurface* pSource, int surfaceAlpha)
{
    ISurface* const pResult = CreateSurface(pSource);

    if(pResult)
    {
        // This code currently assumes that the format is ARGB and that stride == width.
        EAW_ASSERT((pResult->GetStride() == (pResult->GetWidth() * 4)) && (pResult->GetPixelFormat().mPixelFormatType == kPixelFormatTypeARGB));
        
        pResult->Lock();
        uint32_t* pPixels = (uint32_t*)pResult->GetData();

        for (int y = 0, yEnd = pSource->GetHeight(), xEnd = pSource->GetWidth(); y < yEnd; ++y)
        {
            for (int x = 0; x < xEnd; ++x, ++pPixels)
            {
                *pPixels = MultiplyColorAlpha(*pPixels, surfaceAlpha);
            }
        }
        pResult->Unlock();
    }

    return pResult;
}


} // namespace Raster

} // namespace EA



