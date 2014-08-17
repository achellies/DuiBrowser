/*
Copyright (C) 2009 Electronic Arts, Inc.  All rights reserved.

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

///////////////////////////////////////////////////////////////////////////////
// Config.h
//
// Copyright (c) 2006 Electronic Arts Inc.
// Written and maintained by Paul Pedriana.
///////////////////////////////////////////////////////////////////////////////


#ifndef UTFXML_INTERNAL_CONFIG_H
#define UTFXML_INTERNAL_CONFIG_H


#include <EABase/eabase.h>


///////////////////////////////////////////////////////////////////////////////
// UTFXML_VERSION
//
// We more or less follow the conventional EA packaging approach to versioning 
// here. A primary distinction here is that minor versions are defined as two
// digit entities (e.g. .03") instead of minimal digit entities ".3"). The logic
// here is that the value is a counter and not a floating point fraction.
// Note that the major version doesn't have leading zeros.
//
// Example version strings:
//      "0.91.00"   // Major version 0, minor version 91, patch version 0. 
//      "1.00.00"   // Major version 1, minor and patch version 0.
//      "3.10.02"   // Major version 3, minor version 10, patch version 02.
//     "12.03.01"   // Major version 12, minor version 03, patch version 
//
// Example usage:
//     printf("UTFXML version: %s", UTFXML_VERSION);
//     printf("UTFXML version: %d.%d.%d", UTFXML_VERSION_N / 10000 % 100, UTFXML_VERSION_N / 100 % 100, UTFXML_VERSION_N % 100);
//
///////////////////////////////////////////////////////////////////////////////

#ifndef UTFXML_VERSION
    #define UTFXML_VERSION   "3.06.08"
    #define UTFXML_VERSION_N  30608
#endif


///////////////////////////////////////////////////////////////////////////////
// UTFXML_ALLOC_PREFIX
//
// Defined as a string literal. Defaults to this package's name.
// Can be overridden by the user by predefining it or by editing this file.
// This define is used as the default name used by this package for naming
// memory allocations and memory allocators.
//
// All allocations names follow the same naming pattern:
//     <package>/<module>[/<specific usage>]
// 
// Example usage:
//     void* p = pCoreAllocator->Alloc(37, UTFXML_ALLOC_PREFIX, 0);
//
// Example usage:
//     gMessageServer.GetMessageQueue().get_allocator().set_name(UTFXML_ALLOC_PREFIX "MessageSystem/Queue");
//
#ifndef UTFXML_ALLOC_PREFIX
    #define UTFXML_ALLOC_PREFIX "UTFXml/"
#endif



///////////////////////////////////////////////////////////////////////////////
// UTF_USE_EATRACE    Defined as 0 or 1. Default is 1 but may be overridden by your project file.
// UTF_USE_EAASSERT   Defined as 0 or 1. Default is 0.
//
// Defines whether this package uses the EATrace or EAAssert package to do assertions.
// EAAssert is a basic lightweight package for portable asserting.
// EATrace is a larger "industrial strength" package for assert/trace/log.
// If both UTF_USE_EATRACE and UTF_USE_EAASSERT are defined as 1, then UTF_USE_EATRACE
// overrides UTF_USE_EAASSERT.
//
#if defined(UTF_USE_EATRACE) && UTF_USE_EATRACE
    #undef  UTF_USE_EAASSERT
    #define UTF_USE_EAASSERT 0
#elif !defined(UTF_USE_EAASSERT)
    #define UTF_USE_EAASSERT 0
#endif

#undef UTF_USE_EATRACE
#if defined(UTF_USE_EAASSERT) && UTF_USE_EAASSERT
    #define UTF_USE_EATRACE 0
#else
    #define UTF_USE_EATRACE 1
#endif

#if !defined(EA_ASSERT_HEADER)
    #if defined(UTF_USE_EAASSERT) && UTF_USE_EAASSERT
        #define EA_ASSERT_HEADER <EAAssert/eaassert.h>
    #else
        #define EA_ASSERT_HEADER <EATrace/EATrace.h>
    #endif
#endif
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// UTFXML_DLL
//
// Defined as 0 or 1. The default is dependent on the definition of EA_DLL.
// If EA_DLL is defined, then UTFXML_DLL is 1, else UTFXML_DLL is 0.
// EA_DLL is a define that controls DLL builds within the EAConfig build system. 
// UTFXML_DLL controls whether UTFXml is built and used as a DLL. 
// Normally you wouldn't do such a thing, but there are use cases for such
// a thing, particularly in the case of embedding C++ into C# applications.
//
#ifndef UTFXML_DLL
    #if defined(EA_DLL)
        #define UTFXML_DLL 1
    #else
        #define UTFXML_DLL 0
    #endif
#endif



///////////////////////////////////////////////////////////////////////////////
// UTFXML_API
//
// This is used to label functions as DLL exports under Microsoft platforms.
// If EA_DLL is defined, then the user is building UTFXml as a DLL and UTFXml's
// non-templated functions will be exported. UTFXML template functions are not
// labelled as UTFXML_API (and are thus not exported in a DLL build). This is 
// because it's not possible (or at least unsafe) to implement inline templated 
// functions in a DLL.
//
// Example usage of UTFXML_API:
//    UTFXML_API int someVariable = 10;  // Export someVariable in a DLL build.
//
//    struct UTFXML_API SomeClass{       // Export SomeClass and its member functions in a DLL build.
//    };
//
//    UTFXML_API void SomeFunction();    // Export SomeFunction in a DLL build.
//
//
#ifndef UTFXML_API // If the build file hasn't already defined this to be dllexport...
    #if UTFXML_DLL && defined(_MSC_VER)
        #define UTFXML_API           __declspec(dllimport)
        #define UTFXML_TEMPLATE_API  // Not sure if there is anything we can do here.
    #else
        #define UTFXML_API
        #define UTFXML_TEMPLATE_API
    #endif
#endif


#endif // Header include guard

















