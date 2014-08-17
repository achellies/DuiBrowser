#define NO_WIN32_WCE
#define __DBGAPI_H__ // 屏蔽dbgapi.h

// 一些库的预定义
#ifndef NO_CONFIG_H
#ifndef HAVE_CONFIG_H
#define HAVE_CONFIG_H 1
#endif
#endif

// public
#ifndef HAVE_MODE_T
#define HAVE_MODE_T 1
#endif

// for curl & iconv & zlib
#ifndef __STDC__
#define __STDC__ 1
#endif

#ifndef __CLEANUP_C
#define __CLEANUP_C
#endif

#ifndef HAVE_PTHREAD_H
#define HAVE_PTHREAD_H 1
#endif

// for wceshunt，目前都采用静态库
#ifndef WCESHUNT_STATICLIB
#define WCESHUNT_STATICLIB
#endif

// for cairo
#ifndef NO_FONTCONFIG
#define NO_FONTCONFIG
#endif

// for cairo
#ifndef CAIRO_WIN32_STATIC_BUILD
#define CAIRO_WIN32_STATIC_BUILD
#endif

// for expat
#ifndef XML_STATIC
#define XML_STATIC
#define XML_UNICODE_WCHAR_T
#endif

// for libiconv
#define EXEEXT ".exe"

// for jpeglib & libpng
#ifndef ALL_STATIC
#define ALL_STATIC
#endif

// for pthreads
#ifdef HAVE_PTHREAD_H
#ifndef PTW32_STATIC_LIB
#define PTW32_STATIC_LIB 1
#endif
#endif

// for libxml
#ifndef HAVE_ZLIB_H
#define HAVE_ZLIB_H 1
#endif

#ifndef LIBXML_STATIC
#define LIBXML_STATIC 1
#endif

// for libxslt
#ifndef LIBXSLT_STATIC
#define LIBXSLT_STATIC
#endif

// for libcurl
#ifndef CURL_STATICLIB
#define CURL_STATICLIB
#endif

// for icu
#ifndef U_STATIC_IMPLEMENTATION
#define U_STATIC_IMPLEMENTATION
#endif

#include "wceshunt.h"
