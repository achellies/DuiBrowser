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
// BCGraphicsContextEA.cpp
// Modified by Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "GraphicsContext.h"

#include "AffineTransform.h"
#include "FloatRect.h"
#include "Font.h"
#include "ImageBuffer.h"
#include "IntRect.h"
#include "NotImplemented.h"
#include "Path.h"
#include "SimpleFontData.h"
#include <math.h>
#include <stdio.h>
#include <wtf/MathExtras.h>
#include <wtf/FastAllocBase.h>

#include "GraphicsContextPrivate.h"
#include "BCGraphicsContextPlatformPrivateEA.h"

#include <EARaster/EARaster.h>
#include <EARaster/EARasterColor.h>
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/internal/EAWebKitAssert.h>

#ifndef M_PI
    #define M_PI 3.14159265359
#endif

#if (defined(__GNUC__) && (__GNUC__ >= 3)) || defined(__MWERKS__) || defined(__xlC__)
#include <alloca.h>
#elif defined(_MSC_VER)
#include <malloc.h>
#elif (defined(__GNUC__) && (__GNUC__ < 3)) && !defined(alloca)
#define alloca __builtin_alloca
#endif


using std::min;
using std::max;


namespace WKAL {


GraphicsContext::GraphicsContext(PlatformGraphicsContext* cr)
    : m_common(createGraphicsContextPrivate())
    , m_data(new GraphicsContextPlatformPrivate)
    , clippingRect(0,0,0,0)
    , m_expose(0)
{
    m_data->surface = cr;
    setPaintingDisabled(!cr);
}

GraphicsContext::~GraphicsContext()
{
     destroyGraphicsContextPrivate(m_common);
     delete m_data;
}

AffineTransform GraphicsContext::getCTM() const
{
    return m_common->state.transform;
}

bool GraphicsContext::hasTransform() const
{
    return !m_common->state.transform.isIdentity(); 
}


EA::Raster::ISurface* GraphicsContext::transform(EA::Raster::ISurface *pSrc, EA::Raster::Rect &srcRect, EA::Raster::Rect &dstRect )
{
    WKAL::AffineTransform t = getCTM();
    
    // Convert to external matrix format
    EA::Raster::Matrix2D m(t.a(),t.b(),t.c(),t.d(),t.e(),t.f());
    
    // Build the transformed surface
    EA::Raster::IEARaster* pRaster = EA::WebKit::GetEARasterInstance();
    EA::Raster::ISurface *pTransformSurface = pRaster->TransformSurface(pSrc, srcRect, m);
    if(!pTransformSurface)
        return pTransformSurface;    
    

    // Build the source rect and rotate it to find the new rotated position
    IntRect rect = t.mapRect(srcRect);

    // Adjust the position of the dstRect in local space (remove origin)
    t.setE(0);
    t.setF(0);
    int x1 = dstRect.x - origin().width();
    int y1 = dstRect.y - origin().height();
    double x2, y2;
    t.map((double) x1, (double) y1, &x2, &y2);
    dstRect.x = rect.x() + origin().width() + (int) x2;
    dstRect.y = rect.y() + origin().height() + (int) y2;  

    // Reset to the new source surface
    srcRect.x =0; 
    srcRect.y =0; 
    srcRect.w = pTransformSurface->GetWidth(); 
    srcRect.h = pTransformSurface->GetHeight(); 

    return pTransformSurface;
}

PlatformGraphicsContext* GraphicsContext::platformContext() const
{
    return m_data->surface;
}

PlatformGraphicsContext* GraphicsContext::setPlatformContext(PlatformGraphicsContext* p)
{
    PlatformGraphicsContext* pPrev = m_data->surface;
    m_data->surface = p;
    return pPrev;
}

void GraphicsContext::savePlatformState()
{
    if (paintingDisabled())
        return;

    // 2/11/09 CSidhall - Save clip rect 
    m_common->state.setClipRectRestore(m_data->surface->GetClipRect());    
  
    m_common->stack.append(m_common->state);
}

void GraphicsContext::restorePlatformState()
{
    if (paintingDisabled())
        return;

    if (m_common->stack.isEmpty()) {
        DS_WAR("ERROR void BCGraphicsContext::restore() stack is empty");
        return;
    }

    // 2/11/09 CSidhall - Restore the clip rect 
    m_data->surface->SetClipRect(m_common->state.getClipRectRestore());

    m_common->state = m_common->stack.last();
    m_common->stack.removeLast();

    // CS - Removed this as it does not truly restore the previous clip
    // clip(clippingRect);

}


// Draws a filled rectangle with a stroked border.
void GraphicsContext::drawRect(const IntRect& rect)
{
    if (paintingDisabled())
        return;

    EA::Raster::ISurface* pSurface = m_data->surface;

    EA::Raster::Rect dstRect;
    dstRect.x = rect.x() + origin().width();
    dstRect.y = rect.y() + origin().height();
    dstRect.w = rect.width();
    dstRect.h = rect.height();

     if (fillColor().alpha())
    {
        if (!m_data->layers.isEmpty())
        {
            const EA::Raster::Color rectFillColor(fillColor().red(), fillColor().green(), fillColor().blue(), static_cast<int>(fillColor().alpha() * m_data->layers.last()));

            EA::WebKit::GetEARasterInstance()->FillRectColor(pSurface, &dstRect, rectFillColor);
        }
        else
        {
            const EA::Raster::Color rectFillColor(fillColor().rgb());
            EA::WebKit::GetEARasterInstance()->FillRectColor(pSurface, &dstRect, rectFillColor);
        }
    }

    if (strokeStyle() != NoStroke)
    {
        const WKAL::IntSize& o = origin();
        const int            x = o.width();
        const int            y = o.height();

        if (!m_data->layers.isEmpty())
        {
            EA::Raster::RectangleRGBA(pSurface,
                                      (rect.x() + x), 
                                      (rect.y() + y),
                                      (rect.x() + x  + rect.width() - 1), 
                                      (rect.y() + y + rect.height() - 1),
                                      strokeColor().red(), strokeColor().green(), strokeColor().blue(), static_cast<int>(strokeColor().alpha() * m_data->layers.last()));
        }
        else
        {
            EA::Raster::RectangleRGBA(pSurface,
                                      (rect.x() + x), 
                                      (rect.y() + y),
                                      (rect.x() + x  + rect.width() - 1), 
                                      (rect.y() + y + rect.height() - 1),
                                      strokeColor().red(), strokeColor().green(), strokeColor().blue(), strokeColor().alpha());
        }
    }
}


// This is only used to draw borders.  
void GraphicsContext::drawLine(const IntPoint& point1, const IntPoint& point2)
{
    if (paintingDisabled())
        return;

    StrokeStyle style = strokeStyle();  // Note: SolidStroke does not handle thickness.
    if (style == NoStroke)
        return;
    
    int thickness = static_cast<int> (strokeThickness());    
    if (thickness < 1)
        thickness = 1;

    IntPoint p1(point1 + origin());
    IntPoint p2(point2 + origin());
    Color    color = strokeColor();

    int alpha;

    if (!m_data->layers.isEmpty())
        alpha = static_cast<int>(strokeColor().alpha() * m_data->layers.last());
    else
        alpha = strokeColor().alpha();

    // Default (dotted)
    int width = thickness;  // The width of the dot or dash
    int space = thickness;  // The empty space in between 2 dots or dashes

    switch(style)
    {
        case DashedStroke:
        {
            const int kDashWidth = 3;   // These numbers are based on experimentation with other browsers.  About the same.
            const int kDaskSpace = 2;

            width = kDashWidth * thickness;  
            space = kDaskSpace * thickness;  
        }
         // Fall into DottedStroke here...

        case DottedStroke:
        {
            int x1 =  p1.x();
            int y1 =  p1.y();
            int x2 =  p2.x();
            int y2 =  p2.y();
            
            // Safety ordering 
            if(x1 > x2)
            {
                int temp =x2;
                x2 = x1;
                x1 = temp;
            }
            if(y1 > y2)
            {
                int temp =y2;
                y2 = y1;
                y1 = temp;
            }    

            // It appears that the passed line should not include the last point but in some cases, we do want it.
            // This box draw also assumes that the thickness will be added so gives the averaged center of the line instead
            // of just a box that contains the thick line.
            if(y1 == y2) // Since the lines are axis aligned for now, this should work.
            {
                x2 -= 1; 
            }
            else if(x1 == x2)
            {
                y2 -= 1;
            }

            // Build the delta
            int deltaX = x2 - x1;
            int deltaY = y2 - y1;
            int length = static_cast<int> (sqrt( static_cast<float> ((deltaX * deltaX) + (deltaY * deltaY)) ));
            if(length <= 0)
                return;         // Not a line

            // Normalize the dash/dot vector to the width       
            int dashX = (deltaX * (width-1))/ length; // The dot does not include the end point so width -1
            int dashY = (deltaY * (width-1))/ length;

            //Normalize the space vector to the space
            int spaceX = (deltaX * (space+1))/length; // This is the empty space delta from the dash/dot location ( so + 1.0f) 
            int spaceY = (deltaY * (space+1))/length;

            // Normalize it to the displacement         // The displacement is how much we need to move the center line to take into account the line thickness.         
            int displace = thickness >> 1;  // /2 to find how much should go over the center. A 1 pixel line would have 0 displacement.
            if(y1 != y2)
                displace = -displace; // Flip normal for vertical lines so that we start on the right side
            int displaceX = (deltaY * displace) / length;
            int displaceY = (-deltaX * displace) / length;

            // Preset the line color    
            Color    c(color.red(), color.green(), color.blue(), alpha);                

            // Starting coord    
            int curX1 = x1;
            int curY1 = y1;
            int curX2, curY2; 

            // Width and height of the rectangle
            int w,h;
            if(y1 != y2)
            {
                h = width;
                w = thickness;           
            }
            else
            {
                w = width;
                h = thickness;  
            }

            // Find out how many dots we need to draw
            int dotCount = ( (length + (width + space -1)) / (width + space) );  // We add a partial extra dot with (width + space -1)

            // Note: Seems that some other browsers might adjust their starting dot/dash width so that the end point does not 
            // end on a blank space, leaving a corner bare.  We could consider this.  The current system clips the end point instead 
            // if needed.

            EA::Raster::Rect rect;

            // Draw loop
            while(dotCount--)
            {
                // new end point
                curX2 = curX1 + dashX;
                curY2 = curY1 + dashY;
                
                // Overflow checks - Should only kick in for the last dot
                if(curX2 > x2)
                {
                    EAW_ASSERT(!dotCount);                   
                    curX2 = x2;
                    // Don't draw if too small in relation to the thickness or it looks like a pixel bug
                    w = (curX2 - curX1) ;                 
                    if((w  <= 2) && (width > 3)) 
                        break;
                }
                if(curY2 > y2)
                {
                    EAW_ASSERT(!dotCount);              
                    curY2 = y2;
                    // Don't draw if too small in relation to the thickness
                    h = (curY2 - curY1) + 1; // +1 because the starting point is included  
                    if( (h <= 2) && (width > 3) ) 
                        break;
                }
 
                rect.x = curX1 + displaceX;
                rect.y = curY1 + displaceY;
                rect.w = w;
                rect.h = h;
                EA::WebKit::GetEARasterInstance()->FillRectColor(m_data->surface, &rect, c);

           
                // new start point
                curX1 = curX2 + spaceX;
                curY1 = curY2 + spaceY;
            }
            break;

        }
        
        // Used by the scroll bar, the regular CSS border does not seem to use this one. 
        case SolidStroke:
        default:
        {
            int x2 = p2.x();
            int y2 = p2.y();

            if (p1.y() == p2.y())
            {
                x2 -=1;  // Correction to not include end point
            }
            else
            {
                // TODO: Verify if y2 should also be -1 here 
            }

            EA::WebKit::GetEARasterInstance()->LineRGBA(m_data->surface,
                                     p1.x(), p1.y(),
                                     x2, y2,
                                     color.red(),
                                     color.green(),
                                     color.blue(),
                                     alpha);
        }
  }    
}

// This method is only used to draw the little circles used in lists.
void GraphicsContext::drawEllipse(const IntRect& rect)
{
    if (paintingDisabled())
        return;

    float yRadius = .5f * rect.height();
    float xRadius = .5f * rect.width();

    // Mean that we will draw a circle
    if (xRadius == yRadius) {
        strokeArc(rect, 0, 360);
        return;
    }

    if (strokeStyle() != NoStroke) {
        unsigned width = static_cast<unsigned int>(strokeThickness());
        if (width == 0)
            width++;
    }

    Color color = strokeColor();

    int alpha;

    if (!m_data->layers.isEmpty())
        alpha = static_cast<int>(strokeColor().alpha() * m_data->layers.last());
    else
        alpha = strokeColor().alpha();

    EA::WebKit::GetEARasterInstance()->EllipseRGBA(m_data->surface, 
                            (int)(rect.x() + origin().width() + xRadius),
                            (int)(rect.y() + origin().height() + yRadius),
                            (int)(xRadius),
                            (int)(yRadius),
                            color.red(),
                            color.green(),
                            color.blue(),
                            alpha);
}


// TODO: draw points instead of lines for nicer circles
inline void drawArc(EA::Raster::ISurface* pSurface, const WebCore::Color color, int zone, int xc, int yc, float& x0, float& y0, float x1, float y1, bool doSwap = true)
{

    EA::Raster::IEARaster* pRaster =EA::WebKit::GetEARasterInstance();

    // Mean First draw => will not draw just a point.
    if (x0 != x1)
    {
        switch(zone)
        {
            case 0:
                pRaster->LineRGBA(pSurface,
                            static_cast<int>(xc + ceilf(x0)), static_cast<int>(yc - ceilf(y0)),
                            static_cast<int>(xc + ceilf(x1)), static_cast<int>(yc - ceilf(y1)),
                            color.red(),
                            color.green(),
                            color.blue(),
                            color.alpha());
                break;

            case 1:
                pRaster->LineRGBA(pSurface,
                            static_cast<int>(xc - ceilf(y0)), static_cast<int>(yc - ceilf(x0)),
                            static_cast<int>(xc - ceilf(y1)), static_cast<int>(yc - ceilf(x1)),
                            color.red(),
                            color.green(),
                            color.blue(),
                            color.alpha());
                break;

            case 2:
                pRaster->LineRGBA(pSurface,
                            static_cast<int>(xc - ceilf(x0)), static_cast<int>(yc + ceilf(y0)),
                            static_cast<int>(xc - ceilf(x1)), static_cast<int>(yc + ceilf(y1)),
                            color.red(),
                            color.green(),
                            color.blue(),
                            color.alpha());
                break;

            case 3:
                pRaster->LineRGBA(pSurface,
                            static_cast<int>(xc + ceilf(y0)), static_cast<int>(yc + ceilf(x0)),
                            static_cast<int>(xc + ceilf(y1)), static_cast<int>(yc + ceilf(x1)),
                            color.red(),
                            color.green(),
                            color.blue(),
                            color.alpha());
                break;
        }

        if (doSwap)
        {
            x0 = x1;
            y0 = y1;
        }
    }
}


void drawArc(EA::Raster::ISurface* pSurface, const IntRect rect, uint16_t startAngle, uint16_t angleSpan, const WebCore::Color color)
{
    //
    //        |y          (This diagram is supposed to be a circle).
    //        |
    //       ...
    // z=1 .  |  . z=0      We know that there are 360 degrees in a circle. First we
    //   .    |    .        see that a circle is symetrical about the x axis, so
    //  .     |     .       only the first 180 degrees need to be calculated. Next
    //--.-----+-----.--     we see that it's also symetrical about the y axis, so now
    //  .     |     . x     we only need to calculate the first 90 degrees.
    //   .    |    .        Each 90° region is associated to a zone (z value). Zone where
    // z=2 .  |  . z=3      we will draw is defined from startAngle and angleSpan.
    //       ...            Thus after coord computation, we draw in the zone(s).
    //        |             Note: 0 <= alpha0, alpha1 <= 90
    //        |

    int   r       = (rect.width() - 1) / 2;
    int   xc      = rect.x() + r;
    int   yc      = rect.y() + (rect.height() - 1)/ 2;
    int   z0      = startAngle / 90;
    int   z1      = (startAngle + angleSpan) / 90;
    int   alpha0  = startAngle % 90;
    float xalpha0 = r * cosf((alpha0 * M_PI / 180));
    int   alpha1  = (startAngle + angleSpan) % 90;
    float xalpha1 = r * cosf((alpha1 * M_PI / 180));

    float x0, y0, x1, y1;

    if (z0 == z1)
    {
        // Draw in the same zone from angle = z0 * 90 + alpha0 to angle = z0 * 90 + alpha1
        x0 = xalpha0;
        y0 = sqrt(pow((float)r, 2.f) - pow(x0, 2.f));

        for (x1 = xalpha0; x1 >= xalpha1; x1--)
        {
            y1 = sqrt(pow((float)r, 2.f) - pow(x1, 2.f));
            drawArc(pSurface, color, z0, xc, yc, x0, y0, x1, y1);
        }
    }
    else if ((z1 - z0) == 1)
    {
        // Begin to draw in zone Z and end in zone Z + 1
        if (alpha1 < alpha0)
        {
            // Draw from angle = z1 * 90 to angle = z1 * 90 + alpha1
            // Then from angle = z0 * 90 + alpha0 to angle = z1 * 90
            x0 = r;
            y0 = 0;

            for (x1 = r; x1 >= xalpha1; x1--)
            {
                y1 = sqrt(pow(r, 2.f) - pow(x1, 2.f));
                drawArc(pSurface, color, z1, xc, yc, x0, y0, x1, y1);
            }

            x0 = xalpha0;
            y0 = sqrt(pow((float)r, 2.f) - pow(x0, 2.f));

            for (x1 = xalpha0; x1 >= 0; x1--)
            {
                y1 = sqrt(pow((float)r, 2.f) - pow(x1, 2.f));
                drawArc(pSurface, color, z0, xc, yc, x0, y0, x1, y1);
            }
        }
        else
        {
            // Compute a complete quarter of circle.
            // Draw in zone1 from 0 to alpha0
            // Draw in zone0 and zone1 from alpha0 to alpha1
            // Draw in zone0 from alpha1 to 90
            x0 = r;
            y0 = 0;

            for (x1 = r; x1 >= 0; x1--)
            {
                y1 = sqrt(pow((float)r, 2.f) - pow(x1, 2.f));

                if (x1 < xalpha1)
                    drawArc(pSurface, color, z0, xc, yc, x0, y0, x1, y1);
                else if (x1 > xalpha0)
                    drawArc(pSurface, color, z1, xc, yc, x0, y0, x1, y1);
                else {
                    drawArc(pSurface, color, z0, xc, yc, x0, y0, x1, y1, false);
                    drawArc(pSurface, color, z1, xc, yc, x0, y0, x1, y1);
                }
            }
        }
    }
    else
    {
        // Draw at least a complete quarter of circle and probably many more
        x0 = r;
        y0 = 0;

        for (x1 = r; x1 >= 0; x1--)
        {
            y1 = sqrt(pow((float)r, 2.f) - pow(x1, 2.f));

            if ((z1 - z0) >= 3)
                drawArc(pSurface, color, z1 - 2, xc, yc, x0, y0, x1, y1, false);

            if (x1 < xalpha1)
                drawArc(pSurface, color, z1 % 3, xc, yc, x0, y0, x1, y1, false);

            if (x1 < xalpha0)
                drawArc(pSurface, color, z0, xc, yc, x0, y0, x1, y1, false);

            drawArc(pSurface, color, z1 - 1, xc, yc, x0, y0, x1, y1);
        }
    }
}



// FIXME: This function needs to be adjusted to match the functionality on the Mac side.
// Note by Paul Pedriana: I don't know what this Mac thing means.
void GraphicsContext::strokeArc(const IntRect& rect, int startAngle, int angleSpan)
{
    if (paintingDisabled())
        return;

    if (strokeStyle() == NoStroke)
        return;

    //draw as many arcs as needed for the thickness
    //this is a quick hack until we re-implement the drawArc in lower level.
    float   thick    = strokeThickness() / 2.0;
    IntRect rectWork = rect;
    int     a0       = (startAngle) / 90;
  //int     a1       = (startAngle + angleSpan) / 90;

    int alpha;

    if (!m_data->layers.isEmpty())
        alpha = static_cast<int>(strokeColor().alpha() * m_data->layers.last());
    else
        alpha = strokeColor().alpha();

    WebCore::Color color(strokeColor().red(), strokeColor().green(), strokeColor().blue(), alpha);

    switch (a0)
    {
        case 0:
            rectWork.setX(rect.x() + origin().width());

            for (int i = 0; i < (int)thick; i++)
            {
                rectWork.setY(rect.y() + origin().height() + i);
                rectWork.setWidth(rect.width() - i);
                rectWork.setHeight(rect.height() - i);
                drawArc(m_data->surface, rectWork, startAngle, angleSpan, color);
            }
            break;

        case 1:
            for (int i = 0; i < (int)thick; i++)
            {
                rectWork.setX(rect.x() + origin().width() + i);
                rectWork.setY(rect.y() + origin().height() + i);
                rectWork.setWidth(rect.width() - i*2);
                rectWork.setHeight(rect.height() - i*2);
                drawArc(m_data->surface, rectWork, startAngle, angleSpan, color);
            }
            break;

        case 2:
            rectWork.setY(rect.y() + origin().height());

            for (int i = 0; i < (int)thick; i++)
            {
                rectWork.setX(rect.x() + origin().width() + i);
                rectWork.setWidth(rect.width() - i);
                rectWork.setHeight(rect.height() - i);
                drawArc(m_data->surface, rectWork, startAngle, angleSpan, color);
            }
            break;

        case 3:
            for (int i = 0; i < (int)thick; i++)
            {
                rectWork.setX(rect.x() + origin().width() + i);
                rectWork.setY(rect.y() + origin().height() + i);
                rectWork.setWidth(rect.width() - i*2);
                rectWork.setHeight(rect.height() - i*2);
                drawArc(m_data->surface, rectWork, startAngle, angleSpan, color);
            }
            break;
    }
}


void GraphicsContext::drawConvexPolygon(size_t npoints, const FloatPoint* points, bool shouldAntialias)
{
    if (paintingDisabled())
        return;

    if (npoints <= 1)
        return;

	// We use stack space if possible.
	const size_t kMaxStackSize = 32;//Total 64 for 2 of the following
	int*       pMallocVX = NULL;
	int*       pUsedVX   = ((npoints * sizeof(int)) < kMaxStackSize) ? (int*)alloca(npoints * sizeof(int)) : (pMallocVX = (int*)fastMalloc(npoints * sizeof(int)));
	int*       pMallocVY = NULL;
	int*       pUsedVY   = ((npoints * sizeof(int)) < kMaxStackSize) ? (int*)alloca(npoints * sizeof(int)) : (pMallocVY = (int*)fastMalloc(npoints * sizeof(int)));

	
	int  x1 = 0, x2 = 0, y1 = 0, y2 = 0;

    for(size_t i=0; i < npoints; i++)
    {
        pUsedVX[i] = static_cast<int16_t>(points[i].x() + origin().width());
        pUsedVY[i] = static_cast<int16_t>(points[i].y() + origin().height());
        x1 = min(x1, pUsedVX[i]);
        y1 = min(y1, pUsedVY[i]);
        x2 = max(x2, pUsedVX[i]);
        y2 = max(y2, pUsedVY[i]);
    }

    int alpha;

    if (!m_data->layers.isEmpty())
        alpha = static_cast<int>(strokeColor().alpha() * m_data->layers.last());
    else
        alpha = strokeColor().alpha();

    Color color = fillColor();

    EA::WebKit::GetEARasterInstance()->FilledPolygonRGBA(m_data->surface, pUsedVX, pUsedVY, npoints,
                                   color.red(),
                                   color.green(),
                                   color.blue(),
                                   alpha);
    if(pMallocVX) 
		fastFree(pMallocVX);
	
	if(pMallocVY)
		fastFree(pMallocVY);

}


void GraphicsContext::fillRect(const IntRect& rectWK, const Color& color, bool solidFill)
{
    if (paintingDisabled())
        return;

    const IntSize&   o = origin();
    EA::Raster::Rect rect(rectWK.x() + o.width(), rectWK.y() + o.height(), rectWK.width(), rectWK.height());

    EA::Raster::ISurface* const pSurface = m_data->surface;
    EA::Raster::IEARaster* pRaster = EA::WebKit::GetEARasterInstance();

    EA::Raster::Color c(color.red(), color.green(), color.blue(), color.alpha());
    if( (!solidFill) &&  (color.alpha()) )
    {
        // To do: What really want to do is modify the alpha of 'color' in place instead of create temporary.
        // 12/8/10 CSidhall - Replaced the alpha stroke by the color alpha to fix background color fills with alpha and text highlight bug.       
        // Note: Still remains an issue with overlapping child nodes with a same alpha which should not be darker.
        // const int            alpha = m_data->layers.isEmpty() ? strokeColor().alpha() : static_cast<int>(strokeColor().alpha() * m_data->layers.last());
        const int            alpha = m_data->layers.isEmpty() ? color.alpha() : static_cast<int>(color.alpha() * m_data->layers.last());    
        c.setAlpha(alpha);
    }

    // 12/18/10 CSidhall - Added transform support for fill.
    if(hasTransform())
    {
        AffineTransform t = getCTM();
        double x[4];
        double y[4];
        double x1 = double (rect.x - origin().width());       
        double y1 = double (rect.y - origin().height());       
        
        t.map( x1, y1, &x[0],&y[0]);
        t.map((x1 + (double) rect.w), (y1 + (double) rect.h), &x[1],&y[1]);
        t.map((x1 + (double) rect.w), y1, &x[2],&y[2]);
        t.map( x1,  (y1 + (double) rect.h), &x[3],&y[3]);

        int orgW = origin().width();
        int orgH = origin().height();
        int vx[3] ={ (int) x[0] + orgW ,(int) x[2] + orgW,(int) x[3] + orgW}; 
        int vy[3] ={ (int) y[0] + orgH,(int) y[2] + orgH,(int) y[3] + orgH}; 
            
        pRaster->FilledPolygonColor( pSurface, &vx[0], &vy[0], 3 ,c);

        vx[0] = (int) x[1] + orgW;
        vy[0] = (int) y[1] + orgH;
        pRaster->FilledPolygonColor( pSurface, &vx[0], &vy[0], 3 ,c);
    }
    else if(solidFill)
    {
        pRaster->FillRectSolidColor(pSurface, &rect, c);
    }
    else
    {    
        pRaster->FillRectColor(pSurface, &rect, c);
    }
}


void GraphicsContext::fillRect(const FloatRect& rect, const Color& color)
{
    fillRect(IntRect(rect), color);
}


void GraphicsContext::clip(const FloatRect& rect)
{
    if (paintingDisabled())
        return;

    if (rect.isEmpty())
        m_data->surface->SetClipRect(NULL);
    else
    {
        float x = rect.x() + origin().width();
        x = (x > 0) ? x : 0;

        float y = rect.y() + origin().height();
        y = (y > 0) ? y : 0;

        const EA::Raster::Rect dstRect((int)x, (int)y, (int)rect.width(), (int)rect.height());
        m_data->surface->SetClipRect(&dstRect);
    }
}


// 3/13/09 CSidhall - Added to get easy access to the current clip rectangle in screen space
FloatRect GraphicsContext::getClip() const
{
     FloatRect rect;

    // Note: seems m_data might be decrepated eventually
    if((m_data) && (m_data->surface))
    {
        rect.setX( (float) m_data->surface->GetClipRect().x);
        rect.setY((float) m_data->surface->GetClipRect().y);
        rect.setWidth(m_data->surface->GetClipRect().width());
        rect.setHeight(m_data->surface->GetClipRect().height());
    }
    return rect;
}




// It's called a "focus ring" but it's not a ring, it's a (possibly rounded) rect around 
// something that is highlighted, like a text input.
void GraphicsContext::drawFocusRing(const Color& color)
{
    if (paintingDisabled())
        return;

    const int radius = (focusRingWidth() - 1) / 2;
    const int offset = radius + focusRingOffset();

    const Vector<IntRect>& rects     = focusRingRects();
    unsigned               rectCount = rects.size();
    IntRect                finalFocusRect;

    for (unsigned i = 0; i < rectCount; i++)
    {
        IntRect focusRect = rects[i];

        focusRect.setLocation(focusRect.location() + origin());
        focusRect.inflate(offset);
        finalFocusRect.unite(focusRect);
    }

    bool useDefaultFocusRingDraw = true;
    EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();

    EA::WebKit::FocusRingDrawInfo focusInfo;
    
    EA::Raster::ISurface* pSurface = m_data->surface; 
    EA::WebKit::View* pView = NULL;
    if(pSurface)
        pView = static_cast<EA::WebKit::View*> (pSurface->GetUserData());  // Need to verify that this cast is always safe...
    
    focusInfo.mpView = pView;
    EA::WebKit::GetEARasterInstance()->IntRectToEARect(finalFocusRect, focusInfo.mFocusRect);
    focusInfo.mFocusRect.h -= 1;
    focusInfo.mFocusRect.w -= 1;
    focusInfo.mSuggestedColor.setRGB(color.rgb());
    focusInfo.mpSurface = m_data->surface;
    if(pVN)
    {
        useDefaultFocusRingDraw = !pVN->DrawFocusRing(focusInfo);
    }

    //default focus rect draw method
    if(useDefaultFocusRingDraw)
    {
        // Force the alpha to 50%. This matches what the Mac does with outline rings.
        const  EA::Raster::Color ringColor(color.red(), color.green(), color.blue(), 127);
        EA::WebKit::GetEARasterInstance()->RectangleColor(focusInfo.mpSurface, focusInfo.mFocusRect, ringColor);
    }
}


void GraphicsContext::drawLineForText(const IntPoint& startPoint, int width, bool printing)
{
    if (paintingDisabled())
        return;

    // 2/12//09 CSidhall - Removed orgin offset from startPoint for is already added in by drawLine()
    IntPoint point(startPoint);

    IntPoint endPoint = point + IntSize(width, 0);

    // NOTE we should adjust line to pixel boundaries
    drawLine(point, endPoint);
}

void GraphicsContext::drawLineForMisspellingOrBadGrammar(const IntPoint& origin, int width, bool grammar)
{
    NotImplemented();
}

FloatRect GraphicsContext::roundToDevicePixels(const FloatRect& frect)
{
    return frect;
}

void GraphicsContext::translate(float x, float y)
{
    if (paintingDisabled())
        return;
    m_common->state.origin += IntSize(static_cast<int>(x), static_cast<int>(y));
}

IntSize GraphicsContext::origin()
{
    return m_common->state.origin;
}

void GraphicsContext::setPlatformFillColor(const Color& col)
{
    // FIXME: this is probably a no-op but I'm not sure
    // notImplemented(); // commented-out because it's chatty and clutters output
    m_common->state.fillColor = col;
}

void GraphicsContext::setPlatformStrokeColor(const Color& col)
{
    // FIXME: this is probably a no-op but I'm not sure
    //notImplemented(); // commented-out because it's chatty and clutters output
    m_common->state.strokeColor = col;
}

void GraphicsContext::setPlatformStrokeThickness(float strokeThickness)
{
    m_common->state.strokeThickness = strokeThickness;
}

void GraphicsContext::setPlatformStrokeStyle(const StrokeStyle& strokeStyle)
{
    m_common->state.strokeStyle = strokeStyle;
}

void GraphicsContext::setURLForRect(const KURL& link, const IntRect& destRect)
{
    notImplemented();
}

void GraphicsContext::concatCTM(const WKAL::AffineTransform& transform)
{
    m_common->state.transform *= transform;
}

void GraphicsContext::addInnerRoundedRectClip(const IntRect& rect, int thickness)
{
    NotImplemented();
}

void GraphicsContext::clipToImageBuffer(const FloatRect& rect, const ImageBuffer* imageBuffer)
{
    if (paintingDisabled())
        return;

    notImplemented();
}

void GraphicsContext::setPlatformShadow(IntSize const&, int, Color const&)
{
    notImplemented();
}

void GraphicsContext::clearPlatformShadow()
{
    notImplemented();
}

void GraphicsContext::beginTransparencyLayer(float opacity)
{
    if (paintingDisabled())
        return;
    m_data->layers.append(opacity);
    m_data->beginTransparencyLayer();
}

void GraphicsContext::endTransparencyLayer()
{
    if (paintingDisabled())
        return;

    m_data->layers.removeLast();
    m_data->endTransparencyLayer();
}

void GraphicsContext::clearRect(const FloatRect& rect, bool solidFill)
{
    if (paintingDisabled())
        return;

    IntRect rectangle(rect);

    // 7/22/09 CSidhall - the fillRect also adds the origin so we should not added it here...
    //    rectangle.setLocation(rectangle.location() + origin());
    fillRect(rectangle, Color::white, solidFill);
}

void GraphicsContext::strokeRect(const FloatRect& rect, float width)
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::setLineCap(LineCap lineCap)
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::setLineJoin(LineJoin lineJoin)
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::setMiterLimit(float miter)
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::setAlpha(float)
{
    notImplemented();
}

void GraphicsContext::setCompositeOperation(CompositeOperator op)
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::beginPath()
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::addPath(const Path& path)
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::clip(const Path& path)
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::clipOut(const Path& path)
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::rotate(float radians)
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::scale(const FloatSize& size)
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::clipOut(const IntRect& r)
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::clipOutEllipseInRect(const IntRect& r)
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::fillRoundedRect(const IntRect& r, const IntSize& topLeft, const IntSize& topRight, const IntSize& bottomLeft, const IntSize& bottomRight, const Color& color)
{
    if (paintingDisabled())
        return;

    NotImplemented();
}

void GraphicsContext::setBalExposeEvent(BalEventExpose* expose)
{
    m_expose = expose;
}

BalEventExpose* GraphicsContext::balExposeEvent() const
{
    return m_expose;
}

BalDrawable* GraphicsContext::balDrawable() const
{
    //if (!m_data->expose)
    //    return 0;

    return 0;//GDK_DRAWABLE(m_data->expose->window);
}

float GraphicsContext::transparencyLayer()
{
    if (!m_data->layers.isEmpty())
        return m_data->layers.last();
    else
        return 1.0;
}

IntPoint GraphicsContext::translatePoint(const IntPoint& point) const
{
    NotImplemented();
    return point;
}

void GraphicsContext::setUseAntialiasing(bool enable)
{
    if (paintingDisabled())
        return;

    // When true, use the default Cairo backend antialias mode (usually this
    // enables standard 'grayscale' antialiasing); false to explicitly disable
    // antialiasing. This is the same strategy as used in drawConvexPolygon().
    //cairo_set_antialias(m_data->cr, enable ? CAIRO_ANTIALIAS_DEFAULT : CAIRO_ANTIALIAS_NONE);
}

} // namespace WebCore

