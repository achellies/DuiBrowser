/*
 * Copyright (C) 2004, 2006, 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Alexey Proskuryakov <ap@nypop.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#include "config.h"
#include "TextEncoding.h"

#include "CString.h"
#include "PlatformString.h"
#include "TextCodec.h"
#include "TextDecoder.h"
#include "TextEncodingRegistry.h"
#if USE(ICU_UNICODE)
#include <unicode/unorm.h>
#elif USE(QT4_UNICODE)
#include <QString>
#endif
#include <wtf/HashSet.h>
#include <wtf/OwnPtr.h>

namespace OWBAL {

static void addEncodingName(HashSet<const char*>& set, const char* name)
{
    const char* atomicName = atomicCanonicalTextEncodingName(name);
    if (atomicName)
        set.add(atomicName);
}

TextEncoding::TextEncoding(const char* name)
    : m_name(atomicCanonicalTextEncodingName(name))
{
}

TextEncoding::TextEncoding(const String& name)
    : m_name(atomicCanonicalTextEncodingName(name.characters(), name.length()))
{
}

String TextEncoding::decode(const char* data, size_t length, bool stopOnError, bool& sawError) const
{
    if (!m_name)
        return String();

    return TextDecoder(*this).decode(data, length, true, stopOnError, sawError);
}

CString TextEncoding::encode(const UChar* characters, size_t length, UnencodableHandling handling) const
{
    if (!m_name)
        return CString();

    if (!length)
        return "";

#if USE(ICU_UNICODE) && !USE(BALI18N)
    // FIXME: What's the right place to do normalization?
    // It's a little strange to do it inside the encode function.
    // Perhaps normalization should be an explicit step done before calling encode.

    const UChar* source = characters;
    size_t sourceLength = length;

    Vector<UChar> normalizedCharacters;

    UErrorCode err = U_ZERO_ERROR;
    if (unorm_quickCheck(source, sourceLength, UNORM_NFC, &err) != UNORM_YES) {
        // First try using the length of the original string, since normalization to NFC rarely increases length.
        normalizedCharacters.grow(sourceLength);
        int32_t normalizedLength = unorm_normalize(source, length, UNORM_NFC, 0, normalizedCharacters.data(), length, &err);
        if (err == U_BUFFER_OVERFLOW_ERROR) {
            err = U_ZERO_ERROR;
            normalizedCharacters.resize(normalizedLength);
            normalizedLength = unorm_normalize(source, length, UNORM_NFC, 0, normalizedCharacters.data(), normalizedLength, &err);
        }
        ASSERT(U_SUCCESS(err));

        source = normalizedCharacters.data();
        sourceLength = normalizedLength;
    }
    return newTextCodec(*this)->encode(source, sourceLength, handling);
#elif USE(QT4_UNICODE)
    QString str(reinterpret_cast<const QChar*>(characters), length);
    str = str.normalized(QString::NormalizationForm_C);
    return newTextCodec(*this)->encode(reinterpret_cast<const UChar *>(str.utf16()), str.length(), handling);
#elif USE(ICU_UNICODE) && USE(BALI18N)
    //FIXME
    //return newTextCodec(*this)->encode(characters, length, handling);
#elif USE(CASQT_UNICODE)
	// added by achellies
    //OwnPtr<TextCodec> textCodec = newTextCodec(*this);
    return newTextCodec(*this)->encode(characters, length, handling);
#endif
}

bool TextEncoding::usesVisualOrdering() const
{
    if (noExtendedTextEncodingNameUsed())
        return false;

    static const char* const a = atomicCanonicalTextEncodingName("ISO-8859-8");
    return m_name == a;
}

//+daw ca 24/07 static and global management
int TextEncoding::m_st_iIsEmpty = 1;


HashSet<const char*>* setList()
{
    static HashSet<const char*> set;
    if (set.isEmpty()) 
    {
        TextEncoding::m_st_iIsEmpty = 0;
        addEncodingName(set, "x-mac-japanese");
        addEncodingName(set, "cp932");
        addEncodingName(set, "JIS_X0201");
        addEncodingName(set, "JIS_X0208-1983");
        addEncodingName(set, "JIS_X0208-1990");
        addEncodingName(set, "JIS_X0212-1990");
        addEncodingName(set, "JIS_C6226-1978");
        addEncodingName(set, "Shift_JIS_X0213-2000");
        addEncodingName(set, "ISO-2022-JP");
        addEncodingName(set, "ISO-2022-JP-2");
        addEncodingName(set, "ISO-2022-JP-1");
        addEncodingName(set, "ISO-2022-JP-3");
        addEncodingName(set, "EUC-JP");
        addEncodingName(set, "Shift_JIS");
    }
    return &set;
}


bool TextEncoding::isJapanese() const
{
    if (noExtendedTextEncodingNameUsed())
        return false;

    return m_name && setList()->contains(m_name);
}

static TextEncoding* pGlobalASCIIEncoding = NULL;
static TextEncoding* pGlobalLatin1Encoding = NULL;
static TextEncoding* pGlobalUTF16BigEndianEncoding = NULL;
static TextEncoding* pGlobalUTF16LittleEndianEncoding = NULL;
static TextEncoding* pGlobalUTF32LittleEndianEncoding = NULL;
static TextEncoding* pGlobalUTF32BigEndianEncoding = NULL;
static TextEncoding* pGlobalUTF8Encoding = NULL;
static TextEncoding* pGlobalWindowsLatin1Encoding = NULL;

void TextEncoding::staticFinalize()
{
    if (!TextEncoding::m_st_iIsEmpty)
    {
        setList()->clear();
        TextEncoding::m_st_iIsEmpty = 1;
    }
    releaseBaseTextCodecMaps();

    delete pGlobalASCIIEncoding;
    pGlobalASCIIEncoding = NULL;

    delete pGlobalLatin1Encoding;
    pGlobalLatin1Encoding = NULL;

    delete pGlobalUTF16BigEndianEncoding;
    pGlobalUTF16BigEndianEncoding = NULL;

    delete pGlobalUTF16LittleEndianEncoding;
    pGlobalUTF16LittleEndianEncoding = NULL;

    delete pGlobalUTF32LittleEndianEncoding;
    pGlobalUTF32LittleEndianEncoding = NULL;

    delete pGlobalUTF32BigEndianEncoding;
    pGlobalUTF32BigEndianEncoding = NULL;

    delete pGlobalUTF8Encoding;
    pGlobalUTF8Encoding = NULL;

    delete pGlobalWindowsLatin1Encoding;
    pGlobalWindowsLatin1Encoding = NULL;
}

UChar TextEncoding::backslashAsCurrencySymbol() const
{
    if (noExtendedTextEncodingNameUsed())
        return '\\';

    // The text encodings below treat backslash as a currency symbol.
    // See http://blogs.msdn.com/michkap/archive/2005/09/17/469941.aspx for more information.
    static const char* const a = atomicCanonicalTextEncodingName("Shift_JIS_X0213-2000");
    static const char* const b = atomicCanonicalTextEncodingName("EUC-JP");
    return (m_name == a || m_name == b) ? 0x00A5 : '\\';
}

const TextEncoding& TextEncoding::closest8BitEquivalent() const
{
    if (*this == UTF16BigEndianEncoding() || *this == UTF16LittleEndianEncoding())
        return UTF8Encoding();
    return *this;
}

const TextEncoding& ASCIIEncoding()
{
    if (!pGlobalASCIIEncoding)
    {
        pGlobalASCIIEncoding = new TextEncoding("ASCII");
    }
    return *pGlobalASCIIEncoding;
}

const TextEncoding& Latin1Encoding()
{
    if (!pGlobalLatin1Encoding)
    {
        pGlobalLatin1Encoding = new TextEncoding("Latin-1");
    }
    return *pGlobalLatin1Encoding;
}

const TextEncoding& UTF16BigEndianEncoding()
{
    if (!pGlobalUTF16BigEndianEncoding)
    {
        pGlobalUTF16BigEndianEncoding = new TextEncoding("UTF-16BE");
    }
    return *pGlobalUTF16BigEndianEncoding;
}

const TextEncoding& UTF16LittleEndianEncoding()
{
    if (!pGlobalUTF16LittleEndianEncoding)
    {
        pGlobalUTF16LittleEndianEncoding = new TextEncoding("UTF-16LE");
    }
    return *pGlobalUTF16LittleEndianEncoding;
}

const TextEncoding& UTF32BigEndianEncoding()
{
    if (!pGlobalUTF32BigEndianEncoding)
    {
        pGlobalUTF32BigEndianEncoding = new TextEncoding("UTF-32BE");
    }
    return *pGlobalUTF32BigEndianEncoding;
}

const TextEncoding& UTF32LittleEndianEncoding()
{
    if (!pGlobalUTF32LittleEndianEncoding)
    {
        pGlobalUTF32LittleEndianEncoding = new TextEncoding("UTF-32LE");
    }
    return *pGlobalUTF32LittleEndianEncoding;
}


const TextEncoding& UTF8Encoding()
{
    if (!pGlobalUTF8Encoding)
    {
        pGlobalUTF8Encoding = new TextEncoding("UTF-8");
    }
    return *pGlobalUTF8Encoding;
}

const TextEncoding& WindowsLatin1Encoding()
{
    if (!pGlobalWindowsLatin1Encoding)
    {
        pGlobalWindowsLatin1Encoding = new TextEncoding("WinLatin-1");
    }
    return *pGlobalLatin1Encoding;
}

} // namespace WebCore
