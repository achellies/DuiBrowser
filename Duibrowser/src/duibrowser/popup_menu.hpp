//
// popup_menu.hpp
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

#ifndef POPUP_MENU_HPP
#define POPUP_MENU_HPP

class WindowImplBase;
class PopupMenu : public WindowImplBase
{
public:
	PopupMenu();

	void Init(HWND hosted_wnd);
	void Popup(CPoint point);

	bool AddMenuItem(std::wstring menu_item_title, int menu_item_tag, bool enable, bool checked, bool add_seperate, const std::wstring& icon = std::wstring());

protected:

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void Close();

	void OnFinalMessage(HWND hWnd);	

	void OnPrepare(TNotifyUI& msg);

	void Notify(TNotifyUI& msg);

	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual tString GetSkinFile();

private:
	HWND				hosted_window_;
	std::vector<int>	menu_item_tags_;
};

#endif // POPUP_MENU_HPP