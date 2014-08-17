/*
Copyright (C) 2004,2009-2010 Electronic Arts, Inc.  All rights reserved.

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
// EATextStyle.cpp
//
// By Paul Pedriana - 2004
//
///////////////////////////////////////////////////////////////////////////////


#include <EAText/EAText.h>
#include <EAText/EATextStyle.h>
#include <EAText/internal/StdC.h>
#include <coreallocator/icoreallocator_interface.h>
#include <coreallocator/icoreallocatormacros.h>
#include EA_ASSERT_HEADER
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifdef _MSC_VER // Enable highest warning for this file alone under MSVC.
   #pragma warning(push, 4)
#endif



namespace EA
{

namespace Text
{


#if EATEXT_STYLE_MANAGER_ENABLED

    FontStyle::FontStyle()
      : mfSize(12.f), mStyle(kStyleDefault), mfWeight(kWeightDefault), 
        mVariant(kVariantDefault), mPitch(kPitchDefault), mSmooth(kSmoothDefault),
        mEffect(kEffectNone), mfEffectX(1.f), mfEffectY(1.f)
    {
        for(uint32_t i = 0; i < kFamilyNameArrayCapacity; i++)
            mFamilyNameArray[i][0] = 0; 
    }


    FontStyle::FontStyle(const FontStyle& fs)
    {
        operator=(fs);
    }

    FontStyle& FontStyle::operator=(const FontStyle& fs)
    {
        mfSize      = fs.mfSize;
        mStyle      = fs.mStyle;
        mfWeight    = fs.mfWeight; 
        mVariant    = fs.mVariant;
        mPitch      = fs.mPitch;
        mSmooth     = fs.mSmooth;
        mEffect     = fs.mEffect;
        mfEffectX   = fs.mfEffectX;
        mfEffectY   = fs.mfEffectY;

        for(uint32_t i = 0; i < kFamilyNameArrayCapacity; i++)
            Strlcpy(mFamilyNameArray[i], fs.mFamilyNameArray[i], kFamilyNameCapacity); 

        return *this;
    }


    bool operator==(const FontStyle& a, const FontStyle& b)
    {
        // Compare built-in types.
        if((a.mfSize     == b.mfSize)    &&
           (a.mStyle     == b.mStyle)    &&
           (a.mfWeight   == b.mfWeight)  &&
           (a.mVariant   == b.mVariant)  &&
           (a.mPitch     == b.mPitch)    &&
           (a.mSmooth    == b.mSmooth)   &&
           (a.mEffect    == b.mEffect)   &&
           (a.mfEffectX  == b.mfEffectX) &&
           (a.mfEffectY  == b.mfEffectY))
        {
            // Compare family name.
            for(uint32_t i = 0; i < kFamilyNameArrayCapacity; i++)
            {
                if(Strcmp(a.mFamilyNameArray[i], b.mFamilyNameArray[i]) != 0)
                    return false;
            }

            return true;
        }

        return false;
    }


    ///////////////////////////////////////////////////////////////////////////
    // TextStyle
    ///////////////////////////////////////////////////////////////////////////

    TextStyle::TextStyle()
      : mColor(0xffffffff), mColorBackground(0x00000000), mEffectBaseColor(0xffffffff), mEffectColor(0xffffffff), 
        mHighLightColor(0x00000000), mnDecorationFlags(kDFDefault), mDirection(kDirectionDefault), 
        mOrientation(kOrientationDefault), mHAlignment(kHADefault), mVAlignment(kVADefault), 
        mJustify(kJustifyDefault), mTextWrap(kTWDefault), mTextOverflow(kTODefault), 
        mEmphasisStyle(kESDefault), mEmphasisPosition(kEPDefault), mDigitSubstitution(kDSDefault), 
        mPasswordMode(kPMDefault), mfStretch(1.f), mfLetterSpacing(0.f), mfWordSpacing(0.f), 
        mfLineSpace(0.f)
    {
    }

    TextStyle& TextStyle::operator=(const TextStyle& ts)
    {
        memcpy(this, &ts, sizeof(TextStyle));

        // Explicit member-by-member init, required if this class is not a POD.
        //FontStyle::operator=(ts);
        //
        //mColor             = b.mColor;
        //mColorBackground   = b.mColorBackground;
        //mnDecorationFlags  = b.mnDecorationFlags;
        //mDirection         = b.mDirection;
        //mOrientation       = b.mOrientation;
        //mHAlignment        = b.mHAlignment;
        //mVAlignment        = b.mVAlignment;
        //mJustify           = b.mJustify;
        //mTextWrap          = b.mTextWrap;
        //mTextOverflow      = b.mTextOverflow;
        //mEmphasisStyle     = b.mEmphasisStyle;
        //mEmphasisPosition  = b.mEmphasisPosition;
        //mDigitSubstitution = b.mDigitSubstitution;
        //mPasswordMode      = b.mPasswordMode;
        //mfStretch          = b.mfStretch;
        //mfLetterSpacing    = b.mfLetterSpacing;
        //mfWordSpacing      = b.mfWordSpacing;
        //mfLineSpace        = b.mfLineSpace;

        return *this;
    }


    bool operator==(const TextStyle& a, const TextStyle& b)
    {
        if(operator==((FontStyle&)a, (const FontStyle&)b)) // If the base class compares as equal...
        {
            return ((a.mColor             == b.mColor) &&
                    (a.mColorBackground   == b.mColorBackground) &&
                    (a.mnDecorationFlags  == b.mnDecorationFlags) &&
                    (a.mDirection         == b.mDirection) &&
                    (a.mOrientation       == b.mOrientation) &&
                    (a.mHAlignment        == b.mHAlignment) &&
                    (a.mVAlignment        == b.mVAlignment) &&
                    (a.mJustify           == b.mJustify) &&
                    (a.mTextWrap          == b.mTextWrap) &&
                    (a.mTextOverflow      == b.mTextOverflow) &&
                    (a.mEmphasisStyle     == b.mEmphasisStyle) &&
                    (a.mEmphasisPosition  == b.mEmphasisPosition) &&
                    (a.mDigitSubstitution == b.mDigitSubstitution) &&
                    (a.mPasswordMode      == b.mPasswordMode) &&
                    (a.mfStretch          == b.mfStretch) &&
                    (a.mfLetterSpacing    == b.mfLetterSpacing) &&
                    (a.mfWordSpacing      == b.mfWordSpacing) &&
                    (a.mfLineSpace        == b.mfLineSpace) &&
                    (a.mEffectColor       == b.mEffectColor) &&
                    (a.mHighLightColor    == b.mHighLightColor));
        }

        return false;
    }

    ///////////////////////////////////////////////////////////////////////////
    // StyleManager
    ///////////////////////////////////////////////////////////////////////////


    StyleManager::StyleManager(Allocator::ICoreAllocator* pCoreAllocator)
      : mpCoreAllocator(pCoreAllocator ? pCoreAllocator : EA::Text::GetAllocator()),
        mStyleMap(),
        mnNextStyleId(1)
        #if EATEXT_THREAD_SAFETY_ENABLED
      , mMutex()
        #endif
    {
        // Empty
    }


    StyleManager::~StyleManager()
    {
        // Empty
        for (StyleMap::iterator it = mStyleMap.begin(); it != mStyleMap.end(); ++it)
        {
            TextStyleEx* const pTS = it->second;

            if(--pTS->mnRefCount == 0)
                CORE_DELETE(mpCoreAllocator, pTS);

            it->second = NULL;
        }
    }


    void StyleManager::SetAllocator(Allocator::ICoreAllocator* pCoreAllocator)
    {
        mpCoreAllocator = pCoreAllocator;
    }


    const TextStyle* StyleManager::GetStyle(uint32_t nStyleId, TextStyle* pTS) const
    {
        #if EATEXT_THREAD_SAFETY_ENABLED
            EA::Thread::AutoFutex autoMutex(mMutex);
        #endif

        const StyleMap::const_iterator it = mStyleMap.find(nStyleId);

        if(it == mStyleMap.end())
            pTS = NULL;
        else
        {
            TextStyle* const pTSTemp = (*it).second;

            if(pTS)
               *pTS = *pTSTemp; // Copy the style
            else
                pTS =  pTSTemp; // Return pointer to the style.
        }

        return pTS;
    }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    uint32_t GetStyleId(const char* pStyleName)
    {
        if(pStyleName && *pStyleName)
        {
            uint32_t c, nHash = UINT32_C(2166136261);
            while((c = (uint8_t)*pStyleName++) != 0)   // We implement an FNV1 hash here.
                nHash = (nHash * 16777619) ^ c;
            return nHash;
        }

        return 0;
    }


    namespace Parse
    {


        const uint32_t kBufferSize = 256;

        enum ParseState
        {
            kParseStateGetName,
            kParseStateGetProperties
        };

        /// GetTextLine
        /// Reads the next line of non-empty text. 
        bool GetTextLine(const char*& pText, char* pLine)
        {
            char*       pLineCur = pLine;
            char* const pLineEnd = pLine + kBufferSize;

            while(*pText)
            {
                if((*pText == '\n') || (*pText == '\r'))
                {
                    while((*pText == '\n') || (*pText == '\r'))
                        pText++;
                    break;
                }
                
                if(pLineCur < (pLineEnd - 1))
                    *pLineCur++ = *pText;

                pText++;
            }

            *pLineCur = 0;

            return (*pText != 0) || (*pLine != 0);
        }

        /// IsUsableLine
        /// Returns true if line is non-empty and isn't a comment line.
        bool IsUsableLine(const char* pText)
        {
            if(pText[0] == '\0')
                return false;
            if((pText[0] == '/') && (pText[1] == '/'))
                return false;
            return true;
        }

        /// StripSpace
        /// Removes leading and trailing space from a string.
        bool StripSpace(char* pText)
        {
            unsigned char* pTextBegin = (unsigned char*)pText;
            while(*pTextBegin && isspace(*pTextBegin))
                pTextBegin++;

            const size_t nStrlen = strlen((const char*)pTextBegin);
            if(pTextBegin > (unsigned char*)pText)
                memmove(pText, pTextBegin, nStrlen + 1);

            unsigned char* pTextEnd = (unsigned char*)pText + nStrlen - 1;
            while((pTextEnd >= pTextBegin) && isspace(*pTextEnd))
                *pTextEnd-- = 0;

            return (*pText != 0);
        }

        /// StripQuotes
        /// Removes leading and trailing quote marks from a string.
        bool StripQuotes(char* pText)
        {
            size_t nStrlen = strlen(pText);

            if(nStrlen && pText[nStrlen - 1] == '"')
                pText[--nStrlen] = 0;
            if(pText[0] == '"')
                memmove(pText, pText + 1, nStrlen * sizeof(char));

            return (*pText != 0);
        }


        /// GetProperty
        /// Expects a string like this: "font-size : 37".
        /// or this: "font-size : 37"; font-style : italic.
        /// and returns the property and value of "font-size" and "37".
        bool GetProperty(const char*& pText, char* pProperty, char* pValue)
        {
            char* const pDivider = strchr((char *)pText, ':');

            if(pDivider)
            {
                size_t nLength = (size_t)(pDivider - pText);
                if(nLength >= kBufferSize)
                    nLength = kBufferSize - 1;
                strncpy(pProperty, pText, nLength);
                pProperty[nLength] = 0;
                StripSpace(pProperty);

                char* const pSemicolon = strchr(pDivider, ';');

                if(pSemicolon)
                {
                    nLength = (size_t)(pSemicolon - pDivider);
                    if(nLength >= kBufferSize)
                        nLength = kBufferSize - 1;
                    strncpy(pValue, pDivider + 1, nLength);
                    pValue[nLength - 1] = 0; // - 1 here because we want to get rid of the trailing ';' char.
                    pText = pSemicolon + 1;
                }
                else
                {
                    strncpy(pValue, pDivider + 1, kBufferSize - 1);
                    pValue[kBufferSize - 1] = 0;
                    pText += strlen(pText);
                }

                StripSpace(pValue);

                return true;
            }

            return false;
        }

        /// GetNameAndId
        /// Expects a string line this: "Main Menu"
        /// or this: "Main Menu (0x12345678)"
        /// and returns the name and id from it (e.g. "Main Menu" and 0x12345678)
        bool GetNameAndId(const char* pText, char* pParentName, uint32_t& parentId)
        {
            uint32_t    nId = 0;
            size_t      nLength;
            char* const pId = strchr((char *)pText, '(');

            if(pId)
            {
                nId = strtoul(pId + 1, NULL, 16);
                nLength = (size_t)(pId - pText);
            }
            else
                nLength = strlen(pText);

            if(nLength >= kBufferSize)
                nLength = kBufferSize - 1;
            strncpy(pParentName, pText, nLength);
            pParentName[nLength] = 0;
            StripSpace(pParentName);

            if(*pParentName || nId)
            {
                if(!pParentName)
                    sprintf(pParentName, "0x%08x", (unsigned)nId);
                else if(nId == 0)
                    nId = GetStyleId(pParentName);
                parentId = nId;
                return true;
            }
            return false;
        }

        void Strncpy8To16(char16_t* pDestination, const char8_t* pSource, uint32_t n)
        {
            const char8_t* s = pSource - 1;
            char16_t*      d = pDestination - 1;

            n++;
            while(--n)
            {
                if((*++d = (uint8_t)*++s) == 0)
                {
                    while(--n)
                        *++d = 0;
                    break;
                }
            }
        }

        uint32_t Strlen16(const char16_t* pString)
        {
            uint32_t nLength = (uint32_t) - 1;
            const char16_t* p = pString - 1;
            do{ ++nLength; }while(*++p);
            return nLength;
        }

    } // namespace Parse


    using namespace Parse;

    ///////////////////////////////////////////////////////////////////////////////
    // ParseStyleText
    //
    // We implement this in as modular and simple way as we can. If this 
    // implementation grows beyond some size then we may need to make it act like 
    // a more generic parsing engine.
    //
    bool ParseStyleText(const char*& pCSSText, char *pStyleName, uint32_t &pId, TextStyle &pTSCSSStyle, GetStyleFunc pFunc, void *pContext, AtCommandCallback pAtCallback, void *pAtContext )
    {
        //using namespace EA::StdC;

        char                  currentLine[kBufferSize];
        char                  currentPropertyName[kBufferSize];
        char                  currentPropertyValue[kBufferSize];
        TextStyle             currentStyle;
        ParseState            currentParseState = kParseStateGetName;

        while(GetTextLine(pCSSText, currentLine))
        {
            StripSpace(currentLine);

            // An @command. If a line begins with '@' then we call the user callback
            // and otherwise ignore the line. The primary use for this is so that the 
            // user can specify font files to load which happen to support the given
            // styles specified in the file.
            if(currentLine[0] == '@')
            {
                if(pAtCallback)
                    pAtCallback(currentLine + 1, pAtContext);
                continue;
            }

            if(IsUsableLine(currentLine)) // See if the line is empty or is a comment line.
            {
                switch (currentParseState)
                {
                    case kParseStateGetName:
                    {
                        // To do: Put some debug-time code in here to verify the format.
                        char* const pEnd = strchr(currentLine, '{');

                        if(pEnd)
                        {
                            *pEnd = 0;

                            char* pParent = strchr(currentLine, ':');

                            if(pParent)
                            {
                                uint32_t parentId;
                                char     parentName[256];

                                *pParent++ = 0;
                                if(GetNameAndId(pParent, parentName, parentId) && pFunc)
                                    (*pFunc)( parentId, currentStyle, pContext );
                            }

                            if(GetNameAndId(currentLine, pStyleName, pId))
                                currentParseState = kParseStateGetProperties;
                        }
                        break;
                    }

                    case kParseStateGetProperties:
                    {
                        if(currentLine[0] == '}') // If we have reached the end of the current style...
                        {
                            pTSCSSStyle = currentStyle;
                            return true;
                        }
                        else
                        {
                            const char* pText = currentLine;

                            while(GetProperty(pText, currentPropertyName, currentPropertyValue))
                            {
                                char* pValue = currentPropertyValue;

                                if(strcmp(currentPropertyName, "font-family") == 0) // (font-family: "Helvetica Neue", arial)
                                {
                                    for(uint32_t i = 0; i < kFamilyNameArrayCapacity;)
                                    {
                                        char* const pComma = strchr(pValue, ',');
                                        if(pComma)
                                            *pComma = 0;
                                        if(StripSpace(pValue))
                                        {
                                            if(StripQuotes(pValue))
                                            {
                                                Strncpy8To16(currentStyle.mFamilyNameArray[i], pValue, kFamilyNameCapacity - 1);
                                                currentStyle.mFamilyNameArray[i][kFamilyNameCapacity - 1] = 0;
                                                i++;
                                            }
                                        }
                                        if(pComma)
                                            pValue = pComma + 1;
                                        else
                                            break;
                                    }
                                }
                                else if(strcmp(currentPropertyName, "font-size") == 0) // Format = Npx or Npt. (font-size : 12px) (font-size : 12pt)
                                {
                                    // We aren't currently supporting named sizes.
                                    currentStyle.mfSize = (float)atof(pValue);
                                    EA_ASSERT(currentStyle.mfSize > 0.f);
                                }
                                else if(strcmp(currentPropertyName, "font-style") == 0)
                                {
                                    if(strcmp(pValue, "normal") == 0)
                                        currentStyle.mStyle = kStyleNormal;
                                    else if(strcmp(pValue, "italic") == 0)
                                        currentStyle.mStyle = kStyleItalic;
                                    else if(strcmp(pValue, "oblique") == 0)
                                        currentStyle.mStyle = kStyleOblique;
                                }
                                else if(strcmp(currentPropertyName, "font-weight") == 0)
                                {
                                    if(strcmp(pValue, "normal") == 0)
                                        currentStyle.mfWeight = 400;
                                    else if(strcmp(pValue, "bold") == 0)
                                        currentStyle.mfWeight = 700;
                                    else // We aren't currently supporting other named weights.
                                        currentStyle.mfWeight = (float)atof(pValue);
                                }
                                else if(strcmp(currentPropertyName, "font-variant") == 0)
                                {
                                    if(strcmp(pValue, "normal") == 0)
                                        currentStyle.mVariant = kVariantNormal;
                                    else if(strcmp(pValue, "small-caps") == 0)
                                        currentStyle.mVariant = kVariantSmallCaps;
                                }
                                else if(strcmp(currentPropertyName, "font-pitch") == 0)
                                {
                                    if(strcmp(pValue, "variable") == 0)
                                        currentStyle.mPitch = kPitchVariable;
                                    else if(strcmp(pValue, "fixed") == 0)
                                        currentStyle.mPitch = kPitchFixed;
                                }
                                else if(strcmp(currentPropertyName, "font-smooth") == 0)
                                {
                                    // currentStyle.mSmooth
                                    if(strcmp(pValue, "auto") == 0)
                                        currentStyle.mSmooth = kSmoothDefault;
                                    else if(strcmp(pValue, "never") == 0)
                                        currentStyle.mSmooth = kSmoothNone;
                                    else if(strcmp(pValue, "always") == 0)
                                        currentStyle.mSmooth = kSmoothEnabled;
                                    //else
                                    //{   // CSS smoothing seems screwy to me. I think smoothing should occur below a given size, not above it.
                                    //    const float fSize = (float)atof(pValue);
                                    //    if(fSize > 0)
                                    //        currentStyle.mSmooth = (currentStyle.mfSize >= fSize) ? kSmoothEnabled : kSmoothNone;
                                    //}
                                }
                                else if(strcmp(currentPropertyName, "color") == 0) // Format = #RGB or #RGBA. (color : #cc2255) (color : #cc2255ff)
                                {
                                    if(*pText == '#')
                                        pText++;

                                    currentStyle.mColor = 0xff000000 | StrtoU32( pText, NULL, 16 );
                                }
                                else if(strcmp(currentPropertyName, "background-color") == 0)
                                {
                                    if(*pText == '#')
                                        pText++;

                                    currentStyle.mColorBackground = 0xff000000 | StrtoU32( pText, NULL, 16 );
                                }
                                else if(strcmp(currentPropertyName, "text-decoration") == 0) //  (text-decoration: underline, line-through)
                                {
                                    currentStyle.mnDecorationFlags = kDFNone; // If there is a text-decoration property, then we erase any inherited properties.

                                    for(;;) // use a for loop to avoid compiler warnings that you'd get with while loop.
                                    {
                                        char* const pComma = strchr(pValue, ',');
                                        if(pComma)
                                            *pComma = 0;
                                        if(StripSpace(pValue))
                                        {
                                            if(strcmp(pValue, "underline") == 0)
                                                currentStyle.mnDecorationFlags |= kDFUnderline;
                                            else if(strcmp(pValue, "overline") == 0)
                                                currentStyle.mnDecorationFlags |= kDFOverline;
                                            else if(strcmp(pValue, "line-through") == 0)
                                                currentStyle.mnDecorationFlags |= kDFLineThrough;
                                        }
                                        if(pComma)
                                            pValue = pComma + 1;
                                        else
                                            break;
                                    }
                                }
                                else if(strcmp(currentPropertyName, "writing-mode") == 0) // lr-tb | rl-tb | tb-rl | tb-lr
                                {
                                    // To do.
                                    currentStyle.mDirection   = kDirectionDefault;
                                    currentStyle.mOrientation = kOrientationDefault;
                                }
                                else if(strcmp(currentPropertyName, "text-align") == 0) // left | center | right | justify
                                {
                                    if(strcmp(pValue, "left") == 0)
                                        currentStyle.mHAlignment = kHALeft;
                                    else if(strcmp(pValue, "center") == 0)
                                        currentStyle.mHAlignment = kHACenter;
                                    else if(strcmp(pValue, "right") == 0)
                                        currentStyle.mHAlignment = kHARight;
                                    else if(strcmp(pValue, "justify") == 0)
                                        currentStyle.mHAlignment = kHAJustify;
                                }
                                else if(strcmp(currentPropertyName, "text-valign") == 0) // top, middle, bottom
                                {
                                    // As of this writing (2/2005), this is not a standard CSS style.
                                    if(strcmp(pValue, "top") == 0)
                                        currentStyle.mVAlignment = kVATop;
                                    else if(strcmp(pValue, "middle") == 0) // HTML uses "middle" instead of center.
                                        currentStyle.mVAlignment = kVACenter;
                                    else if(strcmp(pValue, "bottom") == 0)
                                        currentStyle.mVAlignment = kVABottom;
                                }
                                else if(strcmp(currentPropertyName, "text-justify") == 0) // inter-word | inter-ideograph | distribute | newspaper | inter-cluster | kashida
                                {
                                    if(strcmp(pValue, "inter-word") == 0)
                                        currentStyle.mJustify = kJustifyInterWord;
                                    else if(strcmp(pValue, "inter-ideograph") == 0)
                                        currentStyle.mJustify = kJustifyInterIdeograph;
                                    else if(strcmp(pValue, "distribute") == 0)
                                        currentStyle.mJustify = kJustifyDistribute;
                                    else if(strcmp(pValue, "newspaper") == 0)
                                        currentStyle.mJustify = kJustifyNewspaper;
                                    else if(strcmp(pValue, "inter-cluster") == 0)
                                        currentStyle.mJustify = kJustifyInterCluster;
                                    else if(strcmp(pValue, "kashida") == 0)
                                        currentStyle.mJustify = kJustifyKashida;
                                }
                                else if(strcmp(currentPropertyName, "wrap-option") == 0) // wrap | no-wrap | soft-wrap | hard-wrap | emergency
                                {
                                    if(strcmp(pValue, "wrap") == 0)
                                        currentStyle.mTextWrap = kTWWrap;
                                    else if(strcmp(pValue, "no-wrap") == 0)
                                        currentStyle.mTextWrap = kTWNone;
                                    else if(strcmp(pValue, "soft-wrap") == 0)
                                        currentStyle.mTextWrap = kTWSoft;
                                    else if(strcmp(pValue, "hard-wrap") == 0)
                                        currentStyle.mTextWrap = kTWHard;
                                    else if(strcmp(pValue, "emergency") == 0)
                                        currentStyle.mTextWrap = kTWEmergency;
                                }
                                else if(strcmp(currentPropertyName, "text-overflow-mode") == 0) // none | clip | ellipsis | ellipsis-word
                                {
                                    if(strcmp(pValue, "none") == 0)
                                        currentStyle.mTextOverflow = kTONone;
                                    else if(strcmp(pValue, "clip") == 0)
                                        currentStyle.mTextOverflow = kTOClip;
                                    else if(strcmp(pValue, "ellipsis") == 0)
                                        currentStyle.mTextOverflow = kTOEllipsis;
                                }
                                else if(strcmp(currentPropertyName, "font-emphasize-style") == 0)
                                {
                                    if(strcmp(pValue, "none") == 0)
                                        currentStyle.mEmphasisStyle = kESNone;
                                    else if(strcmp(pValue, "accent") == 0)
                                        currentStyle.mEmphasisStyle = kESAccent;
                                    else if(strcmp(pValue, "dot") == 0)
                                        currentStyle.mEmphasisStyle = kESDot;
                                    else if(strcmp(pValue, "circle") == 0)
                                        currentStyle.mEmphasisStyle = kESCircle;
                                    else if(strcmp(pValue, "disc") == 0)
                                        currentStyle.mEmphasisStyle = kESDisc;
                                }
                                else if(strcmp(currentPropertyName, "font-emphasize-position") == 0)
                                {
                                    if(strcmp(pValue, "before") == 0)
                                        currentStyle.mEmphasisPosition = kEPBefore;
                                    else if(strcmp(pValue, "after") == 0)
                                        currentStyle.mEmphasisPosition = kEPAfter;
                                }
                                else if(strcmp(currentPropertyName, "digit-substitution") == 0) // none | context | western | national | traditional
                                {
                                    // As of this writing (2/2005), this is not a standard CSS style.
                                    if(strcmp(pValue, "none") == 0)
                                        currentStyle.mDigitSubstitution = kDSNone;
                                    else if(strcmp(pValue, "context") == 0)
                                        currentStyle.mDigitSubstitution = kDSContext;
                                    else if(strcmp(pValue, "western") == 0)
                                        currentStyle.mDigitSubstitution = kDSWestern;
                                    else if(strcmp(pValue, "national") == 0)
                                        currentStyle.mDigitSubstitution = kDSNational;
                                    else if(strcmp(pValue, "traditional") == 0)
                                        currentStyle.mDigitSubstitution = kDSTraditional;
                                }
                                else if(strcmp(currentPropertyName, "password-mode") == 0) // none | password
                                {
                                    // As of this writing (2/2005), this is not a standard CSS style.
                                    if(strcmp(pValue, "none") == 0)
                                        currentStyle.mPasswordMode = kPMNone;
                                    else if(strcmp(pValue, "password") == 0)
                                        currentStyle.mPasswordMode = kPMPassword;
                                }
                                else if(strcmp(currentPropertyName, "font-stretch") == 0)
                                {
                                    // font-stretch is normally specified by "condensed", "semi-condensed", "normal", etc.
                                    // In practice it is often more useful for us to use floating point stretching.
                                    // We may want to establish a mapping between these names and stretch values.
                                    currentStyle.mfStretch = (float)atof(pValue);
                                }
                                else if(strcmp(currentPropertyName, "text-letterspacing") == 0) // (letter-spacing: 12px) (letter-spacing: -0.5px)
                                {
                                    currentStyle.mfLetterSpacing = (float)atof(pValue);
                                }
                                else if(strcmp(currentPropertyName, "text-wordspacing") == 0)
                                {
                                    currentStyle.mfWordSpacing = (float)atof(pValue);
                                }
                                else if(strcmp(currentPropertyName, "text-linespacing") == 0)
                                {
                                    currentStyle.mfLineSpace = (float)atof(pValue);
                                    EA_ASSERT(currentStyle.mfLineSpace > 0);
                                }
                            }
                        }

                        break;
                    }
                }
            }
        }
        return false;

    } // ParseStyleText()

#endif // #endif // EATEXT_STYLE_MANAGER_ENABLED

} // namespace Text

} // namespace EA











