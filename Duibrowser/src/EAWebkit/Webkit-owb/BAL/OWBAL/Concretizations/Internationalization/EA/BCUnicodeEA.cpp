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

/////////////////////////////////////////////////////////////////////////////
// BCUnicodeEA.cpp
// Written by Paul Pedriana
/////////////////////////////////////////////////////////////////////////////

#include "config.h"
#include "Unicode.h"
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebKitTextInterface.h>

#if defined(EA_PLATFORM_WINDOWS)
    #include <windows.h>    // For the PC only CharUpperBuffW and CharLowerUpperBuffW
#elif defined(EA_PLATFORM_WINCE)
	#include <windows.h>
#endif



namespace WTF {
  namespace Unicode {

    UChar32 toLower(UChar32 c)
    {
        #if defined(EA_PLATFORM_WINDOWS)
            // To do: Use use system services.
            if ((c >= 0x41 && c <= 0x5A) || (c >= 0xC0 && c <= 0xDE))
                return c+0x20;
            else if (c == 0xB5)
                return 0x3BC;
            //+ 1/14/10 CSidhall - Added as a temp solution for PC only as certain characters are
            // not converted correcty (ignored). 
            // Example: in CZECH U+0158. 
            else {
                WCHAR out = (WCHAR) c;
                CharLowerBuffW(&out, 1);
                return (UChar32) out;
            }
            //-CS
            return c;
		#elif defined(EA_PLATFORM_WINCE)
            // To do: Use use system services.
            if ((c >= 0x41 && c <= 0x5A) || (c >= 0xC0 && c <= 0xDE))
                return c+0x20;
            else if (c == 0xB5)
                return 0x3BC;
            //+ 1/14/10 CSidhall - Added as a temp solution for PC only as certain characters are
            // not converted correcty (ignored). 
            // Example: in CZECH U+0158. 
            else {
                WCHAR out = (WCHAR) c;
                CharLowerBuff(&out, 1);
                return (UChar32) out;
            }
            //-CS
            return c;
        #else
            // To do: Use EATextCollation when it is complete.
            if ((c >= 0x41 && c <= 0x5A) || (c >= 0xC0 && c <= 0xDE))
                return c+0x20;
            else if (c == 0xB5)
                return 0x3BC;
            return c;
        #endif
    }


    UChar32 foldCase(UChar32 c)
    {
        return toLower(c);
    }


    int foldCase(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error)
    {
        #if defined(EA_PLATFORM_WINDOWS)
            // To do: Use use system services.
            *error = false;
            if (resultLength < srcLength) {
                *error = true;
                return srcLength;
            }
            for (int i = 0; i < srcLength; ++i)
                result[i] = foldCase(src[i]);

            return srcLength;
		#elif defined(EA_PLATFORM_WINCE)
            // To do: Use use system services.
            *error = false;
            if (resultLength < srcLength) {
                *error = true;
                return srcLength;
            }
            for (int i = 0; i < srcLength; ++i)
                result[i] = foldCase(src[i]);

            return srcLength;
        #else
            // To do: Use EATextCollation when it is complete.
            *error = false;
            if (resultLength < srcLength) {
                *error = true;
                return srcLength;
            }
            for (int i = 0; i < srcLength; ++i)
                result[i] = foldCase(src[i]);

            return srcLength;
        #endif
    }


    int toLower(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error)
    {
        #if defined(EA_PLATFORM_WINDOWS)
            // To do: Use use system services.
            *error = false;
            if (resultLength < srcLength) {
                *error = true;
                return srcLength;
            }
            for (int i = 0; i < srcLength; ++i)
                result[i] = toLower(src[i]);
            return srcLength;
		#elif defined(EA_PLATFORM_WINCE)
            // To do: Use use system services.
            *error = false;
            if (resultLength < srcLength) {
                *error = true;
                return srcLength;
            }
            for (int i = 0; i < srcLength; ++i)
                result[i] = toLower(src[i]);
            return srcLength;
        #else
            // To do: Use EATextCollation when it is complete.
            *error = false;
            if (resultLength < srcLength) {
                *error = true;
                return srcLength;
            }
            for (int i = 0; i < srcLength; ++i)
                result[i] = toLower(src[i]);
            return srcLength;
        #endif
    }


    UChar32 toUpper(UChar32 c)
    {
		#if defined(EA_PLATFORM_WINDOWS)
			// To do: Use use system services.

			if ((c >= 0x61 && c <= 0x7A) || (c >= 0xE0 && c <= 0xFE))
				return c-0x20;
			else if (c == 0x3BC)
				return 0xB5;
			//+ 1/14/10 CSidhall - Added as a temp solution for PC only as certain characters are
			// not converted correcty (ignored). 
			// Example: in CZECH U+0159 when upper cased via style sheet should display U+0158. 
			else {
				WCHAR out = (WCHAR) c;
				CharUpperBuffW(&out, 1);
				return (UChar32) out;
			}
			//-CS
			return c;
		#elif defined(EA_PLATFORM_WINCE)
			// To do: Use use system services.

			if ((c >= 0x61 && c <= 0x7A) || (c >= 0xE0 && c <= 0xFE))
				return c-0x20;
			else if (c == 0x3BC)
				return 0xB5;
			//+ 1/14/10 CSidhall - Added as a temp solution for PC only as certain characters are
			// not converted correcty (ignored). 
			// Example: in CZECH U+0159 when upper cased via style sheet should display U+0158. 
			else {
				WCHAR out = (WCHAR) c;
				CharUpperBuff(&out, 1);
				return (UChar32) out;
			}
			//-CS
			return c;
		#else
			// To do: Use EATextCollation when it is complete.
			if ((c >= 0x61 && c <= 0x7A) || (c >= 0xE0 && c <= 0xFE))
				return c-0x20;
			else if (c == 0x3BC)
				return 0xB5;
			return c;
		#endif
    }


    int toUpper(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error)
    {
        #if defined(EA_PLATFORM_WINDOWS)
            // To do: Use use system services.
            *error = false;
            if (resultLength < srcLength) {
                *error = true;
                return srcLength;
            }
            for (int i = 0; i < srcLength; ++i)
                result[i] = toUpper(src[i]);
            return srcLength;
		#elif defined(EA_PLATFORM_WINCE)
            // To do: Use use system services.
            *error = false;
            if (resultLength < srcLength) {
                *error = true;
                return srcLength;
            }
            for (int i = 0; i < srcLength; ++i)
                result[i] = toUpper(src[i]);
            return srcLength;
        #else
            // To do: Use EATextCollation when it is complete.
            *error = false;
            if (resultLength < srcLength) {
                *error = true;
                return srcLength;
            }
            for (int i = 0; i < srcLength; ++i)
                result[i] = toUpper(src[i]);
            return srcLength;
        #endif
    }


    UChar32 toTitleCase(UChar32 c)
    {
        // It's not correct to use toUpper, but we'll have to do with this for now.
        // EATextCollation.h/cpp doesn't yet support title case, but can be made
        // to do so when needed. Ask Paul Pedriana.

        // Unicode defines three kinds of case mapping: lowercase, uppercase, and
        // titlecase. The difference between uppercasing and titlecasing a character
        // or character sequence can be seen in compound characters (that is,
        // a single character that represents a compount of two characters).
        // 
        // For example, in Unicode, character U+01F3 is LATIN SMALL LETTER DZ. 
        // Let us write this compound character using ASCII as "dz".
        // This character uppercases to character U+01F1, LATIN CAPITAL LETTER DZ, 
        // which is basically "DZ". But it titlecases to to character U+01F2, 
        // LATIN CAPITAL LETTER D WITH SMALL LETTER Z, which we can write "Dz".
        // 
        // character uppercase titlecase
        // --------- --------- ---------
        //    dz        DZ        Dz

        // To do: Use EATextCollation when it is complete.
        // To consider: Allow disabling of this in order to trim functionality, as this is unlikely to be used in most cases.
        return toUpper(c);
    }


    bool isArabicChar(UChar32 c)
    {
        // This is a very simplistic test and doesn't handle alternative forms. But pretty much all Arabic you see will be in this range.
        return (((unsigned)c - 0x0600u) <= (0x06FFu - 0x0600u));
    }


    CharCategory category(UChar32 c)
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


    bool isFormatChar(UChar32 c)
    {
        ////////////////////////////////////////////////////////////////////////////////////
        // http://code.google.com/p/google-caja/wiki/JsControlFormatChars
        // 
        // The Unicode format-control characters (i.e., the characters in category "Cf" in 
        // the Unicode Character Database such as LEFT-TO-RIGHT MARK or RIGHT-TO-LEFT MARK) 
        // are control codes used to control the formatting of a range of text in the 
        // absence of higher-level protocols for this (such as mark-up languages). 
        // It is useful to allow these in source text to facilitate editing and display. 
        // The format control characters can occur anywhere in the source text of an ECMAScript program. 
        // These characters are removed from the source text before applying the lexical grammar. 
        // Since these characters are removed before processing string and regular expression literals, 
        // one must use a. Unicode escape sequence (see 7.6) to include a Unicode format-control 
        // character inside a string or regular expression literal. 
        // 
        // Format chars: 0x00ad, 0x0600-0x0603, 0x070f, 0x17b4-0x17b5, 0x200c-0x200f, 0x202a-0x202e, 0x2060-0x206f, 
        //               0xfeff, 0xfff9-0xfffb, 0x0001d173-0x0001d17a, 0x000e0001, 0x000e0020-0x000e007f. 
        ////////////////////////////////////////////////////////////////////////////////////

        // Test the most common case first. Most characters will be evaluated with two true 'if' evaluations.
        if(c < 0x00AD) // This will usually be true.
            return true;

        // To consider: Make sorted table for the test of the values and do a binary search, 
        //              as it will probably result in faster execution. It will have 36 values
        //              if we ignore values > 0xffff.

        // Test 0x00AD
        if(c < 0x0600) // This will usually be true.
        {
            if(c != 0x00AD) // This will usually be true.
                return false;

            return true;
        }

        // Test 0xfeff, 0xfff9-0xfffb, 0x0001d173-0x0001d17a, 0x000e0001, 0x000e0020-0x000e007f
        if(c > 0x206F) // This will usually be true. Only for uncommon languages will it be false.
        {
            if(c < 0xFEFF)  // This will usually be true.
                return false;

            if((c != 0xFEFF) && ((c - 0xFFF9u) > (0xFFFBu - 0xFFF9u))) // This will usually be true.
            {
                // We could test the values > 0xffff here, but we don't support them and they aren't really used by anybody in practice.
                return false;
            }

            return true;
        }

        // Test 0x0600-0x0603, 0x070f, 0x17b4-0x17b5, 0x200c-0x200f, 0x202a-0x202e, 0x2060-0x206f.
        return (c <= 0x0603) || 
               (c == 0x070f) || 
               ((c - 0x17B5u) <= (0x17B5u - 0x17B4u)) || 
               ((c - 0x200Fu) <= (0x200Fu - 0x200Cu)) || 
               ((c - 0x202Au) <= (0x202Eu - 0x202Au)) || 
               ((c - 0x2060u) <= (0x206Fu - 0x2060u));
    }


    bool isSeparatorSpace(UChar32 c)
    {
        // We don't support chars above 0xffff.
        EA::WebKit::IFontServer*pServer = EA::WebKit::GetFontServer();
        if(!pServer)
            return false;

        const EA::WebKit::CharCategory cc = pServer->GetCharCategory((EA::WebKit::Char)c);
        return (cc == EA::WebKit::kCCSpaceSeparator);
    }


    bool isPrintableChar(UChar32 c)
    {
        //EA_COMPILETIME_ASSERT(EA::Text::kCCSurrogate == 4);  // Just a basic sanity check that the table hasn't been modified.
        
        // We don't support chars above 0xffff.
        EA::WebKit::IFontServer*pServer = EA::WebKit::GetFontServer();
        if(!pServer)
            return false;

        const EA::WebKit::CharCategory cc = pServer->GetCharCategory((EA::WebKit::Char)c);
        return (cc <= EA::WebKit::kCCSurrogate);  // Currently in EAText all the non-printables are enumerated first.
    }

    
    bool isDigit(UChar32 c)
    {
        // We don't support chars above 0xffff.
        EA::WebKit::IFontServer*pServer = EA::WebKit::GetFontServer();
        if(!pServer)
            return false;

        const EA::WebKit::CharCategory cc = pServer->GetCharCategory((EA::WebKit::Char)c);
        return (cc == EA::WebKit::kCCDecimalDigitNumber);
    }


    bool isPunct(UChar32 c)
    {
         // We don't support chars above 0xffff.
        EA::WebKit::IFontServer*pServer = EA::WebKit::GetFontServer();
        if(!pServer)
            return false;
        const EA::WebKit::CharCategory cc = pServer->GetCharCategory((EA::WebKit::Char)c);
        return (cc == EA::WebKit::kCCDashPunctuation
             || cc == EA::WebKit::kCCStartPunctuation
             || cc == EA::WebKit::kCCEndPunctuation
             || cc == EA::WebKit::kCCConnectorPunctuation
             || cc == EA::WebKit::kCCOtherPunctuation
             || cc == EA::WebKit::kCCInitialPunctuation
             || cc == EA::WebKit::kCCFinalPunctuation);
    }


    UChar32 mirroredChar(UChar32 c)
    {
        EA::WebKit::IFontServer*pServer = EA::WebKit::GetFontServer();
        if(!pServer)
            return false;
        return pServer->GetMirrorChar((EA::WebKit::Char)c);
    }


    Direction direction(UChar32 c)
    {
        using namespace WTF::Unicode;
        
        EA::WebKit::IFontServer*pServer = EA::WebKit::GetFontServer();
        if(!pServer)
            return LeftToRight;
       
        const EA::WebKit::BidiClass bidiClass = pServer->GetBidiClass((EA::WebKit::Char)c);

        // The EAText BidiClass is the same thing as the WTF::Unicode::Direction enum, 
        // except that they have a different ordering. We may be able to change the 
        // WTF version to match the EA version, but let's be safe now and not mess
        // with it until later when we have some better confidence.

        switch (bidiClass)
        {
            case EA::WebKit::kBidiClassON:  return OtherNeutral;
            case EA::WebKit::kBidiClassL:   return LeftToRight;
            case EA::WebKit::kBidiClassR:   return RightToLeft;
            case EA::WebKit::kBidiClassAN:  return ArabicNumber;
            case EA::WebKit::kBidiClassEN:  return EuropeanNumber;
            case EA::WebKit::kBidiClassAL:  return RightToLeftArabic;
            case EA::WebKit::kBidiClassNSM: return NonSpacingMark;
            case EA::WebKit::kBidiClassCS:  return CommonNumberSeparator;
            case EA::WebKit::kBidiClassES:  return EuropeanNumberSeparator;
            case EA::WebKit::kBidiClassET:  return EuropeanNumberTerminator;
            case EA::WebKit::kBidiClassBN:  return BoundaryNeutral;
            case EA::WebKit::kBidiClassS:   return SegmentSeparator;
            case EA::WebKit::kBidiClassWS:  return WhiteSpaceNeutral;
            case EA::WebKit::kBidiClassB:   return BlockSeparator;
            case EA::WebKit::kBidiClassRLO: return RightToLeftOverride;
            case EA::WebKit::kBidiClassRLE: return RightToLeftEmbedding;
            case EA::WebKit::kBidiClassLRO: return LeftToRightOverride;
            case EA::WebKit::kBidiClassLRE: return LeftToRightEmbedding;
            case EA::WebKit::kBidiClassPDF: return PopDirectionalFormat;
        }
        return LeftToRight;
    }


    bool isLower(UChar32 c)
    {
         // We don't support chars above 0xffff.
        EA::WebKit::IFontServer*pServer = EA::WebKit::GetFontServer();
        if(!pServer)
            return false;
        
        const EA::WebKit::CharCategory cc = pServer->GetCharCategory((EA::WebKit::Char)c);
        return (cc == EA::WebKit::kCCLowercaseLetter);
    }


    int digitValue(UChar32 c)
    {
        // The proper solution to this is to find out what the zero value is for the 
        // given digit. Ideally all Unicode digits would begin on a 0x10 boundary, but 
        // they don't, and so we can't use 

        // Complicated but most correct solution:
        // Deal with each Unicode digit type one at a time. There are about 20 of them.

        // Less complicated but doesn't work for every language (but does work for Western, Arabic, Thai, Lao, Tibetan, Fullwidth):
        return (c % 0x10);

        // Simplest solution that works for ASCII digits only. On the other hand, 
        // 99% of the time that's what we'll be working with, even if the text 
        // is from another language.
        // return (c - 0x30);
    }


    unsigned char combiningClass(UChar32 c)
    {
        // To do: Make sure that the EAText combining class values are the same as ICU.
        //        They should be so, as EAText follows the Unicode Standard on this.
        EA::WebKit::IFontServer*pServer = EA::WebKit::GetFontServer();
        if(!pServer)
            return false;
        
        const int32_t cc = pServer->GetCombiningClass((EA::WebKit::Char)c);
        return (unsigned char)(uint32_t)cc;
    }


    DecompositionType decompositionType(UChar32 c)
    {
        // To do: Implement this in EATextUnicode.
        // Currently WebKit only uses this to check of a char is DecompositionFont or DecompositionCompat.
        // But if you look at the chars that are Font or Compat, they are unusual chars that we're not 
        // likely to see (with the possible exception of:
        //      00A8          ; Compat # Sk       DIAERESIS
        //      00AF          ; Compat # Sk       MACRON
        //      00B4          ; Compat # Sk       ACUTE ACCENT
        //      00B5          ; Compat # L&       MICRO SIGN
        //      00B8          ; Compat # Sk       CEDILLA
        // So just always return DecompositionNone.

        return WTF::Unicode::DecompositionNone;
    }


    int umemcasecmp(const UChar* a, const UChar* b, int len)
    {
        // Ideally we would case-fold the two strings and then do a char-by-char
        // comparison. But we don't yet have working case folding functionality 
        // in EATextCollation. For many cases the code below will suffice.

        for (int i = 0; i < len; ++i)
        {
            const UChar32 c1 = toLower(a[i]);
            const UChar32 c2 = toLower(b[i]);

            if (c1 != c2)
                return (c1 - c2);
        }

        return 0;
    }

  } // namespace
} // namespace


