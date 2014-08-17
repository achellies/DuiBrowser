#define __WIN32__
#define _CRT_SECURE_NO_WARNINGS

// 一些库的预定义
#ifndef NO_CONFIG_H
#ifndef HAVE_CONFIG_H
#define HAVE_CONFIG_H 1
#endif
#endif

#ifndef HAVE_MODE_T
#define HAVE_MODE_T 1
#endif

// for pthreads
#ifndef __CLEANUP_C
#define __CLEANUP_C
#endif

#ifndef PTW32_STATIC_LIB
#define PTW32_STATIC_LIB 1
#endif

// for libxml
#ifndef LIBXML_STATIC
#define LIBXML_STATIC 1
#endif

#ifndef HAVE_ZLIB_H
#define HAVE_ZLIB_H 1
#endif

#ifndef HAVE_PTHREAD_H
#define HAVE_PTHREAD_H 1
#endif

// for libxslt
#ifndef LIBXSLT_STATIC
#define LIBXSLT_STATIC
#endif

// for libcurl
#ifndef CURL_STATICLIB
#define CURL_STATICLIB
#endif

// for expat
#ifndef XML_STATIC
#define XML_STATIC
#define XML_UNICODE_WCHAR_T
#endif

// for libiconv
#ifndef BUILDING_LIBCHARSET
#define BUILDING_LIBCHARSET
#endif
#define EXEEXT ".exe"

// for FreeType2
#ifndef FT2_BUILD_LIBRARY
#define FT2_BUILD_LIBRARY
#endif

// for cairo
#ifndef CAIRO_WIN32_STATIC_BUILD
#define CAIRO_WIN32_STATIC_BUILD
#endif

#ifndef NO_FONTCONFIG
#define NO_FONTCONFIG
#endif

// for icu
#ifndef U_STATIC_IMPLEMENTATION
#define U_STATIC_IMPLEMENTATION
#endif

// for jpeglib
#ifndef ALL_STATIC
#define ALL_STATIC
#endif

typedef unsigned char UInt8;

#ifdef INCLUDE_WINDOWS_H
#ifndef WIN32_LEAN_AND_MEAN
// WIN32_LEAN_AND_MEAN implies NOCRYPT and NOGDI.
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif
