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
#ifndef __UICOMBO_H__
#define __UICOMBO_H__

#ifdef _MSC_VER
#pragma once
#endif
#include "UICommonControls.h"

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class CComboWnd;
class CEditWnd;

class UILIB_API CComboUI : public CContainerUI, public IEditUI, public IListOwnerUI
{
    friend CComboWnd;
public:
	typedef enum enumComboDropType
	{
		COMBODROP_SIMPLE = 1,
		COMBODROP_DOWN   = 2,
		COMBODROP_LIST   = 3
	};

	enum eDropBoxAlign
	{
		DROPBOXALIGN_LEFT	= 0x00000001,
		DROPBOXALIGN_RIGHT	= 0x00000002,
		DROPBOXALIGN_TOP	= 0x00000004,
		DROPBOXALIGN_BOTTOM	= 0x00000008,
	};

    CComboUI();

	~CComboUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    void DoInit();
    UINT GetControlFlags() const;

    void SetEnabled(bool bEnable = true);

    CStdString GetDropBoxAttributeList();
    void SetDropBoxAttributeList(LPCTSTR pstrList);
    SIZE GetDropBoxSize() const;
    void SetDropBoxSize(SIZE szDropBox);
	UINT GetDropType() const;
	void SetDropType(UINT uDropType);

    bool SetItemIndex(CControlUI* pControl, int iIndex);
    bool Add(CControlUI* pControl);
    bool AddAt(CControlUI* pControl, int iIndex);
    bool Remove(CControlUI* pControl);
    bool RemoveAt(int iIndex);
    void RemoveAll();

    bool ActivateDropWnd();

	UINT GetDropBoxAlign() const;
	void SetDropBoxAlign(UINT align);
	RECT GetTextPadding() const;
    void SetTextPadding(RECT rc);
    LPCTSTR GetNormalImage() const;
    void SetNormalImage(LPCTSTR pStrImage);
    LPCTSTR GetHotImage() const;
    void SetHotImage(LPCTSTR pStrImage);
    LPCTSTR GetPushedImage() const;
    void SetPushedImage(LPCTSTR pStrImage);
    LPCTSTR GetFocusedImage() const;
    void SetFocusedImage(LPCTSTR pStrImage);
    LPCTSTR GetDisabledImage() const;
    void SetDisabledImage(LPCTSTR pStrImage);
	LPCTSTR GetDropBtnImage() const;
	void SetDropBtnImage(LPCTSTR pStrImage);

    void SetItemFont(int index);
    void SetItemTextStyle(UINT uStyle);
	RECT GetItemTextPadding() const;
    void SetItemTextPadding(RECT rc);
	DWORD GetItemTextColor() const;
    void SetItemTextColor(DWORD dwTextColor);
	DWORD GetItemBkColor() const;
    void SetItemBkColor(DWORD dwBkColor);
	LPCTSTR GetItemImage() const;
    void SetItemImage(LPCTSTR pStrImage);
	DWORD GetSelectedItemTextColor() const;
    void SetSelectedItemTextColor(DWORD dwTextColor);
	DWORD GetSelectedItemBkColor() const;
    void SetSelectedItemBkColor(DWORD dwBkColor);
	LPCTSTR GetSelectedItemImage() const;
    void SetSelectedItemImage(LPCTSTR pStrImage);
	DWORD GetHotItemTextColor() const;
    void SetHotItemTextColor(DWORD dwTextColor);
	DWORD GetHotItemBkColor() const;
    void SetHotItemBkColor(DWORD dwBkColor);
	LPCTSTR GetHotItemImage() const;
    void SetHotItemImage(LPCTSTR pStrImage);
	DWORD GetDisabledItemTextColor() const;
    void SetDisabledItemTextColor(DWORD dwTextColor);
	DWORD GetDisabledItemBkColor() const;
    void SetDisabledItemBkColor(DWORD dwBkColor);
	LPCTSTR GetDisabledItemImage() const;
    void SetDisabledItemImage(LPCTSTR pStrImage);
	DWORD GetItemLineColor() const;
    void SetItemLineColor(DWORD dwLineColor);
    bool IsItemShowHtml();
    void SetItemShowHtml(bool bShowHtml = true);

    SIZE EstimateSize(SIZE szAvailable);

    void SetPos(RECT rc);
    void DoEvent(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    
    void DoPaint(void* ctx, const RECT& rcPaint);
    void PaintText(void* ctx);
    void PaintStatusImage(void* ctx);
	void PaintBorder(void* ctx);
	void PaintDropdownButton(void* ctx);

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
	virtual bool IsEnabled() const;
	virtual bool IsReadOnly() const;
	virtual CStdString GetName() const;
	virtual void SetEidtWndNull();
	virtual bool IsMultiLine() const;
#if(WINVER >= 0x0400)
	virtual bool IsDigitalNumber() const;
#endif
	virtual DWORD GetBkColor() const;

	//Parent IListOwnerUI
	virtual TListInfoUI* GetListInfo();
	virtual int GetCurSel() const;  
	virtual bool SelectItem(int iIndex, bool bSendNofitied = true);
	virtual bool Activate();
protected:
    CComboWnd* m_pWindow;
	CEditWnd* m_pEditWnd;

	UINT m_uDropBoxAlign;
	UINT m_uDropType;
    int m_iCurSel;
    RECT m_rcTextPadding;
    CStdString m_sDropBoxAttributes;
    SIZE m_szDropBox;
	RECT m_rcDropBtn;
    UINT m_uButtonState;

    CStdString m_sNormalImage;
    CStdString m_sHotImage;
    CStdString m_sPushedImage;
    CStdString m_sFocusedImage;
    CStdString m_sDisabledImage;
	CStdString m_sDropBtnImage;

    TListInfoUI m_ListInfo;
};

} // namespace DuiLib

#endif // __UICOMBO_H__
