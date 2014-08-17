//
// input_dialog.hpp
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

#ifndef INPUT_DIALOG_HPP
#define INPUT_DIALOG_HPP

class WindowImplBase;
class InputDialog : public WindowImplBase
{
public:
	InputDialog();

	tString GetInputString();

protected:

	LPCTSTR GetWindowClassName() const;

	void Close();

	void OnFinalMessage(HWND hWnd);	

	void OnPrepare(TNotifyUI& msg);

	void Notify(TNotifyUI& msg);

	virtual tString GetSkinFile();

private:
	tString input_string_;
};

#endif // INPUT_DIALOG_HPP