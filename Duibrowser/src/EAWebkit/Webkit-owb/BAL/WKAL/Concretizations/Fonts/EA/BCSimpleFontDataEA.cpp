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

/////////////////////////////////////////////////////////////////////////////
// BCSimpleFontDataEA.cpp
// Written by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////

#include "config.h"
#include "SimpleFontData.h"
#include "FloatRect.h"
#include "Font.h"
#include "FontCache.h"
#include "FontDescription.h"
#include "GlyphBuffer.h"
#include "BALBase.h"
#include <unicode/uchar.h>
#include <unicode/unorm.h>
#include <wtf/MathExtras.h>
#include <EAWebKit/EAWebKitTextInterface.h>    
#include <EAWebKit/internal/EAWebKitAssert.h>

namespace WKAL {

void SimpleFontData::platformInit()
{
    EA::WebKit::IFont* const pFont = m_font.mpFont;
    EAW_ASSERT(pFont);

	if(pFont)
	{
		EA::WebKit::FontMetrics fontMetrics;
		pFont->GetFontMetrics(fontMetrics);

		// With this code EAWebKit implements line spacing seemingly just like Safari on Windows:
		// m_ascent      = (int)(fontMetrics.mfAscent + 0.5f);
		// m_descent     = (int)(-fontMetrics.mfDescent + 0.5f);    // Apparently WebKit wants descent to be a positive value, which is unconventional.
		// m_lineSpacing = (int)(fontMetrics.mfLineHeight + 0.5f);

		// With this code EAWebKit emulates FireFox line spacing fairly closely: 
		m_ascent      = (int)(fontMetrics.mfAscent + 1.5f);
		m_descent     = (int)(-fontMetrics.mfDescent + 1.5f);    // Apparently WebKit wants descent to be a positive value, which is unconventional.
		m_lineSpacing = (int)(m_ascent + m_descent);

		m_lineGap     = m_lineSpacing - m_ascent + m_descent;
		m_unitsPerEm  = 1000;                           // This is a typical value for a TrueType font, not necessarily the actual value.
		m_xHeight     = fontMetrics.mfXHeight;

		const EA::WebKit::Char  cSpace  = ' ';
		EA::WebKit::GlyphId  glyphId = 0;
		pFont->GetGlyphIds(&cSpace, 1, &glyphId, true);

		EA::WebKit::GlyphMetrics glyphMetrics;
		pFont->GetGlyphMetrics(glyphId, glyphMetrics);
		m_spaceWidth = glyphMetrics.mfHAdvanceX;
	}
}


void SimpleFontData::platformDestroy()
{
    // Do we need to destroy m_font.mpFont?

    if (m_smallCapsFontData)
    {
        delete m_smallCapsFontData;
        m_smallCapsFontData = NULL;
    }
}


SimpleFontData* SimpleFontData::smallCapsFontData(const FontDescription& fontDescription) const
{
    if (!m_smallCapsFontData)
    {
        // Note by Paul Pedriana:
        // We simply create a version of this font that is 70% of its current size.
        // Question: Is there a means to get a more proper small caps variant?
        // Actually I think we just need to modify the code below to get a true small
        // caps font, but shouldn't desc.m_smallCaps be set to true in such a case?
        // and if m_smallCaps is true then we don't need 70% size.

        FontDescription desc = FontDescription(fontDescription);
        desc.setSpecifiedSize(0.70f * fontDescription.computedSize());

        // Note by Paul Pedriana: This using of ' ' is wrong. What we need to do is somehow find out what
        // characters the caller is expecting this font to support, but currently webkit doesn't provide
        // that information. It doesn't provide it likely because Windows and Macintosh have font systems
        // that fake support for characters not in a font by silently substituting other fonts. A result
        // of our situation here is that if small caps are requested for Japanese glyphs, we probably not
        // get a Japanese font.

        UChar cSpace = ' ';
        const WebCore::AtomicString& familyName = desc.family().family();
        const FontPlatformData* const pData = new FontPlatformData(desc, familyName, &cSpace, 1);
        m_smallCapsFontData = new SimpleFontData(*pData);

        // 4/21/09 CSidhall - Leak Fix.  Since we copied pData into SimpleFontData with the constructor, we need to free the memory without calling 
        // the destructors.  They will be called later when m_smallCapFonData is destroyed.
        fastFree( (void*) pData);  

    
    }

    return m_smallCapsFontData;
}


bool SimpleFontData::containsCharacters(const UChar* characters, int length) const
{
    int i = 0;
    EA::WebKit::IFont* const pFont = m_font.mpFont;
    EAW_ASSERT(pFont);

    if(pFont)
    {
        while(pFont->IsCharSupported(characters[i]))
            ++i;
    }

    return (i == length);
}


void SimpleFontData::determinePitch()
{
    m_treatAsFixedPitch = m_font.isFixedPitch();
}


float SimpleFontData::platformWidthForGlyph(Glyph glyphId) const
{
    EA::WebKit::IFont* const pFont = m_font.mpFont;
    EAW_ASSERT(pFont);

    EA::WebKit::GlyphMetrics glyphMetrics;
    glyphMetrics.mfHAdvanceX = m_spaceWidth; // Default value.
    if(pFont)
		pFont->GetGlyphMetrics(glyphId, glyphMetrics);

    return glyphMetrics.mfHAdvanceX;
}


void SimpleFontData::setFont(BalFont* pFont) const
{
    EAW_ASSERT(pFont);
    m_font.setFont(pFont);
}


}  // namespace WKAL
