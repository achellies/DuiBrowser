/*
 * Copyright (C) 2004, 2005, 2006 Apple Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */


/*
* This file was modified by Electronic Arts Inc Copyright © 2010
*/

#include <wtf/Platform.h>

#define MOBILE 0

#ifdef __APPLE__
#define HAVE_FUNC_USLEEP 1
#endif /* __APPLE__ */

#if PLATFORM(WIN_OS)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#ifndef WINVER
#define WINVER 0x0500
#endif

// If we don't define these, they get defined in windef.h.
// We want to use std::min and std::max.
#ifndef max
#define max max
#endif
#ifndef min
#define min min
#endif

// CURL needs winsock, so don't prevent inclusion of it
#if !USE(CURL)
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_ // Prevent inclusion of winsock.h in windows.h
#endif
#endif

#endif /* PLATFORM(WIN_OS) */

// On MSW, wx headers need to be included before windows.h is.
// The only way we can always ensure this is if we include wx here.
#if PLATFORM(WX)
#include <wx/defs.h>
#endif

#if !PLATFORM(SYMBIAN)
#define IMPORT_C
#define EXPORT_C
#endif

#ifdef __cplusplus

// These undefs match up with defines in WebCorePrefix.h for Mac OS X.
// Helps us catch if anyone uses new or delete by accident in code and doesn't include "config.h".
#undef new
#undef delete
#include <wtf/FastMalloc.h>

#endif

// this breaks compilation of <QFontDatabase>, at least, so turn it off for now
// Also generates errors on wx on Windows, presumably because these functions
// are used from wx headers.
#if !PLATFORM(QT) && !PLATFORM(WX)
#include <wtf/DisallowCType.h>
#endif

#if PLATFORM(GTK) || PLATFORM(BAL)
#define WTF_USE_JAVASCRIPTCORE_BINDINGS 1
#endif

#if !COMPILER(MSVC) // can't get this to compile on Visual C++ yet
#define AVOID_STATIC_CONSTRUCTORS 1
#endif


#if PLATFORM(WIN)
    #ifndef WTF_USE_JAVASCRIPTCORE_BINDINGS
        #define WTF_USE_JAVASCRIPTCORE_BINDINGS 1
    #endif
    #ifndef WTF_PLATFORM_CG
        #define WTF_PLATFORM_CG 1
    #endif
    #undef WTF_PLATFORM_CAIRO
    #ifndef WTF_USE_CFNETWORK
        #define WTF_USE_CFNETWORK 1
    #endif
    #undef WTF_USE_WININET
    #ifndef WTF_PLATFORM_CF
        #define WTF_PLATFORM_CF 1
    #endif
    #ifndef WTF_USE_PTHREADS
        #define WTF_USE_PTHREADS 0
    #endif
#endif

#if PLATFORM(MAC)
#define WTF_USE_JAVASCRIPTCORE_BINDINGS 1
#endif

#if PLATFORM(SYMBIAN)
#define WTF_USE_JAVASCRIPTCORE_BINDINGS 1
#undef WIN32
#undef _WIN32
#undef AVOID_STATIC_CONSTRUCTORS
#define USE_SYSTEM_MALLOC 1
#define U_HAVE_INT8_T 0
#define U_HAVE_INT16_T 0
#define U_HAVE_INT32_T 0
#define U_HAVE_INT64_T 0
#define U_HAVE_INTTYPES_H 0

#include <stdio.h>
#include <snprintf.h>
#include <limits.h>
#include <wtf/MathExtras.h>
#endif

#if PLATFORM(CG)
#ifndef CGFLOAT_DEFINED
#ifdef __LP64__
typedef double CGFloat;
#else
typedef float CGFloat;
#endif
#define CGFLOAT_DEFINED 1
#endif
#endif /* PLATFORM(CG) */

#ifdef BUILDING_ON_TIGER
#undef ENABLE_FTPDIR
#define ENABLE_FTPDIR 0
#endif

#if PLATFORM(WIN) && PLATFORM(CG)
#define WTF_USE_SAFARI_THEME 1
#endif


// To avoid printf in release builds
 #if EAWEBKIT_TRACE_ENABLED
    extern "C" void WTFLogEvent(const char* format, ...);
	#define OWB_PRINTF(msg)						WTFLogEvent("%s", msg)
	#define OWB_PRINTF_FORMATTED(fmt, ...)		WTFLogEvent(fmt, __VA_ARGS__)
	#define OWB_PRINT_ACTIVE 1
#else
	#define OWB_PRINTF(msg)						((void)0)
	#define OWB_PRINTF_FORMATTED(fmt, ...)		((void)0)
#endif

