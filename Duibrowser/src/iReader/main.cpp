//
// main.cpp
// ~~~~~~~~
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
#include <EAWebKit/EAWebKitView.h>

#include "PostilMaker.h"

#include "win_impl_base.hpp"
#include "frame.hpp"

//#if !defined(UNDER_CE)
//
//#include <crtdbg.h>
//
//void EnableMemLeakCheck()
//{
//   _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
//}
//
//#endif

IReaderDataPtr g_iReaderData(IREADER_DATA);
UINT	g_uInterProcMsg = 0;

#if !defined(UNDER_CE) && defined(_DEBUG)
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#if defined(WIN32) && !defined(UNDER_CE)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
#else
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int nCmdShow)
#endif
{
//#if defined(WIN32)
//	EnableMemLeakCheck();
//#endif

	CPaintManagerUI::SetInstance(hInstance);

#if defined(WIN32) && !defined(UNDER_CE)
	HRESULT Hr = ::CoInitialize(NULL);
#else
	HRESULT Hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
#endif
	if( FAILED(Hr) ) return 0;

	g_uInterProcMsg = RegisterWindowMessage(IREADER_INTERPROC_MSG);

	MainFrame* pFrame = new MainFrame();
	if( pFrame == NULL ) return 0;
#if defined(WIN32) && !defined(UNDER_CE)
	pFrame->Create(NULL, _T("iReader"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 800);
#else
	pFrame->Create(NULL, _T("iReader"), UI_WNDSTYLE_FRAME, WS_EX_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
#endif
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	try
	{
		CPaintManagerUI::MessageLoop();
	}
	catch(...)
	{
		pFrame->m_layoutEvent.set();
		pFrame->m_syncData.set();
		if (g_iReaderData)
			PostMessage(g_iReaderData->m_hNofityWnd, g_uInterProcMsg, IREADER_EXCEPTION_RAISED, (LPARAM)0);
	}

	CPaintManagerUI::ShutDown();

	::CoUninitialize();

//#if defined(WIN32)
//	_CrtDumpMemoryLeaks();
//#endif
	return 0;
}