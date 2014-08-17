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
// EARasterUtils.h
// By Yee Cheng Chin
///////////////////////////////////////////////////////////////////////////////

#ifndef EARASTER_EARASTERUTILS_H
#define EARASTER_EARASTERUTILS_H

#if EAWEBKIT_THROW_BUILD_ERROR
#error This file should be included only in a dll build
#endif

namespace EA
{
    namespace Raster
    {
        // Accurate for dividing 16-bit values by 255
        EA_FORCE_INLINE uint32_t DivideBy255(uint32_t a)
        {
            a++;
            return (a + (a >> 8)) >> 8;
        }

        EA_FORCE_INLINE uint32_t DivideBy255Rounded(uint32_t a)
        {
            a += 128;
            return (a + (a >> 8)) >> 8;
        }

        EA_FORCE_INLINE uint32_t MultiplyBy255(uint32_t a)
        {
            return (a << 8) - a;
        }

        // Divides two 16-bit values packed into a uint32_t in parallel
        EA_FORCE_INLINE uint32_t DividePairBy255(uint32_t a)
        {
            a += 0x00010001;
            a = (a + ((a >> 8) & 0x00ff00ff)) >> 8;
            return (a & 0x00ff00ff);
        }

        EA_FORCE_INLINE uint32_t DividePairBy255Rounded(uint32_t a)
        {
            a += 0x00800080;
            a = (a + ((a >> 8) & 0x00ff00ff)) >> 8;
            return (a & 0x00ff00ff);
        }

        // does a premultiplied src0 + src1 * invSrc0Alpha blend
        EA_FORCE_INLINE uint32_t BlendARGB32Premultiplied(uint32_t src0ARGB, uint32_t src1ARGB)
        {
            // Pack data into pairs to decrease the number of calculations
            uint32_t sag0 = (src0ARGB & 0xff00ff00) >> 8;
            uint32_t srb0 = (src0ARGB & 0x00ff00ff);
            uint32_t sag1 = (src1ARGB & 0xff00ff00) >> 8;
            uint32_t srb1 = (src1ARGB & 0x00ff00ff);

            uint32_t alpha = (src0ARGB >> 24);
            uint32_t invAlpha = 255 - alpha;

            // add src to (dst)(1-alpha)
            uint32_t dag = sag0 + (DividePairBy255Rounded(sag1 * invAlpha));
            uint32_t drb = srb0 + (DividePairBy255Rounded(srb1 * invAlpha));

            return (dag << 8) | drb;
        }

        // Apply the alpha on the color to make it premultiplied. I.e.
        // color.rgb = color.a * color.rgb
        EA_FORCE_INLINE uint32_t PremultiplyColor(uint32_t color)
        {
            uint32_t alphaComponent = (color & 0xff000000);
            uint32_t alpha = (alphaComponent >> 24);
            uint32_t redBlue = (color & 0x00ff00ff);
            uint32_t green = (color & 0x0000ff00);

            // Multiply the colors 
            redBlue = redBlue * alpha;
            green = green * alpha;

            // Divide by 255 with rounding.
            redBlue = DividePairBy255Rounded(redBlue);
            green = green + 0x00008000;             // not using existing helper functions because of the offset in the
            green = (green + (green >> 8)) >> 8;    // green channel, and don't want to waste cycles on bit shifts
            green = green & 0x0000ff00;

            return (alphaComponent | redBlue | green);
        }

        // Multiply all the components of a color (including alpha) by
        // an alpha value
        EA_FORCE_INLINE uint32_t MultiplyColorAlpha(uint32_t color, uint32_t alpha)
        {
            // Pack data into pairs to decrease the number of calculations
            uint32_t srcAlphaGreen = (color & 0xff00ff00) >> 8;
            uint32_t srcRedBlue = (color & 0x00ff00ff);

            // Multiply the color parts with alpha
            uint32_t dstAlphaGreen = DividePairBy255Rounded(srcAlphaGreen * alpha);
            uint32_t dstRedBlue = DividePairBy255Rounded(srcRedBlue * alpha);

            return (dstAlphaGreen << 8) | dstRedBlue;
        }

    } // namespace Raster

} // namespace EA


#endif // Header include guard

