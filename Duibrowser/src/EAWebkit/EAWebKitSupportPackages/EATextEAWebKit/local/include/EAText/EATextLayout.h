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
// EATextLayout.h
//
// By Paul Pedriana - 2004
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// *** This file deprecated. ***
//
// It is completely superceded by EATextTypesetter.
///////////////////////////////////////////////////////////////////////////////


#ifndef EATEXT_EATEXTLAYOUT_H
#define EATEXT_EATEXTLAYOUT_H


#include <EABase/EABase.h>
#include <EAText/EAText.h>
#include <EAText/EATextScript.h>
#include <EAText/EATextStyle.h>
#include <EAText/EATextFont.h>
#include <EAText/EATextTypesetter.h>
#include <EAText/EATextIterator.h>
#include <EAText/EATextUnicode.h>
#include <EASTL/vector.h>
#include <stddef.h>
#include <string.h>


namespace EA
{
    namespace Text
    {
        // Forward declarations
        class Font;
        class FontServer;



        /// TextProperties
        ///
        /// Defines properties of a given string of text. This is useful for telling the engine what kind
        /// of text it has to work on and how it might be able to take shortcuts by eliminating steps for
        /// simple text. TextProperties are similar to ScriptProperties. The distinction is that TextProperties
        /// are simply the sum of the ScriptProperties found for a piece of text that possibly has multiple
        /// scripts present. Only those ScriptProperties which affect text properties are represented here.
        ///
        struct TextProperties
        {
            bool mbNumericChars   : 1;  /// True if string has characters that are only digits and the other characters used in writing numbers by the rules of the Unicode bidirectional algorithm. For example, currency symbols, the thousands separator, and the decimal point are classified as numeric when adjacent to or between digits.
            bool mbControlChars   : 1;  /// True if string has characters that are only control characters.
            bool mbAmbiguousChars : 1;  /// True if string has characters that are supported by more than one charset.
            bool mbPrivateChars   : 1;  /// True if string has characters from the private Unicode range (E000 - F8FF). 
            bool mbComplex        : 1;  /// True if string has characters that would require complex layout.
            bool mbLTRChars       : 1;  /// True if string has characters that are categorized as left to right.
            bool mbRTLChars       : 1;  /// True if string has characters that are categorized as right to left.
            bool mbNeutralChars   : 1;  /// True if string has characters that are categorized as directionally neutral.
            bool mbClusters       : 1;  /// True if string has characters that are always clustered (e.g. composed Thai).

            TextProperties()
              : mbNumericChars(false), mbControlChars(false), mbAmbiguousChars(false), mbPrivateChars(false),
                mbComplex(false), mbLTRChars(false), mbRTLChars(false), mbNeutralChars(false), mbClusters(false) { }
        };


        /// TextState
        ///
        /// Defines the state of text before analysis (e.g. at the beginning of a paragraph) 
        /// and for each item (run) in the paragraph. To understand what these values mean, 
        /// you really need to understand the Unicode standard related to bidirectionality. 
        /// See http://www.unicode.org/reports/tr9/. This struct is similar to the Uniscribe
        /// SCRIPT_STATE struct.
        ///
        struct TextState
        {
            int mnLevel                 : 4;   /// Bidirectional embedding level.
            int mbForcedDirection       : 1;   /// True if this run's direction was set by an explicit direction override (LRO or RLO).
            int mbSwapInhibited         : 1;   /// True if character mirroring has been explicitly disabled (by ISS). ISS and ASS are deprecated by the Unicode standard (http://www.unicode.org/reports/tr20/#Deprecated), but some text editors generate these codes and so we support them.
            int mbDisableNationalDigits : 1;   /// True if the use of locale-specific digits is disabled (NaDS, NoDS). NaDS and NoDS are deprecated by the Unicode standard (http://www.unicode.org/reports/tr20/#Deprecated), but some text editors generate these codes and so we support them.
        };


        /// Analysis
        ///
        /// An analysis defines a segment of text of a single direction and script.
        /// The layout pipeline takes a string of text and produces one or more Analysis
        /// objects for it. Plain English text would only ever have one such Analysis, 
        /// but the use of mixed languages or the use of numbers within RTL languages
        /// can produce more than one Analysis objects for the text. An Analysis is 
        /// particularly important in enabling proper reordering of bidirectional text.
        ///
        struct Analysis
        {
            uint32_t  mnPosition;           /// The position in a string of text this analysis applies to.
            uint32_t  mnLength;             /// The length in a string of text that this analysis applies to.
            TextState mTextState;           /// This is analysis info that applies not just to a text run, but also to the layout state at the beginning of the paragraph before the first run.
            Script    mScript        : 7;   /// One of enum Script. For runs that have multiple scripts present, this is the primary script.
            int       mnParity       : 1;   /// Embedding level 'parity'. 0 for LTR, 1 for RTL.
            int       mnParentParity : 1;   /// Embedding level 'parity'. 0 for LTR, 1 for RTL. Parent partity is not necessarily the opposite of parity.
            int       mbJoinBefore   : 1;   /// There is a joiner before this item (run) such as a zero-width joiner.
            int       mbJoinAfter    : 1;   /// There is a joiner after this item (run) such as a zero-width joiner.
        };

        /// AnalysisArray
        ///
        /// To do: Make this use our custom allocator. 
        /// To consider: Convert this to an intrusive linked list. That is, add mpPrev and mpNext 
        ///              pointers to the Analysis struct. This may not be a benefit as it uses more
        ///              memory and usually results in more allocations.
        ///
        typedef eastl::vector<Analysis> AnalysisArray; 



        /// GlyphLayoutAttributes
        ///
        struct GlyphLayoutAttributes // 2 bytes
        {
            GlyphJustificationClass mGJC             : 4;   /// Justification classification
            unsigned                mClusterPosition : 3;   /// Zero-based position of this glyph within its cluster. Standalone glyphs have a value of 0.
            unsigned                mClusterSize     : 3;   /// Size of the cluster this glyph is in. Standalone glyphs have a value of 1.
            unsigned                mbDiacritic      : 1;   /// True if this character is a diacritic of any type.
            unsigned                mbInvisible      : 1;   /// True if this character has nothing to it (e.g. space, tab, zero-width).
            unsigned                mbZeroWidth      : 1;   /// ZWSP, ZWNBSP, ZWJ, ZWNJ, or anything with no width. If this is true, then mbInvisible will be true also.
        };


        /// GlyphAttributes
        ///
        /// Provides information for an individual glyph that is layed out or in a display list.
        ///
        /// To do: Find a way to compress this data, perhaps by reducing the font pointer to 
        ///        a 16 bit font id and using some form of compression on the floating point
        ///        coordinate values, perhaps to 12.4 fixed point. This struct could be packed
        ///        down to 20 bytes or less. Note that it is not terrible that this struct be
        ///        so big, as it is usually a temporary intermediate struct and there usually
        ///        aren't any more than a hundred or so of them in an array.
        ///
        struct GlyphAttributes // 36 bytes
        {
            Font*                   mpFont;         /// Font. If this is stored on disk as a persisted object, it holds the numerical unique font id. Ideally this member would be of type uintptr_t, but that would make it less readable.
            float                   mfPenX;         /// Pen position, used for display lists of layed out text. When GlyphAttributes applies to an individual glyph, this is zero. This field is more or less required for fonts which are not based on bitmaps, such as polygonal fonts or vector fonts. The pen position is the local coordinate origin for the glyph.
            float                   mfPenY;         /// When GlyphAttributes applies to an individual glyph, this is zero.
            float                   mfX1;           /// Same as x bearing: x position of top-left corner. For individual glyphs the position is relative to the pen position; for layed out display lists it is in absolute coordinates.
            float                   mfY1;           /// Same as y bearing: y position of top-left corner.
            float                   mfX2;           /// X position of bottom-right corner. We use x2 instead of width  because the former maps to what is done with the value better.
            float                   mfY2;           /// Y position of bottom-right corner. We use y2 instead of height because the former maps to what is done with the value better.
            float                   mfAdvance;      /// Distance along x (horizontal text) or y (vertical text) to next glyph.
            GlyphLayoutAttributes   mGLA;           /// Glyph layout attributes. Defines basic properties of glyphs needed for layout.
            GlyphId                 mGlyphId;       /// Maps to a glyph id in the font. Not usually the same as Unicode value.
        };

        struct GlyphDisplayEntry : public GlyphAttributes { };


        /// LayoutMetrics
        ///
        /// Defines metrics of layed out text. These are useful for knowing the space used by 
        /// layed out text. These layout metrics are coordinate-system-orientation-independent.
        /// Thus, the mfBaseline value for a char like 'A' is always positive for both y-down
        /// and y-up coordinate systems, and mfDescent for a char like 'g' is always negative.
        ///
        struct LayoutMetrics
        {
            uint32_t mnSize;          /// Number of layed out Unicode characters.
            float    mfSpace;         /// Space used by layed out Unicode characters, defined by final pen position. For line layout, this is the end of the line; for paragraph layout, this is the height of the paragraph for horizontal layout or the width of the paragraph for vertical layout.
            uint32_t mnVisibleSize;   /// Number of layout out Unicode characters to the last visible one. Trailing invisible characters are not included.
            float    mfVisibleSpace;  /// Space used by the mnVisibleSize characters, defined by pen position of first trailing invisible character. For paragraph layout, this will be equal to mfSpace.
            float    mfBaseline;      /// Equal to max of FontMetrics::mfBaseline for all fonts represented on the line. The entire line height is the baseline minus (because descent is negative) the descent.
            float    mfDescent;       /// Equal to min of FontMetrics::mfDescent for all fonts represented on the line. Recall that lower descents mean lower values.
            uint32_t mnLineCount;     /// Number of lines resulting from the layout.
        };



        ///////////////////////////////////////////////////////////////////////
        /// class Layout
        ///
        /// Implements text layout.
        ///       
        class Layout
        {
        public:
            Layout();
           ~Layout();


         protected:
            LayoutSettings mLayoutSettings;

        }; // class Layout


    } // namespace Text


} // namespace EA


#endif // EATEXT_EATEXTLAYOUT_H










