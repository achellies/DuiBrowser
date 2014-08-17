//
//
// DirectUI - UI Library
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2006-2007 Bjarke Viksoe.
//
// This code may be used in compiled form in any way you desire. These
// source files may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
////
// Acknowledgements :
// Bjarke Viksoe (http://www.viksoe.dk/code/windowless1.htm)
//
//
//
// Beware of bugs.
//
//
//
////////////////////////////////////////////////////////
#ifndef PreConfig_DEFINED
#define PreConfig_DEFINED

//////////////////////////////////////////////////////////////////////

#if !defined(UI_BUILD_FOR_ANDROID_NDK) && !defined(UI_BUILD_FOR_IOS) && !defined(UI_BUILD_FOR_PALM) && !defined(UI_BUILD_FOR_WINCE) && !defined(UI_BUILD_FOR_WIN32) && !defined(UI_BUILD_FOR_SYMBIAN) && !defined(UI_BUILD_FOR_UNIX) && !defined(UI_BUILD_FOR_MAC) && !defined(UI_BUILD_FOR_SDL) && !defined(UI_BUILD_FOR_BREW)

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#if defined(PALMOS_SDK_VERSION)
#define UI_BUILD_FOR_PALM
#elif defined(UNDER_CE)
#define UI_BUILD_FOR_WINCE
#elif defined(WIN32)
#define UI_BUILD_FOR_WIN32
#elif defined(__SYMBIAN32__)
#define UI_BUILD_FOR_WIN32
#elif defined(linux)
#define UI_BUILD_FOR_UNIX
#elif TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#define UI_BUILD_FOR_IOS
#elif defined(ANDROID_NDK)
#define UI_BUILD_FOR_ANDROID_NDK
#elif defined(ANROID)
#define UI_BUILD_FOR_ANDROID
#else
#define UI_BUILD_FOR_MAC
#endif

#endif

//////////////////////////////////////////////////////////////////////

#if !defined(UI_DEBUG) && !defined(UI_RELEASE)
#ifdef NDEBUG
#define UI_RELEASE
#else
#define UI_DEBUG
#endif
#endif

#if defined(UI_BUILD_FOR_WIN32) || defined(UI_BUILD_FOR_WINCE)

// use the Raster GDI as the Render Engine
#if !defined(UI_BUILD_FOR_WINGDI)
#define UI_BUILD_FOR_WINGDI
#endif

// use the SKIA as Render Engine
#if !defined(UI_BUILD_FOR_WINGDI)
#if !defined(UI_BUILD_FOR_SKIA)
#define UI_BUILD_FOR_SKIA
#endif
#endif

//#define UI_BUILD_FOR_SKIA
//#undef UI_BUILD_FOR_WINGDI

#endif

// switch this marco for the UI Designer
//#define UI_BUILD_FOR_DESIGNER

//#define USING_XUNZIP

#define USING_SYSTEM_SUPPLIED_RICHEDIT

#endif

