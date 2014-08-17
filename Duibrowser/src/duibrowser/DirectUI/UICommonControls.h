//
//
// DirectUI - UI Library
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2006-2007 Bjarke Viksoe.
//
// This code may be used in compiled form in any way you desire. These
// source files may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
////
// Acknowledgements :
// Bjarke Viksoe (http://www.viksoe.dk/code/windowless1.htm)
//
//
//
// Beware of bugs.
//
//
//
////////////////////////////////////////////////////////
#ifndef __UICOMMONCONTROLS_H__
#define __UICOMMONCONTROLS_H__

#ifdef _MSC_VER
#pragma once
#endif

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CLabelUI : public CControlUI
{
public:
    CLabelUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

	// 字体的样式
	// #define DT_TOP                      0x00000000
	// #define DT_LEFT                     0x00000000
	// #define DT_CENTER                   0x00000001
	// #define DT_RIGHT                    0x00000002
	// #define DT_VCENTER                  0x00000004
	// #define DT_BOTTOM                   0x00000008
	// #define DT_WORDBREAK                0x00000010
	// #define DT_SINGLELINE               0x00000020
    void SetTextStyle(UINT uStyle);
	UINT GetTextStyle() const;
	// 字体的颜色
    void SetTextColor(DWORD dwTextColor);
	DWORD GetTextColor() const;
    void SetDisabledTextColor(DWORD dwTextColor);
	DWORD GetDisabledTextColor() const;

	// 字体大小
    void SetFont(int index);
	int GetFont() const;
	// 文字显示的左右边距
    RECT GetTextPadding() const;
    void SetTextPadding(RECT rc);

	// 是否按照类html格式类型显示，如果按照html格式显示，只能采用默认的字体，默认字体可以通过来设定
	//  <Font name="微软雅黑" size="18" bold="false" default = "true"/>
    bool IsShowHtml();
    void SetShowHtml(bool bShowHtml = true);

    SIZE EstimateSize(SIZE szAvailable);
    void DoEvent(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    virtual void PaintText(void* ctx);

protected:
    DWORD m_dwTextColor;
    DWORD m_dwDisabledTextColor;
    int m_iFont;
    UINT m_uTextStyle;
    RECT m_rcTextPadding;
    bool m_bShowHtml;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CButtonUI : public CLabelUI
{
public:
    CButtonUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    bool Activate();
    void SetEnabled(bool bEnable = true);
    void DoEvent(TEventUI& event);

    LPCTSTR GetNormalImage();
    void SetNormalImage(LPCTSTR pStrImage);
    LPCTSTR GetHotImage();
    void SetHotImage(LPCTSTR pStrImage);
    LPCTSTR GetPushedImage();
    void SetPushedImage(LPCTSTR pStrImage);
    LPCTSTR GetFocusedImage();
    void SetFocusedImage(LPCTSTR pStrImage);
    LPCTSTR GetDisabledImage();
    void SetDisabledImage(LPCTSTR pStrImage);
	LPCTSTR GetForeImage();
	void SetForeImage(LPCTSTR pStrImage);

    void SetHotTextColor(DWORD dwColor);
	DWORD GetHotTextColor() const;
    void SetPushedTextColor(DWORD dwColor);
	DWORD GetPushedTextColor() const;
    void SetFocusedTextColor(DWORD dwColor);
	DWORD GetFocusedTextColor() const;

    SIZE EstimateSize(SIZE szAvailable);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	virtual void PaintText(void* ctx);
    virtual void PaintStatusImage(void* ctx);
	virtual void PaintBorder(void* ctx);

protected:
	void PaintForeImage(void* ctx);

protected:
    UINT m_uButtonState;

	DWORD m_dwHotTextColor;
	DWORD m_dwPushedTextColor;
	DWORD m_dwFocusedTextColor;

    CStdString m_sNormalImage;
    CStdString m_sHotImage;
    CStdString m_sPushedImage;
    CStdString m_sFocusedImage;
    CStdString m_sDisabledImage;
	CStdString m_sForeImage;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API COptionUI : public CButtonUI
{
public:
    COptionUI();
    ~COptionUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

    bool Activate();
    void SetEnabled(bool bEnable = true);

    LPCTSTR GetSelectedImage();
    void SetSelectedImage(LPCTSTR pStrImage);
	void SetSelectedTextColor(DWORD dwTextColor);
	DWORD GetSelectedTextColor();

    LPCTSTR GetGroup() const;
    void SetGroup(LPCTSTR pStrGroupName = NULL);
    bool IsSelected() const;
    void Selected(bool bSelected);	

    SIZE EstimateSize(SIZE szAvailable);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void PaintStatusImage(void* ctx);
	void PaintText(void* ctx);

protected:
    bool m_bSelected;
    CStdString m_sGroupName;

	DWORD m_dwSelectedTextColor;

    CStdString m_sSelectedImage;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CTextUI : public CLabelUI
{
public:
    CTextUI();
    ~CTextUI();

    LPCTSTR GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    CStdString* GetLinkContent(int iIndex);

    void DoEvent(TEventUI& event);
    SIZE EstimateSize(SIZE szAvailable);

    void PaintText(void* ctx);

protected:
    enum { MAX_LINK = 8 };
    int m_nLinks;
    RECT m_rcLinks[MAX_LINK];
    CStdString m_sLinks[MAX_LINK];
    int m_nHoverLink;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CProgressUI : public CLabelUI
{
public:
    CProgressUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    bool IsHorizontal();
    void SetHorizontal(bool bHorizontal = true);
    int GetMinValue() const;
    void SetMinValue(int nMin);
    int GetMaxValue() const;
    void SetMaxValue(int nMax);
    int GetValue() const;
    void SetValue(int nValue);
    LPCTSTR GetFgImage() const;
    void SetFgImage(LPCTSTR pStrImage);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    void PaintStatusImage(void* ctx);

protected:
    bool m_bHorizontal;
    int m_nMax;
    int m_nMin;
    int m_nValue;

    CStdString m_sFgImage;
    CStdString m_sFgImageModify;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CSliderUI : public CProgressUI
{
public:
    CSliderUI();

    LPCTSTR GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetEnabled(bool bEnable = true);

    int GetChangeStep() const;
    void SetChangeStep(int step);
    void SetThumbSize(SIZE szXY);
    RECT GetThumbRect() const;
    LPCTSTR GetThumbImage() const;
    void SetThumbImage(LPCTSTR pStrImage);
    LPCTSTR GetThumbHotImage() const;
    void SetThumbHotImage(LPCTSTR pStrImage);
    LPCTSTR GetThumbPushedImage() const;
    void SetThumbPushedImage(LPCTSTR pStrImage);

    void DoEvent(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    void PaintStatusImage(void* ctx);

protected:
    SIZE m_szThumb;
    UINT m_uButtonState;
    int m_nStep;

    CStdString m_sThumbImage;
    CStdString m_sThumbHotImage;
    CStdString m_sThumbPushedImage;

    CStdString m_sImageModify;
};

/////////////////////////////////////////////////////////////////////////////////////
//
class IEditUI
{
public:
	virtual LPCTSTR GetEditClass() const = 0;
	virtual CRect GetEditPos() = 0;
	virtual RECT GetEditTextPadding() const = 0;
	virtual DWORD GetEditTextColor() = 0;
	virtual bool IsPasswordMode() const = 0;
	virtual TCHAR GetPasswordChar() const = 0;
	virtual CPaintManagerUI* GetManager() const = 0;
	virtual UINT GetMaxChar() = 0;
	virtual CStdString GetText() const = 0;
	virtual void SetText(LPCTSTR pstrText) = 0;
	virtual bool IsEnabled() const = 0;
	virtual bool IsReadOnly() const = 0;
	virtual CStdString GetName() const = 0;
	virtual void SetEidtWndNull() = 0;
	virtual bool IsMultiLine() const = 0;
#if(WINVER >= 0x0400)
	virtual bool IsDigitalNumber() const = 0;
#endif
	virtual DWORD GetBkColor() const = 0;
	virtual CControlUI* const GetHostedControl() = 0;
};

class CEditUI;

#if defined(UI_BUILD_FOR_WINGDI)
class CEditWnd : public CWindowWnd
#elif defined(UI_BUILD_FOR_SKIA)
class CEditWnd : public CSkUIWindow
#endif
{
#if defined(UI_BUILD_FOR_SKIA)
	typedef CSkUIWindow INHERITED;
#endif
public:
    CEditWnd();

    void Init(IEditUI* pOwner);
	RECT CalPos();
	void SetEditWndText(LPCTSTR lpsText,bool bSelect=true);
	void MoveEditWnd(RECT rc);

    LPCTSTR GetWindowClassName() const;
    LPCTSTR GetSuperClassName() const;
    void OnFinalMessage(HWND hWnd);

#if defined(UI_BUILD_FOR_WINGDI)
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
#elif defined(UI_BUILD_FOR_SKIA)
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#endif
    LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void BreakLinkage();

	DWORD GetBkColor();
	DWORD GetEditTextColor();

protected:
    IEditUI* m_pOwner;
	HBRUSH m_hBkBrush;
};

class UILIB_API CEditUI : public IEditUI, public CLabelUI
{
    friend CEditWnd;
public:
    CEditUI();
	~CEditUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    void SetEnabled(bool bEnable = true);
    void SetMaxChar(UINT uMax);
    void SetReadOnly(bool bReadOnly);
    void SetPasswordMode(bool bPasswordMode);
    void SetPasswordChar(TCHAR cPasswordChar);
	void SetMultiLine(bool bMultiLine);
#if(WINVER >= 0x0400)
	bool IsDigitalNumber() const;
	void SetDigitalNumber(bool bDigitalNumber);
#endif

    LPCTSTR GetNormalImage();
    void SetNormalImage(LPCTSTR pStrImage);
    LPCTSTR GetHotImage();
    void SetHotImage(LPCTSTR pStrImage);
    LPCTSTR GetFocusedImage();
    void SetFocusedImage(LPCTSTR pStrImage);
    LPCTSTR GetDisabledImage();
    void SetDisabledImage(LPCTSTR pStrImage);

    void SetPos(RECT rc);
    void SetVisible(bool bVisible = true);
    void SetInternVisible(bool bVisible = true);
    SIZE EstimateSize(SIZE szAvailable);
    void DoEvent(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void PaintStatusImage(void* ctx);
    void PaintText(void* ctx);

	//Parent IEditUI
	virtual CControlUI* const GetHostedControl();
	virtual LPCTSTR GetEditClass() const;
	virtual CRect GetEditPos();
	virtual RECT GetEditTextPadding() const;
	virtual DWORD GetEditTextColor();
	virtual bool IsPasswordMode() const;
	virtual TCHAR GetPasswordChar() const;
	virtual CPaintManagerUI* GetManager() const;
	virtual UINT GetMaxChar();
	virtual CStdString GetText() const;
	virtual void SetText(LPCTSTR pstrText);
	virtual bool IsMultiLine() const;
	virtual bool IsEnabled() const;
	virtual bool IsReadOnly() const;
	virtual CStdString GetName() const;
	virtual void SetEidtWndNull();
	virtual DWORD GetBkColor() const;

	HWND GetNativeHWND();

protected:
    CEditWnd* m_pWindow;

    UINT m_uMaxChar;
    bool m_bReadOnly;
    bool m_bPasswordMode;
	bool m_bMultiLine;
#if(WINVER >= 0x0400)
	bool m_bDigitalNumber;
#endif
    TCHAR m_cPasswordChar;
    UINT m_uButtonState;
    CStdString m_sNormalImage;
    CStdString m_sHotImage;
    CStdString m_sFocusedImage;
    CStdString m_sDisabledImage;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CScrollBarUI : public CControlUI
{
public:
    CScrollBarUI(bool bHorizontal = false);

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    CContainerUI* GetOwner() const;
    void SetOwner(CContainerUI* pOwner);

    void SetEnabled(bool bEnable = true);
	void SetFocus();

    bool IsHorizontal();
    void SetHorizontal(bool bHorizontal = true);
    int GetScrollRange() const;
    void SetScrollRange(int nRange);
    int GetScrollPos() const;
    void SetScrollPos(int nPos);
    int GetLineSize() const;
    void SetLineSize(int nSize);

    bool GetShowButton1();
    void SetShowButton1(bool bShow);
    LPCTSTR GetButton1NormalImage();
    void SetButton1NormalImage(LPCTSTR pStrImage);
    LPCTSTR GetButton1HotImage();
    void SetButton1HotImage(LPCTSTR pStrImage);
    LPCTSTR GetButton1PushedImage();
    void SetButton1PushedImage(LPCTSTR pStrImage);
    LPCTSTR GetButton1DisabledImage();
    void SetButton1DisabledImage(LPCTSTR pStrImage);

    bool GetShowButton2();
    void SetShowButton2(bool bShow);
    LPCTSTR GetButton2NormalImage();
    void SetButton2NormalImage(LPCTSTR pStrImage);
    LPCTSTR GetButton2HotImage();
    void SetButton2HotImage(LPCTSTR pStrImage);
    LPCTSTR GetButton2PushedImage();
    void SetButton2PushedImage(LPCTSTR pStrImage);
    LPCTSTR GetButton2DisabledImage();
    void SetButton2DisabledImage(LPCTSTR pStrImage);

    LPCTSTR GetThumbNormalImage();
    void SetThumbNormalImage(LPCTSTR pStrImage);
    LPCTSTR GetThumbHotImage();
    void SetThumbHotImage(LPCTSTR pStrImage);
    LPCTSTR GetThumbPushedImage();
    void SetThumbPushedImage(LPCTSTR pStrImage);
    LPCTSTR GetThumbDisabledImage();
    void SetThumbDisabledImage(LPCTSTR pStrImage);

    LPCTSTR GetRailNormalImage();
    void SetRailNormalImage(LPCTSTR pStrImage);
    LPCTSTR GetRailHotImage();
    void SetRailHotImage(LPCTSTR pStrImage);
    LPCTSTR GetRailPushedImage();
    void SetRailPushedImage(LPCTSTR pStrImage);
    LPCTSTR GetRailDisabledImage();
    void SetRailDisabledImage(LPCTSTR pStrImage);

    LPCTSTR GetBkNormalImage();
    void SetBkNormalImage(LPCTSTR pStrImage);
    LPCTSTR GetBkHotImage();
    void SetBkHotImage(LPCTSTR pStrImage);
    LPCTSTR GetBkPushedImage();
    void SetBkPushedImage(LPCTSTR pStrImage);
    LPCTSTR GetBkDisabledImage();
    void SetBkDisabledImage(LPCTSTR pStrImage);

    void SetPos(RECT rc);
    void DoEvent(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void DoPaint(void* ctx, const RECT& rcPaint);

    void PaintBk(void* ctx);
    void PaintButton1(void* ctx);
    void PaintButton2(void* ctx);
    void PaintThumb(void* ctx);
    void PaintRail(void* ctx);

protected:

    enum { 
        DEFAULT_SCROLLBAR_SIZE = 16,
        DEFAULT_TIMERID = 10,
    };

    bool m_bHorizontal;
    int m_nRange;
    int m_nScrollPos;
    int m_nLineSize;
    CContainerUI* m_pOwner;
    POINT ptLastMouse;
    int m_nLastScrollPos;
    int m_nLastScrollOffset;
    int m_nScrollRepeatDelay;

    CStdString m_sBkNormalImage;
    CStdString m_sBkHotImage;
    CStdString m_sBkPushedImage;
    CStdString m_sBkDisabledImage;

	bool m_bShowButton1;
    RECT m_rcButton1;
    UINT m_uButton1State;
    CStdString m_sButton1NormalImage;
    CStdString m_sButton1HotImage;
    CStdString m_sButton1PushedImage;
    CStdString m_sButton1DisabledImage;

	bool m_bShowButton2;
    RECT m_rcButton2;
    UINT m_uButton2State;
    CStdString m_sButton2NormalImage;
    CStdString m_sButton2HotImage;
    CStdString m_sButton2PushedImage;
    CStdString m_sButton2DisabledImage;

    RECT m_rcThumb;
    UINT m_uThumbState;
    CStdString m_sThumbNormalImage;
    CStdString m_sThumbHotImage;
    CStdString m_sThumbPushedImage;
    CStdString m_sThumbDisabledImage;

    CStdString m_sRailNormalImage;
    CStdString m_sRailHotImage;
    CStdString m_sRailPushedImage;
    CStdString m_sRailDisabledImage;

    CStdString m_sImageModify;
};

} // namespace DuiLib

#endif // __UICOMMONCONTROLS_H__

