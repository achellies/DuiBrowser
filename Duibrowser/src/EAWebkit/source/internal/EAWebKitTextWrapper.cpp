/*
Copyright (C) 2009-2010 Electronic Arts, Inc.  All rights reserved.

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
// EAWebKitTextWrapper.cpp
// 6/16/09 CSidhall - Wrapper example for EAText package for EAWebKit
// 8/9/10 CS -Added more interfaces for structs and enums.
///////////////////////////////////////////////////////////////////////////////

#include <EAAssert/eaassert.h>
#include <EABase/EABase.h>
#include <EAWebKit/Internal/EAWebKitTextWrapper.h>
#include <CoreAllocator/icoreallocator_interface.h>
#include <wtf/FastAllocBase.h>
#include <EAWebKit/internal/EAWebKitAssert.h>
#include <EAWebKit/EAWebKit.h>


namespace EA
{
    namespace TextWrapper
    {
// --- Various interface functions to convert enums and structures from EA::Text and WebKit internal text --- 

static void ConvertToInternal_GlyphMetrics(const EA::Text::GlyphMetrics& in, EA::WebKit::GlyphMetrics& out)
{
    memset(&out,0, sizeof(EA::WebKit::GlyphMetrics));  // We fill for it does not have a constructor            
    
    out.mfSizeX = in.mfSizeX;           
    out.mfSizeY = in.mfSizeY;            
    out.mfHBearingX = in.mfHBearingX;        
    out.mfHBearingY = in.mfHBearingY;        
    out.mfHAdvanceX = in.mfHAdvanceX;     
}

static EA::WebKit::Pitch GetInternal_Pitch(const EA::Text::Pitch pitchExternal)
{
    EA::WebKit::Pitch pitchInternal;
    switch(pitchExternal)
    {
        case EA::Text::kPitchVariable:
         pitchInternal = EA::WebKit::kPitchVariable;
            break;
        case EA::Text::kPitchFixed:
           pitchInternal = EA::WebKit::kPitchFixed;
           break;
        default:    
            EAW_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Script Value.");           
            pitchInternal = EA::WebKit::kPitchVariable;
            break;
    }
    return pitchInternal;
}

static void ConvertToInternal_FontMetrics(EA::Text::FontMetrics& in, EA::WebKit::FontMetrics& out)
{
    out.mfSize                  = in.mfSize;                  
    out.mPitch                  = GetInternal_Pitch(in.mPitch);                   
    out.mfHAdvanceXMax          = in.mfHAdvanceXMax;           
    out.mfVAdvanceYMax          = in.mfVAdvanceYMax;           
    out.mfAscent                = in.mfAscent;                 
    out.mfDescent               = in.mfDescent;                
    out.mfLeading               = in.mfLeading;                
    out.mfBaseline              = in.mfBaseline;               
    out.mfLineHeight            = in.mfLineHeight;            
    out.mfXHeight               = in.mfXHeight;                
    out.mfCapsHeight            = in.mfCapsHeight;             
    out.mfUnderlinePosition     = in.mfUnderlinePosition;      
    out.mfUnderlineThickness    = in.mfUnderlineThickness;     
    out.mfLinethroughPosition   = in.mfLinethroughPosition;    
    out.mfLinethroughThickness  = in.mfLinethroughThickness;   
    out.mfOverlinePosition      = in.mfOverlinePosition;       
    out.mfOverlineThickness     = in.mfOverlineThickness;      
};

static void ConvertToInternal_Kerning(const EA::Text::Kerning& in, EA::WebKit::Kerning& out)
{
    memset(&out,0, sizeof(EA::WebKit::Kerning));  // We fill for it does not have a constructor            
    out.mfKernX = in.mfKernX;
    out.mfKernY = in.mfKernY;
}

static EA::Text::Script GetExternal_Script(const EA::WebKit::Script scriptInternal)
{
    EA::Text::Script scriptExternal;            
    
    switch(scriptInternal)
    {
           case EA::WebKit::kScriptUnknown: 
               scriptExternal = EA::Text::kScriptUnknown;
               break;
           case EA::WebKit::kScriptCommon:
               scriptExternal = EA::Text::kScriptCommon;
               break;
           case EA::WebKit::kScriptInherited:
               scriptExternal = EA::Text::kScriptInherited;
               break;
           case EA::WebKit::kScriptStart:
               scriptExternal = EA::Text::kScriptStart;
               break;
           case EA::WebKit::kScriptArmenian:
               scriptExternal = EA::Text::kScriptArmenian;
               break;
           case EA::WebKit::kScriptBengali:
               scriptExternal = EA::Text::kScriptBengali;
               break;
           case EA::WebKit::kScriptBraille:
               scriptExternal = EA::Text::kScriptBraille;
               break;
           case EA::WebKit::kScriptBopomofo:
               scriptExternal = EA::Text::kScriptBopomofo;
               break;
           case EA::WebKit::kScriptBuhid:
               scriptExternal = EA::Text::kScriptBuhid;
               break;
           case EA::WebKit::kScriptCanadianAboriginal:
               scriptExternal = EA::Text::kScriptCanadianAboriginal;
               break;
           case EA::WebKit::kScriptCherokee:
               scriptExternal = EA::Text::kScriptCherokee;
               break;
           case EA::WebKit::kScriptCoptic:
               scriptExternal = EA::Text::kScriptCoptic;
               break;
           case EA::WebKit::kScriptCypriot:
               scriptExternal = EA::Text::kScriptCypriot;
               break;
           case EA::WebKit::kScriptCyrillic:
               scriptExternal = EA::Text::kScriptCyrillic;
               break;
           case EA::WebKit::kScriptDeseret:
               scriptExternal = EA::Text::kScriptDeseret;
               break;
           case EA::WebKit::kScriptDevanagari:
               scriptExternal = EA::Text::kScriptDevanagari;
               break;
           case EA::WebKit::kScriptEthiopic:
               scriptExternal = EA::Text::kScriptEthiopic;
               break;
           case EA::WebKit::kScriptGeorgian:
               scriptExternal = EA::Text::kScriptGeorgian;
               break;
           case EA::WebKit::kScriptGothic:
               scriptExternal = EA::Text::kScriptGothic;
               break;
           case EA::WebKit::kScriptGreek:
               scriptExternal = EA::Text::kScriptGreek;
               break;
           case EA::WebKit::kScriptGujarati:
               scriptExternal = EA::Text::kScriptGujarati;
               break;
           case EA::WebKit::kScriptGurmukhi:
               scriptExternal = EA::Text::kScriptGurmukhi;
               break;
           case EA::WebKit::kScriptHan:
               scriptExternal = EA::Text::kScriptHan;
               break;
           case EA::WebKit::kScriptHangul:
               scriptExternal = EA::Text::kScriptHangul;
               break;
           case EA::WebKit::kScriptHanunoo:
               scriptExternal = EA::Text::kScriptHanunoo; 
               break;
           case EA::WebKit::kScriptHebrew:
               scriptExternal = EA::Text::kScriptHebrew;
               break;
           case EA::WebKit::kScriptHiragana:
               scriptExternal = EA::Text::kScriptHiragana;
               break;
           case EA::WebKit::kScriptKannada:
               scriptExternal = EA::Text::kScriptKannada;
               break;
           case EA::WebKit::kScriptKatakana:
               scriptExternal = EA::Text::kScriptKatakana;
               break;
           case EA::WebKit::kScriptKhmer:
               scriptExternal = EA::Text::kScriptKhmer;
               break;
           case EA::WebKit::kScriptLao:
               scriptExternal = EA::Text::kScriptLao;
               break;
           case EA::WebKit::kScriptLatin:
               scriptExternal = EA::Text::kScriptLatin;
               break;
           case EA::WebKit::kScriptLimbu:
               scriptExternal = EA::Text::kScriptLimbu;
               break;
           case EA::WebKit::kScriptLinearB:
               scriptExternal = EA::Text::kScriptLinearB;
               break;
           case EA::WebKit::kScriptMalayalam:
               scriptExternal = EA::Text::kScriptMalayalam;
               break;
           case EA::WebKit::kScriptMongolian:
               scriptExternal = EA::Text::kScriptMongolian;
               break;
           case EA::WebKit::kScriptMyanmar:
               scriptExternal = EA::Text::kScriptMyanmar;
               break;
           case EA::WebKit::kScriptOgham:
               scriptExternal = EA::Text::kScriptOgham;
               break;
           case EA::WebKit::kScriptOldItalic:
               scriptExternal = EA::Text::kScriptOldItalic;
               break;
           case EA::WebKit::kScriptOriya:
               scriptExternal = EA::Text::kScriptOriya;
               break;
           case EA::WebKit::kScriptOsmanya:
               scriptExternal = EA::Text::kScriptOsmanya;
               break;
           case EA::WebKit::kScriptRunic:
               scriptExternal = EA::Text::kScriptRunic;
               break;
           case EA::WebKit::kScriptShavian:
               scriptExternal = EA::Text::kScriptShavian;
               break;
           case EA::WebKit::kScriptSinhala:
               scriptExternal = EA::Text::kScriptSinhala;
               break;
           case EA::WebKit::kScriptSyriac:
               scriptExternal = EA::Text::kScriptSyriac;
               break;
           case EA::WebKit::kScriptTagalog:
               scriptExternal = EA::Text::kScriptTagalog;
               break;
           case EA::WebKit::kScriptTagbanwa:
               scriptExternal = EA::Text::kScriptTagbanwa;
               break;
           case EA::WebKit::kScriptTaiLe:
               scriptExternal = EA::Text::kScriptTaiLe;
               break;
           case EA::WebKit::kScriptTamil:
               scriptExternal = EA::Text::kScriptTamil;
               break;
           case EA::WebKit::kScriptTelugu:
               scriptExternal = EA::Text::kScriptTelugu;
               break;
           case EA::WebKit::kScriptThaana:
               scriptExternal = EA::Text::kScriptThaana;
               break;
           case EA::WebKit::kScriptThai:
               scriptExternal = EA::Text::kScriptThai;
               break;
           case EA::WebKit::kScriptTibetan:
               scriptExternal = EA::Text::kScriptTibetan;
               break;
           case EA::WebKit::kScriptUgaritic:
               scriptExternal = EA::Text::kScriptUgaritic;
               break;
           case EA::WebKit::kScriptYi:
               scriptExternal = EA::Text::kScriptYi;
               break;
           default:
               EAW_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Script Value.");
               scriptExternal = EA::Text::kScriptUnknown;
               break;
    }    
    return  scriptExternal;
}

static EA::Text::FontType GetExternal_FontType(const EA::WebKit::FontType fontTypeInternal)
{
    EA::Text::FontType fontTypeExternal;        
    switch(fontTypeInternal)
    {
        case EA::WebKit::kFontTypeUnknown:
            fontTypeExternal = EA::Text::kFontTypeUnknown;
            break;
        case EA::WebKit::kFontTypeBitmap:
            fontTypeExternal = EA::Text::kFontTypeBitmap;
            break;
        case EA::WebKit::kFontTypeOutline:
            fontTypeExternal = EA::Text::kFontTypeOutline;
            break;
        case EA::WebKit::kFontTypeStroke:
            fontTypeExternal = EA::Text::kFontTypeStroke;
            break;
        case EA::WebKit::kFontTypePolygon:
            fontTypeExternal = EA::Text::kFontTypePolygon;
            break;
        default:
            EAW_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Font Type");
            fontTypeExternal = EA::Text::kFontTypeUnknown;
            break;
    }
    return fontTypeExternal;
}

static EA::WebKit::BidiClass GetInternal_BidiClass(const EA::Text::BidiClass typeExternal)
{
    EA::WebKit::BidiClass typeInternal;        
    switch(typeExternal)
    {
        case EA::Text::kBidiClassON:
             typeInternal = EA::WebKit::kBidiClassON; 
             break;
        case EA::Text::kBidiClassL:
             typeInternal = EA::WebKit::kBidiClassL; 
             break;
        case EA::Text::kBidiClassR:
             typeInternal = EA::WebKit::kBidiClassR; 
             break;
        case EA::Text::kBidiClassAN:
             typeInternal = EA::WebKit::kBidiClassAN;
             break;
        case EA::Text::kBidiClassEN:
             typeInternal = EA::WebKit::kBidiClassEN;
             break;
        case EA::Text::kBidiClassAL:
             typeInternal = EA::WebKit::kBidiClassAL;
             break;
        case EA::Text::kBidiClassNSM:
             typeInternal = EA::WebKit::kBidiClassNSM;
             break;
        case EA::Text::kBidiClassCS:
             typeInternal = EA::WebKit::kBidiClassCS;
             break;
        case EA::Text::kBidiClassES:
             typeInternal = EA::WebKit::kBidiClassES;
             break;
        case EA::Text::kBidiClassET:
             typeInternal = EA::WebKit::kBidiClassET;
             break;
        case EA::Text::kBidiClassBN:
             typeInternal = EA::WebKit::kBidiClassBN;
             break;
        case EA::Text::kBidiClassS:
             typeInternal = EA::WebKit::kBidiClassS;
             break;
        case EA::Text::kBidiClassWS:
             typeInternal = EA::WebKit::kBidiClassWS;
             break;
        case EA::Text::kBidiClassB:
             typeInternal = EA::WebKit::kBidiClassB; 
             break;
        case EA::Text::kBidiClassRLO:
             typeInternal = EA::WebKit::kBidiClassRLO;
             break;
        case EA::Text::kBidiClassRLE:
             typeInternal = EA::WebKit::kBidiClassRLE;
             break;
        case EA::Text::kBidiClassLRO:
             typeInternal = EA::WebKit::kBidiClassLRO;
             break;
        case EA::Text::kBidiClassLRE:
             typeInternal = EA::WebKit::kBidiClassLRE;
             break;
        case EA::Text::kBidiClassPDF:
            typeInternal = EA::WebKit::kBidiClassPDF;
             break;
        default:
            EAW_ASSERT_MSG(false, "TextWrapper: Warning: Unknown BidClass Type");
            typeInternal = EA::WebKit::kBidiClassON; 
            break;
    }
    return typeInternal;
}

static void ConvertToInternal_GlyphTextureInfo(const EA::Text::GlyphTextureInfo &in, EA::WebKit::IGlyphTextureInfo &out)
{
    if(out.mpTextureInfo)
    {
        out.mpTextureInfo->SetTextureInfoPointer(in.mpTextureInfo);
    }
    else if(in.mpTextureInfo)
    {
        out.mpTextureInfo = GlyphCacheProxy::CreateTextureInfoProxy(in.mpTextureInfo);
    }
    out.mX1 = in.mX1;
    out.mY1 = in.mY1;
    out.mX2 = in.mX2;
    out.mY2 = in.mY2;
}

static void ConvertToExternal_GlyphTextureInfo(const EA::WebKit::IGlyphTextureInfo &in, EA::Text::GlyphTextureInfo &out)
{
    memset(&out,0, sizeof(EA::Text::GlyphTextureInfo));  // We fill for it might not have a constructor            
    EA::Text::TextureInfo* pTextureInfoEA = NULL;
    if(in.mpTextureInfo)
        pTextureInfoEA = static_cast<EA::Text::TextureInfo*> (in.mpTextureInfo->GetTextureInfoPointer());
    out.mpTextureInfo = pTextureInfoEA;
    out.mX1 = in.mX1;
    out.mY1 = in.mY1;
    out.mX2 = in.mX2;
    out.mY2 = in.mY2;
}

static EA::Text::Smooth GetExternal_Smooth(const EA::WebKit::Smooth typeInternal)
{
    EA::Text::Smooth typeExternal;        
    switch(typeInternal)
    {
        case EA::WebKit::kSmoothNone:  
            typeExternal = EA::Text::kSmoothNone;
            break;
        case EA::WebKit::kSmoothEnabled:
            typeExternal = EA::Text::kSmoothEnabled;
            break;
        default:
            EAW_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Smooth Type"); 
            typeExternal = EA::Text::kSmoothNone;
            break;
    }
    return typeExternal;
}

static EA::WebKit::Smooth GetInternal_Smooth(const EA::Text::Smooth typeExternal)
{
    EA::WebKit::Smooth typeInternal;        
    switch(typeExternal)
    {
        case EA::Text::kSmoothNone:       
            typeInternal = EA::WebKit::kSmoothNone;
            break;
        case EA::Text::kSmoothEnabled:
            typeInternal = EA::WebKit::kSmoothEnabled;
            break;
        default:
             EAW_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Smooth Type"); 
            typeInternal = EA::WebKit::kSmoothNone;
            break;
    }
    return typeInternal;
}

static EA::Text::Variant GetExternal_Variant(const EA::WebKit::Variant typeInternal)
{
    EA::Text::Variant typeExternal;        
    switch(typeInternal)
    {
         case EA::WebKit::kVariantNormal:
            typeExternal = EA::Text::kVariantNormal;
            break;
         case EA::WebKit::kVariantSmallCaps:
            typeExternal = EA::Text::kVariantSmallCaps;
            break;
        default:
             EAW_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Variant Type"); 
            typeExternal = EA::Text::kVariantNormal;
            break;
    }
    return typeExternal;
}

static EA::WebKit::Variant GetInternal_Variant(const EA::Text::Variant typeExternal)
{
    EA::WebKit::Variant typeInternal;        
    switch(typeExternal)
    {
        case EA::Text::kVariantNormal:    
            typeInternal = EA::WebKit::kVariantNormal;
            break;
        case EA::Text::kVariantSmallCaps:
            typeInternal = EA::WebKit::kVariantSmallCaps;
            break;
        default:
             EAW_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Variant Type"); 
            typeInternal = EA::WebKit::kVariantNormal;
            break;
    }
    return typeInternal;
}

static EA::WebKit::Style GetInternal_Style(const EA::Text::Style typeExternal)
{
    EA::WebKit::Style typeInternal;        
    switch(typeExternal)
    {
        case EA::Text::kStyleNormal:
            typeInternal = EA::WebKit::kStyleNormal;
            break;

        case EA::Text::kStyleOblique:
            typeInternal = EA::WebKit::kStyleOblique;
            break;
        case EA::Text::kStyleItalic:
            typeInternal = EA::WebKit::kStyleItalic;
            break;
        
        default:
            EAW_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Style Type"); 
            typeInternal = EA::WebKit::kStyleNormal;
            break;
    }
    return typeInternal;
}

static EA::Text::Style GetExternal_Style(const EA::WebKit::Style typeInternal)
{
    EA::Text::Style typeExternal;        
    switch(typeInternal)
    {
        case EA::WebKit::kStyleNormal:
            typeExternal = EA::Text::kStyleNormal;
            break;
        case EA::WebKit::kStyleOblique:
            typeExternal = EA::Text::kStyleOblique;
            break;
        case EA::WebKit::kStyleItalic:
            typeExternal = EA::Text::kStyleItalic;
            break;
        default:
             EAW_ASSERT_MSG(false, "TextWrapper: Warning: Unknown Style Type"); 
            typeExternal = EA::Text::kStyleNormal;
            break;
    }
    return typeExternal;
}

static EA::WebKit::Effect GetInternal_Effect(uint32_t type)
{
    EA::Text::Effect typeExternal = static_cast<EA::Text::Effect> (type);
    EA::WebKit::Effect typeInternal;        
    switch(typeExternal)
    {
        case EA::Text::kEffectNone:
            typeInternal = EA::WebKit::kEffectNone;
            break;
        case EA::Text::kEffectOutline:
            typeInternal = EA::WebKit::kEffectOutline;
            break;
        case EA::Text::kEffectShadow:
            typeInternal = EA::WebKit::kEffectShadow;
            break;
        case EA::Text::kEffectRaised:
            typeInternal = EA::WebKit::kEffectRaised;
            break;
        case EA::Text::kEffectDepressed:
            typeInternal = EA::WebKit::kEffectDepressed;
            break;
        case EA::Text::kEffectUser:    
            typeInternal = EA::WebKit::kEffectUser;
            break;
        default:
            EAW_ASSERT_MSG(type > EA::Text::kEffectUser, "TextWrapper: Warning: Unknown Effect Type"); 
            // Possible user effects to be converted back
            typeInternal = static_cast<EA::WebKit::Effect> (type); 
            break;
    }
    return typeInternal;
}


static uint32_t GetExternal_Effect(uint32_t type)
{
    EA::WebKit::Effect typeInternal = static_cast<EA::WebKit::Effect> (type);
    EA::Text::Effect typeExternal;        
    switch(typeInternal)
    {
        case EA::WebKit::kEffectNone:
            typeExternal = EA::Text::kEffectNone;
            break;
        case EA::WebKit::kEffectOutline:
            typeExternal = EA::Text::kEffectOutline;
            break;
        case EA::WebKit::kEffectShadow:
            typeExternal = EA::Text::kEffectShadow;
            break;
        case EA::WebKit::kEffectRaised:
            typeExternal = EA::Text::kEffectRaised;
            break;
        case EA::WebKit::kEffectDepressed:
            typeExternal = EA::Text::kEffectDepressed;
            break;
        case EA::WebKit::kEffectUser:    
            typeExternal = EA::Text::kEffectUser;
            break;
        default:
            EAW_ASSERT_MSG(type > EA::WebKit::kEffectUser, "TextWrapper: Warning: Unknown Effect Type"); 
            // Possible user effects to be converted back
            typeExternal = static_cast<EA::Text::Effect> (type); 
            break;   
    }
    return (static_cast<uint32_t> (typeExternal));
}


static EA::Text::Char GetExternal_Char(EA::WebKit::Char c)
{
    // This is just a typedef  
    return static_cast<EA::Text::Char> (c);
}

static EA::WebKit::Char GetInternal_Char(EA::Text::Char c)
{
    // This is just a typedef  
    return static_cast<EA::WebKit::Char> (c);
}

static EA::WebKit::Color GetInternalColor(EA::Text::Color color)
{
    // This is just a typedef  typedef uint32_t Color;
    return static_cast<EA::WebKit::Color> (color);
}

static inline EA::Text::Color GetExternalColor(EA::WebKit::Color color)
{
    // This is just a typedef  typedef uint32_t Color;
    return static_cast<EA::Text::Color> (color);
}

static EA::Text::GlyphId GetExternal_GlyphId(EA::WebKit::GlyphId glyphId)
{
    return static_cast<EA::Text::GlyphId> (glyphId);
}

static void ConvertToInternal_FontDescription(const EA::Text::FontDescription& in, EA::WebKit::IFontDescription& out)
{
    // Seems the EAText version does not copy everything over in the default copy as not all info is needed to match a font but 
    // better be safe in case it ends up being used differently.
    for(uint32_t i=0; i < EA::WebKit::kFamilyNameCapacity; ++i)
    {
        if(i >= EA::Text::kFamilyNameCapacity)
            break;
        out.mFamily[i] =  GetInternal_Char(in.mFamily[i]);
    }
    
    out.mfSize              = in.mfSize;     
    out.mStyle              = GetInternal_Style(in.mStyle);                       
    out.mfWeight            = in.mfWeight;
    out.mfStretch           = in.mfStretch;
    out.mPitch              = GetInternal_Pitch(in.mPitch);  
    out.mVariant            = GetInternal_Variant(in.mVariant);
    out.mSmooth             = GetInternal_Smooth(in.mSmooth);
    out.mEffect             = GetInternal_Effect(in.mEffect);
    out.mfEffectX           = in.mfEffectX;
    out.mfEffectY           = in.mfEffectY;
    out.mEffectBaseColor    = GetInternalColor(in.mEffectBaseColor);
    out.mEffectColor        = GetInternalColor(in.mEffectColor);
    out.mHighLightColor     = GetInternalColor(in.mHighLightColor);
}

static EA::WebKit::CharCategory GetInternal_CharCategory(const EA::Text::CharCategory typeExternal)
{
    EA::WebKit::CharCategory typeInternal;        
    switch(typeExternal)
    {
        case EA::Text::kCCUnassigned:
        //case EA::Text::kCCGeneralOtherTypes:
            typeInternal = EA::WebKit::kCCGeneralOtherTypes;      
            break;
        case EA::Text::kCCControlChar:
            typeInternal = EA::WebKit::kCCControlChar;            
            break;
        case EA::Text::kCCFormatChar:
            typeInternal = EA::WebKit::kCCFormatChar;             
            break;
        case EA::Text::kCCPrivateUseChar:
            typeInternal = EA::WebKit::kCCPrivateUseChar;         
            break;
        case EA::Text::kCCSurrogate:
            typeInternal = EA::WebKit::kCCSurrogate;              
            break;
        case EA::Text::kCCUppercaseLetter:
            typeInternal = EA::WebKit::kCCUppercaseLetter;        
            break;
        case EA::Text::kCCLowercaseLetter:
            typeInternal = EA::WebKit::kCCLowercaseLetter;        
            break;
        case EA::Text::kCCTitlecaseLetter:
            typeInternal = EA::WebKit::kCCTitlecaseLetter;        
            break;
        case EA::Text::kCCModifierLetter:
            typeInternal = EA::WebKit::kCCModifierLetter;         
            break;
        case EA::Text::kCCOtherLetter:
            typeInternal = EA::WebKit::kCCOtherLetter;            
            break;
        case EA::Text::kCCNonSpacingMark:
            typeInternal = EA::WebKit::kCCNonSpacingMark;         
            break;
        case EA::Text::kCCEnclosingMark:
            typeInternal = EA::WebKit::kCCEnclosingMark;          
            break;
        case EA::Text::kCCCombiningSpacingMark:
            typeInternal = EA::WebKit::kCCCombiningSpacingMark;   
            break;
        case EA::Text::kCCDecimalDigitNumber:
            typeInternal = EA::WebKit::kCCDecimalDigitNumber;     
            break;
        case EA::Text::kCCLetterNumber:
            typeInternal = EA::WebKit::kCCLetterNumber;           
            break;
        case EA::Text::kCCOtherNumber:
            typeInternal = EA::WebKit::kCCOtherNumber;            
            break;
        case EA::Text::kCCSpaceSeparator:
            typeInternal = EA::WebKit::kCCSpaceSeparator;         
            break;
        case EA::Text::kCCLineSeparator:
            typeInternal = EA::WebKit::kCCLineSeparator;          
            break;
        case EA::Text::kCCParagraphSeparator:
            typeInternal = EA::WebKit::kCCParagraphSeparator;     
            break;
        case EA::Text::kCCDashPunctuation:
            typeInternal = EA::WebKit::kCCDashPunctuation;        
            break;
        case EA::Text::kCCStartPunctuation:
            typeInternal = EA::WebKit::kCCStartPunctuation;       
            break;
        case EA::Text::kCCEndPunctuation:
            typeInternal = EA::WebKit::kCCEndPunctuation;         
            break;
        case EA::Text::kCCConnectorPunctuation:
            typeInternal = EA::WebKit::kCCConnectorPunctuation;   
            break;
        case EA::Text::kCCOtherPunctuation:
            typeInternal = EA::WebKit::kCCOtherPunctuation;       
            break;
        case EA::Text::kCCInitialPunctuation:
            typeInternal = EA::WebKit::kCCInitialPunctuation;     
            break;
        case EA::Text::kCCFinalPunctuation:
            typeInternal = EA::WebKit::kCCFinalPunctuation;       
            break;
        case EA::Text::kCCMathSuymbol:
            typeInternal = EA::WebKit::kCCMathSuymbol;            
            break;
        case EA::Text::kCCCurrencySymbol:
            typeInternal = EA::WebKit::kCCCurrencySymbol;         
            break;
        case EA::Text::kCCModifierSymbol:
            typeInternal = EA::WebKit::kCCModifierSymbol;         
            break;
        case EA::Text::kCCOtherSymbol:
            typeInternal = EA::WebKit::kCCOtherSymbol;            
            break;
        default:
             EAW_ASSERT_MSG(false, "TextWrapper: Warning: Unknown CharCategory Type"); 
            typeInternal = EA::WebKit::kCCUnassigned;             
            break;
    }
    return typeInternal;
}

// For locally created glyph server
static GlyphCacheProxy*   spGlyphCacheWrapperInterface = NULL; 
// For controlling if the font system has init
static bool sFontSystemHasInit = false;

void InitFontSystem()
{
    if(!sFontSystemHasInit)
    {
        EA::Text::Init();
        sFontSystemHasInit = true;
    }
}

void ShutDownFontSystem()
{
    if(sFontSystemHasInit)
    {
        EA::Text::Shutdown();
        sFontSystemHasInit = false;
    }
}    

//+ Grabbed these following macro/templates EAText (written by Paul Pedriana)
#define EATEXT_WRAPPER_NEW(Class, pAllocator, pName) \
    new ((pAllocator)->Alloc(sizeof(Class), pName, 0, EA_ALIGN_OF(Class), 0)) Class

template <typename T>
inline void delete_object(T* pObject, Allocator::ICoreAllocator* pAllocator)
{
    if(pObject) // As per the C++ standard, deletion of NULL results in a no-op.
    {
        pObject->~T();
        pAllocator->Free(pObject);
    }
}

#define EATEXT_WRAPPER_DELETE(pObject, pAllocator) delete_object(pObject,pAllocator)
//-

// Small helper function to find allocator used
static Allocator::ICoreAllocator* GetAllocator_Helper()
{
    Allocator::ICoreAllocator* pAllocator = EA::Text::GetAllocator();        
    if(!pAllocator)
            pAllocator = Allocator::ICoreAllocator::GetDefaultAllocator();

    return pAllocator;
}

// --- Font Wrappers ---
void FontProxy::SetAllocator(Allocator::ICoreAllocator* pCoreAllocator)
{
    EAW_ASSERT(mpFont);
    if(mpFont)
        mpFont->SetAllocator(pCoreAllocator);        
}

int FontProxy::AddRef()
{
    mRefCount++;           // Proxy keeps it's own ref count for could be different than font ref

    int returnValue =0;
    EAW_ASSERT(mpFont);
    if(mpFont)
        returnValue = mpFont->AddRef(); 
    return returnValue;
}

int FontProxy::Release()
{
    mRefCount--;

    int returnValue =0;
    EAW_ASSERT(mpFont);
    if(mpFont) 
        returnValue =  mpFont->Release();

    if((returnValue <= 0) ||(mRefCount <= 0))
        DestroyWrapper();

    return returnValue;
}

bool FontProxy::GetFontMetrics(EA::WebKit::FontMetrics& fontMetricsInternal)
{
    EAW_ASSERT(mpFont);
    bool returnFlag = false;
    if(mpFont)
    {
        EA::Text::FontMetrics fontMetricsExternal;
        returnFlag = mpFont->GetFontMetrics(fontMetricsExternal);
        ConvertToInternal_FontMetrics(fontMetricsExternal, fontMetricsInternal);
    }
    return returnFlag;
}

bool FontProxy::GetGlyphMetrics(EA::WebKit::GlyphId glyphId, EA::WebKit::GlyphMetrics& glyphMetricsInternal)
{
    EAW_ASSERT(mpFont);
    bool returnFlag = false;
    if(mpFont)
    {
        // 8/23/10 - This it to fix a possible acid 3 crash. 
        // If the transform was not set, it can crash EAText in certain cases, depending on its configuration.
        // Seems if a font is created by type (outline) and then opened, it can bypass the transform setting which is 
        // normally done when using FontServer::CreateNewFont(FaceSource* pFaceSource, const TextStyle& ssCSS, bool bManaged) in EAText.
        // So if no size is found, we assume it has not been set up and init the transforms with a default size ourselves.
        float size = mpFont->GetSize();
        const float kDefaultFontSize = 10.0f;
        if(size == 0.0f)
            mpFont->SetTransform(kDefaultFontSize);

        EA::Text::GlyphMetrics glyphMetricsExternal;
        EA::Text::GlyphId glyphIdExternal = GetExternal_GlyphId(glyphId);
        returnFlag= mpFont->GetGlyphMetrics(glyphIdExternal, glyphMetricsExternal);
        ConvertToInternal_GlyphMetrics(glyphMetricsExternal, glyphMetricsInternal);
    }
    return returnFlag;
}


uint32_t FontProxy::GetGlyphIds(const EA::WebKit::Char* pCharArray, uint32_t nCharArrayCount, EA::WebKit::GlyphId* pGlyphIdArray, 
                             bool bUseReplacementGlyph, const uint32_t nGlyphIdStride, bool bWriteInvalidGlyphs)
{
    EAW_ASSERT(mpFont);
    int returnValue = 0;
    if(mpFont)
    {
        // Seems that in all EAWebKit use cases, only 1 count is used so casting the typdef is probably less dangerous here than it appears.
        // We also runtime compile check to make sure the sizes match
        returnValue = mpFont->GetGlyphIds( static_cast<const EA::Text::Char*> (pCharArray), nCharArrayCount, static_cast<EA::Text::GlyphId*> (pGlyphIdArray),
                                           bUseReplacementGlyph, nGlyphIdStride, bWriteInvalidGlyphs);
    }
    return returnValue;
}

bool FontProxy::IsCharSupported(EA::WebKit::Char c, EA::WebKit::Script scriptInternal)
{
    EAW_ASSERT(mpFont);
    bool returnFlag = false;
    if(mpFont)
    {
        EA::Text::Script scriptExternal = GetExternal_Script(scriptInternal);
        EA::Text::Char cExternal = GetExternal_Char(c);
        returnFlag = mpFont->IsCharSupported( cExternal, scriptExternal);
    }
    return returnFlag;
}

float FontProxy::GetSize() const
{
    EAW_ASSERT(mpFont);
    float returnValue = 0.0f;
    if(mpFont) 
        returnValue =  mpFont->GetSize();

    return returnValue;
}

bool FontProxy::GetKerning(EA::WebKit::GlyphId g1, EA::WebKit::GlyphId g2, EA::WebKit::Kerning& kerningInternal, int direction, bool bHorizontalLayout)
{
    EAW_ASSERT(mpFont);
    bool returnFlag = false;
    if(mpFont)
    {
        EA::Text::Kerning kerningExternal;
        
        EA::Text::GlyphId g1External= GetExternal_GlyphId(g1);
        EA::Text::GlyphId g2External= GetExternal_GlyphId(g2);
        
        returnFlag = mpFont->GetKerning(g1External, g2External, kerningExternal, direction, bHorizontalLayout);
        ConvertToInternal_Kerning(kerningExternal, kerningInternal);
    }
    return returnFlag;
}

// This is special for the down casted outline font class
bool FontProxy::OpenOutline(const void* pSourceData, uint32_t nSourceSize, int nFaceIndex)
{
    EAW_ASSERT(mpFont);
    bool returnFlag = false;
    if(mpFont)
    {    
        EA::Text::OutlineFont* pOutlineFont = static_cast<EA::Text::OutlineFont*> (mpFont);                  
        returnFlag = pOutlineFont->Open(pSourceData, nSourceSize, nFaceIndex);
    }
    return returnFlag;
}

bool FontProxy::DrawGlyphBitmap(EA::WebKit::IGlyphCache* pGlyphCache, EA::WebKit::GlyphId gInternal, EA::WebKit::IGlyphTextureInfo& glyphTextureInfoInternal)
{
    EAW_ASSERT(mpFont);

    bool returnFlag = true;

    EA::Text::GlyphId gIdExternal = (EA::Text::GlyphId) gInternal;
    EA::Text::GlyphTextureInfo glyphTextureInfoExternal;
    
    ConvertToExternal_GlyphTextureInfo(glyphTextureInfoInternal, glyphTextureInfoExternal);

    const EA::Text::Font::GlyphBitmap* pGlyphBitmap;

    if(mpFont->RenderGlyphBitmap(&pGlyphBitmap, gIdExternal))
    {
        EA::Text::GlyphCache* pGlyphCacheExternal = static_cast<EA::Text::GlyphCache*> (pGlyphCache->GetGlyphCachePointer());
        EAW_ASSERT(pGlyphCacheExternal);
        if(pGlyphCacheExternal->AddGlyphTexture(mpFont, gIdExternal, pGlyphBitmap->mpData, pGlyphBitmap->mnWidth, pGlyphBitmap->mnHeight, 
                                               pGlyphBitmap->mnStride, (uint32_t)pGlyphBitmap->mBitmapFormat, glyphTextureInfoExternal))

        {
            pGlyphCacheExternal->EndUpdate(glyphTextureInfoExternal.mpTextureInfo);
        }
        else
        {
            EAW_ASSERT_MSG(false, "TextWrapper: drawGlyphs: AddGlyphTexture failed.");
            glyphTextureInfoExternal.mX1 = glyphTextureInfoExternal.mX2 = 0; // Make this an empty glyph. Normally this should never execute.
            returnFlag = false;
        }

        mpFont->DoneGlyphBitmap(pGlyphBitmap);
    } 
    else
    {
        EAW_ASSERT_MSG(false, "TextWrapper: drawGlyphs: invalid glyph/Font combo.");
        glyphTextureInfoExternal.mX1 = glyphTextureInfoExternal.mX2 = 0; // Make this an empty glyph. Normally this should never execute.
        returnFlag = false;
    }
    ConvertToInternal_GlyphTextureInfo(glyphTextureInfoExternal,glyphTextureInfoInternal);    

    return returnFlag;
}

void FontProxy::DestroyWrapper()
{
    if(!mpFont)
         return;

     mpFont = 0;
     Allocator::ICoreAllocator* pAllocator = GetAllocator_Helper();                
     EATEXT_WRAPPER_DELETE(this, pAllocator);     
}

FontProxy::FontProxy()
    :mpFont(0),
    mRefCount(1)
{
} 

FontProxy::FontProxy(EA::Text::Font* pFont)
    :mpFont(pFont),
    mRefCount(1)
{
} 

FontProxy::~FontProxy()
{
    if(mpFont)
    {                
        Allocator::ICoreAllocator* pAllocator = GetAllocator_Helper();
        EATEXT_WRAPPER_DELETE(mpFont, pAllocator);            
        mpFont =0;
    }
}

void FontProxy::SetFontPointer(void* pFont)
{
    mpFont = static_cast<EA::Text::Font*> (pFont);
}

void* FontProxy::GetFontPointer() 
{
    return mpFont;
}

// --- Font Style Wrappers ---
EA::WebKit::Char* FontStyleProxy::GetFamilyNameArray(int index)
{
    EAW_ASSERT(mpFontStyle); 
    return  (EA::WebKit::Char*) &mpFontStyle->mFamilyNameArray[index];
}    
void FontStyleProxy::SetSize(float value)
{
    EAW_ASSERT(mpFontStyle);
    if(mpFontStyle)
        mpFontStyle->mfSize = value;
}

void FontStyleProxy::SetStyle(EA::WebKit::Style value)
{
    EAW_ASSERT(mpFontStyle);
    if(mpFontStyle)
        mpFontStyle->mStyle = GetExternal_Style(value);
}

void FontStyleProxy::SetWeight(float value)
{
    EAW_ASSERT(mpFontStyle);
    if(mpFontStyle)
        mpFontStyle->mfWeight = value;
}

void FontStyleProxy::SetVariant(EA::WebKit::Variant value)
{
    EAW_ASSERT(mpFontStyle);
    if(mpFontStyle)
        mpFontStyle->mVariant = GetExternal_Variant(value);
}

void FontStyleProxy::SetSmooth(EA::WebKit::Smooth value)
{
    EAW_ASSERT(mpFontStyle);
    if(mpFontStyle)   
        mpFontStyle->mSmooth = GetExternal_Smooth(value);
}

void FontStyleProxy::SetFontStyle(void* pFontStyle)
{
    mpFontStyle = static_cast<EA::Text::FontStyle*>(pFontStyle);
}           

void* FontStyleProxy::GetFontStylePointer()
{
    return mpFontStyle;
}

void FontStyleProxy::Destroy()
{
     if(!mpFontStyle)
         return;
     
     Allocator::ICoreAllocator* pAllocator = GetAllocator_Helper();                
     EATEXT_WRAPPER_DELETE(mpFontStyle, pAllocator);     
     mpFontStyle = 0;
     EATEXT_WRAPPER_DELETE(this, pAllocator);     
}

//--- Font Server Wrappers ---
EA::WebKit::IFont* FontServerProxy::CreateNewFont(EA::WebKit::FontType fontTypeInternal)
{
    EA::WebKit::IFont* pIFont = NULL;
    
    EAW_ASSERT(mpFontServer);
    if(mpFontServer)
    {   
        EA::Text::FontType fontTypeExternal = GetExternal_FontType(fontTypeInternal);

        EA::Text::Font* pFont = mpFontServer->CreateNewFont(fontTypeExternal);
        pIFont = CreateFontProxyWrapper(pFont);
    }
    return pIFont;
}

EA::WebKit::IFont* FontServerProxy::GetFont(EA::WebKit::IFontStyle* pTextStyle, EA::WebKit::IFont* pFontArray[], uint32_t nFontArrayCapacity, 
                                            const EA::WebKit::TextEffectData* pEffect, EA::WebKit::Char c, EA::WebKit::Script scriptInternal, bool bManaged)
{
    EA::WebKit::IFont* pIFont = NULL;
    EAW_ASSERT(mpFontServer);
    if(mpFontServer)
    {
        EAW_ASSERT(!pFontArray); // FontArray is always NULL in EAWebKit
        EA::Text::TextStyle* pFontStyle = static_cast<EA::Text::TextStyle*> (pTextStyle->GetFontStylePointer());
        EA::Text::Script scriptExternal = GetExternal_Script(scriptInternal);
        EA::Text::Char cExternal = GetExternal_Char(c);
        
        // Pass down the effect settings to EA Text 
        if((pEffect) && (pEffect->type != EA::WebKit::kEffectNone))
        {
           
            // Note that if the glyph cache is EA::Text::kTextureFormat8Bpp, we don't want to call effects as we need 32bit
            pFontStyle->mEffect = GetExternal_Effect(pEffect->type);
            pFontStyle->mfEffectX = static_cast <float> (pEffect->x); // EAWebkit stores these as ints so we need to convert.
            pFontStyle->mfEffectY = static_cast <float> (pEffect->y);
            pFontStyle->mEffectColor = GetExternalColor(pEffect->c);
            pFontStyle->mEffectBaseColor = GetExternalColor(pEffect->cBase);
            
        }

        
        EA::Text::Font* pFont = mpFontServer->GetFont(pFontStyle, NULL, nFontArrayCapacity, cExternal, scriptExternal, bManaged);
        pIFont = CreateFontProxyWrapper(pFont);
    }
    return  pIFont; 
}

uint32_t FontServerProxy::EnumerateFonts(EA::WebKit::IFontDescription* pFontDescriptionArrayInternal, uint32_t nCount)
{
    int returnValue =0;
    
    EAW_ASSERT(mpFontServer);
    if(mpFontServer)
    {
        EA::Text::FontDescription* pFontDescriptionArrayExternal = NULL;

        // This seems rarely called so just memory allocatate versus stack tricks as in the EAWebKit call.   
        if(nCount > 0)
            pFontDescriptionArrayExternal = new(WebKit::GetAllocator()->Malloc( (sizeof(EA::Text::FontDescription) * nCount), 0, "Util: Font Description Array")) EA::Text::FontDescription[nCount];
       
        returnValue = mpFontServer->EnumerateFonts( pFontDescriptionArrayExternal, nCount);

        if(pFontDescriptionArrayExternal)
        {
            for(uint32_t i=0; i < nCount; i++)
            {
                ConvertToInternal_FontDescription(pFontDescriptionArrayExternal[i],pFontDescriptionArrayInternal[i]); // Copy to internal EAWebKit version               
                pFontDescriptionArrayExternal[i].~FontDescription();   // Call destructor (none currently) in same loop to save a loop as no longer needed.
            }
            WebKit::GetAllocator()->Free(pFontDescriptionArrayExternal, (sizeof(EA::Text::FontDescription) * nCount));
        }
    }
    return returnValue;
}

// Access to some general unicode interface functions
int32_t FontServerProxy::GetCombiningClass(EA::WebKit::Char c)
{
    EA::Text::Char cExternal = GetExternal_Char(c);
    
    return EA::Text::GetCombiningClass(cExternal);            
}

EA::WebKit::Char FontServerProxy::GetMirrorChar(EA::WebKit::Char c)
{
    EA::Text::Char cExternal = GetExternal_Char(c);
    
    return GetInternal_Char(EA::Text::GetMirrorChar(cExternal));                 
}

EA::WebKit::CharCategory FontServerProxy::GetCharCategory(EA::WebKit::Char c)
{
    EA::Text::Char cExternal= GetExternal_Char(c);
    EA::Text::CharCategory catExternal = EA::Text::GetCharCategory(cExternal);            
    return GetInternal_CharCategory(catExternal);
}

EA::WebKit::BidiClass FontServerProxy::GetBidiClass(EA::WebKit::Char c)
{
    EA::Text::Char cExternal= GetExternal_Char(c);
    EA::Text::BidiClass bidiExternal = EA::Text::GetBidiClass(cExternal);
    return GetInternal_BidiClass(bidiExternal);
}

uint32_t FontServerProxy::GetFamilyNameArrayCapacity()
{
    return EATEXT_FAMILY_NAME_ARRAY_CAPACITY;    
}

uint32_t FontServerProxy::GetFamilyNameCapacity()
{
    return EA::Text::kFamilyNameCapacity;    
}

FontServerProxy::FontServerProxy(EA::Text::FontServer* pServer)
   :mpFontServer(pServer)
   ,mbOwnFontServer(false)
{
    if(!pServer)
    {
        // Create our own font server 
    	mbOwnFontServer = true;
		
        mpFontServer = new(WebKit::GetAllocator()->Malloc(sizeof(EA::Text::FontServer), 0, "Util Font Server Proxy")) EA::Text::FontServer();
		EA::Text::SetFontServer(mpFontServer);
       
        // New to set up the default memory allocator for the font system
        // EA::Text::SetAllocator(pCoreAllocator);
        EA::Text::SetAllocator(Allocator::ICoreAllocator::GetDefaultAllocator());

        EAW_ASSERT_MSG(spGlyphCacheWrapperInterface,"TextWrapper: Please call CreateGlyphCacheWrapperInterface first and set the default GlyphCache");
        EAW_ASSERT_MSG(spGlyphCacheWrapperInterface->mpGlyphCache,"TextWrapper: Please call CreateGlyphCacheWrapperInterface first and set the default GlyphCache");
		mpFontServer->SetDefaultGlyphCache(spGlyphCacheWrapperInterface->mpGlyphCache); // Might not be needed if set already by app
    }
     
     // Since we have some locally exposed const and struct, need to verify they are in sync with EAText
    // If we get an assert here, just need to update the values in EAWebkitTextInterface.h to sync up but that could be a problem
    // for older versions of EAText 
    
    EA_COMPILETIME_ASSERT( sizeof(EA::Text::Char) == sizeof(EA::WebKit::Char));
    EA_COMPILETIME_ASSERT( sizeof(EA::Text::GlyphId) == sizeof(EA::WebKit::GlyphId));
    EA_COMPILETIME_ASSERT( sizeof(EA::Text::Color) == sizeof(EA::WebKit::Color));

    EA_COMPILETIME_ASSERT( static_cast<int> (EA::Text::kCharInvalid) == static_cast<int> (EA::WebKit::kCharInvalid));
    EA_COMPILETIME_ASSERT( static_cast<int> (EA::Text::kGlyphIdInvalid) == static_cast<int> (EA::WebKit::kGlyphIdInvalid));
} 

FontServerProxy::~FontServerProxy()
{
	if(mbOwnFontServer)
	{
		// We need to remove the font server since it was created inside EAWebkit
        mpFontServer->Shutdown();
        mpFontServer->~FontServer();

        WebKit::GetAllocator()->Free(mpFontServer, sizeof(EA::Text::FontServer));
		mpFontServer = NULL;
                    
        EA::Text::SetFontServer(0);   
        mbOwnFontServer = false;
    }
}

EA::WebKit::IFontStyle* FontServerProxy::CreateTextStyle()
{
    Allocator::ICoreAllocator* pAllocator = GetAllocator_Helper();                
    EA::Text::FontStyle* pFontStyle=EATEXT_WRAPPER_NEW(EA::Text::TextStyle, pAllocator, "TextWrapper: TextStyle");
    if(!pFontStyle)
        return 0;

    FontStyleProxy* pFontStyleProxy = EATEXT_WRAPPER_NEW(FontStyleProxy, pAllocator, "TextWrapper: FontStyleProxy");
    if(pFontStyleProxy)
        pFontStyleProxy->SetFontStyle(pFontStyle);
    else
        EATEXT_WRAPPER_DELETE(pFontStyle, pAllocator);    

    return pFontStyleProxy;
}

// FontServer Proxy Class
FontProxy* FontServerProxy::CreateFontProxyWrapper(EA::Text::Font* pFont)
{ 
   if(!pFont)
       return 0;     

   Allocator::ICoreAllocator* pAllocator = GetAllocator_Helper();                
   FontProxy* pFontProxy =EATEXT_WRAPPER_NEW(FontProxy, pAllocator, "TextWrapper: FontProxy");
   if(pFontProxy)
        pFontProxy->SetFontPointer(pFont);
    else
    {
        EAW_ASSERT(0);                
    }
    return pFontProxy;
}

uint32_t FontServerProxy::AddFace(IO::IStream* pStream, EA::WebKit::FontType fontTypeInternal)
{
    uint32_t returnValue =0;
    EAW_ASSERT(mpFontServer);   
    if(mpFontServer)    
    {
        EA::Text::FontType fontTypeExternal = GetExternal_FontType(fontTypeInternal);
        returnValue = mpFontServer->AddFace(pStream, fontTypeExternal);
    }
    return returnValue;
}

uint32_t FontServerProxy::AddDirectory(const char16_t* pFaceDirectory, const char16_t* pFilter)
{
	uint32_t returnValue =0;
    EAW_ASSERT(mpFontServer);   
    if(mpFontServer)    
	    returnValue =  mpFontServer->AddDirectory(pFaceDirectory, pFilter);

    return returnValue;
}

bool FontServerProxy::AddSubstitution(const char16_t* pFamily, const char16_t* pFamilySubstitution)
{
    bool returnFlag = false;

#if EATEXT_FAMILY_SUBSTITUTION_ENABLED
    EAW_ASSERT(mpFontServer);   
    if(mpFontServer)    
	    returnFlag = mpFontServer->AddSubstitution(pFamily,pFamilySubstitution);
#endif
	return returnFlag;
}

// Text break iterator statics 
static EA::Text::CharacterBreakIterator sCharacterBreakIterator;
static EA::Text::WordBreakIterator      sWordBreakIterator;
static EA::Text::LineBreakIterator      sLineBreakIterator;
static EA::Text::SentenceBreakIterator  sSentenceBreakIterator;
static EA::Text::TextRun                sTextRun;
static EA::Text::BreakIteratorBase*     sTextBreakIterator;
const int TextBreakDone = -1;

void* FontServerProxy::CharacterBreakIterator(EA::WebKit::Char* pText, int length)
{
   
    // To do: Have EAText iterators keep a member TextRun so you don't need to maintain one externally.
    sTextRun.mpText     = (EA::Text::Char*) pText;
    sTextRun.mnTextSize = (uint32_t)length;
    sCharacterBreakIterator.GetIterator().SetTextRunArray(&sTextRun, 1);
    sTextBreakIterator = &sCharacterBreakIterator;

    return sTextBreakIterator;
}

void* FontServerProxy::WordBreakIterator(EA::WebKit::Char* pText, int length)
{
    // To do: Have EAText iterators keep a member TextRun so you don't need to maintain one externally.
    sTextRun.mpText     = (EA::Text::Char*) pText;
    sTextRun.mnTextSize = (uint32_t)length;
    sWordBreakIterator.GetIterator().SetTextRunArray(&sTextRun, 1);
    sTextBreakIterator = &sWordBreakIterator;

    return sTextBreakIterator;
}

void* FontServerProxy::LineBreakIterator(EA::WebKit::Char* pText, int length)
{
    // To do: Have EAText iterators keep a member TextRun so you don't need to maintain one externally.
    sTextRun.mpText     = (EA::Text::Char*) pText;
    sTextRun.mnTextSize = (uint32_t)length;
    sLineBreakIterator.GetIterator().SetTextRunArray(&sTextRun, 1);
    sTextBreakIterator = &sLineBreakIterator;

    return sTextBreakIterator;
}

void* FontServerProxy::SentenceBreakIterator(EA::WebKit::Char* pText, int length)
{
    // To do: Have EAText iterators keep a member TextRun so you don't need to maintain one externally.
    sTextRun.mpText     = (EA::Text::Char*) pText;
    sTextRun.mnTextSize = (uint32_t)length;
    sSentenceBreakIterator.GetIterator().SetTextRunArray(&sTextRun, 1);
    sTextBreakIterator = &sSentenceBreakIterator;

    return sTextBreakIterator;
}

int FontServerProxy::TextBreakFirst(void* pIter)
{
        EA::Text::BreakIteratorBase* pIterator = static_cast<EA::Text::BreakIteratorBase*> (pIter);
    
    // We have a problem: EATextBreak doesn't implement a pure-virtual NextBreak/PrevBreak function.
    // It's easy to do this but it requires us to update EAText. We do something of a hack in the 
    // short run to allow this to work now by checking each of the global variables manually.
    pIterator->SetPosition(0);
    return TextBreakNext(pIterator);
}

int FontServerProxy::TextBreakNext(void* pIter)
{
    uint32_t originalPosition;
    uint32_t newPosition;
    EA::Text::BreakIteratorBase* pIterator = static_cast<EA::Text::BreakIteratorBase*> (pIter);

    if(pIterator == &sCharacterBreakIterator)
    {
        originalPosition = sCharacterBreakIterator.GetPosition();
        newPosition      = sCharacterBreakIterator.GetNextCharBreak();
    }
    else if(pIterator == &sWordBreakIterator)
    {
        originalPosition = sWordBreakIterator.GetPosition();
        newPosition      = sWordBreakIterator.GetNextWordBreak();
    }
    else if(pIterator == &sSentenceBreakIterator)
    {
        originalPosition = sLineBreakIterator.GetPosition();
        newPosition      = sLineBreakIterator.GetNextLineBreak();
    }
    else // if(pIterator == &sSentenceBreakIterator)
    {
        originalPosition = sSentenceBreakIterator.GetPosition();
        newPosition      = sSentenceBreakIterator.GetNextSentenceBreak();
    }

    if(pIterator->AtEnd() && (newPosition == originalPosition))
        return TextBreakDone;

    return static_cast<int> (newPosition);
}

int FontServerProxy::TextBreakCurrent(void* pIter)
{
    EA::Text::BreakIteratorBase* pIterator = static_cast<EA::Text::BreakIteratorBase*> (pIter);

    return static_cast<int> (pIterator->GetPosition());
}

int FontServerProxy::TextBreakPreceding(void* pIter, int position)
{
    uint32_t originalPosition;
    uint32_t newPosition;
    EA::Text::BreakIteratorBase* pIterator = static_cast<EA::Text::BreakIteratorBase*> (pIter);

    pIterator->SetPosition((uint32_t)position);

    if(pIterator == &sCharacterBreakIterator)
    {
        originalPosition = sCharacterBreakIterator.GetPosition();
        newPosition      = sCharacterBreakIterator.GetPrevCharBreak();
    }
    else if(pIterator == &sWordBreakIterator)
    {
        originalPosition = sWordBreakIterator.GetPosition();
        newPosition      = sWordBreakIterator.GetPrevWordBreak();
    }
    else if(pIterator == &sSentenceBreakIterator)
    {
        originalPosition = sLineBreakIterator.GetPosition();
        newPosition      = sLineBreakIterator.GetPrevLineBreak();
    }
    else // if(pIterator == &sSentenceBreakIterator)
    {
        originalPosition = sSentenceBreakIterator.GetPosition();
        newPosition      = sSentenceBreakIterator.GetPrevSentenceBreak();
    }

    if(pIterator->AtBegin() && (newPosition == originalPosition))
        return TextBreakDone;

    return static_cast<int> (newPosition);
}

int FontServerProxy::TextBreakFollowing(void* pIter, int position)
{
    EA::Text::BreakIteratorBase* pIterator = static_cast<EA::Text::BreakIteratorBase*> (pIter);

    pIterator->SetPosition((uint32_t)position);
    return TextBreakNext(pIterator);
}

bool FontServerProxy::IsTextBreak(void* pIter, int position)
{
    EA::Text::BreakIteratorBase* pIterator = static_cast<EA::Text::BreakIteratorBase*> (pIter);

    // If we are being asked about the most recent break position, then we know it is a break position.
    // Otherwise the only means we have to implementing this is to walk through the entire string
    // and check each break against position.
    if(position == static_cast<int> (pIterator->GetPosition()))
        return true;

    pIterator->SetPosition(0);

    while((static_cast<int> (pIterator->GetPosition()) < position) && !pIterator->AtEnd())
        TextBreakNext(pIterator);

    return (position == static_cast<int> (pIterator->GetPosition()));
}

int FontServerProxy::FindNextWordFromIndex(EA::WebKit::Char* chars, int len, int position, bool forward)
{
    /// The arrows below show the positions that would be returned by 
    /// successive calls to GetNextWordBreak with the default wordBreakType.
    ///    "Hello   world   hello   world"
    ///     ^    ^  ^    ^  ^    ^  ^    ^

    EA::Text::TextRun           tr((EA::Text::Char*)chars, (uint32_t)len);
    EA::Text::WordBreakIterator wbi;

    wbi.SetIterator(EA::Text::TextRunIterator(&tr));
    wbi.SetPosition((uint32_t)position);

    if(forward)
        return static_cast<int> (wbi.GetNextWordBreak(EA::Text::kWordBreakTypeBegin));    // It turns out that as of this writing EAText doesn't implement kWordBreakTypeBegin/End. We will need to make sure that's working so this code can work as expected.
    else
        return static_cast<int> (wbi.GetPrevWordBreak(EA::Text::kWordBreakTypeBegin));
}

void FontServerProxy::FindWordBoundary(EA::WebKit::Char* chars, int len, int position, int* start, int* end)
{
    /// The arrows below show the positions that would be returned by 
    /// successive calls to GetNextWordBreak with the default wordBreakType.
    ///    "Hello   world   hello   world"
    ///     ^    ^  ^    ^  ^    ^  ^    ^

    EA::Text::TextRun           tr( static_cast<EA::Text::Char*> (chars), static_cast<uint32_t> (len));
    EA::Text::WordBreakIterator wbi;

    wbi.SetIterator(EA::Text::TextRunIterator(&tr));
    wbi.SetPosition( static_cast<uint32_t> (position));

    *start = static_cast<int> (wbi.GetPrevWordBreak(EA::Text::kWordBreakTypeBegin));

    wbi.SetPosition(static_cast<uint32_t> (*start));

    // WebKit may expect that 'end' refer to the last char and not one-past it.
    // Also, it may expect that end not refer to the end of the space after word and instead mean the very end of the word chars.
    *end = static_cast<int> (wbi.GetNextWordBreak(EA::Text::kWordBreakTypeEnd) - 1);
}

int TextureInfoProxy::AddRef()
{
    EAW_ASSERT(mpInfo);
    return mpInfo->AddRef();
}

int TextureInfoProxy::Release()
{
    EAW_ASSERT(mpInfo);
    return mpInfo->Release();
}

uint32_t TextureInfoProxy::GetSize()
{
    EAW_ASSERT(mpInfo);
    return mpInfo->mnSize;
}

intptr_t TextureInfoProxy::GetStride()
{
    EAW_ASSERT(mpInfo);
    return mpInfo->mnStride;
}

uint8_t* TextureInfoProxy::GetData()
{
    EAW_ASSERT(mpInfo);
    return mpInfo->mpData;
}

uint32_t TextureInfoProxy::GetFormat()
{
    EAW_ASSERT(mpInfo);
    return mpInfo->mFormat;
}

void TextureInfoProxy::DestroyWrapper()
{
    if(!mpInfo)
     return;

    mpInfo = NULL;
    Allocator::ICoreAllocator* pAllocator = GetAllocator_Helper();                
    EATEXT_WRAPPER_DELETE(this, pAllocator);     
}

void TextureInfoProxy::SetTextureInfoPointer(void* pInfo)
{
    mpInfo = static_cast<EA::Text::TextureInfo*> (pInfo);
}

void* TextureInfoProxy::GetTextureInfoPointer() 
{
    return mpInfo;
}

EA::WebKit::ITextureInfo* GlyphCacheProxy::GetTextureInfo(uint32_t nTextureIndex)
{
    if(!mpGlyphCache)
    {
        EAW_ASSERT(mpGlyphCache);
        return NULL;
    }    
    
    EA::Text::TextureInfo* pInfo = mpGlyphCache->GetTextureInfo(nTextureIndex);
    EAW_ASSERT_MSG(pInfo, "TextWrapper: Please make sure that you have initialized GlyphCache so that nTextureIndex falls inside the number of textures that GlyphCache has.");
	if(!pInfo) 
        return NULL;

    // Allocate proxy
    return CreateTextureInfoProxy(pInfo);
}

bool GlyphCacheProxy::GetGlyphTextureInfo(EA::WebKit::IFont* pFont, EA::WebKit::GlyphId glyphId,EA::WebKit::IGlyphTextureInfo& glyphTextureInfo) 
{
    EA::Text::Font* pFontEA =  static_cast<EA::Text::Font*> (pFont->GetFontPointer());
    EA::Text::GlyphTextureInfo  glyphTextureInfoEA;

    bool returnFlag = false;
    EAW_ASSERT(mpGlyphCache);        
    
    EA::Text::GlyphId glyphIdEA = GetExternal_GlyphId(glyphId);

    if(mpGlyphCache)
        returnFlag = mpGlyphCache->GetGlyphTextureInfo(pFontEA, glyphIdEA, glyphTextureInfoEA);

    if(returnFlag)
        ConvertToInternal_GlyphTextureInfo(glyphTextureInfoEA, glyphTextureInfo);
    
    return returnFlag;
}

bool GlyphCacheProxy::AddGlyphTexture(EA::WebKit::IFont* pFont, EA::WebKit::GlyphId glyphId, const void* pSourceData, uint32_t nSourceSizeX, 
                             uint32_t nSourceSizeY, uint32_t nSourceStride, uint32_t nSourceFormat,
                             EA::WebKit::IGlyphTextureInfo& glyphTextureInfo)
{
    EA::Text::Font* pFontEA =  static_cast<EA::Text::Font*>(pFont->GetFontPointer());
    EA::Text::GlyphTextureInfo  glyphTextureInfoEA;
   
    bool returnFlag = false;
    EAW_ASSERT(mpGlyphCache);        
    EA::Text::GlyphId glyphIdExternal = GetExternal_GlyphId(glyphId);
    
    if(mpGlyphCache)
        returnFlag = mpGlyphCache->AddGlyphTexture(pFontEA, glyphIdExternal, pSourceData, nSourceSizeX, 
                             nSourceSizeY, nSourceStride, nSourceFormat, glyphTextureInfoEA);
    if(returnFlag)
    {
        ConvertToInternal_GlyphTextureInfo(glyphTextureInfoEA, glyphTextureInfo);
    }
    return returnFlag;            
}

bool GlyphCacheProxy::EndUpdate(EA::WebKit::ITextureInfo* pTextureInfo)
{
    EA::Text::TextureInfo* pTextureInfoEA = (EA::Text::TextureInfo*) pTextureInfo->GetTextureInfoPointer();
    
    bool returnFlag = false;
    EAW_ASSERT(mpGlyphCache);        
    if(mpGlyphCache)
    {
        mpGlyphCache->EndUpdate(pTextureInfoEA);
        returnFlag = true;
    }
    return returnFlag;
}

TextureInfoProxy* GlyphCacheProxy::CreateTextureInfoProxy(EA::Text::TextureInfo* pInfo)
{
    Allocator::ICoreAllocator* pAllocator = GetAllocator_Helper();                
    TextureInfoProxy* pTextureInfoProxy =EATEXT_WRAPPER_NEW(TextureInfoProxy, pAllocator, "TextWrapper: TextureInfo Proxy");
    if(pTextureInfoProxy)
        pTextureInfoProxy->SetTextureInfoPointer(pInfo);
    return pTextureInfoProxy;
}

GlyphCacheProxy::GlyphCacheProxy(EA::Text::GlyphCache* pGlypheCache)
   : mpGlyphCache(pGlypheCache)
   , mbOwnGlyphCache(false)
{
	if(!mpGlyphCache) //If Glyph Cache was not passed to us, we will create ours.
	{
        EAW_ASSERT_MSG(!spGlyphCacheWrapperInterface, "TextWrapper: Cache wrapper interface is alreday created!");
	          
        InitFontSystem();
		
        mbOwnGlyphCache = true;
		
        mpGlyphCache = new(WebKit::GetAllocator()->Malloc(sizeof(EA::Text::GlyphCache_Memory), 0, "TextWrapper: Glyph Proxy")) EA::Text::GlyphCache_Memory(EA::Text::kTextureFormat8Bpp);
        EAW_ASSERT(mpGlyphCache);
        
		mpGlyphCache->SetAllocator(EA::Text::GetAllocator());
		mpGlyphCache->SetOption(EA::Text::GlyphCache::kOptionDefaultSize, 2048);
		mpGlyphCache->SetOption(EA::Text::GlyphCache::kOptionDefaultFormat, EA::Text::kTextureFormat8Bpp);
		const int result = mpGlyphCache->Init(2, 1); // (nMaxTextureCount = 1, nInitialTextureCount = 1)
		EAW_ASSERT(result == 1); 
		(void)result;

     }
    spGlyphCacheWrapperInterface = this;
} 

GlyphCacheProxy::~GlyphCacheProxy()
{
	if(mbOwnGlyphCache)
	{
		// We need to remove the GlypyCache since it was created inside EAWebKit.
        mpGlyphCache->Shutdown();
        mpGlyphCache->~GlyphCache();
        WebKit::GetAllocator()->Free(mpGlyphCache, sizeof(EA::Text::GlyphCache_Memory));
        mpGlyphCache = NULL;
        mbOwnGlyphCache = false;
    }
  	spGlyphCacheWrapperInterface = NULL;
}

    } // Namespace TextWrapper
} // Namespace EA

