//
// window_util.cpp
// ~~~~~~~~~~~~~~
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

#include "stdafx.h"
#include "window_util.hpp"

#if defined(UNDER_CE)
#pragma comment(lib, "Coredll.lib")
#else
#pragma comment(lib, "Version.lib")
#endif
// ...........................................................................
//
// NAME:	GetVerString
//
// IMPLEMENTATION NOTES:
// ...........................................................................
bool GetVerString(unsigned int maxStrSize, TCHAR *szComments,
						TCHAR *szCompanyName, TCHAR *szFileDescription,
						TCHAR *szFileVersion, TCHAR *szLegalCopyright,
						TCHAR *szProductName, TCHAR *szProductVersion)
{
	struct TTransArray
	{
		WORD LanguageID, CharacterSet;
	};

	// Get pathname of this executable
	// If hInst is NULL, GetModuleFileName will retrieve the module name of 
	// the executable of this process.
	TCHAR szModuleFullPath[MAX_PATH];
	szModuleFullPath[0] = _T('\0');
	::GetModuleFileName(NULL, szModuleFullPath, sizeof(szModuleFullPath));
	TRACE(_T("GetVerString: szModuleFullPath: '%s'\n"), 
		szModuleFullPath);

	// Get size of the current file version structure
	DWORD dwVerHnd;
	DWORD verInfoSize = ::GetFileVersionInfoSize(szModuleFullPath, &dwVerHnd);
	if ((verInfoSize == 0) || (maxStrSize == 0))
	{
		TRACE(_T("#### GetVerString: verInfoSize: %ld, ")
			_T("maxStrSize: %d\n"), verInfoSize, maxStrSize);
		return false;
	}

	// Allocate space for file information block and retrieve it
	char *pMem;				// Pointer to mem cpy of version resource
	TCHAR *szCurInfo = NULL;// Pointer into pMem block for currently requested
	// information.
	unsigned int curInfoLen;	// Length of info in szCurInfo in characters
	if ((pMem = (char *)malloc((size_t)verInfoSize)) == NULL)
	{
		//TRACE(_T("#### CPdkApp::GetVerString: failed to alloc.\n"));
		return false;
	}

	if (::GetFileVersionInfo(szModuleFullPath, dwVerHnd, verInfoSize, pMem) == 0)
	{
		free(pMem);
		//TRACE(_T("#### CPdkApp::GetVerString: GetFileVersionInfo() returned zero.\n"));
		return false;
	}

	// Get the array of available languages for this version block
	::VerQueryValue(pMem, _T("\\VarFileInfo\\Translation"), 
		(void **)&szCurInfo, &curInfoLen);
	TTransArray *arrAvailLangs = (TTransArray *)szCurInfo;

	// Get the Comments information.
	TCHAR szQuery[256];
	if (szComments)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\Comments"),
			arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szComments, szCurInfo, min(maxStrSize, curInfoLen));
			szComments[maxStrSize - 1] = '\0';
		}
	}

	// Get the CompanyName information
	if (szCompanyName)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\CompanyName"),
			arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szCompanyName, szCurInfo, min(maxStrSize, curInfoLen));
			szCompanyName[maxStrSize - 1] = '\0';
		}
	}

	// Get the FileDescription information
	if (szFileDescription)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\FileDescription"),
			arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szFileDescription, szCurInfo, min(maxStrSize, curInfoLen));
			szFileDescription[maxStrSize - 1] = '\0';
		}
	}

	// Get the FileVersion information
	if (szFileVersion)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\FileVersion"),
			arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szFileVersion, szCurInfo, min(maxStrSize, curInfoLen));
			szFileVersion[maxStrSize - 1] = '\0';
		}
	}

	// Get the LegalCopyright information
	if (szLegalCopyright)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\LegalCopyright"),
			arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szLegalCopyright, szCurInfo, min(maxStrSize, curInfoLen));
			szLegalCopyright[maxStrSize - 1] = '\0';
		}
	}

	// Get the ProductName information
	if (szProductName)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\ProductName"),
			arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szProductName, szCurInfo, min(maxStrSize, curInfoLen));
			szProductName[maxStrSize - 1] = '\0';
		}
	}

	// Get the ProductVersion information
	if (szProductVersion)
	{
		wsprintf(szQuery, _T("\\StringFileInfo\\%04x%04x\\ProductVersion"),
			arrAvailLangs[0].LanguageID, arrAvailLangs[0].CharacterSet);
		if (::VerQueryValue(pMem, szQuery, (void **)&szCurInfo, &curInfoLen) != 0)
		{
			_tcsncpy(szProductVersion, szCurInfo, min(maxStrSize, curInfoLen));
			szProductVersion[maxStrSize - 1] = '\0';
		}
	}

	// Free the space for file information block
	free(pMem);

	return true;
}