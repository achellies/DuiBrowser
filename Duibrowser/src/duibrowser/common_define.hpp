//
// common_define.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2011 achellies (achellies at 163 dot com)
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//
#ifndef COMMON_DEFINE_HPP
#define COMMON_DEFINE_HPP

// Automatically include the DirectUI namespace
// define NO_USING_DUILIB_NAMESPACE to skip this step
namespace DuiLib {
// tString is a TCHAR std::string
typedef std::basic_string<TCHAR> tString;
}
#ifndef NO_USING_DUILIB_NAMESPACE
	using namespace DuiLib;
	using namespace std;
#endif

#define USE_ZIP_SKIN 1

#define	WM_MENU		WM_USER + 101

#endif // COMMON_DEFINE_HPP

