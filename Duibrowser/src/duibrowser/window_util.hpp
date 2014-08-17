//
// window_util.hpp
// ~~~~~~~~~~~~~~~
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

#ifndef WINDOW_UTIL_HPP
#define WINDOW_UTIL_HPP

bool GetVerString(unsigned int maxStrSize, TCHAR *szComments,
						TCHAR *szCompanyName, TCHAR *szFileDescription,
						TCHAR *szFileVersion, TCHAR *szLegalCopyright,
						TCHAR *szProductName, TCHAR *szProductVersion);

#endif // WINDOW_UTIL_HPP