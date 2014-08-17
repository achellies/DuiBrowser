// -*- mode: c++; c-basic-offset: 4 -*-
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
* This file was modified by Electronic Arts Inc Copyright ?2009-2010
*/

#include "config.h"
#include "Assertions.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <Logging.h>
#include <EAWebKit/EAWebKit.h>
#include <wtf/FastAllocBase.h>
#include <EAWebKit/internal/EAWebKitViewHelper.h> // For multiview support

#if COMPILER(MSVC)
	#if !PLATFORM(WINCE_OS)
		#include <crtdbg.h>
	#endif

    #if PLATFORM(WIN_OS)
        #include <windows.h>
    #elif PLATFORM(XBOX)
        #include <comdecl.h>
    #endif

#endif



static void printf_user_common_str(bool bAssert, const char* pOutput)
{
    bool bHandled = false;
    EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();

    if(pVN)
    {
        if(bAssert)
        {
            EA::WebKit::AssertionFailureInfo afi = { EA::WebKit::AutoSetActiveView::GetActiveView(), pOutput };
            bHandled = pVN->AssertionFailure(afi);
        }
        else
        {
            EA::WebKit::DebugLogInfo dli = { EA::WebKit::AutoSetActiveView::GetActiveView(), pOutput, EA::WebKit::kDebugLogAssertion };
            bHandled = pVN->DebugLog(dli);
        }
    }
	if(!bHandled)
	{
		//Note by Arpit Baldeva: Use the platform calls and not the EAWebKit output macro here. The EAWebKit output macro is implemented in terms of 
		//this function call itself. Calling it from here would cause a cyclic loop and causes EAWebKit to crash on the shutdown.
		//This happens, for example, if you shutdown EAWebKit and some things keep trying to access functionality from the library.
		//2nd part of this would be to fix such cases.
		//Since the log function is wrapped behind the logging preprocessor, it would be compiled out in the release build.
#if EAWEBKIT_DEFAULT_LOG_HANDLING_ENABLED  // This defaults to false in release builds.
	#if defined(_MSC_VER)
		#if PLATFORM(WINCE_OS)
			printf("%s", pOutput);
			printf("\n");
		#else
			OutputDebugStringA(pOutput);
			OutputDebugStringA("\n");
		#endif
	#else
		printf("%s", pOutput);
		printf("\n");
	#endif
#endif
	}
}


static void vprintf_user_common(bool bAssert, const char* format, va_list args)
{
    char*  pBufferAllocated = NULL;
	const unsigned int initial_size = 512;
	size_t size = initial_size;
	char   pBufferLocal[initial_size];
    char*  pBufferUsed = pBufferLocal;

    do {
        #if defined(_MSC_VER)
            int result = _vsnprintf(pBufferUsed, size, format, args);
        #else
            int result = vsnprintf(pBufferUsed, size, format, args);
        #endif

        if ((result != -1))
        {
            printf_user_common_str(bAssert, pBufferUsed);
            break;
        }

        EAWEBKIT_DELETE[] pBufferAllocated;//WTF::fastDeleteArray<char>(pBufferAllocated);
        pBufferAllocated = NULL;

        size *= 2;
		pBufferAllocated = pBufferUsed = EAWEBKIT_NEW("debug printf") char[size];//WTF::fastNewArray<char>(size);    // To do: Convert this usage of new to our custom allocator.
    } while ((size < 8192) && pBufferUsed);

    EAWEBKIT_DELETE[] pBufferAllocated;//WTF::fastDeleteArray<char>(pBufferAllocated);
}


static void printf_user_common(bool bAssert, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf_user_common(bAssert, format, args);
    va_end(args);
}




extern "C" {


void WTFReportAssertionFailure(const char* file, int line, const char* function, const char* assertion)
{
    if (assertion)
        printf_user_common(true, "WebKit assertion failure: %s\n(%s:%d %s)\n", assertion, file, line, function);
    else
        printf_user_common(true, "WebKit should never be reached\n(%s:%d %s)\n", file, line, function);
}


void WTFReportAssertionFailureWithMessage(const char* file, int line, const char* function, const char* assertion, const char* format, ...)
{
    // printf_user_common("ASSERTION FAILED: ");

    // Problem: We are doing to assertions here instead of one. We need to fix this, but it's a something of a pain to do so.
    va_list args;
    va_start(args, format);
    vprintf_user_common(true, format, args);
    va_end(args);

    printf_user_common("\n%s\n(%s:%d %s)\n", assertion, file, line, function);
}


void WTFReportArgumentAssertionFailure(const char* file, int line, const char* function, const char* argName, const char* assertion)
{
    printf_user_common(true, "WebKit bad argument: %s, %s\n(%s:%d %s)\n", argName, assertion, file, line, function);
}


void WTFReportFatalError(const char* file, int line, const char* function, const char* format, ...)
{
    printf_user_common(false, "WebKit fatal error: ");

    va_list args;
    va_start(args, format);
    vprintf_user_common(false, format, args);
    va_end(args);

    printf_user_common(false, "\n(%s:%d %s)\n", file, line, function);
}


void WTFReportError(const char* file, int line, const char* function, const char* format, ...)
{
    printf_user_common(false, "WebKit error: ");

    va_list args;
    va_start(args, format);
    vprintf_user_common(false, format, args);
    va_end(args);

    printf_user_common(false, "\n(%s:%d %s)\n", file, line, function);
}


void WTFLog(WTFLogChannel* channel, const char* format, ...)
{
    if (channel->state == WTFLogChannelOn)
    {
        va_list args;
        va_start(args, format);
        vprintf_user_common(false, format, args);
        va_end(args);

        //if (format[strlen(format) - 1] != '\n')
        //    printf_user_common(false, "\n");
    }
}


void WTFLogEvent(const char* format, ...)
{
    if (WebCore::LogEvents.state == WTFLogChannelOn)
    {
        va_list args;
        va_start(args, format);
        vprintf_user_common(false, format, args);
        va_end(args);

        //if (format[strlen(format) - 1] != '\n')
        //    printf_user_common(false, "\n");
    }
}


void WTFLogVerbose(const char* file, int line, const char* function, WTFLogChannel* channel, const char* format, ...)
{
    if (channel->state == WTFLogChannelOn)
    {
        va_list args;
        va_start(args, format);
        vprintf_user_common(false, format, args);
        va_end(args);

        if (format[strlen(format) - 1] == '\n')
            printf_user_common(false,   "(%s:%d %s)\n", file, line, function);
        else
            printf_user_common(false, "\n(%s:%d %s)\n", file, line, function);
    }
}

} // extern "C"
