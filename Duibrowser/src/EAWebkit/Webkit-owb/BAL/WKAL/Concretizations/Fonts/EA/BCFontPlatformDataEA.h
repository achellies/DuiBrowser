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
// FontPlatformData.h
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#ifndef FontPlatformData_h
#define FontPlatformData_h


#include "GlyphBuffer.h"
#include "FontDescription.h"
#include "BALBase.h"
#include <EAWebKit/EAWebKitTextInterface.h>

namespace WKAL {


class FontPlatformData : public WKALBase
{
public:
    FontPlatformData(WTF::HashTableDeletedValueType);
    FontPlatformData();
    FontPlatformData(const FontDescription&, const AtomicString& family, const UChar* characters, int length);
    FontPlatformData(float size, bool bold, bool italic, const UChar* characters, int length,const EA::WebKit::TextEffectData *pEffect);
    FontPlatformData(BalFontFace* pFontFace, int size, bool bold, bool italic, const UChar* characters, int length);  // BalFontFace == EA::Text::Font
   ~FontPlatformData();
    FontPlatformData(const FontPlatformData& r);    // 7/17/09 CSidhall - Added to control font ref count

    // Do we need to implement this function? I'm not sure that we do.
    // static bool init();

    bool  isFixedPitch();
    float size() const;
    void  setFont(BalFont*) const; // Note by Paul Pedriana: I can't seem find that this function is ever used.

    unsigned hash() const;
    bool operator==(const FontPlatformData&) const;
    bool isHashTableDeletedValue() const;

public:
    // Note by Paul Pedriana:
    // The original WebKit equivalent of this class has this data public. Does it really need to be so?
     EA::WebKit::IFont*   mpFont;
     int                    m_isDeletedValue;
};



///////////////////////////////////////////////////////////////////////////////
// inlines
///////////////////////////////////////////////////////////////////////////////

inline FontPlatformData::FontPlatformData(WTF::HashTableDeletedValueType)
  : mpFont(0), 
    m_isDeletedValue(-1)
{ }


inline FontPlatformData::FontPlatformData()
  : mpFont(0), 
    m_isDeletedValue(0)
{ }


// I can't seem to find that this function is ever used by WebKit.
// And I don't know what it would mean to do if it was used.
inline void FontPlatformData::setFont(BalFont* /*pFont*/) const
{
}


// Note by Paul Pedriana: it is not clear to me that our hash() must use the 
// StringImpl::computeHash function or if we can use an arbitrary one.
// In any case, the StringImpl::computeHash function is more complex than 
// it needs to be for most uses and could stand to be simplified.
inline unsigned FontPlatformData::hash() const
{
    const uintptr_t hashCode = reinterpret_cast<uintptr_t>(mpFont);
    return StringImpl::computeHash(reinterpret_cast<const UChar*>(&hashCode), sizeof(hashCode) / sizeof(UChar));
}


// Note by Paul Pedriana: I'm trying to understand what isHashTableDeletedValue is about,
// as it is not documented (most of WebKit is not documented). I think it's a means for 
// an object to be seen as no longer being in the hash table, without knowing where the
// actual hash table is. So you can set a value in your class so that the hash table
// pretends the class is not there when the value is set. Usually this is because the 
// object has been deleted but the user doesn't know what hash table(s) own(s) it.
// These dead values sit around in the hash table until the next time the table is resized/rehashed.
inline bool FontPlatformData::isHashTableDeletedValue() const
{
    return (m_isDeletedValue == -1);
}



} // namespace WKAL


#endif  // Header include guard
