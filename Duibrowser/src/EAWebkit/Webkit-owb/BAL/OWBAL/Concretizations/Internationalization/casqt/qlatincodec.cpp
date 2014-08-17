/* Portions of this code based on QT. */
/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qlatincodec.h"

QT_BEGIN_NAMESPACE

static QString fromLatin1(const char *chars, int len)
{
	if (chars == 0)
        return QString();

	QString r;
	r.resize(len);

	ushort *i = r.data();
    while (len--)
       *i++ = (uchar)*chars++;

	return r;
}

QLatin1Codec::~QLatin1Codec()
{
}

QString QLatin1Codec::convertToUnicode(const char *chars, int len, ConverterState *)
{
    return fromLatin1(chars, len);
}


QByteArray QLatin1Codec::convertFromUnicode(const ushort* ch, int len, ConverterState *state)
{
    const char replacement = (state && state->flags & ConvertInvalidToNull) ? 0 : '?';
    QByteArray r;
    r.resize(len);
    char *d = r.data();
    int invalid = 0;
    for (int i = 0; i < len; ++i) {
        if (ch[i] > 0xff) {
            d[i] = replacement;
            ++invalid;
        } else {
			d[i] = (char)QChar::cell(ch[i]);
        }
    }
    if (state) {
        state->invalidChars += invalid;
    }
    return r;
}

const Aliase* QLatin1Codec::aliases()
{
	if (!m_aliases)
	{
		m_aliases = new Aliase[6];
		m_aliases[0] = "latin1";
		m_aliases[1] = "CP819";
		m_aliases[2] = "IBM819";
		m_aliases[3] = "iso-ir-100";
		m_aliases[4] = "csISOLatin1";
		m_aliases[5] = 0;
	}
	return m_aliases;
}

QLatin15Codec::~QLatin15Codec()
{
}

QString QLatin15Codec::convertToUnicode(const char* chars, int len, ConverterState *)
{
    if (chars == 0)
        return QString();

    QString str = fromLatin1(chars, len);
    ushort *uc = str.data();
    while(len--) {
        switch(*uc) {
            case 0xa4:
                *uc = 0x20ac;
                break;
            case 0xa6:
                *uc = 0x0160;
                break;
            case 0xa8:
                *uc = 0x0161;
                break;
            case 0xb4:
                *uc = 0x017d;
                break;
            case 0xb8:
                *uc = 0x017e;
                break;
            case 0xbc:
                *uc = 0x0152;
                break;
            case 0xbd:
                *uc = 0x0153;
                break;
            case 0xbe:
                *uc = 0x0178;
                break;
            default:
                break;
        }
        uc++;
    }
    return str;
}

QByteArray QLatin15Codec::convertFromUnicode(const ushort* in, int length, ConverterState *state)
{
    const char replacement = (state && state->flags & ConvertInvalidToNull) ? 0 : '?';
    QByteArray r;
    r.resize(length);
    char *d = r.data();
    int invalid = 0;
    for (int i = 0; i < length; ++i) {
        uchar c;
        ushort uc = in[i];
        if (uc < 0x0100) {
            if (uc > 0xa3) {
                switch(uc) {
                case 0xa4:
                case 0xa6:
                case 0xa8:
                case 0xb4:
                case 0xb8:
                case 0xbc:
                case 0xbd:
                case 0xbe:
                    c = replacement;
                    ++invalid;
                    break;
                default:
                    c = (unsigned char) uc;
                    break;
                }
            } else {
                c = (unsigned char) uc;
            }
        } else {
            if (uc == 0x20ac)
                c = 0xa4;
            else if ((uc & 0xff00) == 0x0100) {
                switch(uc) {
                case 0x0160:
                    c = 0xa6;
                    break;
                case 0x0161:
                    c = 0xa8;
                    break;
                case 0x017d:
                    c = 0xb4;
                    break;
                case 0x017e:
                    c = 0xb8;
                    break;
                case 0x0152:
                    c = 0xbc;
                    break;
                case 0x0153:
                    c = 0xbd;
                    break;
                case 0x0178:
                    c = 0xbe;
                    break;
                default:
                    c = replacement;
                    ++invalid;
                }
            } else {
                c = replacement;
                ++invalid;
            }
        }
        d[i] = (char)c;
    }
    if (state) {
        state->remainingChars = 0;
        state->invalidChars += invalid;
    }
    return r;
}

const Aliase* QLatin15Codec::aliases()
{
	if (!m_aliases)
	{
		m_aliases = new Aliase[2];
		m_aliases[0] = "latin9";
		m_aliases[1] = 0;
	}
	return m_aliases;
}

QT_END_NAMESPACE
