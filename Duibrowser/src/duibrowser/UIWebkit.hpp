//
// UIWebkit.hpp
// ~~~~~~~~~~~~
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

#ifndef UIWEBKIT_HPP
#define UIWEBKIT_HPP


namespace EA {
	namespace WebKit {
		class View;
	}
}

namespace EA {
	namespace Raster {
		class IEARaster;
		class Surface;
		class Color;
	}
}

using namespace EA::WebKit;
using namespace EA::Raster;

namespace DuiLib {

class CControlUI;
class CWebkitUI : public CControlUI
{
public:
	CWebkitUI();
	~CWebkitUI();

	virtual UINT GetControlFlags() const;

	void SetEARasterAndView(IEARaster* raster, View* view);

    virtual void DoEvent(TEventUI& event);
	virtual void DoPaint(void* ctx, const RECT& rcPaint);
	virtual void SetPos(RECT rc);

	bool LayoutChanged(RECT rc);

protected:
	void RestoreSurfaceBuffer();

private:
	View*		view_;
	IEARaster*	raster_;
	LPBYTE		bitmap_bits_;
	BITMAPINFOHEADER bitmap_header_info_;
	bool		did_first_layout_;
    POINT       last_mouse_point_;
};

} // namespace DuiLib

#endif // UIWEBKIT_HPP