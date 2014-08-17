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

#ifndef QUTFCODEC_H
#define QUTFCODEC_H

#include "qtextcodec.h"

QT_BEGIN_NAMESPACE

class QUtf8Codec : public QTextCodec {
public:
    ~QUtf8Codec();

	const char* name() { return "UTF-8"; }

    QString convertToUnicode(const char*, int, ConverterState*);
    QByteArray convertFromUnicode(const ushort*, int, ConverterState*);

    void convertToUnicode(QString *, const char*, int, ConverterState*);
};

class QUtf16Codec : public QTextCodec {
protected:
    enum Endianness {
        Detect,
        BE,
        LE
    };
public:
    QUtf16Codec() { e = Detect; }
    ~QUtf16Codec();

	const char* name() { return "UTF-16"; }
	const Aliase* aliases();

    QString convertToUnicode(const char*, int, ConverterState*);
    QByteArray convertFromUnicode(const ushort*, int, ConverterState*);

protected:
    Endianness e;
};

class QUtf16BECodec : public QUtf16Codec {
public:
    QUtf16BECodec() : QUtf16Codec() { e = BE; }
    const char* name() { return "UTF-16BE"; }
	const Aliase* aliases() { return QTextCodec::aliases(); }
};

class QUtf16LECodec : public QUtf16Codec {
public:
    QUtf16LECodec() : QUtf16Codec() { e = LE; }
    const char* name() { return "UTF-16LE"; }
	const Aliase* aliases() { return QTextCodec::aliases(); }
};

class QUtf32Codec : public QTextCodec {
protected:
    enum Endianness {
        Detect,
        BE,
        LE
    };
public:
    QUtf32Codec() { e = Detect; }
    ~QUtf32Codec();

    const char* name() { return "UTF-32"; }

    QString convertToUnicode(const char*, int, ConverterState*);
    QByteArray convertFromUnicode(const ushort*, int, ConverterState*);

protected:
    Endianness e;
};

class QUtf32BECodec : public QUtf32Codec {
public:
    QUtf32BECodec() : QUtf32Codec() { e = BE; }
    const char* name() { return "UTF-32BE"; }
};

class QUtf32LECodec : public QUtf32Codec {
public:
    QUtf32LECodec() : QUtf32Codec() { e = LE; }
    const char* name() { return "UTF-32LE"; }
};

QT_END_NAMESPACE

#endif // QUTFCODEC_H
