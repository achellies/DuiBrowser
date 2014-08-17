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

#include "qstring.h"
#include "qtextcodec.h"

#include "qchar.cpp"
#include "qbytearray.cpp"
#include "qstringmatcher.cpp"
#include "qurl.cpp"

QT_BEGIN_NAMESPACE

// QString
QString::Data QString::shared_null = { 1, 0, 0, shared_null.array, {0} };
QString::Data QString::shared_empty = { 1, 0, 0, shared_empty.array, {0} };

QString::QString(const ushort *s, int len)
: d(&shared_null)
{	
	(void)append(s, len);
}

int QString::grow(int size)
{
    return qAllocMore(size * sizeof(ushort), sizeof(Data)) / sizeof(ushort);
}

void QString::reallocData(int alloc)
{
	if (d->size == 0 || d->ref != 1 || d->data != d->array) {
        Data *x = static_cast<Data *>(qMalloc(sizeof(Data) + alloc * sizeof(ushort)));
        if (!x)
            return;
        x->size = qMin(alloc, d->size);
        ::memcpy(x->array, d->data, x->size * sizeof(ushort));
        x->array[x->size] = 0;
        x->ref = 1;
        x->alloc = alloc;
        x->data = x->array;
		delete_();
        d = x;
    } else {
        Data *x = static_cast<Data *>(qRealloc(d, sizeof(Data) + alloc * sizeof(ushort)));
        if (!x)
            return;
        x->alloc = alloc;
        x->data = x->array;
        d = x;
    }
}

QString &QString::append(const ushort *s, int len)
{
	if (s) {
		if (len == -1) len = qwcslen(s);
		if (d->ref != 1 || d->size + len > d->alloc)
            reallocData(grow(d->size + len));
        memcpy(d->data + d->size, s, len * sizeof(ushort));
        d->size += len;
        d->data[d->size] = '\0';
    }
    return *this;
}

void QString::resize(int size)
{
	if (size < 0)
        size = 0;

    if (size == 0) {
		delete_();
		d = &shared_empty;
    } else {
        if (d->ref != 1 || size > d->alloc || (size < d->size && size < d->alloc >> 1))
            reallocData(grow(size));
        if (d->alloc >= size) {
            d->size = size;
            if (d->data == d->array) {
                d->array[size] = '\0';
            }
        }
    }	
}

void QString::expand(int i)
{
    int sz = d->size;
    resize(qMax(i + 1, sz));
    if (d->size - 1 > sz) {
        ushort *n = d->data + d->size - 1;
        ushort *e = d->data + sz;
        while (n != e)
           * --n = ' ';
    }
}

QString& QString::insert(int i, const ushort* s, int size)
{
    if (i < 0 || size <= 0)
        return *this;

    if (s >= d->data && s < d->data + d->alloc) {
        // Part of me - take a copy
        ushort *tmp = static_cast<ushort *>(qMalloc(size * sizeof(ushort)));
        memcpy(tmp, s, size * sizeof(ushort));
        insert(i, reinterpret_cast<const ushort *>(tmp), size);
        qFree(tmp);
        return *this;
    }

    expand(qMax(d->size, i) + size - 1);

    ::memmove(d->data + i + size, d->data + i, (d->size - i - size) * sizeof(QChar));
    memcpy(d->data + i, s, size * sizeof(QChar));
    return *this;
}

QString &QString::remove(int pos, int len)
{
	if (pos < 0)
        pos += d->size;
    if (pos < 0 || pos >= d->size) {
        // range problems
    } else if (pos + len >= d->size) {  // pos ok
        resize(pos);
    } else if (len > 0) {
        detach();
        memmove(d->data + pos, d->data + pos + len,
                (d->size - pos - len + 1) * sizeof(ushort));
        d->size -= len;
    }
    return *this;
}

QString &QString::replace(int pos, int len, const ushort* unicode, int size)
{
	remove(pos, size);
	return insert(pos, unicode, size);
}

QString &QString::replace(const QString &before, const QString &after, bool cs)
{
    if (d->size == 0) {
        if (before.d->size)
            return *this;
    } else {
        if (cs && before == after)
            return *this;
    }
    if (d->ref != 1)
        reallocData(d->size);

    QStringMatcher matcher(before, cs);
    int index = 0;
    const int bl = before.d->size;
    const int al = after.d->size;

    if (bl == al) {
        if (bl) {
            const ushort *auc = (const ushort*) after.d->data;
            while ((index = matcher.indexIn(*this, index)) != -1) {
                // we need memmove(), not memcpy(), in the rare case where
                // this == &after, before == after, and cs is Qt::CaseInsensitive
                memmove(d->data + index, auc, al * sizeof(ushort));
                index += bl;
            }
        }
    } else if (al < bl) {
        const ushort *auc = after.unicode();
        uint to = 0;
        uint movestart = 0;
        uint num = 0;
        while ((index = matcher.indexIn(*this, index)) != -1) {
            if (num) {
                int msize = index - movestart;
                if (msize > 0) {
                    memmove(d->data + to, d->data + movestart, msize * sizeof(ushort));
                    to += msize;
                }
            } else {
                to = index;
            }
            if (al) {
                memcpy(d->data+to, auc, al*sizeof(ushort));
                to += al;
            }
            index += bl;
            movestart = index;
            num++;
        }
        if (num) {
            int msize = d->size - movestart;
            if (msize > 0)
                memmove(d->data + to, d->data + movestart, msize * sizeof(ushort));
            resize(d->size - num*(bl-al));
        }
    } else {
        const QString copy = after;

        // the most complex case. We don't want to lose performance by doing repeated
        // copies and reallocs of the string.
        while (index != -1) {
            uint indices[4096];
            uint pos = 0;
            while (pos < 4095) {
                index = matcher.indexIn(*this, index);
                if (index == -1)
                    break;
                indices[pos++] = index;
                index += bl;
                // avoid infinite loop
                if (!bl)
                    index++;
            }
            if (!pos)
                break;

            // we have a table of replacement positions, use them for fast replacing
            int adjust = pos*(al-bl);
            // index has to be adjusted in case we get back into the loop above.
            if (index != -1)
                index += adjust;
            int newLen = d->size + adjust;
            int moveend = d->size;
            if (newLen > d->size)
                resize(newLen);

            while (pos) {
                pos--;
                int movestart = indices[pos] + bl;
                int insertstart = indices[pos] + pos*(al-bl);
                int moveto = insertstart + al;
                memmove(d->data + moveto, d->data + movestart, (moveend - movestart)*sizeof(ushort));
                memcpy(d->data + insertstart, copy.unicode(), al*sizeof(ushort));
                moveend = movestart-bl;
            }
        }
    }
    return *this;
}

QString QString::fromUtf16(const ushort* unicode, int size)
{
	return QString(unicode, size);
}

QString QString::toLower() const
{
    const ushort *p = d->data;
    if (!p)
        return *this;
    if (!d->size)
        return *this;

    const ushort *e = d->data + d->size;

    // this avoids one out of bounds check in the loop
    if (QChar(*p).isLowSurrogate())
        ++p;

    while (p != e) {
        uint c = *p;
        if (QChar(c).isLowSurrogate() && QChar(*(p - 1)).isHighSurrogate())
            c = QChar::surrogateToUcs4(*(p - 1), c);
        const QUnicodeTables::Properties *prop = qGetProp(c);
        if (prop->lowerCaseDiff || prop->lowerCaseSpecial) {
            QString s;
            s.resize(d->size);
            memcpy(s.d->data, d->data, (p - d->data)*sizeof(ushort));
            ushort *pp = s.d->data + (p - d->data);
            while (p < e) {
                uint c = *p;
                if (QChar(c).isLowSurrogate() && QChar(*(p - 1)).isHighSurrogate())
                    c = QChar::surrogateToUcs4(*(p - 1), c);
                prop = qGetProp(c);
                if (prop->lowerCaseSpecial) {
                    int pos = pp - s.d->data;
                    s.resize(s.d->size + SPECIAL_CASE_MAX_LEN);
                    pp = s.d->data + pos;
                    const ushort *specialCase = specialCaseMap + prop->lowerCaseDiff;
                    while (*specialCase)
                        *pp++ = *specialCase++;
                } else {
                    *pp++ = *p + prop->lowerCaseDiff;
                }
                ++p;
            }
            s.truncate(pp - s.d->data);
            return s;
        }
        ++p;
    }
    return *this;
}

QString QString::toUpper() const
{
    const ushort *p = d->data;
    if (!p)
        return *this;
    if (!d->size)
        return *this;

    const ushort *e = d->data + d->size;

    // this avoids one out of bounds check in the loop
    if (QChar(*p).isLowSurrogate())
        ++p;

    while (p != e) {
        uint c = *p;
        if (QChar(c).isLowSurrogate() && QChar(*(p - 1)).isHighSurrogate())
            c = QChar::surrogateToUcs4(*(p - 1), c);
        const QUnicodeTables::Properties *prop = qGetProp(c);
        if (prop->upperCaseDiff || prop->upperCaseSpecial) {
            QString s;
            s.resize(d->size);
            memcpy(s.d->data, d->data, (p - d->data)*sizeof(ushort));
            ushort *pp = s.d->data + (p - d->data);
            while (p < e) {
                uint c = *p;
                if (QChar(c).isLowSurrogate() && QChar(*(p - 1)).isHighSurrogate())
                    c = QChar::surrogateToUcs4(*(p - 1), c);
                prop = qGetProp(c);
                if (prop->upperCaseSpecial) {
                    int pos = pp - s.d->data;
                    s.resize(s.d->size + SPECIAL_CASE_MAX_LEN);
                    pp = s.d->data + pos;
                    const ushort *specialCase = specialCaseMap + prop->upperCaseDiff;
                    while (*specialCase)
                        *pp++ = *specialCase++;
                } else {
                    *pp++ = *p + prop->upperCaseDiff;
                }
                ++p;
            }
            s.truncate(pp - s.d->data);
            return s;
        }
        ++p;
    }
    return *this;
}

/*!
    \enum QString::NormalizationForm

    This enum describes the various normalized forms of Unicode text.

    \value NormalizationForm_D  Canonical Decomposition
    \value NormalizationForm_C  Canonical Decomposition followed by Canonical Composition
    \value NormalizationForm_KD  Compatibility Decomposition
    \value NormalizationForm_KC  Compatibility Decomposition followed by Canonical Composition

    \sa normalized(),
        {http://www.unicode.org/reports/tr15/}{Unicode Standard Annex #15}
*/

/*!
    \fn QString QString::normalized(NormalizationForm mode) const
    Returns the string in the given Unicode normalization \a mode.
*/
QString QString::normalized(QString::NormalizationForm mode) const
{
    return normalized(mode, CURRENT_VERSION);
}

/*!
    \overload
    \fn QString QString::normalized(NormalizationForm mode, QChar::UnicodeVersion version) const

    Returns the string in the given Unicode normalization \a mode,
    according to the given \a version of the Unicode standard.
*/
QString QString::normalized(QString::NormalizationForm mode, QChar::UnicodeVersion version) const
{
    QString s = *this;
    if (version != CURRENT_VERSION) {
        for (int i = 0; i < NumNormalizationCorrections; ++i) {
            const NormalizationCorrection n = uc_normalization_corrections[i];
            if (n.version > version) {
                QString orig;
                orig.append(QChar::highSurrogate(n.ucs4));
                orig.append(QChar::lowSurrogate(n.ucs4));
                QString replacement;
                replacement.append(QChar::highSurrogate(n.old_mapping));
                replacement.append(QChar::lowSurrogate(n.old_mapping));
                s.replace(orig, replacement);
            }
        }
    }
    s = decomposeHelper(s, mode < QString::NormalizationForm_KD, version);

    s = canonicalOrderHelper(s, version);

    if (mode == QString::NormalizationForm_D || mode == QString::NormalizationForm_KD)
        return s;

    return composeHelper(s);
}

// ÔÓ7ÔÓ8µÄº¯Êý
int qstricmp(const char *str1, const char *str2)
{
    register const uchar *s1 = reinterpret_cast<const uchar *>(str1);
    register const uchar *s2 = reinterpret_cast<const uchar *>(str2);
    int res;
    uchar c;
    if (!s1 || !s2)
        return s1 ? 1 : (s2 ? -1 : 0);
    for (; !(res = (c = (uchar)QChar::toLower((ushort)*s1)) - QChar::toLower((ushort)*s2)); s1++, s2++)
        if (!c)                                // strings are equal
            break;
    return res;
}

int qstricmp(const QByteArray &s1, const QByteArray &s2)
{
	return qstricmp(s1.constData(), s2.constData());
}

#ifdef _WIN32_WCE
void *bsearch(const void *key, const void *base, size_t high, size_t size, int (*compar)(const void *, const void *))
{
	register char *p;
	size_t low;
	size_t mid;
	int r;

	if (size > 0) {				/* TODO: change this to an assert?? */
		low = 0;
		while (low < high) {
			mid = low + ((high - low) >> 1); /* Avoid possible overflow here. */
			p = ((char *)base) + mid * size; /* Could overflow here... */
			r = (*compar)(key, p); /* but that's an application problem! */
			if (r > 0) {
				low = mid + 1;
			} else if (r < 0) {
				high = mid;
			} else {
				return p;
			}
		}
	}
	return 0;
}
#endif

QT_END_NAMESPACE
