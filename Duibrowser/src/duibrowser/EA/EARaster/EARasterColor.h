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
// EARasterColor.h
// By Nicki Vankoughnhett
///////////////////////////////////////////////////////////////////////////////


#ifndef EARASTER_EARASTERCOLOR_H
#define EARASTER_EARASTERCOLOR_H


#include <EARaster/EARasterConfig.h>
#include <EABase/eabase.h>



// Windows #defines TRANSPARENT to something.
#ifdef TRANSPARENT
    #undef TRANSPARENT
#endif


// Forward declare webcore color
namespace WKAL  // a.k.a. namespace WebCore
{
    class Color;
}


namespace EA
{
    namespace Raster
    {
        // The code here is the same as WebCore::Color (WKAL::Color), including 
        // its mistakes as described below.
        // We have our own copy of it in order to shield users from the internals
        // of WebKit and exposure to its header file web.

        // RGBA32 is misnamed; it should be called ARGB32 or maybe BGRA32, but it is definitely not an RGBA32.
        typedef uint32_t RGBA32; 

        EARASTER_API RGBA32 makeRGB(int32_t r, int32_t g, int32_t b);
        EARASTER_API RGBA32 makeRGBA(int32_t r, int32_t g, int32_t b, int32_t a);

		#define rgba_clamp(x) if(x < 0) x = 0; else if(x > 255) x = 255;

        class EARASTER_API Color 
        {
        public:
            Color() : m_color(0) { }
            Color(RGBA32 col) : m_color(col) { }
            Color(int32_t r, int32_t g, int32_t b)  
			{
				setRGB(r, g, b);
			}
            Color(int32_t r, int32_t g, int32_t b, int32_t a)
			{
				setRGBA(r, g, b, a);
			}

            Color(const WKAL::Color& wbColor);
            
            bool hasAlpha() const { return alpha() < 255; }

            int32_t red()   const { return (int32_t)((m_color >> 16) & 0xFF); }
            int32_t green() const { return (int32_t)((m_color >> 8)  & 0xFF); }
            int32_t blue()  const { return (int32_t) (m_color        & 0xFF); }
            int32_t alpha() const { return (int32_t)((m_color >> 24) & 0xFF); }

            void setBlue(int32_t b)   {  b &= 0xFF;  m_color &= ~BLUE_MASK;   m_color |= b; }
            void setGreen(int32_t g)  {  g &= 0xFF;  m_color &= ~GREEN_MASK;  m_color |= (g << 8);   }
            void setRed(int32_t r)    {  r &= 0xFF;  m_color &= ~RED_MASK;    m_color |= (r << 16);  }
            void setAlpha(int32_t a)  {  a &= 0xFF;  m_color &= ~ALPHA_MASK;  m_color |= (a << 24);  }

            RGBA32 rgb() const                           { return m_color; }             // This function's name is mistaken; it should be argb (or maybe rgba).
            void setRGB(int32_t r, int32_t g, int32_t b) 
			{ 
				rgba_clamp(r);
				rgba_clamp(g);
				rgba_clamp(b);
				m_color = (uint32_t)( 0xFF000000 | r << 16 | g << 8 | b );
			}
            void setRGB(RGBA32 rgb)                      { m_color = rgb;  }             // This function's name is mistaken; it should be setARGB (or maybe setRGBA).
			
			void setRGBA(int32_t r, int32_t g, int32_t b, int32_t a)
			{
				rgba_clamp(r);
				rgba_clamp(g);
				rgba_clamp(b);
				rgba_clamp(a);
				m_color = (uint32_t)(a << 24 | r << 16 | g << 8 | b);
			}
			void getRGBA(float_t& r, float_t& g, float_t& b, float_t& a) const
			{
				r = red() / 255.0f;
				g = green() / 255.0f;
				b = blue() / 255.0f;
				a = alpha() / 255.0f;
			}


            static const RGBA32 BLACK       = 0xFF000000;
            static const RGBA32 WHITE       = 0xFFFFFFFF;
            static const RGBA32 DARKGRAY    = 0xFF808080;
            static const RGBA32 GRAY        = 0xFFA0A0A0;
            static const RGBA32 LIGHTGRAY   = 0xFFC0C0C0;
            static const RGBA32 TRANSPARENT = 0x00000000;

            static const RGBA32 BLUE        = 0xFF0000FF;
            static const RGBA32 GREEN       = 0xFF00FF00;
            static const RGBA32 RED         = 0xFFFF0000;
            static const RGBA32 MAGENTA     = 0xFFFF00FF;
            static const RGBA32 CYAN        = 0xFF00FFFF;
            static const RGBA32 YELLOW      = 0xFFFFFF00;


        private:
            RGBA32 m_color;

            static const int32_t BLUE_MASK  = 0x000000FF;
            static const int32_t GREEN_MASK = 0x0000FF00;
            static const int32_t RED_MASK   = 0x00FF0000;
            static const int32_t ALPHA_MASK = 0xFF000000;

        };

    } // namespace Raster
} // namespace EA


#endif // Header include guard
