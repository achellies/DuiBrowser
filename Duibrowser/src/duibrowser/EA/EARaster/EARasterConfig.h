/*
Copyright (C) 2008-2010 Electronic Arts, Inc.  All rights reserved.

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
// EARasterConfig.h
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#ifndef EARASTERCONFIG_H
#define EARASTERCONFIG_H


///////////////////////////////////////////////////////////////////////////////
// EARASTER_VERSION
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
//     printf("EARASTER version: %s", EARASTER_VERSION);
//     printf("EARASTER version: %d.%d.%d", EARASTER_VERSION_N / 10000 % 100, EARASTER_VERSION_N / 100 % 100, EARASTER_VERSION_N % 100);
//
///////////////////////////////////////////////////////////////////////////////

#ifndef EARASTER_VERSION
    #define EARASTER_VERSION   "1.00.00"
    #define EARASTER_VERSION_N  10000
#endif



///////////////////////////////////////////////////////////////////////////////
// EARASTER_DEBUG
//
// Defined as an integer >= 0. Default is 1 for debug builds and 0 for 
// release builds. This define is also a master switch for the default value 
// of some other settings.
//
// Example usage:
//    #if EARASTER_DEBUG
//       ...
//    #endif
//
///////////////////////////////////////////////////////////////////////////////

#ifndef EARASTER_DEBUG
    #if defined(EA_DEBUG) || defined(_DEBUG)
        #define EARASTER_DEBUG 1
    #else
        #define EARASTER_DEBUG 0
    #endif
#endif



///////////////////////////////////////////////////////////////////////////////
// EARASTER_ASSERT_ENABLED
//
// Defined as 0 or 1. The default is to equal EARASTER_DEBUG.
// Enables the EAW_ASSERT macros.
//
#ifndef EARASTER_ASSERT_ENABLED
    #define EARASTER_ASSERT_ENABLED EARASTER_DEBUG
#endif



///////////////////////////////////////////////////////////////////////////////
// EARASTER_TRACE_ENABLED
//
// Defined as 0 or 1. The default is to equal EARASTER_DEBUG.
//
#ifndef EARASTER_TRACE_ENABLED
    #define EARASTER_TRACE_ENABLED EARASTER_DEBUG
#endif



///////////////////////////////////////////////////////////////////////////////
// EARASTER_DLL
//
// Defined as 0 or 1. The default is dependent on the definition of EA_DLL.
// If EA_DLL is defined, then EARASTER_DLL is 1, else EARASTER_DLL is 0.
// EA_DLL is a define that controls DLL builds within the EAConfig build system. 
// EARASTER_DLL controls whether EARASTER is built and used as a DLL. 
// Normally you wouldn't do such a thing, but there are use cases for such
// a thing, particularly in the case of embedding C++ into C# applications.
//
#ifndef EARASTER_DLL
    #if defined(EA_DLL)
        #define EARASTER_DLL 1
    #else
        #define EARASTER_DLL 0
    #endif
#endif



///////////////////////////////////////////////////////////////////////////////
// EARASTER_API
//
// This is used to label functions as DLL exports under Microsoft platforms.
// If EA_DLL is defined, then the user is building EARaster as a DLL and EARaster's
// non-templated functions will be exported. EARaster template functions are not
// labelled as EARASTER_API (and are thus not exported in a DLL build). This is 
// because it's not possible (or at least unsafe) to implement inline templated 
// functions in a DLL.
//
// Example usage of EARASTER_API:
//    EARASTER_API int someVariable = 10;  // Export someVariable in a DLL build.
//
//    struct EARASTER_API SomeClass{       // Export SomeClass and its member functions in a DLL build.
//    };
//
//    EARASTER_API void SomeFunction();    // Export SomeFunction in a DLL build.
//
//
#if defined(EA_DLL) && !defined(EARASTER_DLL)
    #define EARASTER_DLL 1
#endif

#ifndef EARASTER_API // If the build file hasn't already defined this to be dllexport...
    #if EARASTER_DLL && defined(_MSC_VER)
        #define EARASTER_API           __declspec(dllimport)
        #define EARASTER_TEMPLATE_API  // Not sure if there is anything we can do here.
    #else
        #define EARASTER_API
        #define EARASTER_TEMPLATE_API
    #endif
#endif


///////////////////////////////////////////////////////////////////////////////
// EARASTER_DEFAULT_NAME_PREFIX
//
// Defined as a string literal. Defaults to "EARaster".
// Used to prefix memory allocation name tags.
//
#ifndef EARASTER_DEFAULT_NAME_PREFIX
    #define EARASTER_DEFAULT_NAME_PREFIX "EARaster"
#endif


#endif // Header include guard
