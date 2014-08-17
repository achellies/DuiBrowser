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
// EARasterColor.cpp
// By Nicki Vankoughnhett
///////////////////////////////////////////////////////////////////////////////


#include <EARaster/EARasterColor.h>
#include "Color.h"  //for WKAL::Color


namespace EA
{
namespace Raster
{


Color::Color(const WKAL::Color& wbColor) : m_color( wbColor.rgb() )
{
}
EARASTER_API RGBA32 makeRGB(int32_t r, int32_t g, int32_t b)
{
    rgba_clamp(r);
    rgba_clamp(g);
    rgba_clamp(b);
    return 0xFF000000 | r << 16 | g << 8 | b;
}

// Note that the return value is not RGBA but ARGB. The reason this is so 
// is that WebKit itself makes this mistake and since its use of color like
// this is prevalent throughout WebKit, we have little choice but to propogate
// the convention here. Otherwise we could have a lot of confusion about 
// what format is in actual use. In our current case, the color format is 
// wrong, but at least it's consistently wrong.
EARASTER_API RGBA32 makeRGBA(int32_t r, int32_t g, int32_t b, int32_t a)
{
    rgba_clamp(r);
    rgba_clamp(g);
    rgba_clamp(b);
    rgba_clamp(a);
    return a << 24 | r << 16 | g << 8 | b;
}



} // namespace Raster
} // namespace EA