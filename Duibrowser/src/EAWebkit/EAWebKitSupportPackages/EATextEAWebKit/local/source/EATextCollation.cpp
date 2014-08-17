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
// EATextCollation.cpp
//
// By Paul Pedriana - 2004
//
///////////////////////////////////////////////////////////////////////////////


#include <EAText/EATextCollation.h>
#include <EAText/internal/EATextScriptHangul.h>
#include <EAText/internal/StdC.h>
#include <EASTL/bonus/sort_extra.h>
#include "internal/CollationData.inl"
#include EA_ASSERT_HEADER

#ifdef EA_PLATFORM_WINDOWS
    #include <windows.h>
#endif


namespace EA
{

namespace Text
{


struct SortByCombiningClass // : public eastl::binary_function<Char, Char, bool>
{
    bool operator()(Char a, Char b) const {
        return (GetCombiningClass(a) < GetCombiningClass(b));
    }
};


#ifdef EA_PLATFORM_WINDOWS

    // This table is borrowed from the EALocale package.
    const char8_t* sOSLocaleTable[] = 
    {
        // "ISO^Win32LCID
        "en-us\0" "0400",      // Process or User Default Language     
        "sq-al\0" "041c",      // Albanian     
        "ar-sa\0" "0401",      // Arabic (Saudi Arabia)     
        "zh-tw\0" "0404",      // Chinese (Taiwan)     
        "zh-cn\0" "0804",      // Chinese (PRC)     
        "zh-hk\0" "0c04",      // Chinese (Hong Kong SAR, PRC)     
        "zh-sg\0" "1004",      // Chinese (Singapore)     
        "cs-cz\0" "0405",      // Czech     
        "da-dk\0" "0406",      // Danish     
        "nl-nl\0" "0413",      // Dutch (Netherlands)     
        "nl-be\0" "0813",      // Dutch (Belgium)     
        "en-us\0" "0409",      // English United States    
        "en-gb\0" "0809",      // English Great Britain (UK)    
        "fi-fi\0" "040b",      // Finnish     
        "fr-fr\0" "040c",      // French (Standard)     
        "fr-be\0" "080c",      // French (Belgian)     
        "fr-ca\0" "0c0c",      // French (Canadian)     
        "de-de\0" "0407",      // German (Standard)     
        "de-ch\0" "0807",      // German (Switzerland)     
        "el-gr\0" "0408",      // Greek     
        "iw-il\0" "040d",      // Hebrew     
        "hi-in\0" "0439",      // Hindi   
        "hu-hu\0" "040e",      // Hungarian     
        "is-is\0" "040f",      // Icelandic     
        "it-it\0" "0410",      // Italian (Standard)     
        "it-ch\0" "0810",      // Italian (Switzerland)     
        "ja-jp\0" "0411",      // Japanese     
        "ko-kp\0" "0412",      // Korean (North)    
        "ko-kr\0" "0412",      // Korean (South)    
        "no-no\0" "0414",      // Norwegian (Bokmal)     
        "pl-pl\0" "0415",      // Polish     
        "pt-br\0" "0416",      // Portuguese (Brazil)     
        "pt-pt\0" "0816",      // Portuguese (Portugal)     
        "ro-ro\0" "0418",      // Romanian     
        "ru-ru\0" "0419",      // Russian     
        "sk-sk\0" "041b",      // Slovak     
        "es-es\0" "040a",      // Spanish (Spain, Traditional Sort)     
        "es-mx\0" "080a",      // Spanish (Mexican)     
        "es-es\0" "0c0a",      // Spanish (Spain, Modern Sort)     
        "sv-se\0" "041d",      // Swedish     
        "sv-fi\0" "081d",      // Swedish (Finland)
        "th-th\0" "041e",      // Thai     
        "tr-tr\0" "041f"       // Turkish     
    };

#endif


///////////////////////////////////////////////////////////////////////////////
// GetCanonicalDecomposition
//
const Char* GetCanonicalDecomposition(Char& c, uint32_t& count)
{
    // There are no decomposable chars below 0x00C0, and there are none
    // of interest to us above 0x30FE.

    if((c >= 0x00C0) && (c <= 0x30FE))
    {
        // To consider: Replace this with eastl::lower_bound.

        int nLower = 0;
        int nUpper = (int)(sizeof(gNFDEntryArray) / sizeof(gNFDEntryArray[0]));

        while(nLower <= nUpper)
        {
            const int nMid = (nLower + nUpper) / 2;

            if(c == gNFDEntryArray[nMid].mChar)
            {
                count = 2;
                return &gNFDEntryArray[nMid].mD0; // We should change mD0 to be an array of size 2 instead of being two successive chars.
            }
            else if(c < gNFDEntryArray[nMid].mChar)
                nUpper = nMid - 1;
            else
                nLower = nMid + 1;
        }
    }

    count = 1;
    return &c;
}


///////////////////////////////////////////////////////////////////////////////
// IsNormalized
//
bool IsNormalized(const Char* pText, uint32_t nTextLength, NormalizationType nt, int* pNormalizationResultFlags)
{
    // There is a potentially faster way to go about this, which is to use the "quick check" 
    // approach described in UAX #15 and via the DerivedNormalizationProps.txt file published
    // with the Unicode Standard data. 

    int flags;

    Normalize(pText, nTextLength, NULL, 0, nt, &flags);

    if(pNormalizationResultFlags)
        *pNormalizationResultFlags = flags;

    return (flags & kNRFUnchanged) != 0;
}


///////////////////////////////////////////////////////////////////////////////
// Normalize
//
// Implement no normalization form. Simply copy the text and return the flags
// associated with it. This function is essentially a fallback that has little
// practical purpose beyond giving the caller the resulting flag information.
//
uint32_t NormalizeNone(const Char* pTextInput,  uint32_t nTextInputLength,
                             Char* pTextOutput, uint32_t nTextOutputCapacity, 
                       NormalizationType /*nt*/, int* pNormalizationResultFlags)
{
    int flags = (kNRFAscii | kNRFUnchanged);

    for(uint32_t i = 0; i < nTextInputLength; ++i, ++pTextInput)
    {
        if(*pTextInput >= 0x0080)
            flags &= ~kNRFAscii;

        if(i < nTextOutputCapacity)
            *pTextOutput++ = *pTextInput;
    }

    if(pNormalizationResultFlags)
       *pNormalizationResultFlags = flags;

    return nTextInputLength;
}


///////////////////////////////////////////////////////////////////////////////
// NormalizeD
//
// Implement normalization NFD and NFKD (decomposition forms).
//
// As with much of the rest of this Unicode library, we don't support the 
// entire Unicode character space, though we support the large majority that
// we are going to be interested in. There are a large number of characters
// in the Unicode Standard which have non-trivial decompositions but also 
// are unlikely to be seen by our users. Instead of making a huge table of
// decomposition data, we use the Expands_On_NFD section of DerivedNormalizationProps.txt
// to decide what to decompose and write manual decompositions.
//
// If you need a more full treatment of the Unicode standard, please contact
// the maintainer of this package.
//
uint32_t NormalizeD(const Char* pTextInput,  uint32_t nTextInputLength,
                          Char* pTextOutput, uint32_t nTextOutputCapacity, 
                    NormalizationType nt, int* pNormalizationResultFlags)
{
    uint32_t nOutputLength = 0;
    int      flags = (kNRFAscii | kNRFUnchanged); // The text is ASCII and denormalized until proven otherwise.

    for(uint32_t i = 0; i < nTextInputLength; ++i, ++pTextInput)
    {
        Char c = *pTextInput;

        if(c >= 0x0080)
            flags &= ~kNRFAscii;

        // Hangul (Korean) decomposition. With Hangul, NFD and NFKD are the same,
        // so both normalization forms can share the same code here.
        if((c - 0xAC00u) <= (0xD7A4u - 0xAC00u)) // If c is composed Hangul...
        {
            c -= kHangulBegin;

            const Char cL = kJamoLBase + (c / kJamoNCount);
            const Char cV = kJamoVBase + (c % kJamoNCount) / kJamoTCount;
            const Char cT = kJamoTBase + (c % kJamoTCount);

            if(nOutputLength++ < nTextOutputCapacity)
                *pTextOutput++ = cL;

            if(nOutputLength++ < nTextOutputCapacity)
                *pTextOutput++ = cV;

            if(cT != kJamoTBase)
            {
                if(nOutputLength++ < nTextOutputCapacity)
                    *pTextOutput++ = cT;
            }
        }
        else
        {
            // We need to recursively apply character decomposition. By recursively, we mean that
            // char c may decompose into char c1 and c2, but either of c1 or c2 may itself decompose.
            // In practice such recursive decomposition is rarely required.

            typedef eastl::fixed_string<Char, 16> DString;

            DString      ds(&c, 1);
            uint32_t     count;
            eastl_size_t i = 0;
            eastl_size_t iEnd;

            (void)nt;                  // Avoid unused var warning.
            EA_ASSERT(nt != kNT_NFKD); // We don't yet have support for compatibility decomposition.

            while(i < ds.size()) // Size can change so we re-evaluate it every loop iteration.
            {
                const Char* const p = GetCanonicalDecomposition(ds[i], count);

                if(count > 1) // If there is a decomposition...
                    ds.replace(i, 1, p, count);
                else
                    ++i;
            }

            // We need to sort the ds entries by combining class.
            if(ds.size() > 1)
                eastl::bubble_sort(ds.begin(), ds.end(), SortByCombiningClass());

            for(i = 0, iEnd = ds.size(); i < iEnd; i++)
            {
                if(nOutputLength++ < nTextOutputCapacity)
                    *pTextOutput++ = ds[i];
            }
        }
    }

    if(pNormalizationResultFlags)
       *pNormalizationResultFlags = flags;

    return nOutputLength;
}


///////////////////////////////////////////////////////////////////////////////
// NormalizeC
//
// Implement normalization NFC and NFKC (composition forms).
//
// As with much of the rest of this Unicode library, we don't support the 
// entire Unicode character space, though we support the large majority that
// we are going to be interested in. There are a large number of characters
// in the Unicode Standard which have non-trivial compositions but also 
// are unlikely to be seen by our users. 
//
// If you need a more full treatment of the Unicode standard, please contact
// the maintainer of this package.
//
uint32_t NormalizeC(const Char* pTextInput,  uint32_t nTextInputLength,
                          Char* pTextOutput, uint32_t nTextOutputCapacity, 
                    NormalizationType nt, int* pNormalizationResultFlags)
{
    // To do: Implement this. It currently just does a no-op copy.

    return NormalizeNone(pTextInput, nTextInputLength, pTextOutput, nTextOutputCapacity, nt, pNormalizationResultFlags);
}


///////////////////////////////////////////////////////////////////////////////
// Normalize
//
// This is the main entrypoint to Normalize.
//
uint32_t Normalize(const Char* pTextInput,  uint32_t nTextInputLength,
                         Char* pTextOutput, uint32_t nTextOutputCapacity, 
                   NormalizationType nt, int* pNormalizationResultFlags)
{
    if(nTextInputLength == 0xffffffff)
        nTextInputLength = (uint32_t)Strlen(pTextInput);

    if((nt == kNT_NFD) || (nt == kNT_NFKD)) // Decomposition forms
        return NormalizeD(pTextInput, nTextInputLength, pTextOutput, nTextOutputCapacity, nt, pNormalizationResultFlags);

    if((nt == kNT_NFC) || (nt == kNT_NFKC)) // Composition forms
        return NormalizeC(pTextInput, nTextInputLength, pTextOutput, nTextOutputCapacity, nt, pNormalizationResultFlags);

    return NormalizeNone(pTextInput, nTextInputLength, pTextOutput, nTextOutputCapacity, nt, pNormalizationResultFlags);
}



CaseType GetCase(Char /*c*/)
{
    // To do: Implement this.
    return kCaseTypeNone;
}



uint32_t ConvertCase(const Char* pTextInput,  uint32_t nTextInputLength,
                           Char* pTextOutput, uint32_t nTextOutputCapacity, CaseType /*caseType*/)
{
    // This function is unimplemented. It currently merely echoes the 
    // characters back and says they are normalized. To do: Implement this.

    for(uint32_t i = 0; (i < nTextInputLength) && (i < nTextOutputCapacity); i++)
        *pTextOutput++ = *pTextInput++;

    return nTextInputLength;
}




///////////////////////////////////////////////////////////////////////////////
// Collator
///////////////////////////////////////////////////////////////////////////////

Collator::Collator(bool /*bEnableCache*/, Allocator::ICoreAllocator* /*pAllocator*/)
{
    #ifdef EA_PLATFORM_WINDOWS
        mLCID = LOCALE_USER_DEFAULT;
    #endif
}


void Collator::SetLocale(const char* pLocale)
{
    #ifdef EA_PLATFORM_WINDOWS
        mLCID = LOCALE_USER_DEFAULT;

        const size_t kTableSize = sizeof(sOSLocaleTable) / sizeof(sOSLocaleTable[0]);
        size_t i = 0;

        for(i = 0; i < kTableSize; ++i)
        {
            if(Stricmp(pLocale, sOSLocaleTable[i]) == 0)
            {
                mLCID = StrtoU32(sOSLocaleTable[i] + 6, NULL, 16);
                break;
            }
        }

        if(i == kTableSize)
        {
            for(i = 0; i < kTableSize; ++i)
            {
                if(Strnicmp(pLocale, sOSLocaleTable[i], 2) == 0)
                {
                    mLCID = StrtoU32(sOSLocaleTable[i] + 6, NULL, 16);
                    break;
                }
            }
        }
    #else
        (void)pLocale;
    #endif
}


void Collator::EnableCache(bool /*bEnableCache*/, Allocator::ICoreAllocator* /*pAllocator*/, uint32_t /*maxMemoryUsage*/)
{

}


void Collator::SetSortOption(SortOption /*option*/, int /*value*/)
{

}


namespace Internal
{
    int SortKeyCompare(const uint8_t* pString1, const uint8_t* pString2)
    {
        uint8_t c1, c2;

        // PowerPC has lhzu and sthu instructions which make pre-increment 
        // updates faster for the PowerPC's load and store architecture.
        #ifdef EA_PROCESSOR_POWERPC
            --pString1;
            --pString2;

            while((c1 = *++pString1) == (c2 = *++pString2))
            {
                if(c1 == 0)
                    return 0;
            }
        #else
            while((c1 = *pString1++) == (c2 = *pString2++))
            {
                if(c1 == 0)
                    return 0;
            }
        #endif

        return (c1 - c2);
    }
}



///////////////////////////////////////////////////////////////////////////////
// BuildSortKey
//
// This function expects the string argument to be a decomposed string of 
// normalization type NFD (canonical decomposition). 
//
void Collator::BuildSortKey(const FixedSortString& /*s*/, FixedSortKey& /*k*/)
{
    // To do.
}



///////////////////////////////////////////////////////////////////////////////
// Compare
//
// Implement Unicode Technical Standard #10, section 4 (Main Algorithm)
//
// If the two strings are both entirely ASCII and mSpecialCharOption == kSCONonIgnorable, 
// then we can use a simple strcmp comparison. But we need to mind the kSOCapitalsFirst option.
//
// If caching is enabled and either string has a cached sort key, use the key.
//
int Collator::Compare(const Char* p1, const Char* p2)
{
    #ifdef EA_PLATFORM_WINDOWS
        // Until we completely finish the platform-independent code, we use the Windows CompareString API under Windows.
        const int result = CompareStringW(mLCID, 0, p1, (int)Strlen(p1), p2, (int)Strlen(p2));
        return (result - 2);

    #else
        using namespace Internal;

        int             result = 0;
        const Char*     p[2] = { p1, p2 };
        FixedSortString s[2];
        FixedSortKey    k[2];
        int             resultFlags[2];
        size_t          i;

        for(i = 0; i < 2; i++)
        {
            s[i].resize(kFixedLength - 1, Char(0)); // To do: Make it so this resize faster, as it currently fills the string with zeroes.

            const uint32_t requiredLength = Normalize(p[i], 0xffffffff, &s[i][0], s[i].size(), kNT_NFD, &resultFlags[i]);

            s[i].resize(requiredLength);

            if(requiredLength > s[i].size()) // If there wasn't initially enough room...
                Normalize(p[i], 0xffffffff, &s[i][0], s[i].size(), kNT_NFD, &resultFlags[i]);
        }

        // If we can do a simple strcmp...
        if((mSpecialCharOption == kSCONonIgnorable) && (resultFlags[0] & kNRFAscii) && (resultFlags[1] & kNRFAscii))
            result = Strcmp(s[0].c_str(), s[1].c_str());
        else
        {
            BuildSortKey(s[0], k[0]);
            BuildSortKey(s[1], k[1]);

            result = SortKeyCompare(k[0].c_str(), k[1].c_str());

            // If caching is enabled, cache the sort keys.
            //   (to do)
        }

        return result;
    #endif
}





} // namespace Text

} // namespace EA





















