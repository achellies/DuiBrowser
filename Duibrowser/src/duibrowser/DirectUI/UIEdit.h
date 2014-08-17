// DirectUI - UI Library
// Version 1.0.0
// Released: 24th January, 2011 by:
//
//      achellies
//      email: achellies@163.com
//      url: http://directui.googlecode.com/
//
//
// Copyright (c) 2010-2011  achellies
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
//
// Acknowledgements :
// Bjarke Viksoe (http://www.viksoe.dk/code/windowless1.htm)
//
// wangchyz (https://github.com/wangchyz/duilib/)
//
// Beware of bugs.
//
//
//
////////////////////////////////////////////////////////

#ifndef __UIEDIT_H__
#define __UIEDIT_H__

namespace DirectUI {

/////////////////////////////////////////////////////////////////////////////////////
//

class CMultiLineEditWnd;
class CMultiLineEditCanvasWnd;

class UILIB_API CMultiLineEditUI : public CControlUI
{
friend CMultiLineEditWnd;
friend CMultiLineEditCanvasWnd;
public:
	CMultiLineEditUI();
	~CMultiLineEditUI();

	LPCTSTR GetClass() const;
	LPCTSTR GetTypeName() const;
	UINT GetControlFlags() const;

	void Init();

	CStdString GetText() const;
	void SetText(LPCTSTR pstrText);

	void SetEnabled(bool bEnabled);
	void SetVisible(bool bVisible);
	void SetReadOnly(bool bReadOnly);
	void SetFocus(bool bReadOnly);
	bool IsReadOnly() const;

	void Event(TEventUI& event);

	SIZE EstimateSize(SIZE szAvailable);
	void SetPos(RECT rc);
	void SetPos(int left, int top, int right, int bottom);

	void PaintStatusImage(HDC hDC);

	void PaintText(HDC hDC);

	SIZE GetScrollPos() const;
	SIZE GetScrollRange() const;
	void SetScrollRange(SIZE szRange);
	void SetScrollPos(SIZE szPos);
	void LineUp();
	void LineDown();
	void PageUp();
	void PageDown();
	void HomeUp();
	void EndDown();
	void LineLeft();
	void LineRight();
	void PageLeft();
	void PageRight();
	void HomeLeft();
	void EndRight();

protected:
	CMultiLineEditWnd* m_pWindow;
	CMultiLineEditCanvasWnd* m_pCanvas;

	CSize m_szScrollPos;
	CSize m_szScrollRange;
};

}; // namespace DirectUI


#endif //#ifndef __UIEDIT_H__

