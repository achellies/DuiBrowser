//
// frame.hpp
// ~~~~~~~~~
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

#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

namespace EA {
	namespace WebKit {
		class Allocator;
		class View;
		class IEAWebkit;

        struct KeyboardEvent;
        struct MouseButtonEvent;
        struct MouseMoveEvent;
        struct MouseWheelEvent;

		class ViewNotification;
		struct LoadInfo;
		struct ViewUpdateInfo;
		struct StringInfo;
		struct UriHistoryChangedInfo;

		struct Parameters;
		struct ViewParameters;
	}
}

namespace EA {
	namespace WebKit {
		class IFontServer;
		class IGlyphCache;
		struct IFontStyle;
	}
}

namespace EA {
	namespace Raster {
		class IEARaster;
		class Surface;		
	}
}

namespace EA {
	namespace Allocator {
		class StackAllocator;
	}
}

using namespace EA::WebKit;
using namespace EA::Raster;
using namespace EA::Allocator;

//class MyAllocator;
class WindowImplBase;
class MainFrame : public WindowImplBase, public ViewNotification
{
public:
	MainFrame();
	~MainFrame();

public:
	LPCTSTR GetWindowClassName() const;	

	virtual void OnFinalMessage(HWND hWnd);

	virtual void Init();

	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	virtual tString GetSkinFile();

	virtual CControlUI* CreateControl(LPCTSTR pstrClass, CPaintManagerUI* pManager);

	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:	
	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
	void OnSystemMenu(TNotifyUI& msg);

	void UpdateNavigatingButtonStatus();

	virtual bool LoadUpdate(LoadInfo&);
	virtual bool ViewUpdate(ViewUpdateInfo&);
	virtual bool UriHistoryChanged(UriHistoryChangedInfo&);

	std::string GetUserAgent();

	std::string UrlEncode(const std::string& szToEncode);
	std::string UrlDecode(const std::string& szToDecode);

	void InitEAWebkit();

private:
	HINSTANCE		webkit_dll_;	
	IEAWebkit*		webkit_;
	IFontServer*	font_server_;
	IGlyphCache*	glyph_cache_;
	IEARaster*		raster_;
	Surface*		surface_;
	View*			view_;
	IFontStyle*		font_style_;
	//MyAllocator*	allocator_;

	int				logo_image_index;
	bool			is_loading_;
	tString			navigating_url_;

	char			user_agent_[MAX_PATH];
	char			application_name_[MAX_PATH];
};

#endif // MAINFRAME_HPP