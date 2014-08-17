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
#include "FontPlatformData.h"
#include "CString.h"
#include "PlatformString.h"
#include "FontDescription.h"
#include "WebPreferences.h"
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/internal/EAWebKitAssert.h>
#include <EAWebKit/internal/EAWebKitString.h>
#include <EAWebKit/EAWebKitTextInterface.h>  
#include <eastl/algorithm.h>
#include <EAWebKit/EAWebKit.h>

namespace WKAL {


// Note by Paul Pedriana: Is there no means to move the font code below into some higher 
// level system? Normally I would think that this kind of factory functionality should
// not be in a lower level class. The FontCache::createFontPlatformData function might
// be just such a location for this code. We can call FontCache::createFontPlatformData
// from here if we need to.


// This is like strlcpy except handles WebCore::String objects, which aren't null-terminated
// (not without modifying them, at least). Unlike Strlcpy, we assume that destCapacity > 0.
void WebCoreStrlcpy(char16_t* pD, const WebCore::String& s, size_t destCapacity)
{
    const char16_t* const pS     = s.characters(); // This isn't null-terminated.
    unsigned              length = s.length();
    size_t                i, iEnd;

    for(i = 0, iEnd = eastl::min_alt<size_t>(length, destCapacity - 1); i < iEnd; ++i) // We can't use strcpy, strncpy, strlcpy, because p is not null-terminated.
        pD[i] = pS[i];
    pD[i] = 0;
}


bool WebCoreStrIEqual(const char16_t* pD, const WebCore::String& s)
{
    const char16_t* const pS     = s.characters(); // This isn't null-terminated.
    unsigned              length = s.length();
    size_t                i, iEnd;

    for(i = 0, iEnd = length; pD[i] && (i < iEnd); ++i)
    {
        if(towlower(pD[i]) != towlower(pS[i]))
            return false;
    }

    return (pD[i] == 0) && (i == iEnd); // Return true if we are at the end of both strings at the same time.
}


//+ 8/21/09  CSidhall - Status of text software anti-aliasing 
bool IsTextSmoothingActive(void)
{
   // We have it as a global param 
    const EA::WebKit::Parameters& parameters = EA::WebKit::GetParameters();
    bool smoothFlag = parameters.mEnableSmoothText;
    return smoothFlag;
} //- CS

//+11/30/10 CSidhall - Added for a more finer control of the smoothing
float GetTextSmoothingDefaultSize() 
{
     const EA::WebKit::Parameters& parameters = EA::WebKit::GetParameters();   
     const float size = parameters.mSmoothDefaultTextSize;
     return size;   
}

FontPlatformData::FontPlatformData(const FontDescription& fontDescription, const AtomicString& /*familyName*/, const UChar* characters, int length)
    : mpFont(NULL)
    , m_isDeletedValue(0)
{
    // Note by Paul Pedriana: This function is sometimes called by WebKit with 
    // a FontDescription that specifies the family name as "-webkit-serif",
    // "-webkit-sans-serif", "-webkit-monospace", "-webkit-cursive", or "-webkit-fantasy".
    // Clearly we are expected to translate these pseudo names into actual face
    // names such as "Arial", "Times", "Courier", etc. This seems odd to me because the 
    // WebKit per-page Settings class already has a function to retrieve the preferred 
    // face name for serif, sans-serif, etc. fonts. Why doesn't WebKit use that?
    // We don't have access to the Page or its Settings from here, but we have a global 
    // WebPreferences class which globally defines the values that are written to the 
    // per page Settings instance each time it's created. We can read that data here.


    // We need to convert FontDescription to EA::Text::TextStyle.
    EA::WebKit::IFontServer* pServer = EA::WebKit::GetFontServer();
    if(!pServer) {
        return;
    }
    EA::WebKit::IFontStyle* pTextStyle = pServer->CreateTextStyle();

    WebPreferences* pWP = WebPreferences::sharedStandardPreferences();
    WebCore::String sWPFamily;

    // Set TextStyle::mFamilyNameArray. 
    // We ignore the familyName argument because more info is present in fontDescription.family(). 
    uint32_t          i   = 0;
    const FontFamily* pFF = &fontDescription.family();

  
    uint32_t kFamilyNameArrayCapacity  = pServer->GetFamilyNameArrayCapacity();
    uint32_t kFamilyNameCapacity  = pServer->GetFamilyNameCapacity();

    while(pFF && (i < kFamilyNameArrayCapacity))
    {
        WebCoreStrlcpy(pTextStyle->GetFamilyNameArray(i), pFF->family(), kFamilyNameCapacity);

        if(*pTextStyle->GetFamilyNameArray(i) == '-')
        {
            if(EA::Internal::Strcmp(pTextStyle->GetFamilyNameArray(i), L"-webkit-standard") == 0)
                sWPFamily = pWP->standardFontFamily();
            else if(EA::Internal::Strcmp(pTextStyle->GetFamilyNameArray(i), L"-webkit-serif") == 0)
                sWPFamily = pWP->serifFontFamily();
            else if(EA::Internal::Strcmp(pTextStyle->GetFamilyNameArray(i), L"-webkit-sans-serif") == 0)
                sWPFamily = pWP->sansSerifFontFamily();
            else if(EA::Internal::Strcmp(pTextStyle->GetFamilyNameArray(i), L"-webkit-monospace") == 0)
                sWPFamily = pWP->fixedFontFamily();
            else if(EA::Internal::Strcmp(pTextStyle->GetFamilyNameArray(i), L"-webkit-cursive") == 0)
                sWPFamily = pWP->cursiveFontFamily();
            else // if(EA::Internal::Strcmp(textStyle.mFamilyNameArray[i], L"-webkit-fantasy") == 0)
                sWPFamily = pWP->fantasyFontFamily();

            WebCoreStrlcpy(pTextStyle->GetFamilyNameArray(i), sWPFamily, kFamilyNameCapacity);
        }

        // Debug code
        // const FixedFamilyString ffs(textStyle.mFamilyNameArray[i]);
        // if(gFamilyStringHashSet.find(ffs) == gFamilyStringHashSet.end())
        // {
        //     gFamilyStringHashSet.insert(ffs);
        //     OWB_OUTPUT_DEBUG_STRING_W(L"Font family: "); OWB_OUTPUT_DEBUG_STRING_W(textStyle.mFamilyNameArray[i]); OWB_OUTPUT_DEBUG_STRING_W(L"\n");
        // }

        pFF = pFF->next();
        ++i;
    }

    if((i < kFamilyNameArrayCapacity) && sWPFamily.isEmpty())  // If sWPFamily is non-empty, then we've already added a generic family name to mFamilyNameArray.
    {
        const int genericType = fontDescription.genericFamily();

        switch (genericType)
        {
            // We use Windows names, but you can set these names up as aliases with the FontServer
            // and map them to whatever you want. Maybe we should use names like "serif" and expect
            // the user to set up aliases for those.
            default:
            case FontDescription::NoFamily:
            case FontDescription::StandardFamily:
                sWPFamily = pWP->standardFontFamily();
                break;

            case FontDescription::SerifFamily:
                sWPFamily = pWP->serifFontFamily();
                break;

            case FontDescription::SansSerifFamily:
                sWPFamily = pWP->sansSerifFontFamily();
                break;

            case FontDescription::MonospaceFamily:
                sWPFamily = pWP->fixedFontFamily();
                break;

            case FontDescription::CursiveFamily:
                sWPFamily = pWP->cursiveFontFamily();
                break;

            case FontDescription::FantasyFamily:
                sWPFamily = pWP->fantasyFontFamily();
                break;
        }

        if((i > 0) && !WebCoreStrIEqual(pTextStyle->GetFamilyNameArray(i - 1), sWPFamily))  // If this family isn't already added... (well we only test the last entry, which is usually the only entry)
        {
            WebCoreStrlcpy(pTextStyle->GetFamilyNameArray(i), sWPFamily, kFamilyNameCapacity);
            i++;
        }
    }

    if(i < kFamilyNameArrayCapacity)
        *pTextStyle->GetFamilyNameArray(i) = 0;

    // Set TextStyle::mfSize
    //
    // Do we need/want to do this 72/96 dpi transformation? Fonts seem to big relative to 
    // FireFox/Safari without it. The implication is that Safari expects that the video 
    // display is 72 point instead of 96 point (the latter of which is standard on Windows).
    // 
    // Here is a table of HTML point sizes to WebKit's resulting pixel sizes.
    // Recall that you can specify text in HTML as pixels and points (and some other less common types).
    // The pixel size is always the point size * 1.33333
    //      HTML point size  | computedSize
    //      --------------------------------
    //             9         |    12.000
    //            10         |    13.333
    //            11         |    14.666
    //            12         |    16.000
    //            etc.       |     etc.
    //
    // The following calculation has been shown to perfectly match the font usage of 
    // FireFox and Internet Explorer on Windows XP.
    const float fFontSize = fontDescription.computedSize();
    pTextStyle->SetSize(fFontSize * 72.0f / 96.0f);  // (72.0 / 96.0 = 0.75)

    // Set TextStyle::mStyle
    const bool bItalic = fontDescription.italic();
    if(bItalic)
        pTextStyle->SetStyle(EA::WebKit::kStyleItalic);

    // Set TextStyle::mfWeight
    COMPILE_ASSERT( WKAL::FontWeight100 == 0, FontWeight);
    COMPILE_ASSERT( WKAL::FontWeight900 == 8, FontWeight);

    const WebCore::FontWeight fontWeight = fontDescription.weight();
    pTextStyle->SetWeight((fontWeight + 1) * 100);

    // Set TextStyle::mVariant
    if(fontDescription.smallCaps())
        pTextStyle->SetVariant(EA::WebKit::kVariantSmallCaps);

    // Set TextStyle::mPitch
    // if()
    //     textStyle.mPitch = kPitchFixed;

    // Set TextStyle::mSmooth
    // We act like FireFox under Windows does with respect to sizes and weights.
    const bool bBold = (fontWeight == WebCore::FontWeightBold);
    const float defaultSmoothSize = GetTextSmoothingDefaultSize();

    if(((fFontSize >= defaultSmoothSize) && !bItalic && !bBold) || // Normal text
       ((fFontSize >= 18) &&  bItalic &&  bBold) || // Bold italic text
       ((fFontSize >= 14) && !bItalic &&  bBold) || // Bold text
       ((fFontSize >= 22) &&  bItalic && !bBold))   // Italic text
    {
        pTextStyle->SetSmooth(EA::WebKit::kSmoothEnabled);
    }
    else
    {
        // 8/21/09 CSidhall - Added switch to allow user control (slow when active however)
        if(!IsTextSmoothingActive())     
            pTextStyle->SetSmooth(EA::WebKit::kSmoothNone);
        else    
            pTextStyle->SetSmooth(EA::WebKit::kSmoothEnabled);    
    }
    
    // 10/16/10 CSidhall - Set TextStyle::mEffect
    const EA::WebKit::TextEffectData& effectData = fontDescription.getTextEffectData();
    

    // Get the font from the FontServer.
    // The FontServer supports the concept of multiple returned fonts, one for each family name,
    //
    mpFont = pServer->GetFont(pTextStyle, NULL, 0, &effectData, length ? characters[0] : ' ');
    EAW_ASSERT(mpFont);

    pTextStyle->Destroy();
    pTextStyle = 0;

    // Debug code
    // EA::Text::FontDescription fd;
    // mpFont->GetFontDescription(fd);
    // if(EA::Text::Stricmp(textStyle.mFamilyNameArray[0], fd.mFamily) != 0)
    //    OWB_PRINTF("Font family match failure.\n");
    // else if(textStyle.mfWeight != fd.mfWeight)
    //    OWB_PRINTF("Font weight match failure.\n");
    // else if(textStyle.mStyle != fd.mStyle)
    //    OWB_PRINTF("Font style match failure.\n");

}

FontPlatformData::FontPlatformData(float size, bool bold, bool italic, const UChar* characters, int length, const EA::WebKit::TextEffectData* pEffect)
    : mpFont(NULL)
    , m_isDeletedValue(0)
{
    EA::WebKit::IFontServer* const pFontServer = EA::WebKit::GetFontServer();
    EAW_ASSERT_MSG(pFontServer, "EAText: Need to call EA::Text::SetFontServer.");
    
    // We need to convert FontDescription to EA::Text::TextStyle.
    EA::WebKit::IFontStyle* pTextStyle = pFontServer->CreateTextStyle();

    // Set TextStyle::mFamilyNameArray. 
    EA::Internal::Strlcpy( pTextStyle->GetFamilyNameArray(0), L"Arial", EA::WebKit::kFamilyNameCapacity);

    // Set TextStyle::mfSize
    pTextStyle->SetSize(size);

    // Set TextStyle::mStyle
    if(italic)
        pTextStyle->SetStyle(EA::WebKit::kStyleItalic);

    // Set TextStyle::mfWeight
    if(bold)
        pTextStyle->SetWeight( (float) EA::WebKit::kWeightBold);
    
    

    // Get the font from the FontServer.
    mpFont = pFontServer->GetFont(pTextStyle, NULL, 0, pEffect,  length ? characters[0] : ' ');
    EAW_ASSERT(mpFont);

    pTextStyle->Destroy();  // Matching the Create or it will leak.
}


// BalFontFace == EA::Text::Font
FontPlatformData::FontPlatformData(BalFontFace* pFontFace, int /*size*/, bool /*bold*/, bool /*italic*/, const UChar* /*characters*/, int /*length*/)
    : mpFont(NULL)
    , m_isDeletedValue(0)
{
    // Are we supposed to pretend that our size is 'size' and that we are bold or italic even if we aren't?
    if(pFontFace)
    {
        mpFont = pFontFace;
        mpFont->AddRef();
    }
}


/*  I don't think we need this.
bool FontPlatformData::init()
{
    // This function is intended to be the main font system init function.
    // However, we don't consider it correct for a library to do subsystem
    // initialization and instead require the application to do it, as the 
    // application probably has a better idea about how it wants things 
    // initialized than we do. 
    #if defined(EA_DEBUG) 
        #if EATEXT_FONT_SERVER_ENABLED
            EA::Text::FontServer* const pFontServer = EA::WebKit::GetFontServer();
            return (pFontServer != NULL);
        #else
            return false; // We really do need a font server.
        #endif
    #else
        // We just assume the user user initialized the font system.
        return true;
    #endif
}
*/


FontPlatformData::~FontPlatformData()
{
    // Note by Paul Pedriana: Somewhere we need to Release mpFont, but I'm not sure if
    // this is the place to do it. 
    if(mpFont)
    {
        mpFont->Release();
        mpFont = NULL;
    }
}

// 7/17/09 CSidhall - Added copy constructor to control font ref count.
// (std template pairs could copy without incrementing the font ref count and then calling the destructor which decremented the font ref count).
FontPlatformData::FontPlatformData(const FontPlatformData& r)
{
    if(r.mpFont)
    {
        r.mpFont->AddRef();
    }
    *this = r;
}


bool FontPlatformData::isFixedPitch()
{
    // Can mpFont ever be NULL?
    if(mpFont)
    {
        EA::WebKit::FontMetrics fontMetrics;
        mpFont->GetFontMetrics(fontMetrics);
        return fontMetrics.mPitch == EA::WebKit::kPitchFixed;
    }
    return false;
}


float FontPlatformData::size() const
{
    // Can mpFont ever be NULL?
    if(mpFont)
        return mpFont->GetSize();

    return 1; // 1 instead of zero just in case somebody decides to mathematically divide by it.
}


bool FontPlatformData::operator==(const FontPlatformData& other) const
{
    // Other versions of this function seem to want to compare font characteristics
    // instead of font pointers. But our font server will only ever supply unique
    // fonts, so we should be able to just compare pointers.
    return (mpFont == other.mpFont);
}


} // namespace WKAL
