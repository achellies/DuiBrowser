#ifndef QENDIAN_H
#define QENDIAN_H

#include "casqt.h"

QT_BEGIN_NAMESPACE

class QSysInfo {
public:
    enum Sizes {
        WordSize = (sizeof(void *)<<3)
    };

    enum Endian {
        BigEndian,
        LittleEndian
    };
    /* needed to bootstrap qmake */
    static int ByteOrder;

	QSysInfo()
	{
		const ushort BOM = 0xFEFF;
        const unsigned char BOMHighByte = *reinterpret_cast<const unsigned char*>(&BOM);
		ByteOrder = BOMHighByte == 0xFF ? LittleEndian : BigEndian;
	}
};

inline quint64 qFromLittleEndian_helper(const uchar *src, quint64 *dest)
{
    return 0
        | src[0]
        | src[1] * Q_UINT64_C(0x0000000000000100)
        | src[2] * Q_UINT64_C(0x0000000000010000)
        | src[3] * Q_UINT64_C(0x0000000001000000)
        | src[4] * Q_UINT64_C(0x0000000100000000)
        | src[5] * Q_UINT64_C(0x0000010000000000)
        | src[6] * Q_UINT64_C(0x0001000000000000)
        | src[7] * Q_UINT64_C(0x0100000000000000);
}

inline quint32 qFromLittleEndian_helper(const uchar *src, quint32 *dest)
{
    return 0
        | src[0]
        | src[1] * quint32(0x00000100)
        | src[2] * quint32(0x00010000)
        | src[3] * quint32(0x01000000);
}

inline quint16 qFromLittleEndian_helper(const uchar *src, quint16 *dest)
{
    return 0
        | src[0]
        | src[1] * 0x0100;
}

inline qint64 qFromLittleEndian_helper(const uchar *src, qint64 * dest)
{ return static_cast<qint64>(qFromLittleEndian_helper(src, reinterpret_cast<quint64*>(0))); }
inline qint32 qFromLittleEndian_helper(const uchar *src, qint32 * dest)
{ return static_cast<qint32>(qFromLittleEndian_helper(src, reinterpret_cast<quint32*>(0))); }
inline qint16 qFromLittleEndian_helper(const uchar *src, qint16 * dest)
{ return static_cast<qint16>(qFromLittleEndian_helper(src, reinterpret_cast<quint16*>(0))); }

template <class T> inline T qFromLittleEndian(const uchar *src)
{
    return qFromLittleEndian_helper(src, reinterpret_cast<T*>(0));
}

inline quint64 qFromBigEndian_helper(const uchar *src, quint64 *dest)
{
    return 0
        | src[7]
        | src[6] * Q_UINT64_C(0x0000000000000100)
        | src[5] * Q_UINT64_C(0x0000000000010000)
        | src[4] * Q_UINT64_C(0x0000000001000000)
        | src[3] * Q_UINT64_C(0x0000000100000000)
        | src[2] * Q_UINT64_C(0x0000010000000000)
        | src[1] * Q_UINT64_C(0x0001000000000000)
        | src[0] * Q_UINT64_C(0x0100000000000000);
}

inline quint32 qFromBigEndian_helper(const uchar *src, quint32 * dest)
{
    return 0
        | src[3]
        | src[2] * quint32(0x00000100)
        | src[1] * quint32(0x00010000)
        | src[0] * quint32(0x01000000);
}

inline quint16 qFromBigEndian_helper(const uchar *src, quint16 * des)
{
    return 0
        | src[1]
        | src[0] * 0x0100;
}


inline qint64 qFromBigEndian_helper(const uchar *src, qint64 * dest)
{ return static_cast<qint64>(qFromBigEndian_helper(src, reinterpret_cast<quint64*>(0))); }
inline qint32 qFromBigEndian_helper(const uchar *src, qint32 * dest)
{ return static_cast<qint32>(qFromBigEndian_helper(src, reinterpret_cast<quint32*>(0))); }
inline qint16 qFromBigEndian_helper(const uchar *src, qint16 * dest)
{ return static_cast<qint16>(qFromBigEndian_helper(src, reinterpret_cast<quint16*>(0))); }

template <class T> inline T qFromBigEndian(const uchar *src)
{
    return qFromBigEndian_helper(src, reinterpret_cast<T*>(0));
}

QT_END_NAMESPACE

#endif // QENDIAN_H
