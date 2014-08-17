/*
* Copyright (C) 2010, 2011 achellies <achellies@163.com>
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "stdafx.h"
#include "PostilMaker.h"
#include "CppSQLite3U.h"
#include "iReader.h"

#ifdef MIPS
#include "ebook_hard.h"
#endif

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <iostream>
#include <sstream>
#include <iosfwd>

static DWORD dwVersion = MAKELONG(1, 0);
static LPCTSTR lpszPostilInfo = _T("PostilInfo");
static LPCTSTR lpszPostilInfoCmd = _T("CREATE TABLE IF NOT EXISTS %s (totalPages INTEGER, currentFontSize INTEGER, version INTEGER)");

static LPCTSTR lpszPostilPage = _T("PostilPage");
static LPCTSTR lpszPostilPageCmd = _T("CREATE TABLE IF NOT EXISTS %s (currentPage INTEGER, postilData BLOB, currentFontSize INTEGER)");

static LPCTSTR lpszBeginTransaction = _T("begin IMMEDIATE transaction;");
static LPCTSTR lpszCommitTransaction = _T("commit transaction;");
static LPCTSTR lpszRollbackTranscation = _T("rollback transaction");

extern CMemroySharePtr<iReaderData,1>	g_iReaderData;

void DatabaseCheckUpdate(const wchar_t* const pTableName, const wchar_t* const pCreateSQL, CppSQLite3DB* pDBHandler);

CPostilMaker::CPostilMaker()
	: m_dwCurPage(0)
	, m_hHostWnd(NULL)
	, m_dwPageCount(0)
	, m_dwFontSize(0)
	, m_bPostilExisted(FALSE)
	, m_bPostilChanged(FALSE)
{
	ZeroMemory(m_szFile, sizeof(m_szFile));
}

CPostilMaker::~CPostilMaker()
{
#ifdef MIPS
	_Ebook_Enter_Handwrite(FALSE);
#endif
}

void CPostilMaker::SetInfo(HWND hHost, LPCTSTR lpszFile)
{
	m_hHostWnd = hHost;
	if (lpszFile == 0)
		return;

	_stprintf(m_szFile, lpszFile);

	// check the file existed
    HANDLE hFile = CreateFile(m_szFile, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;

	_stprintf(m_szFile, _T("%s.psl"), m_szFile);
	
}

BOOL CPostilMaker::LoadFile()
{
	// open the file
	CppSQLite3DB db;
	CppSQLite3Query query;
	int loop_count = 0;
	TCHAR szBuf[MAX_PATH] = {0};

LOAD_FILE_START:
	try
	{
		// check the file existed
		db.open(m_szFile);
		//db.execDML(lpszBeginTransaction);		
		_stprintf(szBuf, lpszPostilInfo);
		if (!db.tableExists(szBuf)) {
			_stprintf(szBuf, lpszPostilInfoCmd, lpszPostilInfo);
			db.execDML(szBuf);
			_stprintf(szBuf, _T("INSERT INTO %s VALUES(0, 0, %d);"), lpszPostilInfo, dwVersion);
			db.execDML(szBuf);
			_stprintf(szBuf, lpszPostilPage);
			if (!db.tableExists(szBuf)) {
				_stprintf(szBuf, lpszPostilPageCmd, lpszPostilPage);
				db.execDML(szBuf);
			}
			m_bPostilExisted = TRUE;
			m_bPostilChanged = FALSE;
		}
		else {
			_stprintf(szBuf, _T("SELECT * FROM %s WHERE currentFontSize = %d;"), lpszPostilInfo, m_dwFontSize);
			query = db.execQuery(szBuf);
			DWORD dwFontSize = m_dwFontSize;
			if (!query.eof()) {				
				dwFontSize = query.getIntField(_T("currentFontSize"), 0);
				if (dwFontSize == m_dwFontSize) {
					m_dwPageCount = query.getIntField(_T("totalPages"), 0);

					m_bPostilExisted = TRUE;
					m_bPostilChanged = FALSE;
				}
				else {
					m_bPostilExisted = FALSE;
					m_bPostilChanged = FALSE;
				}
			}
			query.finalize();

			_stprintf(szBuf, lpszPostilPage);
			if (!db.tableExists(szBuf)) {
				_stprintf(szBuf, lpszPostilPageCmd, lpszPostilPage);
				db.execDML(szBuf);
			}
			else
				DatabaseCheckUpdate(lpszPostilPage, lpszPostilPageCmd, &db);
		}

		//db.execDML(lpszCommitTransaction);
		db.close();
	}
	catch (CppSQLite3Exception& e)
	{
		UNREFERENCED_PARAMETER(e);
		query.finalize();
		db.close();	
		
		DeleteFile(m_szFile);
		//wprintf(_T("Exception occur 1 (m_szFile = %s), error msg = %s, error code = %d, dwErr = %d.\n"), m_szFile, e.errorMessage(),e.errorCode(), GetLastError());
		
		m_bPostilExisted = FALSE;
		m_bPostilChanged = FALSE;

		if (loop_count++ <= 1)
		{
			goto LOAD_FILE_START;
		}
	}

	return m_bPostilExisted;
}

void CPostilMaker::Save()
{
	SavePageData();
}

void CPostilMaker::ClearPageData(DWORD dwPage)
{
	if (m_bPostilExisted)
	{
		m_bPostilChanged = FALSE;

		TCHAR szBuf[MAX_PATH] = {0};
		CppSQLite3DB db;
		// open the file		
		try
		{
			db.open(m_szFile);

			_stprintf(szBuf, lpszPostilPage);
			if (db.tableExists(szBuf)) {
				_stprintf(szBuf, _T("DELETE FROM %s WHERE (currentPage = %d) AND (currentFontSize = %d);"), lpszPostilPage, dwPage, m_dwFontSize);
				db.execDML(szBuf);
			}
			db.close();
			
			m_currentTraceArray.data.clear();
			m_currentTrace.points.clear();
			m_clearTrace.points.clear();
			InvalidateRect(m_hHostWnd, NULL, TRUE);
		}
		catch (CppSQLite3Exception& e)
		{
			UNREFERENCED_PARAMETER(e);
			db.close();
			DeleteFile(m_szFile);
			//wprintf(_T("Exception occur 2, error msg = %s, error code = %d.\n"),e.errorMessage(),e.errorCode());
		}
	}
}

void CPostilMaker::SavePageData()
{
	if (m_bPostilExisted && m_bPostilChanged)
	{
		if (!m_currentTrace.points.empty())
		{
			m_currentTraceArray.data.push_back(m_currentTrace);
		}

		m_currentTrace.points.clear();

		int loop_count = 0;

		TCHAR szBuf[MAX_PATH] = {0};
		CppSQLite3DB db;
		CppSQLite3Query query;
SAVE_DATA_START:
		// open the file		
		try
		{
			db.open(m_szFile);

			_stprintf(szBuf, lpszBeginTransaction);
			db.execDML(szBuf);

			DWORD dwFontSize = m_dwFontSize;
			_stprintf(szBuf, lpszPostilInfo);
			if (!db.tableExists(szBuf)) {
				_stprintf(szBuf, lpszPostilInfoCmd, lpszPostilInfo);
				db.execDML(szBuf);
				_stprintf(szBuf, _T("INSERT INTO %s VALUES(%d, %d, %d);"), lpszPostilInfo, m_dwFontSize, m_dwFontSize, dwVersion);
				db.execDML(szBuf);
				_stprintf(szBuf, lpszPostilPage);
				if (!db.tableExists(szBuf)) {
					_stprintf(szBuf, lpszPostilPageCmd, lpszPostilPage);
					db.execDML(szBuf);
				}
			}
			//else {
			//	_stprintf(szBuf, _T("SELECT * FROM %s WHERE currentFontSize = %d;"), lpszPostilInfo, m_dwFontSize);
			//	query = db.execQuery(szBuf);
			//	if (!query.eof())
			//		dwFontSize = query.getIntField(_T("currentFontSize"), 0);

			//	query.finalize();
			//}

			//if (dwFontSize != m_dwFontSize) {
			//	DeleteFile(m_szFile);
			//	LoadFile();
			//}

			_stprintf(szBuf, _T("DELETE FROM %s WHERE (currentPage = %d) AND (currentFontSize = %d);"), lpszPostilPage, m_dwCurPage, m_dwFontSize);
			db.execDML(szBuf);

			if (!m_currentTraceArray.data.empty())
			{
				DWORD dwLength = 0;
				for (vector<Trace>::iterator iter = m_currentTraceArray.data.begin(); iter != m_currentTraceArray.data.end(); ++iter) {
					iter->dwCount = static_cast<DWORD>(iter->points.size());
					dwLength += 2 * iter->dwCount + 6;
				}

				DWORD* pBlob = new DWORD[dwLength];
				if (pBlob != NULL) {
					DWORD nIndex = 0;
					for (vector<Trace>::iterator iter = m_currentTraceArray.data.begin(); (nIndex < dwLength) && (iter != m_currentTraceArray.data.end()); ++iter) {
						pBlob[nIndex++] = 0xFF;
						pBlob[nIndex++] = iter->dwWidth;
						pBlob[nIndex++] = iter->dwColor;
						pBlob[nIndex++] = iter->dwPress;
						pBlob[nIndex++] = iter->dwCount;
						for (vector<POINT>::iterator iter2 = iter->points.begin(); (nIndex < dwLength) && (iter2 != iter->points.end()); ++iter2) {
							pBlob[nIndex] = iter2->x;
							if (pBlob[nIndex] != iter2->x)
							{
								int a = 0;
								++a;
								int b = a;
							}

							++nIndex;
							pBlob[nIndex] = iter2->y;
							if (pBlob[nIndex] != iter2->y)
							{
								int a = 0;
								++a;
								int b = a;
							}
							++nIndex;
						}
						pBlob[nIndex++] = 0xFF;
					}

					_stprintf(szBuf, _T("INSERT INTO %s VALUES(%d, ?, %d);"), lpszPostilPage, m_dwCurPage, m_dwFontSize);
					CppSQLite3Statement statement = db.compileStatement(szBuf);
					statement.bind(1, reinterpret_cast<unsigned char*>(pBlob), static_cast<DWORD>(sizeof(DWORD)/ sizeof(unsigned char)) * dwLength);
					statement.execDML();
					statement.finalize();
					_stprintf(szBuf, _T("UPDATE %s SET totalPages = %d WHERE currentFontSize = %d;"), lpszPostilInfo, m_dwPageCount, m_dwFontSize);
					db.execDML(szBuf);

					delete[] pBlob;
					pBlob = NULL;
				}
			}
			_stprintf(szBuf, lpszCommitTransaction);
			db.execDML(szBuf);
			db.close();
		}
		catch (CppSQLite3Exception& e)
		{
			UNREFERENCED_PARAMETER(e);
			query.finalize();
			_stprintf(szBuf, lpszRollbackTranscation);
			db.execDML(szBuf);			
			db.close();
			DeleteFile(m_szFile);
			//wprintf(_T("Exception occur 3, error msg = %s, error code = %d.\n"),e.errorMessage(),e.errorCode());

			if (loop_count++ <= 1)
			{
				goto SAVE_DATA_START;
			}
		}

		m_currentTraceArray.data.clear();
	}
}

void CPostilMaker::SetZoom(DWORD dwFontSize, DWORD dwPageCounts)
{
	if (m_bPostilExisted && m_bPostilChanged)
	{
		SavePageData();
	}

	m_dwPageCount = dwPageCounts;
	m_dwFontSize = dwFontSize;
	if (!m_bPostilExisted) {
		HANDLE hFile = CreateFile(m_szFile, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			m_bPostilExisted = FALSE;
			m_bPostilChanged = FALSE;
			return;
		}
		m_bPostilExisted = TRUE;
		LoadFile();
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
	m_bPostilExisted = TRUE;
	m_bPostilChanged = FALSE;
}

BOOL CPostilMaker::GetPageData(DWORD dwPage)
{
	if (m_bPostilExisted && m_bPostilChanged)
	{
		SavePageData();
	}
	BOOL bHasPostil = FALSE;

	m_bPostilChanged = FALSE;

	m_dwCurPage = dwPage;

	m_currentTraceArray.data.clear();
	m_currentTrace.points.clear();
	if (m_bPostilExisted)
	{
		// open the file
		CppSQLite3DB db;
		CppSQLite3Query query;
		TCHAR szBuf[MAX_PATH] = {0};
		try
		{
			db.open(m_szFile);
			_stprintf(szBuf, lpszPostilInfo);
			if (db.tableExists(szBuf)) {
				_stprintf(szBuf, _T("SELECT * FROM %s WHERE (currentPage = %d) AND (currentFontSize = %d);"), lpszPostilPage, dwPage, m_dwFontSize);
				query = db.execQuery(szBuf);
				if (!query.eof()) {
					bHasPostil = TRUE;
					int nLength = 0;
					const unsigned char* pBlob = query.getBlobField(1, nLength);
					if ((nLength > 0) && (pBlob != NULL))
					{
						DWORD* pData = new DWORD[nLength + 1];
						if (pData != NULL) {
							ZeroMemory(pData, sizeof(DWORD) * (nLength + 1));
							nLength = static_cast<DWORD>(nLength / (sizeof(DWORD)/ sizeof(unsigned char)));
							for (int i = 0; i < nLength; ++i)
								pData[i] = reinterpret_cast<DWORD*>(const_cast<unsigned char*>(pBlob))[i];					

							BOOL bNewTrace = FALSE;
							Trace penTrace;
							for (int i = 0; i < nLength;) {
								if ((pData[i] == 0xFF) && !bNewTrace) {
									bNewTrace = TRUE;
									penTrace.points.clear();
									++i;
								}
								else if ((pData[i] == 0xFF) && bNewTrace) {
									m_currentTraceArray.data.push_back(penTrace);
									penTrace.points.clear();
									bNewTrace = FALSE;
									++i;
								}
								else {
									if (i + sizeof(Trace) / sizeof(DWORD) < nLength) {
										penTrace.dwWidth = pData[i++];
										penTrace.dwColor = pData[i++];
										penTrace.dwPress = pData[i++];
										penTrace.dwCount = pData[i++];

										DWORD dwFinishIndex = i + penTrace.dwCount * 2;
										if (dwFinishIndex < nLength) {
											for (;i < dwFinishIndex;) {
												POINT point;
												point.x = pData[i++];
												point.y = pData[i++];
												penTrace.points.push_back(point);
											}
										}
										else
											break;
									}
									else
										break;
								}
							}

							delete[] pData;
							pData = 0;
						}
					}
				}

				query.finalize();
			}
			db.close();
		}
		catch (CppSQLite3Exception& e)
		{
			UNREFERENCED_PARAMETER(e);
			query.finalize();
			db.close();
			DeleteFile(m_szFile);
			//wprintf(_T("Exception occur 4 (m_szFile = %s), error msg = %s, error code = %d.\n"), m_szFile, e.errorMessage(),e.errorCode());
		}

		return bHasPostil;
	}

	return bHasPostil;
}

void CPostilMaker::AddPoint(POINT point, DWORD dwWidth, bool clear, COLORREF clr, DWORD dwPress)
{
	m_bPostilExisted = TRUE;
	m_bPostilChanged = TRUE;

	if (!clear && !m_currentTrace.points.empty()) {
		HDC doubleBufferDC = GetDC(m_hHostWnd);
		HPEN hPen = CreatePen(PS_SOLID, dwWidth, clr);
		HPEN hOldPen = static_cast<HPEN>(SelectObject(doubleBufferDC, hPen));
		HBRUSH hOldBrush = (HBRUSH)SelectObject(doubleBufferDC, GetStockObject(BLACK_BRUSH));

		RECT rcDot;
		rcDot.left = m_currentTrace.points.back().x - (dwWidth - 1) / 2;
		rcDot.right = rcDot.left + dwWidth - 1;
		rcDot.top = m_currentTrace.points.back().y - (dwWidth - 1) / 2;
		rcDot.bottom = rcDot.top + dwWidth - 1;
		Ellipse(doubleBufferDC, rcDot.left, rcDot.top, rcDot.right, rcDot.bottom);				

		MoveToEx(doubleBufferDC, m_currentTrace.points.back().x, m_currentTrace.points.back().y, NULL);
		LineTo(doubleBufferDC, point.x, point.y);

		SelectObject(doubleBufferDC, hOldPen);
		SelectObject(doubleBufferDC,hOldBrush);

		ReleaseDC(m_hHostWnd, doubleBufferDC);
	}

	if (clear)
		m_clearTrace.points.push_back(point);
	else {
		m_currentTrace.dwWidth = dwWidth;
		m_currentTrace.dwColor = clr;
		m_currentTrace.dwPress = dwPress;
		m_currentTrace.points.push_back(point);
	}
}

void CPostilMaker::TraceStart(POINT point, DWORD dwWidth, bool clear, COLORREF clr, DWORD dwPress)
{
#ifdef MIPS
	_Ebook_Enter_Handwrite(TRUE);
#endif
	if (!m_bPostilExisted) {
		HANDLE hFile = CreateFile(m_szFile, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
			LoadFile();
		else {
			CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
		}
	}

	m_bPostilExisted = TRUE;
	m_bPostilChanged = TRUE;

	m_currentTrace.dwWidth = dwWidth;
	m_currentTrace.dwColor = clr;
	m_currentTrace.dwPress = dwPress;

	if (!clear && !m_currentTrace.points.empty())
		m_currentTraceArray.data.push_back(m_currentTrace);

	m_clearTrace.points.clear();
	m_currentTrace.points.clear();
	if (clear)
		m_clearTrace.points.clear();
	AddPoint(point, dwWidth, clear, clr, dwPress);
}

void CPostilMaker::TraceEnd(POINT point, DWORD dwWidth, bool clear, COLORREF clr, DWORD dwPress)
{
	m_bPostilExisted = TRUE;
	m_bPostilChanged = TRUE;

	AddPoint(point, dwWidth, clear, clr, dwPress);

	if (!clear && !m_currentTrace.points.empty())
		m_currentTraceArray.data.push_back(m_currentTrace);

	if (clear && !m_clearTrace.points.empty()) {
		vector<POINT>::const_iterator citerPre = m_clearTrace.points.begin();
		vector<POINT>::const_iterator citer = m_clearTrace.points.begin() + 1;
		for (; citer != m_clearTrace.points.end(); ++citer) {
			POINT a1 = *citerPre;
			POINT b1 = *citer;
			RECT rcClear;
			rcClear.left = min(a1.x, b1.x) - 5;
			rcClear.top = min(a1.y, b1.y) - 5;
			rcClear.right = max(a1.x, b1.x) + 5;
			rcClear.bottom = max(a1.y, b1.y) + 5;

			for (vector<Trace>::iterator iter2 = m_currentTraceArray.data.begin(); iter2 != m_currentTraceArray.data.end(); ++iter2)
				for (vector<POINT>::const_iterator citer3 = iter2->points.begin(); citer3 != iter2->points.end(); ++citer3)
					if (PtInRect(&rcClear, *citer3)) {
						vector<Trace>::difference_type offset = iter2 - m_currentTraceArray.data.begin();
						if (offset < 1)
							offset = 1;
						m_currentTraceArray.data.erase(iter2);
						InvalidateRect(m_hHostWnd, NULL, TRUE);
						if (!m_currentTraceArray.data.empty())
							iter2 = m_currentTraceArray.data.begin() + offset - 1;
						else
						{
							g_iReaderData->m_bCurrentPageHasPostil = false;
							m_clearTrace.points.clear();
							return;
						}
						break;
					}
			citerPre = citer;
		}
	}

	m_clearTrace.points.clear();
	m_currentTrace.points.clear();

#ifdef MIPS
	//_Ebook_Enter_Handwrite(FALSE);
#endif
}

void CPostilMaker::ClearTrace()
{
	m_currentTraceArray.data.clear();
	m_currentTrace.points.clear();
	m_clearTrace.points.clear();

	DeleteFile(m_szFile);
	LoadFile();

	m_bPostilExisted = FALSE;
	m_bPostilChanged = FALSE;
}

BOOL CPostilMaker::Draw(HDC hDCDest)
{
	if (m_bPostilExisted && m_currentTraceArray.Count() > 0) {

		HPEN hPen = CreatePen(PS_SOLID, g_iReaderData->m_nPostilPenWidth, 0);
		HPEN hOldPen = static_cast<HPEN>(SelectObject(hDCDest, hPen));
		for (vector<Trace>::const_iterator citer = m_currentTraceArray.data.begin(); citer != m_currentTraceArray.data.end(); ++citer) {			
			LOGPEN lgPen;
			GetObject(hPen, sizeof(LOGPEN), &lgPen);
			if (lgPen.lopnWidth.x != citer->dwWidth)
			{
				SelectObject(hDCDest, hOldPen);
				DeleteObject(hPen);
				hPen = CreatePen(PS_SOLID, citer->dwWidth, citer->dwColor);
				hOldPen = static_cast<HPEN>(SelectObject(hDCDest, hPen));
			}

			MoveToEx(hDCDest, citer->points.front().x, citer->points.front().y, NULL);
			for (vector<POINT>::const_iterator citer2 = citer->points.begin(); citer2 != citer->points.end(); ++citer2) {
				LineTo(hDCDest, citer2->x, citer2->y);
				MoveToEx(hDCDest, citer2->x, citer2->y, NULL);
			}
		}
		SelectObject(hDCDest, hOldPen);
		DeleteObject(hPen);

		return TRUE;
	}

	return FALSE;
}


void DatabaseCheckUpdate(const wchar_t* const pTableName, const wchar_t* const pCreateSQL, CppSQLite3DB* pDBHandler)
{
	int nRet = 0;
	//EBOOK_ENG_RESULT_T result = EBOOK_ENG_RES_SUCCESS;

	CppSQLite3Query query;
	try
	{
		if ((pTableName != NULL) && (pCreateSQL != NULL) && (pDBHandler != NULL))
		{
			// TABLE_LABEL table exist
			TCHAR sql_command[MAX_PATH] = {0};
#ifdef UNDER_CE
			_stprintf(sql_command, _T("SELECT * FROM \"%s\";"), pTableName);
#else
			_stprintf_s(sql_command, MAX_PATH, _T("SELECT * FROM \"%s\";"), pTableName);
#endif
			query = pDBHandler->execQuery(sql_command);
			if (query.numFields())
			{
				//if (query.eof())
				//{
				//	query.finalize();
				//	// if the existed table is empty, we will create a new one to avoid the our table defined is different with the old one
				//	_stprintf_s(sql_command, MAX_PATH, _T("DROP TABLE IF EXISTS %s;"), pTableName);
				//	nRet = pDBHandler->execDML(sql_command);
				//	shared_array<wchar_t> sql_command2(new wchar_t[_tcslen(pCreateSQL) + _tcslen(pTableName) + 1]);
				//	if (sql_command2.get() != NULL)
				//	{
				//		_stprintf_s(sql_command2.get(), _tcslen(pCreateSQL) + _tcslen(pTableName) + 1, pCreateSQL, pTableName);
				//		//memcpy_s(sql_command2.get(), _tcslen(pCreateSQL) * sizeof(wchar_t), pCreateSQL, _tcslen(pCreateSQL) * sizeof(wchar_t));
				//		//sql_command2[_tcslen(pCreateSQL)] = '\0';

				//		nRet = pDBHandler->execDML(sql_command2.get());
				//	}
				//}
				//else
				{
					std::wstring strSQL = pCreateSQL;
					std::wstring::size_type nStart = strSQL.find_first_of(_T("("));
					std::wstring::size_type  nEnd = strSQL.find_last_of(_T(")"));
					// Check the table definition
					if ((nStart != std::wstring::npos) && (nEnd != std::wstring::npos) && (nEnd > nStart))
					{
						std::vector< std::pair< std::wstring, std::wstring > > newTable;
						std::vector<std::wstring> oldTable;
						strSQL = strSQL.substr(nStart + 1,nEnd - nStart - 1);
						//strSQL = boost::algorithm::trim_copy(strSQL);
						std::wstring strTemp;
						nStart = std::wstring::npos;
						nStart = strSQL.find_first_of(_T(","));
						while (nStart != std::wstring::npos)
						{
							strTemp = strSQL.substr(0, nStart);
							strSQL = strSQL.substr(nStart + 1);

							std::wstringstream istr(strTemp.c_str());
							std::wstring strtemp1;
							std::wstring strtemp2;
							if (!istr.eof())
							{
								istr >> strtemp1;
							}
							if (!istr.eof())
							{
								istr >> strtemp2;
							}
							newTable.push_back(std::make_pair(strtemp1,strtemp2));
							nStart = strSQL.find_first_of(_T(","));
						}

						if (!strSQL.empty())
						{
							std::wstringstream istr(strSQL.c_str());
							std::wstring strtemp1;
							std::wstring strtemp2;
							if (!istr.eof())
							{
								istr >> strtemp1;
							}
							if (!istr.eof())
							{
								istr >> strtemp2;
							}
							newTable.push_back(make_pair(strtemp1,strtemp2));
						}

						for (int i = 0; i < query.numFields(); ++i)
						{
							oldTable.push_back(query.fieldName(i));
						}

						for (std::vector< std::pair< std::wstring, std::wstring > >::const_iterator citer = newTable.begin();
							citer != newTable.end();++citer)
						{
							std::vector<std::wstring>::const_iterator citer2 = find(oldTable.begin(),oldTable.end(),citer->first);
							if (citer2 == oldTable.end())
							{
#ifdef UNDER_CE
								_stprintf(sql_command, _T("ALTER TABLE %s ADD %s %s"), pTableName, citer->first.c_str(), citer->second.c_str());
#else
								_stprintf_s(sql_command, MAX_PATH, _T("ALTER TABLE %s ADD %s %s"), pTableName, citer->first.c_str(), citer->second.c_str());
#endif
								pDBHandler->execDML(sql_command);
							}
						}
					}
				}
			}
			query.finalize();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		UNREFERENCED_PARAMETER(e);
		query.finalize();
		//result = EBOOK_ENG_RES_SUCCESS;
	}
	return;
}