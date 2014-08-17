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
// EAWebKitConfig.h
// By Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#ifndef EAWEBKIT_EAWEBKITCONFIG_H
#define EAWEBKIT_EAWEBKITCONFIG_H

#include <EABase/eabase.h>
///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_VERSION
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
//     printf("EAWEBKIT version: %s", EAWEBKIT_VERSION);
//     printf("EAWEBKIT version: %d.%d.%d", EAWEBKIT_VERSION_N / 10000 % 100, EAWEBKIT_VERSION_N / 100 % 100, EAWEBKIT_VERSION_N % 100);
//
///////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKIT_VERSION
    #define EAWEBKIT_VERSION   "1.21.00"
    #define EAWEBKIT_VERSION_N 12100
#endif

/////////////////////////////////////////////////////////////////////////////
// EAWebKit Build Version 
// This is similar to the package version above but gives some more detail. 
//
/////////////////////////////////////////////////////////////////////////////
#if defined (EA_PLATFORM_PS3)
    const char gEAWebKitBuildVersion[]="EAWebKit: 1.21.00 PS3_SDK: 330.001 VS: 2008 ID: 0001"; 
#elif defined (EA_PLATFORM_XENON)
    const char gEAWebKitBuildVersion[]="EAWebKit: 1.21.00 XENON_SDK: 2.0.11164.3-proxy VS: 2008 ID: 0001"; 
#else // EA_PLATFORM_WINDOWS
    #if (_MSC_VER < 1500) // this is Visual C++ 2005 or lower since 2008 is 1500
         const char gEAWebKitBuildVersion[]="EAWebKit: 1.21.00 VS: 2005_SP1 ID: 0001"; 
    #else
         const char gEAWebKitBuildVersion[]="EAWebKit: 1.21.00 VS: 2008_SP1 ID: 0001"; 
    #endif
#endif
///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_DEBUG
//
// Defined as an integer >= 0. Default is 1 for debug builds and 0 for 
// release builds. This define is also a master switch for the default value 
// of some other settings.
//
// Example usage:
//    #if EAWEBKIT_DEBUG
//       ...
//    #endif
//
///////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKIT_DEBUG
    #if defined(EA_DEBUG) || defined(_DEBUG)
        #define EAWEBKIT_DEBUG 0
    #else
        #define EAWEBKIT_DEBUG 0
    #endif
#endif

///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_MEMORY_DEBUG
//
// Defined as an integer >= 0. Default is 1 for debug builds and 0 for 
// release builds. 
//
// Example usage:
//    #if EAWEBKIT_MEMORY_DEBUG
//       ...
//    #endif
//
///////////////////////////////////////////////////////////////////////////////

#ifndef EAWEBKIT_MEMORY_DEBUG
    #if defined(EA_DEBUG) || defined(_DEBUG)
        #define EAWEBKIT_MEMORY_DEBUG 1
    #else
        #define EAWEBKIT_MEMORY_DEBUG 0
    #endif
#endif




///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_ASSERT_ENABLED
//
// Defined as 0 or 1. The default is to equal EAWEBKIT_DEBUG.
// Enables the EAW_ASSERT macros.
//
#ifndef EAWEBKIT_ASSERT_ENABLED
    #define EAWEBKIT_ASSERT_ENABLED EAWEBKIT_DEBUG
#endif



///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_TRACE_ENABLED
//
// Defined as 0 or 1. The default is to equal EAWEBKIT_DEBUG.
// Enables the EAW_TRACE macros.
//
#ifndef EAWEBKIT_TRACE_ENABLED
    #define EAWEBKIT_TRACE_ENABLED EAWEBKIT_DEBUG
#endif



///////////////////////////////////////////////////////////////////////////////
// WTF_USE_UTFXML / WTF_USE_LIBXML2
//
// This sets UTFXml usage in case it isn't already defined by the build system.
// Normally in our nant build we have this defined in the EAWebKit.build file,
// and that setting will take precedence.
//
// Example usage in WebKit code:
//     #if USE(UTFXML)    // WebKit defines the 'USE' macro, and we take advantage of it.
//         blah
//     #endif
//
#ifndef WTF_USE_UTFXML
    #define WTF_USE_UTFXML 1
#endif

#ifndef WTF_USE_LIBXML2
    #if WTF_USE_UTFXML
        #define WTF_USE_LIBXML2 0
    #else
        #define WTF_USE_LIBXML2 1
    #endif
#endif


///////////////////////////////////////////////////////////////////////////////
// WTF_USE_DIRTYSDK / WTF_USE_UTFINTERNET / WTF_USE_CURL
//
// This sets the HTTP transport package that's used.
// Normally in our nant build we have this defined in the EAWebKit.build file,
// and that setting will take precedence.
// These packages aren't mutually exclusive, but if you have one of them 
// working for HTTP then you probably don't need another to do HTTP. 
// However, if one does HTTP (e.g. DirtySDK) but doesn't do FTP, then it 
// could be useful to enable multiple transport packages.
//
// Example usage in WebKit code:
//     #if USE(DIRTYSDK)    // WebKit defines the 'USE' macro, and we take advantage of it.
//         blah
//     #endif
//
#ifndef WTF_USE_CURL
    #define WTF_USE_CURL 0
#endif

#ifndef WTF_USE_DIRTYSDK
    #define WTF_USE_DIRTYSDK 1
#endif

#ifndef WTF_USE_UTFINTERNET
    #define WTF_USE_UTFINTERNET 0
#endif



///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_API
//
// This is used to label functions as DLL exports under Microsoft platforms.
// If EA_DLL is defined, then the user is building EAWebKit as a DLL and EAWebKit's
// non-templated functions will be exported. EAWebKit template functions are not
// labelled as EAWEBKIT_API (and are thus not exported in a DLL build). This is 
// because it's not possible (or at least unsafe) to implement inline templated 
// functions in a DLL.
//
// Example usage of EAWEBKIT_API:
//    EAWEBKIT_API int someVariable = 10;  // Export someVariable in a DLL build.
//
//    struct EAWEBKIT_API SomeClass{       // Export SomeClass and its member functions in a DLL build.
//    };
//
//    EAWEBKIT_API void SomeFunction();    // Export SomeFunction in a DLL build.
//
//

#ifndef EAWEBKIT_API // If the build file hasn't already defined this to be dllexport...
    #if defined(EA_DLL) && defined(_MSC_VER)
        #define EAWEBKIT_API           __declspec(dllimport)
        #define EAWEBKIT_TEMPLATE_API  // Not sure if there is anything we can do here.
    #else
		#define EAWEBKIT_API
        #define EAWEBKIT_TEMPLATE_API
    #endif
#endif

//Following defines help in separating the initialization code related to the 3 platforms
#if defined(EA_DLL) && defined(EA_PLATFORM_PS3)
	#define EAWEBKIT_PS3_PRX 1
#else
	#define EAWEBKIT_PS3_PRX 0
#endif

#if defined(EA_DLL) && defined(EA_PLATFORM_XENON)
	#define EAWEBKIT_XENON_DLL 1
#else
	#define EAWEBKIT_XENON_DLL 0
#endif

#if defined(EA_DLL) && defined(EA_PLATFORM_WINDOWS)
	#define EAWEBKIT_WINDOWS_DLL 1
#else
	#define EAWEBKIT_WINDOWS_DLL 0
#endif

#if defined(EA_DLL) && defined(EA_PLATFORM_WINCE)
	#undef EAWEBKIT_WINDOWS_DLL
	#define EAWEBKIT_WINDOWS_DLL 1
#else
	#define EAWEBKIT_WINDOWS_DLL 0
#endif

#ifndef EAWEBKIT_THROW_BUILD_ERROR
	#ifdef EAWEBKIT_STATIC_LINKAGE //If you are linking statically in a production build (the one you are not shipping, you need to ship a dll build), you can include anything you want.
		#define EAWEBKIT_THROW_BUILD_ERROR 0
	#else
		#ifndef EA_DLL
			#define EAWEBKIT_THROW_BUILD_ERROR 1
		#endif
	#endif
#endif
///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_DEFAULT_FILE_SYSTEM_ENABLED
//
// Defined as 0 or 1; 1 is default.
// Controls if a default platform-specific file system is available for use.
//
#ifndef EAWEBKIT_DEFAULT_FILE_SYSTEM_ENABLED
    #define EAWEBKIT_DEFAULT_FILE_SYSTEM_ENABLED 1
#endif


///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_DEFAULT_CURSOR_HANDLING_ENABLED
//
// Defined as 0 or 1; 1 is default.
// Controls if a default platform-specific cursor handling is available for use.
// However, some platforms don't have built-in cursor handling and so this 
// option doesn't have an effect on these platforms (e.g. gaming consoles).
//
#ifndef EAWEBKIT_DEFAULT_CURSOR_HANDLING_ENABLED
    #define EAWEBKIT_DEFAULT_CURSOR_HANDLING_ENABLED 1
#endif


///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_DEFAULT_FILE_CHOOSER_HANDLING_ENABLED
//
// Defined as 0 or 1; 0 is default.
// Controls if a default platform-specific file choosing is available for use.
// However, some platforms don't have built-in file choosing and so this 
// option doesn't have an effect on these platforms (e.g. gaming consoles).
//
#ifndef EAWEBKIT_DEFAULT_FILE_CHOOSER_HANDLING_ENABLED
    #define EAWEBKIT_DEFAULT_FILE_CHOOSER_HANDLING_ENABLED 0
#endif


///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_DEFAULT_LOG_HANDLING_ENABLED
//
// Defined as 0 or 1; 1 is default for debug.
// Controls if a default platform-specific log writing is available for use.
// So if the user doesn't install a log handler then something like 
// printf will be used instead.
//
#ifndef EAWEBKIT_DEFAULT_LOG_HANDLING_ENABLED
    #ifdef EA_DEBUG
        #define EAWEBKIT_DEFAULT_LOG_HANDLING_ENABLED 1
    #else
        #define EAWEBKIT_DEFAULT_LOG_HANDLING_ENABLED 0
    #endif
#endif


///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_DEFAULT_STRING_HANDLING_ENABLED
//
// Defined as 0 or 1; 0 is default.
// Controls if a default platform-specific string localization is available for use.
// You normally want to have this disabled in a localized shipping application,
// as you will be providing your own localized strings.
//
#ifndef EAWEBKIT_DEFAULT_STRING_HANDLING_ENABLED
    #define EAWEBKIT_DEFAULT_STRING_HANDLING_ENABLED 0
#endif


///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_EAIO_ENABLED
//
// Defined as 0 or 1; 0 is default.
// Controls if EAIO system file IO is used intrinsically in the WebKit back-end.
// Normally a user-callback file system is used.
//
#ifndef EAWEBKIT_EAIO_ENABLED
    #define EAWEBKIT_EAIO_ENABLED 0
#endif


///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_DEFAULT_NAME_PREFIX
//
// Defined as a string literal. Defaults to "EAWebKit".
// Used to prefix memory allocation name tags.
//
#ifndef EAWEBKIT_DEFAULT_NAME_PREFIX
    #define EAWEBKIT_DEFAULT_NAME_PREFIX "EAWebKit"
#endif


///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_USE_RWFILESYS
//
#ifndef EAWEBKIT_USE_RWFILESYS
    #define EAWEBKIT_USE_RWFILESYS 1
#endif


///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_USE_EAIO
//
#ifndef EAWEBKIT_USE_EAIO
    #define EAWEBKIT_USE_EAIO 0
#endif


///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_USE_DIRTYSDK
//
#ifndef EAWEBKIT_USE_CURL
    #define EAWEBKIT_USE_CURL 0
#endif

///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_USE_DIRTYSDK
//
#ifndef EAWEBKIT_USE_DIRTYSDK
    #define EAWEBKIT_USE_DIRTYSDK 1
#endif


///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_USE_UTFSOCKETS
//
#ifndef EAWEBKIT_USE_UTFSOCKETS
    #define EAWEBKIT_USE_UTFSOCKETS 0
#endif


///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_USE_RLE_COMPRESSION
//
// If defined as 1 then the ability to do in-memory RLE graphics compression
// is compiled into the library. There is a separate EAWebKit Parameters 
// setting to enable it at runtime.
//
#ifndef EAWEBKIT_USE_RLE_COMPRESSION
    #define EAWEBKIT_USE_RLE_COMPRESSION 1
#endif


///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_USE_YCOCGDXT5_COMPRESSION
//
// If defined as 1 then the ability to do in-memory DXT graphics compression
// is compiled into the library. There is a separate EAWebKit Parameters 
// setting to enable it at runtime.
//
#ifndef EAWEBKIT_USE_YCOCGDXT5_COMPRESSION
    #define EAWEBKIT_USE_YCOCGDXT5_COMPRESSION 1
#endif

///////////////////////////////////////////////////////////////////////////////
// EAWEBKIT_ENABLE_JUMP_NAVIGATION_DEBUGGING
//
// If defined as 1, then the debug functionality in the Jump Navigation system is exposed. These calls/variables have run time penalty, so wrapping it behind a pre-processor.
//
#ifndef EAWEBKIT_ENABLE_JUMP_NAVIGATION_DEBUGGING
	#define EAWEBKIT_ENABLE_JUMP_NAVIGATION_DEBUGGING 0
#endif



#endif // Header include guard
