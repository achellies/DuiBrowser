//
// WebkitLayout.h
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

#ifndef WebKitLayout_H
#define WebKitLayout_H

using EA::WebKit::View;

namespace Hanvon
{
	namespace WebkitLayout
	{
		class IWebKitLayout
		{
		public:
			virtual ~IWebKitLayout()
			{}

			virtual void Load(LPCTSTR pURI, View* pView) = 0;

			virtual int BeginWebkitLayout(View* pView, RECT rcClient) = 0;

			virtual void EndWebkitLayout(View* pView) = 0;

			virtual int NavigatePage(int nIndex, View* pView) = 0;

			virtual void ClearCache(View* pView) = 0;

			// navigate pages related routines
			virtual int PageCount(View* pView) = 0;
			virtual int CurPage(View* pView) = 0;
			virtual int FirstPage(View* pView) = 0;
			virtual int LastPage(View* pView) = 0;
			virtual int NextPage(View* pView) = 0;
			virtual int PrevPage(View* pView) = 0;
			virtual int GetCurrOffset(View* pView) = 0;

			virtual void FontSizeChanged(View* pView) = 0;

			virtual int GetPageNumByOffset(int nOffset, View* pView) = 0;

			virtual int GetPageOffset(int nIndex, View* pView) = 0;

			virtual LPWSTR GetSelectedText(View* pView) = 0;

			virtual LPWSTR GetPageText(int nIndex, View* pView, bool bStoredIntoStringShare) = 0;

			virtual bool SearchText(LPCTSTR pSearchingText, View* pView, unsigned limit, unsigned& matches, unsigned** ppMatchedPages) = 0;

			virtual void ClearTextSelection(View* pView) = 0;
		};
	}
}

#endif // Header include guard
