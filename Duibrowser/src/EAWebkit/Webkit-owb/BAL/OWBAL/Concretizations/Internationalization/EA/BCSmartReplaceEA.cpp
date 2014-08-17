/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
 * Copyright (C) 2008 Tony Chang <idealisms@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#include "config.h"
#include "SmartReplace.h"
#include "BALBase.h"
#include "stdio.h"



namespace WebCore {


// Note by Paul Pedriana: This function is undocumented by Apple/WebKit, but it seems to be
// a function which decides if a space char is not automatically appended (is exempt) after 
// pasted in text. This system is a matter of user convenience and not HTML/Unicode correctness.
//
bool isCharacterSmartReplaceExempt(UChar32 c, bool isPreviousCharacter)
{
    // To do: Disable this entire function for embedded platforms, as it represents merely user convenience code.
    // To do: Disable the Asian versions of Asian support is #defined away by config option we invent. - Paul Pedriana

    if((c - 0x000Au) <= 3u)             // Whitespace
        return true;

    if((c - 0x1100u) <= 256u)           // Hangul Jamo (0x1100 - 0x11FF)
        return true;

    if((c - 0x2E80u) <= 352u)           // CJK & Kangxi Radicals (0x2E80 - 0x2FDF)
        return true;

    if((c - 0x2FF0u) <= 464u)           // Ideograph Descriptions, CJK Symbols, Hiragana, Katakana, Bopomofo, Hangul Compatibility Jamo, Kanbun, & Bopomofo Ext (0x2FF0 - 0x31BF)
        return true;

    if((c - 0x3200u) <= 29392u)         // Enclosed CJK, CJK Ideographs (Uni Han & Ext A), & Yi (0x3200 - 0xA4CF)
        return true;

    if((c - 0xAC00u) <= 11183u)         // Hangul Syllables (0xAC00 - 0xD7AF)
        return true;

    // Disbled because it's uncommon:
    // if((c - 0xF900u) <= 352u)        // CJK Compatibility Ideographs (0xF900 - 0xFA5F)
    //     return true;

    // Disbled because it's uncommon:
    // if((c - 0xFE30u) <= 32u)         // CJK Compatibility From (0xFE30 - 0xFE4F)
    //     return true;

    if((c - 0xFF00u) <= 240u)           // Half/Full Width Form (0xFF00 - 0xFFEF)
        return true;

    // Disbled because it's uncommon:
    // if((c - 0x20000u) <= 0xA6D7u)    // CJK Ideograph Exntension B
    //     return true;

    // Disbled because it's uncommon:
    // if((c - 0x2F800u) <= 0x021Eu)    // CJK Compatibility Ideographs (0x2F800 - 0x2FA1D)
    //     return true;

    if(isPreviousCharacter)
    {
        // To do: Return true if it's one of these:
        //    ([\"\'#$/-`{
    }
    else
    {
        // To do: Return true if it's one of these:
        //    )].,;:?\'!\"%*-/}
        //    <Unicode punctuation characters>
    }

    return false;
}

} // namespace




