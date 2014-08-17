/*
Copyright (C) 2009-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Copyright (c) 2006 Electronic Arts Inc. All rights reserved.

#ifndef EAASSERT_EAASSERT_H
#define EAASSERT_EAASSERT_H

// Users can #define EA_ASSERT_HAVE_OWN_HEADER to their own header file and provide their
// own definition of EA_ASSERT. Make sure to include quotes in your 
// definition, i.e. #define EA_ASSERT_HAVE_OWN_HEADER "my_game/my_game_assert.h"
//
// At minimum, this header needs to provide the following:
//
//      EA_ASSERT(expr)
//      EA_ASSERT_MSG(expr, msg)
//      EA_ASSERT_FORMATTED(expr, fmt)
//      EA_FAIL()
//      EA_FAIL_MSG(msg)
//      EA_FAIL_FORMATTED(fmt)
//      EA_COMPILETIME_ASSERT(expr)
//
//  Where:
//  
//      expr    is an expression that evaluates to a boolean
//      msg     is a string (char*)
//      fmt     is a tuple wrapped in parentheses where the first message is 
//              a formatted string (char*) and the rest is printf style parameters
//
#ifdef EA_ASSERT_HAVE_OWN_HEADER
    #include EA_ASSERT_HAVE_OWN_HEADER

#else // The user did not supply their own assert definition, so we'll use ours instead


#include "EABase/eabase.h"
#include <stdarg.h>

// The default assert handling mechanism just breaks into the debugger on an assert failure. If you wish to overwrite this 
// functionality, simply implement the following callback:
//
//      bool EAAssertFailure(
//              const char* expr, 
//              const char* filename,
//              int line,
//              const char* function,
//              const char* msg,
//              va_list args)
//
// And then use:
//
//      EA::Assert::SetCallback(&yourCallback);
//

#if defined(EA_DLL) && defined(_MSC_VER)
    #ifndef EA_ASSERT_API
        #define EA_ASSERT_API __declspec(dllimport)
    #endif
#else
    #define EA_ASSERT_API
#endif

#ifdef __cplusplus
namespace EA { 
namespace Assert {

    typedef bool (*FailureCallback)(const char*, const char*, int, const char*, const char*, va_list);

    EA_ASSERT_API   FailureCallback GetFailureCallback();
    EA_ASSERT_API   void            SetFailureCallback(FailureCallback failureCallback);
}}
#endif

#if ! defined EA_DEBUG_BREAK && ! defined EA_DEBUG_BREAK_DEFINED
    #ifdef EA_PLATFORM_PS3
      #if defined(__SN_VER__)
		#define EA_DEBUG_BREAK() *(int*)(0) = 0
      #else
        #define EA_DEBUG_BREAK() __asm__ volatile ("tw 31,1,1")
      #endif
    #elif defined EA_PLATFORM_XENON
        #define EA_DEBUG_BREAK() __debugbreak()
    #elif defined EA_PLATFORM_WINDOWS
        #define EA_DEBUG_BREAK() __debugbreak()
	#elif defined(EA_PLATFORM_WINCE)
		#define EA_DEBUG_BREAK() __debugbreak()
    #else
        #define EA_DEBUG_BREAK() *(int*)(0) = 0
    #endif

    #define EA_DEBUG_BREAK_DEFINED
#endif

#ifndef EA_CURRENT_FUNCTION
    #if defined __GNUC__ || (defined __ICC && __ICC >= 600)
        #define EA_CURRENT_FUNCTION __PRETTY_FUNCTION__
    #elif defined(__FUNCSIG__)
        #define EA_CURRENT_FUNCTION __FUNCSIG__
    #elif (defined __INTEL_COMPILER && __INTEL_COMPILER >= 600) || (defined __IBMCPP__ && __IBMCPP__ >= 500) || (defined __CWCC__ && __CWCC__ >= 0x4200)
        #define EA_CURRENT_FUNCTION __FUNCTION__
    #elif defined __BORLANDC__ && __BORLANDC__ >= 0x550
        #define EA_CURRENT_FUNCTION __FUNC__
    #elif defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901
        #define EA_CURRENT_FUNCTION __func__
    #elif defined __CWCC__ && __CWCC__ < 0x4200
        #define EA_CURRENT_FUNCTION 0
    #else
        #define EA_CURRENT_FUNCTION "(unknown function)"
    #endif
#endif

#ifdef __cplusplus
namespace EA { 
namespace Assert { 
namespace Detail 
{
    #ifdef EA_PLATFORM_PS3_SPU
        // Not needed on the SPU
    #else
        // I'm not happy about this, but it seems the least-ugly way to pass the formatted msg
        // to the user-supplied callback. Too bad it can't take a ... (ellipsis) instead of a va_list though. I would
        // love to see a less complicated design here, and am open for suggestions.
        struct Forwarder
        { 
            const char* mExpr;
            const char* mFunction;
            const char* mFilename;
            int mLine;
            Forwarder(const char* expr, const char* filename, int line,  const char* function) : mExpr(expr), mFunction(function), mFilename(filename), mLine(line) {}
            bool Call(const char* msg, ...) const
            {
                va_list args;
                va_start(args, msg);
                bool ret = (*GetFailureCallback())(mExpr, mFilename, mLine, mFunction, msg, args);
                va_end(args);
                return ret;
            }
        };
    #endif
}}}
#endif

#ifndef EA_COMPILETIME_ASSERT
    #define EAASSERT_TOKEN_PASTE(a,b) a ## b
    #define EAASSERT_CONCATENATE_HELPER(a,b) EAASSERT_TOKEN_PASTE(a,b)
    #define EA_COMPILETIME_ASSERT(expr) typedef char EAASSERT_CONCATENATE_HELPER(compileTimeAssert,__LINE__) [((expr) != 0) ? 1 : -1]
#endif

#ifdef EA_DEBUG
    #ifndef EA_ASSERT_ENABLED
        #define EA_ASSERT_ENABLED
    #endif
#endif

#ifdef EA_ASSERT_ENABLED    
    #ifdef __cplusplus
        #ifndef EA_PLATFORM_PS3_SPU
            #ifndef EA_ASSERT
				#define EA_ASSERT(expr)                if (!(expr) && EA::Assert::Detail::Forwarder(#expr,     __FILE__, __LINE__, EA_CURRENT_FUNCTION).Call("none"))   EA_DEBUG_BREAK(); else ((void)0)
            #endif
            #ifndef EA_ASSERT_MSG
                #define EA_ASSERT_MSG(expr, msg)       if (!(expr) && EA::Assert::Detail::Forwarder(#expr,     __FILE__, __LINE__, EA_CURRENT_FUNCTION).Call(msg))      EA_DEBUG_BREAK(); else ((void)0)
            #endif
            #ifndef EA_ASSERT_FORMATTED
                #define EA_ASSERT_FORMATTED(expr, fmt) if (!(expr) && EA::Assert::Detail::Forwarder(#expr,     __FILE__, __LINE__, EA_CURRENT_FUNCTION).Call fmt)       EA_DEBUG_BREAK(); else ((void)0)
            #endif
            #ifndef EA_FAIL
                #define EA_FAIL()                      if (           EA::Assert::Detail::Forwarder("EA_FAIL", __FILE__, __LINE__, EA_CURRENT_FUNCTION).Call("none"))   EA_DEBUG_BREAK(); else ((void)0)
            #endif
            #ifndef EA_FAIL_MSG
                #define EA_FAIL_MSG(msg)               if (           EA::Assert::Detail::Forwarder("EA_FAIL", __FILE__, __LINE__, EA_CURRENT_FUNCTION).Call(msg))      EA_DEBUG_BREAK(); else ((void)0)
            #endif
            #ifndef EA_FAIL_FORMATTED
                #define EA_FAIL_FORMATTED(fmt)         if (           EA::Assert::Detail::Forwarder("EA_FAIL", __FILE__, __LINE__, EA_CURRENT_FUNCTION).Call fmt)       EA_DEBUG_BREAK(); else ((void)0)
            #endif
        #else
          
        #endif
    #else
            #ifndef EA_ASSERT
                #define EA_ASSERT(expr)                if (!(expr)) { printf("\nEA_ASSERT(%s) failed in %s(%d)\n", #expr, __FILE__, __LINE__); EA_DEBUG_BREAK(); } else ((void)0)
            #endif
            #ifndef EA_ASSERT_MSG
                #define EA_ASSERT_MSG(expr, msg)       if (!(expr)) { printf("\nEA_ASSERT(%s) failed in %s(%d): %s\n", #expr, __FILE__, __LINE__, msg); EA_DEBUG_BREAK(); } else ((void)0)
            #endif
            #ifndef EA_ASSERT_FORMATTED
                #define EA_ASSERT_FORMATTED(expr, fmt) if (!(expr)) { printf fmt; EA_DEBUG_BREAK(); } else ((void)0)
            #endif
            #ifndef EA_FAIL
				#define EA_FAIL()                      if(true) { printf("\nEA_FAIL: %s(%d)\n", __FILE__, __LINE__); EA_DEBUG_BREAK(); } else ((void)0)
            #endif
            #ifndef EA_FAIL_MSG
                #define EA_FAIL_MSG(msg)               if(true) { printf("\nEA_FAIL: %s(%d): %s\n", __FILE__, __LINE__, msg); EA_DEBUG_BREAK(); } else ((void)0)
            #endif
            #ifndef EA_FAIL_FORMATTED
				#define EA_FAIL_FORMATTED(fmt)         if(true) { printf fmt; EA_DEBUG_BREAK(); } else ((void)0)
            #endif
    #endif
#else
    #ifndef EA_ASSERT
        #define EA_ASSERT(expr)                 ((void)0)
    #endif
    #ifndef EA_ASSERT_MSG
        #define EA_ASSERT_MSG(expr, msg)        ((void)0)
    #endif
    #ifndef EA_ASSERT_FORMATTED
        #define EA_ASSERT_FORMATTED(expr, fmt)  ((void)0)
    #endif
    #ifndef EA_FAIL
        #define EA_FAIL()                       ((void)0)
    #endif
    #ifndef EA_FAIL_MSG
        #define EA_FAIL_MSG(msg)                ((void)0)
    #endif
    #ifndef EA_FAIL_FORMATTED
        #define EA_FAIL_FORMATTED(fmt)          ((void)0)
    #endif
#endif

#endif

// We provide some synonyms for the message style macros, because we couldn't reach common ground
// and there was too much existing code using different versions. It's not pretty, but relatively harmless
#ifndef EA_ASSERT_M
    #define EA_ASSERT_M EA_ASSERT_MSG
#endif
#ifndef EA_ASSERT_MESSAGE 
    #define EA_ASSERT_MESSAGE EA_ASSERT_MSG
#endif
#ifndef EA_FAIL_M
    #define EA_FAIL_M EA_FAIL_MSG
#endif
#ifndef EA_FAIL_MESSAGE
    #define EA_FAIL_MESSAGE EA_FAIL_MSG
#endif

#endif
