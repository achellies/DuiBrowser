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
// BCFontPlatformDataEA.cpp
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "FontCustomPlatformData.h"
#include "SharedBuffer.h"
#include "FontPlatformData.h"
#include <EAWebKit/EAWebkitTextInterface.h>
#include <EAWebKit/internal/EAWebKitAssert.h>
#include <EAWebKit/EAWebKit.h>

namespace WKAL {

FontCustomPlatformData::FontCustomPlatformData(EA::WebKit::IFont* pFontFace, WKAL::SharedBuffer* pFontData)  // BalFontFace == EA::Text::Font
  : m_fontFace(pFontFace)
  , mpFontData(pFontData)
{
    if(pFontData)
        pFontData->ref();

    if(pFontFace)
        pFontFace->AddRef();
}

FontCustomPlatformData::~FontCustomPlatformData()
{
    if (mpFontData)
    {
        mpFontData->deref();
        mpFontData = NULL;
    }

    if (m_fontFace)
    {
        m_fontFace->Release();
        m_fontFace = NULL;
    }
}


FontPlatformData FontCustomPlatformData::fontPlatformData(int size, bool bold, bool italic, FontRenderingMode)
{
    // Problem: We don't have a means to tell what characters are needed. We allow this to compile
    // by using a ' ' char, but that's not really right.

    UChar cSpace = ' ';
    return FontPlatformData(m_fontFace, size, bold, italic, &cSpace, 1);
}


// Windows has a standard downloadable font type called .eot (Embedded OpenType) which is used
// with the Windows font system. We don't need to use that, but it might be required to support
// it in the future. WebKit has some code to load .eot files; see TTLoadEmbeddedFont() usage, etc.
// In the meantime there is nothing preventing us from just reading .ttf files transferred to
// us or possibly our own custom encrypted font system.
//
// Currently we directly interpret the buffer data as a .ttf/.otf file image. 
//
// To consider: Since fonts can be of non-trivial size (sometimes over a megabyte), it may be
// useful for use to take the font data and copy it to disk when possible and free the RAM 
// memory.
//
// If we want to implement EAText effects (outlines, shadows, glows), then it's best if we 
// solve this either by a CSS style or by some kind of additional data attached to this 
// font data. Possibly the additional data can be a text effect script. 

FontCustomPlatformData* createFontCustomPlatformData(SharedBuffer* pFontData)
{
    EAW_ASSERT(pFontData);
    FontCustomPlatformData* pFontCustomPlatformData = NULL;

    EA::WebKit::IFontServer* const pFontServer = EA::WebKit::GetFontServer();
    if(!pFontServer)
        return NULL;

    EA::WebKit::IFont* const pOutlineFont = pFontServer->CreateNewFont(EA::WebKit::kFontTypeOutline);
    if(!pOutlineFont){
        EAW_ASSERT(pOutlineFont);
        return NULL;
    }
    if(pOutlineFont->OpenOutline(pFontData->data(), (uint32_t)pFontData->size()))  // This increments the pOutlineFont reference count.
    {
        // The new object will adopt pFont and pFontData.
        pFontCustomPlatformData = new FontCustomPlatformData(pOutlineFont, pFontData);
        pOutlineFont->Release();    // Matches AddRef from Open().

        // We might want to add this font to the FontServer, though doing so will result in the 
        // Font and its memory being held for the duration of the FontServer's life.
        // pFontServer->AddFont(pFont);
    }

    return pFontCustomPlatformData;
}


} // namespace WKAL
