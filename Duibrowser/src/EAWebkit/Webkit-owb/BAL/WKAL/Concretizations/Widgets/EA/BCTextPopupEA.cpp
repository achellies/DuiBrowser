/*
Copyright (C) 2010 Electronic Arts, Inc.  All rights reserved.

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
// BCTextPopupEA.cpp
// By Chris Sidhall
///////////////////////////////////////////////////////////////////////////////

// This is to support simple text popups windows like tooltips but could be used
// for other text messages too.

#include "config.h"
#include "BCTextPopupEA.h"
#include "SystemTime.h"
#include "WebCore/platform/graphics/FontDescription.h"
#include "WebCore/platform/graphics/font.h"
#include "BAL/WKAL/Concretizations/Fonts/WK/BCFontCacheWK.h"
#include "WebCore/page/settings.h"
#include "WebCore/dom/document.h"
#include <EAWebKit/internal/EAWebKitAssert.h>
#include <EAWebKit/EAWebKitView.h>

namespace WKAL {

const double kTooltipDelay = 0.75;    // Default setting for how long in seconds to keep the popup on screen (avoid flicker)

    TextPopupData::TextPopupData()
        : mpName(0)
        , mpLocation(0)
        , mpView(0)
        , mDelay(kTooltipDelay)
        , mHandleID(0)
    {
    }

    TextPopupData::TextPopupData(WebCore::String* pName, WebCore::IntPoint*  pLocation, EA::WebKit::View* pView, int handleID)
        : mpName(pName)
        , mpLocation(pLocation)
        , mpView(pView)
        , mDelay(kTooltipDelay)
        , mHandleID(handleID)
    {
    }

TextPopup::TextPopup()
    : mVisible(false)
    , mDrawBox(true)
    , mXOffset(3)
    , mYOffset(2)
    , mFontSize(11.0f)
    , mBackColor(0xff,0xff,0xec,0xff)
    , mTextColor(0x0,0x0,0x0,0xff)
    , mpView(0)
    , mpSurface(0)   
    , mMoveDeltaSq(1000 * 1000)         // Pretty much disabled by default
    , mWindowRect()
    , mText()
    , mDrawnText()
    , mLastTime(0)
    , mDelayTime(0)
    ,mPopupTimer(this, &TextPopup::timerFired)
{
}

TextPopup::~TextPopup()
{
    hide();
}
    
void TextPopup::timerFired(WebCore::Timer<TextPopup>* pPop)
{
    const double curTime = currentTime();
    double timeDelta = (curTime - mLastTime) - mDelayTime;
    if(timeDelta >= 0)
    {
        hide();
    }
    else
    {
        // Keep going longer. 
        // Add a timeout safety here in case of a popup getting stuck?
        mPopupTimer.startOneShot(timeDelta);
    }
}

void TextPopup::show(const TextPopupData& data) 
{
    ASSERT(data.mpView);
    ASSERT(data.mpName);
    ASSERT(data.mpLocation);
    if( (!data.mpView) ||
        (!data.mpName) ||
        (!data.mpLocation) )
        return;

    if(data.mpName->isEmpty()) 
    {
        hide();
        mText = *data.mpName;       
        return;
    }

    mDelayTime = data.mDelay;

    // Verify we already are handling it  
    if( (mVisible) &&
        (mpView == data.mpView) &&
        (mHandleID == data.mHandleID) &&
        (mText.length() == data.mpName->length()) &&
        (mText.contains(*data.mpName)) )
    {
        // Evaluate location delta to see if this could possibly a different string    
        int deltaX = mWindowRect.x() - data.mpLocation->x();
        int deltaY = mWindowRect.y() - data.mpLocation->y();
        int deltaSq = ((deltaX * deltaX) + (deltaY * deltaY));
        if(deltaSq < mMoveDeltaSq) 
        {
            mLastTime = currentTime();  // Update timer to retrigger if needed
            return;
        }
    }
        
    // At this point, the tootip needs to be refreshed so 
    // Shut down timer if active
    if (mPopupTimer.isActive())
        mPopupTimer.stop();
    
    // Remove so we can reset it for these overlay's probably need a move option.
    if(mVisible)   
    {
        ASSERT(mpView);
        ASSERT(mpSurface);
        mpView->RemoveOverlaySurface(mpSurface); 
        mVisible = false;
    }
        
    // Store     
    mText = *data.mpName;   
    mpView = data.mpView;
    mHandleID = data.mHandleID;

    // Find default font to use
    AtomicString stdfont;   
    WebCore::Document* pDoc = data.mpView->GetDocument();
    if(pDoc) 
    {
        WebCore::Settings* pSettings = pDoc->settings();  
        if(pSettings)
        {
            stdfont = pSettings->standardFontFamily();  
        }
    }
    if(stdfont.isNull())
         stdfont = "Arial";


    // Set up the font to get the width (and for the draw)
    WebCore::FontDescription fontDescription;
    fontDescription.setComputedSize(mFontSize);
    fontDescription.setSpecifiedSize(mFontSize);    
    WebCore::FontPlatformData* pData = FontCache::getCachedFontPlatformData(fontDescription, stdfont);
    WebCore::Font toolFont(*pData, true);
    const TextRun textRun(mText.characters(), mText.length());
    int height = toolFont.height() + mYOffset + mYOffset;
    int width = toolFont.width(textRun) + mXOffset + mXOffset;   
  
    // Find if the width goes beyond the screen
    int viewX=0;
    int viewY=0;
    data.mpView->GetSize(viewX, viewY);

    // Width handling
    int viewDeltaX = (data.mpLocation->x() + width) -  viewX; 
    if(viewDeltaX > 0)
    {
        const int kOverflowDeltaX =30; // Overflow delta X under which we will consider moving versus chopping
         
         // If the overflow is small we can try to move it in the x to the left first
        if((viewDeltaX < kOverflowDeltaX) &&
           (data.mpLocation->x() > viewDeltaX))
        {
           // Move the string to the left
            data.mpLocation->setX(data.mpLocation->x() -viewDeltaX); 
            mDrawnText = mText;
        }   
        else
        {
            // Chop off and hyphen the string.
             // Using a percentage is not totally correct but should yield a close enough result
             // We use some correction magic numbers to keep it to the right size
             const int kCharAdjust = 3 + 3; // (we need to inject 3 extra chars + some extra room)
             
             float percentage = 0.0f;
             if(width)
             {   
                percentage = (float) (width - viewDeltaX) / (float) width; 
                if((percentage > 1.0f) || (percentage < 0.0f) )
                {
                    percentage = 1.0f;  // Or 0.0f to not display might work too
                }
             }   
             int nChar =  (int) (mText.length() * percentage);
             
             nChar -=kCharAdjust;   
             if(nChar <=1 )
                 return;                // Refuse or consider moving to the left if less than 2 characters                
             
             String adjustedText(mText.characters(), nChar);
             const char* pHyphen = "...";                
             adjustedText +=pHyphen;
             mDrawnText = adjustedText;   
             
             // Get the new width
             const TextRun textRun2(mDrawnText.characters(), mDrawnText.length());
             width = toolFont.width(textRun2) + mXOffset + mXOffset;   
         }
     }   
     else
     {
        mDrawnText = mText;
     }   
        

     // Check the Y for overflow
     int viewDeltaY = (data.mpLocation->y() + height) -  viewY; 
     if(viewDeltaY > 0)
     {
         if(data.mpLocation->y() > viewDeltaY)
         {
            // Move the string to the left
             data.mpLocation->setY(data.mpLocation->y() -viewDeltaY); 
         }               
     }
      // Set the new location
     mWindowRect.setLocation(*data.mpLocation);
     mWindowRect.setHeight(height);    
     mWindowRect.setWidth(width);    

    // Create the surface    
    if(!mpSurface)
    {
        mpSurface = EA::WebKit::GetEARasterInstance()->CreateSurface(mWindowRect.width(), mWindowRect.height(), EA::Raster::kPixelFormatTypeARGB, EA::Raster::kSurfaceCategoryTooltip);
        if(!mpSurface)
        {
            ASSERT(mpSurface);
            return;
        }
    }
    else
    {
        bool resized = mpSurface->Resize(mWindowRect.width(), mWindowRect.height(), false); 
        if(!resized)
        {
            ASSERT(resized);        
            return;        
        }
    }

    // Draw the texture (not to the view yet)
    drawToSurface(pData);   

    // Add the texture to the draw list (no Z sort currently but last should win)
    if(mpSurface)
    {
        mpView->SetOverlaySurface(mpSurface, EA::Raster::Rect(mWindowRect));
        mVisible = true;
    }

    // Set up timer for auto shut down
    if(mVisible)    
    {
        mPopupTimer.startOneShot(data.mDelay);
        mLastTime = currentTime();
    }
}




void TextPopup::hide()
{
    // Shut down timer if active
    if (mPopupTimer.isActive())
        mPopupTimer.stop();

    // Remove from draw list
    if(mVisible)   
    {
        ASSERT(mpView);
        ASSERT(mpSurface);
        mpView->RemoveOverlaySurface(mpSurface); 
    }

    // Delete texture
    if(mpSurface)
    {
        EA::WebKit::GetEARasterInstance()->DestroySurface(mpSurface);
        mpSurface = NULL;
    } 

    mHandleID = 0;
    mLastTime = 0;
    mVisible = false;
  }


    // This draws to the pop up texture but does not yet draw to the main view.
void TextPopup::drawToSurface(WebCore::FontPlatformData* pData)
{
    GraphicsContext ctx(mpSurface);
    ctx.setFillColor(mTextColor);
    ctx.setStrokeColor(mTextColor);

    // Fill background    
    const EA::Raster::Rect rect(0, 0, mWindowRect.width(), mWindowRect.height());
    ctx.fillRect(rect, mBackColor);

    // Draw box around edge   
    if(mDrawBox)
    {
        int w = mWindowRect.width() - 1;
        if(w < 0)
            w =0;
        int h = mWindowRect.height() - 1;
        if(h < 0)
            h =0;
        IntPoint p1(0,0);
        IntPoint p2(w,0);
        IntPoint p3(0,h);
        IntPoint p4(w,h);
        ctx.drawLine(p1,p2);
        ctx.drawLine(p1,p3);
        ctx.drawLine(p2,p4);
        ctx.drawLine(p3,p4);
    }

    // Draw the text 
    WebCore::Font toolFont(*pData, true);  
    ctx.setFont(toolFont);   
    const TextRun textRun(mDrawnText.characters(), mDrawnText.length());
    ctx.drawText(textRun, IntPoint(mXOffset, (int) mFontSize + mYOffset ));   
}

} // namespace
