/*
Copyright (C) 2008-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
// BCPopupMenuEA.h
// By Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#ifndef PopupMenu_h
#define PopupMenu_h


#include <wtf/RefCounted.h>
#include "IntRect.h"
#include "PopupMenuClient.h"
#include "ScrollBar.h"
#include "BALBase.h"
#include <wtf/PassRefPtr.h>
#include <wtf/HashMap.h>
#include <wtf/Vector.h>
#include "RenderBlock.h"
#include <EAWebKit/EAWebKitInput.h>
#include "BCPlatformScrollBarEA.h"

namespace WebCore 
{
    class RenderText;
}


namespace WKAL {


class FrameView;
class PlatformScrollbar;
class PopupMenu; 

// Client class for scrollbar changes notification
class ScrollClient : public ScrollbarClient
{
public:    
    ScrollClient();
    ScrollClient(PopupMenu* p);
    void valueChanged(Scrollbar*);

    // Used to obtain a window clip rect.
    IntRect windowClipRect() const;

    bool isActive() const; 

private:
    PopupMenu   *m_pPopupMenu;
};


class PopupMenu : public RefCounted<PopupMenu>, private ScrollbarClient, public EA::WebKit::ModalInputClient
{
public:
    friend class ScrollClient;

    static PassRefPtr<PopupMenu> create(PopupMenuClient* client) 
        { return new PopupMenu(client); }

    PopupMenu(PopupMenuClient* client);
   ~PopupMenu();
    
    void show(const IntRect&, FrameView*, int index);
    void hide();

    void disconnectClient()                  { m_popupClient = 0; }
    void updateFromElement();
    void setStyle(RenderStyle* newStyle);

    PopupMenuClient* client() const          { return m_popupClient;  }
    BalMenu*         getBalMenu()            { return m_popupSurface; }    
    const IntRect&   windowRect() const      { return m_windowRect; }
    virtual IntRect  windowClipRect() const  { return m_windowRect; }
    virtual bool     isActive() const        { return true; }
    int              focusedIndex() const    { return m_focusedIndex; }

    static bool itemWritingDirectionIsNatural() { return false; }

    // ModalInputClient
    void ModalBegin();
    void ModalEnd();
    void OnKeyboardEvent(const EA::WebKit::KeyboardEvent& keyboardEvent);
    void OnMouseMoveEvent(const EA::WebKit::MouseMoveEvent& mouseMoveEvent);
    void OnMouseButtonEvent(const EA::WebKit::MouseButtonEvent& mouseButtonEvent);
    void OnMouseWheelEvent(const EA::WebKit::MouseWheelEvent& mouseWheelEvent);
	bool OnButtonEvent(const EA::WebKit::ButtonEvent& buttonEvent);
    void OnFocusChangeEvent(bool bHasFocus);
    void OnScrollViewEvent();

    void scrollUpdateOffset(int& newOffset); 

protected:

    // Manage the RenderText pointers in the droplist.
    void    createDropListText();
    void    releaseDropListText();
    void    populateDropListText();

    void    calculatePositionAndSize(const IntRect&, FrameView*);
    void    redrawItems(int index, int count);
    bool    setFocusedIndex(int i, bool hotTracking = false);
    void    updateFocusedIndex(int newFocusedIndex);
    IntRect clientRect() const;
    int     visibleItems() const;
    bool    scrollToRevealSelection();
    void    valueChanged(Scrollbar* /*scrollBar*/) { }

private:
    typedef WTF::Vector<RenderText*> RenderTextArray;

    EA::WebKit::View*       m_View;
    FrameView*              m_frameView;
    PopupMenuClient*        m_popupClient;
    IntPoint                m_menuPosition;
    EA::Raster::ISurface*    m_popupSurface;
    RenderTextArray         m_droplistText;
    bool                    m_wasClicked;
    IntRect                 m_windowRect;
    int                     m_itemHeight;
    int                     m_scrollOffset;
    int                     m_wheelDelta;
    int                     m_itemCount;
    int                     m_focusedIndex;
    bool                    m_scrollbarCapturingMouse;
    int                     m_scrollItemCount;  // Number of items visible in the scroll bar
protected:
    int                     m_scrollItemOffset; // Current offset to top of the visible items 
    bool                    m_scrollBarActive;
    IntRect                 m_scrollRect;
    IntRect                 m_mouseScrollRect; // Same as scroll Rect but with some extra x padding
private:
    ScrollClient            m_scrollClient;
    RefPtr<PlatformScrollbar> m_scrollVBar;   
    RefPtr<Scrollbar>       m_vBar;
    IntSize                 m_viewScrollOffset;         // Save the original scroll offset to detect changes
};

}  // namespace WKAL

#endif
