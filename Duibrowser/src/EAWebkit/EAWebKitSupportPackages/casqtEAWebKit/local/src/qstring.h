#ifndef QSTRING_H
#define QSTRING_H

#include "casqt.h"
#include <wtf/FastMalloc.h>

#define qMalloc(N)			fastMalloc(N)
#define qRealloc(P, N)		fastRealloc(P, N)
#define qFree(P)			fastFree(P)
#define qMemSet(P, C, S)	memset(P, C, S)
#define qMemCopy(D, S, SZ)	memcpy(D, S, SZ)
#define qMin(A, B)			(((A) < (B)) ? (A) : (B))
#define qMax(A, B)			(((A) > (B)) ? (A) : (B))
#define qBound(N, V, M)		qMax(N, qMin(M, V))

QT_BEGIN_NAMESPACE

struct QLatin1Char;
class QChar;
class QByteArray;
class QString;

struct QLatin1Char
{
public:
    inline explicit QLatin1Char(char c) : ch(c) {}
    inline char toLatin1() const { return ch; }
    inline ushort unicode() const { return ushort(uchar(ch)); }

private:
    char ch;
};

class QChar
{
public:
	enum SpecialCharacter {
        Null = 0x0000,
        Nbsp = 0x00a0,
        ReplacementCharacter = 0xfffd,
        ObjectReplacementCharacter = 0xfffc,
        ByteOrderMark = 0xfeff,
        ByteOrderSwapped = 0xfffe,
        ParagraphSeparator = 0x2029,
        LineSeparator = 0x2028
    };
    
    // Unicode information

    enum Category
    {
        NoCategory,

        Mark_NonSpacing,          //   Mn
        Mark_SpacingCombining,    //   Mc
        Mark_Enclosing,           //   Me

        Number_DecimalDigit,      //   Nd
        Number_Letter,            //   Nl
        Number_Other,             //   No

        Separator_Space,          //   Zs
        Separator_Line,           //   Zl
        Separator_Paragraph,      //   Zp

        Other_Control,            //   Cc
        Other_Format,             //   Cf
        Other_Surrogate,          //   Cs
        Other_PrivateUse,         //   Co
        Other_NotAssigned,        //   Cn

        Letter_Uppercase,         //   Lu
        Letter_Lowercase,         //   Ll
        Letter_Titlecase,         //   Lt
        Letter_Modifier,          //   Lm
        Letter_Other,             //   Lo

        Punctuation_Connector,    //   Pc
        Punctuation_Dash,         //   Pd
        Punctuation_Open,         //   Ps
        Punctuation_Close,        //   Pe
        Punctuation_InitialQuote, //   Pi
        Punctuation_FinalQuote,   //   Pf
        Punctuation_Other,        //   Po

        Symbol_Math,              //   Sm
        Symbol_Currency,          //   Sc
        Symbol_Modifier,          //   Sk
        Symbol_Other,             //   So

        Punctuation_Dask = Punctuation_Dash // oops
    };

    enum Direction
    {
        DirL, DirR, DirEN, DirES, DirET, DirAN, DirCS, DirB, DirS, DirWS, DirON,
        DirLRE, DirLRO, DirAL, DirRLE, DirRLO, DirPDF, DirNSM, DirBN
    };

    enum Decomposition
    {
        NoDecomposition,
        Canonical,
        Font,
        NoBreak,
        Initial,
        Medial,
        Final,
        Isolated,
        Circle,
        Super,
        Sub,
        Vertical,
        Wide,
        Narrow,
        Small,
        Square,
        Compat,
        Fraction
    };

    enum Joining
    {
        OtherJoining, Dual, Right, Center
    };

    enum CombiningClass
    {
        Combining_BelowLeftAttached       = 200,
        Combining_BelowAttached           = 202,
        Combining_BelowRightAttached      = 204,
        Combining_LeftAttached            = 208,
        Combining_RightAttached           = 210,
        Combining_AboveLeftAttached       = 212,
        Combining_AboveAttached           = 214,
        Combining_AboveRightAttached      = 216,

        Combining_BelowLeft               = 218,
        Combining_Below                   = 220,
        Combining_BelowRight              = 222,
        Combining_Left                    = 224,
        Combining_Right                   = 226,
        Combining_AboveLeft               = 228,
        Combining_Above                   = 230,
        Combining_AboveRight              = 232,

        Combining_DoubleBelow             = 233,
        Combining_DoubleAbove             = 234,
        Combining_IotaSubscript           = 240
    };

    enum UnicodeVersion {
        Unicode_Unassigned,
        Unicode_1_1,
        Unicode_2_0,
        Unicode_2_1_2,
        Unicode_3_0,
        Unicode_3_1,
        Unicode_3_2,
        Unicode_4_0,
        Unicode_4_1,
        Unicode_5_0
    };

public:
	QChar() : ucs(0) {}
	QChar(char c) : ucs(ushort(c)) {}
	QChar(ushort c) : ucs(c) {}
	QChar(uchar c, uchar r) : ucs((r << 8) | c) {}
	QChar(short rc) : ucs(ushort(rc)) {}
	QChar(uint rc) : ucs(ushort(rc & 0xffff)) {}
	QChar(int rc) : ucs(ushort(rc & 0xffff)) {}
	QChar(SpecialCharacter s) : ucs(ushort(s)) {}
	~QChar() {}

	Category category() const;
    Direction direction() const;
    Joining joining() const;
    bool hasMirrored() const;
    unsigned char combiningClass() const;

    QChar mirroredChar() const;
    QString decomposition() const;
    Decomposition decompositionTag() const;

    int digitValue() const;
    QChar toLower() const;
    QChar toUpper() const;
    QChar toTitleCase() const;
    QChar toCaseFolded() const;

    UnicodeVersion unicodeVersion() const;

	inline ushort unicode() const { return ucs; }
	char toAscii() const;
	QChar fromAscii(char c);

	inline bool isNull() const { return ucs == 0; }
    bool isPrint() const;
    bool isPunct() const;
    bool isSpace() const;
    bool isMark() const;
    bool isLetter() const;
    bool isNumber() const;
    bool isLetterOrNumber() const;
    bool isDigit() const;
    bool isSymbol() const;
    inline bool isLower() const { return category() == Letter_Lowercase; }
    inline bool isUpper() const { return category() == Letter_Uppercase; }
    inline bool isTitleCase() const { return category() == Letter_Titlecase; }

    inline bool isHighSurrogate() const {
        return ((ucs & 0xfc00) == 0xd800);
    }
    inline bool isLowSurrogate() const {
        return ((ucs & 0xfc00) == 0xdc00);
    }

	inline uchar cell() const { return uchar(ucs & 0xff); }
    inline uchar row() const { return uchar((ucs>>8)&0xff); }
	inline void setCell(uchar acell) { ucs = (ucs & 0xff00) + acell; }
	inline void setRow(uchar arow) { ucs = (ushort(arow)<<8) + (ucs&0xff); }

	static uchar cell(ushort ch) { return uchar(ch & 0xff); }
    static uchar row(ushort ch) { return uchar((ch>>8)&0xff); }

	static inline uint surrogateToUcs4(ushort high, ushort low) {
        return (uint(high)<<10) + low - 0x35fdc00;
    }
    static inline uint surrogateToUcs4(QChar high, QChar low) {
        return (uint(high.ucs)<<10) + low.ucs - 0x35fdc00;
    }
    static inline ushort highSurrogate(uint ucs4) {
        return (ucs4>>10) + 0xd7c0;
    }
    static inline ushort lowSurrogate(uint ucs4) {
        return ucs4%0x400 + 0xdc00;
    }

	static Category category(uint ucs4);
    static Category category(ushort ucs2);
    static Direction direction(uint ucs4);
    static Direction direction(ushort ucs2);
    static Joining joining(uint ucs4);
    static Joining joining(ushort ucs2);
    static unsigned char combiningClass(uint ucs4);
    static unsigned char combiningClass(ushort ucs2);

    static uint mirroredChar(uint ucs4);
    static ushort mirroredChar(ushort ucs2);
    static Decomposition decompositionTag(uint ucs4);

    static int digitValue(uint ucs4);
    static int digitValue(ushort ucs2);
    static uint toLower(uint ucs4);
    static ushort toLower(ushort ucs2);
    static uint toUpper(uint ucs4);
    static ushort toUpper(ushort ucs2);
    static uint toTitleCase(uint ucs4);
    static ushort toTitleCase(ushort ucs2);
    static uint toCaseFolded(uint ucs4);
    static ushort toCaseFolded(ushort ucs2);

    static UnicodeVersion unicodeVersion(uint ucs4);
    static UnicodeVersion unicodeVersion(ushort ucs2);

	static QString decomposition(uint ucs4);

	static bool isLetterOrNumber(uint ucs4);

private:
	ushort ucs;
};

class QByteArray
{
public:
	QByteArray() : d(&shared_null) {}
	QByteArray(const char *s, int len = -1);
	inline QByteArray(const QByteArray& src);
	~QByteArray() { delete_(); }

	inline bool isNull() const { return d == &shared_null; }
	inline bool isEmpty() const { return d->size == 0; }

	inline char* data() const { return d->data; }
	inline const char* constData() const { return d->data; }
	inline int size() const { return d->size; }
	inline int length() const { return d->size; }

	QByteArray &append(const char *s, int len = -1);
	inline QByteArray &operator+=(const char *s) { 
		return append(s);
	}
	inline QByteArray &operator+=(char c) {
		return append(&c, 1);
	}
	QByteArray &prepend(const char *s);
	inline QByteArray &operator+=(const QByteArray &a) {
		return append(a.data(), a.length());
	}

	int indexOf(char ch, int from = 0);
	QByteArray mid(int pos, int len = -1);
	QByteArray trimmed() const;

	void resize(int size);
	inline void truncate(int pos)
	{
		if (pos < d->size)
			resize(pos);
	}

private:
	void delete_()
	{
		if (d != &shared_null && d != &shared_empty)
		{
			if (d->ref == 1)
			{
				Data *x = d;
				d = &shared_null;
				qFree(x);
			}
			else
				d->ref--;
		}
	}

	void reallocData(int alloc);

private:
	struct Data {
		uint ref;
        int alloc, size;
        char *data;
		char array[1];
    };
	static Data shared_null;
	static Data shared_empty;

	Data *d;
};
inline QByteArray::QByteArray(const QByteArray &other) : d(other.d) { if (d != &shared_null && d != &shared_empty) d->ref++; }

class QString
{
public:
	QString() : d(&shared_null) {}
	QString(const ushort *s, int len = -1);
	inline QString(const QString& src);
	~QString() { delete_(); }
	inline QString &QString::operator=(const QString &other) {
		if (other.d != &shared_null && other.d != &shared_empty)
			other.d->ref++;
		delete_();
		d = other.d;
		return *this;
	}
	inline bool QString::operator==(const QString &other) const {
		return (size() == other.size()) &&
			(memcmp((char*)unicode(),(char*)other.unicode(), size() * sizeof(QChar)) == 0);
	}

	inline bool isNull() const { return d == &shared_null; }
	inline bool isEmpty() const { return d->size == 0; }

	inline ushort* data() { return d->data; }
	inline const ushort* constData() const { return d->data; }
	inline const ushort *utf16() const { return d->data; }
	inline const ushort *unicode() const { return d->data; }
	inline int size() const { return d->size; }
    inline int count() const { return d->size; }
	inline int length() const { return d->size; }

	inline const QChar at(int i) const {
		return (i >= 0 && i < d->size) ? QChar(d->data[i]) : QChar();
	}

	QString &append(const ushort *s, int len = -1);
	inline QString &append(ushort ucs) { return append(&ucs, 1); }
	inline QString &operator+=(const ushort *s) { return append(s); }
	inline QString &operator+=(ushort ucs) { return append(&ucs, 1); }
	inline QString &operator+=(char c) { 
		ushort ucs = ushort(c);
		return append(&ucs, 1);
	}
	inline QString &operator+=(uchar c) { 
		ushort ucs = ushort(c);
		return append(&ucs, 1);
	}
	inline QString &operator+=(uint u) { 
		ushort ucs = ushort(u);
		return append(&ucs, 1);
	}
	inline QString &operator+=(QLatin1Char c) {
		ushort ucs = c.unicode();
		return append(&ucs, 1);
	}
	inline QString &operator+=(QChar c) {
		ushort ucs = c.unicode();
		return append(&ucs, 1);
	}

	void resize(int size);
	inline void truncate(int pos)
	{
		if (pos < d->size)
			resize(pos);
	}
	inline void clear() { if (!isNull()) *this = QString(); }

	void expand(int i);
	QString &remove(int pos, int len);
	QString &insert(int i, const ushort* s, int size);
    QString &replace(int i, int len, const ushort*s, int slen);
    QString &replace(const QString &before, const QString &after, bool cs = true);
	QString toLower() const;
	QString toUpper() const;

    static QString fromUtf16(const ushort *, int size = -1);

    enum NormalizationForm {
        NormalizationForm_D,
        NormalizationForm_C,
        NormalizationForm_KD,
        NormalizationForm_KC
    };
    QString normalized(NormalizationForm mode) const Q_REQUIRED_RESULT;
    QString normalized(NormalizationForm mode, QChar::UnicodeVersion version) const Q_REQUIRED_RESULT;
    	
    static QByteArray toAce(const QString &);

private:
	void delete_()
	{
		if (d != &shared_null && d != &shared_empty)
		{
			if (d->ref == 1)
			{
				Data *x = d;
				d = &shared_null;
				qFree(x);
			}
			else
				d->ref--;
		}
	}

	inline void detach() { if (d->ref != 1 || d->data != d->array) reallocData(); }
	int grow(int size);
	void reallocData(int alloc);
	inline void reallocData() { reallocData(d->size); }

private:
	struct Data {
		uint ref;
        int alloc, size;
        ushort *data;
		ushort array[1];
    };
	static Data shared_null;
	static Data shared_empty;

	Data *d;
};
inline QString::QString(const QString &other) : d(other.d) { if (d != &shared_null && d != &shared_empty) d->ref++; }

#define qgetenv(N)	""

int qstricmp(const char *str1, const char *str2);
int qstricmp(const QByteArray &s1, const QByteArray &s2);
#ifdef _WIN32_WCE
void *bsearch(const void *key, const void *base, size_t high, size_t size, int (*compar)(const void *, const void *));
#endif

QT_END_NAMESPACE

#endif // QSTRING_H
