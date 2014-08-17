/* -*- mode: c; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2003, 2006, 2007 Apple Inc.  All rights reserved.
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
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#ifndef WTF_Assertions_h
#define WTF_Assertions_h

/*
   no namespaces because this file has to be includable from C and Objective-C

   Note, this file uses many GCC extensions, but it should be compatible with
   C, Objective C, C++, and Objective C++.

   For non-debug builds, everything is disabled by default.
   Defining any of the symbols explicitly prevents this from having any effect.
   
   MSVC7 note: variadic macro support was added in MSVC8, so for now we disable
   those macros in MSVC7. For more info, see the MSDN document on variadic 
   macros here:
   
   http://msdn2.microsoft.com/en-us/library/ms177415(VS.80).aspx
*/

#include <wtf/Platform.h>
#include <EABase/eabase.h>


#if COMPILER(MSVC)
    #include <stddef.h>
#else
    #include <inttypes.h>
#endif

#ifdef NDEBUG
    #define ASSERTIONS_DISABLED_DEFAULT 1
#else
    #define ASSERTIONS_DISABLED_DEFAULT 0
#endif

#ifndef ASSERT_DISABLED
    #define ASSERT_DISABLED ASSERTIONS_DISABLED_DEFAULT
#endif

#ifndef ASSERT_ARG_DISABLED
    #define ASSERT_ARG_DISABLED ASSERTIONS_DISABLED_DEFAULT
#endif

#ifndef FATAL_DISABLED
    #define FATAL_DISABLED ASSERTIONS_DISABLED_DEFAULT
#endif

#ifndef ERROR_DISABLED
    #define ERROR_DISABLED ASSERTIONS_DISABLED_DEFAULT
#endif

#ifndef LOG_DISABLED
    #define LOG_DISABLED ASSERTIONS_DISABLED_DEFAULT
#endif

#if COMPILER(GCC)
    #define WTF_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else
    #define WTF_PRETTY_FUNCTION __FUNCTION__
#endif

/* WTF logging functions can process %@ in the format string to log a NSObject* but the printf format attribute
   emits a warning when %@ is used in the format string.  Until <rdar://problem/5195437> is resolved we can't include
   the attribute when being used from Objective-C code in case it decides to use %@. */
#if COMPILER(GCC) && !defined(__OBJC__)
    #define WTF_ATTRIBUTE_PRINTF(formatStringArgument, extraArguments) __attribute__((__format__(printf, formatStringArgument, extraArguments)))
#else
    #define WTF_ATTRIBUTE_PRINTF(formatStringArgument, extraArguments) 
#endif

/* These helper functions are always declared, but not necessarily always defined if the corresponding function is disabled. */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { WTFLogChannelOff, WTFLogChannelOn } WTFLogChannelState;

typedef struct {
    unsigned mask;
    const char *defaultName;
    WTFLogChannelState state;
} WTFLogChannel;

void WTFReportAssertionFailure(const char* file, int line, const char* function, const char* assertion);
void WTFReportAssertionFailureWithMessage(const char* file, int line, const char* function, const char* assertion, const char* format, ...) WTF_ATTRIBUTE_PRINTF(5, 6);
void WTFReportArgumentAssertionFailure(const char* file, int line, const char* function, const char* argName, const char* assertion);
void WTFReportFatalError(const char* file, int line, const char* function, const char* format, ...) WTF_ATTRIBUTE_PRINTF(4, 5);
void WTFReportError(const char* file, int line, const char* function, const char* format, ...) WTF_ATTRIBUTE_PRINTF(4, 5);
void WTFLog(WTFLogChannel* channel, const char* format, ...) WTF_ATTRIBUTE_PRINTF(2, 3);
void WTFLogEvent(const char* format, ...) WTF_ATTRIBUTE_PRINTF(1, 2);
void WTFLogVerbose(const char* file, int line, const char* function, WTFLogChannel* channel, const char* format, ...) WTF_ATTRIBUTE_PRINTF(5, 6);

#ifdef __cplusplus
}
#endif



///////////////////////////////////////////////////////////////////////////////
/// EA_DEBUG_BREAK
///
/// This function causes an app to immediately stop under the debugger.
/// It is implemented as a macro in order to all stopping at the site 
/// of the call.
///
/// Example usage:
///    EA_DEBUG_BREAK();
///
/// The EA_DEBUG_BREAK function must be implemented on a per-platform basis. 
/// On a PC, you would normally define this to function to be the inline
/// assembly: "asm int 3", which tells the debugger to stop here immediately.
/// A very basic platform-independent implementation of EA_DEBUG_BREAK could 
/// be the following:
///   void EA_DEBUG_BREAK()
///   {
///      atoi(""); // Place a breakpoint here if you want to catch breaks.
///   }
/// 
/// The EA_DEBUG_BREAK default behaviour here can be disabled or changed by 
/// globally defining EA_DEBUG_BREAK_DEFINED and implementing an alternative
/// implementation of it. Our implementation here doesn't simply always have
/// it be defined externally because a major convenience of EA_DEBUG_BREAK
/// being inline is that it stops right on the troublesome line of code and
/// not in another function.
///
#if !defined(EA_DEBUG_BREAK)
    #if defined(EA_DEBUG_BREAK_ENABLED)
        #ifndef EA_DEBUG_BREAK_DEFINED
            #define EA_DEBUG_BREAK_DEFINED

            #if defined(EA_COMPILER_MSVC) && (_MSC_VER >= 1300)
                #define EA_DEBUG_BREAK() __debugbreak() // This is a compiler intrinsic which will map to appropriate inlined asm for the platform.
            #elif defined(EA_PROCESSOR_MIPS)    // 
                #define EA_DEBUG_BREAK() asm("break")
            #elif defined(__SNC__)
                #define EA_DEBUG_BREAK() *(int*)(0) = 0
            #elif defined(EA_PLATFORM_PS3)
                #define EA_DEBUG_BREAK() asm volatile("tw 31,1,1")
            #elif defined(EA_PROCESSOR_POWERPC) // Generic PowerPC. This triggers an exception by executing opcode 0x00000000.
                #define EA_DEBUG_BREAK() asm(".long 0")
            #elif defined(EA_PROCESSOR_X86) && defined(EA_ASM_STYLE_INTEL)
                #define EA_DEBUG_BREAK() { __asm int 3 }
            #elif defined(EA_PROCESSOR_X86) && defined(EA_ASM_STYLE_ATT)
                #define EA_DEBUG_BREAK() asm("int3") 
            #else
                void EA_DEBUG_BREAK(); // User must define this externally.
            #endif
        #else
            void EA_DEBUG_BREAK(); // User must define this externally.
        #endif
    #else
        #define EA_DEBUG_BREAK()
    #endif
#else
    #ifndef EA_DEBUG_BREAK_DEFINED
        #define EA_DEBUG_BREAK_DEFINED
    #endif
#endif



/* CRASH -- gets us into the debugger or the crash reporter -- signals are ignored by the crash reporter so we must do better */

#ifndef CRASH
    #define CRASH() EA_DEBUG_BREAK()
#endif



/* ASSERT, ASSERT_WITH_MESSAGE, ASSERT_NOT_REACHED */

#if PLATFORM(WIN_OS)
    /* FIXME: Change to use something other than ASSERT to avoid this conflict with win32. */
    #undef ASSERT
#endif

#if ASSERT_DISABLED

    #define ASSERT(assertion) ((void)0)
    #define ASSERT_WITH_MESSAGE(assertion, ...) ((void)0)
    #define ASSERT_NOT_REACHED() ((void)0)

#else

    #define ASSERT(assertion) do \
        if (!(assertion)) { \
            WTFReportAssertionFailure(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, #assertion); \
            CRASH(); \
        } \
    while (0)

    #if COMPILER(MSVC7)
        #define ASSERT_WITH_MESSAGE(assertion) ((void)0)
    #else
        #define ASSERT_WITH_MESSAGE(assertion, ...) do \
            if (!(assertion)) { \
                WTFReportAssertionFailureWithMessage(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, #assertion, __VA_ARGS__); \
                CRASH(); \
            } \
        while (0)
    #endif /* COMPILER(MSVC7) */

    #define ASSERT_NOT_REACHED() do { \
        WTFReportAssertionFailure(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, 0); \
        CRASH(); \
    } while (0)

#endif



/* ASSERT_ARG */

#if ASSERT_ARG_DISABLED

    #define ASSERT_ARG(argName, assertion) ((void)0)

#else

    #define ASSERT_ARG(argName, assertion) do \
        if (!(assertion)) { \
            WTFReportArgumentAssertionFailure(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, #argName, #assertion); \
            CRASH(); \
        } \
    while (0)

#endif



/* COMPILE_ASSERT */
#ifndef COMPILE_ASSERT
    #define COMPILE_ASSERT(exp, name) typedef int dummy##name [(exp) ? 1 : -1];
#endif



/* FATAL */

#if FATAL_DISABLED
    #define FATAL(...) ((void)0)
#elif COMPILER(MSVC7)
    #define FATAL() ((void)0)
#else
    #define FATAL(...) do { \
        WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, __VA_ARGS__); \
        CRASH(); \
    } while (0)
#endif



/* LOG_ERROR */

#if ERROR_DISABLED
    #define LOG_ERROR(...) ((void)0)
#elif COMPILER(MSVC7)
    #define LOG_ERROR() ((void)0)
#else
    #define LOG_ERROR(...) WTFReportError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, __VA_ARGS__)
#endif



/* LOG */

#if LOG_DISABLED
    #define LOG(channel, ...) ((void)0)
#elif COMPILER(MSVC7)
    #define LOG() ((void)0)
#else
    #define LOG(channel, ...) WTFLog(&JOIN_LOG_CHANNEL_WITH_PREFIX(LOG_CHANNEL_PREFIX, channel), __VA_ARGS__)
    #define JOIN_LOG_CHANNEL_WITH_PREFIX(prefix, channel) JOIN_LOG_CHANNEL_WITH_PREFIX_LEVEL_2(prefix, channel)
    #define JOIN_LOG_CHANNEL_WITH_PREFIX_LEVEL_2(prefix, channel) prefix ## channel
#endif



/* LOG_VERBOSE */

#if LOG_DISABLED
    #define LOG_VERBOSE(channel, ...) ((void)0)
#elif COMPILER(MSVC7)
    #define LOG_VERBOSE(channel) ((void)0)
#else
    #define LOG_VERBOSE(channel, ...) WTFLogVerbose(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, &JOIN_LOG_CHANNEL_WITH_PREFIX(LOG_CHANNEL_PREFIX, channel), __VA_ARGS__)
#endif


#endif /* WTF_Assertions_h */
