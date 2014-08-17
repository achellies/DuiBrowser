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
// BCFontPlatformDataEA.h
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#ifndef FontCustomPlatformData_h
#define FontCustomPlatformData_h


#include "FontRenderingMode.h"
#include <wtf/Noncopyable.h>
#include "BALBase.h"

namespace WKAL {


class FontPlatformData;
class SharedBuffer;


// Note by Paul Pedriana: This is a class which manages the creation of 
// a FontPlatformData (more or less equivalent to EA::Text::Font) from
// a font file transferred over the Internet, as with the @font-face
// directive (dynamic downloadable fonts).

struct FontCustomPlatformData : Noncopyable, public WKALBase
{
public:
    FontCustomPlatformData(BalFontFace* pFontFace, SharedBuffer* pFontData);// BalFontFace == EA::Text::Font
   ~FontCustomPlatformData();

    // This setting is used to provide ways of switching between multiple rendering modes that 
    // may have different metrics. It is used to switch between CG and GDI text on Windows.
    // enum FontRenderingMode { NormalRenderingMode, AlternateRenderingMode };
    FontPlatformData fontPlatformData(int size, bool bold, bool italic, FontRenderingMode = WKAL::NormalRenderingMode);

public:
    // This is an EAText Font, and so in theory it can be a BitmapFont as well as an
    // OutlineFont. We would need to find a way to transfer the BitmapFont as a single
    // file and unpack it in our createFontCustomPlatformData function. Probably as a
    // .zip file with the .bmpFont and individual .png files.
    EA::WebKit::IFont* m_fontFace;
    SharedBuffer*   mpFontData;
};


FontCustomPlatformData* createFontCustomPlatformData(SharedBuffer* pSharedBuffer);


} // namespace WKAL


#endif  // Header include guard
