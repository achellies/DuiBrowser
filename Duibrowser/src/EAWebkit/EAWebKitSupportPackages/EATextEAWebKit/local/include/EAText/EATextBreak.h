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
// EATextBreak.h
// By Paul Pedriana - 2004
//
// The following are defined here:
//    CharacterBreakIterator
//    WordBreakIterator
//    SentenceBreakIterator
//    ParagraphBreakIterator
//    LineBreakIterator
//
///////////////////////////////////////////////////////////////////////////////


#ifndef EATEXT_EATEXTBREAK_H
#define EATEXT_EATEXTBREAK_H


#include <EABase/EABase.h>
#include <EAText/EAText.h>
#include <EAText/EATextUnicode.h>
#include <EAText/EATextIterator.h>
#include <stddef.h>


namespace EA
{
    namespace Text
    {
        // Forward declarations
        class LayoutContext;


        /// BreakType
        ///
        /// Defines types of character, word, line, and sentence delineation. It is common that 
        /// a text layout algorithm will want to do line breaking analysis, while it is common 
        /// that a text editor implementing double clicking on a word would want to do word 
        /// boundary analysis.
        ///
        enum BreakType
        {
            kBreakTypeNone      =  0,    
            kBreakTypeCharacter =  1,    /// Character cluster boundary.
            kBreakTypeWord      =  2,    /// Word boundary.
            kBreakTypeLine      =  4,    /// Line break boundary.
            kBreakTypeSentence  =  8,    /// Sentence boundary.
            kBreakTypeParagraph = 16,    /// Paragraph boundary.
            kBreakTypeAll       = 31
        };


        /// WordBreakType
        ///
        /// Defines various types of word break types.
        ///
        enum WordBreakType
        {
            kWordBreakTypeNone     = 0,   /// No word break opportunity.
            kWordBreakTypeBegin    = 1,   /// Beginning of a word.
            kWordBreakTypeEnd      = 2,   /// Ending of a word.
            kWordBreakTypeAll      = 3    /// 
        };


        /// LineBreakType
        ///
        /// Defines various types of line break types.
        ///
        enum LineBreakType
        {
            kLineBreakTypeNone      =  0,   /// No line break opportunity. For example, you cannot under any circumstances break a line in the middle of a cluster.
            kLineBreakTypeEmergency =  1,   /// Only with "emergency" style line breaking would you break a line at such a position.
            kLineBreakTypeHyphen    =  2,   /// Hyphen break opportunity within a word that has no explicit hyphens. Hyphens are added to words as a last resort.
            kLineBreakTypePossible  =  4,   /// Possible (a.k.a. direct) break opportunity. For example, between words or after an explicit hyphen.
            kLineBreakTypeMandatory =  8,   /// Forced break (e.g. '/n').
            kLineBreakTypeAll       = 15    /// 
        };




        ///////////////////////////////////////////////////////////////////////
        /// InitWordBreakDictionary
        ///
        /// Initializes the break dictionary for use.
        ///
        void InitWordBreakDictionary();

        ///////////////////////////////////////////////////////////////////////
        /// ShutdownWordBreakDictionary
        ///
        /// Shuts down the break dictionary initialized by InitWordBreakDictionary
        /// and augmented by AddWordBreakDictionaryEntry.
        ///
        void ShutdownWordBreakDictionary();

        ///////////////////////////////////////////////////////////////////////
        /// AddWordBreakDictionaryEntry
        ///
        /// Adds a word to the word break dictionary. Typically this word will
        /// be a Thai word, as Thai is the primary language that requires a 
        /// dictionary to accurately break words and lines. You can use this
        /// to add new words to the existing break dictionary. For languages
        /// like Thai, there are many words that are either new, are slang, 
        /// or are transliterations from other languages and thus are not
        /// present in a typical dictionary. 
        ///
        void AddWordBreakDictionaryEntry(const char16_t* pWord);




        ///////////////////////////////////////////////////////////////////////
        /// BreakIteratorBase
        ///
        /// This is a base class for specific break iterators.
        ///
        class BreakIteratorBase
        {
        public:
            BreakIteratorBase()
                : mIterator(), mpLayoutContext(NULL) { }

            BreakIteratorBase(const TextRun* pTextRunArray, uint32_t nTextRunArraySize, 
                              const LayoutContext* pLayoutContext = NULL)
                : mIterator(pTextRunArray, nTextRunArraySize), mpLayoutContext(pLayoutContext) { }

            BreakIteratorBase(const TextRunIterator& tri, const LayoutContext* pLayoutContext = NULL)
                : mIterator(tri), mpLayoutContext(pLayoutContext) { }

            // Disabled, as enabling it would require us to maintain a TextRun object:
            // BreakIteratorBase(const Char* pText, uint32_t nTextLength, const LayoutContext* pLayoutContext = NULL)
            //    : BreakIteratorBase(pText, nTextLength, pLayoutContext) { }

            /// GetBegin
            uint32_t GetBegin() const
                { return 0; } // This code will change if and when we support variable starting offsets.

            /// AtBegin
            bool AtBegin() const
                { return mIterator.AtBegin(); }

            /// GetEnd
            uint32_t GetEnd() const
                { return mIterator.GetSize(); }

            /// AtEnd
            bool AtEnd() const
                { return mIterator.AtEnd(); }

            /// GetPosition
            uint32_t GetPosition() const
                { return mIterator.GetPosition(); }

            /// SetCurrent
            uint32_t SetPosition(uint32_t absolutePosition);

            /// SetRelativePosition
            uint32_t SetRelativePosition(int32_t relativePosition);

            Char operator*() const
                { return *mIterator; }

            /// operator[]
            Char operator[](uint32_t i) const
                { return mIterator[(uint32_t)i]; }

            /// GetIterator
            TextRunIterator& GetIterator()
                { return mIterator; }

            /// SetIterator
            void SetIterator(const TextRunIterator& tri)
                { mIterator = tri; }

        protected:
            TextRunIterator      mIterator;
            const LayoutContext* mpLayoutContext;
        };


        ///////////////////////////////////////////////////////////////////////
        /// CharacterBreakIterator
        ///
        /// Also known as a GraphemeClusterBreakIterator.
        /// 
        /// The arrows below show the positions that would be returned by 
        /// successive calls to GetNextCharBreak. In this example, the '`' char 
        /// is assumed to be a Unicode combining diacritical and not the standalone 
        /// ascii '`' char.
        ///    "Hello` Wòrld"
        ///     ^^^^^ ^^^^^^^
        ///
        /// Example usage:
        ///     uint32_t GetClusterSize(const Char* pText, uint32_t nLength) {
        ///         const TextRun tr(pText, nLength);
        ///         CharacterBreakIterator cbi(&tr, 1);
        ///         return cbi.GetNextCharBreak();
        ///     }
        ///
        class CharacterBreakIterator : public BreakIteratorBase
        {
        public:
            CharacterBreakIterator() { }

            CharacterBreakIterator(const TextRun* pTextRunArray, uint32_t nTextRunArraySize, 
                                   const LayoutContext* pLayoutContext = NULL)
                : BreakIteratorBase(pTextRunArray, nTextRunArraySize, pLayoutContext) { }

            CharacterBreakIterator(const TextRunIterator& tri, const LayoutContext* pLayoutContext = NULL)
                : BreakIteratorBase(tri, pLayoutContext) { }

            // Disabled, as enabling it would require us to maintain a TextRun object:
            // CharacterBreakIterator(const Char* pText, uint32_t nTextLength, const LayoutContext* pLayoutContext = NULL)
            //    : BreakIteratorBase(pText, nTextLength, pLayoutContext) { }

            /// GetPrevCharBreak
            /// Moves to the previous boundary or returns the end.
            uint32_t GetPrevCharBreak();

            /// GetNextCharBreak
            /// Moves to the next boundary. 
            /// The returned positions define the beginning of characters or the end.
            uint32_t GetNextCharBreak();
        };




        ///////////////////////////////////////////////////////////////////////
        /// WordBreakIterator
        ///
        /// The arrows below show the positions that would be returned by 
        /// successive calls to GetNextWordBreak with the default wordBreakType.
        ///    "Hello   world   hello   world"
        ///     ^    ^  ^    ^  ^    ^  ^    ^
        ///
        ///     TextRun           tr(L"Hello   world   hello   world", 29);
        ///     WordBreakIterator wbi.SetIterator(TextRunIterator(&tr));
        ///     uint32_t          b = 0;
        /// 
        ///     while(b < 29)
        ///     {
        ///         b = lbi.GetNextWordBreak(kWordBreakTypeBegin | kWordBreakTypeEnd);
        ///         // b will be: 0, 5, 8, 14, 16, 21, 24, 29 (for consecutive trips through this loop).
        ///     }
        ///
        class WordBreakIterator : public CharacterBreakIterator
        {
        public:
            WordBreakIterator()
                : mWordBreakType(kWordBreakTypeNone) { }

            WordBreakIterator(const TextRun* pTextRunArray, uint32_t nTextRunArraySize, 
                                const LayoutContext* pLayoutContext = NULL)
                : CharacterBreakIterator(pTextRunArray, nTextRunArraySize, pLayoutContext),
                  mWordBreakType(kWordBreakTypeNone) { }

            WordBreakIterator(const TextRunIterator& tri, const LayoutContext* pLayoutContext = NULL)
                : CharacterBreakIterator(tri, pLayoutContext),
                  mWordBreakType(kWordBreakTypeNone) { }

            // Disabled, as enabling it would require us to maintain a TextRun object:
            // WordBreakIterator(const Char* pText, uint32_t nTextLength, const LayoutContext* pLayoutContext = NULL)
            //    : CharacterBreakIterator(pText, nTextLength, pLayoutContext) { }

            /// GetWordBreakType
            /// Returns the WordBreakType of the most recent GetNextBreak or 
            /// GetPrevBreak call.
            WordBreakType GetWordBreakType() const
                { return mWordBreakType; }

            /// GetPrevBreak
            /// Moves to the previous boundary or returns the end.
            uint32_t GetPrevWordBreak(int wordBreakType = kWordBreakTypeBegin | kWordBreakTypeEnd);

            /// GetNextBreak
            /// Moves to the next boundary or returns the end. 
            /// The beginnings and/or endings of word boundaries are returned, 
            /// depending on the input wordBreakType.
            uint32_t GetNextWordBreak(int wordBreakType = kWordBreakTypeBegin | kWordBreakTypeEnd);

        protected:
            WordBreakType mWordBreakType;
        };



        ///////////////////////////////////////////////////////////////////////
        /// SentenceBreakIterator
        ///
        /// The arrows below show the positions that would be returned by 
        /// successive calls to GetNextSentenceBreak.
        ///    "Hello world.   Hello world.   "
        ///                 ^              ^  ^
        ///
        /// Example usage:
        ///     TextRun               tr(L"Hello world.   Hello world.   ", 30);
        ///     SentenceBreakIterator sbi.SetIterator(TextRunIterator(&tr));
        ///     uint32_t              b = 0;
        /// 
        ///     while(b < 30)
        ///     {
        ///         b = lbi.GetNextSentenceBreak();
        ///         // b will be: 12, 27, 30 (for consecutive trips through this loop).
        ///     }
        ///
        class SentenceBreakIterator : public CharacterBreakIterator
        {
        public:
            SentenceBreakIterator() { }

            SentenceBreakIterator(const TextRun* pTextRunArray, uint32_t nTextRunArraySize, 
                                  const LayoutContext* pLayoutContext = NULL)
                : CharacterBreakIterator(pTextRunArray, nTextRunArraySize, pLayoutContext) { }

            SentenceBreakIterator(const TextRunIterator& tri, const LayoutContext* pLayoutContext = NULL)
                : CharacterBreakIterator(tri, pLayoutContext) { }

            // Disabled, as enabling it would require us to maintain a TextRun object:
            // SentenceBreakIterator(const Char* pText, uint32_t nTextLength, const LayoutContext* pLayoutContext = NULL)
            //    : CharacterBreakIterator(pText, nTextLength, pLayoutContext) { }

            /// GetPrevSentenceBreak
            /// Moves to the previous boundary or returns the end.
            uint32_t GetPrevSentenceBreak();

            /// GetNextSentenceBreak
            /// Moves to the next boundary or returns the end. 
            uint32_t GetNextSentenceBreak();
        };



        ///////////////////////////////////////////////////////////////////////
        /// ParagraphBreakIterator
        ///
        /// Iterates paragraph boundaries. 
        /// Paragraph boundaries are defined to be any of the following:
        ///     "\n"
        ///     "\r"
        ///     "\r\n"
        ///     "\x2029"
        /// 
        /// The arrows below show the positions that would be returned by 
        /// successive calls to GetNextParagraphBreak.
        ///    "____\n____\r____\r\n____\x2029____"
        ///         ^     ^     ^       ^
        ///
        /// Example usage:
        ///     TextRun                tr(L"____\n____\r\n____\x2029____", 20);
        ///     ParagraphBreakIterator pbi.SetIterator(TextRunIterator(&tr));
        ///     uint32_t               b = 0;
        ///
        ///     while(b < 20)
        ///     {
        ///         b = pbi.GetNextParagraphBreak();
        ///         // b will be: 4, 9, 15, 20 (for consecutive trips through this loop).
        ///     }
        ///
        class ParagraphBreakIterator : public CharacterBreakIterator
        {
        public:
            ParagraphBreakIterator() { }

            ParagraphBreakIterator(const TextRun* pTextRunArray, uint32_t nTextRunArraySize, 
                                  const LayoutContext* pLayoutContext = NULL)
                : CharacterBreakIterator(pTextRunArray, nTextRunArraySize, pLayoutContext) { }

            ParagraphBreakIterator(const TextRunIterator& tri, const LayoutContext* pLayoutContext = NULL)
                : CharacterBreakIterator(tri, pLayoutContext) { }

         };



        ///////////////////////////////////////////////////////////////////////
        /// LineBreakIterator
        ///
        /// Line boundary analysis determines where a text string can be broken
        /// when line-wrapping. We implement Unicode Standard Annex #14.
        ///
        /// The arrows below show the positions that would be returned by 
        /// successive calls to GetNext.
        ///    "Hello   world   hello   world"
        ///     ^       ^       ^       ^    ^
        ///
        /// Example usage:
        ///     TextRun           tr(L"\n\r\n_\r_", 6);
        ///     LineBreakIterator lbi.SetIterator(TextRunIterator(&tr));
        ///     uint32_t          b = 0;
        ///
        ///     while(b < 6)
        ///     {
        ///         b = lbi.GetNextLineBreak(kLineBreakTypePossible | kLineBreakTypeMandatory);
        ///         // b will be: 1, 3, 5, 6 (for consecutive trips through this loop).
        ///     }
        ///
        class LineBreakIterator : public CharacterBreakIterator
        {
        public:
            LineBreakIterator()
                : mLineBreakType(kLineBreakTypeNone) { }

            LineBreakIterator(const TextRun* pTextRunArray, uint32_t nTextRunArraySize,
                                const LayoutContext* pLayoutContext = NULL)
                : CharacterBreakIterator(pTextRunArray, nTextRunArraySize, pLayoutContext),
                  mLineBreakType(kLineBreakTypeNone) { }

            LineBreakIterator(const TextRunIterator& tri, const LayoutContext* pLayoutContext = NULL)
                : CharacterBreakIterator(tri, pLayoutContext),
                  mLineBreakType(kLineBreakTypeNone) { }

            // Disabled, as enabling it would require us to maintain a TextRun object:
            //LineBreakIterator(const Char* pText, uint32_t nTextLength, const LayoutContext* pLayoutContext = NULL)
            //    : CharacterBreakIterator(pText, nTextLength, pLayoutContext),
            //      mLineBreakType(kLineBreakTypeMandatory) { }

            /// GetLineBreakType
            /// Returns the LineBreakType of the most recent GetNextBreak or 
            /// GetPrevBreak call.
            LineBreakType GetLineBreakType() const
                { return mLineBreakType; }

            /// GetPrevBreak
            /// Moves to the previous boundary and returns its position index, 
            /// else returns the end position index.
            uint32_t GetPrevLineBreak(int lineBreakType = kLineBreakTypePossible | kLineBreakTypeMandatory);

            /// GetNextBreak
            /// Moves to the next boundary and returns its position index, 
            /// else returns the end position index. 
            uint32_t GetNextLineBreak(int lineBreakType = kLineBreakTypePossible | kLineBreakTypeMandatory);

        protected:
            LineBreakType GetNextSoutheastAsianLineBreak(Char& cCurrent, Char& cNext, LineBreakCategory& lbcBase, 
                                                         LineBreakCategory& lbcCurrent, LineBreakCategory& lbcNext);

            LineBreakType mLineBreakType;
        };


    } // namespace Text

} // namespace EA


#endif // EATEXT_EATEXTBREAK_H














