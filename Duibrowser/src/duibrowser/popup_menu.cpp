//
// popup_menu.cpp
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
#include "win_impl_base.hpp"
#include "popup_menu.hpp"

#if !defined(UNDER_CE) && defined(_DEBUG)
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

const int kContextMenuWidth = 300;
const int kContextMenuItemHeight = 26;

static const wchar_t* const kMenuItemSkin = _T("popup_menu_item.xml");
static const wchar_t* const kMenuList = _T("menu");
static const wchar_t* const kMenuCheckPngName = _T("menu_check.png");
static const wchar_t* const kMenuItemImageName = _T("image");
static const wchar_t* const kMenuItemTitleName = _T("title");
static const wchar_t* const kMenuItemUnderlineName = _T("underline");

PopupMenu::PopupMenu()
{}

LPCTSTR PopupMenu::GetWindowClassName() const
{
	return _T("PopupMenu");
}

UINT PopupMenu::GetClassStyle() const
{
	return CS_DBLCLKS;
}

void PopupMenu::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void PopupMenu::Close()
{
    ASSERT(::IsWindow(m_hWnd));
    if( !::IsWindow(m_hWnd) ) return;
    PostMessage(WM_CLOSE);
}

tString PopupMenu::GetSkinFile()
{
	return _T("popup_menu.xml");
}

LRESULT PopupMenu::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_hWnd != (HWND)wParam)
	{
		Close();
	}
	return 0;
}

LRESULT PopupMenu::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ((wParam == VK_ESCAPE) || (wParam == VK_MENU)) Close();
	else bHandled = FALSE;
	return 0;
}

void PopupMenu::OnPrepare(TNotifyUI& msg)
{}

void PopupMenu::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{}
	else if (_tcsicmp(msg.sType, _T("itemselect")) == 0)
	{
		//Close();
	}
	else if ((_tcsicmp(msg.sType, _T("itemactivate")) == 0) || (_tcsicmp(msg.sType, _T("itemclick")) == 0))
	{
		if ((msg.pSender->GetInterface(_T("ListContainerElement")) != NULL) && !menu_item_tags_.empty())
		{
			bool fire = false;
			DWORD fire_id = 0;
			CListUI* menu_list = static_cast<CListUI*>(paint_manager_.FindControl(kMenuList));
			if (menu_list != NULL)
			{
				CListContainerElementUI* menu_item = static_cast<CListContainerElementUI*>(msg.pSender);
				int selected_item = menu_list->GetItemIndex(msg.pSender);
				CLabelUI* item_title = static_cast<CLabelUI*>(paint_manager_.FindControl(menu_item, kMenuItemTitleName));
				if ((item_title != NULL) && (selected_item >= 0) && (selected_item < static_cast<int>(menu_item_tags_.size())))
				{
					fire = item_title->IsEnabled();
					fire_id = menu_item_tags_[selected_item];
				}
			}

			if (fire)
			{
				::PostMessage(hosted_window_, WM_MENU, (WPARAM)fire_id, (LPARAM)0);
				Close();
			}
		}
	}
}

bool PopupMenu::AddMenuItem(std::wstring menu_item_title, int menu_item_tag, bool enable, bool checked, bool add_seperate, const std::wstring& icon)
{
	CListUI* menu_list = static_cast<CListUI*>(paint_manager_.FindControl(kMenuList));
	if (menu_list != NULL)
	{
		CDialogBuilder builder;
		CListContainerElementUI* menu_item = static_cast<CListContainerElementUI*>(builder.Create(kMenuItemSkin, (UINT)0, NULL, &paint_manager_));
		if (menu_item != NULL)
		{
			menu_item_tags_.push_back(menu_item_tag);

			CLabelUI* item_title = static_cast<CLabelUI*>(paint_manager_.FindControl(menu_item, kMenuItemTitleName));
			CControlUI* item_image = paint_manager_.FindControl(menu_item, kMenuItemImageName);
			CControlUI* underline = paint_manager_.FindControl(menu_item, kMenuItemUnderlineName);
			if ((item_title != NULL) && (item_image != NULL) && (underline != NULL))
			{
				if (checked)
					item_image->SetBkImage(kMenuCheckPngName);
				else if (!icon.empty())
					item_image->SetBkImage(icon.c_str());

				underline->SetVisible(add_seperate);

				item_title->SetText(menu_item_title.c_str());
				item_title->SetEnabled(enable);
			}

			menu_item->SetVisible(true);
			menu_list->Add(menu_item);

			return true;
		}
	}
	return false;
}

void PopupMenu::Init(HWND hosted_wnd)
{
	hosted_window_ = hosted_wnd;

	CRect rc;
	rc.right = rc.left + kContextMenuWidth;
	rc.bottom = rc.top + kContextMenuItemHeight + 2 * 8;

	Create(hosted_window_, NULL, WS_POPUP, WS_EX_TOOLWINDOW, rc);
}

void PopupMenu::Popup(CPoint point)
{
	CListUI* menu_list = static_cast<CListUI*>(paint_manager_.FindControl(kMenuList));
	if (menu_list != NULL)
	{
		CRect rc;
		rc.left = point.x;
		rc.top = point.y;
		rc.right = rc.left + kContextMenuWidth;
		rc.bottom = rc.top + menu_list->GetCount() * kContextMenuItemHeight + 2 * 8;

		int nWidth = rc.GetWidth();
		int nHeight = rc.GetHeight();

//#if defined(WIN32) && !defined(UNDER_CE)
//		MONITORINFO oMonitor = {};
//		oMonitor.cbSize = sizeof(oMonitor);
//		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
//		CRect rcWork = oMonitor.rcWork;
//#else
		CRect rcWork;
		GetWindowRect(hosted_window_, &rcWork);
//#endif
		if( rc.bottom > rcWork.bottom )
		{
			if( nHeight >= rcWork.GetHeight() )
			{
				rc.top = 0;
				rc.bottom = nHeight;
			}
			else {
				rc.bottom = point.y;
				rc.top = rc.bottom - nHeight;
			}
		}
		if (rc.right > rcWork.right)
		{
			if (nWidth >= rcWork.GetWidth())
			{
				rc.left = 0;
				rc.right = nWidth;
			}
			else
			{
				rc.right = rcWork.right;
				rc.left = rc.right - nWidth;
			}
		}

		menu_list->SelectItem(0);

		MoveWindow(*this, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), TRUE);
		SetWindowPos(*this, NULL, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), SWP_FRAMECHANGED);

		HWND hWndParent = m_hWnd;
		while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
		::ShowWindow(m_hWnd, SW_SHOW);
#if defined(WIN32) && !defined(UNDER_CE)
		::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
#endif
	}
}