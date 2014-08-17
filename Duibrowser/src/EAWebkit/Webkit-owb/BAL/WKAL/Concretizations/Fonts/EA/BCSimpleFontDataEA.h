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
// BCSimpleFontDataEA.h
// Written by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////

#ifndef SimpleFontData_h
#define SimpleFontData_h

#include "FontData.h"
#include "FontPlatformData.h"
#include "GlyphPageTreeNode.h"
#include "GlyphWidthMap.h"
#include <wtf/OwnPtr.h>
#include <EAWebKit/EAWebKitTextInterface.h>

namespace WKAL {


class FontDescription;
class FontPlatformData;
class SharedBuffer;
class SVGFontData;
class WidthMap;



// SimpleFontData
// This class is mostly analagous to EAText's Font class. 
//
class SimpleFontData : public FontData
{
public:
    SimpleFontData(const FontPlatformData&, bool customFont = false, bool loading = false, SVGFontData* data = 0);
    virtual ~SimpleFontData();

public:
    const FontPlatformData& platformData() const { return m_font; }
    SimpleFontData* smallCapsFontData(const FontDescription& fontDescription) const;

    // vertical metrics
    int ascent() const          { return m_ascent; }
    int descent() const         { return m_descent; }
    int lineSpacing() const     { return m_lineSpacing; }
    int lineGap() const         { return m_lineGap; }
    float xHeight() const       { return m_xHeight; }
    unsigned unitsPerEm() const { return m_unitsPerEm; }

    float widthForGlyph(Glyph) const;
    float platformWidthForGlyph(Glyph) const;

    // Note by Paul Pedriana:
    // By default this function merely does 'return this'. I'm trying to understand how
    // it would go about doing anything else. Where would it go to find alternative
    // SimpleFontData?
    virtual const SimpleFontData* fontDataForCharacter(UChar32) const;

    // Return true only if all characters are supported.
    virtual bool containsCharacters(const UChar*, int length) const;

    void  determinePitch();
    
    EA::WebKit::Pitch pitch() const { return m_treatAsFixedPitch ? EA::WebKit::kPitchFixed :  EA::WebKit::kPitchVariable; }

    #if ENABLE(SVG_FONTS)
        SVGFontData* svgFontData() const { return m_svgFontData.get(); }
        bool isSVGFont() const { return m_svgFontData; }
    #else
        bool isSVGFont() const { return false; }
    #endif

    // Custom fonts are also known as "downloadable fonts" and are specified in HTML by @font-face.
    virtual bool isCustomFont() const { return m_isCustomFont; }

    // Return true if the font is busy loading (which may be so if it is a custom font).
    virtual bool isLoading() const { return m_isLoading; }

    // True if this class (SimpleFontData) is class SegmentedFontData, which if course it isn't. 
    virtual bool isSegmented() const; 

    const GlyphData& missingGlyphData() const { return m_missingGlyphData; }

    void setFont(BalFont*) const;

private:
    void platformInit();
    void platformDestroy();
    void commonInit();

public:
    // Note by Paul Pedriana:
    // The original WebKit equivalent of this class has this data public. Does it really need to be so?
    // To do: See if we can avoid having much of this member data, as it's already present as part of
    //        the m_font.mpFont object.
    int                     m_ascent;
    int                     m_descent;
    int                     m_lineSpacing;
    int                     m_lineGap;
    float                   m_xHeight;
    unsigned                m_unitsPerEm;

    FontPlatformData        m_font;

    mutable GlyphWidthMap   m_glyphToWidthMap;

    bool                    m_treatAsFixedPitch;

    #if ENABLE(SVG_FONTS)
        OwnPtr<SVGFontData> m_svgFontData;
    #endif

    bool                    m_isCustomFont;     // Whether or not we are custom font loaded via @font-face
    bool                    m_isLoading;        // Whether or not this custom font is still in the act of loading.

    Glyph                   m_spaceGlyph;
    float                   m_spaceWidth;           // This is the advance of the space char.
    float                   m_adjustedSpaceWidth;

    GlyphData               m_missingGlyphData;

    mutable SimpleFontData* m_smallCapsFontData;

};



///////////////////////////////////////////////////////////////////////////////
// inlines
///////////////////////////////////////////////////////////////////////////////


} // namespace WebCore


#endif // SimpleFontData_h
