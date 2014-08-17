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
// BCTextPopupEA.h
// By Chris Sidhall - 2010
///////////////////////////////////////////////////////////////////////////////

// Description: popup surface that can display a text string (on a single line currently) as for tooltips for example.

#ifndef BCTextPopupEA_h
#define BCTextPopupEA_h

#include "PlatformString.h"
#include "IntPoint.h"
#include "Timer.h"
#include <IntRect.h>
#include "GraphicsContext.h"
#include "BAL/WKAL/Concretizations/Fonts/EA/BCFontPlatformDataEA.h"

namespace WKAL {

// This is so we can pass more params down to the popup.  Eventually we might want to pass font size, font type, background color.
struct TextPopupData 
{
    WebCore::String*    mpName;     // Name of the tooltip to distplay
    WebCore::IntPoint*  mpLocation; // Screen location of the tooltip
    EA::WebKit::View*   mpView;     // View context
    double              mDelay;     // How long on screen to keep active, This reduces flicker.
    int                 mHandleID;  // ID for the inner tool tip node which can be used to detect a new tooltip string as some string are identical.

    TextPopupData();
    TextPopupData(WebCore::String* pName, WebCore::IntPoint*  pLocation, EA::WebKit::View* pView, int handleID);
};

class TextPopup
{
public:
    
    TextPopup();
    virtual ~TextPopup();

    virtual void show(const TextPopupData& data);
    virtual void hide(); 
    virtual void timerFired(WebCore::Timer<TextPopup>*);
    virtual void drawToSurface(WebCore::FontPlatformData* pData);


    void SetMoveDelta(int delta) { mMoveDeltaSq = (delta * delta); }
    void SetOffsets(int x, int y){ mXOffset =x; mYOffset=y; }
    void SetBackColor(WebCore::Color col) { mBackColor = col;}
    void SetTextColor(WebCore::Color col) { mTextColor = col;}
    void SetDrawBox(bool flag) { mDrawBox = flag; }
    void SetFontSize(float size) { mFontSize = size; }
private:    
    bool                 mVisible;     // Has been added to the draw list 
    bool                 mDrawBox;     // Draw box around Text 
    int                  mXOffset;     // Offset from left-right edge = border
    int                  mYOffset;     // Offset from top-down edge = border
    float                mFontSize;    // Default font size     
    WebCore::Color       mBackColor;   // Background color
    WebCore::Color       mTextColor;   // Text color
    EA::WebKit::View*    mpView;       // View context 
    EA::Raster::ISurface* mpSurface;    // Pointer to the popup surface (= texture)
    int                  mMoveDeltaSq; // This can be used to control when the tooltip should reposition. 
    WebCore::IntRect     mWindowRect;   // Popup window size and position
    OWBAL::String        mText;         // The full source text string.  This is used to find if already active.
    OWBAL::String        mDrawnText;    // The actual text string used to draw
    int                  mHandleID;     // Handle uses to track if location needs to change
    double               mLastTime;     // Last time the tooltip was triggered to draw   
    double               mDelayTime;    // Time to leave on screen after last active   
    WebCore::Timer<TextPopup> mPopupTimer; // Timer to trigger auto shut down of popup
};


}  // namespace WKAL

#endif //BCTextPopupEA_h
