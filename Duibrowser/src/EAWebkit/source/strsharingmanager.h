/*++

Copyright (c) 2010 Hanvon Ltd. All Right Reserved.

All rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

File Name:

infoSharingManager.h

Abstract:
a wrapper for the string sharing manager

Author:
woods

--*/
#ifndef _STRING_SHARING_MANAGER_H
#define _STRING_SHARING_MANAGER_H

#define SHARING_STRING_BUFFER		_T("FormatRender.SharingStringBuffer")
#define SHARING_STRING_BUFFER_SIZE	512

class CStrSharingManager
{
public:
	CStrSharingManager(int length = 0)
		:m_hStrMappingHdr(NULL)
		,m_pStrMappingFile(NULL)
		,m_nLength(length)
	{
		if (length > 0)
		{
			Initialize(length);
		}
	}

	~CStrSharingManager()
	{
		Uninitialize();
	}

	operator bool()
	{
		return (m_pStrMappingFile != NULL)?true:false;
	}

	bool Initialize(int length)
	{
		if (length <= 0)
		{
			return false;
		}

		if (m_nLength < 0)
		{
			return false;
		}

		if (length != m_nLength)
		{
			if (m_pStrMappingFile != NULL)
			{
				UnmapViewOfFile(m_pStrMappingFile);
				m_pStrMappingFile = NULL;
			}

			if (m_hStrMappingHdr != NULL)
			{
				CloseHandle(m_hStrMappingHdr);
				m_hStrMappingHdr = NULL;
			}
		}

		m_nLength = length;
		if (m_nLength > 0 && (m_hStrMappingHdr == NULL) && (m_pStrMappingFile == NULL))
		{
			m_hStrMappingHdr = CreateFileMapping(
				INVALID_HANDLE_VALUE, 
				NULL, 
				PAGE_READWRITE, 
				0, 
				m_nLength * sizeof(TCHAR) + 4,
				SHARING_STRING_BUFFER);

			if ((m_hStrMappingHdr != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS))
			{
				m_pStrMappingFile = (char*)MapViewOfFile(m_hStrMappingHdr,FILE_MAP_ALL_ACCESS,0,0,0);
			}
			else if (m_hStrMappingHdr != NULL)
			{
				m_pStrMappingFile = (char*)MapViewOfFile(m_hStrMappingHdr,FILE_MAP_ALL_ACCESS,0,0,0);
				if (m_pStrMappingFile != NULL)
				{
					ZeroMemory(m_pStrMappingFile, m_nLength * sizeof(TCHAR) + 4);
				}
			}
		}

		return true;
	}

	void Uninitialize()
	{
		if (m_pStrMappingFile != NULL)
		{
			UnmapViewOfFile(m_pStrMappingFile);
			m_pStrMappingFile = NULL;
		}

		if (m_hStrMappingHdr != NULL)
		{
			CloseHandle(m_hStrMappingHdr);
			m_hStrMappingHdr = NULL;
		}
		m_nLength = 0;
	}

	void FillBuffer(LPCTSTR szSrc, int nLen)
	{
		if (nLen <= m_nLength)
		{
			CopyMemory(m_pStrMappingFile, szSrc, nLen * sizeof(TCHAR));
			*(int*)((TCHAR*)m_pStrMappingFile + m_nLength * sizeof(TCHAR)) = nLen;
		}
	}

	void ExtractString(LPTSTR szDest, int& nLen)
	{
		nLen = *(int*)((TCHAR*)m_pStrMappingFile + m_nLength * sizeof(TCHAR));
		CopyMemory(szDest, m_pStrMappingFile, nLen * sizeof(TCHAR));
		szDest[nLen + 1] = 0;
	}

private:
	HANDLE									m_hStrMappingHdr;
	void*									m_pStrMappingFile;
	int										m_nLength;
};

#endif // _STRING_SHARING_MANAGER_H