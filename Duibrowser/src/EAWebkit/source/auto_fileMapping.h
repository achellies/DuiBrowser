/*++

Copyright (c) 2010 Hanvon Ltd. All Right Reserved.

All rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

File Name:

	auto_fileMapping.h

Abstract:
	a wrapper for the file mapping pointer

Author:
	achellies

--*/
#ifndef _AUTO_FILEMAPPING_H
#define _AUTO_FILEMAPPING_H

template <typename _T,int _SIZE>
class CMemroySharePtr
{
public:
	CMemroySharePtr(LPCTSTR pszName = NULL)
		:m_hMappingHdr(NULL)
		,m_pMappingFile(NULL)
	{
		if ((pszName != NULL) && (m_hMappingHdr == NULL) && (m_pMappingFile == NULL))
		{
			m_hMappingHdr = CreateFileMapping(
				INVALID_HANDLE_VALUE, 
				NULL, 
				PAGE_READWRITE, 
				0, 
				sizeof(_T)*_SIZE, 
				pszName);
			if ((m_hMappingHdr != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS))
			{
				m_pMappingFile = (_T*)MapViewOfFile(m_hMappingHdr,FILE_MAP_ALL_ACCESS,0,0,0);
			}
			else if (m_hMappingHdr != NULL)
			{
				m_pMappingFile = (_T*)MapViewOfFile(m_hMappingHdr,FILE_MAP_ALL_ACCESS,0,0,0);
				if (m_pMappingFile != NULL)
				{
					ZeroMemory(m_pMappingFile, sizeof(_T));
					_T temp;
					*m_pMappingFile = temp;
				}
			}
		}
	}

	~CMemroySharePtr()
	{
		if (m_pMappingFile != NULL)
		{
			UnmapViewOfFile(m_pMappingFile);
			m_pMappingFile = NULL;
		}

		if (m_hMappingHdr != NULL)
		{
			CloseHandle(m_hMappingHdr);
			m_hMappingHdr = NULL;
		}
	}

	void Initalize(LPCTSTR pszName)
	{
		if ((pszName != NULL) && (m_hMappingHdr == NULL) && (m_pMappingFile == NULL))
		{
			m_hMappingHdr = CreateFileMapping(
				INVALID_HANDLE_VALUE, 
				NULL, 
				PAGE_READWRITE, 
				0, 
				sizeof(_T)*_SIZE, 
				pszName);
			if ((m_hMappingHdr != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS))
			{
				m_pMappingFile = (_T*)MapViewOfFile(m_hMappingHdr,FILE_MAP_ALL_ACCESS,0,0,0);
			}
			else if (m_hMappingHdr != NULL)
			{
				m_pMappingFile = (_T*)MapViewOfFile(m_hMappingHdr,FILE_MAP_ALL_ACCESS,0,0,0);
				if (m_pMappingFile != NULL)
				{
					ZeroMemory(m_pMappingFile, sizeof(_T));
					_T temp;
					*m_pMappingFile = temp;
				}
			}
		}
	}

	void Uninitialize()
	{
		if (m_pMappingFile != NULL)
		{
			UnmapViewOfFile(m_pMappingFile);
			m_pMappingFile = NULL;
		}

		if (m_hMappingHdr != NULL)
		{
			CloseHandle(m_hMappingHdr);
			m_hMappingHdr = NULL;
		}
	}

	_T* const operator->() throw()
	{
		return m_pMappingFile;
	}

	operator bool()
	{
		return (m_pMappingFile != NULL)?true:false;
	}

private:
	HANDLE	m_hMappingHdr;
	_T*		m_pMappingFile;
};

#endif // _AUTO_FILEMAPPING_H