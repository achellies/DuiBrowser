/*
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
 * Copyright (C) 2008 Collabora Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#include "config.h"
#include "PluginView.h"

#include "Document.h"
#include "DocumentLoader.h"
#include "Element.h"
#include "FrameLoader.h"
#include "FrameLoadRequest.h"
#include "FrameTree.h"
#include "Frame.h"
#include "FrameView.h"
#include "GraphicsContext.h"
#include "Image.h"
#include "HTMLNames.h"
#include "HTMLPlugInElement.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"
#include "NotImplemented.h"
#include "Page.h"
#include "PlatformMouseEvent.h"
#include "PluginDebug.h"
#include "PluginPackage.h"
#include "RenderLayer.h"
#include "Settings.h"
#include "JSDOMBinding.h"
#include "ScriptController.h"
#include "npruntime_impl.h"
#include "runtime.h"
#include "runtime_root.h"
#include <kjs/JSLock.h>
#include <kjs/JSValue.h>
#include <EAWebKit/EAWebKit.h>      // +CS Added for movie plugin 
#include <EAWebKit/EAWebKitView.h>  // 
#include "WebView.h"

using KJS::ExecState;
using KJS::Interpreter;
using KJS::JSLock;
using KJS::JSObject;
using KJS::JSValue;
using KJS::UString;

using std::min;

using namespace WTF;

namespace WebCore {

using namespace HTMLNames;

void PluginView::updateWindow() const
{
}

void PluginView::setFocus()
{
}

void PluginView::show()
{
}

void PluginView::hide()
{
}

#ifdef _DEBUG  

// 6/8/10 Chris Sidhall - A simple red box with a white "x" to help identify missing plugins.  
// Not all missing pluggins will go here for they would need a mime type and go through the same pathway
// Might need to add SetClipRect handling bur rect is currently unsused
// 9/12/10 - Changed name and parms to match existing code (the compiled out win version)
void PluginView::paintMissingPluginIcon(GraphicsContext* context, const IntRect& rect)
{
    const IntPoint center(frameGeometry().x() + (frameGeometry().width()>>1), frameGeometry().y()+ (frameGeometry().height()>>1));
    const int kBoxOffset=15;    // Offset relative of box center
    
    const IntRect rt(center.x() - kBoxOffset, center.y() - kBoxOffset, kBoxOffset+kBoxOffset,kBoxOffset+kBoxOffset);

    // Save the orginal colors
    const Color savedFColor(context->fillColor());
    const Color savedSColor(context->strokeColor());

    // Draw the red box
    const Color color1(0xff,0,0);
    context->setFillColor(color1);
    context->setStrokeColor(color1);
    context->drawRect(rt);    


    // Draw the white "x"
    const Color color2(0xff,0xff,0xff);
    context->setStrokeColor(color2);   
    
    IntPoint p1( center.x() - kBoxOffset, center.y() - kBoxOffset);
    IntPoint p2( center.x() + kBoxOffset, center.y() + kBoxOffset);
    context->drawLine(p1,p2);    
    // Draw a second line to make it thicker
    p1.move(1,0);
    p2.move(0,-1);
    context->drawLine(p1,p2);        
    
    IntPoint p3( center.x() + kBoxOffset, center.y() - kBoxOffset);
    IntPoint p4( center.x() - kBoxOffset, center.y() + kBoxOffset);
    context->drawLine(p3,p4);    
    // Draw a second line to make it thicker
    p3.move(-1,0);
    p4.move(0,-1);
    context->drawLine(p3,p4);    

    // Restore the colors
    context->setStrokeColor(savedSColor);   
    context->setFillColor(savedFColor);   
}
#endif  

void PluginView::paint(GraphicsContext* context, const IntRect& rect)
{
#ifdef _DEBUG    
    // 5/28/01 CSidhall - Added plugging fail draw
    // 
    if(this->status() != PluginStatusLoadedSuccessfully)
        paintMissingPluginIcon(context, rect);
#endif
}

void PluginView::handleKeyboardEvent(KeyboardEvent* event)
{
}

void PluginView::handleMouseEvent(MouseEvent* event)
{
}

void PluginView::setParent(ScrollView* parent)
{
}

void PluginView::setNPWindowRect(const IntRect& rect)
{
}

void PluginView::attachToWindow()
{
}

void PluginView::detachFromWindow()
{
}

void PluginView::stop()
{
}

static const char* MozillaUserAgent = "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8.1) Gecko/20061010 Firefox/2.0";

const char* PluginView::userAgent()
{
    if (m_plugin->quirks().contains(PluginQuirkWantsMozillaUserAgent))
        return MozillaUserAgent;

    if (m_userAgent.isNull())
        m_userAgent = m_parentFrame->loader()->userAgent(m_url).utf8();

    return m_userAgent.data();
}

const char* PluginView::userAgentStatic()
{
    //FIXME - Lie and say we are Mozilla
    //    return MozillaUserAgent;
    return "";
}


NPError PluginView::handlePostReadFile(Vector<char>& buffer, uint32 len, const char* buf)
{
    return NPERR_NO_ERROR;
}

NPError PluginView::getValue(NPNVariable variable, void* value)
{
    return NPERR_NO_ERROR;
}

NPError PluginView::getValueStatic(NPNVariable variable, void* value)
{
    return NPERR_GENERIC_ERROR;
}

void PluginView::invalidateRect(NPRect* rect)
{
}

void PluginView::forceRedraw()
{
}

PluginView::~PluginView()
{
}

/*static bool
plug_removed_cb(GtkSocket *socket, gpointer)
{
    return true;
}*/

void PluginView::init()
{
}



//+ 10/15/10 CSidhall - Added movie plugin API to notify of a movie tag -
// The system sets up a timer which pulses info back to the API.
// It also creates its own surface which the app can use to draw to if it wants.
// If the app returns true, it will make the area dirty so that a redraw will be triggered and update the view with
// the changed texture.  

const double k30HzMovieDelay = 1.0/30.0; // 30HZ default delay interval to trigger the API callback

const char* kMovieSourceAttrib="src";       // Source movie attribute
const char* kMovieWidthAttrib="width";      // Width movie attribute
const char* kMovieHeightAttrib="height";    // Height movie attribute

MoviePluginView::MoviePluginView(Frame* parentFrame, const IntSize& s, PluginPackage* pPlug, Element* pElem, const KURL& kurl, const Vector<String>& paramNames, const Vector<String>& paramValues, const String& mimeType, bool loadManually) 
    : PluginView(parentFrame, s, pPlug, pElem, kurl, paramNames, paramValues, mimeType, loadManually)
    , m_movieTimer(this, &MoviePluginView::movieTimerFired) 
    , m_pMovieSurface(0)
    , m_movieWidth(0)
    , m_movieHeight(0)
    , m_triggerDelay(k30HzMovieDelay)  
    , m_scrURI()
    , m_isMovieUpdateActive(true) 
    , m_removeSurfaceFlag(false)
    , m_loopFlag(false)
    , m_pauseFlag(false)
    , m_autoPlayFlag(false)
    , m_preloadFlag(false)
{
    // Extract initial attributes (we can grow this with more values as needed)
    bool sourceSet = false;
    bool widthSet = false;
    bool heightSet = false;
    int count = paramNames.size();
   
    for (int i = 0; i < count; i++) {
        if ((!sourceSet) && (paramNames[i] == kMovieSourceAttrib)) {
            m_scrURI.append( paramValues[i].characters(),paramValues[i].length() );
            sourceSet = true;
        }
        else if ((!widthSet) && (paramNames[i] == kMovieWidthAttrib)) {
            m_movieWidth = paramValues[i].toInt();
            widthSet = true;
        }
        else if ((!heightSet) && (paramNames[i] == kMovieHeightAttrib)) {
            m_movieHeight = paramValues[i].toInt();
            heightSet = true;
        }
    }

    // We keep a copy of the movie on a special surface so that we redraw the view without needing to get the surface from the API
    createMovieSurface(m_movieWidth, m_movieHeight);
    
    // Start timer to trigger API
    startMovieTimer(m_triggerDelay);
}

MoviePluginView::~MoviePluginView()
{
    if(m_isMovieUpdateActive)
        notify(true);   // Notify with terminate flag 
    stopMovieTimer();        
    destroyMovieSurface();
}

void MoviePluginView::startMovieTimer(double delay)
{
    m_movieTimer.startOneShot(delay);
}
    
void MoviePluginView::stopMovieTimer()
{
    if (m_movieTimer.isActive())
        m_movieTimer.stop();
}

bool MoviePluginView::notify(bool terminate)
{
    bool responseFlag = false;

    EA::WebKit::View* const pView = EA::WebKit::GetView(GetPluginViewParentFrame());
    ASSERT(pView);
    EA::WebKit::ViewNotification* const pVN = EA::WebKit::GetViewNotification();
    ASSERT(pVN);
    if((!pView) ||(!pVN))    
        return false;

    // Get the windwo view rect with scroll offset
    EA::Raster::Rect windowRect(0,0,frameGeometry().width(), frameGeometry().height());
    WebCore::FrameView* const pFrameView = pView->GetFrameView();
    ASSERT(pFrameView);
    if(pFrameView)
    {
        WebCore::IntSize scrollOffset = pFrameView->scrollOffset();
        windowRect.x = frameGeometry().x() - scrollOffset.width();
        windowRect.y = frameGeometry().y() - scrollOffset.height();
    }

    // Call the notification
    EA::WebKit::MovieUpdateInfo info ={
        pView,
        m_scrURI.charactersWithNullTermination(),
        getMovieSurface(),
        getMovieRect(),
        windowRect,
        m_triggerDelay,
        terminate,  // Terminate flag
        m_removeSurfaceFlag,
        isMovieLooping(),
        isMoviePaused(),
        isMovieAutoPlay(),
        isMoviePreload()
    };

    responseFlag = pVN->MovieUpdate(info);   

    // Check if should remove the surface
    if(info.mRemoveMovieSurfaceFlag)  
    {
        if(getMovieSurface()) {
            destroyMovieSurface();
            m_removeSurfaceFlag = info.mRemoveMovieSurfaceFlag;
        }
    }
    else if(responseFlag) {
        // Trigger the paint by setting the dirty rect
        WebCore::FrameView* const pFV = pView->GetFrameView();
        ASSERT(pFV);   
        if(pFV) {
            WebCore::IntSize scrollOffset = pFV->scrollOffset();
            const WKAL::IntRect rect(frameGeometry().x() - scrollOffset.width(),frameGeometry().y() - scrollOffset.height(), frameGeometry().width(), frameGeometry().height());
            if(pView->GetWebView()) {
                pView->GetWebView()->addToDirtyRegion(rect);
                pFV->SetDirty(true);
            }
        }
    }

    // Transfer/save time trigger
    m_triggerDelay = info.mTriggerDelay;    

    return info.mTerminateCallbackFlag;
}


// Timer callback which in turns call the view notification  
void MoviePluginView::movieTimerFired(Timer<MoviePluginView>* )
{
    bool responseFlag = notify(false);

    // Check if should retrigger
    if(responseFlag) {
        m_isMovieUpdateActive = false;      
        stopMovieTimer();
    }
    else {
        startMovieTimer(m_triggerDelay);
    }
}

EA::Raster::ISurface* MoviePluginView::createMovieSurface(const int width, const int height)
{
    if((width <= 0) || (height <= 0) || (m_removeSurfaceFlag))
        return 0;
    EA::Raster::IEARaster* pRaster =EA::WebKit::GetEARasterInstance();
    EA::Raster::ISurface* pSurface = pRaster->CreateSurface(width, height, EA::Raster::kPixelFormatTypeARGB, EA::Raster::kSurfaceCategoryMovie);
    ASSERT(pSurface);
    if(pSurface) {
        setMovieSurface(pSurface);
        setMovieRect(0,0,width,height);
        pSurface->SetClipRect(&getMovieRect());
        
        // Fill the surface 
        EA::Raster::Color col(0x00,0x00,0x00);  // Black
        pRaster->FillRectColor(pSurface, &getMovieRect(), col);
    
    }

    return pSurface;
}

void MoviePluginView::destroyMovieSurface()
{
    if(getMovieSurface()) {
        EA::WebKit::GetEARasterInstance()->DestroySurface(getMovieSurface());
        setMovieSurface(0);
    }
}
   
void MoviePluginView::paint(GraphicsContext* pGraphicContext, const IntRect& viewRect)
{
    EA::WebKit::View* const pView = EA::WebKit::GetView(GetPluginViewParentFrame());
    ASSERT(pView);
    WebCore::FrameView* const pFrameView = pView->GetFrameView();
    ASSERT(pFrameView);
    if((!getMovieSurface()) || (!pView) || (!pFrameView)) 
        return;

    // Build the source and target rects with scroll offset
    EA::Raster::ISurface* pTargetSurface = pGraphicContext->platformContext();
    WebCore::IntSize scrollOffset = pFrameView->scrollOffset();
    EA::Raster::Rect targetRect(frameGeometry().x() - scrollOffset.width(),frameGeometry().y() - scrollOffset.height(), frameGeometry().width(), frameGeometry().height());
    EA::Raster::Rect clipRect(viewRect.x(),viewRect.y(),viewRect.height(),viewRect.width());
    targetRect.constrainRect(clipRect);

    // Draw to the view surface
    EA::WebKit::GetEARasterInstance()->Blit(getMovieSurface(), &getMovieRect(), pTargetSurface, &targetRect, NULL);
}
//-CS

} // namespace WebCore








