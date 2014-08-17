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
// BCUnicodeEA.h
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#ifndef BCUNICODEEA_H
#define BCUNICODEEA_H

#include "config.h"
#include <stdlib.h>

#include <unicode/uchar.h>
#include <unicode/ustring.h>
#include <unicode/utf16.h>



namespace WTF {
  namespace Unicode {

    enum Direction {
      LeftToRight = U_LEFT_TO_RIGHT,
      RightToLeft = U_RIGHT_TO_LEFT,
      EuropeanNumber = U_EUROPEAN_NUMBER,
      EuropeanNumberSeparator = U_EUROPEAN_NUMBER_SEPARATOR,
      EuropeanNumberTerminator = U_EUROPEAN_NUMBER_TERMINATOR,
      ArabicNumber = U_ARABIC_NUMBER,
      CommonNumberSeparator = U_COMMON_NUMBER_SEPARATOR,
      BlockSeparator = U_BLOCK_SEPARATOR,
      SegmentSeparator = U_SEGMENT_SEPARATOR,
      WhiteSpaceNeutral = U_WHITE_SPACE_NEUTRAL,
      OtherNeutral = U_OTHER_NEUTRAL,
      LeftToRightEmbedding = U_LEFT_TO_RIGHT_EMBEDDING,
      LeftToRightOverride = U_LEFT_TO_RIGHT_OVERRIDE,
      RightToLeftArabic = U_RIGHT_TO_LEFT_ARABIC,
      RightToLeftEmbedding = U_RIGHT_TO_LEFT_EMBEDDING,
      RightToLeftOverride = U_RIGHT_TO_LEFT_OVERRIDE,
      PopDirectionalFormat = U_POP_DIRECTIONAL_FORMAT,
      NonSpacingMark = U_DIR_NON_SPACING_MARK,
      BoundaryNeutral = U_BOUNDARY_NEUTRAL
    };

    enum DecompositionType {
      DecompositionNone = U_DT_NONE,
      DecompositionCanonical = U_DT_CANONICAL,
      DecompositionCompat = U_DT_COMPAT,
      DecompositionCircle = U_DT_CIRCLE,
      DecompositionFinal = U_DT_FINAL,
      DecompositionFont = U_DT_FONT,
      DecompositionFraction = U_DT_FRACTION,
      DecompositionInitial = U_DT_INITIAL,
      DecompositionIsolated = U_DT_ISOLATED,
      DecompositionMedial = U_DT_MEDIAL,
      DecompositionNarrow = U_DT_NARROW,
      DecompositionNoBreak = U_DT_NOBREAK,
      DecompositionSmall = U_DT_SMALL,
      DecompositionSquare = U_DT_SQUARE,
      DecompositionSub = U_DT_SUB,
      DecompositionSuper = U_DT_SUPER,
      DecompositionVertical = U_DT_VERTICAL,
      DecompositionWide = U_DT_WIDE,
    };

    enum CharCategory {
      NoCategory =  0,
      Other_NotAssigned = U_MASK(U_GENERAL_OTHER_TYPES),
      Letter_Uppercase = U_MASK(U_UPPERCASE_LETTER),
      Letter_Lowercase = U_MASK(U_LOWERCASE_LETTER),
      Letter_Titlecase = U_MASK(U_TITLECASE_LETTER),
      Letter_Modifier = U_MASK(U_MODIFIER_LETTER),
      Letter_Other = U_MASK(U_OTHER_LETTER),

      Mark_NonSpacing = U_MASK(U_NON_SPACING_MARK),
      Mark_Enclosing = U_MASK(U_ENCLOSING_MARK),
      Mark_SpacingCombining = U_MASK(U_COMBINING_SPACING_MARK),

      Number_DecimalDigit = U_MASK(U_DECIMAL_DIGIT_NUMBER),
      Number_Letter = U_MASK(U_LETTER_NUMBER),
      Number_Other = U_MASK(U_OTHER_NUMBER),

      Separator_Space = U_MASK(U_SPACE_SEPARATOR),
      Separator_Line = U_MASK(U_LINE_SEPARATOR),
      Separator_Paragraph = U_MASK(U_PARAGRAPH_SEPARATOR),

      Other_Control = U_MASK(U_CONTROL_CHAR),
      Other_Format = U_MASK(U_FORMAT_CHAR),
      Other_PrivateUse = U_MASK(U_PRIVATE_USE_CHAR),
      Other_Surrogate = U_MASK(U_SURROGATE),

      Punctuation_Dash = U_MASK(U_DASH_PUNCTUATION),
      Punctuation_Open = U_MASK(U_START_PUNCTUATION),
      Punctuation_Close = U_MASK(U_END_PUNCTUATION),
      Punctuation_Connector = U_MASK(U_CONNECTOR_PUNCTUATION),
      Punctuation_Other = U_MASK(U_OTHER_PUNCTUATION),

      Symbol_Math = U_MASK(U_MATH_SYMBOL),
      Symbol_Currency = U_MASK(U_CURRENCY_SYMBOL),
      Symbol_Modifier = U_MASK(U_MODIFIER_SYMBOL),
      Symbol_Other = U_MASK(U_OTHER_SYMBOL),

      Punctuation_InitialQuote = U_MASK(U_INITIAL_PUNCTUATION),
      Punctuation_FinalQuote = U_MASK(U_FINAL_PUNCTUATION)
    };

    UChar32 toLower(UChar32 c);
    int toLower(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error);
    UChar32 toUpper(UChar32 c);
    int toUpper(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error);
    UChar32 foldCase(UChar32 c);
    int foldCase(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error);
    UChar32 toTitleCase(UChar32 c);
    bool isFormatChar(UChar32 c);
    bool isSeparatorSpace(UChar32 c);
    bool isPrintableChar(UChar32 c);
    bool isDigit(UChar32 c);
    bool isPunct(UChar32 c);
    UChar32 mirroredChar(UChar32 c);
    CharCategory category(UChar32 c);
    Direction direction(UChar32 c);
    bool isLower(UChar32 c);
    bool isUpper(UChar32 c);
    int digitValue(UChar32 c);
    uint8_t combiningClass(UChar32 c);
    DecompositionType decompositionType(UChar32 c);
    int umemcasecmp(const UChar* a, const UChar* b, int len);
    bool isArabicChar(UChar32 c);
    /*inline UChar32 toLower(UChar32 c)
    {
        if ((c >= 0x41 && c <= 0x5A) || (c >= 0xC0 && c <= 0xDE))
            return c+0x20;
        else if (c == 0xB5)
            return 0x3BC;
        return c;
    }

    inline UChar32 foldCase(UChar32 c)
    {
        return toLower(c);
    }

    inline int foldCase(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error)
    {
        *error = false;
        if (resultLength < srcLength) {
            *error = true;
            return srcLength;
        }
        for (int i = 0; i < srcLength; ++i)
            result[i] = foldCase(src[i]);
        return srcLength;
    }

    inline int toLower(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error)
    {
        *error = false;
        if (resultLength < srcLength) {
            *error = true;
            return srcLength;
        }
        for (int i = 0; i < srcLength; ++i)
            result[i] = toLower(src[i]);
        return srcLength;
    }

    inline UChar32 toUpper(UChar32 c)
    {
        if ((c >= 0x61 && c <= 0x7A) || (c >= 0xE0 && c <= 0xFE))
            return c-0x20;
        else if (c == 0x3BC)
            return 0xB5;
        return c;
    }

    inline int toUpper(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error)
    {
        *error = false;
        if (resultLength < srcLength) {
            *error = true;
            return srcLength;
        }
        for (int i = 0; i < srcLength; ++i)
            result[i] = toUpper(src[i]);
        return srcLength;
    }

    inline UChar32 toTitleCase(UChar32 c)
    {
        return c;
    }

    inline bool isArabicChar(UChar32 c)
    {
        BalNotImplemented();
        return false;
    }

    inline CharCategory category(UChar32 c)
    {
        if (c == 0x0D)
            return WTF::Unicode::Separator_Line;
        else if (c < 0x20)
            return WTF::Unicode::NoCategory;
        else if (c == 0x20)
            return WTF::Unicode::Separator_Space;
        else if ((c >= 0x21 && c<=0x27)
                || c == 0x2E
                || (c >= 0x3A && c<=0x3B)
                || c == 0x3F
                || c == 0x40
                || (c >= 0x5B && c<=0x60)
                || (c >= 0x7B && c<=0x7E))
            return WTF::Unicode::Punctuation_Other;
        else if (c == 0x28)
            return WTF::Unicode::Punctuation_Open;
        else if (c == 0x29)
            return WTF::Unicode::Punctuation_Close;
        else if ((c >= 0x2A && c<=0x2F) || (c >= 0x3C && c<=0x3E))
            return WTF::Unicode::Symbol_Math;
        else if (c >= 0x30 && c<=0x39)
            return WTF::Unicode::Number_DecimalDigit;
        else if (c >= 0x41 && c<=0x5A)
            return WTF::Unicode::Letter_Uppercase;
        else if (c >= 0x61 && c<=0x7A)
            return WTF::Unicode::Letter_Lowercase;
        else
            return WTF::Unicode::NoCategory;
    }


    inline bool isFormatChar(UChar32 c)
    {
        return (c & 0xffff0000) == 0 && category(c) == WTF::Unicode::Other_Format;
    }

    inline bool isSeparatorSpace(UChar32 c)
    {
        return (c & 0xffff0000) == 0 && category(c) == WTF::Unicode::Separator_Space;
    }

    inline bool isPrintableChar(UChar32 c)
    {
        BalNotImplemented();
        return false;
    }
    
    inline bool isDigit(UChar32 c)
    {
        return (c & 0xffff0000) == 0 && category(c) == WTF::Unicode::Number_DecimalDigit;
    }

    inline bool isPunct(UChar32 c)
    {
        return (c & 0xffff0000) == 0 && (category(c) == WTF::Unicode::Punctuation_Dash
                                    || category(c) == WTF::Unicode::Punctuation_Open
                                    || category(c) == WTF::Unicode::Punctuation_Close
                                    || category(c) == WTF::Unicode::Punctuation_Connector
                                    || category(c) == WTF::Unicode::Punctuation_Other
                                    || category(c) == WTF::Unicode::Punctuation_InitialQuote
                                    || category(c) == WTF::Unicode::Punctuation_FinalQuote);
    }

    inline UChar32 mirroredChar(UChar32 c)
    {
        BalNotImplemented();
        return c;
    }

    inline Direction direction(UChar32 c)
    {
        BalNotImplemented();
        return WTF::Unicode::LeftToRight;
    }

    inline bool isLower(UChar32 c)
    {
        return (c & 0xffff0000) == 0 && category(c) == WTF::Unicode::Letter_Lowercase;
    }

    inline int digitValue(UChar32 c)
    {
        return c-0x30;
    }

    inline unsigned char combiningClass(UChar32 c)
    {
        BalNotImplemented();
        return c;
    }

    inline DecompositionType decompositionType(UChar32 c)
    {
        BalNotImplemented();
        return WTF::Unicode::DecompositionNone;
    }

    inline int umemcasecmp(const UChar* a, const UChar* b, int len)
    {
        for (int i = 0; i < len; ++i) {
            UChar32 c1 = toLower(a[i]);
            UChar32 c2 = toLower(b[i]);
            if (c1 != c2)
                return c1 < c2;
        }
        return 0;
    }*/

  }
}

#endif
// vim: ts=2 sw=2 et
