//
// input_dialog.cpp
// ~~~~~~~~~~~~~~~~
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
#include "input_dialog.hpp"

#if !defined(UNDER_CE) && defined(_DEBUG)
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

InputDialog::InputDialog(const tString& title, const tString& text, bool read_only)
: input_string_(text)
, title_(title)
, read_only_(read_only)
{}

LPCTSTR InputDialog::GetWindowClassName() const
{
	return _T("InputDialog");
}

void InputDialog::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

void InputDialog::Close()
{
    ASSERT(::IsWindow(m_hWnd));
    if( !::IsWindow(m_hWnd) ) return;
    PostMessage(WM_CLOSE);
}

tString InputDialog::GetSkinFile()
{
	return _T("input_dialog.xml");
}

void InputDialog::OnPrepare(TNotifyUI& msg)
{
	if (!title_.empty())
	{
		CControlUI* title_control = paint_manager_.FindControl(_T("apptitle"));
		if (title_control != NULL)
			title_control->SetText(title_.c_str());
	}

#if !defined(UNDER_CE)
	if (read_only_ || !input_string_.empty())
	{
		CRichEditUI* rich_edit = static_cast<CRichEditUI*>(paint_manager_.FindControl(_T("input")));
		if (rich_edit != NULL)
		{
			rich_edit->SetText(input_string_.c_str());
			rich_edit->SetReadOnly(read_only_);
		}
	}
#endif
}

tString InputDialog::GetInputString()
{
	return input_string_;
}

void InputDialog::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), _T("closebtn")) == 0)
		{
#if !defined(UNDER_CE)
			CRichEditUI* rich_edit = static_cast<CRichEditUI*>(paint_manager_.FindControl(_T("input")));
			if (rich_edit != NULL)
				input_string_ = rich_edit->GetText();
#endif

			Close();
		}
	}
}