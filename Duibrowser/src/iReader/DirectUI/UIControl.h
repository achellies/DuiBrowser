#ifndef __UICONTROL_H__
#define __UICONTROL_H__

#pragma once

namespace DuiLib {

/////////////////////////////////////////////////////////////////////////////////////
//

typedef CControlUI* (CALLBACK* FINDCONTROLPROC)(CControlUI*, LPVOID);

class UILIB_API CControlUI
{
public:
    CControlUI();
    virtual ~CControlUI();

public:
    virtual CStdString GetName() const;
    virtual void SetName(LPCTSTR pstrName);
    virtual LPCTSTR GetClass() const;
    virtual LPVOID GetInterface(LPCTSTR pstrName);
    virtual UINT GetControlFlags() const;

    virtual bool Activate();
    virtual CPaintManagerUI* GetManager() const;
    virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
    virtual CControlUI* GetParent() const;

    // 文本相关
    virtual CStdString GetText() const;
    virtual void SetText(LPCTSTR pstrText);	

    // 图形相关
    DWORD GetBkColor() const;
    void SetBkColor(DWORD dwBackColor);
    DWORD GetBkColor2() const;
    void SetBkColor2(DWORD dwBackColor);
    DWORD GetBkColor3() const;
    void SetBkColor3(DWORD dwBackColor);
    LPCTSTR GetBkImage();
	void SetBkImage(LPCTSTR pStrImage);
	DWORD GetBorderColor() const;
	void SetBorderColor(DWORD dwBorderColor);    
	DWORD GetFocusBorderColor() const;
	void SetFocusBorderColor(DWORD dwBorderColor);
	int GetBorderSize() const;
    void SetBorderSize(int nSize);
    SIZE GetBorderRound() const;
    void SetBorderRound(SIZE cxyRound);
    bool DrawImage(void* ctx, LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);

    // 位置相关
    virtual const RECT& GetPos() const;
    virtual void SetPos(RECT rc);
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual int GetX() const;
    virtual int GetY() const;
    virtual RECT GetPadding() const;
    virtual void SetPadding(RECT rcPadding); // 设置外边距，由上层窗口绘制
    virtual SIZE GetFixedXY() const;         // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
    virtual void SetFixedXY(SIZE szXY);      // 仅float为true时有效
    virtual int GetFixedWidth() const;       // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
    virtual void SetFixedWidth(int cx);      // 预设的参考值
    virtual int GetFixedHeight() const;      // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
    virtual void SetFixedHeight(int cy);     // 预设的参考值
    virtual int GetMinWidth() const;
    virtual void SetMinWidth(int cx);
    virtual int GetMaxWidth() const;
    virtual void SetMaxWidth(int cx);
    virtual int GetMinHeight() const;
    virtual void SetMinHeight(int cy);
    virtual int GetMaxHeight() const;
    virtual void SetMaxHeight(int cy);
	virtual void SetRelativePos(SIZE szMove,SIZE szZoom);
	virtual void SetRelativeParentSize(SIZE sz);
	virtual TRelativePosUI GetRelativePos() const;
	virtual bool IsRelativePos() const;

    // 鼠标提示
    virtual CStdString GetToolTip() const;
    virtual void SetToolTip(LPCTSTR pstrText);

    // 快捷键
    virtual TCHAR GetShortcut() const;
    virtual void SetShortcut(TCHAR ch);

    // 菜单
    virtual bool IsContextMenuUsed() const;
    virtual void SetContextMenuUsed(bool bMenuUsed);

    // 用户属性
    virtual const CStdString& GetUserData(); // 辅助函数，供用户使用
    virtual void SetUserData(LPCTSTR pstrText); // 辅助函数，供用户使用
    virtual UINT_PTR GetTag() const; // 辅助函数，供用户使用
    virtual void SetTag(UINT_PTR pTag); // 辅助函数，供用户使用

    // 一些重要的属性
	virtual bool IsImageFitallArea() const;
	virtual void SetImageFitAllArea(bool bFit);
    virtual bool IsVisible() const;
    virtual void SetVisible(bool bVisible = true);
    virtual void SetInternVisible(bool bVisible = true); // 仅供内部调用，有些UI拥有窗口句柄，需要重写此函数
    virtual bool IsEnabled() const;
    virtual void SetEnabled(bool bEnable = true);
    virtual bool IsMouseEnabled() const;
    virtual void SetMouseEnabled(bool bEnable = true);
    virtual bool IsFocused() const;
    virtual void SetFocus();
    virtual bool IsFloat() const;
    virtual void SetFloat(bool bFloat = true);

    virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

    void Invalidate();
    bool IsUpdateNeeded() const;
    void NeedUpdate();
    void NeedParentUpdate();

    virtual void Init();
    virtual void DoInit();

    virtual void Event(TEventUI& event);
    virtual void DoEvent(TEventUI& event);

    virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    CControlUI* ApplyAttributeList(LPCTSTR pstrList);

	virtual void SetStretchMode(LPCTSTR pstrStretchMode);
	virtual LPCTSTR GetStretchMode() const;

    virtual SIZE EstimateSize(SIZE szAvailable);
    virtual void DoPaint(void* ctx, const RECT& rcPaint);
    virtual void PaintBkColor(void* ctx);
    virtual void PaintBkImage(void* ctx);
    virtual void PaintStatusImage(void* ctx);
    virtual void PaintText(void* ctx);
    virtual void PaintBorder(void* ctx);

    virtual void DoPostPaint(void* ctx, const RECT& rcPaint);

#ifdef UI_BUILD_FOR_DESIGNER
	virtual bool IsPitchUpon() const;
	virtual void SetPitchUpon(bool bPitchUpon = true);

	virtual bool IsPitchUponContinousTwice() const;
	virtual void SetPitchUponContinousTwice(bool bPitchUponContinousTwice = true);

	virtual bool IsUsedForPitchUpon() const;
	virtual void SetUsedForPitchUpon(bool bUsedForPitchUpon = true);

	virtual void Move(LONG xOffset, LONG yOffset);

	virtual bool IsImageAbsolutePath() const;
	virtual void SetImageAbsolutePath(bool bUseAbsolutePath = false);
#endif

public:
    CEventSource OnInit;
    CEventSource OnDestroy;
    CEventSource OnSize;
    CEventSource OnEvent;
    CEventSource OnNotify;

protected:
    CPaintManagerUI* m_pManager;
    CControlUI* m_pParent;
    CStdString m_sName;
    bool m_bUpdateNeeded;	
	bool m_bMenuUsed;

	// 该控件的大小
    RECT m_rcItem;

	// 文字显示的左右边距
    RECT m_rcPadding;
    SIZE m_cXY;

	// 指定的固定宽高
    SIZE m_cxyFixed;

	// 指定的最小宽高
    SIZE m_cxyMin;

	// 指定的最大宽高
    SIZE m_cxyMax;

    bool m_bVisible;
    bool m_bInternVisible;
    bool m_bEnabled;
    bool m_bMouseEnabled;
    bool m_bFocused;
    bool m_bFloat;

	// 防止SetPos循环调用
    bool m_bSetPos;

	// 选择图片是否需要填充整个区域，false可以用来画checkbox & radio box，而且文字的区域是不包含图片的。
	bool m_bImageFitallArea;

#ifdef UI_BUILD_FOR_DESIGNER
	// 用于编辑界面时，该控件是否选中
	bool m_bPitchUpon;

	// 用于控件对齐时，对齐的基准
	bool m_bPitchUponContinousTwice;

	// 不参与控件的选中
	bool m_bUsedForPitchUpon;

	// 图片的路径是否采用绝对路径
	bool m_bUsedAbsolutePath;
#endif

	TRelativePosUI m_tRelativePos;

    CStdString m_sText;
    CStdString m_sToolTip;
    TCHAR m_chShortcut;
    CStdString m_sUserData;
    UINT_PTR m_pTag;

    DWORD m_dwBackColor;
    DWORD m_dwBackColor2;
    DWORD m_dwBackColor3;
    CStdString m_sBkImage;
	CStdString m_sStretchMode;
    DWORD m_dwBorderColor;
	DWORD m_dwFocusBorderColor;
    int m_nBorderSize;
    SIZE m_cxyBorderRound;
    RECT m_rcPaint;
};

} // namespace DuiLib

#endif // __UICONTROL_H__
