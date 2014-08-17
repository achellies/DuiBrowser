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
// BCFontCacheEA.h
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "FontCache.h"
#include "Font.h"
#include "FontTraitsMask.h"
#include "SimpleFontData.h"
#include <wtf/Assertions.h>
#include "CString.h"
#include <EAWebkit/EAWebKitTextInterface.h>
#include <EAWebkit/internal/EAWebKitString.h>
#include <EAWebKit/EAWebKit.h>


namespace WKAL {


// To do: Remove usage of this function altogether by whoever happens to call it.
void FontCache::platformInit()
{
    // We don't need this. We assume the font system is fully initialized at
    // the application level.
    // if (!FontPlatformData::init())
    //     ASSERT_NOT_REACHED();
}


const SimpleFontData* FontCache::getFontDataForCharacters(const Font& font, const UChar* characters, int length)
{
    // Note by Paul Pedriana (March 17, 2009): The webkit code that calls this function currently 
    // puts the return pointer into an ever growing cache of pointers, which are deleted on shutdown.
    const FontDescription& fontDescription(font.fontDescription());
    const AtomicString&    fontFamily(font.family().family());
    FontPlatformData       fontPlatformData(fontDescription, fontFamily, characters, length);

    return new SimpleFontData(fontPlatformData);

    // To consider: The Windows version has code somewhat like this, which we may want to use.
    //              Are we wasting memory or something else non-optimal with the code above?
    // Follow up:   This function gets called a lot, so indeed it is probably useful to set up
    //              some kind of cache. The only problem is that webkit doesn't seem to refcount
    //              the returned pointer and so returning a shared pointer makes it hard to managed
    //              shared ownership lifetime, especially if webkit keeps multiple copies of the 
    //              same pointer.
    //
    // SimpleFontData*   pSimpleFontData   = NULL;
    // FontPlatformData* pFontPlatformData = getCachedFontPlatformData(font.fontDescription(), font.family().family());
    // 
    // if (pFontPlatformData)
    // {
    //     // getCachedFontData looks in a hashtable<FontPlatformData, SimpleFontData*> and 
    //     // if not found then it creates new SimpleFontData(*pFontPlatformData). 
    //     pSimpleFontData = getCachedFontData(pFontPlatformData);
    // 
    // }
    // return pSimpleFontData;
}


FontPlatformData* FontCache::getSimilarFontPlatformData(const Font& font)
{
    const FontDescription&       fontDescription(font.fontDescription());
    const WebCore::AtomicString& fontFamily(font.family().family());
    const UChar                  cSpace(' ');

    return new FontPlatformData(fontDescription, fontFamily, &cSpace, 1);
}


FontPlatformData* FontCache::getLastResortFallbackFont(const FontDescription& fontDescription)
{
    // To consider: Provide a means in the FontServer or EAWebKit to explicitly identify the 
    //              system fallback font. And use that font here. One means of doing
    //              this is to set up an alias font family called L"Fallback" and 
    //              have the user specify that alias in font system setup. That would
    //              have the benefit of decoupling this code from that code.
    // To consider: Possibly we should use L"serif" as the name instead of Arial and 
    //              expect the user to set up an alias for it in the Font Server.
    //              That is, if we don't do the fallback solution above.

    // getCachedFontPlatformData is a function that looks in gFontPlatformDataCache for
    // a font that matches the fontDescription and if it doesn't find one it tries 
    // looking under an alias for the family name. But it returns NULL if not found.
    // To consider: Since getCachedFontPlatformData can return NULL, should we test
    // for that here and find a way to not return NULL? Or maybe since we are FontCache
    // we can return NULL as well and the caller will then use createFontPlatformData.
    return getCachedFontPlatformData(fontDescription, AtomicString(L"Arial"));
}


//#ifdef EA_DEBUG
//    // Used to temporarily help track down memory leaks.
//    int gFPDCount = 0;
//    FontPlatformData* gFPDArray[128]; 
//#endif

FontPlatformData* FontCache::createFontPlatformData(const FontDescription& fontDescription, const AtomicString& family)
{
    // To do: Move the factory code from the FontPlatformData ctor to here. We should be doing this work
    //        and call the FontPlatformData(EA::Text::Font* pFont, int size, bool bold, bool italic) ctor here.
    const UChar cSpace(' ');

    FontPlatformData* pFPD = new FontPlatformData(fontDescription, family, &cSpace, 1);

    //#ifdef EA_DEBUG
    //    if(gFPDCount < 128)
    //        gFPDArray[++gFPDCount] = pFPD; 
    //#endif

    return pFPD;
}


void FontCache::getTraitsInFamily(const AtomicString& familyName, Vector<unsigned>& traitsMaskArray)
{
    // What we need to do is find all fonts that are of the given familyName and for each
    // one add a vector entry of its properties defined in enum FontTraitsMask.
    EA::WebKit::IFontServer* const pFontServer = EA::WebKit::GetFontServer(); 
    if(!pFontServer)
        return;

    #if defined(EA_PLATFORM_WINDOWS) | defined(EA_PLATFORM_UNIX)
        const size_t kCapacity = 48;    // This correlates to the number of expected .ttf (or other type of font) files registered with the FontServer.
    #else                               // Too large of a kCapcity value could blow the stack on weaker machines.
        const size_t kCapacity = 16;
    #endif

    EA::WebKit::IFontDescription  pFontDescriptionArrayLocal[kCapacity];
    EA::WebKit::IFontDescription* pFontDescriptionArrayAllocated = NULL;
    EA::WebKit::IFontDescription* pFontDescriptionArray;

    // FontServer has only one basic font enumeration function, and it simply copies all 
    // FontDescription data to the user. We need to wade through it.
    uint32_t count = pFontServer->EnumerateFonts(NULL, 0);
    pFontDescriptionArray = (count <= kCapacity) ? pFontDescriptionArrayLocal : (pFontDescriptionArrayAllocated = new EA::WebKit::IFontDescription[count]);
    count = pFontServer->EnumerateFonts(pFontDescriptionArray, count);

    // Make an alias to the family name for frequent use below.
    const char16_t* const pFamilyName = familyName.characters();

    for(uint32_t i = 0; i < count; ++i)
    {
        const EA::WebKit::IFontDescription& fd = pFontDescriptionArray[i];

        if(EA::Internal::Stricmp(fd.mFamily, pFamilyName) == 0) // If the family name matches...
        {
            unsigned traitsMask = 0;

            // WebKit doesn't recognize the CSS oblique style (http://www.w3schools.com/css/pr_font_font-style.asp), so we pretend oblique is the same as italic.
            traitsMask |= (fd.mStyle == EA::WebKit::kStyleNormal) ? WKAL::FontStyleNormalMask : WKAL::FontStyleItalicMask;

            traitsMask |= (fd.mVariant == EA::WebKit::kVariantNormal) ? WKAL::FontVariantNormalMask : WKAL::FontVariantSmallCapsMask;

            // Convert EAText mfWeight (float values in range of 100.f to 900.f) to WebKit's
            // FontWeightNNNMask (bits in the range of 1<<4 to 1<<12).
            COMPILE_ASSERT(WKAL::FontWeight100Bit == 4, FontWeight);
            COMPILE_ASSERT(WKAL::FontWeight900Bit == 12, FontWeight);
            traitsMask |= (1 << (((int)fd.mfWeight / 100) + 3));

            // Ideally we'd check to make sure traitsMask isn't already in traitsMaskArray, but this will usually or always be so in our case.
            traitsMaskArray.append(traitsMask);
        }
    }

    delete[] pFontDescriptionArrayAllocated; // OK if this is NULL.
}


} // namespace WKAL
