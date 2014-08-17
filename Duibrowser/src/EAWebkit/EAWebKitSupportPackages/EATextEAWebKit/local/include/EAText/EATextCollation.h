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
// EATextCollation.h
// By Paul Pedriana - 2004
//
///////////////////////////////////////////////////////////////////////////////


#ifndef EATEXT_EATEXTCOLLATION_H
#define EATEXT_EATEXTCOLLATION_H


#include <EAText/EAText.h>
#include <EAText/EATextUnicode.h>
#include <EASTL/fixed_string.h>
#include <stddef.h>


namespace EA
{
    namespace Allocator
    {
        class ICoreAllocator;
    }

    namespace Text
    {

        enum NormalizationResultFlags
        {
            kNRFNone,
            kNRFAscii,     // The text was ASCII-only.
            kNRFUnchanged  // The text was already normalized or denormalized.
        };


        /// Normalize
        ///
        /// Normalizes a string as per the given normalization type.
        /// Returns the required output capacity, which will be greater than
        /// the nTextOutputCapacity value if there wasn't enough space. 
        /// Any UCS-2 Unicode value is valid as input text, including 0.
        /// The input is not expected to be 0-terminated, nor is the output
        /// 0-terminated; the characters are simply compressed as-is.
        ///
        uint32_t Normalize(const Char* pTextInput, uint32_t nTextInputLength,
                           Char* pTextOutput, uint32_t nTextOutputCapacity, 
                           NormalizationType nt, int* pNormalizationResultFlags = NULL);

        /// IsNormalized
        ///
        /// Tells if a string is normalized as per the given normalization type.
        /// Note that with a return value of true, the kNRFNormalized flag will
        /// also be set.
        ///
        /// This function has the same affect as the following, though it may
        /// possibly not be implemented as such:
        ///     int flags;
        ///     Normalize(pText, nTextLength, NULL, 0, nt, &flags);
        ///     if(pNormalizationResultFlags)
        ///         *pNormalizationResultFlags = flags;
        ///     return (flags & kNRFUnchanged) != 0;
        ///
        bool IsNormalized(const Char* pText, uint32_t nTextLength, 
                          NormalizationType nt, int* pNormalizationResultFlags = NULL);


        /// GetCase
        ///
        /// Returns the case of the given character.
        ///
        CaseType GetCase(Char c);


        /// ConvertCase
        ///
        /// Converts the case of a string.
        /// Returns the required output capacity, which will be greater than
        /// the nTextOutputCapacity value if there wasn't enough space. 
        /// Any UCS-2 Unicode value is valid as input text, including 0.
        /// The input is not expected to be 0-terminated, nor is the output
        /// 0-terminated; the characters are simply compressed as-is.
        ///
        uint32_t ConvertCase(const Char* pTextInput, uint32_t nTextInputLength,
                             Char* pTextOutput, uint32_t nTextOutputCapacity, CaseType caseType);



        /// Collator
        ///
        /// Implements string collation (sorting and comparison).
        /// To implement a string sort, you could use this Collator with 
        /// a conventional sorting algorithm. 
        ///
        /// This class design and implementation is based on the Unicode Standard 5.0.
        /// See the Unicode Technical Standard #10 (Collation) for a detailed discussion
        /// of proper collation.
        ///
        /// String sort ordering is something that varies per language. By this we mean
        /// that a given language might sort the same strings in a different way from 
        /// another language. So you need to specify the language used by this class.
        /// You should not mix sorting orders produced by a Collator set to use two 
        /// different languages, as they may produce different results with the same 
        /// input.
        ///
        /// The Compare function compares two strings, returning one of three values:
        ///     < 0  -- p1 is less than p2
        ///     = 0  -- p1 == p2
        ///     > 0  -- p2 is less than p1
        ///
        /// Note that it is possible for two different input strings to compare as equal.
        /// This is possible when one of the strings decomposes into the other. For example,
        /// the a character with an accent built into it decomposes into two chars, the base
        /// char and the accent char. Such a character will compare as equal to its decomposed
        /// equivalent.
        /// 
        /// Example usage:
        ///     Collator collator;
        ///     eastl::vector<char16_t*> stringArray;
        /// 
        ///     collator.SetLocale("ko-kr");
        ///     collator.SetLocale(Collator::kSOLocaleFirst, 1);
        ///     eastl::sort(stringArray.begin(), stringArray.end(), collator);
        ///
        class Collator
        {
        public:
            enum SpecialCharOption  // See Unicode Technical Standard #10, section 3.2.2.
            {
                kSCOBlanked,
                kSCONonIgnorable,
                kSCOShifted,
                kSCOShiftTrimmed
            };

            enum AccentSortOption
            {
                kASODefault,
                kASOForward,
                kASOReverse
            };

            enum SortOption
            {
                kSONone,
                kSOSpecialChars,    /// One of SpecialCharOption. Controls how characters like ' ' and '-' participate (or don't participate) in sorting. Default is kSCOShifted.
                kSOLocaleFirst,     /// True or false. If enabled, sort strings from the script associated with the locale in front of other scripts. Default is disabled.
                kSOCapitalsFirst,   /// True or false. If enabled, upper case sorts before lower case instead of lower case first. Default is disabled.
                kSOReverseAccents   /// One of AccentSortOption. Specifies how to sort accents; allows you to override the default behavior for the current language.
            };

        public:
            Collator(bool bEnableCache = false, Allocator::ICoreAllocator* pAllocator = NULL);

            /// Sets a locale as per the ISO-639 and ISO-3166 letter and country codes.
            /// An example locale might be "en-us", "fr-fr", or "ja-jp".
            /// The locale must be set correctly for sorting to occur properly for the given locale.
            void SetLocale(const char* pLocale);

            /// Enables the sort cache, which causes intermediate calculations to be 
            /// cached for later use. If you will be sorting a large number of strings
            /// then you can speed up the operation by enabling the cache.
            /// If the cache is disabled then any cache memory is immediately freed.
            void EnableCache(bool bEnableCache, Allocator::ICoreAllocator* pAllocator = NULL, uint32_t maxMemoryUsage = 0xffffffff);

            /// Sets one of the options specified by SortOptions.
            void SetSortOption(SortOption option, int value);

            /// The Compare function compares two strings, returning one of three values:
            ///     < 0  -- p1 is less than p2
            ///     = 0  -- p1 == p2
            ///     > 0  -- p2 is less than p1
            int Compare(const Char* p1, const Char* p2);

            /// STL/EASTL comparison operator.
            /// Returns true if p1 is less than p2. A return value of false means that p2 is >= p1.
            bool operator()(const Char* p1, const Char* p2)
                { return (Compare(p1, p2) < 0); }

        protected:
            struct CollationElement
            {
                unsigned mPrimary   : 16;
                unsigned mSecondary : 10;
                unsigned mTertiary  :  6;
            };

            static const size_t kFixedLength = 32;

            typedef eastl::fixed_string<Char,    kFixedLength> FixedSortString;
            typedef eastl::fixed_string<uint8_t, kFixedLength> FixedSortKey;

        protected:
            void BuildSortKey(const FixedSortString& s, FixedSortKey& k);

        protected:
            char              mLocale[8];
            SpecialCharOption mSpecialCharOption;
            bool              mbLocaleFirst;
            bool              mbCapitalsFirst;
            bool              mbReverseAccents;
            bool              mbForwardAccents;

            #ifdef EA_PLATFORM_WINDOWS
                unsigned long mLCID;
            #endif

            // Cache data
            // To to: Make sort keys use machine words instead of bytes.
            // To consider: It might be a little more optimal if we implemented a specialized SortStringSortKey struct.

            // typedef eastl::basic_string<Char, EASTLCoreAllocator>    SortString;
            // typedef eastl::basic_string<uint8_t, EASTLCoreAllocator> SortKey;
            // typedef eastl::hash_map<SortString, SortKey>             SortCache;
            // SortCache mSortCache;

        }; // class Collator

    } // namespace Text

} // namespace EA




#endif // EATEXT_EATEXTCOLLATION_H











