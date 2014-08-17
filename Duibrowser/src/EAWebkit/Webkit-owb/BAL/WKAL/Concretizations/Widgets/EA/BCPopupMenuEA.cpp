/*
 * Copyright (C) 2008 Pleyo.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Pleyo nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY PLEYO AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PLEYO OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2008-2009
*/

///////////////////////////////////////////////////////////////////////////////
// BCPopupMenuEA.cpp
// Modified by Paul Pedriana
///////////////////////////////////////////////////////////////////////////////
//+ 11/17/09 CSidhall - Added Scroll bar support


#include "config.h"
#include "PopupMenu.h"
#include "CString.h"
#include "FrameView.h"
#include "Font.h"
#include "FontDescription.h"
#include "FloatRect.h"
#include "FontSelector.h"
#include "PlatformScrollBar.h"
#include "PlatformScreen.h"
#include "PlatformString.h"
#include "RenderStyle.h"
#include "RenderText.h"
#include "RenderBlock.h"
#include "RenderLayer.h"
#include "ObserverService.h"
#include "GraphicsContext.h"
#include <EARaster/EARaster.h>
#include "RenderTheme.h"
#include <EAWebKit/internal/EAWebKitAssert.h>
#include <EAWebKit/EAWebKitView.h>
#include <stdio.h>
#include "PlatformMouseEvent.h"

using std::min;


namespace WKAL {


// Default Window animation duration in milliseconds
// const int defaultAnimationDuration = 200;
//
// Maximum height of a popup window
// const int maxPopupHeight = 320;
//
// const int popupWindowAlphaPercent = 95;
// const int optionSpacingMiddle = 1;

const int kPopupWindowBorderWidth = 1;
const int kPopupMenuBorderPadding = 2;
const int kPopUpScrollWidth = 10;      
const int kPopUpScrollOffset = 6; 
const int kTextYOffsetCorrection = 1;
const int kScrollMinItems = 3;  // Seems we need about 45 pixels for a decent scroll bar draw

PopupMenu::PopupMenu(PopupMenuClient* client)
    : RefCounted<PopupMenu>(0)
    , m_View(NULL)
    , m_frameView(NULL)
    , m_popupClient(client)
    , m_menuPosition()
    , m_popupSurface(0)
    , m_droplistText()
    , m_wasClicked(false)
    , m_windowRect()
    , m_itemHeight(0)
    , m_scrollOffset(0)
    , m_wheelDelta(0)
    , m_itemCount(0)
    , m_focusedIndex(0)
    , m_scrollbarCapturingMouse(false)
    , m_scrollItemCount(0)
    , m_scrollItemOffset(0)
    , m_scrollBarActive(false)
    , m_scrollRect()
    , m_mouseScrollRect()
    , m_scrollClient(this)
    , m_scrollVBar(0)
    , m_viewScrollOffset()
{
    m_itemCount = m_popupClient->listSize();

    m_focusedIndex = m_popupClient->selectedIndex();
    if(m_focusedIndex < 0)
        m_focusedIndex = 0;
}


PopupMenu::~PopupMenu()
{
    hide();

    // Remove the vBar widget from the view
    if(m_vBar)
    {
       if ((m_vBar->isWidget()) && (m_frameView)) 
       {
            m_frameView->removeChild(m_scrollVBar.get());
       }
        m_vBar->setClient(0);
    }
}

// The Windows version of this code implements a Windows HWND manually and modally tracks it.
// It doesn't work via any HTML/Javascript implementation. We implement our own modal popup
// list via a Raster surface we create ourselves.
void PopupMenu::show(const IntRect& r, FrameView* v, int indexIn)
{
    m_View = EA::WebKit::GetView(v);
    calculatePositionAndSize(r, v);

    if(!clientRect().isEmpty() && m_itemCount)
    {
//        EAW_TRACE_FORMATTED("PopupMenu::show (%d %d %d %d) (%d %d %d %d)\n", r.x(), r.y(), r.width(), r.height(), m_windowRect.x(), m_windowRect.y(), m_windowRect.width(), m_windowRect.height());

        createDropListText();
        populateDropListText();
        m_View->SetModalInput(this);

        m_popupSurface =   EA::WebKit::GetEARasterInstance()->CreateSurface(m_windowRect.width(), m_windowRect.height(), EA::Raster::kPixelFormatTypeARGB, EA::Raster::kSurfaceCategorySelectDropDown);

		// 10/05/2010 Gautam Narain : If we don't set the clip rect then sometimes the scrollbar draw causes memory corruption
		// and causes a crash on a popup hide() call when deleting the memory. 
		const EA::Raster::Rect clipRect(0, 0, m_windowRect.width(), m_windowRect.height());
		m_popupSurface->SetClipRect(&clipRect);

        if(m_popupSurface)
        {
            // We store a pointer to the view 'this' in the Surface.
            m_popupSurface->SetUserData(m_View);
            
			// Update the focused index otherwise the scroll bar system does not get the proper offset and m_scrollItemOffset
			// is stuck to 0.
			setFocusedIndex(indexIn);
			// Scroll offset correction
            int index;
            int count;
            if(m_scrollBarActive)
            {
                count =  m_scrollItemCount;
                index = m_scrollItemOffset;
            }
            else
            {
                count = m_itemCount;
                index = 0;
            }

            redrawItems(index, count);
            m_View->SetOverlaySurface(m_popupSurface, EA::Raster::Rect(m_windowRect));

			// Todo : Gautam Narain 08/05/2010 To investigate if we can make the popup surface a widget so that then the rendering layer will
			// work correctly in case of of scrollbar redrawing. For now if it is a popup we will just not do the redrawing of the scrollbar.			
			if(m_scrollVBar.get())
			{
				//m_scrollVBar.get()->setContainingWindow(m_popupSurface);
				m_scrollVBar.get()->SetIsFromPopup(true);
			}
        }
    }
}


// This is typically triggered by an escape key press or by clicking outside the 
// popup menu window, whereupon a call to m_popupClient->hidePopup() is made,
// which will take care of things and call this function.
void PopupMenu::hide()
{
    m_View->SetModalInput(NULL);
    releaseDropListText();

    if(m_popupSurface)
    {
        m_View->RemoveOverlaySurface(m_popupSurface);
        EA::WebKit::GetEARasterInstance()->DestroySurface(m_popupSurface);
        m_popupSurface = NULL;
    }
    
    m_scrollbarCapturingMouse = false;
    m_scrollBarActive = false;
}

// Question: What is hotTracking?
bool PopupMenu::setFocusedIndex(int newFocusedIndex, bool /*hotTracking*/)
{

    if(newFocusedIndex >= m_itemCount)
        newFocusedIndex = (m_itemCount - 1);

    if(newFocusedIndex < 0) // Don't use else if here so we can double check 
        newFocusedIndex = 0;

    if(!m_popupClient->itemIsEnabled(newFocusedIndex))
        return false;

    updateFocusedIndex(newFocusedIndex);

    // We don't do this here, but rather we'll call m_popupClient->valueChanged(index) when the user clicks or presses return.
    // m_popupClient->setTextFromItem(newFocusedIndex);

    return true;
}


// This is called if the currently selected item is programmatically changed from within the HTML/Javascript.
void PopupMenu::updateFromElement()
{
    const int newFocusedIndex = m_popupClient->selectedIndex();

    updateFocusedIndex(newFocusedIndex);
}


void PopupMenu::updateFocusedIndex(int newFocusedIndex)
{
    // Update 10/15/2010 - Note by Arpit Baldeva:
	// Disabled this check.
	// 1. This function is not called unnecessarily so there is no real need for this change.
	// 2. This bit me few weeks ago too when calling the function like updateFocusedIndex(m_focusedIndex++) as this check would inadvertently fail.
	// 3. If there is a scroll bar, and you select an item down the list -> close the list -> reopen, you would want to see the item being drawn
	// on screen. However, currently that check is done few lines below. We can probably take that code out here and do more adjustments but that
	// seems like more trouble. 
	
	// 11/01/10 - Update: Enabled the check again otherwise it results in random visual glitch. 
	// Solved the problem of bringing the last selected item in the visible area by checking if the scroll bar is active. If the scroll bar is active, 
	// force a draw at least one time. 

	bool forceDraw = false;
	if(m_scrollBarActive)
	{
		if(newFocusedIndex < m_scrollItemOffset)
		{
			forceDraw = true;
		}
		else if(newFocusedIndex >= (m_scrollItemOffset + m_scrollItemCount)) 
		{
			forceDraw = true;
		}
	}
		
	if(newFocusedIndex != m_focusedIndex || forceDraw) // If there is a change...
	{
        if( (newFocusedIndex < 0) || (newFocusedIndex >= m_itemCount) )
            return;
        
//        EAW_TRACE_FORMATTED("PopupMenu::updateFocusedIndex: old: %d new: %d\n", m_focusedIndex, newFocusedIndex);

        int oldIndex = m_focusedIndex;
        m_focusedIndex = newFocusedIndex;
        
        // Scroll Control
        bool fullRedraw = false;
        bool skipDrawForOldIndex = false;
        if(m_scrollBarActive)
        {
            
            // Check if we need to update the scroll
            if(newFocusedIndex < m_scrollItemOffset)
            {
                m_scrollItemOffset = newFocusedIndex;    
                fullRedraw = true;
            }
            else if(newFocusedIndex >= (m_scrollItemOffset + m_scrollItemCount)) 
            {
                m_scrollItemOffset = newFocusedIndex - m_scrollItemCount + 1;  
                fullRedraw = true;
            }

            // Adjust the scroll bar position for the new offset
            if((fullRedraw) && (m_vBar) )
                m_vBar->setValue(m_scrollItemOffset);

            // Now check if the old value is in range
            if( (oldIndex < m_scrollItemOffset) || 
                 (oldIndex >= (m_scrollItemOffset + m_scrollItemCount)) ) 
                skipDrawForOldIndex = true;
        }
        if(!fullRedraw)
        {    
            if(!skipDrawForOldIndex)         
                redrawItems(oldIndex,        1);    // Redraw the old item as unselected.
            redrawItems(newFocusedIndex, 1);        // Draw the new item as selected.
        }
        else
        {
            redrawItems(m_scrollItemOffset, m_scrollItemCount);        
        }

        if(m_popupSurface) 
            m_View->SetOverlaySurface(m_popupSurface, EA::Raster::Rect(m_windowRect));
    }
}


bool PopupMenu::scrollToRevealSelection()
{
    // We don't support scrolling at this time. Hopefully there aren't too many items in the menu.
    return true;
}


void PopupMenu::setStyle(RenderStyle* newStyle)
{
    for(size_t i = 0, iEnd = m_droplistText.size(); i < iEnd; ++i) 
    {
        if(m_droplistText[i])
            m_droplistText[i]->setStyle(newStyle);
    }
}


void PopupMenu::calculatePositionAndSize(const IntRect& r, FrameView* v)
{
    // The input rect r is a rect of the inline popup menu element. It consists of some text and a
    // dropdown arrow on the right. The popup menu we are implementing here is a menu of text 
    // strings that displays directly below this inline element.

    EAW_ASSERT(v);

    m_viewScrollOffset = v->scrollOffset();   // Save to determine scroll changes
    IntRect rScreenCoords(v->contentsToWindow(r.location()), r.size());
    rScreenCoords.setY(rScreenCoords.y() + r.height());
    
    m_itemHeight      = r.height();                  // We set the popup menu line height to be the same as the inline element height.
    m_itemCount       = m_popupClient->listSize();   // Currently, m_itemCount is already calculated in our constructor.

    int naturalHeight = m_itemHeight * m_itemCount;
    int popupWidth    = 0;

    for(int i = 0; i < m_itemCount; ++i)
    {
        String text = m_popupClient->itemText(i);

        if(!text.isEmpty())
        {
            WebCore::RenderStyle* pRenderStyle = m_popupClient->clientStyle();
            WebCore::Font         itemFont     = pRenderStyle->font();           // Make a copy of the Font.

            if(m_popupClient->itemIsLabel(i))  // Note by Paul Pedriana: I don't know what this is about. It appears to be something about making text bold.
            {
                WebCore::FontDescription d = itemFont.fontDescription();  // Make a copy of the FontDescription.

                d.setWeight(d.bolderWeight());
                itemFont = WebCore::Font(d, itemFont.letterSpacing(), itemFont.wordSpacing());
                itemFont.update(m_popupClient->fontSelector());
            }

            const TextRun textRun(text.characters(), text.length());
            popupWidth = max(popupWidth, itemFont.width(textRun));
        }
    }
 


    // Set up the width (this needs to be done before the scroll bar calc 
    rScreenCoords.setWidth(popupWidth + kPopUpScrollWidth);
    if(rScreenCoords.width() < r.width())
        rScreenCoords.setWidth(r.width());
 

    //+ Scroll bar support
    IntSize offset = v->scrollOffset();
    int screenYMax = v->y() + v->height() + offset.height();
    int deltaYMax = screenYMax - r.y();
    if(deltaYMax < naturalHeight)
    {
        // We can't fit all on screen so will need to scroll - Find new height
        int scrollCount = 1; 
        if(m_itemHeight)
        {
            scrollCount = deltaYMax / m_itemHeight; 
            if(scrollCount > kScrollMinItems)
                scrollCount--;  // One less or too close to the bottom
            else if(scrollCount <= 0)
                scrollCount = 1;    // A min of 1 element visible
        }
        naturalHeight = m_itemHeight * scrollCount;
        m_scrollItemCount = scrollCount;
        m_scrollItemOffset = 0; 
        if(!m_scrollVBar.get())
        {
            m_scrollVBar =  WebCore::PlatformScrollbar::create(0, WebCore::VerticalScrollbar, WebCore::SmallScrollbar);
            if(m_scrollVBar.get()) 
            {
                m_vBar = m_scrollVBar;            
                m_vBar->setClient(&m_scrollClient);        
                v->addChild(m_scrollVBar.get());
                m_frameView = v;

            }
            else
            {
                EAW_ASSERT(0);        
            }
        }

        if(m_scrollVBar.get())
        {
			m_scrollRect.setX(v->x() + rScreenCoords.width() - kPopUpScrollWidth - kPopUpScrollOffset);
            m_scrollRect.setY(v->y());
            m_scrollRect.setWidth(kPopUpScrollWidth);
            m_scrollRect.setHeight(naturalHeight);
            m_mouseScrollRect = m_scrollRect;
            m_mouseScrollRect.setWidth(kPopUpScrollWidth + kPopUpScrollOffset);
            m_scrollbarCapturingMouse = false;
            m_scrollVBar->setRect(m_scrollRect);
            m_scrollBarActive = true;
            m_scrollVBar->setEnabled(true);
            m_vBar->setSteps(1, min(1, m_scrollItemCount -1), naturalHeight);
            m_vBar->setProportion(m_scrollItemCount, m_itemCount);
            m_vBar->setEnabled(true); 			
        }        
    }
    else
    {
        m_scrollBarActive = false;
        m_scrollItemOffset = 0;
        if(m_scrollVBar.get())
            m_scrollVBar->setEnabled(false);
        if(m_vBar)
            m_vBar->setEnabled(false);  
    }


    // Adjusted natural height if needed by the scroll bar calculations
    rScreenCoords.setHeight(naturalHeight);
    m_windowRect = rScreenCoords;
}


void PopupMenu::createDropListText()
{
    m_droplistText.resize(m_itemCount);

    // Question: Do we really need to manually set these to NULL? Doesn't it do that already for us?
    // CSidhall: Yes we need to clear. The resize does not init the mem. Confirmed that it can return a non NULL value. 
    for(size_t i = 0; i < (size_t)m_itemCount; ++i)
        m_droplistText[i] = NULL;
}


void PopupMenu::releaseDropListText()
{
    for (size_t i = 0; i < (size_t)m_itemCount; ++i) 
    {
        if(m_droplistText[i])
            m_droplistText[i]->destroy();
        m_droplistText[i] = NULL;
    }
}


void PopupMenu::populateDropListText()
{
    const IntRect& referenceRect = m_popupClient->clientRenderBlock()->absoluteBoundingBoxRect();

    int index;
    int count;
    if(m_scrollBarActive)
    {
        count =  m_scrollItemCount;
        index = m_scrollItemOffset;
    }
    else
    {
        count = m_itemCount;
        index = 0;
    }

    for(int i = index; i < count; ++i) 
    {
        const WebCore::String text = m_popupClient->itemText(i);

        if(!text.isEmpty())     // 2/16/09 CSidhall - Added for some menu items can have no text and will crash if expecting text here.
        {    

            if (m_droplistText[i])
                m_droplistText[i]->destroy();

            WebCore::RenderArena* arena = m_popupClient->clientDocument()->renderArena();
            m_droplistText[i] = new (arena) WebCore::RenderText(m_popupClient->clientDocument(), text.impl());

            WebCore::RenderText* pRenderText = m_droplistText[i];

            pRenderText->setStyle(m_popupClient->clientStyle());
            pRenderText->setWidth(referenceRect.width());
            pRenderText->setHeight(m_itemHeight);
            int y = (i - index) * m_itemHeight;
            pRenderText->setPos(0, y);
        }
    }
}


void PopupMenu::redrawItems(int index, int count)
{
    if(m_popupSurface)  // Just to make sure
    {
        const EA::WebKit::Parameters& param = EA::WebKit::GetParameters();

        // Draw the text background
        EA::Raster::Color cSelectedBack(param.mColors[EA::WebKit::kColorActiveListBoxSelectionBack]);
        // 10/28/09 CSidhall - Added support for selected text draw    
        EA::Raster::Color cSelectedText(param.mColors[EA::WebKit::kColorActiveListBoxSelectionFore]);

        GraphicsContext ctx(m_popupSurface);

        RenderStyle* pRenderStyle = m_popupClient->clientStyle();
        const Font& font = pRenderStyle->font();
        ctx.setFont(font);
        const int fontAscent = font.ascent();

        // If scroll active, we adjust the items with an offset
        int offset = 0;
        if(m_scrollBarActive)
            offset = m_scrollItemOffset;

        EA::Raster::IEARaster* pRaster =EA::WebKit::GetEARasterInstance();    
        for(int i = index; i < index + count; ++i)
        {
            const bool bHighlighted(i == m_focusedIndex);
            const int y = (i - offset) * m_itemHeight;

            const EA::Raster::Rect rect(0, y, m_popupSurface->GetWidth(), m_itemHeight);

            EAW_ASSERT(((rect.x + rect.w) <= m_popupSurface->GetWidth()) && ((rect.y + rect.h) <= m_popupSurface->GetHeight()));
            if(m_scrollBarActive)
            {
                if( (i < m_scrollItemOffset) || (i > (m_scrollItemOffset + m_scrollItemCount)) ) 
                    continue;
            }

            if(bHighlighted)
            {
                pRaster->FillRectSolidColor(m_popupSurface, &rect, cSelectedBack);
                ctx.setFillColor(WebCore::Color(cSelectedText.rgb()));
            }
            else
            {
                EA::Raster::Color cNormalBack(m_popupClient->itemBackgroundColor(i));  // I copied this from the Windows port (PopupMenuWin.cpp)
                pRaster->FillRectSolidColor(m_popupSurface, &rect, cNormalBack);
                EA::Raster::Color cNormalText(m_popupClient->itemStyle(i)->color());
                ctx.setFillColor(WebCore::Color(cNormalText.rgb()));           
            }

            // Draw the text into this rect.
            // Text is drawn at its baseline, so we need to position the text accordingly.
            const String  text(m_popupClient->itemText(i));
            const TextRun textRun(text.characters(), text.length());

            ctx.drawText(textRun, IntPoint(rect.x + kPopupMenuBorderPadding, rect.y + fontAscent));
        }

        // Draw the outline for the entire box all the time. We could make this a little more efficient by doing just what we need.
        const int x2 = m_popupSurface->GetWidth()  - 1;
        const int y2 = m_popupSurface->GetHeight() - 1;

        //+ Draw scroll bar draw if active
        if( (m_scrollBarActive) && (m_vBar) )
            m_vBar->paint(&ctx, m_scrollRect);

        EA::Raster::Color cOutlineTL(0xffc0c0c0);   // We need an enumerated color Parameter for these.
        EA::Raster::Color cOutlineBR(0xff000000);

        pRaster->HLineColor(m_popupSurface, 0,  x2,  0, cOutlineTL);
        pRaster->VLineColor(m_popupSurface, 0,   0, y2, cOutlineTL);
        pRaster->HLineColor(m_popupSurface, 0 , x2, y2, cOutlineBR);
        pRaster->VLineColor(m_popupSurface, x2,  0, y2, cOutlineBR);      
            
        #ifdef EA_DEBUG
            if(m_popupSurface == NULL) // This is impossible; we use it for debugging whereby you move the instruction pointer to the next line..
            {
                static int counter = 0;
                char       filePath[256];

                sprintf(filePath, "D:\\temp\\Popup%d.ppm", counter % 8);  // Use 0, 1, ... 6, 7, 0, 1, ...
                pRaster->WritePPMFile(filePath, m_popupSurface, false);

                sprintf(filePath, "D:\\temp\\Popup%dAlpha.ppm", counter % 8);
                pRaster->WritePPMFile(filePath, m_popupSurface, true);

                ++counter;
            }
        #endif
    }
}


// This returns a rectangle in coordinate frame of the popup menu (i.e. left/top is 0/0).
IntRect PopupMenu::clientRect() const
{
    IntRect clientRect = m_windowRect;

    clientRect.inflate(-kPopupWindowBorderWidth);
    clientRect.setLocation(IntPoint(0, 0));

    return clientRect;
}


int PopupMenu::visibleItems() const
{
    return clientRect().height() / m_itemHeight;
}


void PopupMenu::ModalBegin()
{
    // We increment our reference on behalf of the View object, just in case some kind of async destruction occurs.
    ref();
}


void PopupMenu::ModalEnd()
{
    deref();
}


void PopupMenu::OnKeyboardEvent(const EA::WebKit::KeyboardEvent& keyboardEvent)
{
    using namespace EA::WebKit;

    if(keyboardEvent.mbDepressed)
    {
        if(keyboardEvent.mbChar)
        {
            // Test for letters that match the first letter of the menu item.
            const UChar c = keyboardEvent.mId;   
            const WebCore::String s(&c, 1);

             for(int i = 0; i < m_itemCount; ++i)
            {
                String text = m_popupClient->itemText(i);

                if(!text.isEmpty())
                {
                    if(text.startsWith(s, false))
                    {
                        setFocusedIndex(i);
                        break;
                    }
                }
            }
        
        
        }
        else
        {
            switch(keyboardEvent.mId)
            {
				case kCarriageReturn:
				case kNewLine:
                    m_popupClient->hidePopup();
                    m_popupClient->valueChanged(m_focusedIndex);
					m_popupClient->setTextFromItem(m_focusedIndex); //Note by Arpit Baldeva: This is needed otherwise the text is not updated. 
                    break;

                case kEscape:
                    m_popupClient->hidePopup();
                    break;

                case kArrowUp:
                    setFocusedIndex(m_focusedIndex - 1);  // The function will test for out-of-bounds.
                    break;

                case kArrowDown:
                    setFocusedIndex(m_focusedIndex + 1);  // The function will test for out-of-bounds.
                    break;

                case kHome:
                    setFocusedIndex(0);
                    break;

                case kEnd:
                    setFocusedIndex(m_itemCount - 1);     // The function will test for out-of-bounds.
                    break;
            }
        }
    }
}


void PopupMenu::OnMouseMoveEvent(const EA::WebKit::MouseMoveEvent& mouseMoveEvent)
{
    // Added scroll support
    if(m_scrollBarActive)
    {
        // Make sure mouse is not in the scroll bar
        int x = mouseMoveEvent.mX - m_windowRect.x(); 
        int y = mouseMoveEvent.mY - m_windowRect.y(); 
        if(m_mouseScrollRect.contains(x,y))
        {
            
            // We need to rebuild a mouse mouse event to send down to the scroll system
            IntSize offset = m_frameView->scrollOffset();
            IntPoint localPos(x - offset.width(),y - offset.height()); 
            IntPoint pos(mouseMoveEvent.mX,mouseMoveEvent.mY);
            
            WebCore::MouseEventType mousePressType;			
            if(m_scrollbarCapturingMouse)
                mousePressType = WKAL::MouseEventPressed;
            else
               mousePressType = WKAL::MouseEventReleased;
            
            PlatformMouseEvent evt(localPos,pos, WKAL::LeftButton, mousePressType, 1, 
            mouseMoveEvent.mbShift, mouseMoveEvent.mbControl, mouseMoveEvent.mbAlt,mouseMoveEvent.mbOS,0);                

            m_scrollVBar->handleMouseMoveEvent(evt);        
            return;
        }

        if(m_scrollbarCapturingMouse)
        {    
            // We need to send a fake mouse release message so that the scroll system can know not to track the thumb
            // We need to rebuild a mouse mouse event to send down to the scroll system
            IntSize offset = m_frameView->scrollOffset();
            IntPoint localPos(x - offset.width(),y - offset.height()); 
            IntPoint pos(mouseMoveEvent.mX,mouseMoveEvent.mY);

            PlatformMouseEvent evt(localPos,pos, WKAL::LeftButton, WKAL::MouseEventReleased, 1, 
            false, false, false,false,0);                
            m_scrollVBar->handleMouseReleaseEvent(evt);
        }
    }

    if(m_windowRect.contains(mouseMoveEvent.mX, mouseMoveEvent.mY))
    {
        int selectedIndex = (mouseMoveEvent.mY - m_windowRect.y()) / m_itemHeight;

        if(m_scrollBarActive)
        {
            if(selectedIndex < 0)
                selectedIndex = 0;
            else if(selectedIndex >= m_scrollItemCount)
                selectedIndex = (m_scrollItemCount -1);
            
            selectedIndex +=m_scrollItemOffset;
        }
        setFocusedIndex(selectedIndex);
    }
}


void PopupMenu::OnMouseButtonEvent(const EA::WebKit::MouseButtonEvent& mouseButtonEvent)
{
    if(mouseButtonEvent.mId == EA::WebKit::kMouseLeft)
    {

        // Added scroll support
        if(m_scrollBarActive)
        {
            // Update mouse press status - we track this for the mouse move event does not have a press
            m_scrollbarCapturingMouse = mouseButtonEvent.mbDepressed;

            // Make sure mouse is not in the scroll bar
           int x = mouseButtonEvent.mX - m_windowRect.x(); 
           int y = mouseButtonEvent.mY - m_windowRect.y(); 
            if(m_mouseScrollRect.contains(x,y))
            {
                
                // We need to rebuild a mouse mouse event to send down to the scroll system
                IntSize offset = m_frameView->scrollOffset();
                IntPoint localPos(x - offset.width(),y - offset.height()); 
                IntPoint pos(mouseButtonEvent.mX,mouseButtonEvent.mY);
            
                // Mouse pressType
                WebCore::MouseEventType mousePressType;
                if(mouseButtonEvent.mbDepressed)
                    mousePressType = WKAL::MouseEventPressed;
                else
                   mousePressType = WKAL::MouseEventReleased;

                PlatformMouseEvent evt(localPos,pos, WKAL::LeftButton, mousePressType, 1, 
                mouseButtonEvent.mbShift, mouseButtonEvent.mbControl, mouseButtonEvent.mbAlt,mouseButtonEvent.mbOS,0);                

                if(mouseButtonEvent.mbDepressed)
                    m_scrollVBar->handleMousePressEvent(evt);        
                else
                    m_scrollVBar->handleMouseReleaseEvent(evt);
                return;
            }
        }
        
        if(mouseButtonEvent.mbDepressed)
        {
            // We should only change the selected element if the user clicked inside the html select element. Otherwise, following code would
			// set the focus to a random element based on where the user clicks in the rest of the window.
			if(m_windowRect.contains(mouseButtonEvent.mX, mouseButtonEvent.mY))
		    {
				int selectedIndex = (mouseButtonEvent.mY - m_windowRect.y()) / m_itemHeight;

				if(m_scrollBarActive)
				{
					if(selectedIndex < 0)
						selectedIndex = 0;
					else if(selectedIndex >= m_scrollItemCount)
						selectedIndex = (m_scrollItemCount -1);

					selectedIndex +=m_scrollItemOffset;
				}


				setFocusedIndex(selectedIndex);


				m_popupClient->valueChanged(m_focusedIndex);
			    m_popupClient->setTextFromItem(m_focusedIndex); //Note by Arpit Baldeva: This is needed otherwise the text is not updated. 
		    }
			else 
			{
#if PLATFORM(PS3) || PLATFORM(XBOX)
				bool onConsole = true;
#elif PLATFORM(WIN_OS)
				bool onConsole = m_View->IsEmulatingConsoleOnPC();
#else
				#error "Unknown Platform"
#endif
				if(onConsole)
				{
					IntRect windowRectIncludingNode = m_windowRect;
					//Shift the rectangle to encompass the original node(That is where our cursor is as we dont move it).
					windowRectIncludingNode.move(0, -m_itemHeight);
					if(windowRectIncludingNode.contains(mouseButtonEvent.mX, mouseButtonEvent.mY))
					{
						m_popupClient->valueChanged(m_focusedIndex);
						m_popupClient->setTextFromItem(m_focusedIndex); //Note by Arpit Baldeva: This is needed otherwise the text is not updated. 
					}
				}
			}

			m_View->SetModalInput(NULL);
			m_popupClient->hidePopup();

        }
    }
}

// 2/15/10 CSidhall - Added for scroll change detection outside of normal mouse\keyboard events 
// so that we can shut down the popup. It keeps track of the original scroll value and exits if
// it detects any new delta.   
//
void PopupMenu::OnScrollViewEvent()
{
    if(m_View)
    {
        int x=0;
        int y=0;
        m_View->GetScrollOffset(x,y);    
        if((x != m_viewScrollOffset.width()) || (y != m_viewScrollOffset.height()))
        {    
            m_View->SetModalInput(NULL);
            m_popupClient->hidePopup();
        }
    }
}


void PopupMenu::OnMouseWheelEvent(const EA::WebKit::MouseWheelEvent& mouseWheelEvent)
{
	// 6/15/2010: Note by Gautam Narain - Scrolling on a popup box like list boxes will force us
	// to scroll down if we are scrolling on the left and right axis
	if(!mouseWheelEvent.mbShift)
	{
		int delta = static_cast<int>(mouseWheelEvent.mLineDelta);
/*
		if(abs(mouseWheelEvent.mLineDelta) < 1.4)
		{
			if(mouseWheelEvent.mLineDelta>0.0)
				delta = 1;
			else
				delta = -1;
		}
*/
		int selectedIndex = m_focusedIndex - delta;
		setFocusedIndex(selectedIndex);
	}
}

bool PopupMenu::OnButtonEvent(const EA::WebKit::ButtonEvent& buttonEvent)
{
	switch(buttonEvent.mID)
	{
		case EA::WebKit::kButton1://Up 
		{
			
			if(m_focusedIndex == 0)
				return true;//intentional to return true so that we indicate that the modal input client handled the input so that we don't end up jumping else where on the page.
			
			setFocusedIndex(m_focusedIndex-1);
			return true;
		}

		case EA::WebKit::kButton3://Down 
		{
			if(m_focusedIndex == m_itemCount-1)
				return true;//intentional to return true so that we indicate that the modal input client handled the input so that we don't end up jumping else where on the page.
			
			setFocusedIndex(m_focusedIndex+1);
			return true;
		}
		
		default:
		{
			//OnFocusChangeEvent(false);
			return false;
		}
	}

	return false;
}

void PopupMenu::OnFocusChangeEvent(bool bHasFocus)
{
    if(!bHasFocus)
	{
		m_View->SetModalInput(NULL);
		m_popupClient->hidePopup();
	}
	
}


// This is to udpate the draw with new offsets from the scrollbar callback
void PopupMenu::scrollUpdateOffset(int& newOffset)
{
    if(newOffset < 0)
        newOffset = 0;
    else if( newOffset > (m_itemCount - m_scrollItemCount) )
        newOffset = (m_itemCount - m_scrollItemCount); 

    if(newOffset != m_scrollItemOffset)
    {
        m_scrollItemOffset = newOffset;
        redrawItems(m_scrollItemOffset, m_scrollItemCount);    // Draw the new item as selected.

        if(m_popupSurface) // Just to make sure.
            m_View->SetOverlaySurface(m_popupSurface, EA::Raster::Rect(m_windowRect));
    }
}


// Client callback for the scroll bar 
// When the user holds the mouse button down, a fire timer is started and updates the scroll
ScrollClient::ScrollClient()
  :ScrollbarClient()
  ,m_pPopupMenu(0)
{
}    
     
 ScrollClient::ScrollClient(PopupMenu* p)
  :ScrollbarClient()
  ,m_pPopupMenu(p)
{
}         

void ScrollClient::valueChanged(Scrollbar* pVBar)
{
    if( (!m_pPopupMenu) || (!pVBar) || (!m_pPopupMenu->m_scrollBarActive) )
    {    
        EAW_ASSERT(0);     
        return;
    }
 
    int newOffset = pVBar->value();
    if(m_pPopupMenu->m_scrollItemOffset != newOffset)
    {
        m_pPopupMenu->scrollUpdateOffset(newOffset); 
        if(newOffset != pVBar->value())
        {
            // Should not be needed to correct here but just in case
            pVBar->setValue(newOffset);            
        }
    }
}

// Used to obtain a window clip rect.
IntRect ScrollClient::windowClipRect() const
{
    if(m_pPopupMenu)
        return m_pPopupMenu->m_scrollRect;

    // Should not get here
    EAW_ASSERT(m_pPopupMenu);
    const IntRect r;
    return r;
}

bool ScrollClient::isActive() const
{
    return false;

}


} // namespace
