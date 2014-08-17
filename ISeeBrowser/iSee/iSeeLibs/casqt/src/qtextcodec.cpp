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

#include "qtextcodec.h"

#include "qutfcodec.h"
#include "qsimplecodec.h"
#include "qlatincodec.h"
#include "qtsciicodec.h"
#include "qisciicodec.h"
#include "qgb18030codec.h"
#include "qeucjpcodec.h"
#include "qjiscodec.h"
#include "qsjiscodec.h"
#include "qeuckrcodec.h"
#include "qbig5codec.h"

#include <list>

QT_BEGIN_NAMESPACE

static std::list<QTextCodec*> *all = 0;
static bool destroying_is_ok = false;
static QTextCodec *localeMapper = 0;
static Aliase* allCodecNames = 0;

static void setup()
{
    if (all)
        return;

    if (destroying_is_ok)
        qWarning("QTextCodec: Creating new codec during codec cleanup");
	all = new std::list<QTextCodec*>;

    (void)new QTsciiCodec;

    for (int i = 0; i < 9; ++i)
        (void)new QIsciiCodec(i);

    // no asian codecs when bootstrapping, sorry
    (void)new QGb18030Codec;
    (void)new QGbkCodec;
    (void)new QGb2312Codec;
    (void)new QEucJpCodec;
    (void)new QJisCodec;
    (void)new QSjisCodec;
    (void)new QEucKrCodec;
    (void)new QBig5Codec;
    (void)new QBig5hkscsCodec;

    // (void)new QWindowsLocalCodec; // FIXME:

    (void)new QUtf16Codec;
    (void)new QUtf16BECodec;
    (void)new QUtf16LECodec;
    (void)new QUtf32Codec;
    (void)new QUtf32BECodec;
    (void)new QUtf32LECodec;
    (void)new QLatin15Codec;
    (void)new QLatin1Codec;
    (void)new QUtf8Codec;

    for (int i = 0; i < QSimpleTextCodec::numSimpleCodecs; ++i)
        (void)new QSimpleTextCodec(i);
}

class QTextCodecCleanup
{
public:
	QTextCodecCleanup() { setup(); }
    ~QTextCodecCleanup();
};

/*
    Deletes all the created codecs. This destructor is called just
    before exiting to delete any QTextCodec objects that may be lying
    around.
*/
QTextCodecCleanup::~QTextCodecCleanup()
{
    if (!all)
        return;

    destroying_is_ok = true;

	for (std::list<QTextCodec*>::iterator i = all->begin(); i != all->end(); i++)
	{
		(*i)->delete_();
	}
	all->clear();
    delete all;
    all = 0;
    localeMapper = 0;

    destroying_is_ok = false;
}

static QTextCodecCleanup this_createQTextCodecCleanup;

QTextCodec::QTextCodec() : m_aliases(0)
{
	setup();
    all->push_back(this);
}

QTextCodec::~QTextCodec()
{
	if (!destroying_is_ok)
        qWarning("QTextCodec::~QTextCodec: Called by application");
#if 0
    if (all)
        all->remove(this);
#endif

	if (m_aliases)
		delete []m_aliases;
}

const Aliase* QTextCodec::aliases()
{
	if (!m_aliases)
	{
		m_aliases = new Aliase[1];
		m_aliases[0] = 0;
	}
	return m_aliases;
}

static bool nameMatch(const char* name, const char* test)
{
    // if they're the same, return a perfect score
    if (qstricmp(name, test) == 0)
        return true;

    const char *n = name;
    const char *h = test;

    // if the letters and numbers are the same, we have a match
    while (*n != '\0') {
        if (isalnum((uchar)*n)) {
            for (;;) {
                if (*h == '\0')
                    return false;
                if (isalnum((uchar)*h))
                    break;
                ++h;
            }
            if (tolower((uchar)*n) != tolower((uchar)*h))
                return false;
            ++h;
        }
        ++n;
    }
    while (*h && !isalnum((uchar)*h))
           ++h;
    return (*h == '\0');
}

const Aliase* QTextCodec::availableCodecs()
{
	if (!allCodecNames)
	{
		int cnt = 0;
		allCodecNames = new Aliase[all->size() + 1];
		for (std::list<QTextCodec*>::iterator i = all->begin(); i != all->end(); i++, cnt++)
		{
			allCodecNames[cnt] = (*i)->name();
		}
		allCodecNames[cnt] = 0;
	}
	return allCodecNames;
}

QTextCodec* QTextCodec::codecForName(const char *name)
{
	if (name == 0 || *name == 0)
        return 0;

    setup();

    for (std::list<QTextCodec*>::iterator i = all->begin(); i != all->end(); i++) {
        QTextCodec *cursor = *i;
        if (nameMatch(cursor->name(), name))
            return cursor;
        const Aliase* aliases = cursor->aliases();
        for (int i = 0; aliases[i]; ++i)
            if (nameMatch(aliases[i], name))
                return cursor;
    }

    return 0;
}

QTextCodec* QTextCodec::codecForLocale()
{
	return 0;
}

QT_END_NAMESPACE
