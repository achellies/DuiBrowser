/*
 * Copyright (C) 2006 Lars Knoll <lars@trolltech.com>
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

#include "config.h"
#include "TextCodecCasQt.h"
#include "PlatformString.h"
#include "CString.h"

QT_USE_NAMESPACE

namespace WebCore {

void TextCodecCasQt::registerEncodingNames(EncodingNameRegistrar registrar)
{
	const Aliase* allAliases = QTextCodec::availableCodecs();
	for (int i = 0; allAliases[i]; i++)
	{
		QTextCodec *c = QTextCodec::codecForName(allAliases[i]);
		if (c)
		{
			const char *name = c->name();
			registrar(name, name);
			const Aliase* codecAliases = c->aliases();
			for (int k = 0; codecAliases[k]; k++)
			{
				registrar(codecAliases[k], name);
			}
		}
	}
}

static std::auto_ptr<TextCodec> newTextCodecQt(const TextEncoding& encoding, const void*)
{
    return std::auto_ptr<TextCodec>(new TextCodecCasQt(encoding));
}

void TextCodecCasQt::registerCodecs(TextCodecRegistrar registrar)
{
	const Aliase* allAliases = QTextCodec::availableCodecs();
	for (int i = 0; allAliases[i]; i++)
	{
		QTextCodec *c = QTextCodec::codecForName(allAliases[i]);
		if (c)
		{
			registrar(allAliases[i], newTextCodecQt, 0);
		}
	}
}

TextCodecCasQt::TextCodecCasQt(const TextEncoding& encoding)
    : m_encoding(encoding)
{
	m_state.flags = QTextCodec::DefaultConversion | QTextCodec::IgnoreHeader;
    m_state.remainingChars = 0;
    m_state.invalidChars = 0;

    m_codec = QTextCodec::codecForName(m_encoding.name());
}

TextCodecCasQt::~TextCodecCasQt()
{
}


String TextCodecCasQt::decode(const char* bytes, size_t length, bool flush, bool /*stopOnError*/, bool& sawError)
{
    QString unicode = m_codec->toUnicode(bytes, length, &m_state);
    sawError = false;

    if (flush) {
        m_state.flags = QTextCodec::DefaultConversion | QTextCodec::IgnoreHeader;
        m_state.remainingChars = 0;
        m_state.invalidChars = 0;
    }

	return String(reinterpret_cast<const UChar*>(unicode.constData()), unicode.length());
}

CString TextCodecCasQt::encode(const UChar* characters, size_t length, UnencodableHandling)
{
    if (!length)
        return "";

    QByteArray ba = m_codec->fromUnicode(reinterpret_cast<const ushort*>(characters), length, 0);
    return CString(ba.constData(), ba.length());
}

} // namespace WebCore
