//
// main_frame.cpp
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
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif
#include <EAWebKit/EAWebKit.h>
#include <EAWebkit/EAWebkitAllocator.h>
#include <EAWebKit/EAWebKitTextInterface.h>
//#include <PPMalloc/EAStackAllocator.h>
#include <stdlib.h>  // malloc / new

#include <algorithm>
#include <vector>

#include <EAWebkit/WebkitLayout.h>

#include "PostilMaker.h"
#ifndef __UIUTIL_H__
#include "string_convertor.hpp"
#endif
#include "win_impl_base.hpp"
#include "input_dialog.hpp"
#include "UIWebkit.hpp"
#include "frame.hpp"
#include "debug.hpp"

#if !defined(UNDER_CE) && defined(_DEBUG)
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

const int kFontCount = 6;
int kFontSize[kFontCount] = {22, 24, 28, 34, 40, 48};

namespace EA {
	namespace TextWrapper {
		class FontServerProxy;
	}
}

using namespace EA::TextWrapper;

const int kViewTickTimerId = 1001;
const int kViewTickTimerElapse = 75;

const int kStartupTimerId = 1002;
const int kStartupTimerElapse = 500;

const int kDefaultFontSize = 24;
const int kMiniFontSize = 24;
const int kPageTimeoutSeconds = 30;

const TCHAR* const kWebkitControlName = _T("webkit");
const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");
const TCHAR* const kPrevPageControlName = _T("prev_page");
const TCHAR* const kNextPageControlName = _T("next_page");
const TCHAR* const kPageCountControlName = _T("page_count");
const TCHAR* const kGetTextControlName = _T("get_text");
const TCHAR* const kGetSelectedTextControlName = _T("get_select_text");
const TCHAR* const kZoomInCtonrolName = _T("zoom_in");
const TCHAR* const kZoomOutCtonrolName = _T("zoom_out");

const char* const kHomeHTML = "<html>\
    <head>\
    <title> iReader </title>\
	<body>\
    <p>Please Drog *.html file to open it.</p>\
	</body>\
	</html>";
const wchar_t* const kHomeURL = L"file:///E:/Webkit/chapter01/chapter01.html";

enum MenuItem
{
	// system menu item
	kSysetemMenuItem = 0,

	kReadMode,
	kExitReadMode,

	kExit,
};

MainFrame::MainFrame()
: webkit_dll_(NULL)
, webkit_(NULL)
, view_(NULL)
, font_server_(NULL)
, glyph_cache_(NULL)
, surface_(NULL)
, raster_(NULL)
, webkit_layout_(NULL)
, font_style_(NULL)
, webkit_control_(NULL)
, font_size_index_(1)
, layout_completed_(false)
, using_webkit_layout_(false)
, m_layoutEvent(IREADER_LAYOUT_EVENT, TRUE, TRUE)
, m_syncData(IREADER_SYNC_DATA, TRUE, TRUE)
{
#if defined(UNDER_CE)
	sprintf(application_name_, "iReader");
	sprintf(user_agent_, GetUserAgent().c_str());
#else
	sprintf_s(application_name_, MAX_PATH, "iReader");
	sprintf_s(user_agent_, MAX_PATH, GetUserAgent().c_str());
#endif
	g_iReaderData->m_nFontSize = kFontSize[font_size_index_];
}

MainFrame::~MainFrame()
{
	PostQuitMessage(0);
}

std::string MainFrame::GetUserAgent()
{
	std::string user_agent;
	// FireFox 3.0.3 uses:
	//     Mozilla/5.0 (Windows; U; Windows NT 6.0; en-US; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3
	// IE7 uses:
	//     Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0; Win64; x64; .NET CLR 2.0.50727; SLCC1; Media Center PC 5.0)
	// Safari uses:
	//     Mozilla/5.0 (Windows; U; Windows NT 6.0; en-US) AppleWebKit/525.19 (KHTML, like Gecko) Version/3.1.2 Safari/525.21

	std::string osVersion;
	OSVERSIONINFO versionInfo = {0};
	versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
	GetVersionEx(&versionInfo);
#if defined(EA_PLATFORM_WINDOWS)
#define kPlatformName "Windows"
#else
#define kPlatformName "WinCE"
#endif

	char szBuf[MAX_PATH] = {0};
	if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
		if (versionInfo.dwMajorVersion == 4) {
			if (versionInfo.dwMinorVersion == 0)
				osVersion = "Windows 95";
			else if (versionInfo.dwMinorVersion == 10)
				osVersion = "Windows 98";
			else if (versionInfo.dwMinorVersion == 90)
				osVersion = "Windows 98; Win 9x 4.90";
		}
	}
	else if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
#if defined(UNDER_CE)
		sprintf(szBuf, "Windows NT %d.%d", versionInfo.dwMajorVersion, versionInfo.dwMinorVersion);
#else
		sprintf_s(szBuf, MAX_PATH, "Windows NT %d.%d", versionInfo.dwMajorVersion, versionInfo.dwMinorVersion);
#endif
		osVersion = szBuf;
	}
#if defined(UNDER_CE)
	else if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_CE)
	{
		osVersion = "Windows CE";
	}
#endif

	if (osVersion.empty())
	{
#if defined(UNDER_CE)
		sprintf(szBuf, "Windows CE %d.%d", versionInfo.dwMajorVersion, versionInfo.dwMinorVersion);
#else
		sprintf_s(szBuf, MAX_PATH, "Windows %d.%d", versionInfo.dwMajorVersion, versionInfo.dwMinorVersion);
#endif
	}

#if defined(UNDER_CE)
	sprintf(szBuf, "Mozilla/5.0 (%s; U; %s; zh-cn) AppleWebKit/525.1 (KHTML, like Gecko) EAWebKit/1.0.0 %s",   
		kPlatformName,
		osVersion.c_str(),
		application_name_);
#else
	sprintf_s(szBuf, MAX_PATH, "Mozilla/5.0 (%s; U; %s; zh-cn) AppleWebKit/525.1 (KHTML, like Gecko) EAWebKit/1.0.0 %s",   
		kPlatformName,
		osVersion.c_str(),
		application_name_);
#endif

	return user_agent = szBuf;
}

std::string MainFrame::UrlEncode(const std::string& szToEncode)
{
	std::string src = szToEncode;
	char hex[] = "0123456789ABCDEF";
	string dst;

	for (size_t i = 0; i < src.size(); ++i)
	{
		unsigned char cc = src[i];
		if (isascii(cc))
		{
			if (cc == ' ')
			{
				dst += "%20";
			}
			else
				dst += cc;
		}
		else
		{
			unsigned char c = static_cast<unsigned char>(src[i]);
			dst += '%';
			dst += hex[c / 16];
			dst += hex[c % 16];
		}
	}
	return dst;
}


std::string MainFrame::UrlDecode(const std::string& szToDecode)
{
	std::string result;
	int hex = 0;
	for (size_t i = 0; i < szToDecode.length(); ++i)
	{
		switch (szToDecode[i])
		{
		case '+':
			result += ' ';
			break;
		case '%':
			if (isxdigit(szToDecode[i + 1]) && isxdigit(szToDecode[i + 2]))
			{
				std::string hexStr = szToDecode.substr(i + 1, 2);
				hex = strtol(hexStr.c_str(), 0, 16);
				//字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@]
				//可以不经过编码直接用于URL
				if (!((hex >= 48 && hex <= 57) ||	//0-9
					(hex >=97 && hex <= 122) ||	//a-z
					(hex >=65 && hex <= 90) ||	//A-Z
					//一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]
					hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29
					|| hex == 0x2a || hex == 0x2b|| hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f
					|| hex == 0x3A || hex == 0x3B|| hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f
					))
				{
					result += char(hex);
					i += 2;
				}
				else result += '%';
			}else {
				result += '%';
			}
			break;
		default:
			result += szToDecode[i];
			break;
		}
	}
	return result;
}

LPCTSTR MainFrame::GetWindowClassName() const
{
	return lpszIReaderWindowClassName;
}

CControlUI* MainFrame::CreateControl(LPCTSTR pstrClass, CPaintManagerUI* pManager)
{
	if (_tcsicmp(pstrClass, _T("Webkit")) == 0)
#if defined(UNDER_CE)
		return new CWebkitUI(m_postilMaker);
#else
		return new CWebkitUI();
#endif

	return NULL;
}

void MainFrame::OnFinalMessage(HWND hWnd)
{
	if ((webkit_ != NULL) && (view_ != NULL))
	{
		view_->CancelLoad();
		view_->ShutdownView();
		webkit_->DestroyView(view_);
		webkit_->DestroyFontServerWrapperInterface(font_server_);
		webkit_->DestroyGlyphCacheWrapperInterface(glyph_cache_);	
		webkit_->Shutdown();
		webkit_->Destroy();

		if (font_style_)
			font_style_->Destroy();

		view_ = NULL;
		webkit_ = NULL;

		FreeLibrary(webkit_dll_);
		webkit_dll_ = NULL;
	}

	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

tString MainFrame::GetSkinFile()
{
#if defined(UNDER_CE)
	return _T("main_ce.xml");
#else
	return _T("main.xml");
#endif
}

LRESULT MainFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//m_layoutEvent.set();
	return __super::OnSize(uMsg, wParam, lParam, bHandled);
}

LRESULT MainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed) 
	{
		if (!bZoomed)
		{
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
		}
		else 
		{
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
		}
	}
#else
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}

LRESULT MainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT MainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void MainFrame::OnTimer(TNotifyUI& msg)
{
	switch (msg.wParam)
	{
	case kStartupTimerId:
		{
			paint_manager_.KillTimer(msg.pSender, kStartupTimerId);

			webkit_control_ = static_cast<CWebkitUI*>(paint_manager_.FindControl(kWebkitControlName));
			if (webkit_control_ != NULL)
			{
				InitEAWebkit();

				paint_manager_.SetTimer(webkit_control_, kViewTickTimerId, kViewTickTimerElapse);
				if (view_ != NULL)
				{
					webkit_control_->SetEARasterAndView(raster_, view_, webkit_layout_);
					UpdateNavigatingButtonStatus();
					//view_->SetHTML(kHomeHTML, strlen(kHomeHTML));
					//view_->SetURI(kHomeURL);
					//webkit_layout_->Load(kHomeURL, view_);
				}
			}
		}
		break;
	case kViewTickTimerId:
		{
			if (view_ != NULL)
				view_->Tick();
			//paint_manager_.KillTimer(msg.pSender, kViewTickTimerId);
		}
		break;
	default:
		break;
	}
}

LRESULT MainFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ((uMsg == WM_KEYDOWN) && (wParam == VK_TAB))
	{
		CWebkitUI* webkit_control = static_cast<CWebkitUI*>(paint_manager_.FindControl(kWebkitControlName));
		if (view_ && webkit_control && paint_manager_.GetFocus() == webkit_control)
		{
			KeyboardEvent event;
			view_->AdvanceFocus((GetKeyState(VK_SHIFT) < 0 ) ? FocusDirectionBackward : FocusDirectionForward, event);
			bHandled = true;
		}
	}

	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}

void MainFrame::OnExit(TNotifyUI& msg)
{
	Close();
}

void MainFrame::Init()
{
#if !defined(UNDER_CE)
	DragAcceptFiles(m_hWnd, TRUE);
#endif
}

void MainFrame::InitEAWebkit()
{
	typedef EA::WebKit::IEAWebkit* (* PF_CreateEAWebkitInstance)(void);
	typedef Hanvon::WebkitLayout::IWebKitLayout* (* PF_CreateWebkitLayoutInstance)(void);

	PF_CreateEAWebkitInstance create_webkit_instance = NULL;
	PF_CreateWebkitLayoutInstance create_webkit_layout_instance;

	webkit_dll_ = LoadLibrary(_T("EAWebkit.dll"));
	if (webkit_dll_ != NULL)
	{
#if defined(UNDER_CE)
		create_webkit_instance = reinterpret_cast<PF_CreateEAWebkitInstance>(GetProcAddress(webkit_dll_, _T("CreateEAWebkitInstance")));
		create_webkit_layout_instance = reinterpret_cast<PF_CreateWebkitLayoutInstance>(GetProcAddress(webkit_dll_, _T("CreateWebkitLayoutInstance")));
#else
		create_webkit_instance = reinterpret_cast<PF_CreateEAWebkitInstance>(GetProcAddress(webkit_dll_, "CreateEAWebkitInstance"));
		create_webkit_layout_instance = reinterpret_cast<PF_CreateWebkitLayoutInstance>(GetProcAddress(webkit_dll_, "CreateWebkitLayoutInstance"));
#endif
	}

	if ((create_webkit_instance != NULL) && (webkit_control_ != NULL) && (create_webkit_layout_instance != NULL))
	{
		webkit_layout_ = create_webkit_layout_instance();
		webkit_ = create_webkit_instance();
		webkit_->Init(NULL);

		glyph_cache_ = webkit_->CreateGlyphCacheWrapperInterface(NULL);
		font_server_ = webkit_->CreateFontServerWrapperInterface(NULL);

#if defined(UNDER_CE)
		uint32_t nCount = font_server_->AddDirectory(paint_manager_.GetInstancePath().GetData(), L"*.tt?");
#else
		wchar_t szWindowsDir[MAX_PATH] = {0};
		GetWindowsDirectoryW(szWindowsDir, MAX_PATH);
		wchar_t szFontDir[MAX_PATH] = {0};
		swprintf_s(szFontDir, MAX_PATH - 1, L"%s\\Fonts", szWindowsDir);
		uint32_t nCount = font_server_->AddDirectory(szFontDir, L"*.tt?");
		//uint32_t nCount = font_server_->AddDirectory(L"./", L"*.tt?");
#endif

		font_style_ = font_server_->CreateTextStyle();
		font_style_->SetSize(static_cast<float>(kDefaultFontSize));
		font_style_->SetSmooth(kSmoothEnabled);

#if !defined(UNDER_CE)
		nCount = font_server_->EnumerateFonts(NULL, 0);
		if (nCount > 0)
		{
			IFontDescription* font_description_array = new IFontDescription[nCount];
			font_server_->EnumerateFonts(font_description_array, nCount);
			for (uint32_t index = 0; index < nCount; ++index)
			{
				if (_tcsicmp(_T("Simsun"), (font_description_array + index)->mFamily) == 0)
				{}
			}
			delete[] font_description_array;
		}
#endif

		Parameters& param = webkit_->GetParameters();
		param.mpUserAgent = user_agent_;
		param.mpApplicationName = application_name_;

		//param.mbEnableGammaCorrection = false;
		//param.mColors[kColorActiveSelectionBack] = Color::MAGENTA;
		//param.mColors[kColorActiveSelectionFore] = Color::BLUE;

		// default "en-us"
		param.mpLocale = "zh-cn";

		param.mDefaultFontSize = kDefaultFontSize;
		param.mDefaultMonospaceFontSize = kDefaultFontSize;
		param.mMinimumFontSize = kMiniFontSize;
		param.mMinimumLogicalFontSize = kMiniFontSize;

		param.mSystemFontDescription.mSize = kDefaultFontSize;

#if defined(UNDER_CE)
		param.mEnableSmoothText = false;

		param.mJavaScriptEnabled = false;
		param.mJavaScriptCanOpenWindows = false;
		param.mPluginsEnabled = false;

		sprintf(param.mSystemFontDescription.mFamilies, "Simsun");
		sprintf(param.mFontFamilyStandard, "Simsun");
		sprintf(param.mFontFamilySerif, "Simsun");
		sprintf(param.mFontFamilySansSerif, "Simsun");
		sprintf(param.mFontFamilyMonospace, "Simsun");
		sprintf(param.mFontFamilyCursive, "Simsun");
		sprintf(param.mFontFamilyFantasy, "Simsun");
#else
		param.mEnableSmoothText = true;

		sprintf_s(param.mSystemFontDescription.mFamilies, sizeof(param.mSystemFontDescription.mFamilies) / sizeof(param.mSystemFontDescription.mFamilies[0]),\
			"Arial,Microsoft Yahei,Simsun,sans-serif");
		sprintf_s(param.mFontFamilyStandard, sizeof(param.mFontFamilyStandard) / sizeof(param.mFontFamilyStandard[0]), "Simsun");
		sprintf_s(param.mFontFamilySerif, sizeof(param.mFontFamilySerif) / sizeof(param.mFontFamilySerif[0]), "Simsun");
		sprintf_s(param.mFontFamilySansSerif, sizeof(param.mFontFamilySansSerif) / sizeof(param.mFontFamilySansSerif[0]), "NSimsun");
		sprintf_s(param.mFontFamilyMonospace, sizeof(param.mFontFamilyMonospace) / sizeof(param.mFontFamilyMonospace[0]), "Simsun");
		sprintf_s(param.mFontFamilyCursive, sizeof(param.mFontFamilyCursive) / sizeof(param.mFontFamilyCursive[0]), "Simsun");
		sprintf_s(param.mFontFamilyFantasy, sizeof(param.mFontFamilyFantasy) / sizeof(param.mFontFamilyFantasy[0]), "Comic Sans MS");
#endif

		webkit_->SetParameters(param);

		view_ = webkit_->CreateView();
		webkit_->SetViewNotification(this);
		raster_ = webkit_->GetEARasterInstance();
	}
}

void MainFrame::OnPrepare(TNotifyUI& msg)
{
	CWebkitUI* webkit_control = static_cast<CWebkitUI*>(paint_manager_.FindControl(kWebkitControlName));
	if (webkit_control != NULL)
		paint_manager_.SetTimer(webkit_control, kStartupTimerId, kStartupTimerElapse);
}

void MainFrame::UpdateNavigatingButtonStatus()
{
#if !defined(UNDER_CE)
	if (view_ != NULL)
	{
		CButtonUI* prev_page_button = static_cast<CButtonUI*>(paint_manager_.FindControl(kPrevPageControlName));
		CButtonUI* next_page_button = static_cast<CButtonUI*>(paint_manager_.FindControl(kNextPageControlName));
		CControlUI* page_count = paint_manager_.FindControl(kPageCountControlName);
		CButtonUI* get_text_button = static_cast<CButtonUI*>(paint_manager_.FindControl(kGetTextControlName));
		CButtonUI* get_selected_text_button = static_cast<CButtonUI*>(paint_manager_.FindControl(kGetSelectedTextControlName));
		//CButtonUI* zoom_in_button = static_cast<CButtonUI*>(paint_manager_.FindControl(kZoomInCtonrolName));
		//CButtonUI* zoom_out_button = static_cast<CButtonUI*>(paint_manager_.FindControl(kZoomOutCtonrolName));
		if ((prev_page_button != NULL) && (next_page_button != NULL) && (page_count != NULL) \
			&& (get_text_button != NULL) &&	(get_selected_text_button != NULL) \
			/*&& (zoom_in_button != NULL) && (zoom_out_button != NULL)*/)
		{
			if (using_webkit_layout_)
			{
				//zoom_in_button->SetEnabled();
				//zoom_out_button->SetEnabled();
				get_selected_text_button->SetEnabled();
				get_text_button->SetEnabled();
				prev_page_button->SetEnabled(g_iReaderData->m_nCurPage > 0);
				next_page_button->SetEnabled(g_iReaderData->m_nCurPage + 1 < g_iReaderData->m_nPagesCount);

				TCHAR szBuf[MAX_PATH] = {0};
				_stprintf_s(szBuf, MAX_PATH, _T("%d/%d"), g_iReaderData->m_nCurPage + 1, g_iReaderData->m_nPagesCount);
				page_count->SetText(szBuf);
			}
			else
			{
				//zoom_in_button->SetEnabled(false);
				//zoom_out_button->SetEnabled(false);
				get_selected_text_button->SetEnabled(false);
				prev_page_button->SetEnabled(false);
				next_page_button->SetEnabled(false);
				get_text_button->SetEnabled(false);
				page_count->SetText(_T(""));
			}
		}
	}
#endif
}

void MainFrame::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, kWindowInit) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, kClick) == 0)
	{
#if !defined(UNDER_CE)
		if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
			OnExit(msg);
		else if (_tcsicmp(msg.pSender->GetName(), kPrevPageControlName) == 0)
		{
			if (webkit_layout_ != NULL)
			{
				int layout_height = webkit_layout_->PrevPage(view_);
				webkit_control_->UpdateWebkitLayoutInfo(using_webkit_layout_, layout_height, g_iReaderData->m_nFontSize);
				UpdateNavigatingButtonStatus();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kNextPageControlName) == 0)
		{
			if (webkit_layout_ != NULL)
			{
				int layout_height = webkit_layout_->NextPage(view_);
				webkit_control_->UpdateWebkitLayoutInfo(using_webkit_layout_, layout_height, g_iReaderData->m_nFontSize);
				UpdateNavigatingButtonStatus();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kGetTextControlName) == 0)
		{
			if (webkit_layout_ != NULL)
			{
				LPWSTR buffer = webkit_layout_->GetPageText(g_iReaderData->m_nCurPage, view_, true);
				if (buffer != NULL)
				{
					InputDialog input_dialog(_T("本页面的文字"), buffer, true);
					input_dialog.Create(paint_manager_.GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW, CRect());
					input_dialog.CenterWindow();
					input_dialog.ShowModal(paint_manager_.GetPaintWindow());
					LocalFree(buffer);
				}
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kGetSelectedTextControlName) == 0)
		{
			if (webkit_layout_ != NULL)
			{
				LPWSTR buffer = webkit_layout_->GetSelectedText(view_);
				if (buffer != NULL)
				{
					InputDialog input_dialog(_T("本页面的上选中的文字"), buffer, true);
					input_dialog.Create(paint_manager_.GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW, CRect());
					input_dialog.CenterWindow();
					input_dialog.ShowModal(paint_manager_.GetPaintWindow());
					LocalFree(buffer);
				}
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kZoomInCtonrolName) == 0)
		{
			if (webkit_layout_ != NULL)
			{
				_stprintf_s(g_iReaderData->m_szFontFamily, MAX_PATH, _T("Simsun"));
				if (++font_size_index_ > kFontCount)
					font_size_index_ = 0;
				g_iReaderData->m_nFontSize = kFontSize[font_size_index_];
				webkit_layout_->FontSizeChanged(view_);

				using_webkit_layout_ = true;
				webkit_layout_->BeginWebkitLayout(view_, webkit_control_->GetPos());
				int layout_height = webkit_layout_->NavigatePage(0, view_);
				webkit_control_->UpdateWebkitLayoutInfo(using_webkit_layout_, layout_height, g_iReaderData->m_nFontSize);
				UpdateNavigatingButtonStatus();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kZoomOutCtonrolName) == 0)
		{
			if (webkit_layout_ != NULL)
			{
				_stprintf_s(g_iReaderData->m_szFontFamily, MAX_PATH, _T("Simsun"));
				if (--font_size_index_ < 0)
					font_size_index_ = 0;
				g_iReaderData->m_nFontSize = kFontSize[font_size_index_];

				webkit_layout_->FontSizeChanged(view_);
				using_webkit_layout_ = true;
				webkit_layout_->BeginWebkitLayout(view_, webkit_control_->GetPos());
				int layout_height = webkit_layout_->NavigatePage(0, view_);
				webkit_control_->UpdateWebkitLayoutInfo(using_webkit_layout_, layout_height, g_iReaderData->m_nFontSize);
				UpdateNavigatingButtonStatus();

			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMinButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMaxButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MAXIMIZE);
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
#else
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kRestoreButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_RESTORE);
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
#else
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
#endif
		}
#endif
	}
	else if (_tcsicmp(msg.sType, kTimer) == 0)
		return OnTimer(msg);
}

bool MainFrame::ViewUpdate(ViewUpdateInfo& view_update_info)
{
	if ((webkit_control_ != NULL) && (view_update_info.mDrawEvent == ViewUpdateInfo::kViewDrawEnd))
	{
		if (layout_completed_)
		{
			CRect invalidateRect(view_update_info.mX, view_update_info.mY, view_update_info.mW, view_update_info.mH);
			webkit_control_->LayoutChanged(invalidateRect);
		}
	}
	//VERBOSE(_T("view_update_info.mDrawEvent = %d, view_update_info.mX = %d, view_update_info.mY = %d, view_update_info.mW = %d, view_update_info.mH = %d.\n"), view_update_info.mDrawEvent, view_update_info.mX, view_update_info.mY, view_update_info.mW, view_update_info.mH);
	return true;
}

bool MainFrame::LoadUpdate(LoadInfo& load_info)
{
	CControlUI* app_title = paint_manager_.FindControl(kTitleControlName);
	if ((app_title != NULL) && (load_info.mLET == kLETTitleReceived) && ((load_info.mProgressEstimation == 0.0) || (load_info.mProgressEstimation == 1.0)) && _tcslen(webkit_->GetCharacters(load_info.mPageTitle)) > 0)
		app_title->SetText(webkit_->GetCharacters(load_info.mPageTitle));

	if (load_info.mLET == kLETLayoutCompleted)
	{
		layout_completed_ = true;
		using_webkit_layout_ = true;
		webkit_layout_->BeginWebkitLayout(view_, webkit_control_->GetPos());
#if !defined(UNDER_CE)
		int layout_height = webkit_layout_->NavigatePage(0, view_);
		webkit_control_->UpdateWebkitLayoutInfo(using_webkit_layout_, layout_height, g_iReaderData->m_nFontSize);
		UpdateNavigatingButtonStatus();
#endif

		m_layoutEvent.set();
	}

	return true;
}

LRESULT MainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (webkit_layout_ && view_ && webkit_control_ && g_iReaderData && (g_uInterProcMsg > 0) && (g_uInterProcMsg == uMsg))
	{
		bHandled = TRUE;
		switch (wParam)
		{
		case IREADER_QUIT:
			PostQuitMessage(0);
			break;
		case IREADER_OPEN:
			if (_tcslen(g_iReaderData->m_szFilePath) > 0)
			{
				if (g_iReaderData && (_tcslen(g_iReaderData->m_szFontFamily) > 0) && (g_iReaderData->m_nFontSize > 0))
				{
					Parameters& param = webkit_->GetParameters();

					param.mDefaultFontSize = g_iReaderData->m_nFontSize;
					param.mDefaultMonospaceFontSize = g_iReaderData->m_nFontSize;
					param.mMinimumFontSize = g_iReaderData->m_nFontSize;
					param.mMinimumLogicalFontSize = g_iReaderData->m_nFontSize;

					param.mSystemFontDescription.mSize = g_iReaderData->m_nFontSize;
#if defined(UNDER_CE)
					param.mJavaScriptEnabled = false;
					param.mJavaScriptCanOpenWindows = false;
					param.mPluginsEnabled = false;

					param.mEnableSmoothText = false;
#else
					param.mEnableSmoothText = true;
#endif
					LPSTR lpstrFontFamily = StringConvertor::WideToAnsi(g_iReaderData->m_szFontFamily);
					sprintf(param.mSystemFontDescription.mFamilies, lpstrFontFamily);
					sprintf(param.mFontFamilyStandard, lpstrFontFamily);
					sprintf(param.mFontFamilySerif, lpstrFontFamily);
					sprintf(param.mFontFamilySansSerif, lpstrFontFamily);
					sprintf(param.mFontFamilyMonospace, lpstrFontFamily);
					sprintf(param.mFontFamilyCursive, lpstrFontFamily);
					sprintf(param.mFontFamilyFantasy, lpstrFontFamily);
					StringConvertor::StringFree(lpstrFontFamily);

					webkit_->SetParameters(param);
				}

				if (g_iReaderData) {
					g_iReaderData->m_hasSelectionText = false;
					g_iReaderData->m_nCurPage = 1;
					g_iReaderData->m_nPagesCount = 1;
					g_iReaderData->m_bUnderSearch = false;
					g_iReaderData->m_nNextMatches = 0;
					g_iReaderData->m_nPrevMatches = 0;
				}

				webkit_layout_->ClearTextSelection(view_);

#if defined(UNDER_CE)
				m_postilMaker.SetInfo(m_hWnd, g_iReaderData->m_szPostilPath);
#endif
				layout_completed_ = false;

				for (size_t i = 0; i < _tcslen(g_iReaderData->m_szFilePath); ++i)
					if (g_iReaderData->m_szFilePath[i] == _T('\\'))
						g_iReaderData->m_szFilePath[i] = _T('/');

				webkit_layout_->Load(g_iReaderData->m_szFilePath, view_);
			}
			break;
		case IREADER_CLEAR_CACHE:
			webkit_layout_->ClearCache(view_);
#if defined(UNDER_CE)
			m_postilMaker.Save();
#endif
			break;
		case IREADER_CLEAR_POSTIL:
			m_postilMaker.ClearPageData(static_cast<DWORD>(lParam));
			break;
		case IREADER_INVALIDATE:
			InvalidateRect(m_hWnd, NULL, TRUE);
			if (lParam)
				UpdateWindow(m_hWnd);
			break;
		case IREADER_DIVIDEPAGES:
			webkit_layout_->BeginWebkitLayout(view_, webkit_control_->GetPos());
			m_syncData.set();
			break;
		case IREADER_NAVIGATE:
			if (lParam < webkit_layout_->PageCount(view_))
			{
#if defined(UNDER_CE)
				m_postilMaker.SetZoom(g_iReaderData->m_nFontSize, g_iReaderData->m_nPagesCount);

				g_iReaderData->m_bCurrentPageHasPostil = m_postilMaker.GetPageData(static_cast<int>(lParam));
#endif
				int layout_height = webkit_layout_->NavigatePage(static_cast<int>(lParam), view_);
				webkit_control_->UpdateWebkitLayoutInfo(using_webkit_layout_, layout_height, g_iReaderData->m_nFontSize);
				webkit_control_->LayoutChanged(CRect());
			}
			break;
		case IREADER_ZOOM:
			{
				if (g_iReaderData && (_tcslen(g_iReaderData->m_szFontFamily) > 0) && (g_iReaderData->m_nFontSize > 0))
				{
					Parameters& param = webkit_->GetParameters();

					param.mDefaultFontSize = g_iReaderData->m_nFontSize;
					param.mDefaultMonospaceFontSize = g_iReaderData->m_nFontSize;
					param.mMinimumFontSize = g_iReaderData->m_nFontSize;
					param.mMinimumLogicalFontSize = g_iReaderData->m_nFontSize;

					param.mSystemFontDescription.mSize = g_iReaderData->m_nFontSize;
#if defined(UNDER_CE)
					param.mJavaScriptEnabled = false;
					param.mJavaScriptCanOpenWindows = false;
					param.mPluginsEnabled = false;
					param.mEnableSmoothText = false;
#else
					param.mEnableSmoothText = true;
#endif
					LPSTR lpstrFontFamily = StringConvertor::WideToAnsi(g_iReaderData->m_szFontFamily);
					sprintf(param.mSystemFontDescription.mFamilies, lpstrFontFamily);
					sprintf(param.mFontFamilyStandard, lpstrFontFamily);
					sprintf(param.mFontFamilySerif, lpstrFontFamily);
					sprintf(param.mFontFamilySansSerif, lpstrFontFamily);
					sprintf(param.mFontFamilyMonospace, lpstrFontFamily);
					sprintf(param.mFontFamilyCursive, lpstrFontFamily);
					sprintf(param.mFontFamilyFantasy, lpstrFontFamily);
					StringConvertor::StringFree(lpstrFontFamily);

					webkit_->SetParameters(param);
				}
				webkit_layout_->FontSizeChanged(view_);
			}
			break;
		case IREADER_PAGENUMBYOFFSET:
			if (g_iReaderData) {
				g_iReaderData->m_nPageNumByOffset = webkit_layout_->GetPageNumByOffset(static_cast<int>(lParam), view_);
				m_syncData.set();
			}
			else
				m_syncData.set();
			break;
		case IREADER_PAGEOFFSET:
			if (g_iReaderData) {
				g_iReaderData->m_nCurOffset = webkit_layout_->GetPageOffset(static_cast<int>(lParam), view_);
				m_syncData.set();
			}
			else
				m_syncData.set();
			break;
		case IREADER_CUROFFSET:
			if (g_iReaderData) {
				g_iReaderData->m_nCurOffset = webkit_layout_->GetPageOffset(webkit_layout_->CurPage(view_), view_);
				m_syncData.set();
			}
			else
				m_syncData.set();
			break;
		case IREADER_GET_BEGIN_TEXT:
			if (g_iReaderData) {
				LPWSTR buffer = webkit_layout_->GetPageText(static_cast<int>(lParam), view_, false);
				if (buffer != NULL)
				{
					memcpy_s(g_iReaderData->m_szSearchText, MAX_PATH - 1, buffer, MAX_PATH - 1);
					LocalFree(buffer);
				}
				m_syncData.set();
			}
			break;
		case IREADER_TTS:
			if (g_iReaderData) {
				LPWSTR buffer = webkit_layout_->GetPageText(static_cast<int>(lParam), view_, true);
				if (buffer != NULL)
					LocalFree(buffer);
				m_syncData.set();
			}
			break;
		case IREADER_CLEARSELECTION:
			if (g_iReaderData) {
				webkit_layout_->ClearTextSelection(view_);
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
			break;
		case IREADER_SEARCH:
			{
				static std::vector<unsigned> matchedPages;
				switch (lParam)
				{
					// start a new searching task
				case 0:
					if (g_iReaderData
						&& (_tcslen(g_iReaderData->m_szSearchText) > 0)) {
							g_iReaderData->m_bUnderSearch = false;
							g_iReaderData->m_nNextMatches = 0;
							g_iReaderData->m_nPrevMatches = 0;
							
							matchedPages.clear();

							unsigned matches = 0;
							unsigned* pMatchedPages = NULL;

							webkit_layout_->SearchText(g_iReaderData->m_szSearchText, view_, 0, matches, &pMatchedPages);
							if (pMatchedPages)
							{
								for (unsigned i = 0; i < matches; ++i)
									matchedPages.push_back(pMatchedPages[i]);
								LocalFree(pMatchedPages);
							}

							if (!matchedPages.empty())
							{
								unsigned currentPage = static_cast<unsigned>(webkit_layout_->CurPage(view_));
								if (find(matchedPages.begin(), matchedPages.end(), currentPage) == matchedPages.end())
								{
									for (std::vector<unsigned>::const_iterator citer = matchedPages.begin(); citer != matchedPages.end(); ++citer) {
										if (*citer > currentPage) {
											int layout_height = webkit_layout_->NavigatePage(*citer, view_);
											webkit_control_->UpdateWebkitLayoutInfo(using_webkit_layout_, layout_height, g_iReaderData->m_nFontSize);
											break;
										}
									}
								}
								currentPage = static_cast<unsigned>(webkit_layout_->CurPage(view_));

								for (std::vector<unsigned>::const_iterator citer = matchedPages.begin(); citer != matchedPages.end(); ++citer) {
									if (*citer < currentPage)
										++g_iReaderData->m_nPrevMatches;
									else if (*citer > currentPage)
										++g_iReaderData->m_nNextMatches;
								}
								g_iReaderData->m_bUnderSearch = true;
							}							
					}
					m_syncData.set();
					break;
					// get the previous matched pages
				case 1:
					if ((_tcslen(g_iReaderData->m_szSearchText) > 0)
						&& g_iReaderData->m_bUnderSearch
						&& (g_iReaderData->m_nPrevMatches >= 1)
						&& (g_iReaderData->m_nPrevMatches <= static_cast<int>(matchedPages.size()))
						&& !matchedPages.empty()) {
							g_iReaderData->m_nNextMatches = 0;
							g_iReaderData->m_nPrevMatches = 0;
							unsigned nCurrentMatchedPage = static_cast<unsigned>(webkit_layout_->CurPage(view_));
							unsigned nPrevMatchedPage = webkit_layout_->PageCount(view_);

							for (size_t index = matchedPages.size() - 1; index >= 0; --index) {
								if (matchedPages.at(index) < nCurrentMatchedPage) {
									nPrevMatchedPage = matchedPages.at(index);
									break;
								}
							}

							if ((nPrevMatchedPage >= 0)
								&& (nPrevMatchedPage < webkit_layout_->PageCount(view_)))
							{
								int layout_height = webkit_layout_->NavigatePage(nPrevMatchedPage, view_);
								webkit_control_->UpdateWebkitLayoutInfo(using_webkit_layout_, layout_height, g_iReaderData->m_nFontSize);
							}

							unsigned currentPage = static_cast<unsigned>(webkit_layout_->CurPage(view_));
							for (std::vector<unsigned>::const_iterator citer = matchedPages.begin(); citer != matchedPages.end(); ++citer) {
								if (*citer < currentPage)
									++g_iReaderData->m_nPrevMatches;
								else if (*citer > currentPage)
									++g_iReaderData->m_nNextMatches;
							}
					}
					m_syncData.set();
					break;
					// get the next matched pages
				case 2:
					if (g_iReaderData
						&& (_tcslen(g_iReaderData->m_szSearchText) > 0)
						&& g_iReaderData->m_bUnderSearch
						&& (g_iReaderData->m_nNextMatches > 0)
						&& (g_iReaderData->m_nNextMatches <= static_cast<int>(matchedPages.size()))
						&& !matchedPages.empty()) {
							g_iReaderData->m_nNextMatches = 0;
							g_iReaderData->m_nPrevMatches = 0;
							int nCurrentMatchedPage = static_cast<unsigned>(webkit_layout_->CurPage(view_));
							int nNextMatchedPage = webkit_layout_->PageCount(view_);

							for (unsigned index = 0; index < matchedPages.size(); ++index) {
								if (matchedPages.at(index) > nCurrentMatchedPage) {
									nNextMatchedPage = matchedPages.at(index);
									break;
								}
							}

							if ((nNextMatchedPage >= 0)
								&& (nNextMatchedPage < webkit_layout_->PageCount(view_)))
							{								
								int layout_height = webkit_layout_->NavigatePage(nNextMatchedPage, view_);
								webkit_control_->UpdateWebkitLayoutInfo(using_webkit_layout_, layout_height, g_iReaderData->m_nFontSize);
							}

							unsigned currentPage = static_cast<unsigned>(webkit_layout_->CurPage(view_));
							for (std::vector<unsigned>::const_iterator citer = matchedPages.begin(); citer != matchedPages.end(); ++citer) {
								if (*citer < currentPage)
									++g_iReaderData->m_nPrevMatches;
								else if (*citer > currentPage)
									++g_iReaderData->m_nNextMatches;
							}
					}
					m_syncData.set();
					break;
					// get the first matched page
				case 3:
					if (g_iReaderData
						&& (_tcslen(g_iReaderData->m_szSearchText) > 0)
						&& g_iReaderData->m_bUnderSearch
						&& !matchedPages.empty()) {
							g_iReaderData->m_nNextMatches = 0;
							g_iReaderData->m_nPrevMatches = 0;

							int nFirstMatchedPage = matchedPages.at(0);

							if ((nFirstMatchedPage >= 0)
								&& (nFirstMatchedPage < webkit_layout_->PageCount(view_)))
							{
								int layout_height = webkit_layout_->NavigatePage(nFirstMatchedPage, view_);
								webkit_control_->UpdateWebkitLayoutInfo(using_webkit_layout_, layout_height, g_iReaderData->m_nFontSize);
							}

							unsigned currentPage = static_cast<unsigned>(webkit_layout_->CurPage(view_));
							for (std::vector<unsigned>::const_iterator citer = matchedPages.begin(); citer != matchedPages.end(); ++citer) {
								if (*citer < currentPage)
									++g_iReaderData->m_nPrevMatches;
								else if (*citer > currentPage)
									++g_iReaderData->m_nNextMatches;
							}
					}
					m_syncData.set();
					break;
					// get the last matched page
				case 4:
					if (g_iReaderData
						&& (_tcslen(g_iReaderData->m_szSearchText) > 0)
						&& g_iReaderData->m_bUnderSearch
						&& !matchedPages.empty()) {
							g_iReaderData->m_nNextMatches = 0;
							g_iReaderData->m_nPrevMatches = 0;

							int nLastMatchedPage = matchedPages.at(matchedPages.size() - 1);

							if ((nLastMatchedPage >= 0)
								&& (nLastMatchedPage < webkit_layout_->PageCount(view_)))
							{
								int layout_height = webkit_layout_->NavigatePage(nLastMatchedPage, view_);
								webkit_control_->UpdateWebkitLayoutInfo(using_webkit_layout_, layout_height, g_iReaderData->m_nFontSize);
							}

							unsigned currentPage = static_cast<unsigned>(webkit_layout_->CurPage(view_));
							for (std::vector<unsigned>::const_iterator citer = matchedPages.begin(); citer != matchedPages.end(); ++citer) {
								if (*citer < currentPage)
									++g_iReaderData->m_nPrevMatches;
								else if (*citer > currentPage)
									++g_iReaderData->m_nNextMatches;
							}
					}
					m_syncData.set();
					break;
				}
				break;
			}
		}
	}

#if !defined(UNDER_CE)
	switch (uMsg)
	{
	case WM_DROPFILES:
		{
			TCHAR szFile[MAX_PATH] = {0};
			HDROP hDrop = reinterpret_cast<HDROP>(wParam);

			UINT uFilesCount = DragQueryFile(hDrop, 0xFFFFFFFF, szFile, MAX_PATH);
			if ((uFilesCount == 1) && (DragQueryFile(hDrop, 0, szFile, MAX_PATH) > 0) && (view_ != NULL))
			{
				layout_completed_ = false;
				//using_webkit_layout_ = false;

				//webkit_layout_->EndWebkitLayout(view_);
				webkit_control_->UpdateWebkitLayoutInfo(using_webkit_layout_, 0, g_iReaderData->m_nFontSize);
				UpdateNavigatingButtonStatus();

				for (size_t i = 0; i < _tcslen(szFile); ++i)
					if (szFile[i] == _T('\\'))
						szFile[i] = _T('/');

				tString dropFileName(_T("file:///"));
				dropFileName += szFile;

				LPSTR lpstrUtf8Url = StringConvertor::WideToUtf8(dropFileName.c_str());
				std::string encode_url = UrlEncode(lpstrUtf8Url);
				view_->SetURI(encode_url.c_str());
				StringConvertor::StringFree(lpstrUtf8Url);
			}
			DragFinish(hDrop);
		}
		break;
	default:
		break;
	}
#endif
	return 0;
}


CEvent::CEvent(LPCTSTR lpszName, BOOL bManualReset, BOOL bInitialState)
{
	m_hEvent = CreateEvent(NULL,bManualReset,bInitialState,lpszName);
}

CEvent::~CEvent()
{
	if (m_hEvent != NULL)
	{
		CloseHandle(m_hEvent);
	}
}

bool CEvent::isSigned()
{
	bool IsSigneded = false;
	if (m_hEvent != NULL)
	{
		DWORD dwWaitResult = WaitForSingleObject(m_hEvent,1);
        switch (dwWaitResult) 
        {
		// The thread got ownership of the mutex
		case WAIT_OBJECT_0:
		// The thread got ownership of an abandoned mutex
		case WAIT_ABANDONED:
			break;
		case WAIT_TIMEOUT:
			IsSigneded = true;
			break;
        }
	}
	return IsSigneded;
}

bool CEvent::wait(DWORD dwTimeOut)
{
	bool IsSigneded = false;
	if (m_hEvent != NULL)
	{
		DWORD dwWaitResult = WaitForSingleObject(m_hEvent, dwTimeOut);
        switch (dwWaitResult) 
        {
		// The thread got ownership of the mutex
		case WAIT_OBJECT_0:
		// The thread got ownership of an abandoned mutex
		case WAIT_ABANDONED:
			IsSigneded = true;
			break;
		case WAIT_TIMEOUT:
			IsSigneded = false;
			break;
		case WAIT_FAILED:
			IsSigneded = false;
			break;
		default:
			break;
        }
		ResetEvent(m_hEvent);
	}
	else
	{
		IsSigneded = true;
	}

	return IsSigneded;
}

void CEvent::reset()
{
	if (m_hEvent != NULL)
		ResetEvent(m_hEvent);
}

void CEvent::set()
{
	if (m_hEvent != NULL)
		SetEvent(m_hEvent);
}