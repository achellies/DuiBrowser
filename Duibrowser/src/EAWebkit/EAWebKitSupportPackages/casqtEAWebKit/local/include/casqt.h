//     作者：王伟
// 版本历史：2008.8.27 1.0建立。
// 基于C++实现，可以提供给C使用
#ifndef CASQT_H
#define CASQT_H

typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;

typedef signed char qint8;         /* 8 bit signed */
typedef unsigned char quint8;      /* 8 bit unsigned */
typedef short qint16;              /* 16 bit signed */
typedef unsigned short quint16;    /* 16 bit unsigned */
typedef int qint32;                /* 32 bit signed */
typedef unsigned int quint32;      /* 32 bit unsigned */
typedef long long qint64;           /* 64 bit signed */
typedef unsigned long long quint64; /* 64 bit unsigned */

#define Q_INT64_C(c) static_cast<long long>(c ## LL)     /* signed 64 bit constant */
#define Q_UINT64_C(c) static_cast<unsigned long long>(c ## ULL) /* unsigned 64 bit constant */

#ifdef __cplusplus
#define QT_BEGIN_NAMESPACE	namespace CasQt {
#define QT_END_NAMESPACE	}
#define QT_USE_NAMESPACE	using namespace CasQt;
#define QT_NAMESPACE		CasQt
#endif

#define qWarning(X) ((void*)0)
#define Q_CORE_EXPORT
#define QT_FASTCALL
#define Q_CORE_EXPORT_INLINE inline
#define Q_REQUIRED_RESULT

#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>

#endif // CASQT_H
