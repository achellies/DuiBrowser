/* -*- mode: c++; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

/*
* This file was modified by Electronic Arts Inc Copyright ?2009-2010
*/

#ifndef WTF_Platform_h
#define WTF_Platform_h

#include <EAWebKit/EAWebKitConfig.h>
#if EAWEBKIT_THROW_BUILD_ERROR
#error This file should be included only in a dll build
#endif


/* PLATFORM handles OS, operating environment, graphics API, and CPU */
#define PLATFORM(WTF_FEATURE) (defined( WTF_PLATFORM_##WTF_FEATURE ) && WTF_PLATFORM_##WTF_FEATURE)
#define COMPILER(WTF_FEATURE) (defined( WTF_COMPILER_##WTF_FEATURE ) && WTF_COMPILER_##WTF_FEATURE)
#define HAVE(WTF_FEATURE) (defined( HAVE_##WTF_FEATURE ) && HAVE_##WTF_FEATURE)
#define USE(WTF_FEATURE) (defined( WTF_USE_##WTF_FEATURE ) && WTF_USE_##WTF_FEATURE)
#define ENABLE(WTF_FEATURE) (defined( ENABLE_##WTF_FEATURE ) && ENABLE_##WTF_FEATURE)

/* Operating systems - low-level dependencies */

/* PLATFORM(DARWIN) */
/* Operating system level dependencies for Mac OS X / Darwin that should */
/* be used regardless of operating environment */
#ifdef __APPLE__
#define WTF_PLATFORM_DARWIN 1
#endif

#if defined(__OWBAL_PLATFORM_MACPORT__)
#undef WTF_PLATFORM_MAC 
#define WTF_PLATFORM_MACPORT
/*#define WTF_USE_PTHREADS 1*/
#endif

/* OS(WINCE) - Windows CE; note that for this platform OS(WINDOWS) is also defined */
#if defined(_WIN32_WCE)
#define WTF_PLATFORM_WINCE_OS 1
#endif

/* PLATFORM(WIN_OS) */
/* Operating system level dependencies for Windows that should be used */
/* regardless of operating environment */
#if (defined(WIN32) || defined(_WIN32)) && !defined(_XBOX)
#define WTF_PLATFORM_WIN_OS 1
#endif

/* PLATFORM(XBOX) */
/* Operating system level dependencies for XBox that should be used */
/* regardless of operating environment */
#if defined(_XBOX)
#define WTF_PLATFORM_XBOX 1
#endif

/* PLATFORM(PS3) */
/* Operating system level dependencies for PS3 that should be used */
/* regardless of operating environment */
#if defined(__PPU__)
#define WTF_PLATFORM_PS3 1
#endif

/* PLATFORM(EA) */
/* Operating system-like level dependencies for EA tech */
#if !defined(WTF_PLATFORM_EA) && defined(__OWB__)
#define WTF_PLATFORM_EA 1
#endif

/* PLATFORM(FREEBSD) */
/* Operating system level dependencies for FreeBSD-like systems that */
/* should be used regardless of operating environment */
#ifdef __FreeBSD__
#define WTF_PLATFORM_FREEBSD 1
#endif

/* PLATFORM(SOLARIS) */
/* Operating system level dependencies for Solaris that should be used */
/* regardless of operating environment */
#if defined(sun) || defined(__sun)
#define WTF_PLATFORM_SOLARIS 1
#endif

/* PLATFORM(UNIX) */
/* Operating system level dependencies for Unix-like systems that */
/* should be used regardless of operating environment */
#if   PLATFORM(DARWIN)     \
   || PLATFORM(FREEBSD)    \
   || defined(unix)        \
   || defined(__unix)      \
   || defined(__unix__)    \
   || defined (__NetBSD__) \
   || defined (__OWBAL_PLATFORM_MACPORT__) \
   || defined(_AIX)
#define WTF_PLATFORM_UNIX 1
#endif

/* Operating environments */

/* PLATFORM(QT) */
/* PLATFORM(GTK) */
/* PLATFORM(MAC) */
/* PLATFORM(WIN) */
#if defined(BUILDING_QT__)
#define WTF_PLATFORM_QT 1

/* PLATFORM(KDE) */
#if defined(BUILDING_KDE__)
#define WTF_PLATFORM_KDE 1
#endif

#elif defined(BUILDING_WX__)
    #define WTF_PLATFORM_WX 1
#elif defined(BUILDING_GTK__)
    #define WTF_PLATFORM_GTK 1
#elif PLATFORM(DARWIN) && !defined(__OWBAL_PLATFORM_MACPORT__)
    #define WTF_PLATFORM_MAC 1
#elif PLATFORM(WIN_OS) && !defined(__OWBAL_PLATFORM_WIN32PORT__)
    #define WTF_PLATFORM_WIN 1
#endif

/* Graphics engines */

/* PLATFORM(CG) */
/* PLATFORM(CAIRO) */
#if PLATFORM(MAC)
#define WTF_PLATFORM_CG 1
#define WTF_PLATFORM_CI 1
#elif !PLATFORM(QT) && !PLATFORM(WX)
#define WTF_PLATFORM_CAIRO 1
#endif

#ifdef __S60__
// we are cross-compiling, it is not really windows
#undef WTF_PLATFORM_WIN_OS
#undef WTF_PLATFORM_WIN
#undef WTF_PLATFORM_CAIRO
#define WTF_PLATFORM_S60 1
#define WTF_PLATFORM_SYMBIAN 1
#endif

/* CPU */

#if defined(mips) || defined(__mips__) || defined(MIPS) || defined(_MIPS_)
// according to Fu, Chao-Ying<fu@mips.com>; email, MIPS should disable the JIT

#define WTF_PLATFORM_MIPS 1
/* #define WTF_PLATFORM_BIG_ENDIAN 1 // If your CPU is big endian */
#define ENABLE_JIT 0

#else
/* CPU(MIPS) - MIPS 32-bit */
/* Note: Only O32 ABI is tested, so we enable it for O32 ABI for now.  */
#if (defined(mips) || defined(__mips__) || defined(MIPS) || defined(_MIPS_)) \
    && defined(_ABIO32)
#define WTF_PLATFORM_MIPS 1
#if defined(__MIPSEB__)
#define WTF_PLATFORM_BIG_ENDIAN 1
#endif
#define WTF_MIPS_PIC (defined __PIC__)
#define WTF_MIPS_ARCH __mips
#define WTF_MIPS_ISA(v) (defined WTF_MIPS_ARCH && WTF_MIPS_ARCH == v)
#define WTF_MIPS_ISA_AT_LEAST(v) (defined WTF_MIPS_ARCH && WTF_MIPS_ARCH >= v)
#define WTF_MIPS_ARCH_REV __mips_isa_rev
#define WTF_MIPS_ISA_REV(v) (defined WTF_MIPS_ARCH_REV && WTF_MIPS_ARCH_REV == v)
#define WTF_MIPS_DOUBLE_FLOAT (defined __mips_hard_float && !defined __mips_single_float)
#define WTF_MIPS_FP64 (defined __mips_fpr && __mips_fpr == 64)
/* MIPS requires allocators to use aligned memory */
#define WTF_USE_ARENA_ALLOC_ALIGNMENT_INTEGER 1
#endif /* MIPS */
#endif

/* PLATFORM(PPC) */
#if   defined(__ppc__)     \
   || defined(__PPC__)     \
   || defined(__powerpc__) \
   || defined(__powerpc)   \
   || defined(__POWERPC__) \
   || defined(_M_PPC)      \
   || defined(__PPC)
#define WTF_PLATFORM_PPC 1
#define WTF_PLATFORM_BIG_ENDIAN 1
#endif

/* PLATFORM(PPC64) */
#if   defined(__ppc64__) \
   || defined(__PPC64__)
#define WTF_PLATFORM_PPC64 1
#define WTF_PLATFORM_BIG_ENDIAN 1
#endif

/* PLATFORM(ARM) */
#if   defined(arm) \
	|| defined(__arm__)\
	|| defined(ARM) \
	|| defined(_ARM_)
#define WTF_PLATFORM_ARM 1
#if defined(__ARMEB__)
#define WTF_PLATFORM_BIG_ENDIAN 1
#elif !defined(__ARM_EABI__) && !defined(__ARMEB__) && !defined(__VFP_FP__)
#define WTF_PLATFORM_MIDDLE_ENDIAN 1
#endif
#if !defined(__ARM_EABI__)
#define WTF_PLATFORM_FORCE_PACK 1
#endif
#endif

/* PLATFORM(X86) */
#if   defined(__i386__) \
   || defined(i386)     \
   || defined(_M_IX86)  \
   || defined(_X86_)    \
   || defined(__THW_INTEL)
#define WTF_PLATFORM_X86 1
#endif

/* PLATFORM(X86_64) */
#if   defined(__x86_64__) \
   || defined(__ia64__) \
   || defined(_M_X64)
#define WTF_PLATFORM_X86_64 1
#endif

/* Compiler */

/* COMPILER(MSVC) */
#if defined(_MSC_VER)
#define WTF_COMPILER_MSVC 1
#if _MSC_VER < 1400
#define WTF_COMPILER_MSVC7 1
#endif
#endif

/* COMPILER(GCC) */
#if defined(__GNUC__)
#define WTF_COMPILER_GCC 1
#endif

/* COMPILER(MINGW) */
#if defined(MINGW) || defined(__MINGW32__)
#define WTF_COMPILER_MINGW 1
#endif

/* COMPILER(BORLAND) */
/* not really fully supported - is this relevant any more? */
#if defined(__BORLANDC__)
#define WTF_COMPILER_BORLAND 1
#endif

/* COMPILER(CYGWIN) */
/* not really fully supported - is this relevant any more? */
#if defined(__CYGWIN__)
#define WTF_COMPILER_CYGWIN 1
#endif

/* multiple threads only supported on Mac for now */
#if PLATFORM(MAC) || PLATFORM(WIN)
#define WTF_USE_MULTIPLE_THREADS 1
#endif

/* for Unicode, KDE uses Qt */
#if PLATFORM(KDE) || PLATFORM(QT)
#define WTF_USE_QT4_UNICODE 1
#elif PLATFORM(SYMBIAN)
#define WTF_USE_SYMBIAN_UNICODE 1
#elif PLATFORM(GTK)
/* The GTK+ Unicode backend is configurable */
#else
#if !USE(CASQT_UNICODE)
#define WTF_USE_ICU_UNICODE 1
#endif
#endif

#if PLATFORM(MAC)
#define WTF_PLATFORM_CF 1
#define WTF_USE_PTHREADS 1
#define ENABLE_MAC_JAVA_BRIDGE 1
#define HAVE_READLINE 1
#endif

#if PLATFORM(MAC) || PLATFORM(GTK) || PLATFORM(SYMBIAN) || PLATFORM(WIN) || PLATFORM(WX)
#define ENABLE_NETSCAPE_PLUGIN_API 1
#endif

#if PLATFORM(WIN)
#define WTF_USE_WININET 1
#endif

#if PLATFORM(WX)
#define WTF_USE_CURL 1
#define WTF_USE_PTHREADS 1
#endif

#if PLATFORM(MAC) || PLATFORM(WIN)
#define ENABLE_DASHBOARD_SUPPORT 1
#endif

#if PLATFORM(MAC) || PLATFORM(WIN) || PLATFORM(GTK)
#define HAVE_ACCESSIBILITY 1
#endif

#if COMPILER(GCC)
#define HAVE_COMPUTED_GOTO 1
#endif

#if PLATFORM(DARWIN)

#error

#define HAVE_ERRNO_H 1
#define HAVE_MMAP 1
#define HAVE_MERGESORT 1
#define HAVE_SBRK 1
#define HAVE_STRINGS_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_TIMEB_H 1

#elif PLATFORM(WIN_OS)

#if PLATFORM(WINCE_OS)
#define HAVE_ERRNO_H 0
#else
#define HAVE_FLOAT_H 1
#define HAVE_SYS_TIMEB_H 1
#define HAVE_VIRTUALALLOC 1     // To consider: Disable this because we don't want WebKit using this behind our backs.
#endif

#elif PLATFORM(XBOX)

#define HAVE_FLOAT_H 1
#define HAVE_SYS_TIMEB_H 1
// #define HAVE_VIRTUALALLOC 1  // Disabled because we don't want to use it, even though it is present.

#elif PLATFORM(PS3)

#define HAVE_FLOAT_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_ERRNO_H 1
// #define HAVE_STDINT_H 1  // Already defined by kjs/config.h
// #define HAVE_STRING_H 1  // Already defined by kjs/config.h

#else

/* FIXME: is this actually used or do other platforms generate their own config.h? */

#error

#define HAVE_ERRNO_H 1
#define HAVE_MMAP 1
#define HAVE_SBRK 1
#define HAVE_STRINGS_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_SYS_TIME_H 1

#endif

/* ENABLE macro defaults */
   
#if !defined(ENABLE_FAST_MALLOC_MATCH_VALIDATION)
#define ENABLE_FAST_MALLOC_MATCH_VALIDATION 0
#endif

#if !defined(ENABLE_ICONDATABASE)
#define ENABLE_ICONDATABASE 1
#endif

#if !defined(ENABLE_DATABASE)
#define ENABLE_DATABASE 1
#endif

#if !defined(ENABLE_FTPDIR)
#define ENABLE_FTPDIR 1
#endif

#if !defined(ENABLE_MAC_JAVA_BRIDGE)
#define ENABLE_MAC_JAVA_BRIDGE 0
#endif

#if !defined(ENABLE_NETSCAPE_PLUGIN_API)
#define ENABLE_NETSCAPE_PLUGIN_API 0
#endif

#if !defined(ENABLE_DASHBOARD_SUPPORT)
#define ENABLE_DASHBOARD_SUPPORT 0
#endif

#endif /* WTF_Platform_h */
