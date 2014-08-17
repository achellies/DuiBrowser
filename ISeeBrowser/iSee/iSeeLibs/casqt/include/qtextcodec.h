#ifndef QTEXTCODEC_H
#define QTEXTCODEC_H

#include "qstring.h"

QT_BEGIN_NAMESPACE

typedef const char* Aliase;

class QTextCodec
{
public:
	void delete_() { delete this; }

	static const Aliase* availableCodecs();
    static QTextCodec* codecForName(const char *name);
    static QTextCodec* codecForLocale();

    enum ConversionFlag {
        DefaultConversion,
        ConvertInvalidToNull = 0x80000000,
        IgnoreHeader = 0x1
    };
	typedef uint ConversionFlags;

    struct ConverterState {
        ConverterState(ConversionFlags f = DefaultConversion)
            : flags(f), remainingChars(0), invalidChars(0), d(0) { state_data[0] = state_data[1] = state_data[2] = 0; }
        ~ConverterState() { if (d) qFree(d); }
        ConversionFlags flags;
        int remainingChars;
        int invalidChars;
        uint state_data[3];
        void *d;
    };

    QString toUnicode(const char *in, int length, ConverterState *state = 0)
        { return convertToUnicode(in, length, state); }
    QByteArray fromUnicode(const ushort *in, int length, ConverterState *state = 0)
        { return convertFromUnicode(in, length, state); }

	virtual const char* name() = 0;
	virtual const Aliase* aliases();

protected:
    virtual QString convertToUnicode(const char *in, int length, ConverterState *state) = 0;
    virtual QByteArray convertFromUnicode(const ushort *in, int length, ConverterState *state) = 0;

	Aliase* m_aliases;

    QTextCodec();
    virtual ~QTextCodec();
};

QT_END_NAMESPACE

#endif // QTEXTCODEC_H
