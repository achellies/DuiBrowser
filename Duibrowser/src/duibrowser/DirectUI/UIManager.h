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

#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#ifdef _MSC_VER
#pragma once
#endif

#ifdef UI_BUILD_FOR_SKIA
struct SkWinWindow;
#endif

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class CControlUI;


/////////////////////////////////////////////////////////////////////////////////////
//

typedef enum EVENTTYPE_UI
{
    UIEVENT__FIRST = 1,
    UIEVENT__KEYBEGIN,
    UIEVENT_KEYDOWN,
    UIEVENT_KEYUP,
    UIEVENT_CHAR,
    UIEVENT_SYSKEY,
    UIEVENT__KEYEND,
    UIEVENT__MOUSEBEGIN,
    UIEVENT_MOUSEMOVE,
    UIEVENT_MOUSELEAVE,
    UIEVENT_MOUSEENTER,
    UIEVENT_MOUSEHOVER,
    UIEVENT_BUTTONDOWN,
    UIEVENT_BUTTONUP,
    UIEVENT_RBUTTONDOWN,
    UIEVENT_DBLCLICK,
    UIEVENT_CONTEXTMENU,
    UIEVENT_SCROLLWHEEL,
    UIEVENT__MOUSEEND,
    UIEVENT_KILLFOCUS,
    UIEVENT_SETFOCUS,
    UIEVENT_WINDOWSIZE,
    UIEVENT_SETCURSOR,
    UIEVENT_TIMER,
    UIEVENT_NOTIFY,
    UIEVENT_COMMAND,
    UIEVENT__LAST,
};

/////////////////////////////////////////////////////////////////////////////////////
//

// Flags for CControlUI::GetControlFlags()
#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004

// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_ME_FIRST      0x80000000

// Flags for the CDialogLayout stretching
#define UISTRETCH_NEWGROUP   0x00000001
#define UISTRETCH_NEWLINE    0x00000002
#define UISTRETCH_MOVE_X     0x00000004
#define UISTRETCH_MOVE_Y     0x00000008
#define UISTRETCH_SIZE_X     0x00000010
#define UISTRETCH_SIZE_Y     0x00000020

// Flags used for controlling the paint
#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040



/////////////////////////////////////////////////////////////////////////////////////
//

typedef struct tagTFontInfo
{
    HFONT hFont;
    CStdString sFontName;
    int iSize;
    bool bBold;
    bool bUnderline;
    bool bItalic;
    TEXTMETRIC tm;
} TFontInfo;

typedef struct tagTImageInfo
{
    HBITMAP hBitmap;
    int nX;
    int nY;
    bool alphaChannel;
} TImageInfo;

// Structure for notifications from the system
// to the control implementation.
typedef struct tagTEventUI
{
    int Type;
    CControlUI* pSender;
    DWORD dwTimestamp;
    POINT ptMouse;
    TCHAR chKey;
    WORD wKeyState;
    WPARAM wParam;
    LPARAM lParam;
} TEventUI;

// Structure for notifications to the outside world
typedef struct tagTNotifyUI 
{
    CStdString sType;
    CControlUI* pSender;
    DWORD dwTimestamp;
    POINT ptMouse;
    WPARAM wParam;
    LPARAM lParam;
} TNotifyUI;

// Structure for relative position to the parent
typedef struct tagTRelativePosUI
{
	bool bRelative;
	SIZE szParent;
	int nMoveXPercent;
	int nMoveYPercent;
	int nZoomXPercent;
	int nZoomYPercent;
}TRelativePosUI;

// Listener interface
class INotifyUI
{
public:
    virtual void Notify(TNotifyUI& msg) = 0;
};

// MessageFilter interface
class IMessageFilterUI
{
public:
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) = 0;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CPaintManagerUI
{
public:
    CPaintManagerUI();
    ~CPaintManagerUI();

public:
    void Init(HWND hWnd);
    void NeedUpdate();
    void Invalidate(RECT& rcItem);

    HDC GetPaintDC() const;
    HWND GetPaintWindow() const;
    HWND GetTooltipWindow() const;

    POINT GetMousePos() const;
    SIZE GetClientSize() const;
    SIZE GetInitSize();
    void SetInitSize(int cx, int cy);
    RECT& GetSizeBox();
    void SetSizeBox(RECT& rcSizeBox);
    RECT& GetCaptionRect();
    void SetCaptionRect(RECT& rcCaption);
    SIZE GetRoundCorner() const;
    void SetRoundCorner(int cx, int cy);
	SIZE GetMinInfo() const;
    void SetMinInfo(int cx, int cy);
    SIZE GetMaxInfo() const;
    void SetMaxInfo(int cx, int cy);
	void SetBackgroundTransparent(bool bTrans);
	bool IsBackgroundTransparent() const;
	bool IsShowUpdateRect() const;
    void SetShowUpdateRect(bool show);

	bool IsOpacity() const;
	void SetOpacity(bool bOpacity);
	BYTE GetTransparency() const;
	void SetTransparency(BYTE transparency);

    static HINSTANCE GetInstance();
	static CStdString GetInstancePath();
	HINSTANCE GetResourceDll();    
	CStdString GetResourcePath();
	const CStdString& GetResourceZip();
	static void SetInstance(HINSTANCE hInst);	   
    void SetResourceDll(HINSTANCE hInst);
	void SetResourcePath(LPCTSTR pStrPath); 
    void SetResourceZip(LPCTSTR pStrZip);
	void UnzipResource();

    bool UseParentResource(CPaintManagerUI* pm);
    CPaintManagerUI* GetParentResource() const;

    DWORD GetDefaultDisabledColor() const;
    void SetDefaultDisabledColor(DWORD dwColor);
    DWORD GetDefaultFontColor() const;
    void SetDefaultFontColor(DWORD dwColor);
    DWORD GetDefaultLinkFontColor() const;
    void SetDefaultLinkFontColor(DWORD dwColor);
    DWORD GetDefaultLinkHoverFontColor() const;
    void SetDefaultLinkHoverFontColor(DWORD dwColor);
    DWORD GetDefaultSelectedBkColor() const;
    void SetDefaultSelectedBkColor(DWORD dwColor);
    TFontInfo* GetDefaultFontInfo();
    void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    DWORD GetCustomFontCount() const;
    HFONT AddFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    HFONT AddFontAt(int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    HFONT GetFont(int index);
    HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    bool FindFont(HFONT hFont);
    bool FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    int GetFontIndex(HFONT hFont);
    int GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    bool RemoveFont(HFONT hFont);
    bool RemoveFontAt(int index);
#ifdef UI_BUILD_FOR_DESIGNER
	void RemoveAllFonts();
#else
    static void RemoveAllFonts();
#endif
    TFontInfo* GetFontInfo(int index);
    TFontInfo* GetFontInfo(HFONT hFont);

	const TImageInfo* GetImage(LPCTSTR bitmap);
	const TImageInfo* GetImageEx(LPCTSTR bitmap, CControlUI* pControl, LPCTSTR type = NULL, DWORD mask = 0);
	const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha);	
	const TImageInfo* AddImage(LPCTSTR bitmap, CControlUI* pControl, LPCTSTR type = NULL, DWORD mask = 0);
	const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
	bool RemoveImage(LPCTSTR bitmap);
	void RemoveAllImages();

    void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList);
    LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
    bool RemoveDefaultAttributeList(LPCTSTR pStrControlName);
	const CStdStringPtrMap& GetDefaultAttribultes() const;
#ifdef UI_BUILD_FOR_DESIGNER
	void RemoveAllDefaultAttributeList();
#else
    static void RemoveAllDefaultAttributeList();
#endif

    bool AttachDialog(CControlUI* pControl);
    bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);
    void ReapObjects(CControlUI* pControl);

    bool AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
    void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    void RemoveAllOptionGroups();

    CControlUI* GetFocus() const;
    void SetFocus(CControlUI* pControl);
    void SetFocusNeeded(CControlUI* pControl);

    bool SetNextTabControl(bool bForward = true);

    bool SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
    bool KillTimer(CControlUI* pControl, UINT nTimerID);
    void KillTimer(CControlUI* pControl);
    void RemoveAllTimers();

    void SetCapture();
    void ReleaseCapture();
    bool IsCaptured();

    bool AddNotifier(INotifyUI* pControl);
    bool RemoveNotifier(INotifyUI* pControl);   
    void SendNotify(TNotifyUI& Msg, bool bAsync = false);
    void SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);

    bool AddPreMessageFilter(IMessageFilterUI* pFilter);
    bool RemovePreMessageFilter(IMessageFilterUI* pFilter);

    bool AddMessageFilter(IMessageFilterUI* pFilter);
    bool RemoveMessageFilter(IMessageFilterUI* pFilter);

    int GetPostPaintCount() const;
    bool AddPostPaint(CControlUI* pControl);
    bool RemovePostPaint(CControlUI* pControl);
    bool SetPostPaintIndex(CControlUI* pControl, int iIndex);

    void AddDelayedCleanup(CControlUI* pControl);

    CControlUI* GetRoot() const;
    CControlUI* FindControl(POINT pt) const;
	CControlUI* FindControl(CControlUI* pParent, POINT pt) const;
    CControlUI* FindControl(LPCTSTR pstrName);
	CControlUI* FindControl(CControlUI* pParent, LPCTSTR pstrName);

#ifdef UI_BUILD_FOR_DESIGNER
	CControlUI* FindPitchUponControl();
	int FindPitchUponControlCount();
	bool FinAllPitchUponControl(CStdPtrArray* pControls);
#endif

    static void MessageLoop();
    static bool TranslateMessage(const LPMSG pMsg);
	static void ShutDown();

    bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
    bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);

#ifdef UI_BUILD_FOR_DESIGNER
	bool IsUsedAsUIDesigner();
	void SetUsedAsUIDesigner(bool bUIDesigner = false);

	void RemoveAllPitchUpon(bool bOnlyRemovePitchUponTwice = false);

	void MoveAllPitchUponControls(LONG xOffset, LONG yOffset);
	
	bool IsAssertWhenCustomDefinedNull() const;
	void SetAssertWhenCustomDefinedNull(bool bAsert = true);
#endif

#ifdef UI_BUILD_FOR_SKIA
	SkWinWindow* GetSkiaContext() const;
	void SetSkiaContext(SkWinWindow* ctx);
#endif

private:
    static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromUpdate(CControlUI* pThis, LPVOID pData);
	static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);

#ifdef UI_BUILD_FOR_DESIGNER
	// 查找被选中的控件
	static CControlUI* CALLBACK __FindSinglePitchUponControl(CControlUI* pThis, LPVOID pData);

	// 查找被选中控件的数目
	static CControlUI* CALLBACK __FindCountOfPitchUponControl(CControlUI* pThis, LPVOID pData);

	// 查找被选中控件的列表
	static CControlUI* CALLBACK __FindAllPitchUponControl(CControlUI* pThis, LPVOID pData);
#endif

private:
    HWND m_hWndPaint;
    HDC m_hDcPaint;
#ifdef UI_BUILD_FOR_WINGDI
    HDC m_hDcOffscreen;
	HDC m_hDcBackground;
	HBITMAP m_hbmpOffscreen;
	HBITMAP m_hbmpBackground;
#endif
    HWND m_hwndTooltip;
    TOOLINFO m_ToolTip;
    bool m_bShowUpdateRect;
    //
    CControlUI* m_pRoot;
    CControlUI* m_pFocus;
    CControlUI* m_pEventHover;
    CControlUI* m_pEventClick;
    CControlUI* m_pEventKey;
    //
    POINT m_ptLastMousePos;
    SIZE m_szMinWindow;
    SIZE m_szMaxWindow;
    SIZE m_szInitWindowSize;
    RECT m_rcSizeBox;
    SIZE m_szRoundCorner;
    RECT m_rcCaption;
    UINT m_uTimerID;
    bool m_bFirstLayout;
    bool m_bUpdateNeeded;
    bool m_bFocusNeeded;
    bool m_bOffscreenPaint;
    bool m_bAlphaBackground;
    bool m_bMouseTracking;
    bool m_bMouseCapture;
#ifdef UI_BUILD_FOR_DESIGNER
	bool m_bUsedAsUIDesigner;
	bool m_bAssertWhenCustomDefinedNull;
#endif
	// 是否不透明
	bool m_bOpacity;
	BYTE m_transparency;
    //
    CStdPtrArray m_aNotifiers;
    CStdPtrArray m_aTimers;
    CStdPtrArray m_aPreMessageFilters;
    CStdPtrArray m_aMessageFilters;
    CStdPtrArray m_aPostPaintControls;
    CStdPtrArray m_aDelayedCleanup;
    CStdPtrArray m_aAsyncNotify;
    CStdStringPtrMap m_mNameHash;
	CStdStringPtrMap m_mOptionGroup;

	CStdString	m_pStrResourcePath;
    HINSTANCE	m_hResourceInstance;
    CStdString	m_pStrResourceZip;
    //
    CPaintManagerUI* m_pParentResourcePM;

#ifdef UI_BUILD_FOR_SKIA
	SkWinWindow*	fWinWindow;
#endif

#ifdef UI_BUILD_FOR_DESIGNER
    DWORD m_dwDefalutDisabledColor;
    DWORD m_dwDefalutFontColor;
    DWORD m_dwDefalutLinkFontColor;
    DWORD m_dwDefalutLinkHoverFontColor;
    DWORD m_dwDefalutSelectedBkColor;
    TFontInfo m_DefaultFontInfo;
    CStdPtrArray m_aCustomFonts;

    CStdStringPtrMap m_mImageHash;
    CStdStringPtrMap m_DefaultAttrHash;
#else
    static DWORD m_dwDefalutDisabledColor;
    static DWORD m_dwDefalutFontColor;
    static DWORD m_dwDefalutLinkFontColor;
    static DWORD m_dwDefalutLinkHoverFontColor;
    static DWORD m_dwDefalutSelectedBkColor;
    static TFontInfo m_DefaultFontInfo;
    static CStdPtrArray m_aCustomFonts;

    CStdStringPtrMap m_mImageHash;
    static CStdStringPtrMap m_DefaultAttrHash;
#endif
    //
    static HINSTANCE	m_hInstance;	
    static CStdPtrArray m_aPreMessages;
};

} // namespace DuiLib

#endif // __UIMANAGER_H__
