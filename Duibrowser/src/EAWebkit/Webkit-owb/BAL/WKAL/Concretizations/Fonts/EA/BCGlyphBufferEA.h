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
// BCGlyphBufferEA.h
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#ifndef GlyphBuffer_h
#define GlyphBuffer_h

#include <wtf/FastAllocBase.h>

#include "FloatSize.h"
#include <wtf/Vector.h>
#include "BALBase.h"
#include <EABase/eabase.h>


namespace WKAL {


// Typedef 
//
typedef unsigned short Glyph;


// Forward declarations
//
class SimpleFontData;


// class GlyphBuffer
//
// This class implements a list of what glyphs will be drawn from what font in what location.
// It is like a display list but without information about the starting position of individual
// glyph positions. 
// This class is somewhat like EAText LineLayout/GlyphLayoutInfo, but with a little less info.
//
class GlyphBuffer/*: public WTF::FastAllocBase*/
{
public:
    bool isEmpty() const { return m_fontData.isEmpty(); }
    int  size()    const { return m_fontData.size();    }
    
    void clear()
    {
        m_fontData.clear();
        m_glyphs.clear();
        m_advances.clear();
    }

          GlyphBufferGlyph*   glyphs(int from)         { return m_glyphs.data() + from; }
    const GlyphBufferGlyph*   glyphs(int from)   const { return m_glyphs.data() + from; }

          GlyphBufferAdvance* advances(int from)       { return m_advances.data() + from; }
    const GlyphBufferAdvance* advances(int from) const { return m_advances.data() + from; }

    const SimpleFontData* fontDataAt(int index) const { return m_fontData[index]; }
    
    void swap(int index1, int index2)
    {
        const SimpleFontData* f = m_fontData[index1];
        m_fontData[index1]      = m_fontData[index2];
        m_fontData[index2]      = f;

        GlyphBufferGlyph g = m_glyphs[index1];
        m_glyphs[index1]   = m_glyphs[index2];
        m_glyphs[index2]   = g;

        GlyphBufferAdvance s = m_advances[index1];
        m_advances[index1]   = m_advances[index2];
        m_advances[index2]   = s;

    }

    Glyph glyphAt(int index) const
    {
        return m_glyphs[index];
    }

    float advanceAt(int index) const
    {
        return m_advances[index].width();
    }

    FloatSize offsetAt(int /*index*/) const
    {
        // What is an 'offset'?
        return FloatSize();
    }

    // What is an 'offset'? It doesn't seem like pOffset is ever non-NULL.
    void add(Glyph glyphId, const SimpleFontData* pSimpleFontData, float width, const FloatSize* /*pOffset*/ = 0)
    {
        m_fontData.append(pSimpleFontData);
        m_glyphs.append(glyphId);
        m_advances.append(FloatSize(width, 0));
    }
    
private:
    // Since this class happens to be used on the stack, the stack can be overrun
    // on platforms that don't have much stack space. We need to set the capacity
    // to a smaller value for these platforms. Vector inline capacity is memory
    // that is created in the vector as fixed space that doesn't use the heap.
    #if defined(EA_PLATFORM_WINDOWS) | defined(EA_PLATFORM_UNIX)
        static const size_t kInlineCapacity = 2048;
    #else
        static const size_t kInlineCapacity = 512;
    #endif

    Vector<const SimpleFontData*, kInlineCapacity> m_fontData;     // Font used for the glyph.
    Vector<GlyphBufferGlyph, kInlineCapacity>      m_glyphs;       // GlyphBufferGlyph is unsigned short (uint16_t)
    Vector<GlyphBufferAdvance, kInlineCapacity>    m_advances;     // GlyphBufferAdvance is a FloatSize (width/height)
};


} // namespace EA


#endif // Header include guard
