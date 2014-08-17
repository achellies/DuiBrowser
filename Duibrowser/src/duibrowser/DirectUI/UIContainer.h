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
#ifndef __UICONTAINER_H__
#define __UICONTAINER_H__

#ifdef _MSC_VER
#pragma once
#endif

// todo：布局应该有Dialog,Vertical,Horizontal,Stream,Tile,Grid,Tab,Proxy

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class IContainerUI
{
public:
    virtual CControlUI* GetItemAt(int iIndex) const = 0;
    virtual int GetItemIndex(CControlUI* pControl) const  = 0;
    virtual bool SetItemIndex(CControlUI* pControl, int iIndex)  = 0;
    virtual int GetCount() const = 0;
    virtual bool Add(CControlUI* pControl) = 0;
    virtual bool AddAt(CControlUI* pControl, int iIndex)  = 0;
    virtual bool Remove(CControlUI* pControl) = 0;
    virtual bool RemoveAt(int iIndex)  = 0;
    virtual void RemoveAll() = 0;
};


/////////////////////////////////////////////////////////////////////////////////////
//
class CScrollBarUI;

class UILIB_API CContainerUI : public CControlUI, public IContainerUI
{
public:
    CContainerUI();
    virtual ~CContainerUI();

public:
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    CControlUI* GetItemAt(int iIndex) const;
    int GetItemIndex(CControlUI* pControl) const;
    bool SetItemIndex(CControlUI* pControl, int iIndex);
    int GetCount() const;
    bool Add(CControlUI* pControl);
    bool AddAt(CControlUI* pControl, int iIndex);
    bool Remove(CControlUI* pControl);
    bool RemoveAt(int iIndex);
    void RemoveAll();

    void DoEvent(TEventUI& event);
    void SetVisible(bool bVisible = true);
    void SetInternVisible(bool bVisible = true);
    void SetMouseEnabled(bool bEnable = true);

    virtual RECT GetInset() const;
    virtual void SetInset(RECT rcInset); // 设置内边距，相当于设置客户区
    virtual int GetChildPadding() const;
    virtual void SetChildPadding(int iPadding);
    virtual bool IsAutoDestroy() const;
    virtual void SetAutoDestroy(bool bAuto);
    virtual bool IsDelayedDestroy() const;
    virtual void SetDelayedDestroy(bool bDelayed);
    virtual bool IsMouseChildEnabled() const;
    virtual void SetMouseChildEnabled(bool bEnable = true);

    virtual int FindSelectable(int iIndex, bool bForward = true) const;

    void SetPos(RECT rc);
    void DoPaint(void* ctx, const RECT& rcPaint);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
    CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

    virtual SIZE GetScrollPos() const;
    virtual SIZE GetScrollRange() const;
    virtual void SetScrollPos(SIZE szPos);
    virtual void LineUp();
    virtual void LineDown();
    virtual void PageUp();
    virtual void PageDown();
    virtual void HomeUp();
    virtual void EndDown();
    virtual void LineLeft();
    virtual void LineRight();
    virtual void PageLeft();
    virtual void PageRight();
    virtual void HomeLeft();
    virtual void EndRight();
    virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
    virtual CScrollBarUI* GetVerticalScrollBar() const;
    virtual CScrollBarUI* GetHorizontalScrollBar() const;

#ifdef UI_BUILD_FOR_DESIGNER
	virtual void SetPitchUpon(bool bPitchUpon = true);
	virtual void SetPitchUponContinousTwice(bool bPitchUponContinousTwice = true);
	virtual void Move(LONG xOffset, LONG yOffset);
#endif
protected:
    virtual void SetFloatPos(int iIndex);
    virtual void ProcessScrollBar(RECT rc, int cxRequired, int cyRequired);

protected:
    CStdPtrArray m_items;
    RECT m_rcInset;
    int m_iChildPadding;
    bool m_bAutoDestroy;
    bool m_bDelayedDestroy;
    bool m_bMouseChildEnabled;
    bool m_bScrollProcess; // 防止SetPos循环调用

    CScrollBarUI* m_pVerticalScrollBar;
    CScrollBarUI* m_pHorizontalScrollBar;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CVerticalLayoutUI : public CContainerUI
{
public:
    CVerticalLayoutUI();

    LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);	
    UINT GetControlFlags() const;

    void SetSepHeight(int iHeight);
    int GetSepHeight() const;
    void SetSepImmMode(bool bImmediately);
    bool IsSepImmMode() const;
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    void DoEvent(TEventUI& event);

    void SetPos(RECT rc);
    void DoPostPaint(HDC hDC, const RECT& rcPaint);

    RECT GetThumbRect(bool bUseNew = false) const;

protected:
    int m_iSepHeight;
    UINT m_uButtonState;
    POINT ptLastMouse;
    RECT m_rcNewPos;
    bool m_bImmMode;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CHorizontalLayoutUI : public CContainerUI
{
public:
    CHorizontalLayoutUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;
    
    void SetSepWidth(int iWidth);
	int GetSepWidth() const;
    void SetSepImmMode(bool bImmediately);
	bool IsSepImmMode() const;
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    void DoEvent(TEventUI& event);

    void SetPos(RECT rc);
    void DoPostPaint(void* ctx, const RECT& rcPaint);

    RECT GetThumbRect(bool bUseNew = false) const;

protected:
    int m_iSepWidth;
    UINT m_uButtonState;
    POINT ptLastMouse;
    RECT m_rcNewPos;
    bool m_bImmMode;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CTileLayoutUI : public CContainerUI
{
public:
    CTileLayoutUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetPos(RECT rc);

    int GetColumns() const;
    void SetColumns(int nCols);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
    int m_nColumns;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CDialogLayoutUI : public CContainerUI
{
public:
    CDialogLayoutUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetStretchMode(CControlUI* pControl, UINT uMode);

    void SetPos(RECT rc);
    SIZE EstimateSize(SIZE szAvailable);
    RECT RecalcArea();
protected:  
    typedef struct 
    {
        CControlUI* pControl;
        UINT uMode;
        RECT rcItem;
    } STRETCHMODE;

    CStdValArray m_aModes;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CTabLayoutUI : public CContainerUI
{
public:
    CTabLayoutUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    bool Add(CControlUI* pControl);
    bool AddAt(CControlUI* pControl, int iIndex);
    bool Remove(CControlUI* pControl);
    void RemoveAll();
    int GetCurSel() const;
    bool SelectItem(int iIndex);

    void SetPos(RECT rc);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
    int m_iCurSel;
};

} // namespace DuiLib

#endif // __UICONTAINER_H__
